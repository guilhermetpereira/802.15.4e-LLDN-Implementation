/**
 * \file nwkTx.c
 *
 * \brief Transmit routines implementation
 *
 * Copyright (C) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *
 */

/*
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys\sysConfig.h>
#include <sys\sysTimer.h>
#include "phy.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkRoute.h"
#include "nwkCommand.h"
#include "nwkSecurity.h"

/*- Definitions ------------------------------------------------------------*/
#define NWK_TX_ACK_WAIT_TIMER_INTERVAL    50 /* ms */
#define NWK_TX_DELAY_TIMER_INTERVAL       10 /* ms */
#define NWK_TX_DELAY_JITTER_MASK          0x07

/*- Types ------------------------------------------------------------------*/
enum {
	NWK_TX_STATE_ENCRYPT    = 0x10,
	NWK_TX_STATE_WAIT_DELAY = 0x11,
	NWK_TX_STATE_DELAY      = 0x12,
	NWK_TX_STATE_SEND       = 0x13,
	NWK_TX_STATE_WAIT_CONF  = 0x14,
	NWK_TX_STATE_SENT       = 0x15,
	NWK_TX_STATE_WAIT_ACK   = 0x16,
	NWK_TX_STATE_CONFIRM    = 0x17,
};

/*- Prototypes -------------------------------------------------------------*/
static void nwkTxAckWaitTimerHandler(SYS_Timer_t *timer);
static void nwkTxDelayTimerHandler(SYS_Timer_t *timer);

/*- Variables --------------------------------------------------------------*/
static NwkFrame_t *nwkTxPhyActiveFrame; // Current frame being sent
static SYS_Timer_t nwkTxAckWaitTimer;
static SYS_Timer_t nwkTxDelayTimer;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*  @brief Initializes the Tx module
*****************************************************************************/
void nwkTxInit(void)
{
	nwkTxPhyActiveFrame = NULL;

	nwkTxAckWaitTimer.interval = NWK_TX_ACK_WAIT_TIMER_INTERVAL;
	nwkTxAckWaitTimer.mode = SYS_TIMER_INTERVAL_MODE;
	nwkTxAckWaitTimer.handler = nwkTxAckWaitTimerHandler;

	nwkTxDelayTimer.interval = NWK_TX_DELAY_TIMER_INTERVAL;
	nwkTxDelayTimer.mode = SYS_TIMER_INTERVAL_MODE;
	nwkTxDelayTimer.handler = nwkTxDelayTimerHandler;
}

/*************************************************************************//**
*****************************************************************************/
void nwkTxBeaconFrame(NwkFrame_t *frame)
{
	// when frame->data is used latter this is what it referes
	NwkFrameBeaconHeader_t *beacon = &frame->beacon; // occupies data memory space,

	frame->state = NWK_TX_STATE_SEND; // First state of nwkTxTaskHandler
	frame->tx.status = NWK_SUCCESS_STATUS;
	frame->tx.timeout = 0;

// changed size and bits to LL standart
	beacon->macFcf = 0x88; // 100 0 1 0 00 (LLDN, Security Enabled, Frame Version, ACK Request, Sub Frame Type)
	beacon->macSeq = ++nwkIb.macSeqNum; // Sequence Number: only present when Security Enabled = 1
	beacon->macSrcPanId = nwkIb.panId;
	// beacon->macSrcAddr = nwkIb.addr;
}
void nwkTxFrame(NwkFrame_t *frame)
{
	NwkFrameHeader_t *header = &frame->header;

	if (frame->tx.control & NWK_TX_CONTROL_ROUTING) {
		frame->state = NWK_TX_STATE_DELAY;
	} else {
  #ifdef NWK_ENABLE_SECURITY
		if (header->nwkFcf.security) {
			frame->state = NWK_TX_STATE_ENCRYPT;
		} else
  #endif
		frame->state = NWK_TX_STATE_DELAY;
	}

	frame->tx.status = NWK_SUCCESS_STATUS;

	if (frame->tx.control & NWK_TX_CONTROL_BROADCAST_PAN_ID) {
		header->macDstPanId = NWK_BROADCAST_PANID;
	} else {
		header->macDstPanId = nwkIb.panId;
	}

#ifdef NWK_ENABLE_ROUTING
	if (0 == (frame->tx.control & NWK_TX_CONTROL_DIRECT_LINK) &&
			0 ==
			(frame->tx.control & NWK_TX_CONTROL_BROADCAST_PAN_ID)) {
		nwkRoutePrepareTx(frame);
	} else
#endif
	header->macDstAddr = header->nwkDstAddr;

	header->macSrcAddr = nwkIb.addr;
	header->macSeq = ++nwkIb.macSeqNum;

	if (NWK_BROADCAST_ADDR == header->macDstAddr) { // if message is a broadcast
		header->macFcf = 0x8841;
		frame->tx.timeout = (rand() & NWK_TX_DELAY_JITTER_MASK) + 1; // set a random timer to start afterwards
	} else {
		header->macFcf = 0x8861;
		frame->tx.timeout = 0; // if it isn't a timer timer is set to zero
	}
}

