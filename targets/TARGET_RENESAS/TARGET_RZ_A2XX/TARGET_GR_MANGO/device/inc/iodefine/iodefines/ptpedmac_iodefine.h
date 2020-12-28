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

#ifndef PTPEDMAC_IODEFINE_H
#define PTPEDMAC_IODEFINE_H

struct st_ptpedmac {
    union {
        unsigned long LONG;
        struct {
            unsigned long SWR: 1;
            unsigned long : 3;
            unsigned long DL: 2;
            unsigned long DE: 1;
            unsigned long : 25;
        } BIT;
    } EDMR;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TR: 1;
            unsigned long : 31;
        } BIT;
    } EDTRR;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RR: 1;
            unsigned long : 31;
        } BIT;
    } EDRRR;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TDLAR: 32;
        } BIT;
    } TDLAR;
    char           wk3[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RDLAR: 32;
        } BIT;
    } RDLAR;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TYPE: 4;
            unsigned long PVER: 1;
            unsigned long : 2;
            unsigned long RPORT: 1;
            unsigned long MACE: 1;
            unsigned long : 7;
            unsigned long RFOF: 1;
            unsigned long RDE: 1;
            unsigned long FR: 1;
            unsigned long TFUF: 1;
            unsigned long TDE: 1;
            unsigned long TC: 1;
            unsigned long : 2;
            unsigned long RFCOF: 1;
            unsigned long : 1;
            unsigned long TABT: 1;
            unsigned long : 3;
            unsigned long TWB: 1;
            unsigned long : 1;
        } BIT;
    } EESR;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long PVERIP: 1;
            unsigned long : 2;
            unsigned long RPORTIP: 1;
            unsigned long MACEIP: 1;
            unsigned long : 7;
            unsigned long RFOFIP: 1;
            unsigned long RDEIP: 1;
            unsigned long FRIP: 1;
            unsigned long TFUFIP: 1;
            unsigned long TDEIP: 1;
            unsigned long TCIP: 1;
            unsigned long : 2;
            unsigned long RFCOFIP: 1;
            unsigned long : 1;
            unsigned long TABTIP: 1;
            unsigned long : 3;
            unsigned long TWBIP: 1;
            unsigned long : 1;
        } BIT;
    } EESIPR;
    char           wk6[4];
    char           wk7[4];
    char           wk8[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MFC: 16;
            unsigned long : 16;
        } BIT;
    } RMFCR;
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TFT: 11;
            unsigned long : 21;
        } BIT;
    } TFTR;
    char           wk10[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RFD: 5;
            unsigned long : 3;
            unsigned long TFD: 5;
            unsigned long : 19;
        } BIT;
    } FDR;
    char           wk11[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RNR: 1;
            unsigned long : 31;
        } BIT;
    } RMCR;
    char           wk12[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long UNDER: 16;
            unsigned long : 16;
        } BIT;
    } TFUCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OVER: 16;
            unsigned long : 16;
        } BIT;
    } RFOCR;
    char           wk13[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RFDO: 3;
            unsigned long : 13;
            unsigned long RFFO: 3;
            unsigned long : 13;
        } BIT;
    } FCFTR;
    char           wk14[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long PADR: 6;
            unsigned long : 10;
            unsigned long PADS: 2;
            unsigned long : 14;
        } BIT;
    } RPADIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TIS: 1;
            unsigned long : 3;
            unsigned long TIM: 1;
            unsigned long : 27;
        } BIT;
    } TRIMD;
    char           wk15[72];
    union {
        unsigned long LONG;
        struct {
            unsigned long RBWAR: 32;
        } BIT;
    } RBWAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RDFAR: 32;
        } BIT;
    } RDFAR;
    char           wk16[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TBRAR: 32;
        } BIT;
    } TBRAR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TDFAR: 32;
        } BIT;
    } TDFAR;
};

#define PTPEDMAC  (*(volatile struct st_ptpedmac  *)0xE8204400)

#endif
