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

#ifndef CSI2LINK_IODEFINE_H
#define CSI2LINK_IODEFINE_H

struct st_csi2link {
    union {
        unsigned long LONG;
        struct {
            unsigned long TREF: 1;
            unsigned long : 31;
        } BIT;
    } TREF;
    union {
        unsigned long LONG;
        struct {
            unsigned long SRST: 1;
            unsigned long : 31;
        } BIT;
    } SRST;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENABLE_0: 1;
            unsigned long ENABLE_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long ENABLECLK: 1;
            unsigned long : 11;
            unsigned long RSTZ: 1;
            unsigned long SHUTDOWNZ: 1;
            unsigned long : 14;
        } BIT;
    } PHYCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRC_EN: 1;
            unsigned long ECC_EN: 1;
            unsigned long : 30;
        } BIT;
    } CHKSUM;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL_DT: 6;
            unsigned long SEL_DT_ON: 1;
            unsigned long : 1;
            unsigned long SEL_VC: 2;
            unsigned long : 5;
            unsigned long VCDT_EN: 1;
            unsigned long : 6;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long : 5;
            unsigned long : 1;
        } BIT;
    } VCDT;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long DT_FE: 6;
            unsigned long : 2;
            unsigned long DT_FS: 6;
            unsigned long : 2;
        } BIT;
    } FRDT;
    union {
        unsigned long LONG;
        struct {
            unsigned long FLD_EN: 1;
            unsigned long : 3;
            unsigned long FLD_DET_SEL: 2;
            unsigned long : 10;
            unsigned long FLD_NUM: 16;
        } BIT;
    } FLD;
    union {
        unsigned long LONG;
        struct {
            unsigned long AUTO_STANDBY_EN: 5;
            unsigned long VD_MSK_EN: 1;
            unsigned long : 2;
            unsigned long VD_MSK_CYCLE: 6;
            unsigned long : 18;
        } BIT;
    } ASTBY;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long LNGDT0: 32;
        } BIT;
    } LNGDT0;
    union {
        unsigned long LONG;
        struct {
            unsigned long LNGDT1: 32;
        } BIT;
    } LNGDT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long IEN: 32;
        } BIT;
    } INTEN;
    union {
        unsigned long LONG;
        struct {
            unsigned long ICL: 32;
        } BIT;
    } INTCLOSE;
    union {
        unsigned long LONG;
        struct {
            unsigned long IST: 32;
        } BIT;
    } INTSTATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long IEST: 16;
            unsigned long : 16;
        } BIT;
    } INTERRSTATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long DATA: 16;
            unsigned long ECC: 8;
        } BIT;
    } SHPDAT;
    union {
        unsigned long LONG;
        struct {
            unsigned long NUM: 4;
            unsigned long : 12;
            unsigned long OVF: 1;
            unsigned long : 15;
        } BIT;
    } SHPCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 24;
            unsigned long : 1;
            unsigned long REG_MONI_PACT_EN: 1;
            unsigned long : 5;
            unsigned long MONITOR_EN: 1;
        } BIT;
    } LINKCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long L0SEL: 2;
            unsigned long L1SEL: 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 24;
        } BIT;
    } LSWAP;
    char           wk2[4];
    char           wk3[4];
    char           wk4[4];
    char           wk5[12];
    char           wk6[4];
    char           wk7[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long ERRCONTROL_0: 1;
            unsigned long ERRCONTROL_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
            unsigned long ERRESC_0: 1;
            unsigned long ERRESC_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long CL_ERRCONTROL: 1;
            unsigned long : 19;
        } BIT;
    } PHEERM;
    union {
        unsigned long LONG;
        struct {
            unsigned long STOPSTATECLK: 1;
            unsigned long RXCLKACTIVEHS: 1;
            unsigned long RXULPSCLKNOT: 1;
            unsigned long ULPSACTIVENOTCLK: 1;
            unsigned long : 28;
        } BIT;
    } PHCLM;
    union {
        unsigned long LONG;
        struct {
            unsigned long STOPSTATEDATA_0: 1;
            unsigned long STOPSTATEDATA_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 4;
            unsigned long RXULPSESC_0: 1;
            unsigned long RXULPSESC_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long ULPSACTIVENOT_0: 1;
            unsigned long ULPSACTIVENOT_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 16;
        } BIT;
    } PHDLM;
    char           wk8[112];
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long : 8;
        } BIT;
    } PH0M0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PH_CNT: 16;
            unsigned long : 16;
        } BIT;
    } PH0M1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long : 8;
        } BIT;
    } PH1M0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PH_CNT: 16;
            unsigned long : 16;
        } BIT;
    } PH1M1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long : 8;
        } BIT;
    } PH2M0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PH_CNT: 16;
            unsigned long : 16;
        } BIT;
    } PH2M1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long : 8;
        } BIT;
    } PH3M0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PH_CNT: 16;
            unsigned long : 16;
        } BIT;
    } PH3M1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHRM0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHRM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHRM2;
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long CAL_PARITY: 8;
        } BIT;
    } PHCM0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long CAL_PARITY: 8;
        } BIT;
    } PHCM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAL_CRC: 16;
            unsigned long CRC: 16;
        } BIT;
    } CRCM0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAL_CRC: 16;
            unsigned long CRC: 16;
        } BIT;
    } CRCM1;
    char           wk10[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long ERRSOTHS_CNT: 8;
            unsigned long : 24;
        } BIT;
    } SERRCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long ERRSOTSYNCHS: 4;
            unsigned long : 28;
        } BIT;
    } SSERRCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECC_CRCT_CNT: 8;
            unsigned long : 24;
        } BIT;
    } ECCCM;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECC_ERR_CNT: 8;
            unsigned long : 24;
        } BIT;
    } ECECM;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRC_ERR_CNT: 8;
            unsigned long : 24;
        } BIT;
    } CRCECM;
    char           wk11[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long LINE_CNT: 16;
            unsigned long : 16;
        } BIT;
    } LCNT;
    char           wk12[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MONI_LINECNT: 16;
            unsigned long : 16;
        } BIT;
    } LCNTM;
    char           wk13[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MONI_FCOUNT: 16;
            unsigned long : 16;
        } BIT;
    } FCNTM;
    char           wk14[4];
    char           wk15[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long RXDATAHS_0: 8;
            unsigned long RXDATAHS_1: 8;
            unsigned long : 8;
            unsigned long : 8;
        } BIT;
    } PHYDIM;
    union {
        unsigned long LONG;
        struct {
            unsigned long RXSYNCHS_0_CNT: 4;
            unsigned long RXSYNCHS_1_CNT: 4;
            unsigned long : 4;
            unsigned long : 4;
            unsigned long RXACTIVEHS_0: 1;
            unsigned long RXACTIVEHS_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long RXVALIDHS_0: 1;
            unsigned long RXVALIDHS_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long RXCLK_CNT: 1;
        } BIT;
    } PHYIM;
    char           wk16[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CSIR_DAT: 32;
        } BIT;
    } VINDM;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSIR_HD_CNT: 12;
            unsigned long CSIR_VD_CNT: 4;
            unsigned long : 12;
            unsigned long : 4;
        } BIT;
    } VINSM1;
    char           wk17[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CSIR_PE: 1;
            unsigned long : 3;
            unsigned long CSIR_PEB: 4;
            unsigned long CSIR_FLD: 4;
            unsigned long CSIR_TAG: 2;
            unsigned long CSIR_ERRC: 1;
            unsigned long CSIR_ERRE: 1;
            unsigned long : 16;
        } BIT;
    } VINSM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENABLE_0: 1;
            unsigned long ENABLE_1: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long ENABLECLK: 1;
            unsigned long : 27;
        } BIT;
    } PHYOM;
    char           wk18[32];
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM3;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM4;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM5;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM6;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM7;
    union {
        unsigned long LONG;
        struct {
            unsigned long DT: 6;
            unsigned long VC: 2;
            unsigned long WC: 16;
            unsigned long ECC: 8;
        } BIT;
    } PHM8;
    char           wk19[4];
    char           wk20[4];
    char           wk21[4];
    char           wk22[4];
    char           wk23[4];
    char           wk24[4];
    char           wk25[4];
    char           wk26[4];
    char           wk27[84];
    char           wk28[4];
    char           wk29[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long T_INIT_SLAVE: 16;
            unsigned long : 16;
        } BIT;
    } PHYTIM1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCLK_PREPARE: 5;
            unsigned long : 3;
            unsigned long TCLK_SETTLE: 6;
            unsigned long : 2;
            unsigned long TCLK_MISS: 5;
            unsigned long : 11;
        } BIT;
    } PHYTIM2;
    union {
        unsigned long LONG;
        struct {
            unsigned long THS_PREPARE: 6;
            unsigned long : 2;
            unsigned long THS_SETTLE: 6;
            unsigned long : 18;
        } BIT;
    } PHYTIM3;
    char           wk30[4];
};

#define CSI2LINK  (*(volatile struct st_csi2link  *)0xE8209000)

#endif
