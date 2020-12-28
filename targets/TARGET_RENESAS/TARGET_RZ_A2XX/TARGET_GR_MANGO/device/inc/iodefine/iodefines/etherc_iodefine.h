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

#ifndef ETHERC_IODEFINE_H
#define ETHERC_IODEFINE_H

struct st_etherc {
    union {
        unsigned long LONG;
        struct {
            unsigned long PRM: 1;
            unsigned long DM: 1;
            unsigned long RTM: 1;
            unsigned long ILB: 1;
            unsigned long : 1;
            unsigned long TE: 1;
            unsigned long RE: 1;
            unsigned long : 2;
            unsigned long MPDE: 1;
            unsigned long : 2;
            unsigned long PRCEF: 1;
            unsigned long : 3;
            unsigned long TXF: 1;
            unsigned long RXF: 1;
            unsigned long PFR: 1;
            unsigned long ZPF: 1;
            unsigned long TPC: 1;
            unsigned long : 11;
        } BIT;
    } ECMR;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RFL: 12;
            unsigned long : 20;
        } BIT;
    } RFLR;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long ICD: 1;
            unsigned long MPD: 1;
            unsigned long LCHNG: 1;
            unsigned long : 1;
            unsigned long PSRTO: 1;
            unsigned long BFR: 1;
            unsigned long : 26;
        } BIT;
    } ECSR;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long ICDIP: 1;
            unsigned long MPDIP: 1;
            unsigned long LCHNGIP: 1;
            unsigned long : 1;
            unsigned long PSRTOIP: 1;
            unsigned long BFSIPR: 1;
            unsigned long : 26;
        } BIT;
    } ECSIPR;
    char           wk3[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MDC: 1;
            unsigned long MMD: 1;
            unsigned long MDO: 1;
            unsigned long MDI: 1;
            unsigned long : 28;
        } BIT;
    } PIR;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long LMON: 1;
            unsigned long : 31;
        } BIT;
    } PSR;
    char           wk5[20];
    union {
        unsigned long LONG;
        struct {
            unsigned long RMD: 20;
            unsigned long : 12;
        } BIT;
    } RDMLR;
    char           wk6[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long IPG: 5;
            unsigned long : 27;
        } BIT;
    } IPGR;
    union {
        unsigned long LONG;
        struct {
            unsigned long AP: 16;
            unsigned long : 16;
        } BIT;
    } APR;
    union {
        unsigned long LONG;
        struct {
            unsigned long MP: 16;
            unsigned long : 16;
        } BIT;
    } MPR;
    char           wk7[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RPAUSE: 8;
            unsigned long : 24;
        } BIT;
    } RFCF;
    union {
        unsigned long LONG;
        struct {
            unsigned long TPAUSE: 16;
            unsigned long : 16;
        } BIT;
    } TPAUSER;
    union {
        unsigned long LONG;
        struct {
            unsigned long TXP: 8;
            unsigned long : 24;
        } BIT;
    } TPAUSECR;
    union {
        unsigned long LONG;
        struct {
            unsigned long BCF: 16;
            unsigned long : 16;
        } BIT;
    } BCFRR;
    char           wk8[80];
    union {
        unsigned long LONG;
        struct {
            unsigned long MAHR: 32;
        } BIT;
    } MAHR;
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MALR: 16;
            unsigned long : 16;
        } BIT;
    } MALR;
    char           wk10[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TROCR: 32;
        } BIT;
    } TROCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CDCR: 32;
        } BIT;
    } CDCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LCCR: 32;
        } BIT;
    } LCCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CNDCR: 32;
        } BIT;
    } CNDCR;
    char           wk11[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CEFCR: 32;
        } BIT;
    } CEFCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long FRECR: 32;
        } BIT;
    } FRECR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TSFRCR: 32;
        } BIT;
    } TSFRCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TLFRCR: 32;
        } BIT;
    } TLFRCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RFCR: 32;
        } BIT;
    } RFCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long MAFCR: 32;
        } BIT;
    } MAFCR;
};

#define ETHERC0   (*(volatile struct st_etherc    *)0xE8204100)
#define ETHERC1   (*(volatile struct st_etherc    *)0xE8204300)

#endif
