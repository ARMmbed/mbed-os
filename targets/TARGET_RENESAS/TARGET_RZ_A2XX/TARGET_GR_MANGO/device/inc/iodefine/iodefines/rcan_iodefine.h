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

#ifndef RCAN_IODEFINE_H
#define RCAN_IODEFINE_H

struct st_rcan {
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
            unsigned long BRP: 10;
            unsigned long : 6;
            unsigned long TSEG1: 4;
            unsigned long TSEG2: 3;
            unsigned long : 1;
            unsigned long SJW: 2;
            unsigned long : 6;
        } BIT;
    } RSCAN0C0CFG;
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
            unsigned char CHMDC: 2;
            unsigned char CSLPR: 1;
            unsigned char RTBO: 1;
            unsigned char : 4;
            unsigned char BEIE: 1;
            unsigned char EWIE: 1;
            unsigned char EPIE: 1;
            unsigned char BOEIE: 1;
            unsigned char BORIE: 1;
            unsigned char OLIE: 1;
            unsigned char BLIE: 1;
            unsigned char ALIE: 1;
            unsigned char TAIE: 1;
            unsigned char : 4;
            unsigned char BOM: 2;
            unsigned char ERRD: 1;
            unsigned char CTME: 1;
            unsigned char CTMS: 2;
            unsigned char : 3;
            unsigned char CRCT: 1;
            unsigned char : 1;
        } BIT;
    } RSCAN0C0CTR;
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
            unsigned char CRSTSTS: 1;
            unsigned char CHLTSTS: 1;
            unsigned char CSLPSTS: 1;
            unsigned char EPSTS: 1;
            unsigned char BOSTS: 1;
            unsigned char TRMSTS: 1;
            unsigned char RECSTS: 1;
            unsigned char COMSTS: 1;
            unsigned char : 8;
            unsigned char REC: 8;
            unsigned char TEC: 8;
        } BIT;
    } RSCAN0C0STS;
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
            unsigned long BEF: 1;
            unsigned long EWF: 1;
            unsigned long EPF: 1;
            unsigned long BOEF: 1;
            unsigned long BORF: 1;
            unsigned long OVLF: 1;
            unsigned long BLF: 1;
            unsigned long ALF: 1;
            unsigned long SERR: 1;
            unsigned long FERR: 1;
            unsigned long AERR: 1;
            unsigned long CERR: 1;
            unsigned long B1ERR: 1;
            unsigned long B0ERR: 1;
            unsigned long ADERR: 1;
            unsigned long : 1;
            unsigned long CRCREG: 15;
            unsigned long : 1;
        } BIT;
    } RSCAN0C0ERFL;
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
            unsigned long BRP: 10;
            unsigned long : 6;
            unsigned long TSEG1: 4;
            unsigned long TSEG2: 3;
            unsigned long : 1;
            unsigned long SJW: 2;
            unsigned long : 6;
        } BIT;
    } RSCAN0C1CFG;
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
            unsigned char CHMDC: 2;
            unsigned char CSLPR: 1;
            unsigned char RTBO: 1;
            unsigned char : 4;
            unsigned char BEIE: 1;
            unsigned char EWIE: 1;
            unsigned char EPIE: 1;
            unsigned char BOEIE: 1;
            unsigned char BORIE: 1;
            unsigned char OLIE: 1;
            unsigned char BLIE: 1;
            unsigned char ALIE: 1;
            unsigned char TAIE: 1;
            unsigned char : 4;
            unsigned char BOM: 2;
            unsigned char ERRD: 1;
            unsigned char CTME: 1;
            unsigned char CTMS: 2;
            unsigned char : 3;
            unsigned char CRCT: 1;
            unsigned char : 1;
        } BIT;
    } RSCAN0C1CTR;
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
            unsigned char CRSTSTS: 1;
            unsigned char CHLTSTS: 1;
            unsigned char CSLPSTS: 1;
            unsigned char EPSTS: 1;
            unsigned char BOSTS: 1;
            unsigned char TRMSTS: 1;
            unsigned char RECSTS: 1;
            unsigned char COMSTS: 1;
            unsigned char : 8;
            unsigned char REC: 8;
            unsigned char TEC: 8;
        } BIT;
    } RSCAN0C1STS;
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
            unsigned long BEF: 1;
            unsigned long EWF: 1;
            unsigned long EPF: 1;
            unsigned long BOEF: 1;
            unsigned long BORF: 1;
            unsigned long OVLF: 1;
            unsigned long BLF: 1;
            unsigned long ALF: 1;
            unsigned long SERR: 1;
            unsigned long FERR: 1;
            unsigned long AERR: 1;
            unsigned long CERR: 1;
            unsigned long B1ERR: 1;
            unsigned long B0ERR: 1;
            unsigned long ADERR: 1;
            unsigned long : 1;
            unsigned long CRCREG: 15;
            unsigned long : 1;
        } BIT;
    } RSCAN0C1ERFL;
    char           wk0[100];
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
            unsigned long TPRI: 1;
            unsigned long DCE: 1;
            unsigned long DRE: 1;
            unsigned long MME: 1;
            unsigned long DCS: 1;
            unsigned long : 2;
            unsigned long TMTSCE: 1;
            unsigned long TSP: 4;
            unsigned long TSSS: 1;
            unsigned long TSBTCS: 3;
            unsigned long ITRCP: 16;
        } BIT;
    } RSCAN0GCFG;
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
            unsigned char GMDC: 2;
            unsigned char GSLPR: 1;
            unsigned char : 5;
            unsigned char DEIE: 1;
            unsigned char MEIE: 1;
            unsigned char THLEIE: 1;
            unsigned char : 5;
            unsigned char TSRST: 1;
            unsigned char : 7;
            unsigned char : 8;
        } BIT;
    } RSCAN0GCTR;
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
            unsigned char GRSTSTS: 1;
            unsigned char GHLTSTS: 1;
            unsigned char GSLPSTS: 1;
            unsigned char GRAMINIT: 1;
            unsigned char : 4;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GSTS;
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
            unsigned char DEF: 1;
            unsigned char MES: 1;
            unsigned char THLES: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GERFL;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned long TS: 16;
            unsigned long : 16;
        } BIT;
    } RSCAN0GTSC;
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
            unsigned char AFLPN: 5;
            unsigned char : 3;
            unsigned char AFLDAE: 1;
            unsigned char : 7;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GAFLECTR;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char RNC1: 8;
            unsigned char RNC0: 8;
        } BIT;
    } RSCAN0GAFLCFG0;
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
            unsigned char NRXMB: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RMNB;
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
            unsigned char RMNS0: 1;
            unsigned char RMNS1: 1;
            unsigned char RMNS2: 1;
            unsigned char RMNS3: 1;
            unsigned char RMNS4: 1;
            unsigned char RMNS5: 1;
            unsigned char RMNS6: 1;
            unsigned char RMNS7: 1;
            unsigned char RMNS8: 1;
            unsigned char RMNS9: 1;
            unsigned char RMNS10: 1;
            unsigned char RMNS11: 1;
            unsigned char RMNS12: 1;
            unsigned char RMNS13: 1;
            unsigned char RMNS14: 1;
            unsigned char RMNS15: 1;
            unsigned char RMNS16: 1;
            unsigned char RMNS17: 1;
            unsigned char RMNS18: 1;
            unsigned char RMNS19: 1;
            unsigned char RMNS20: 1;
            unsigned char RMNS21: 1;
            unsigned char RMNS22: 1;
            unsigned char RMNS23: 1;
            unsigned char RMNS24: 1;
            unsigned char RMNS25: 1;
            unsigned char RMNS26: 1;
            unsigned char RMNS27: 1;
            unsigned char RMNS28: 1;
            unsigned char RMNS29: 1;
            unsigned char RMNS30: 1;
            unsigned char RMNS31: 1;
        } BIT;
    } RSCAN0RMND0;
    char           wk2[12];
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC0;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC1;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC2;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC3;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC4;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC5;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC6;
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
            unsigned char RFE: 1;
            unsigned char RFIE: 1;
            unsigned char : 6;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFCC7;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS0;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS1;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS2;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS3;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS4;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS5;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS6;
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
            unsigned char RFEMP: 1;
            unsigned char RFFLL: 1;
            unsigned char RFMLT: 1;
            unsigned char RFIF: 1;
            unsigned char : 4;
            unsigned char RFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFSTS7;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR0;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR1;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR2;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR3;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR4;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR5;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR6;
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
            unsigned char RFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFPCTR7;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC0;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC1;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC2;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC3;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC4;
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
            unsigned char CFE: 1;
            unsigned char CFRXIE: 1;
            unsigned char CFTXIE: 1;
            unsigned char : 5;
            unsigned char CFDC: 3;
            unsigned char : 1;
            unsigned char CFIM: 1;
            unsigned char CFIGCV: 3;
            unsigned char CFM: 2;
            unsigned char CFITSS: 1;
            unsigned char CFITR: 1;
            unsigned char CFTML: 4;
            unsigned char CFITT: 8;
        } BIT;
    } RSCAN0CFCC5;
    char           wk3[72];
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS0;
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS1;
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS2;
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS3;
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS4;
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
            unsigned char CFEMP: 1;
            unsigned char CFFLL: 1;
            unsigned char CFMLT: 1;
            unsigned char CFRXIF: 1;
            unsigned char CFTXIF: 1;
            unsigned char : 3;
            unsigned char CFMC: 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFSTS5;
    char           wk4[72];
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR0;
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR1;
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR2;
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR3;
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR4;
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
            unsigned char CFPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFPCTR5;
    char           wk5[72];
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
            unsigned char RF0EMP: 1;
            unsigned char RF1EMP: 1;
            unsigned char RF2EMP: 1;
            unsigned char RF3EMP: 1;
            unsigned char RF4EMP: 1;
            unsigned char RF5EMP: 1;
            unsigned char RF6EMP: 1;
            unsigned char RF7EMP: 1;
            unsigned char CF0EMP: 1;
            unsigned char CF1EMP: 1;
            unsigned char CF2EMP: 1;
            unsigned char CF3EMP: 1;
            unsigned char CF4EMP: 1;
            unsigned char CF5EMP: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0FESTS;
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
            unsigned char RF0FLL: 1;
            unsigned char RF1FLL: 1;
            unsigned char RF2FLL: 1;
            unsigned char RF3FLL: 1;
            unsigned char RF4FLL: 1;
            unsigned char RF5FLL: 1;
            unsigned char RF6FLL: 1;
            unsigned char RF7FLL: 1;
            unsigned char CF0FLL: 1;
            unsigned char CF1FLL: 1;
            unsigned char CF2FLL: 1;
            unsigned char CF3FLL: 1;
            unsigned char CF4FLL: 1;
            unsigned char CF5FLL: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0FFSTS;
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
            unsigned char RF0MLT: 1;
            unsigned char RF1MLT: 1;
            unsigned char RF2MLT: 1;
            unsigned char RF3MLT: 1;
            unsigned char RF4MLT: 1;
            unsigned char RF5MLT: 1;
            unsigned char RF6MLT: 1;
            unsigned char RF7MLT: 1;
            unsigned char CF0MLT: 1;
            unsigned char CF1MLT: 1;
            unsigned char CF2MLT: 1;
            unsigned char CF3MLT: 1;
            unsigned char CF4MLT: 1;
            unsigned char CF5MLT: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0FMSTS;
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
            unsigned char RF0IF: 1;
            unsigned char RF1IF: 1;
            unsigned char RF2IF: 1;
            unsigned char RF3IF: 1;
            unsigned char RF4IF: 1;
            unsigned char RF5IF: 1;
            unsigned char RF6IF: 1;
            unsigned char RF7IF: 1;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0RFISTS;
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
            unsigned char CF0RXIF: 1;
            unsigned char CF1RXIF: 1;
            unsigned char CF2RXIF: 1;
            unsigned char CF3RXIF: 1;
            unsigned char CF4RXIF: 1;
            unsigned char CF5RXIF: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFRISTS;
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
            unsigned char CF0TXIF: 1;
            unsigned char CF1TXIF: 1;
            unsigned char CF2TXIF: 1;
            unsigned char CF3TXIF: 1;
            unsigned char CF4TXIF: 1;
            unsigned char CF5TXIF: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0CFTISTS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC2;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC4;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC5;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC6;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC7;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC8;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC9;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC10;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC11;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC12;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC13;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC14;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC15;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC16;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC17;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC18;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC19;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC20;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC21;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC22;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC23;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC24;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC25;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC26;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC27;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC28;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC29;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC30;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCAN0TMC31;
    char           wk6[96];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS2;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS4;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS5;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS6;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS7;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS8;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS9;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS10;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS11;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS12;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS13;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS14;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS15;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS16;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS17;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS18;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS19;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS20;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS21;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS22;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS23;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS24;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS25;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS26;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS27;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS28;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS29;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS30;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCAN0TMSTS31;
    char           wk7[96];
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
            unsigned char TMTRSTS0: 1;
            unsigned char TMTRSTS1: 1;
            unsigned char TMTRSTS2: 1;
            unsigned char TMTRSTS3: 1;
            unsigned char TMTRSTS4: 1;
            unsigned char TMTRSTS5: 1;
            unsigned char TMTRSTS6: 1;
            unsigned char TMTRSTS7: 1;
            unsigned char TMTRSTS8: 1;
            unsigned char TMTRSTS9: 1;
            unsigned char TMTRSTS10: 1;
            unsigned char TMTRSTS11: 1;
            unsigned char TMTRSTS12: 1;
            unsigned char TMTRSTS13: 1;
            unsigned char TMTRSTS14: 1;
            unsigned char TMTRSTS15: 1;
            unsigned char TMTRSTS16: 1;
            unsigned char TMTRSTS17: 1;
            unsigned char TMTRSTS18: 1;
            unsigned char TMTRSTS19: 1;
            unsigned char TMTRSTS20: 1;
            unsigned char TMTRSTS21: 1;
            unsigned char TMTRSTS22: 1;
            unsigned char TMTRSTS23: 1;
            unsigned char TMTRSTS24: 1;
            unsigned char TMTRSTS25: 1;
            unsigned char TMTRSTS26: 1;
            unsigned char TMTRSTS27: 1;
            unsigned char TMTRSTS28: 1;
            unsigned char TMTRSTS29: 1;
            unsigned char TMTRSTS30: 1;
            unsigned char TMTRSTS31: 1;
        } BIT;
    } RSCAN0TMTRSTS0;
    char           wk8[12];
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
            unsigned char TMTARSTS0: 1;
            unsigned char TMTARSTS1: 1;
            unsigned char TMTARSTS2: 1;
            unsigned char TMTARSTS3: 1;
            unsigned char TMTARSTS4: 1;
            unsigned char TMTARSTS5: 1;
            unsigned char TMTARSTS6: 1;
            unsigned char TMTARSTS7: 1;
            unsigned char TMTARSTS8: 1;
            unsigned char TMTARSTS9: 1;
            unsigned char TMTARSTS10: 1;
            unsigned char TMTARSTS11: 1;
            unsigned char TMTARSTS12: 1;
            unsigned char TMTARSTS13: 1;
            unsigned char TMTARSTS14: 1;
            unsigned char TMTARSTS15: 1;
            unsigned char TMTARSTS16: 1;
            unsigned char TMTARSTS17: 1;
            unsigned char TMTARSTS18: 1;
            unsigned char TMTARSTS19: 1;
            unsigned char TMTARSTS20: 1;
            unsigned char TMTARSTS21: 1;
            unsigned char TMTARSTS22: 1;
            unsigned char TMTARSTS23: 1;
            unsigned char TMTARSTS24: 1;
            unsigned char TMTARSTS25: 1;
            unsigned char TMTARSTS26: 1;
            unsigned char TMTARSTS27: 1;
            unsigned char TMTARSTS28: 1;
            unsigned char TMTARSTS29: 1;
            unsigned char TMTARSTS30: 1;
            unsigned char TMTARSTS31: 1;
        } BIT;
    } RSCAN0TMTARSTS0;
    char           wk9[12];
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
            unsigned char TMTCSTS0: 1;
            unsigned char TMTCSTS1: 1;
            unsigned char TMTCSTS2: 1;
            unsigned char TMTCSTS3: 1;
            unsigned char TMTCSTS4: 1;
            unsigned char TMTCSTS5: 1;
            unsigned char TMTCSTS6: 1;
            unsigned char TMTCSTS7: 1;
            unsigned char TMTCSTS8: 1;
            unsigned char TMTCSTS9: 1;
            unsigned char TMTCSTS10: 1;
            unsigned char TMTCSTS11: 1;
            unsigned char TMTCSTS12: 1;
            unsigned char TMTCSTS13: 1;
            unsigned char TMTCSTS14: 1;
            unsigned char TMTCSTS15: 1;
            unsigned char TMTCSTS16: 1;
            unsigned char TMTCSTS17: 1;
            unsigned char TMTCSTS18: 1;
            unsigned char TMTCSTS19: 1;
            unsigned char TMTCSTS20: 1;
            unsigned char TMTCSTS21: 1;
            unsigned char TMTCSTS22: 1;
            unsigned char TMTCSTS23: 1;
            unsigned char TMTCSTS24: 1;
            unsigned char TMTCSTS25: 1;
            unsigned char TMTCSTS26: 1;
            unsigned char TMTCSTS27: 1;
            unsigned char TMTCSTS28: 1;
            unsigned char TMTCSTS29: 1;
            unsigned char TMTCSTS30: 1;
            unsigned char TMTCSTS31: 1;
        } BIT;
    } RSCAN0TMTCSTS0;
    char           wk10[12];
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
            unsigned char TMTASTS0: 1;
            unsigned char TMTASTS1: 1;
            unsigned char TMTASTS2: 1;
            unsigned char TMTASTS3: 1;
            unsigned char TMTASTS4: 1;
            unsigned char TMTASTS5: 1;
            unsigned char TMTASTS6: 1;
            unsigned char TMTASTS7: 1;
            unsigned char TMTASTS8: 1;
            unsigned char TMTASTS9: 1;
            unsigned char TMTASTS10: 1;
            unsigned char TMTASTS11: 1;
            unsigned char TMTASTS12: 1;
            unsigned char TMTASTS13: 1;
            unsigned char TMTASTS14: 1;
            unsigned char TMTASTS15: 1;
            unsigned char TMTASTS16: 1;
            unsigned char TMTASTS17: 1;
            unsigned char TMTASTS18: 1;
            unsigned char TMTASTS19: 1;
            unsigned char TMTASTS20: 1;
            unsigned char TMTASTS21: 1;
            unsigned char TMTASTS22: 1;
            unsigned char TMTASTS23: 1;
            unsigned char TMTASTS24: 1;
            unsigned char TMTASTS25: 1;
            unsigned char TMTASTS26: 1;
            unsigned char TMTASTS27: 1;
            unsigned char TMTASTS28: 1;
            unsigned char TMTASTS29: 1;
            unsigned char TMTASTS30: 1;
            unsigned char TMTASTS31: 1;
        } BIT;
    } RSCAN0TMTASTS0;
    char           wk11[12];
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
            unsigned char TMIE0: 1;
            unsigned char TMIE1: 1;
            unsigned char TMIE2: 1;
            unsigned char TMIE3: 1;
            unsigned char TMIE4: 1;
            unsigned char TMIE5: 1;
            unsigned char TMIE6: 1;
            unsigned char TMIE7: 1;
            unsigned char TMIE8: 1;
            unsigned char TMIE9: 1;
            unsigned char TMIE10: 1;
            unsigned char TMIE11: 1;
            unsigned char TMIE12: 1;
            unsigned char TMIE13: 1;
            unsigned char TMIE14: 1;
            unsigned char TMIE15: 1;
            unsigned char TMIE16: 1;
            unsigned char TMIE17: 1;
            unsigned char TMIE18: 1;
            unsigned char TMIE19: 1;
            unsigned char TMIE20: 1;
            unsigned char TMIE21: 1;
            unsigned char TMIE22: 1;
            unsigned char TMIE23: 1;
            unsigned char TMIE24: 1;
            unsigned char TMIE25: 1;
            unsigned char TMIE26: 1;
            unsigned char TMIE27: 1;
            unsigned char TMIE28: 1;
            unsigned char TMIE29: 1;
            unsigned char TMIE30: 1;
            unsigned char TMIE31: 1;
        } BIT;
    } RSCAN0TMIEC0;
    char           wk12[12];
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
            unsigned char TXQE: 1;
            unsigned char : 7;
            unsigned char TXQDC: 4;
            unsigned char TXQIE: 1;
            unsigned char TXQIM: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQCC0;
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
            unsigned char TXQE: 1;
            unsigned char : 7;
            unsigned char TXQDC: 4;
            unsigned char TXQIE: 1;
            unsigned char TXQIM: 1;
            unsigned char : 2;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQCC1;
    char           wk13[24];
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
            unsigned char TXQEMP: 1;
            unsigned char TXQFLL: 1;
            unsigned char TXQIF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQSTS0;
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
            unsigned char TXQEMP: 1;
            unsigned char TXQFLL: 1;
            unsigned char TXQIF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQSTS1;
    char           wk14[24];
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
            unsigned char TXQPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQPCTR0;
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
            unsigned char TXQPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0TXQPCTR1;
    char           wk15[24];
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
            unsigned char THLE: 1;
            unsigned char : 7;
            unsigned char THLIE: 1;
            unsigned char THLIM: 1;
            unsigned char THLDTE: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLCC0;
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
            unsigned char THLE: 1;
            unsigned char : 7;
            unsigned char THLIE: 1;
            unsigned char THLIM: 1;
            unsigned char THLDTE: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLCC1;
    char           wk16[24];
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
            unsigned char THLEMP: 1;
            unsigned char THLFLL: 1;
            unsigned char THLELT: 1;
            unsigned char THLIF: 1;
            unsigned char : 4;
            unsigned char THLMC: 5;
            unsigned char : 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLSTS0;
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
            unsigned char THLEMP: 1;
            unsigned char THLFLL: 1;
            unsigned char THLELT: 1;
            unsigned char THLIF: 1;
            unsigned char : 4;
            unsigned char THLMC: 5;
            unsigned char : 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLSTS1;
    char           wk17[24];
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
            unsigned char THLPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLPCTR0;
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
            unsigned char THLPC: 8;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0THLPCTR1;
    char           wk18[24];
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
            unsigned char TSIF0: 1;
            unsigned char TAIF0: 1;
            unsigned char TQIF0: 1;
            unsigned char CFTIF0: 1;
            unsigned char THIF0: 1;
            unsigned char : 3;
            unsigned char TSIF1: 1;
            unsigned char TAIF1: 1;
            unsigned char TQIF1: 1;
            unsigned char CFTIF1: 1;
            unsigned char THIF1: 1;
            unsigned char : 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GTINTSTS0;
    char           wk19[4];
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
            unsigned char C0ICBCE: 1;
            unsigned char C1ICBCE: 1;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char RTMPS: 7;
            unsigned char : 1;
            unsigned char : 8;
        } BIT;
    } RSCAN0GTSTCFG;
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
            unsigned char ICBCTME: 1;
            unsigned char : 1;
            unsigned char RTME: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GTSTCTR;
    char           wk20[4];
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
            unsigned char : 8;
            unsigned char TSCCFG: 2;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GFDCFG;
    char           wk21[4];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned long LOCK: 16;
            unsigned long : 16;
        } BIT;
    } RSCAN0GLOCKK;
    char           wk22[124];
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
            unsigned char RCMC: 1;
            unsigned char : 7;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCAN0GRMCFG;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID0;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM0;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_0;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_0;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID1;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM1;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_1;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_1;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID2;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM2;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_2;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_2;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID3;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM3;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_3;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_3;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID4;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM4;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_4;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_4;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID5;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM5;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_5;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_5;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID6;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM6;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_6;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_6;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID7;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM7;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_7;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_7;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID8;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM8;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_8;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_8;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID9;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM9;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_9;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_9;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID10;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM10;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_10;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_10;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID11;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM11;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_11;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_11;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID12;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM12;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_12;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_12;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID13;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM13;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_13;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_13;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID14;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM14;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_14;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_14;
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
            unsigned long GAFLID: 29;
            unsigned long GAFLLB: 1;
            unsigned long GAFLRTR: 1;
            unsigned long GAFLIDE: 1;
        } BIT;
    } RSCAN0GAFLID15;
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
            unsigned long GAFLIDM: 29;
            unsigned long : 1;
            unsigned long GAFLRTRM: 1;
            unsigned long GAFLIDEM: 1;
        } BIT;
    } RSCAN0GAFLM15;
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
            unsigned long : 8;
            unsigned long GAFLRMDP: 7;
            unsigned long GAFLRMV: 1;
            unsigned long GAFLPTR: 12;
            unsigned long GAFLDLC: 4;
        } BIT;
    } RSCAN0GAFLP0_15;
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
            unsigned long GAFLFDP: 14;
            unsigned long : 18;
        } BIT;
    } RSCAN0GAFLP1_15;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID0;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_0;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID1;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_1;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID2;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_2;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID3;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_3;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID4;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_4;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID5;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_5;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID6;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_6;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID7;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_7;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID8;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_8;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID9;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_9;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID10;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_10;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID11;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_11;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID12;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_12;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID13;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_13;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID14;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_14;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID15;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_15;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID16;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_16;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID17;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_17;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID18;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_18;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID19;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_19;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID20;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_20;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID21;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_21;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID22;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_22;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID23;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_23;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID24;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_24;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID25;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_25;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID26;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_26;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID27;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_27;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID28;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_28;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID29;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_29;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID30;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_30;
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
            unsigned long RMID: 29;
            unsigned long : 1;
            unsigned long RMRTR: 1;
            unsigned long RMIDE: 1;
        } BIT;
    } RSCAN0RMID31;
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
            unsigned long RMTS: 16;
            unsigned long RMPTR: 12;
            unsigned long RMDLC: 4;
        } BIT;
    } RSCAN0RMPTR31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB0;
            unsigned char RMDB1;
            unsigned char RMDB2;
            unsigned char RMDB3;
        } BYTE;
    } RSCAN0RMDF0_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB4;
            unsigned char RMDB5;
            unsigned char RMDB6;
            unsigned char RMDB7;
        } BYTE;
    } RSCAN0RMDF1_31;
    char           wk23[1536];
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID0;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_0;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID1;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_1;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID2;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_2;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID3;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_3;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID4;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_4;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID5;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_5;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID6;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_6;
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
            unsigned long RFID: 29;
            unsigned long : 1;
            unsigned long RFRTR: 1;
            unsigned long RFIDE: 1;
        } BIT;
    } RSCAN0RFID7;
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
            unsigned long RFTS: 16;
            unsigned long RFPTR: 12;
            unsigned long RFDLC: 4;
        } BIT;
    } RSCAN0RFPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB0;
            unsigned char RFDB1;
            unsigned char RFDB2;
            unsigned char RFDB3;
        } BYTE;
    } RSCAN0RFDF0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB4;
            unsigned char RFDB5;
            unsigned char RFDB6;
            unsigned char RFDB7;
        } BYTE;
    } RSCAN0RFDF1_7;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID0;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_0;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID1;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_1;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID2;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_2;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID3;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_3;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID4;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_4;
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
            unsigned long CFID: 29;
            unsigned long THLEN: 1;
            unsigned long CFRTR: 1;
            unsigned long CFIDE: 1;
        } BIT;
    } RSCAN0CFID5;
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
            unsigned long CFTS: 16;
            unsigned long CFPTR: 12;
            unsigned long CFDLC: 4;
        } BIT;
    } RSCAN0CFPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB0;
            unsigned char CFDB1;
            unsigned char CFDB2;
            unsigned char CFDB3;
        } BYTE;
    } RSCAN0CFDF0_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB4;
            unsigned char CFDB5;
            unsigned char CFDB6;
            unsigned char CFDB7;
        } BYTE;
    } RSCAN0CFDF1_5;
    char           wk24[288];
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID0;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_0;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID1;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_1;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID2;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_2;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID3;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_3;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID4;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_4;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID5;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_5;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID6;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_6;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID7;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_7;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID8;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_8;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID9;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_9;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID10;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_10;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID11;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_11;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID12;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_12;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID13;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_13;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID14;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_14;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID15;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_15;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID16;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_16;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID17;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_17;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID18;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_18;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID19;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_19;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID20;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_20;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID21;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_21;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID22;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_22;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID23;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_23;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID24;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_24;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID25;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_25;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID26;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_26;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID27;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_27;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID28;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_28;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID29;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_29;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID30;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_30;
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
            unsigned long TMID: 29;
            unsigned long THLEN: 1;
            unsigned long TMRTR: 1;
            unsigned long TMIDE: 1;
        } BIT;
    } RSCAN0TMID31;
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
            unsigned char : 8;
            unsigned char : 8;
            unsigned char TMPTR: 8;
            unsigned char : 4;
            unsigned char TMDLC: 4;
        } BIT;
    } RSCAN0TMPTR31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB0;
            unsigned char TMDB1;
            unsigned char TMDB2;
            unsigned char TMDB3;
        } BYTE;
    } RSCAN0TMDF0_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB4;
            unsigned char TMDB5;
            unsigned char TMDB6;
            unsigned char TMDB7;
        } BYTE;
    } RSCAN0TMDF1_31;
    char           wk25[1536];
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
            unsigned long BT: 3;
            unsigned long BN: 4;
            unsigned long : 1;
            unsigned long TID: 8;
            unsigned long TMTS: 16;
        } BIT;
    } RSCAN0THLACC0;
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
            unsigned long BT: 3;
            unsigned long BN: 4;
            unsigned long : 1;
            unsigned long TID: 8;
            unsigned long TMTS: 16;
        } BIT;
    } RSCAN0THLACC1;
    char           wk26[248];
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC0;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC1;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC2;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC3;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC4;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC5;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC6;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC7;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC8;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC9;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC10;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC11;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC12;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC13;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC14;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC15;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC16;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC17;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC18;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC19;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC20;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC21;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC22;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC23;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC24;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC25;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC26;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC27;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC28;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC29;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC30;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC31;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC32;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC33;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC34;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC35;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC36;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC37;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC38;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC39;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC40;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC41;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC42;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC43;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC44;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC45;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC46;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC47;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC48;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC49;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC50;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC51;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC52;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC53;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC54;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC55;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC56;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC57;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC58;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC59;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC60;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC61;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC62;
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
            unsigned long RDTA: 32;
        } BIT;
    } RSCAN0RPGACC63;
};

#define RCAN      (*(volatile struct st_rcan      *)0xE8020000)

#endif
