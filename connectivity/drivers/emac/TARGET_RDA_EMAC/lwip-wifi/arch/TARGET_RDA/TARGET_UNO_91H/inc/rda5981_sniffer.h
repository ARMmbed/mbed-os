/* Copyright (c) 2019 Unisoc Communications Inc.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _RDA5981_SNIFFER_H_
#define _RDA5981_SNIFFER_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "wland_types.h"

/* Enable filtering ACK frames (no support)*/
//#define RDA_RX_FILTER_DROP_ACK                  BIT0

/* Enable filtering CTS frames (no support)*/
//#define RDA_RX_FILTER_DROP_CTS                  BIT1

/* Enable filtering RTS frames (no support)*/
//#define RDA_RX_FILTER_DROP_RTS                  BIT2

/* Enable filtering beacon frames */
#define RDA_RX_FILTER_DROP_BEACON               BIT3

/* Enable filtering ATIM frames (no support)*/
//#define RDA_RX_FILTER_DROP_ATIM                 BIT4

/* Enable filtering CF_END frames (no support)*/
//#define RDA_RX_FILTER_DROP_CF_END               BIT5

/* Enable filtering QCF_POLL frames (no support)*/
//#define RDA_RX_FILTER_DROP_QCF_POLL             BIT6

/* Filter Management frames which are not directed to current STA */
#define RDA_RX_FILTER_DROP_ND_MGMT              BIT7

/* Filter BC/MC MGMT frames belonging to other BSS */
#define RDA_RX_FILTER_DROP_BC_MC_MGMT_OTHER_BSS BIT8

/* Enable filtering of duplicate frames */
#define RDA_RX_FILTER_DROP_DUPLICATE            BIT9

/* Enable filtering of frames whose FCS has failed */
#define RDA_RX_FILTER_DROP_FCS_FAILED           BIT10

/* Enable filtering of De-authentication frame */
#define RDA_RX_FILTER_DROP_DEAUTH               BIT11

/* Filter BA frames which are not received as SIFS response (no support)*/
//#define RDA_RX_FILTER_DROP_NSIFS_RESP_BA        BIT12

/* Filter BA frames which are received as SIFS response (no support)*/
//#define RDA_RX_FILTER_DROP_SIFS_RESP_BA         BIT13

/* Filter frames which are received in secondary channel (20 MHz PPDU from Secondary channel) */
#define RDA_RX_FILTER_DROP_SEC_CHANNEL          BIT14

/* Filter BC/MC DATA frames belonging to other BSS */
#define RDA_RX_FILTER_DROP_BC_MC_DATA_OTHER_BSS BIT15

/* Filter DATA frames not directed to this station */
#define RDA_RX_FILTER_DROP_ND_DATA              BIT16

/* Filter Control frames which are not directed to current STA (no support)*/
//#define RDA_RX_FILTER_DROP_ND_CONTROL           BIT17

/* Filter Beacon frames (in IBSS mode) which are not used for adoption because the timestamp field is lower than TSF timer */
#define RDA_RX_FILTER_DROP_IBSS_BEACON          BIT18

typedef int (*sniffer_handler_t)(unsigned short data_len, void *data);

int rda5981_enable_sniffer(sniffer_handler_t handler);
int rda5981_disable_sniffer(void);
//don't use this in sniffer callback handler
int rda5981_disable_sniffer_nocallback(void);
///TODO: time is no use anymore
int rda5981_start_sniffer(unsigned char channel, unsigned char to_ds,
    unsigned char from_ds, unsigned char mgm_frame, unsigned short time);
int rda5981_stop_sniffer(void);
int wland_sniffer_set_channel(unsigned char channel);
int rda5981_set_filter(unsigned char to_ds, unsigned char from_ds, unsigned int mgm_filter);
int rda5981_sniffer_enable_fcs(void);//for hiflying
#ifdef __cplusplus
}
#endif

#endif /*_RDA5981_SNIFFER_H_*/
