/**
 * \file config.h
 *
 * \brief WSNDemo application and stack configuration
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
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
 * $Id: config.h 7863 2013-05-13 20:14:34Z ataradov $
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BI_COEF												(8)
#define SD_COEF												(6)
#define FINAL_CAP_SLOT										(1)
#define SYMBOL_TIME											0.000016						// s
#define ABASEFRAMEDURATION									(960ul)							// Symbols
#define BI_EXPO												(256)
#define SD_EXPO												(50)
#define BEACON_INTERVAL_BI									(ABASEFRAMEDURATION * BI_EXPO)	// 4s
#define SUPERFRAME_DURATION_SD								(ABASEFRAMEDURATION * SD_EXPO)	// 0.768s
#define TDMA_SLOT_PERIOD									0.05							// s
#define TDMA_FIRST_SLOT										(3125)							// Symbols
#define TDMA_BATTERY_EXTENSION								1

#define PHY_ENABLE_RANDOM_NUMBER_GENERATOR
#define PHY_ENABLE_ENERGY_DETECTION
#define PHY_ENABLE_AES_MODULE

#define SYS_SECURITY_MODE                   0

#define NWK_BUFFERS_AMOUNT                  20
#define NWK_DUPLICATE_REJECTION_TABLE_SIZE  50
#define NWK_DUPLICATE_REJECTION_TTL         2000 // ms
#define NWK_ROUTE_TABLE_SIZE                100
#define NWK_ROUTE_DEFAULT_SCORE             3
#define NWK_ACK_WAIT_TIME                   1000 // ms
#define NWK_GROUPS_AMOUNT                   3
#define NWK_ROUTE_DISCOVERY_TABLE_SIZE      5
#define NWK_ROUTE_DISCOVERY_TIMEOUT         1000 // ms

#define NWK_ENABLE_ROUTING
//#define NWK_ENABLE_SECURITY
#define NWK_ENABLE_MULTICAST
#define NWK_ENABLE_ROUTE_DISCOVERY
//#define NWK_ENABLE_SECURE_COMMANDS

#ifdef __cplusplus
}
#endif

#endif // _CONFIG_H_
