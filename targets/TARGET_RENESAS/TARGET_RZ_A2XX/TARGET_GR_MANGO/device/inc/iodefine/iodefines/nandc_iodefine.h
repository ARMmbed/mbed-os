/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Copyright (c) 2019-2020 Renesas Electronics Corporation.
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
/*******************************************************************************
* Rev: 3.01
* Description : IO define header
*******************************************************************************/

#ifndef NANDC_IODEFINE_H
#define NANDC_IODEFINE_H

struct st_nandc {
    union {
        unsigned long LONG;
        struct {
            unsigned long CMD_SEQ: 6;
            unsigned long INPUT_SEL: 1;
            unsigned long DATA_SEL: 1;
            unsigned long CMD_0: 8;
            unsigned long CMD_1: 8;
            unsigned long CMD_2: 8;
        } BIT;
    } COMMAND;
    union {
        unsigned long LONG;
        struct {
            unsigned long READ_STATUS_EN: 1;
            unsigned long ECC_BLOCK_SIZE: 2;
            unsigned long : 1;
            unsigned long INT_EN: 1;
            unsigned long ECC_EN: 1;
            unsigned long BLOCK_SIZE: 2;
            unsigned long : 4;
            unsigned long : 1;
            unsigned long BBM_EN: 1;
            unsigned long PROT_EN: 1;
            unsigned long : 1;
            unsigned long ADDR0_AUTO_INCR: 1;
            unsigned long ADDR1_AUTO_INCR: 1;
            unsigned long : 3;
            unsigned long SMALL_BLOCK_EN: 1;
            unsigned long MLUN_EN: 1;
            unsigned long AUTO_READ_STAT_EN: 1;
            unsigned long : 8;
        } BIT;
    } CONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long MEM0_ST: 1;
            unsigned long : 7;
            unsigned long CTRL_STAT: 1;
            unsigned long DATASIZE_ERROR_ST: 1;
            unsigned long DATA_REG_ST: 1;
            unsigned long : 5;
            unsigned long CMD_ID: 8;
            unsigned long : 8;
        } BIT;
    } STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long STATE_MASK: 8;
            unsigned long ERROR_MASK: 8;
            unsigned long : 16;
        } BIT;
    } STATUS_MASK;
    union {
        unsigned long LONG;
        struct {
            unsigned long PROT_INT_EN: 1;
            unsigned long CMD_END_INT_EN: 1;
            unsigned long DATA_REG_INT_EN: 1;
            unsigned long DMA_INT_EN: 1;
            unsigned long TRANS_ERR_EN: 1;
            unsigned long : 1;
            unsigned long PG_SZ_ERR_INT_EN: 1;
            unsigned long : 1;
            unsigned long MEM0_RDY_INT_EN: 1;
            unsigned long : 7;
            unsigned long STAT_ERR_INT0_EN: 1;
            unsigned long : 7;
            unsigned long ECC_INT0_EN: 1;
            unsigned long : 7;
        } BIT;
    } INT_MASK;
    union {
        unsigned long LONG;
        struct {
            unsigned long PROT_INT_FL: 1;
            unsigned long CMD_END_INT_FL: 1;
            unsigned long DATA_REG_INT_FL: 1;
            unsigned long DMA_INT_FL: 1;
            unsigned long TRANS_ERR_FL: 1;
            unsigned long : 1;
            unsigned long PG_SZ_ERR_INT_FL: 1;
            unsigned long : 1;
            unsigned long MEM0_RDY_INT_FL: 1;
            unsigned long : 7;
            unsigned long STAT_ERR_INT0_FL: 1;
            unsigned long : 7;
            unsigned long ECC_INT0_FL: 1;
            unsigned long : 7;
        } BIT;
    } INT_STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECC_CAP: 3;
            unsigned long : 5;
            unsigned long ERR_THRESHOLD: 6;
            unsigned long : 2;
            unsigned long ECC_SEL: 2;
            unsigned long : 14;
        } BIT;
    } ECC_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECC_OFFSET: 16;
            unsigned long : 16;
        } BIT;
    } ECC_OFFSET;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECC_ERROR_0: 1;
            unsigned long : 7;
            unsigned long ECC_UNC_0: 1;
            unsigned long : 7;
            unsigned long ECC_OVER_0: 1;
            unsigned long : 15;
        } BIT;
    } ECC_STAT;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADDR0_COL: 16;
            unsigned long : 16;
        } BIT;
    } ADDR0_COL;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADDR0_ROW: 24;
            unsigned long : 8;
        } BIT;
    } ADDR0_ROW;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADDR1_COL: 16;
            unsigned long : 16;
        } BIT;
    } ADDR1_COL;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADDR1_ROW: 24;
            unsigned long : 8;
        } BIT;
    } ADDR1_ROW;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long FIFO_DATA: 32;
        } BIT;
    } FIFO_DATA;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATA_REG: 32;
        } BIT;
    } DATA_REG;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATA_REG_SIZE: 2;
            unsigned long : 30;
        } BIT;
    } DATA_REG_SIZE;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 2;
            unsigned long PTR_ADDR: 10;
            unsigned long : 20;
        } BIT;
    } DEV0_PTR;
    char           wk1[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long DMA_ADDR_L: 32;
        } BIT;
    } DMA_ADDR_L;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CNT_INIT: 32;
        } BIT;
    } DMA_CNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long DMA_READY: 1;
            unsigned long : 1;
            unsigned long DMA_BURST: 3;
            unsigned long DMA_MODE: 1;
            unsigned long : 1;
            unsigned long DMA_START: 1;
            unsigned long : 24;
        } BIT;
    } DMA_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long RMP_INIT: 1;
            unsigned long : 31;
        } BIT;
    } BBM_CTRL;
    char           wk3[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long DATA_SIZE: 15;
            unsigned long : 17;
        } BIT;
    } DATA_SIZE;
    union {
        unsigned long LONG;
        struct {
            unsigned long TRWP: 4;
            unsigned long TRWH: 4;
            unsigned long : 24;
        } BIT;
    } TIMINGS_ASYN;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TCCS: 6;
            unsigned long : 2;
            unsigned long TADL: 6;
            unsigned long : 2;
            unsigned long TRHW: 6;
            unsigned long : 2;
            unsigned long TWHR: 6;
            unsigned long : 2;
        } BIT;
    } TIME_SEQ_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TWB: 6;
            unsigned long : 2;
            unsigned long TRR: 6;
            unsigned long : 18;
        } BIT;
    } TIME_SEQ_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long t0_d0: 6;
            unsigned long : 2;
            unsigned long t0_d1: 6;
            unsigned long : 2;
            unsigned long t0_d2: 6;
            unsigned long : 2;
            unsigned long t0_d3: 6;
            unsigned long : 2;
        } BIT;
    } TIME_GEN_SEQ_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long t0_d4: 6;
            unsigned long : 2;
            unsigned long t0_d5: 6;
            unsigned long : 2;
            unsigned long t0_d6: 6;
            unsigned long : 2;
            unsigned long t0_d7: 6;
            unsigned long : 2;
        } BIT;
    } TIME_GEN_SEQ_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long t0_d8: 6;
            unsigned long : 2;
            unsigned long t0_d9: 6;
            unsigned long : 2;
            unsigned long t0_d10: 6;
            unsigned long : 2;
            unsigned long t0_d11: 6;
            unsigned long : 2;
        } BIT;
    } TIME_GEN_SEQ_2;
    char           wk5[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long FIFO_INIT: 1;
            unsigned long : 31;
        } BIT;
    } FIFO_INIT;
    union {
        unsigned long LONG;
        struct {
            unsigned long DF_R_EMPTY: 1;
            unsigned long DF_W_FULL: 1;
            unsigned long CF_EMPTY: 1;
            unsigned long CF_FULL: 1;
            unsigned long CF_ACCPT_R: 1;
            unsigned long CF_ACCPT_W: 1;
            unsigned long DF_R_FULL: 1;
            unsigned long DF_W_EMPTY: 1;
            unsigned long : 24;
        } BIT;
    } FIFO_STATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long CMD0_EN: 1;
            unsigned long CMD1_EN: 1;
            unsigned long CMD2_EN: 1;
            unsigned long CMD3_EN: 1;
            unsigned long COL_A0: 2;
            unsigned long COL_A1: 2;
            unsigned long ROW_A0: 2;
            unsigned long ROW_A1: 2;
            unsigned long DATA_EN: 1;
            unsigned long DELAY_EN: 2;
            unsigned long IMD_SEQ: 1;
            unsigned long CMD_3: 8;
            unsigned long : 8;
        } BIT;
    } GEN_SEQ_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long MLUN_IDX: 3;
            unsigned long : 5;
            unsigned long LUN_SEL: 2;
            unsigned long : 22;
        } BIT;
    } MLUN;
    union {
        unsigned long LONG;
        struct {
            unsigned long DEV_SIZE: 12;
            unsigned long : 20;
        } BIT;
    } DEV0_SIZE;
    char           wk6[80];
    union {
        unsigned long LONG;
        struct {
            unsigned long DMA_TRIG_TLVL: 8;
            unsigned long : 24;
        } BIT;
    } DMA_TRIG_TLVL;
    char           wk7[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long CMD_ID: 8;
            unsigned long : 24;
        } BIT;
    } CMD_MARK;
    union {
        unsigned long LONG;
        struct {
            unsigned long MEM0_LUN: 8;
            unsigned long : 24;
        } BIT;
    } LUN_STATUS0;
    char           wk8[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long t0_d12: 6;
            unsigned long : 26;
        } BIT;
    } TIME_GEN_SEQ_3;
    char           wk9[20];
    union {
        unsigned long LONG;
        struct {
            unsigned long ERR_LVL: 6;
            unsigned long : 26;
        } BIT;
    } ECC_CNT;
};

#define NANDC     (*(volatile struct st_nandc     *)0xE822C000)

#endif
