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

#ifndef SPIBSC_IODEFINE_H
#define SPIBSC_IODEFINE_H

struct st_spibsc {
    union {
        unsigned long LONG;
        struct {
            unsigned long BSZ: 2;
            unsigned long : 6;
            unsigned long IO0FV: 2;
            unsigned long : 2;
            unsigned long IO2FV: 2;
            unsigned long IO3FV: 2;
            unsigned long MOIIO0: 2;
            unsigned long MOIIO1: 2;
            unsigned long MOIIO2: 2;
            unsigned long MOIIO3: 2;
            unsigned long : 7;
            unsigned long MD: 1;
        } BIT;
    } CMNCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SCKDL: 3;
            unsigned long : 5;
            unsigned long SLNDL: 3;
            unsigned long : 5;
            unsigned long SPNDL: 3;
            unsigned long : 13;
        } BIT;
    } SSLDR;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SSLE: 1;
            unsigned long : 7;
            unsigned long RBE: 1;
            unsigned long RCF: 1;
            unsigned long : 6;
            unsigned long RBURST: 5;
            unsigned long : 3;
            unsigned long SSLN: 1;
            unsigned long : 7;
        } BIT;
    } DRCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OCMD: 8;
            unsigned long : 8;
            unsigned long CMD: 8;
            unsigned long : 8;
        } BIT;
    } DRCMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long EAC: 3;
            unsigned long : 13;
            unsigned long EAV: 8;
            unsigned long : 8;
        } BIT;
    } DREAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OPD0: 8;
            unsigned long OPD1: 8;
            unsigned long OPD2: 8;
            unsigned long OPD3: 8;
        } BIT;
    } DROPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long OPDE: 4;
            unsigned long ADE: 4;
            unsigned long OCDE: 1;
            unsigned long : 1;
            unsigned long CDE: 1;
            unsigned long DME: 1;
            unsigned long DRDB: 2;
            unsigned long : 2;
            unsigned long OPDB: 2;
            unsigned long : 2;
            unsigned long ADB: 2;
            unsigned long : 2;
            unsigned long OCDB: 2;
            unsigned long CDB: 2;
        } BIT;
    } DRENR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPIE: 1;
            unsigned long SPIWE: 1;
            unsigned long SPIRE: 1;
            unsigned long : 5;
            unsigned long SSLKP: 1;
            unsigned long : 23;
        } BIT;
    } SMCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OCMD: 8;
            unsigned long : 8;
            unsigned long CMD: 8;
            unsigned long : 8;
        } BIT;
    } SMCMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADR: 32;
        } BIT;
    } SMADR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OPD0: 8;
            unsigned long OPD1: 8;
            unsigned long OPD2: 8;
            unsigned long OPD3: 8;
        } BIT;
    } SMOPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPIDE: 4;
            unsigned long OPDE: 4;
            unsigned long ADE: 4;
            unsigned long OCDE: 1;
            unsigned long : 1;
            unsigned long CDE: 1;
            unsigned long DME: 1;
            unsigned long SPIDB: 2;
            unsigned long : 2;
            unsigned long OPDB: 2;
            unsigned long : 2;
            unsigned long ADB: 2;
            unsigned long : 2;
            unsigned long OCDB: 2;
            unsigned long CDB: 2;
        } BIT;
    } SMENR;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long RDATA0: 32;
        } BIT;
    } SMRDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long RDATA1: 32;
        } BIT;
    } SMRDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long WDATA0: 32;
        } BIT;
    } SMWDR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long WDATA1: 32;
        } BIT;
    } SMWDR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TEND: 1;
            unsigned long SSLF: 1;
            unsigned long : 30;
        } BIT;
    } CMNSR;
    char           wk2[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long DMCYC: 5;
            unsigned long : 27;
        } BIT;
    } DRDMCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DRDRE: 1;
            unsigned long : 3;
            unsigned long OPDRE: 1;
            unsigned long : 3;
            unsigned long ADDRE: 1;
            unsigned long : 3;
            unsigned long HYPE: 3;
            unsigned long : 17;
        } BIT;
    } DRDRENR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DMCYC: 5;
            unsigned long : 27;
        } BIT;
    } SMDMCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPIDRE: 1;
            unsigned long : 3;
            unsigned long OPDRE: 1;
            unsigned long : 3;
            unsigned long ADDRE: 1;
            unsigned long : 3;
            unsigned long HYPE: 3;
            unsigned long : 17;
        } BIT;
    } SMDRENR;
    char           wk3[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ1: 32;
        } BIT;
    } PHYADJ1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ADJ2: 32;
        } BIT;
    } PHYADJ2;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long PHYMEM: 2;
            unsigned long WBUF: 1;
            unsigned long : 1;
            unsigned long WBUF2: 1;
            unsigned long : 11;
            unsigned long CKSEL: 2;
            unsigned long HS: 1;
            unsigned long : 1;
            unsigned long OCT: 1;
            unsigned long EXDS: 1;
            unsigned long OCTA_1_0: 2;
            unsigned long : 6;
            unsigned long ALT_ALIGN: 1;
            unsigned long CAL: 1;
        } BIT;
    } PHYCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 28;
            unsigned long DDRTMG: 2;
            unsigned long : 2;
        } BIT;
    } PHYOFFSET1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long OCTTMG: 3;
            unsigned long : 21;
        } BIT;
    } PHYOFFSET2;
    union {
        unsigned long LONG;
        struct {
            unsigned long INT: 1;
            unsigned long WPVAL: 1;
            unsigned long RSTVAL: 1;
            unsigned long : 21;
            unsigned long INTEN: 1;
            unsigned long WPEN: 1;
            unsigned long RSTEN: 1;
            unsigned long : 5;
        } BIT;
    } PHYINT;
};

#define SPIBSC    (*(volatile struct st_spibsc    *)0x1F800000)

#endif
