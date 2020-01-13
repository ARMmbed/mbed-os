/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S5SJ100_DCXO_H__
#define __S5SJ100_DCXO_H__

// default option
#define DCXO_FINE_PMU_COARSE_DCXO           1
//#define DCXO_FINE_PMU_COARSE_PMU          0
//#define DCXO_FINE_DCXO_COARSE_PMU         0


// SFR base
#define S5JS100_EFUSE       0x82010000
#define S5JS100_DCXO_CFG    0x87000000
#define S5JS100_PMU_ALIVE   0x81000000

// SFR offset (DCXO)
#define DCXO_CFG_SW_RESETN      0x0
#define DCXO_CFG_DIV_CFG        0x4
#define DCXO_CFG_DSM_CFG0       0x8
#define DCXO_CFG_DSM_CFG1       0xC
#define DCXO_CFG_AVR_CFG        0x10
#define DCXO_CFG_IRQ_CFG0       0x14
#define DCXO_CFG_IRQ_CFG1       0x18
#define DCXO_CFG_IRQ_STA0       0x1C
#define DCXO_CFG_IRQ_STA1       0x20
#define DCXO_CFG_ADC_CFG        0x24

// SFR offset (PMU_Alive)
#define DCXO_Cfg0                      0x520
#define DCXO_Cfg1                      0x524
#define DCXO_Cfg2                      0x528
#define DCXO_Cfg3                      0x52C


/* DCXO FRAME MSG TYPE */
#define DCXO_CTB_FULLSET_UPDATE     0x1
#define DCXO_TEMP_REQUEST           0x2
#define DCXO_CTB_FORCE_REQUEST_FOR_FACTORY_CAL      0x3
#define DCXO_GET_CTB_REQUEST        0x4
#define DCXO_GET_TSX_INFO_REQUEST   0x5

#define DCXO_RESPONSE_ACK       0xDC10
#define DCXO_RESPONSE_NCK       0xDEAD

#define DCXO_TEMPERATURE_RANGE  126 /* -40 ~ 85 */

#if defined(CONFIG_S5JS100_TSX_KDS)
#define S5JS100_TSX_TYPE    0x1
#elif defined(CONFIG_S5JS100_TSX_NDK)
#define S5JS100_TSX_TYPE    0x2
#elif defined(CONFIG_S5JS100_TSX_KYOCERA)
#define S5JS100_TSX_TYPE    0x3
#endif

#ifndef S5JS100_TSX_TYPE
#define S5JS100_TSX_TYPE    0x1
#endif


typedef struct {
    unsigned short ctb;
    unsigned short reserved;
} Full_set_table_t;

typedef struct {
    unsigned int msgType;
    int response;
    int response1;
} UpdateInd_header_t;

typedef struct {
    unsigned int resolution;
    int start_temp;
    unsigned int length;
    Full_set_table_t setTable[DCXO_TEMPERATURE_RANGE];
} UpdateInd_payload_t;

typedef struct {
    UpdateInd_header_t header;
    UpdateInd_payload_t payload;
} Nl1cHiu_UpdateInd_t;

extern int s5js100_dcxo_start_ctb_loop(Full_set_table_t tbl[]);
extern int s5js100_dcxo_stop_ctb_loop(void);
extern int s5js100_dcxo_set_tune_value(unsigned int coarse_value, unsigned int fine_value);
extern int s5js100_tsu_get_temperature(void);
extern int s5js100_dcxo_get_coarse_tune_value(void);

extern void dcxo_ctb_sem_wait(void);
extern void dcxo_ctb_sem_release(void);

extern "C" {
    int s5js100_dcxo_initialize(void);
}

#endif                          /* __S5SJ100_DCXO_H__ */
