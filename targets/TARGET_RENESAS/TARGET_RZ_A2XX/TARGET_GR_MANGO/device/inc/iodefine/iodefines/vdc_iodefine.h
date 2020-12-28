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

#ifndef VDC_IODEFINE_H
#define VDC_IODEFINE_H

struct st_vdc {
    union {
        unsigned long LONG;
        struct {
            unsigned long INP_IMG_UPDATE: 1;
            unsigned long : 3;
            unsigned long INP_EXT_UPDATE: 1;
            unsigned long : 27;
        } BIT;
    } INP_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long INP_HS_EDGE: 1;
            unsigned long : 3;
            unsigned long INP_VS_EDGE: 1;
            unsigned long : 3;
            unsigned long INP_PXD_EDGE: 1;
            unsigned long : 3;
            unsigned long INP_FORMAT: 3;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long INP_SEL: 1;
            unsigned long : 11;
        } BIT;
    } INP_SEL_CNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long INP_H_POS: 2;
            unsigned long : 2;
            unsigned long INP_F525_625: 1;
            unsigned long : 3;
            unsigned long INP_H_EDGE_SEL: 1;
            unsigned long : 7;
            unsigned long INP_HS_INV: 1;
            unsigned long : 3;
            unsigned long INP_VS_INV: 1;
            unsigned long : 3;
            unsigned long INP_SWAP_ON: 1;
            unsigned long : 3;
            unsigned long INP_ENDIAN_ON: 1;
            unsigned long : 3;
        } BIT;
    } INP_EXT_SYNC_CNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long INP_FH25: 12;
            unsigned long : 4;
            unsigned long INP_FH50: 12;
            unsigned long : 4;
        } BIT;
    } INP_VSYNC_PH_ADJ;
    union {
        unsigned long LONG;
        struct {
            unsigned long INP_HS_DLY: 8;
            unsigned long INP_VS_DLY: 8;
            unsigned long INP_FLD_DLY: 8;
            unsigned long INP_VS_DLY_L: 3;
            unsigned long : 5;
        } BIT;
    } INP_DLY_ADJ;
    char           wk0[108];
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_VEN: 1;
            unsigned long : 31;
        } BIT;
    } IMGCNT_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long NR1D_Y_GAIN: 2;
            unsigned long : 2;
            unsigned long NR1D_Y_TAP: 2;
            unsigned long : 2;
            unsigned long NR1D_Y_TH: 7;
            unsigned long : 1;
            unsigned long NR1D_ON: 1;
            unsigned long : 3;
            unsigned long NR1D_MD: 1;
            unsigned long : 11;
        } BIT;
    } IMGCNT_NR_CNT0;
    union {
        unsigned long LONG;
        struct {
            unsigned long NR1D_CR_GAIN: 2;
            unsigned long : 2;
            unsigned long NR1D_CR_TAP: 2;
            unsigned long : 2;
            unsigned long NR1D_CR_TH: 7;
            unsigned long : 1;
            unsigned long NR1D_CB_GAIN: 2;
            unsigned long : 2;
            unsigned long NR1D_CB_TAP: 2;
            unsigned long : 2;
            unsigned long NR1D_CB_TH: 7;
            unsigned long : 1;
        } BIT;
    } IMGCNT_NR_CNT1;
    char           wk1[20];
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_MD: 2;
            unsigned long : 30;
        } BIT;
    } IMGCNT_MTX_MODE;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_GG: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_YG: 8;
            unsigned long : 8;
        } BIT;
    } IMGCNT_MTX_YG_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_GR: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_GB: 11;
            unsigned long : 5;
        } BIT;
    } IMGCNT_MTX_YG_ADJ1;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_BG: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_B: 8;
            unsigned long : 8;
        } BIT;
    } IMGCNT_MTX_CBB_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_BR: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_BB: 11;
            unsigned long : 5;
        } BIT;
    } IMGCNT_MTX_CBB_ADJ1;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_RG: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_R: 8;
            unsigned long : 8;
        } BIT;
    } IMGCNT_MTX_CRR_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long IMGCNT_MTX_RR: 11;
            unsigned long : 5;
            unsigned long IMGCNT_MTX_RB: 11;
            unsigned long : 5;
        } BIT;
    } IMGCNT_MTX_CRR_ADJ1;
    char           wk2[4];
    char           wk3[4];
    char           wk4[60];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_SCL0_VEN_A: 1;
            unsigned long : 3;
            unsigned long SC0_SCL0_VEN_B: 1;
            unsigned long : 3;
            unsigned long SC0_SCL0_UPDATE: 1;
            unsigned long : 3;
            unsigned long SC0_SCL0_VEN_C: 1;
            unsigned long SC0_SCL0_VEN_D: 1;
            unsigned long : 18;
        } BIT;
    } SC0_SCL0_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VMASK_ON: 1;
            unsigned long : 15;
            unsigned long SC0_RES_VMASK: 16;
        } BIT;
    } SC0_SCL0_FRC1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VLACK_ON: 1;
            unsigned long : 15;
            unsigned long SC0_RES_VLACK: 16;
        } BIT;
    } SC0_SCL0_FRC2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VS_SEL: 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 15;
        } BIT;
    } SC0_SCL0_FRC3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FH: 11;
            unsigned long : 5;
            unsigned long SC0_RES_FV: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_FRC4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VSDLY: 8;
            unsigned long SC0_RES_FLD_DLY_SEL: 1;
            unsigned long : 23;
        } BIT;
    } SC0_SCL0_FRC5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_F_VW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_F_VS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_FRC6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_F_HW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_F_HS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_FRC7;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_QVLACK: 1;
            unsigned long : 3;
            unsigned long SC0_RES_QVLOCK: 1;
            unsigned long : 27;
        } BIT;
    } SC0_SCL0_FRC9;
    union {
        unsigned short WORD;
        struct {
            unsigned short SC0_RES_LIN_STAT: 11;
            unsigned short : 5;
        } BIT;
    } SC0_SCL0_MON0;
    union {
        unsigned short WORD;
        struct {
            unsigned short SC0_RES_LINE: 11;
            unsigned short : 5;
        } BIT;
    } SC0_SCL0_INT;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_DS_H_ON: 1;
            unsigned long : 3;
            unsigned long SC0_RES_DS_V_ON: 1;
            unsigned long : 27;
        } BIT;
    } SC0_SCL0_DS1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_VS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_DS2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_HW: 12;
            unsigned long : 4;
            unsigned long SC0_RES_HS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_DS3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_DS_H_RATIO: 16;
            unsigned long : 12;
            unsigned long SC0_RES_DS_H_INTERPOTYP: 1;
            unsigned long SC0_RES_PFIL_SEL: 1;
            unsigned long : 2;
        } BIT;
    } SC0_SCL0_DS4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BTM_INIPHASE: 12;
            unsigned long : 4;
            unsigned long SC0_RES_TOP_INIPHASE: 12;
            unsigned long SC0_RES_V_INTERPOTYP: 1;
            unsigned long : 3;
        } BIT;
    } SC0_SCL0_DS5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_V_RATIO: 16;
            unsigned long : 16;
        } BIT;
    } SC0_SCL0_DS6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_OUT_HW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_OUT_VW: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_DS7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_US_H_ON: 1;
            unsigned long : 3;
            unsigned long SC0_RES_US_V_ON: 1;
            unsigned long : 27;
        } BIT;
    } SC0_SCL0_US1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_P_VW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_P_VS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_US2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_P_HW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_P_HS: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_US3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_IN_HW: 11;
            unsigned long : 5;
            unsigned long SC0_RES_IN_VW: 11;
            unsigned long : 5;
        } BIT;
    } SC0_SCL0_US4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_US_H_RATIO: 16;
            unsigned long : 16;
        } BIT;
    } SC0_SCL0_US5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_US_HB_INIPHASE: 12;
            unsigned long : 4;
            unsigned long SC0_RES_US_HT_INIPHASE: 12;
            unsigned long SC0_RES_US_H_INTERPOTYP: 1;
            unsigned long : 3;
        } BIT;
    } SC0_SCL0_US6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_VCUT: 8;
            unsigned long SC0_RES_HCUT: 8;
            unsigned long : 16;
        } BIT;
    } SC0_SCL0_US7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_DISP_ON: 1;
            unsigned long : 3;
            unsigned long SC0_RES_IBUS_SYNC_SEL: 1;
            unsigned long : 27;
        } BIT;
    } SC0_SCL0_US8;
    char           wk6[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BK_COL_B: 8;
            unsigned long SC0_RES_BK_COL_G: 8;
            unsigned long SC0_RES_BK_COL_R: 8;
            unsigned long : 8;
        } BIT;
    } SC0_SCL0_OVR1;
    char           wk7[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_SCL1_VEN_A: 1;
            unsigned long : 3;
            unsigned long SC0_SCL1_VEN_B: 1;
            unsigned long : 11;
            unsigned long SC0_SCL1_UPDATE_A: 1;
            unsigned long : 3;
            unsigned long SC0_SCL1_UPDATE_B: 1;
            unsigned long : 11;
        } BIT;
    } SC0_SCL1_UPDATE;
    char           wk8[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BST_MD: 1;
            unsigned long SC0_RES_LOOP: 1;
            unsigned long SC0_RES_MD: 2;
            unsigned long SC0_RES_DS_WR_MD: 3;
            unsigned long SC0_RES_TB_ADD_MOD: 1;
            unsigned long : 8;
            unsigned long SC0_RES_WRSWA: 3;
            unsigned long : 13;
        } BIT;
    } SC0_SCL1_WR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BASE: 32;
        } BIT;
    } SC0_SCL1_WR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_NUM: 10;
            unsigned long : 6;
            unsigned long SC0_RES_LN_OFF: 15;
            unsigned long : 1;
        } BIT;
    } SC0_SCL1_WR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_OFF: 23;
            unsigned long : 9;
        } BIT;
    } SC0_SCL1_WR4;
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_WENB: 1;
            unsigned long : 3;
            unsigned long SC0_RES_FLD_SEL: 1;
            unsigned long : 3;
            unsigned long SC0_RES_FS_RATE: 2;
            unsigned long : 2;
            unsigned long SC0_RES_INTER: 1;
            unsigned long : 19;
        } BIT;
    } SC0_SCL1_WR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BITDEC_ON: 1;
            unsigned long : 3;
            unsigned long SC0_RES_DTH_ON: 1;
            unsigned long : 27;
        } BIT;
    } SC0_SCL1_WR6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_CNT: 10;
            unsigned long : 6;
            unsigned long SC0_RES_OVERFLOW: 1;
            unsigned long : 15;
        } BIT;
    } SC0_SCL1_WR7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_BASE_B: 32;
        } BIT;
    } SC0_SCL1_WR8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_NUM_B: 10;
            unsigned long : 6;
            unsigned long SC0_RES_LN_OFF_B: 15;
            unsigned long : 1;
        } BIT;
    } SC0_SCL1_WR9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_OFF_B: 23;
            unsigned long : 9;
        } BIT;
    } SC0_SCL1_WR10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_RES_FLM_CNT_B: 10;
            unsigned long : 22;
        } BIT;
    } SC0_SCL1_WR11;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long SC0_PBUF_NUM: 2;
            unsigned long : 22;
        } BIT;
    } SC0_SCL1_MON1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_PBUF0_ADD: 32;
        } BIT;
    } SC0_SCL1_PBUF0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_PBUF1_ADD: 32;
        } BIT;
    } SC0_SCL1_PBUF1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_PBUF2_ADD: 32;
        } BIT;
    } SC0_SCL1_PBUF2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_PBUF3_ADD: 32;
        } BIT;
    } SC0_SCL1_PBUF3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC0_FLD_INF0: 1;
            unsigned long : 7;
            unsigned long SC0_FLD_INF1: 1;
            unsigned long : 7;
            unsigned long SC0_FLD_INF2: 1;
            unsigned long : 7;
            unsigned long SC0_FLD_INF3: 1;
            unsigned long : 7;
        } BIT;
    } SC0_SCL1_PBUF_FLD;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long SC0_PBUF_RST: 1;
            unsigned long : 15;
        } BIT;
    } SC0_SCL1_PBUF_CNT;
    char           wk10[44];
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_IBUS_VEN: 1;
            unsigned long : 3;
            unsigned long GR0_P_VEN: 1;
            unsigned long : 3;
            unsigned long GR0_UPDATE: 1;
            unsigned long : 23;
        } BIT;
    } GR0_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_R_ENB: 1;
            unsigned long : 31;
        } BIT;
    } GR0_FLM_RD;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_BST_MD: 1;
            unsigned long : 3;
            unsigned long GR0_IMR_FLM_INV: 1;
            unsigned long : 3;
            unsigned long GR0_FLM_SEL: 2;
            unsigned long : 6;
            unsigned long GR0_LN_OFF_DIR: 1;
            unsigned long : 14;
            unsigned long GR0_FLD_SEL: 1;
        } BIT;
    } GR0_FLM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_BASE: 32;
        } BIT;
    } GR0_FLM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_FLM_NUM: 10;
            unsigned long : 6;
            unsigned long GR0_LN_OFF: 15;
            unsigned long GR0_FLD_NXT: 1;
        } BIT;
    } GR0_FLM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_FLM_OFF: 23;
            unsigned long : 9;
        } BIT;
    } GR0_FLM4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_FLM_LOOP: 11;
            unsigned long : 5;
            unsigned long GR0_FLM_LNUM: 11;
            unsigned long : 5;
        } BIT;
    } GR0_FLM5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_STA_POS: 6;
            unsigned long : 2;
            unsigned long GR0_CNV444_MD: 1;
            unsigned long : 1;
            unsigned long GR0_RDSWA: 3;
            unsigned long GR0_YCC_SWAP: 3;
            unsigned long GR0_HW: 11;
            unsigned long : 1;
            unsigned long GR0_FORMAT: 4;
        } BIT;
    } GR0_FLM6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_DISP_SEL: 2;
            unsigned long : 2;
            unsigned long GR0_GRC_DISP_ON: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 9;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
            unsigned long : 1;
            unsigned long : 3;
        } BIT;
    } GR0_AB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_GRC_VW: 11;
            unsigned long : 5;
            unsigned long GR0_GRC_VS: 11;
            unsigned long : 5;
        } BIT;
    } GR0_AB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_GRC_HW: 11;
            unsigned long : 5;
            unsigned long GR0_GRC_HS: 11;
            unsigned long : 5;
        } BIT;
    } GR0_AB3;
    char           wk11[4];
    char           wk12[4];
    char           wk13[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_CK_ON: 1;
            unsigned long : 15;
            unsigned long : 8;
            unsigned long : 8;
        } BIT;
    } GR0_AB7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_CK_KR: 8;
            unsigned long GR0_CK_KB: 8;
            unsigned long GR0_CK_KG: 8;
            unsigned long GR0_CK_KCLUT: 8;
        } BIT;
    } GR0_AB8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_CK_R: 8;
            unsigned long GR0_CK_B: 8;
            unsigned long GR0_CK_G: 8;
            unsigned long GR0_CK_A: 8;
        } BIT;
    } GR0_AB9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_R0: 8;
            unsigned long GR0_B0: 8;
            unsigned long GR0_G0: 8;
            unsigned long GR0_A0: 8;
        } BIT;
    } GR0_AB10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_R1: 8;
            unsigned long GR0_B1: 8;
            unsigned long GR0_G1: 8;
            unsigned long GR0_A1: 8;
        } BIT;
    } GR0_AB11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR0_BASE_R: 8;
            unsigned long GR0_BASE_B: 8;
            unsigned long GR0_BASE_G: 8;
            unsigned long : 8;
        } BIT;
    } GR0_BASE;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 11;
            unsigned long : 5;
            unsigned long GR0_CLT_SEL: 1;
            unsigned long : 15;
        } BIT;
    } GR0_CLUT;
    char           wk14[4];
    char           wk15[40];
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_VEN: 1;
            unsigned long : 31;
        } BIT;
    } ADJ0_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long BKSTR_T2: 5;
            unsigned long : 3;
            unsigned long BKSTR_T1: 5;
            unsigned long : 3;
            unsigned long BKSTR_D: 4;
            unsigned long BKSTR_ST: 4;
            unsigned long BKSTR_ON: 1;
            unsigned long : 7;
        } BIT;
    } ADJ0_BKSTR_SET;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENH_DISP_ON: 1;
            unsigned long : 3;
            unsigned long ENH_MD: 1;
            unsigned long : 27;
        } BIT;
    } ADJ0_ENH_TIM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENH_VW: 11;
            unsigned long : 5;
            unsigned long ENH_VS: 11;
            unsigned long : 5;
        } BIT;
    } ADJ0_ENH_TIM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENH_HW: 11;
            unsigned long : 5;
            unsigned long ENH_HS: 11;
            unsigned long : 5;
        } BIT;
    } ADJ0_ENH_TIM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H1_CORE: 7;
            unsigned long : 9;
            unsigned long SHP_H_ON: 1;
            unsigned long : 15;
        } BIT;
    } ADJ0_ENH_SHP1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H1_GAIN_U: 8;
            unsigned long SHP_H1_GAIN_O: 8;
            unsigned long SHP_H1_CLIP_U: 8;
            unsigned long SHP_H1_CLIP_O: 8;
        } BIT;
    } ADJ0_ENH_SHP2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H2_CORE: 7;
            unsigned long : 9;
            unsigned long SHP_H2_LPF_SEL: 1;
            unsigned long : 15;
        } BIT;
    } ADJ0_ENH_SHP3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H2_GAIN_U: 8;
            unsigned long SHP_H2_GAIN_O: 8;
            unsigned long SHP_H2_CLIP_U: 8;
            unsigned long SHP_H2_CLIP_O: 8;
        } BIT;
    } ADJ0_ENH_SHP4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H3_CORE: 7;
            unsigned long : 25;
        } BIT;
    } ADJ0_ENH_SHP5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SHP_H3_GAIN_U: 8;
            unsigned long SHP_H3_GAIN_O: 8;
            unsigned long SHP_H3_CLIP_U: 8;
            unsigned long SHP_H3_CLIP_O: 8;
        } BIT;
    } ADJ0_ENH_SHP6;
    union {
        unsigned long LONG;
        struct {
            unsigned long LTI_H2_CORE: 8;
            unsigned long LTI_H2_GAIN: 8;
            unsigned long LTI_H2_INC_ZERO: 8;
            unsigned long LTI_H2_LPF_SEL: 1;
            unsigned long : 6;
            unsigned long LTI_H_ON: 1;
        } BIT;
    } ADJ0_ENH_LTI1;
    union {
        unsigned long LONG;
        struct {
            unsigned long LTI_H4_CORE: 8;
            unsigned long LTI_H4_GAIN: 8;
            unsigned long LTI_H4_INC_ZERO: 8;
            unsigned long LTI_H4_MEDIAN_TAP_SEL: 1;
            unsigned long : 7;
        } BIT;
    } ADJ0_ENH_LTI2;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_MD: 2;
            unsigned long : 30;
        } BIT;
    } ADJ0_MTX_MODE;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_GG: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_YG: 8;
            unsigned long : 8;
        } BIT;
    } ADJ0_MTX_YG_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_GR: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_GB: 11;
            unsigned long : 5;
        } BIT;
    } ADJ0_MTX_YG_ADJ1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_BG: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_B: 8;
            unsigned long : 8;
        } BIT;
    } ADJ0_MTX_CBB_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_BR: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_BB: 11;
            unsigned long : 5;
        } BIT;
    } ADJ0_MTX_CBB_ADJ1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_RG: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_R: 8;
            unsigned long : 8;
        } BIT;
    } ADJ0_MTX_CRR_ADJ0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ0_MTX_RR: 11;
            unsigned long : 5;
            unsigned long ADJ0_MTX_RB: 11;
            unsigned long : 5;
        } BIT;
    } ADJ0_MTX_CRR_ADJ1;
    char           wk16[48];
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_IBUS_VEN: 1;
            unsigned long : 3;
            unsigned long GR2_P_VEN: 1;
            unsigned long : 3;
            unsigned long GR2_UPDATE: 1;
            unsigned long : 23;
        } BIT;
    } GR2_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_R_ENB: 1;
            unsigned long : 31;
        } BIT;
    } GR2_FLM_RD;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_BST_MD: 1;
            unsigned long : 7;
            unsigned long GR2_FLM_SEL: 2;
            unsigned long : 6;
            unsigned long GR2_LN_OFF_DIR: 1;
            unsigned long : 15;
        } BIT;
    } GR2_FLM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_BASE: 32;
        } BIT;
    } GR2_FLM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_FLM_NUM: 10;
            unsigned long : 6;
            unsigned long GR2_LN_OFF: 15;
            unsigned long : 1;
        } BIT;
    } GR2_FLM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_FLM_OFF: 23;
            unsigned long : 9;
        } BIT;
    } GR2_FLM4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_FLM_LOOP: 11;
            unsigned long : 5;
            unsigned long GR2_FLM_LNUM: 11;
            unsigned long : 5;
        } BIT;
    } GR2_FLM5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_STA_POS: 6;
            unsigned long : 4;
            unsigned long GR2_RDSWA: 3;
            unsigned long : 3;
            unsigned long GR2_HW: 11;
            unsigned long : 1;
            unsigned long GR2_FORMAT: 4;
        } BIT;
    } GR2_FLM6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_DISP_SEL: 2;
            unsigned long : 2;
            unsigned long GR2_GRC_DISP_ON: 1;
            unsigned long : 3;
            unsigned long GR2_ARC_DISP_ON: 1;
            unsigned long : 3;
            unsigned long GR2_ARC_ON: 1;
            unsigned long : 1;
            unsigned long GR2_ACALC_MD: 1;
            unsigned long GR2_ARC_MUL: 1;
            unsigned long : 16;
        } BIT;
    } GR2_AB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_GRC_VW: 11;
            unsigned long : 5;
            unsigned long GR2_GRC_VS: 11;
            unsigned long : 5;
        } BIT;
    } GR2_AB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_GRC_HW: 11;
            unsigned long : 5;
            unsigned long GR2_GRC_HS: 11;
            unsigned long : 5;
        } BIT;
    } GR2_AB3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_ARC_VW: 11;
            unsigned long : 5;
            unsigned long GR2_ARC_VS: 11;
            unsigned long : 5;
        } BIT;
    } GR2_AB4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_ARC_HW: 11;
            unsigned long : 5;
            unsigned long GR2_ARC_HS: 11;
            unsigned long : 5;
        } BIT;
    } GR2_AB5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_ARC_RATE: 8;
            unsigned long : 8;
            unsigned long GR2_ARC_COEF: 8;
            unsigned long GR2_ARC_MODE: 1;
            unsigned long : 7;
        } BIT;
    } GR2_AB6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_CK_ON: 1;
            unsigned long : 15;
            unsigned long GR2_ARC_DEF: 8;
            unsigned long : 8;
        } BIT;
    } GR2_AB7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_CK_KR: 8;
            unsigned long GR2_CK_KB: 8;
            unsigned long GR2_CK_KG: 8;
            unsigned long GR2_CK_KCLUT: 8;
        } BIT;
    } GR2_AB8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_CK_R: 8;
            unsigned long GR2_CK_B: 8;
            unsigned long GR2_CK_G: 8;
            unsigned long GR2_CK_A: 8;
        } BIT;
    } GR2_AB9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_R0: 8;
            unsigned long GR2_B0: 8;
            unsigned long GR2_G0: 8;
            unsigned long GR2_A0: 8;
        } BIT;
    } GR2_AB10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_R1: 8;
            unsigned long GR2_B1: 8;
            unsigned long GR2_G1: 8;
            unsigned long GR2_A1: 8;
        } BIT;
    } GR2_AB11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_BASE_R: 8;
            unsigned long GR2_BASE_B: 8;
            unsigned long GR2_BASE_G: 8;
            unsigned long : 8;
        } BIT;
    } GR2_BASE;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long GR2_CLT_SEL: 1;
            unsigned long : 15;
        } BIT;
    } GR2_CLUT;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR2_ARC_ST: 1;
            unsigned long : 31;
        } BIT;
    } GR2_MON;
    char           wk17[40];
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_IBUS_VEN: 1;
            unsigned long : 3;
            unsigned long GR3_P_VEN: 1;
            unsigned long : 3;
            unsigned long GR3_UPDATE: 1;
            unsigned long : 23;
        } BIT;
    } GR3_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_R_ENB: 1;
            unsigned long : 31;
        } BIT;
    } GR3_FLM_RD;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_BST_MD: 1;
            unsigned long : 7;
            unsigned long GR3_FLM_SEL: 2;
            unsigned long : 6;
            unsigned long GR3_LN_OFF_DIR: 1;
            unsigned long : 15;
        } BIT;
    } GR3_FLM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_BASE: 32;
        } BIT;
    } GR3_FLM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_FLM_NUM: 10;
            unsigned long : 6;
            unsigned long GR3_LN_OFF: 15;
            unsigned long : 1;
        } BIT;
    } GR3_FLM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_FLM_OFF: 23;
            unsigned long : 9;
        } BIT;
    } GR3_FLM4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_FLM_LOOP: 11;
            unsigned long : 5;
            unsigned long GR3_FLM_LNUM: 11;
            unsigned long : 5;
        } BIT;
    } GR3_FLM5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_STA_POS: 6;
            unsigned long : 4;
            unsigned long GR3_RDSWA: 3;
            unsigned long : 3;
            unsigned long GR3_HW: 11;
            unsigned long : 1;
            unsigned long GR3_FORMAT: 4;
        } BIT;
    } GR3_FLM6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_DISP_SEL: 2;
            unsigned long : 2;
            unsigned long GR3_GRC_DISP_ON: 1;
            unsigned long : 3;
            unsigned long GR3_ARC_DISP_ON: 1;
            unsigned long : 3;
            unsigned long GR3_ARC_ON: 1;
            unsigned long : 1;
            unsigned long GR3_ACALC_MD: 1;
            unsigned long GR3_ARC_MUL: 1;
            unsigned long : 16;
        } BIT;
    } GR3_AB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_GRC_VW: 11;
            unsigned long : 5;
            unsigned long GR3_GRC_VS: 11;
            unsigned long : 5;
        } BIT;
    } GR3_AB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_GRC_HW: 11;
            unsigned long : 5;
            unsigned long GR3_GRC_HS: 11;
            unsigned long : 5;
        } BIT;
    } GR3_AB3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_ARC_VW: 11;
            unsigned long : 5;
            unsigned long GR3_ARC_VS: 11;
            unsigned long : 5;
        } BIT;
    } GR3_AB4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_ARC_HW: 11;
            unsigned long : 5;
            unsigned long GR3_ARC_HS: 11;
            unsigned long : 5;
        } BIT;
    } GR3_AB5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_ARC_RATE: 8;
            unsigned long : 8;
            unsigned long GR3_ARC_COEF: 8;
            unsigned long GR3_ARC_MODE: 1;
            unsigned long : 7;
        } BIT;
    } GR3_AB6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_CK_ON: 1;
            unsigned long : 15;
            unsigned long GR3_ARC_DEF: 8;
            unsigned long : 8;
        } BIT;
    } GR3_AB7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_CK_KR: 8;
            unsigned long GR3_CK_KB: 8;
            unsigned long GR3_CK_KG: 8;
            unsigned long GR3_CK_KCLUT: 8;
        } BIT;
    } GR3_AB8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_CK_R: 8;
            unsigned long GR3_CK_B: 8;
            unsigned long GR3_CK_G: 8;
            unsigned long GR3_CK_A: 8;
        } BIT;
    } GR3_AB9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_R0: 8;
            unsigned long GR3_B0: 8;
            unsigned long GR3_G0: 8;
            unsigned long GR3_A0: 8;
        } BIT;
    } GR3_AB10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_R1: 8;
            unsigned long GR3_B1: 8;
            unsigned long GR3_G1: 8;
            unsigned long GR3_A1: 8;
        } BIT;
    } GR3_AB11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_BASE_R: 8;
            unsigned long GR3_BASE_B: 8;
            unsigned long GR3_BASE_G: 8;
            unsigned long : 8;
        } BIT;
    } GR3_BASE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_LINE: 11;
            unsigned long : 5;
            unsigned long GR3_CLT_SEL: 1;
            unsigned long : 15;
        } BIT;
    } GR3_CLUT_INT;
    union {
        unsigned long LONG;
        struct {
            unsigned long GR3_ARC_ST: 1;
            unsigned long : 15;
            unsigned long GR3_LIN_STAT: 11;
            unsigned long : 5;
        } BIT;
    } GR3_MON;
    char           wk18[40];
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_VEN: 1;
            unsigned long : 31;
        } BIT;
    } GAM_G_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_ON: 1;
            unsigned long : 31;
        } BIT;
    } GAM_SW;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_01: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_00: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_03: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_02: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_05: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_04: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_07: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_06: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_09: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_08: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_11: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_10: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_13: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_12: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_15: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_14: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_17: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_16: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_19: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_18: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_21: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_20: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_23: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_22: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT12;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_25: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_24: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT13;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_27: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_26: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT14;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_29: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_28: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT15;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_GAIN_31: 11;
            unsigned long : 5;
            unsigned long GAM_G_GAIN_30: 11;
            unsigned long : 5;
        } BIT;
    } GAM_G_LUT16;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_03: 8;
            unsigned long GAM_G_TH_02: 8;
            unsigned long GAM_G_TH_01: 8;
            unsigned long : 8;
        } BIT;
    } GAM_G_AREA1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_07: 8;
            unsigned long GAM_G_TH_06: 8;
            unsigned long GAM_G_TH_05: 8;
            unsigned long GAM_G_TH_04: 8;
        } BIT;
    } GAM_G_AREA2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_11: 8;
            unsigned long GAM_G_TH_10: 8;
            unsigned long GAM_G_TH_09: 8;
            unsigned long GAM_G_TH_08: 8;
        } BIT;
    } GAM_G_AREA3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_15: 8;
            unsigned long GAM_G_TH_14: 8;
            unsigned long GAM_G_TH_13: 8;
            unsigned long GAM_G_TH_12: 8;
        } BIT;
    } GAM_G_AREA4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_19: 8;
            unsigned long GAM_G_TH_18: 8;
            unsigned long GAM_G_TH_17: 8;
            unsigned long GAM_G_TH_16: 8;
        } BIT;
    } GAM_G_AREA5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_23: 8;
            unsigned long GAM_G_TH_22: 8;
            unsigned long GAM_G_TH_21: 8;
            unsigned long GAM_G_TH_20: 8;
        } BIT;
    } GAM_G_AREA6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_27: 8;
            unsigned long GAM_G_TH_26: 8;
            unsigned long GAM_G_TH_25: 8;
            unsigned long GAM_G_TH_24: 8;
        } BIT;
    } GAM_G_AREA7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_G_TH_31: 8;
            unsigned long GAM_G_TH_30: 8;
            unsigned long GAM_G_TH_29: 8;
            unsigned long GAM_G_TH_28: 8;
        } BIT;
    } GAM_G_AREA8;
    char           wk19[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_VEN: 1;
            unsigned long : 31;
        } BIT;
    } GAM_B_UPDATE;
    char           wk20[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_01: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_00: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_03: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_02: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_05: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_04: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_07: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_06: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_09: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_08: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_11: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_10: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_13: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_12: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_15: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_14: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_17: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_16: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_19: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_18: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_21: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_20: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_23: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_22: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT12;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_25: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_24: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT13;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_27: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_26: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT14;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_29: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_28: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT15;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_GAIN_31: 11;
            unsigned long : 5;
            unsigned long GAM_B_GAIN_30: 11;
            unsigned long : 5;
        } BIT;
    } GAM_B_LUT16;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_03: 8;
            unsigned long GAM_B_TH_02: 8;
            unsigned long GAM_B_TH_01: 8;
            unsigned long : 8;
        } BIT;
    } GAM_B_AREA1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_07: 8;
            unsigned long GAM_B_TH_06: 8;
            unsigned long GAM_B_TH_05: 8;
            unsigned long GAM_B_TH_04: 8;
        } BIT;
    } GAM_B_AREA2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_11: 8;
            unsigned long GAM_B_TH_10: 8;
            unsigned long GAM_B_TH_09: 8;
            unsigned long GAM_B_TH_08: 8;
        } BIT;
    } GAM_B_AREA3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_15: 8;
            unsigned long GAM_B_TH_14: 8;
            unsigned long GAM_B_TH_13: 8;
            unsigned long GAM_B_TH_12: 8;
        } BIT;
    } GAM_B_AREA4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_19: 8;
            unsigned long GAM_B_TH_18: 8;
            unsigned long GAM_B_TH_17: 8;
            unsigned long GAM_B_TH_16: 8;
        } BIT;
    } GAM_B_AREA5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_23: 8;
            unsigned long GAM_B_TH_22: 8;
            unsigned long GAM_B_TH_21: 8;
            unsigned long GAM_B_TH_20: 8;
        } BIT;
    } GAM_B_AREA6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_27: 8;
            unsigned long GAM_B_TH_26: 8;
            unsigned long GAM_B_TH_25: 8;
            unsigned long GAM_B_TH_24: 8;
        } BIT;
    } GAM_B_AREA7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_B_TH_31: 8;
            unsigned long GAM_B_TH_30: 8;
            unsigned long GAM_B_TH_29: 8;
            unsigned long GAM_B_TH_28: 8;
        } BIT;
    } GAM_B_AREA8;
    char           wk21[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_VEN: 1;
            unsigned long : 31;
        } BIT;
    } GAM_R_UPDATE;
    char           wk22[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_01: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_00: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_03: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_02: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_05: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_04: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_07: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_06: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_09: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_08: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_11: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_10: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_13: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_12: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_15: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_14: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT8;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_17: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_16: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT9;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_19: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_18: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT10;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_21: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_20: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT11;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_23: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_22: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT12;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_25: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_24: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT13;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_27: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_26: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT14;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_29: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_28: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT15;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_GAIN_31: 11;
            unsigned long : 5;
            unsigned long GAM_R_GAIN_30: 11;
            unsigned long : 5;
        } BIT;
    } GAM_R_LUT16;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_03: 8;
            unsigned long GAM_R_TH_02: 8;
            unsigned long GAM_R_TH_01: 8;
            unsigned long : 8;
        } BIT;
    } GAM_R_AREA1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_07: 8;
            unsigned long GAM_R_TH_06: 8;
            unsigned long GAM_R_TH_05: 8;
            unsigned long GAM_R_TH_04: 8;
        } BIT;
    } GAM_R_AREA2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_11: 8;
            unsigned long GAM_R_TH_10: 8;
            unsigned long GAM_R_TH_09: 8;
            unsigned long GAM_R_TH_08: 8;
        } BIT;
    } GAM_R_AREA3;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_15: 8;
            unsigned long GAM_R_TH_14: 8;
            unsigned long GAM_R_TH_13: 8;
            unsigned long GAM_R_TH_12: 8;
        } BIT;
    } GAM_R_AREA4;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_19: 8;
            unsigned long GAM_R_TH_18: 8;
            unsigned long GAM_R_TH_17: 8;
            unsigned long GAM_R_TH_16: 8;
        } BIT;
    } GAM_R_AREA5;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_23: 8;
            unsigned long GAM_R_TH_22: 8;
            unsigned long GAM_R_TH_21: 8;
            unsigned long GAM_R_TH_20: 8;
        } BIT;
    } GAM_R_AREA6;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_27: 8;
            unsigned long GAM_R_TH_26: 8;
            unsigned long GAM_R_TH_25: 8;
            unsigned long GAM_R_TH_24: 8;
        } BIT;
    } GAM_R_AREA7;
    union {
        unsigned long LONG;
        struct {
            unsigned long GAM_R_TH_31: 8;
            unsigned long GAM_R_TH_30: 8;
            unsigned long GAM_R_TH_29: 8;
            unsigned long GAM_R_TH_28: 8;
        } BIT;
    } GAM_R_AREA8;
    char           wk23[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_VEN: 1;
            unsigned long : 31;
        } BIT;
    } TCON_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_OFFSET: 11;
            unsigned long : 5;
            unsigned long TCON_HALF: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STVA_VW: 11;
            unsigned long : 5;
            unsigned long TCON_STVA_VS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_STVA1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STVA_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_STVA_INV: 1;
            unsigned long : 27;
        } BIT;
    } TCON_TIM_STVA2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STVB_VW: 11;
            unsigned long : 5;
            unsigned long TCON_STVB_VS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_STVB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STVB_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_STVB_INV: 1;
            unsigned long : 27;
        } BIT;
    } TCON_TIM_STVB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STH_HW: 11;
            unsigned long : 5;
            unsigned long TCON_STH_HS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_STH1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STH_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_STH_INV: 1;
            unsigned long : 3;
            unsigned long TCON_STH_HS_SEL: 1;
            unsigned long : 23;
        } BIT;
    } TCON_TIM_STH2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STB_HW: 11;
            unsigned long : 5;
            unsigned long TCON_STB_HS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_STB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_STB_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_STB_INV: 1;
            unsigned long : 3;
            unsigned long TCON_STB_HS_SEL: 1;
            unsigned long : 23;
        } BIT;
    } TCON_TIM_STB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_CPV_HW: 11;
            unsigned long : 5;
            unsigned long TCON_CPV_HS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_CPV1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_CPV_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_CPV_INV: 1;
            unsigned long : 3;
            unsigned long TCON_CPV_HS_SEL: 1;
            unsigned long : 23;
        } BIT;
    } TCON_TIM_CPV2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_POLA_HW: 11;
            unsigned long : 5;
            unsigned long TCON_POLA_HS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_POLA1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_POLA_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_POLA_INV: 1;
            unsigned long : 3;
            unsigned long TCON_POLA_HS_SEL: 1;
            unsigned long : 3;
            unsigned long TCON_POLA_MD: 2;
            unsigned long : 18;
        } BIT;
    } TCON_TIM_POLA2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_POLB_HW: 11;
            unsigned long : 5;
            unsigned long TCON_POLB_HS: 11;
            unsigned long : 5;
        } BIT;
    } TCON_TIM_POLB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_POLB_SEL: 3;
            unsigned long : 1;
            unsigned long TCON_POLB_INV: 1;
            unsigned long : 3;
            unsigned long TCON_POLB_HS_SEL: 1;
            unsigned long : 3;
            unsigned long TCON_POLB_MD: 2;
            unsigned long : 18;
        } BIT;
    } TCON_TIM_POLB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCON_DE_INV: 1;
            unsigned long : 31;
        } BIT;
    } TCON_TIM_DE;
    char           wk24[60];
    union {
        unsigned long LONG;
        struct {
            unsigned long OUTCNT_VEN: 1;
            unsigned long : 31;
        } BIT;
    } OUT_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long OUT_PHASE: 2;
            unsigned long : 2;
            unsigned long OUT_DIR_SEL: 1;
            unsigned long : 3;
            unsigned long OUT_FRQ_SEL: 2;
            unsigned long : 2;
            unsigned long OUT_FORMAT: 2;
            unsigned long : 2;
            unsigned long : 5;
            unsigned long : 2;
            unsigned long : 1;
            unsigned long OUT_SWAP_ON: 1;
            unsigned long : 3;
            unsigned long OUT_ENDIAN_ON: 1;
            unsigned long : 3;
        } BIT;
    } OUT_SET;
    union {
        unsigned long LONG;
        struct {
            unsigned long PBRT_G: 10;
            unsigned long : 22;
        } BIT;
    } OUT_BRIGHT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long PBRT_R: 10;
            unsigned long : 6;
            unsigned long PBRT_B: 10;
            unsigned long : 6;
        } BIT;
    } OUT_BRIGHT2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CONT_R: 8;
            unsigned long CONT_B: 8;
            unsigned long CONT_G: 8;
            unsigned long : 8;
        } BIT;
    } OUT_CONTRAST;
    union {
        unsigned long LONG;
        struct {
            unsigned long PDTH_PD: 2;
            unsigned long : 2;
            unsigned long PDTH_PC: 2;
            unsigned long : 2;
            unsigned long PDTH_PB: 2;
            unsigned long : 2;
            unsigned long PDTH_PA: 2;
            unsigned long : 2;
            unsigned long PDTH_FORMAT: 2;
            unsigned long : 2;
            unsigned long PDTH_SEL: 2;
            unsigned long : 10;
        } BIT;
    } OUT_PDTHA;
    char           wk25[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long OUTCNT_POLB_EDGE: 1;
            unsigned long OUTCNT_POLA_EDGE: 1;
            unsigned long OUTCNT_CPV_EDGE: 1;
            unsigned long OUTCNT_STB_EDGE: 1;
            unsigned long OUTCNT_STH_EDGE: 1;
            unsigned long OUTCNT_STVB_EDGE: 1;
            unsigned long OUTCNT_STVA_EDGE: 1;
            unsigned long : 1;
            unsigned long OUTCNT_LCD_EDGE: 1;
            unsigned long : 3;
            unsigned long OUTCNT_FRONT_GAM: 1;
            unsigned long : 19;
        } BIT;
    } OUT_CLK_PHASE;
    char           wk26[88];
    union {
        unsigned long LONG;
        struct {
            unsigned long INT_STA0: 1;
            unsigned long : 3;
            unsigned long INT_STA1: 1;
            unsigned long : 3;
            unsigned long INT_STA2: 1;
            unsigned long : 3;
            unsigned long INT_STA3: 1;
            unsigned long : 3;
            unsigned long INT_STA4: 1;
            unsigned long : 3;
            unsigned long INT_STA5: 1;
            unsigned long : 3;
            unsigned long INT_STA6: 1;
            unsigned long : 3;
            unsigned long INT_STA7: 1;
            unsigned long : 3;
        } BIT;
    } SYSCNT_INT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long INT_STA8: 1;
            unsigned long : 3;
            unsigned long INT_STA9: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
        } BIT;
    } SYSCNT_INT2;
    char           wk27[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long INT_OUT0_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT1_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT2_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT3_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT4_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT5_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT6_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT7_ON: 1;
            unsigned long : 3;
        } BIT;
    } SYSCNT_INT4;
    union {
        unsigned long LONG;
        struct {
            unsigned long INT_OUT8_ON: 1;
            unsigned long : 3;
            unsigned long INT_OUT9_ON: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
        } BIT;
    } SYSCNT_INT5;
    char           wk28[4];
    union {
        unsigned short WORD;
        struct {
            unsigned short PANEL_DCDR: 6;
            unsigned short : 2;
            unsigned short PANEL_ICKEN: 1;
            unsigned short : 1;
            unsigned short PANEL_OCKSEL: 2;
            unsigned short PANEL_ICKSEL: 2;
            unsigned short : 2;
        } BIT;
    } SYSCNT_PANEL_CLK;
    union {
        unsigned short WORD;
        struct {
            unsigned short GR0_CLT_SEL_ST: 1;
            unsigned short : 3;
            unsigned short : 1;
            unsigned short : 3;
            unsigned short GR2_CLT_SEL_ST: 1;
            unsigned short : 3;
            unsigned short GR3_CLT_SEL_ST: 1;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short : 1;
        } BIT;
    } SYSCNT_CLUT;
    char           wk29[356];
    char           wk30[4];
    char           wk31[4];
    char           wk32[4];
    char           wk33[4];
    char           wk34[4];
    char           wk35[4];
    char           wk36[4];
    char           wk37[4];
    char           wk38[4];
    char           wk39[4];
    char           wk40[2];
    char           wk41[2];
    char           wk42[4];
    char           wk43[4];
    char           wk44[4];
    char           wk45[4];
    char           wk46[4];
    char           wk47[4];
    char           wk48[4];
    char           wk49[4];
    char           wk50[4];
    char           wk51[4];
    char           wk52[4];
    char           wk53[4];
    char           wk54[4];
    char           wk55[4];
    char           wk56[4];
    char           wk57[4];
    char           wk58[4];
    char           wk59[16];
    char           wk60[4];
    char           wk61[4];
    char           wk62[4];
    char           wk63[4];
    char           wk64[4];
    char           wk65[4];
    char           wk66[4];
    char           wk67[4];
    char           wk68[4];
    char           wk69[4];
    char           wk70[4];
    char           wk71[4];
    char           wk72[4];
    char           wk73[4];
    char           wk74[4];
    char           wk75[4];
    char           wk76[4];
    char           wk77[4];
    char           wk78[4];
    char           wk79[4];
    char           wk80[4];
    char           wk81[44];
    char           wk82[4];
    char           wk83[4];
    char           wk84[4];
    char           wk85[4];
    char           wk86[4];
    char           wk87[4];
    char           wk88[4];
    char           wk89[4];
    char           wk90[4];
    char           wk91[4];
    char           wk92[4];
    char           wk93[4];
    char           wk94[4];
    char           wk95[4];
    char           wk96[4];
    char           wk97[4];
    char           wk98[4];
    char           wk99[4];
    char           wk100[4];
    char           wk101[4];
    char           wk102[4];
    char           wk103[4];
    char           wk104[40];
    char           wk105[4];
    char           wk106[4];
    char           wk107[4];
    char           wk108[4];
    char           wk109[4];
    char           wk110[4];
    char           wk111[4];
    char           wk112[4];
    char           wk113[4];
    char           wk114[4];
    char           wk115[4];
    char           wk116[4];
    char           wk117[4];
    char           wk118[4];
    char           wk119[4];
    char           wk120[4];
    char           wk121[4];
    char           wk122[4];
    char           wk123[4];
    char           wk124[4];
    char           wk125[48];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long GR_VIN_P_VEN: 1;
            unsigned long : 3;
            unsigned long GR_VIN_UPDATE: 1;
            unsigned long : 23;
        } BIT;
    } GR_VIN_UPDATE;
    char           wk126[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long GR_VIN_DISP_SEL: 2;
            unsigned long GR_VIN_SCL_UND_SEL: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 19;
        } BIT;
    } GR_VIN_AB1;
    char           wk127[4];
    char           wk128[4];
    char           wk129[4];
    char           wk130[4];
    char           wk131[4];
    char           wk132[4];
    char           wk133[16];
    char           wk134[4];
    char           wk135[4];
    char           wk136[4];
    char           wk137[40];
    char           wk138[4];
    char           wk139[4];
    char           wk140[4];
    char           wk141[4];
    char           wk142[4];
    char           wk143[4];
    char           wk144[4];
    char           wk145[4];
    char           wk146[4];
    char           wk147[4];
    char           wk148[2];
    char           wk149[2];
    char           wk150[4];
    char           wk151[4];
    char           wk152[4];
    char           wk153[4];
    char           wk154[4];
    char           wk155[4];
    char           wk156[4];
    char           wk157[4];
    char           wk158[4];
    char           wk159[4];
    char           wk160[4];
    char           wk161[4];
    char           wk162[4];
    char           wk163[4];
    char           wk164[4];
    char           wk165[4];
    char           wk166[4];
    char           wk167[16];
    char           wk168[4];
    char           wk169[4];
    char           wk170[4];
    char           wk171[4];
    char           wk172[4];
    char           wk173[4];
    char           wk174[4];
    char           wk175[4];
    char           wk176[4];
    char           wk177[4];
    char           wk178[4];
    char           wk179[4];
    char           wk180[4];
    char           wk181[4];
    char           wk182[4];
    char           wk183[4];
    char           wk184[4];
    char           wk185[4];
    char           wk186[4];
    char           wk187[4];
    char           wk188[4];
    char           wk189[44];
    char           wk190[4];
    char           wk191[4];
    char           wk192[4];
    char           wk193[4];
    char           wk194[4];
    char           wk195[4];
    char           wk196[4];
    char           wk197[4];
    char           wk198[4];
    char           wk199[4];
    char           wk200[4];
    char           wk201[4];
    char           wk202[4];
    char           wk203[4];
    char           wk204[4];
    char           wk205[4];
    char           wk206[4];
    char           wk207[4];
    char           wk208[4];
    char           wk209[4];
    char           wk210[4];
    char           wk211[4];
};

#define VDC6      (*(volatile struct st_vdc       *)0xFCFF7400)

#endif
