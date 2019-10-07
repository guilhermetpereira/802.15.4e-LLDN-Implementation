/**
 * \file nwk.h
 *
 * \brief Network layer public interface
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

#ifndef _NWK_H_
#define _NWK_H_

#ifdef __cplusplus
extern "C" {
#endif

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "sys\sysConfig.h"
#include "nwk\nwkRoute.h"
#include "nwk\nwkGroup.h"
#include "nwk\nwkSecurity.h"
#include "nwk\nwkDataReq.h"

/**
 * \ingroup group_lwmesh_nwk
 * @{
 */

/*- Definitions ------------------------------------------------------------*/
#define NWK_MAX_PAYLOAD_SIZE            (127 - 16 /*NwkFrameHeader_t*/ - \
	2 /*crc*/)

#define NWK_BROADCAST_PANID             0xffff
#define NWK_BROADCAST_ADDR              0xffff

#define NWK_ENDPOINTS_AMOUNT            16

#define NWK_NUMBER_OF_MGMT_TIMESLOTS    0b111

enum {
	NWK_OPT_ACK_REQUEST       = 1 << 0,
	NWK_OPT_ENABLE_SECURITY		= 1 << 1,
	NWK_OPT_BROADCAST_PAN_ID	= 1 << 2,
	NWK_OPT_LINK_LOCAL        = 1 << 3,
	NWK_OPT_MULTICAST         = 1 << 4,

	NWK_OPT_BEACON = 1 << 5,

	NWK_OPT_LLDN_BEACON					 	= 1 << 6,
	NWK_OPT_LLDN_BEACON_ONLINE		= 1 << 7,
	NWK_OPT_LLDN_BEACON_DISCOVERY = 1 << 8,
	NWK_OPT_LLDN_BEACON_CONFIG		= 1 << 9,
	NWK_OPT_LLDN_BEACON_RESET			= 1 << 10,
	NWK_OPT_LLDN_BEACON_SECOND		= 1 << 11,
	NWK_OPT_LLDN_BEACON_THIRD			= 1 << 12,

	NWK_OPT_LLDN_DATA 	= 1 << 13;
	NWK_OPT_LLDN_ACK 		= 1 << 14;
	NWK_OPT_MAC_COMMAND = 1 << 15;


};


/*- Types ------------------------------------------------------------------*/
typedef enum {
	NWK_SUCCESS_STATUS                      = 0x00,
	NWK_ERROR_STATUS                        = 0x01,
	NWK_OUT_OF_MEMORY_STATUS                = 0x02,

	NWK_NO_ACK_STATUS                       = 0x10,
	NWK_NO_ROUTE_STATUS                     = 0x11,

	NWK_PHY_CHANNEL_ACCESS_FAILURE_STATUS   = 0x20,
	NWK_PHY_NO_ACK_STATUS                   = 0x21,
} NWK_Status_t;

typedef struct NwkIb_t {
	uint16_t addr;
	uint16_t panId;
	uint8_t nwkSeqNum;
	uint8_t macSeqNum;
	bool(*endpoint[NWK_ENDPOINTS_AMOUNT]) (NWK_DataInd_t * ind);
#ifdef NWK_ENABLE_SECURITY
	uint32_t key[4];
#endif
	uint16_t lock;
} NwkIb_t;

/*- Variables --------------------------------------------------------------*/
extern NwkIb_t nwkIb;

/*- Prototypes -------------------------------------------------------------*/
void NWK_Init(void);
void NWK_SetAddr(uint16_t addr);
void NWK_SetPanId(uint16_t panId);
void NWK_OpenEndpoint(uint8_t id, bool (*handler)(NWK_DataInd_t *ind));
bool NWK_Busy(void);
void NWK_Lock(void);
void NWK_Unlock(void);
void NWK_SleepReq(void);
void NWK_WakeupReq(void);
void NWK_TaskHandler(void);

uint8_t NWK_LinearizeLqi(uint8_t lqi);

#ifdef __cplusplus
}
#endif

#endif /* _NWK_H_ */
