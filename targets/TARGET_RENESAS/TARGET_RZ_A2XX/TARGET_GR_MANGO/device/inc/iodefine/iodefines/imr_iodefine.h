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

#ifndef IMR_IODEFINE_H
#define IMR_IODEFINE_H

struct st_imr {
    union {
        unsigned long LONG;
        struct {
            unsigned long RS: 1;
            unsigned long ARS: 1;
            unsigned long SFE: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 9;
            unsigned long SWRST: 1;
            unsigned long : 16;
        } BIT;
    } CR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TRA: 1;
            unsigned long IER: 1;
            unsigned long INT: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long DSA: 1;
            unsigned long SFS: 1;
            unsigned long : 24;
        } BIT;
    } SR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TRACLR: 1;
            unsigned long IERCLR: 1;
            unsigned long INTCLR: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 27;
        } BIT;
    } SRCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TRAENB: 1;
            unsigned long IERENB: 1;
            unsigned long INTENB: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 27;
        } BIT;
    } ICR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TEAM: 1;
            unsigned long IEM: 1;
            unsigned long INM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 27;
        } BIT;
    } IMR;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DLP: 32;
        } BIT;
    } DLPR;
    char           wk1[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 3;
            unsigned long DLSA: 29;
        } BIT;
    } DLSAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 5;
            unsigned long DSA: 27;
        } BIT;
    } DSAR;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DST: 14;
            unsigned long : 18;
        } BIT;
    } DSTR;
    char           wk3[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 5;
            unsigned long DSA2: 27;
        } BIT;
    } DSAR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 3;
            unsigned long DLSA2: 29;
        } BIT;
    } DLSAR2;
    char           wk4[4];
    char           wk5[4];
    char           wk6[4];
    char           wk7[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TME: 1;
            unsigned long BFE: 1;
            unsigned long AUTODG: 1;
            unsigned long AUTOSG: 1;
            unsigned long DXDYM: 1;
            unsigned long DUDVM: 1;
            unsigned long TCM: 1;
            unsigned long : 25;
        } BIT;
    } TRIMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMES: 1;
            unsigned long BFES: 1;
            unsigned long AUTODGS: 1;
            unsigned long AUTOSGS: 1;
            unsigned long DXDYMS: 1;
            unsigned long DUDVMS: 1;
            unsigned long TCMS: 1;
            unsigned long : 25;
        } BIT;
    } TRIMSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMEC: 1;
            unsigned long BFEC: 1;
            unsigned long AUTODGC: 1;
            unsigned long AUTOSGC: 1;
            unsigned long DXDYMC: 1;
            unsigned long DUDVMC: 1;
            unsigned long TCMC: 1;
            unsigned long : 25;
        } BIT;
    } TRIMCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCY: 8;
            unsigned long TCU: 8;
            unsigned long TCV: 8;
            unsigned long : 2;
            unsigned long : 5;
            unsigned long YCFORM: 1;
        } BIT;
    } TRICR;
    union {
        unsigned long LONG;
        struct {
            unsigned long UVDPO: 3;
            unsigned long : 5;
            unsigned long DDP: 1;
            unsigned long : 23;
        } BIT;
    } UVDPOR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SVW: 11;
            unsigned long : 5;
            unsigned long SUW: 11;
            unsigned long : 5;
        } BIT;
    } SUSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SVS: 11;
            unsigned long : 21;
        } BIT;
    } SVSR;
    char           wk8[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long XMIN: 13;
            unsigned long : 19;
        } BIT;
    } XMINR;
    union {
        unsigned long LONG;
        struct {
            unsigned long YMIN: 13;
            unsigned long : 19;
        } BIT;
    } YMINR;
    union {
        unsigned long LONG;
        struct {
            unsigned long XMAX: 13;
            unsigned long : 19;
        } BIT;
    } XMAXR;
    union {
        unsigned long LONG;
        struct {
            unsigned long YMAX: 13;
            unsigned long : 19;
        } BIT;
    } YMAXR;
    union {
        unsigned long LONG;
        struct {
            unsigned long AMXS: 13;
            unsigned long : 19;
        } BIT;
    } AMXSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long AMYS: 13;
            unsigned long : 19;
        } BIT;
    } AMYSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long AMXO: 13;
            unsigned long : 19;
        } BIT;
    } AMXOR;
    union {
        unsigned long LONG;
        struct {
            unsigned long AMYO: 13;
            unsigned long : 19;
        } BIT;
    } AMYOR;
    char           wk9[16];
    char           wk10[4];
    char           wk11[4];
    char           wk12[4];
    char           wk13[4];
    char           wk14[4];
    char           wk15[4];
    char           wk16[8];
    char           wk17[4];
    char           wk18[4];
    char           wk19[4];
    char           wk20[4];
    char           wk21[32];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long EMAM: 1;
            unsigned long : 17;
            unsigned long : 1;
            unsigned long : 1;
        } BIT;
    } MACR1;
    char           wk22[4];
    char           wk23[2296];
    union {
        unsigned long LONG;
        struct {
            unsigned long LSPR: 10;
            unsigned long : 22;
        } BIT;
    } LSPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LEPR: 10;
            unsigned long : 22;
        } BIT;
    } LEPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LMSR: 3;
            unsigned long : 29;
        } BIT;
    } LMSR;
    char           wk24[20];
    char           wk25[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPPC: 11;
            unsigned long : 21;
        } BIT;
    } LMSPPCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long EPPC: 11;
            unsigned long : 21;
        } BIT;
    } LMEPPCR;
};

#define IMR2      (*(volatile struct st_imr       *)0xFCFF3008)

#endif
