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

#ifndef OCTA_IODEFINE_H
#define OCTA_IODEFINE_H

struct st_octa {
    union {
        unsigned long LONG;
        struct {
            unsigned long DVCMD0: 8;
            unsigned long DVCMD1: 8;
            unsigned long : 8;
            unsigned long : 8;
        } BIT;
    } DCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DVAD0: 8;
            unsigned long DVAD1: 8;
            unsigned long DVAD2: 8;
            unsigned long DVAD3: 8;
        } BIT;
    } DAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DALEN: 8;
            unsigned long DMLEN: 8;
            unsigned long : 3;
            unsigned long ACDV: 1;
            unsigned long CMDLEN: 3;
            unsigned long DAOR: 1;
            unsigned long ADLEN: 3;
            unsigned long DOPI: 1;
            unsigned long ACDA: 1;
            unsigned long : 1;
            unsigned long : 2;
        } BIT;
    } DCSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DV0SZ: 30;
            unsigned long DV0TYP: 2;
        } BIT;
    } DSR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DV1SZ: 30;
            unsigned long DV1TYP: 2;
        } BIT;
    } DSR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DV0DEL: 8;
            unsigned long DQSERAM: 4;
            unsigned long DQSESOPI: 4;
            unsigned long DV1DEL: 8;
            unsigned long DQSEDOPI: 4;
            unsigned long : 4;
        } BIT;
    } MDTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CTP: 32;
        } BIT;
    } ACTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAD0: 32;
        } BIT;
    } ACAR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAD1: 32;
        } BIT;
    } ACAR1;
    char           wk0[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long CTRW0: 7;
            unsigned long CTR0: 1;
            unsigned long DVRDCMD0: 3;
            unsigned long DVRDHI0: 3;
            unsigned long DVRDLO0: 2;
            unsigned long : 8;
            unsigned long DVRDCMD1: 3;
            unsigned long DVRDHI1: 3;
            unsigned long DVRDLO1: 2;
        } BIT;
    } DRCSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long DVWCMD0: 3;
            unsigned long DVWHI0: 3;
            unsigned long DVWLO0: 2;
            unsigned long : 8;
            unsigned long DVWCMD1: 3;
            unsigned long DVWHI1: 3;
            unsigned long DVWLO1: 2;
        } BIT;
    } DWCSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long DVSELCMD: 3;
            unsigned long DVSELHI: 3;
            unsigned long DVSELLO: 2;
            unsigned long : 16;
        } BIT;
    } DCSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DV0TTYP: 2;
            unsigned long DV1TTYP: 2;
            unsigned long DV0PC: 1;
            unsigned long DV1PC: 1;
            unsigned long : 4;
            unsigned long ACMEME: 2;
            unsigned long ACMODE: 2;
            unsigned long : 17;
            unsigned long DLFT: 1;
        } BIT;
    } CDSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DV0RDL: 8;
            unsigned long DV0WDL: 8;
            unsigned long DV1RDL: 8;
            unsigned long DV1WDL: 8;
        } BIT;
    } MDLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long D0MRCMD0: 8;
            unsigned long D0MRCMD1: 8;
            unsigned long D0MWCMD0: 8;
            unsigned long D0MWCMD1: 8;
        } BIT;
    } MRWCR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long D1MRCMD0: 8;
            unsigned long D1MRCMD1: 8;
            unsigned long D1MWCMD0: 8;
            unsigned long D1MWCMD1: 8;
        } BIT;
    } MRWCR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long MRAL0: 3;
            unsigned long MRCL0: 3;
            unsigned long MRO0: 1;
            unsigned long : 1;
            unsigned long MWAL0: 3;
            unsigned long MWCL0: 3;
            unsigned long MWO0: 1;
            unsigned long : 1;
            unsigned long MRAL1: 3;
            unsigned long MRCL1: 3;
            unsigned long MRO1: 1;
            unsigned long : 1;
            unsigned long MWAL1: 3;
            unsigned long MWCL1: 3;
            unsigned long MWO1: 1;
            unsigned long : 1;
        } BIT;
    } MRWCSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long MRESR: 8;
            unsigned long MWESR: 8;
            unsigned long : 16;
        } BIT;
    } ESR;
    unsigned long  CWNDR;
    union {
        unsigned long LONG;
        struct {
            unsigned long WD0: 8;
            unsigned long WD1: 8;
            unsigned long WD2: 8;
            unsigned long WD3: 8;
        } BIT;
    } CWDR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RD0: 8;
            unsigned long RD1: 8;
            unsigned long RD2: 8;
            unsigned long RD3: 8;
        } BIT;
    } CRR;
    char           wk1[4];
    char           wk2[4];
    char           wk3[4];
    char           wk4[4];
    char           wk5[4];
    char           wk6[4];
};

#define OCTA      (*(volatile struct st_octa      *)0x1F401000)

#endif
