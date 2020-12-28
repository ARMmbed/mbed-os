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

#ifndef RIIC_IODEFINE_H
#define RIIC_IODEFINE_H

struct st_riic {
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
            unsigned char SDAI: 1;
            unsigned char SCLI: 1;
            unsigned char SDAO: 1;
            unsigned char SCLO: 1;
            unsigned char SOWP: 1;
            unsigned char CLO: 1;
            unsigned char IICRST: 1;
            unsigned char ICE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICCR1;
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
            unsigned char : 1;
            unsigned char ST: 1;
            unsigned char RS: 1;
            unsigned char SP: 1;
            unsigned char : 1;
            unsigned char TRS: 1;
            unsigned char MST: 1;
            unsigned char BBSY: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICCR2;
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
            unsigned char BC: 3;
            unsigned char BCWP: 1;
            unsigned char CKS: 3;
            unsigned char : 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICMR1;
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
            unsigned char TMOS: 1;
            unsigned char TMOL: 1;
            unsigned char TMOH: 1;
            unsigned char : 1;
            unsigned char SDDL: 3;
            unsigned char DLCS: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICMR2;
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
            unsigned char NF: 2;
            unsigned char ACKBR: 1;
            unsigned char ACKBT: 1;
            unsigned char ACKWP: 1;
            unsigned char RDRFS: 1;
            unsigned char WAIT: 1;
            unsigned char SMBE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICMR3;
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
            unsigned char TMOE: 1;
            unsigned char MALE: 1;
            unsigned char NALE: 1;
            unsigned char SALE: 1;
            unsigned char NACKE: 1;
            unsigned char NFE: 1;
            unsigned char SCLE: 1;
            unsigned char FMPE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICFER;
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
            unsigned char SAR0: 1;
            unsigned char SAR1: 1;
            unsigned char SAR2: 1;
            unsigned char GCE: 1;
            unsigned char : 1;
            unsigned char DIDE: 1;
            unsigned char : 1;
            unsigned char HOAE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICSER;
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
            unsigned char TMOIE: 1;
            unsigned char ALIE: 1;
            unsigned char STIE: 1;
            unsigned char SPIE: 1;
            unsigned char NAKIE: 1;
            unsigned char RIE: 1;
            unsigned char TEIE: 1;
            unsigned char TIE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICIER;
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
            unsigned char AAS0: 1;
            unsigned char AAS1: 1;
            unsigned char AAS2: 1;
            unsigned char GCA: 1;
            unsigned char : 1;
            unsigned char DID: 1;
            unsigned char : 1;
            unsigned char HOA: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICSR1;
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
            unsigned char TMOF: 1;
            unsigned char AL: 1;
            unsigned char START: 1;
            unsigned char STOP: 1;
            unsigned char NACKF: 1;
            unsigned char RDRF: 1;
            unsigned char TEND: 1;
            unsigned char TDRE: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICSR2;
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
            unsigned long SVA0: 1;
            unsigned long SVA: 9;
            unsigned long : 5;
            unsigned long FS0: 1;
            unsigned long : 16;
        } BIT;
    } ICSAR0;
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
            unsigned long SVA0: 1;
            unsigned long SVA: 9;
            unsigned long : 5;
            unsigned long FS1: 1;
            unsigned long : 16;
        } BIT;
    } ICSAR1;
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
            unsigned long SVA0: 1;
            unsigned long SVA: 9;
            unsigned long : 5;
            unsigned long FS2: 1;
            unsigned long : 16;
        } BIT;
    } ICSAR2;
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
            unsigned char BRL: 5;
            unsigned char : 3;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICBRL;
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
            unsigned char BRH: 5;
            unsigned char : 3;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICBRH;
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
            unsigned char DRT: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICDRT;
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
            unsigned char DRR: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } ICDRR;
};

#define RIIC30    (*(volatile struct st_riic      *)0xE803A000)
#define RIIC31    (*(volatile struct st_riic      *)0xE803A400)
#define RIIC32    (*(volatile struct st_riic      *)0xE803A800)
#define RIIC33    (*(volatile struct st_riic      *)0xE803AC00)

#endif
