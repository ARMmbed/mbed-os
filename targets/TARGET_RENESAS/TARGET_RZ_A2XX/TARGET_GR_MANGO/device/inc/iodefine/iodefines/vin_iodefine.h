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

#ifndef VIN_IODEFINE_H
#define VIN_IODEFINE_H

struct st_vin {
    union {
        unsigned long LONG;
        struct {
            unsigned long ME: 1;
            unsigned long BPS: 1;
            unsigned long : 1;
            unsigned long IM: 2;
            unsigned long : 1;
            unsigned long EN: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long DC: 2;
            unsigned long INF: 3;
            unsigned long YCAL: 1;
            unsigned long LUTE: 1;
            unsigned long : 1;
            unsigned long : 4;
            unsigned long SCLE: 1;
            unsigned long : 1;
            unsigned long CLP: 2;
            unsigned long : 2;
        } BIT;
    } V0MC;
    union {
        unsigned long LONG;
        struct {
            unsigned long CA: 1;
            unsigned long AV: 1;
            unsigned long FS: 1;
            unsigned long FBS: 2;
            unsigned long : 27;
        } BIT;
    } V0MS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SC: 1;
            unsigned long CC: 1;
            unsigned long : 30;
        } BIT;
    } V0FC;
    union {
        unsigned long LONG;
        struct {
            unsigned long SLPrC: 11;
            unsigned long : 1;
            unsigned long : 20;
        } BIT;
    } V0SLPrC;
    union {
        unsigned long LONG;
        struct {
            unsigned long ELPrC: 11;
            unsigned long : 1;
            unsigned long : 20;
        } BIT;
    } V0ELPrC;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPPrC: 11;
            unsigned long : 1;
            unsigned long : 20;
        } BIT;
    } V0SPPrC;
    union {
        unsigned long LONG;
        struct {
            unsigned long EPPrC: 11;
            unsigned long : 1;
            unsigned long : 20;
        } BIT;
    } V0EPPrC;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long : 21;
            unsigned long DES0: 1;
            unsigned long : 1;
            unsigned long : 5;
        } BIT;
    } V0CSI_IFMD;
    char           wk1[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long IS: 9;
            unsigned long : 19;
        } BIT;
    } V0IS;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 7;
            unsigned long MB1: 25;
        } BIT;
    } V0MB1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 7;
            unsigned long MB2: 25;
        } BIT;
    } V0MB2;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 7;
            unsigned long MB3: 25;
        } BIT;
    } V0MB3;
    union {
        unsigned long LONG;
        struct {
            unsigned long LC: 12;
            unsigned long : 20;
        } BIT;
    } V0LC;
    union {
        unsigned long LONG;
        struct {
            unsigned long FOE: 1;
            unsigned long EFE: 1;
            unsigned long SIE: 1;
            unsigned long : 1;
            unsigned long FIE: 1;
            unsigned long : 11;
            unsigned long VRE: 1;
            unsigned long VFE: 1;
            unsigned long : 13;
            unsigned long FIE2: 1;
        } BIT;
    } V0IE;
    union {
        unsigned long LONG;
        struct {
            unsigned long FOS: 1;
            unsigned long EFS: 1;
            unsigned long SIS: 1;
            unsigned long : 1;
            unsigned long FIS: 1;
            unsigned long : 11;
            unsigned long VRS: 1;
            unsigned long VFS: 1;
            unsigned long : 13;
            unsigned long FIS2: 1;
        } BIT;
    } V0INTS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SI: 11;
            unsigned long : 21;
        } BIT;
    } V0SI;
    char           wk2[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long DTMD: 2;
            unsigned long ABIT: 1;
            unsigned long : 1;
            unsigned long BPSM: 1;
            unsigned long : 3;
            unsigned long EXRGB: 1;
            unsigned long : 2;
            unsigned long YC_THR: 1;
            unsigned long YMODE: 3;
            unsigned long : 1;
            unsigned long EVA: 1;
            unsigned long : 7;
            unsigned long A8BIT: 8;
        } BIT;
    } V0DMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HLV: 11;
            unsigned long : 1;
            unsigned long VLV: 4;
            unsigned long FTEH: 1;
            unsigned long FTEV: 1;
            unsigned long : 4;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
        } BIT;
    } V0DMR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 7;
            unsigned long UVAOF: 25;
        } BIT;
    } V0UVAOF;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSUB: 8;
            unsigned long YSUB: 8;
            unsigned long YMUL: 10;
            unsigned long : 6;
        } BIT;
    } V0CSCC1;
    union {
        unsigned long LONG;
        struct {
            unsigned long GCRMUL: 10;
            unsigned long : 6;
            unsigned long RCRMUL: 10;
            unsigned long : 6;
        } BIT;
    } V0CSCC2;
    union {
        unsigned long LONG;
        struct {
            unsigned long BCBMUL: 10;
            unsigned long : 6;
            unsigned long GCBMUL: 10;
            unsigned long : 6;
        } BIT;
    } V0CSCC3;
    char           wk3[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 14;
            unsigned long NE_BCB: 1;
            unsigned long NE_GY: 1;
            unsigned long NE_RCR: 1;
            unsigned long : 1;
            unsigned long BC: 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long AMD: 1;
            unsigned long : 1;
        } BIT;
    } V0UDS_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long VFRAC: 12;
            unsigned long VMANT: 4;
            unsigned long HFRAC: 12;
            unsigned long HMANT: 4;
        } BIT;
    } V0UDS_SCALE;
    char           wk4[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long BWIDTH_V: 7;
            unsigned long : 9;
            unsigned long BWIDTH_H: 7;
            unsigned long : 9;
        } BIT;
    } V0UDS_PASS_BWIDTH;
    char           wk5[4];
    char           wk6[4];
    char           wk7[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long CL_VSIZE: 12;
            unsigned long : 4;
            unsigned long CL_HSIZE: 12;
            unsigned long : 4;
        } BIT;
    } V0UDS_CLIP_SIZE;
    char           wk8[88];
    union {
        unsigned long LONG;
        struct {
            unsigned long LTCRPR: 10;
            unsigned long LTCBPR: 10;
            unsigned long LTYPR: 10;
            unsigned long : 2;
        } BIT;
    } V0LUTP;
    union {
        unsigned long LONG;
        struct {
            unsigned long LTCRDT: 8;
            unsigned long LTCBDT: 8;
            unsigned long LTYDT: 8;
            unsigned long : 8;
        } BIT;
    } V0LUTD;
    char           wk9[288];
    union {
        unsigned long LONG;
        struct {
            unsigned long YCLRP: 13;
            unsigned long : 19;
        } BIT;
    } V0YCCR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long YCLGP: 13;
            unsigned long : 3;
            unsigned long YCLBP: 13;
            unsigned long : 3;
        } BIT;
    } V0YCCR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long YCLAP: 12;
            unsigned long : 4;
            unsigned long YCLCEN: 1;
            unsigned long : 6;
            unsigned long YCLHEN: 1;
            unsigned long YCLSFT: 5;
            unsigned long : 2;
            unsigned long YEXPEN: 1;
        } BIT;
    } V0YCCR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCLRP: 13;
            unsigned long : 19;
        } BIT;
    } V0CBCCR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCLGP: 13;
            unsigned long : 3;
            unsigned long CBCLBP: 13;
            unsigned long : 3;
        } BIT;
    } V0CBCCR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCLAP: 12;
            unsigned long : 4;
            unsigned long CBCLCEN: 1;
            unsigned long : 6;
            unsigned long CBCLHEN: 1;
            unsigned long CBCLSFT: 5;
            unsigned long : 2;
            unsigned long CBEXPEN: 1;
        } BIT;
    } V0CBCCR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRCLRP: 13;
            unsigned long : 19;
        } BIT;
    } V0CRCCR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRCLGP: 13;
            unsigned long : 3;
            unsigned long CRCLBP: 13;
            unsigned long : 3;
        } BIT;
    } V0CRCCR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRCLAP: 12;
            unsigned long : 4;
            unsigned long CRCLCEN: 1;
            unsigned long : 6;
            unsigned long CRCLHEN: 1;
            unsigned long CRCLSFT: 5;
            unsigned long : 2;
            unsigned long CREXPEN: 1;
        } BIT;
    } V0CRCCR3;
    char           wk10[180];
    union {
        unsigned long LONG;
        struct {
            unsigned long YMUL2: 14;
            unsigned long : 18;
        } BIT;
    } V0CSCE1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSUB2: 12;
            unsigned long : 4;
            unsigned long YSUB2: 12;
            unsigned long : 4;
        } BIT;
    } V0CSCE2;
    union {
        unsigned long LONG;
        struct {
            unsigned long GCRMUL2: 14;
            unsigned long : 2;
            unsigned long RCRMUL2: 14;
            unsigned long : 2;
        } BIT;
    } V0CSCE3;
    union {
        unsigned long LONG;
        struct {
            unsigned long BCBMUL2: 14;
            unsigned long : 2;
            unsigned long GCBMUL2: 14;
            unsigned long : 2;
        } BIT;
    } V0CSCE4;
    char           wk11[16];
    char           wk12[4];
    char           wk13[4];
    char           wk14[4];
    char           wk15[4];
    char           wk16[4];
    char           wk17[4];
    char           wk18[4];
    char           wk19[4];
    char           wk20[4];
    char           wk21[4];
    char           wk22[4];
    char           wk23[4];
    char           wk24[4];
    char           wk25[4];
    char           wk26[4];
};

#define VIN       (*(volatile struct st_vin       *)0xE803F000)

#endif