/*************************************************************************//**
*****************************************************************************/
void nwkTxBroadcastFrame(NwkFrame_t *frame)
{
	NwkFrame_t *newFrame;

	if (NULL == (newFrame = nwkFrameAlloc())) {
		return;
	}

	newFrame->state = NWK_TX_STATE_DELAY;
	newFrame->size = frame->size;
	newFrame->tx.status = NWK_SUCCESS_STATUS;
	newFrame->tx.timeout = (rand() & NWK_TX_DELAY_JITTER_MASK) + 1;
	newFrame->tx.confirm = NULL;
	memcpy(newFrame->data, frame->data, frame->size);

	newFrame->header.macFcf = 0x8841;
	newFrame->header.macDstAddr = NWK_BROADCAST_ADDR;
	newFrame->header.macDstPanId = frame->header.macDstPanId;
	newFrame->header.macSrcAddr = nwkIb.addr;
	newFrame->header.macSeq = ++nwkIb.macSeqNum;
}

/*************************************************************************//**
*****************************************************************************/
bool nwkTxAckReceived(NWK_DataInd_t *ind)
{
	NwkCommandAck_t *command = (NwkCommandAck_t *)ind->data;
	NwkFrame_t *frame = NULL;

	if (sizeof(NwkCommandAck_t) != ind->size) {
		return false;
	}

	while (NULL != (frame = nwkFrameNext(frame))) {
		if (NWK_TX_STATE_WAIT_ACK == frame->state &&
				frame->header.nwkSeq == command->seq) {
			frame->state = NWK_TX_STATE_CONFIRM;
			frame->tx.control = command->control;
			return true;
		}
	}

	return false;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkTxAckWaitTimerHandler(SYS_Timer_t *timer)
{
	NwkFrame_t *frame = NULL;
	bool restart = false;

	while (NULL != (frame = nwkFrameNext(frame))) {
		if (NWK_TX_STATE_WAIT_ACK == frame->state) {
			restart = true;

			if (0 == --frame->tx.timeout) {
				nwkTxConfirm(frame, NWK_NO_ACK_STATUS);
			}
		}
	}

	if (restart) {
		SYS_TimerStart(timer);
	}
}

/*************************************************************************//**
*****************************************************************************/
void nwkTxConfirm(NwkFrame_t *frame, uint8_t status)
{
	frame->state = NWK_TX_STATE_CONFIRM;
	frame->tx.status = status;
}

#ifdef NWK_ENABLE_SECURITY

/*************************************************************************//**
*****************************************************************************/
void nwkTxEncryptConf(NwkFrame_t *frame)
{
	frame->state = NWK_TX_STATE_DELAY;
}

#endif

/*************************************************************************//**
*****************************************************************************/
static void nwkTxDelayTimerHandler(SYS_Timer_t *timer)
{
	NwkFrame_t *frame = NULL;
	bool restart = false;

	while (NULL != (frame = nwkFrameNext(frame))) {
		if (NWK_TX_STATE_WAIT_DELAY == frame->state) {
			restart = true;

			if (0 == --frame->tx.timeout) {
				frame->state = NWK_TX_STATE_SEND;
			}
		}
	}

	if (restart) {
		SYS_TimerStart(timer);
	}
}

/*************************************************************************//**
*****************************************************************************/
static uint8_t nwkTxConvertPhyStatus(uint8_t status)
{
	switch (status) {
	case PHY_STATUS_SUCCESS:
		return NWK_SUCCESS_STATUS;

	case PHY_STATUS_CHANNEL_ACCESS_FAILURE:
		return NWK_PHY_CHANNEL_ACCESS_FAILURE_STATUS;

	case PHY_STATUS_NO_ACK:
		return NWK_PHY_NO_ACK_STATUS;

	default:
		return NWK_ERROR_STATUS;
	}
}

/*************************************************************************//**
*****************************************************************************/
void PHY_DataConf(uint8_t status) // this function is called in PHY_TaskHandler when frame is sent
{
	nwkTxPhyActiveFrame->tx.status = nwkTxConvertPhyStatus(status);
	nwkTxPhyActiveFrame->state = NWK_TX_STATE_SENT;
	nwkTxPhyActiveFrame = NULL;
	nwkIb.lock--;
}

/*************************************************************************//**
*  @brief Tx Module task handler
*****************************************************************************/
void nwkTxTaskHandler(void)
{
	NwkFrame_t *frame = NULL;

	while (NULL != (frame = nwkFrameNext(frame))) { // gets next frame (if frame == NULL return first)
		switch (frame->state) {
#ifdef NWK_ENABLE_SECURITY
		case NWK_TX_STATE_ENCRYPT:
		{
			nwkSecurityProcess(frame, true);
		}
		break;
#endif

		case NWK_TX_STATE_DELAY: // first State
		{
			if (frame->tx.timeout > 0) {
				frame->state = NWK_TX_STATE_WAIT_DELAY;
				SYS_TimerStart(&nwkTxDelayTimer);
			} else {
				frame->state = NWK_TX_STATE_SEND;
			}
		}
		break;

		case NWK_TX_STATE_SEND: // Second State
		{
			if (NULL == nwkTxPhyActiveFrame) { // if no other frame is being sent
				nwkTxPhyActiveFrame = frame; // set this to be sent
				frame->state = NWK_TX_STATE_WAIT_CONF;
				PHY_DataReq(frame->data, frame->size); // It's up to PHY_TaskHandler to send frame
				nwkIb.lock++;
			}
		}
		break;

		case NWK_TX_STATE_WAIT_CONF: // waits for phy layer confirmation
			break;

		case NWK_TX_STATE_SENT:
		{
			if (NWK_SUCCESS_STATUS == frame->tx.status) {
				if (frame->header.nwkSrcAddr == nwkIb.addr &&
						frame->header.nwkFcf.
						ackRequest) { // if NWK_OPT_ACK_REQUEST is enabled start timer to wait
					frame->state = NWK_TX_STATE_WAIT_ACK;
					frame->tx.timeout = NWK_ACK_WAIT_TIME /
							NWK_TX_ACK_WAIT_TIMER_INTERVAL
							+ 1;
					SYS_TimerStart(&nwkTxAckWaitTimer);
				} else { // if NWK_OPT_ACK_REQUEST is not enabled change state
					frame->state = NWK_TX_STATE_CONFIRM;
				}
			} else {
				frame->state = NWK_TX_STATE_CONFIRM;
			}
		}
		break;

		case NWK_TX_STATE_WAIT_ACK:
			break;

		case NWK_TX_STATE_CONFIRM:
		{
#ifdef NWK_ENABLE_ROUTING
			nwkRouteFrameSent(frame);
#endif
			if (NULL == frame->tx.confirm) { // no beacon have confirm
				nwkFrameFree(frame); // if no callback free frame from buffer
			} else {
				frame->tx.confirm(frame); // callback to when massage is sent
			}
		}
		break;

		default:
			break;
		}
	}
}
