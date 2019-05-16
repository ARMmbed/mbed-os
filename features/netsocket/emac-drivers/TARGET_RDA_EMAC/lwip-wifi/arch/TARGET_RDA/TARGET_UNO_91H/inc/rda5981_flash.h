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

#ifndef _RDA5981_FLASH_H_
#define _RDA5981_FLASH_H_

#include "wland_types.h"

/** This struct contains tx power parameter. */
typedef struct
{
    u8 b[14];
    u8 g[14];
    u8 n[14];
    u8 sum;
    u8 padding1;//alignment
} wland_tx_power_t;

/*
 *function: store reg and corresponding value related to test mode
 *@valid: 1 means data is valid
 *@flag: if biti(i=0~7) == 1, it means reg_val[i] is in use
 *@reg_val: store reg and value, reg_val[i][0] is reg, reg_val[i][1] ~ reg_val[i][14] is value
 */
typedef struct {
    u32 valid;
    u32 flag;
    u16 reg_val[8][2];
} wland_rf_t;

typedef struct {
    u32 valid;
    u32 flag;
    u16 reg_val[8][15];
} wland_rf_channels_t;

typedef struct {
    u32 valid;
    u32 flag;
    u32 reg_val[8][2];
} wland_phy_t;

typedef struct {
    u32 valid;
    u32 flag;
    u32 reg_val[8][15];
} wland_phy_channels_t;

/* if you add or delete any macros below, modify RDA5991H_USER_DATA_FLAG_UNINITIALIZED at the same time */
#define RDA5991H_USER_DATA_FLAG_MAC BIT0
#define RDA5991H_USER_DATA_FLAG_STA BIT1
#define RDA5991H_USER_DATA_FLAG_PMK BIT2
#define RDA5991H_USER_DATA_FLAG_IP BIT3
#define RDA5991H_USER_DATA_FLAG_PARTER_DATA_LEN BIT4
#define RDA5991H_USER_DATA_FLAG_TX_POWER BIT5
#define RDA5991H_USER_DATA_FLAG_XTAL_CAL BIT6
#define RDA5991H_USER_DATA_FLAG_TX_POWER_RF BIT7
#define RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_GN BIT8
#define RDA5991H_USER_DATA_FLAG_TX_POWER_PHY_B BIT9
#define RDA5991H_USER_DATA_FLAG_AP BIT10
#define RDA5991H_USER_DATA_FLAG_APNET BIT11
#define RDA5991H_USER_DATA_FLAG_DHCP BIT12
#define RDA5991H_USER_DATA_FLAG_UART BIT13
#define RDA5991H_USER_DATA_FLAG_RF BIT14
#define RDA5991H_USER_DATA_FLAG_RF_CHANNELS BIT15
#define RDA5991H_USER_DATA_FLAG_PHY BIT16
#define RDA5991H_USER_DATA_FLAG_PHY_CHANNELS BIT17
#define RDA5991H_USER_DATA_FLAG_TX_POWER_OFFSET BIT18

#define RDA5981_VBAT_CAL BIT0
#define RDA5981_GPADC0_CAL BIT1
#define RDA5981_GPADC1_CAL BIT2
#define RDA5981_PRODUCT_ID BIT3
#define RDA5981_POWER_CLASS BIT4

#ifdef __cplusplus
extern "C" {
#endif

/*
 * function: read VBAT Calibration value
 * y = k * x + b
 * @k: slope
 * @b: offset
 * return: 0:success, else:fail
 */
int rda5981_flash_read_vbat_cal(float *k, float *b);

#ifdef __cplusplus
}
#endif
#endif
