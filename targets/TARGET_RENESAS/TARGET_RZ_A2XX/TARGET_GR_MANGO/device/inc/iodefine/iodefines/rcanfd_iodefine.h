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

#ifndef RCANFD_IODEFINE_H
#define RCANFD_IODEFINE_H

struct st_rcanfd {
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long NBRP: 10;
            unsigned long : 1;
            unsigned long NSJW: 5;
            unsigned long NTSEG1: 7;
            unsigned long : 1;
            unsigned long NTSEG2: 5;
            unsigned long : 3;
        } BIT;
    } RSCFD0CFDC0NCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char EOCOIE: 1;
            unsigned char SOCOIE: 1;
            unsigned char TDCVFIE: 1;
            unsigned char : 1;
            unsigned char BOM: 2;
            unsigned char ERRD: 1;
            unsigned char CTME: 1;
            unsigned char CTMS: 2;
            unsigned char : 3;
            unsigned char CRCT: 1;
            unsigned char ROM: 1;
        } BIT;
    } RSCFD0CFDC0CTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char ESIF: 1;
            unsigned char : 7;
            unsigned char REC: 8;
            unsigned char TEC: 8;
        } BIT;
    } RSCFD0CFDC0STS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDC0ERFL;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long NBRP: 10;
            unsigned long : 1;
            unsigned long NSJW: 5;
            unsigned long NTSEG1: 7;
            unsigned long : 1;
            unsigned long NTSEG2: 5;
            unsigned long : 3;
        } BIT;
    } RSCFD0CFDC1NCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char EOCOIE: 1;
            unsigned char SOCOIE: 1;
            unsigned char TDCVFIE: 1;
            unsigned char : 1;
            unsigned char BOM: 2;
            unsigned char ERRD: 1;
            unsigned char CTME: 1;
            unsigned char CTMS: 2;
            unsigned char : 3;
            unsigned char CRCT: 1;
            unsigned char ROM: 1;
        } BIT;
    } RSCFD0CFDC1CTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char ESIF: 1;
            unsigned char : 7;
            unsigned char REC: 8;
            unsigned char TEC: 8;
        } BIT;
    } RSCFD0CFDC1STS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDC1ERFL;
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
            unsigned long CMPOC: 1;
            unsigned long : 2;
            unsigned long TSP: 4;
            unsigned long TSSS: 1;
            unsigned long TSBTCS: 3;
            unsigned long ITRCP: 16;
        } BIT;
    } RSCFD0CFDGCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char CMPOFIE: 1;
            unsigned char : 4;
            unsigned char TSRST: 1;
            unsigned char : 7;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDGCTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char CMPOF: 1;
            unsigned char : 4;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDGERFL;
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
    } RSCFD0CFDGTSC;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLECTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLCFG0;
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
            unsigned char RMPLS: 2;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMNB;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMND0;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 2;
            unsigned char RFPLS: 3;
            unsigned char : 1;
            unsigned char RFDC: 3;
            unsigned char : 1;
            unsigned char RFIM: 1;
            unsigned char RFIGCV: 3;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFCC7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFSTS7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPCTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
            unsigned char : 1;
            unsigned char CFPLS: 3;
            unsigned char : 1;
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
    } RSCFD0CFDCFCC5;
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
    } RSCFD0CFDCFSTS0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFSTS1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFSTS2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFSTS3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFSTS4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFSTS5;
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
    } RSCFD0CFDCFPCTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPCTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPCTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPCTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPCTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPCTR5;
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
    } RSCFD0CFDFESTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDFFSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDFMSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFISTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFRISTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFTISTS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC2;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC4;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC5;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC6;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC7;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC8;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC9;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC10;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC11;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC12;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC13;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC14;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC15;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC16;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC17;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC18;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC19;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC20;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC21;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC22;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC23;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC24;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC25;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC26;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC27;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC28;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC29;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC30;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTR: 1;
            unsigned char TMTAR: 1;
            unsigned char TMOM: 1;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDTMC31;
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
    } RSCFD0CFDTMSTS0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS2;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS4;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS5;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS6;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS7;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS8;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS9;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS10;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS11;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS12;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS13;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS14;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS15;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS16;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS17;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS18;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS19;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS20;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS21;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS22;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS23;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS24;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS25;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS26;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS27;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS28;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS29;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS30;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TMTSTS: 1;
            unsigned char TMTRF: 2;
            unsigned char TMTRM: 1;
            unsigned char TMTARM: 1;
            unsigned char : 3;
        } BIT;
    } RSCFD0CFDTMSTS31;
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
    } RSCFD0CFDTMTRSTS0;
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
    } RSCFD0CFDTMTARSTS0;
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
    } RSCFD0CFDTMTCSTS0;
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
    } RSCFD0CFDTMTASTS0;
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
    } RSCFD0CFDTMIEC0;
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
    } RSCFD0CFDTXQCC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTXQCC1;
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
    } RSCFD0CFDTXQSTS0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTXQSTS1;
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
    } RSCFD0CFDTXQPCTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTXQPCTR1;
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
    } RSCFD0CFDTHLCC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTHLCC1;
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
    } RSCFD0CFDTHLSTS0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTHLSTS1;
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
    } RSCFD0CFDTHLPCTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTHLPCTR1;
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
    } RSCFD0CFDGTINTSTS0;
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
    } RSCFD0CFDGTSTCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGTSTCTR;
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
            unsigned char RPED: 1;
            unsigned char : 7;
            unsigned char TSCCFG: 2;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDGFDCFG;
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
    } RSCFD0CFDGLOCKK;
    char           wk22[16];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFDMAE0: 1;
            unsigned char RFDMAE1: 1;
            unsigned char RFDMAE2: 1;
            unsigned char RFDMAE3: 1;
            unsigned char RFDMAE4: 1;
            unsigned char RFDMAE5: 1;
            unsigned char RFDMAE6: 1;
            unsigned char RFDMAE7: 1;
            unsigned char CFDMAE0: 1;
            unsigned char CFDMAE1: 1;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCDTCT;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFDMASTS0: 1;
            unsigned char RFDMASTS1: 1;
            unsigned char RFDMASTS2: 1;
            unsigned char RFDMASTS3: 1;
            unsigned char RFDMASTS4: 1;
            unsigned char RFDMASTS5: 1;
            unsigned char RFDMASTS6: 1;
            unsigned char RFDMASTS7: 1;
            unsigned char CFDMASTS0: 1;
            unsigned char CFDMASTS1: 1;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCDTSTS;
    char           wk23[100];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGRMCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char DBRP: 8;
            unsigned char : 8;
            unsigned char DTSEG1: 4;
            unsigned char DTSEG2: 3;
            unsigned char : 1;
            unsigned char DSJW: 3;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDC0DCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char EOCCFG: 3;
            unsigned char : 5;
            unsigned char TDCOC: 1;
            unsigned char TDCE: 1;
            unsigned char ESIC: 1;
            unsigned char : 5;
            unsigned char TDCO: 7;
            unsigned char : 1;
            unsigned char GWEN: 1;
            unsigned char GWFDF: 1;
            unsigned char GWBRS: 1;
            unsigned char TMME: 1;
            unsigned char FDOE: 1;
            unsigned char REFE: 1;
            unsigned char : 2;
        } BIT;
    } RSCFD0CFDC0FDCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char EOCCLR: 1;
            unsigned char SOCCLR: 1;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDC0FDCTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TDCR: 7;
            unsigned char TDCVF: 1;
            unsigned char EOCO: 1;
            unsigned char SOCO: 1;
            unsigned char : 6;
            unsigned char EOC: 8;
            unsigned char SOC: 8;
        } BIT;
    } RSCFD0CFDC0FDSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long CRCREG: 21;
            unsigned long : 3;
            unsigned long SCNT: 4;
            unsigned long : 4;
        } BIT;
    } RSCFD0CFDC0FDCRC;
    char           wk24[12];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char DBRP: 8;
            unsigned char : 8;
            unsigned char DTSEG1: 4;
            unsigned char DTSEG2: 3;
            unsigned char : 1;
            unsigned char DSJW: 3;
            unsigned char : 5;
        } BIT;
    } RSCFD0CFDC1DCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char EOCCFG: 3;
            unsigned char : 5;
            unsigned char TDCOC: 1;
            unsigned char TDCE: 1;
            unsigned char ESIC: 1;
            unsigned char : 5;
            unsigned char TDCO: 7;
            unsigned char : 1;
            unsigned char GWEN: 1;
            unsigned char GWFDF: 1;
            unsigned char GWBRS: 1;
            unsigned char TMME: 1;
            unsigned char FDOE: 1;
            unsigned char REFE: 1;
            unsigned char : 2;
        } BIT;
    } RSCFD0CFDC1FDCFG;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char EOCCLR: 1;
            unsigned char SOCCLR: 1;
            unsigned char : 6;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDC1FDCTR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TDCR: 7;
            unsigned char TDCVF: 1;
            unsigned char EOCO: 1;
            unsigned char SOCO: 1;
            unsigned char : 6;
            unsigned char EOC: 8;
            unsigned char SOC: 8;
        } BIT;
    } RSCFD0CFDC1FDSTS;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long CRCREG: 21;
            unsigned long : 3;
            unsigned long SCNT: 4;
            unsigned long : 4;
        } BIT;
    } RSCFD0CFDC1FDCRC;
    char           wk25[2764];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLID15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLM15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP0_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDGAFLP1_15;
    char           wk26[3840];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS0;
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
    } RSCFD0CFDRMDF0_0;
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
    } RSCFD0CFDRMDF1_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS1;
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
    } RSCFD0CFDRMDF0_1;
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
    } RSCFD0CFDRMDF1_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS2;
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
    } RSCFD0CFDRMDF0_2;
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
    } RSCFD0CFDRMDF1_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS3;
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
    } RSCFD0CFDRMDF0_3;
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
    } RSCFD0CFDRMDF1_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS4;
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
    } RSCFD0CFDRMDF0_4;
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
    } RSCFD0CFDRMDF1_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS5;
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
    } RSCFD0CFDRMDF0_5;
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
    } RSCFD0CFDRMDF1_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS6;
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
    } RSCFD0CFDRMDF0_6;
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
    } RSCFD0CFDRMDF1_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS7;
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
    } RSCFD0CFDRMDF0_7;
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
    } RSCFD0CFDRMDF1_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS8;
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
    } RSCFD0CFDRMDF0_8;
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
    } RSCFD0CFDRMDF1_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS9;
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
    } RSCFD0CFDRMDF0_9;
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
    } RSCFD0CFDRMDF1_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS10;
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
    } RSCFD0CFDRMDF0_10;
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
    } RSCFD0CFDRMDF1_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS11;
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
    } RSCFD0CFDRMDF0_11;
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
    } RSCFD0CFDRMDF1_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS12;
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
    } RSCFD0CFDRMDF0_12;
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
    } RSCFD0CFDRMDF1_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS13;
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
    } RSCFD0CFDRMDF0_13;
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
    } RSCFD0CFDRMDF1_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS14;
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
    } RSCFD0CFDRMDF0_14;
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
    } RSCFD0CFDRMDF1_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS15;
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
    } RSCFD0CFDRMDF0_15;
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
    } RSCFD0CFDRMDF1_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS16;
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
    } RSCFD0CFDRMDF0_16;
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
    } RSCFD0CFDRMDF1_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS17;
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
    } RSCFD0CFDRMDF0_17;
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
    } RSCFD0CFDRMDF1_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS18;
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
    } RSCFD0CFDRMDF0_18;
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
    } RSCFD0CFDRMDF1_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS19;
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
    } RSCFD0CFDRMDF0_19;
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
    } RSCFD0CFDRMDF1_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS20;
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
    } RSCFD0CFDRMDF0_20;
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
    } RSCFD0CFDRMDF1_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS21;
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
    } RSCFD0CFDRMDF0_21;
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
    } RSCFD0CFDRMDF1_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS22;
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
    } RSCFD0CFDRMDF0_22;
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
    } RSCFD0CFDRMDF1_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS23;
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
    } RSCFD0CFDRMDF0_23;
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
    } RSCFD0CFDRMDF1_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS24;
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
    } RSCFD0CFDRMDF0_24;
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
    } RSCFD0CFDRMDF1_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS25;
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
    } RSCFD0CFDRMDF0_25;
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
    } RSCFD0CFDRMDF1_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS26;
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
    } RSCFD0CFDRMDF0_26;
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
    } RSCFD0CFDRMDF1_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS27;
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
    } RSCFD0CFDRMDF0_27;
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
    } RSCFD0CFDRMDF1_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS28;
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
    } RSCFD0CFDRMDF0_28;
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
    } RSCFD0CFDRMDF1_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS29;
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
    } RSCFD0CFDRMDF0_29;
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
    } RSCFD0CFDRMDF1_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS30;
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
    } RSCFD0CFDRMDF0_30;
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
    } RSCFD0CFDRMDF1_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMID31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRMPTR31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RMESI: 1;
            unsigned char RMBRS: 1;
            unsigned char RMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRMFDSTS31;
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
    } RSCFD0CFDRMDF0_31;
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
    } RSCFD0CFDRMDF1_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB8;
            unsigned char RMDB9;
            unsigned char RMDB10;
            unsigned char RMDB11;
        } BYTE;
    } RSCFD0CFDRMDF2_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB12;
            unsigned char RMDB13;
            unsigned char RMDB14;
            unsigned char RMDB15;
        } BYTE;
    } RSCFD0CFDRMDF3_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RMDB16;
            unsigned char RMDB17;
            unsigned char RMDB18;
            unsigned char RMDB19;
        } BYTE;
    } RSCFD0CFDRMDF4_31;
    char           wk27[3072];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS0;
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
    } RSCFD0CFDRFDF0_0;
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
    } RSCFD0CFDRFDF1_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_0;
    char           wk28[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS1;
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
    } RSCFD0CFDRFDF0_1;
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
    } RSCFD0CFDRFDF1_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_1;
    char           wk29[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS2;
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
    } RSCFD0CFDRFDF0_2;
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
    } RSCFD0CFDRFDF1_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_2;
    char           wk30[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS3;
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
    } RSCFD0CFDRFDF0_3;
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
    } RSCFD0CFDRFDF1_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_3;
    char           wk31[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS4;
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
    } RSCFD0CFDRFDF0_4;
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
    } RSCFD0CFDRFDF1_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_4;
    char           wk32[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS5;
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
    } RSCFD0CFDRFDF0_5;
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
    } RSCFD0CFDRFDF1_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_5;
    char           wk33[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS6;
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
    } RSCFD0CFDRFDF0_6;
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
    } RSCFD0CFDRFDF1_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_6;
    char           wk34[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFID7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRFPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char RFESI: 1;
            unsigned char RFBRS: 1;
            unsigned char RFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDRFFDSTS7;
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
    } RSCFD0CFDRFDF0_7;
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
    } RSCFD0CFDRFDF1_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB8;
            unsigned char RFDB9;
            unsigned char RFDB10;
            unsigned char RFDB11;
        } BYTE;
    } RSCFD0CFDRFDF2_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB12;
            unsigned char RFDB13;
            unsigned char RFDB14;
            unsigned char RFDB15;
        } BYTE;
    } RSCFD0CFDRFDF3_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB16;
            unsigned char RFDB17;
            unsigned char RFDB18;
            unsigned char RFDB19;
        } BYTE;
    } RSCFD0CFDRFDF4_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB20;
            unsigned char RFDB21;
            unsigned char RFDB22;
            unsigned char RFDB23;
        } BYTE;
    } RSCFD0CFDRFDF5_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB24;
            unsigned char RFDB25;
            unsigned char RFDB26;
            unsigned char RFDB27;
        } BYTE;
    } RSCFD0CFDRFDF6_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB28;
            unsigned char RFDB29;
            unsigned char RFDB30;
            unsigned char RFDB31;
        } BYTE;
    } RSCFD0CFDRFDF7_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB32;
            unsigned char RFDB33;
            unsigned char RFDB34;
            unsigned char RFDB35;
        } BYTE;
    } RSCFD0CFDRFDF8_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB36;
            unsigned char RFDB37;
            unsigned char RFDB38;
            unsigned char RFDB39;
        } BYTE;
    } RSCFD0CFDRFDF9_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB40;
            unsigned char RFDB41;
            unsigned char RFDB42;
            unsigned char RFDB43;
        } BYTE;
    } RSCFD0CFDRFDF10_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB44;
            unsigned char RFDB45;
            unsigned char RFDB46;
            unsigned char RFDB47;
        } BYTE;
    } RSCFD0CFDRFDF11_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB48;
            unsigned char RFDB49;
            unsigned char RFDB50;
            unsigned char RFDB51;
        } BYTE;
    } RSCFD0CFDRFDF12_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB52;
            unsigned char RFDB53;
            unsigned char RFDB54;
            unsigned char RFDB55;
        } BYTE;
    } RSCFD0CFDRFDF13_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB56;
            unsigned char RFDB57;
            unsigned char RFDB58;
            unsigned char RFDB59;
        } BYTE;
    } RSCFD0CFDRFDF14_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char RFDB60;
            unsigned char RFDB61;
            unsigned char RFDB62;
            unsigned char RFDB63;
        } BYTE;
    } RSCFD0CFDRFDF15_7;
    char           wk35[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS0;
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
    } RSCFD0CFDCFDF0_0;
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
    } RSCFD0CFDCFDF1_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_0;
    char           wk36[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS1;
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
    } RSCFD0CFDCFDF0_1;
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
    } RSCFD0CFDCFDF1_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_1;
    char           wk37[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS2;
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
    } RSCFD0CFDCFDF0_2;
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
    } RSCFD0CFDCFDF1_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_2;
    char           wk38[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS3;
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
    } RSCFD0CFDCFDF0_3;
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
    } RSCFD0CFDCFDF1_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_3;
    char           wk39[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS4;
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
    } RSCFD0CFDCFDF0_4;
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
    } RSCFD0CFDCFDF1_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_4;
    char           wk40[52];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFID5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDCFPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char CFESI: 1;
            unsigned char CFBRS: 1;
            unsigned char CFFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDCFFDCSTS5;
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
    } RSCFD0CFDCFDF0_5;
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
    } RSCFD0CFDCFDF1_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB8;
            unsigned char CFDB9;
            unsigned char CFDB10;
            unsigned char CFDB11;
        } BYTE;
    } RSCFD0CFDCFDF2_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB12;
            unsigned char CFDB13;
            unsigned char CFDB14;
            unsigned char CFDB15;
        } BYTE;
    } RSCFD0CFDCFDF3_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB16;
            unsigned char CFDB17;
            unsigned char CFDB18;
            unsigned char CFDB19;
        } BYTE;
    } RSCFD0CFDCFDF4_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB20;
            unsigned char CFDB21;
            unsigned char CFDB22;
            unsigned char CFDB23;
        } BYTE;
    } RSCFD0CFDCFDF5_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB24;
            unsigned char CFDB25;
            unsigned char CFDB26;
            unsigned char CFDB27;
        } BYTE;
    } RSCFD0CFDCFDF6_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB28;
            unsigned char CFDB29;
            unsigned char CFDB30;
            unsigned char CFDB31;
        } BYTE;
    } RSCFD0CFDCFDF7_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB32;
            unsigned char CFDB33;
            unsigned char CFDB34;
            unsigned char CFDB35;
        } BYTE;
    } RSCFD0CFDCFDF8_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB36;
            unsigned char CFDB37;
            unsigned char CFDB38;
            unsigned char CFDB39;
        } BYTE;
    } RSCFD0CFDCFDF9_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB40;
            unsigned char CFDB41;
            unsigned char CFDB42;
            unsigned char CFDB43;
        } BYTE;
    } RSCFD0CFDCFDF10_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB44;
            unsigned char CFDB45;
            unsigned char CFDB46;
            unsigned char CFDB47;
        } BYTE;
    } RSCFD0CFDCFDF11_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB48;
            unsigned char CFDB49;
            unsigned char CFDB50;
            unsigned char CFDB51;
        } BYTE;
    } RSCFD0CFDCFDF12_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB52;
            unsigned char CFDB53;
            unsigned char CFDB54;
            unsigned char CFDB55;
        } BYTE;
    } RSCFD0CFDCFDF13_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB56;
            unsigned char CFDB57;
            unsigned char CFDB58;
            unsigned char CFDB59;
        } BYTE;
    } RSCFD0CFDCFDF14_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char CFDB60;
            unsigned char CFDB61;
            unsigned char CFDB62;
            unsigned char CFDB63;
        } BYTE;
    } RSCFD0CFDCFDF15_5;
    char           wk41[2356];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR0;
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
    } RSCFD0CFDTMDF0_0;
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
    } RSCFD0CFDTMDF1_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR1;
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
    } RSCFD0CFDTMDF0_1;
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
    } RSCFD0CFDTMDF1_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR2;
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
    } RSCFD0CFDTMDF0_2;
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
    } RSCFD0CFDTMDF1_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR3;
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
    } RSCFD0CFDTMDF0_3;
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
    } RSCFD0CFDTMDF1_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR4;
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
    } RSCFD0CFDTMDF0_4;
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
    } RSCFD0CFDTMDF1_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR5;
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
    } RSCFD0CFDTMDF0_5;
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
    } RSCFD0CFDTMDF1_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR6;
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
    } RSCFD0CFDTMDF0_6;
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
    } RSCFD0CFDTMDF1_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR7;
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
    } RSCFD0CFDTMDF0_7;
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
    } RSCFD0CFDTMDF1_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR8;
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
    } RSCFD0CFDTMDF0_8;
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
    } RSCFD0CFDTMDF1_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR9;
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
    } RSCFD0CFDTMDF0_9;
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
    } RSCFD0CFDTMDF1_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR10;
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
    } RSCFD0CFDTMDF0_10;
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
    } RSCFD0CFDTMDF1_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR11;
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
    } RSCFD0CFDTMDF0_11;
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
    } RSCFD0CFDTMDF1_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR12;
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
    } RSCFD0CFDTMDF0_12;
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
    } RSCFD0CFDTMDF1_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR13;
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
    } RSCFD0CFDTMDF0_13;
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
    } RSCFD0CFDTMDF1_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR14;
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
    } RSCFD0CFDTMDF0_14;
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
    } RSCFD0CFDTMDF1_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR15;
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
    } RSCFD0CFDTMDF0_15;
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
    } RSCFD0CFDTMDF1_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR16;
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
    } RSCFD0CFDTMDF0_16;
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
    } RSCFD0CFDTMDF1_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR17;
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
    } RSCFD0CFDTMDF0_17;
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
    } RSCFD0CFDTMDF1_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR18;
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
    } RSCFD0CFDTMDF0_18;
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
    } RSCFD0CFDTMDF1_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR19;
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
    } RSCFD0CFDTMDF0_19;
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
    } RSCFD0CFDTMDF1_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR20;
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
    } RSCFD0CFDTMDF0_20;
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
    } RSCFD0CFDTMDF1_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR21;
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
    } RSCFD0CFDTMDF0_21;
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
    } RSCFD0CFDTMDF1_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR22;
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
    } RSCFD0CFDTMDF0_22;
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
    } RSCFD0CFDTMDF1_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR23;
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
    } RSCFD0CFDTMDF0_23;
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
    } RSCFD0CFDTMDF1_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR24;
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
    } RSCFD0CFDTMDF0_24;
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
    } RSCFD0CFDTMDF1_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR25;
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
    } RSCFD0CFDTMDF0_25;
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
    } RSCFD0CFDTMDF1_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR26;
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
    } RSCFD0CFDTMDF0_26;
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
    } RSCFD0CFDTMDF1_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR27;
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
    } RSCFD0CFDTMDF0_27;
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
    } RSCFD0CFDTMDF1_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR28;
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
    } RSCFD0CFDTMDF0_28;
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
    } RSCFD0CFDTMDF1_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR29;
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
    } RSCFD0CFDTMDF0_29;
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
    } RSCFD0CFDTMDF1_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR30;
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
    } RSCFD0CFDTMDF0_30;
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
    } RSCFD0CFDTMDF1_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMID31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTMPTR31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char TMESI: 1;
            unsigned char TMBRS: 1;
            unsigned char TMFDF: 1;
            unsigned char : 5;
            unsigned char : 8;
            unsigned char : 8;
            unsigned char : 8;
        } BIT;
    } RSCFD0CFDTMFDCTR31;
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
    } RSCFD0CFDTMDF0_31;
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
    } RSCFD0CFDTMDF1_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB8;
            unsigned char TMDB9;
            unsigned char TMDB10;
            unsigned char TMDB11;
        } BYTE;
    } RSCFD0CFDTMDF2_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB12;
            unsigned char TMDB13;
            unsigned char TMDB14;
            unsigned char TMDB15;
        } BYTE;
    } RSCFD0CFDTMDF3_31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char TMDB16;
            unsigned char TMDB17;
            unsigned char TMDB18;
            unsigned char TMDB19;
        } BYTE;
    } RSCFD0CFDTMDF4_31;
    char           wk42[7168];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTHLACC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDTHLACC1;
    char           wk43[1016];
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC0;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC1;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC2;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC3;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC4;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC5;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC6;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC7;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC8;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC9;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC10;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC11;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC12;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC13;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC14;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC15;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC16;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC17;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC18;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC19;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC20;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC21;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC22;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC23;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC24;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC25;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC26;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC27;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC28;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC29;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC30;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC31;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC32;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC33;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC34;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC35;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC36;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC37;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC38;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC39;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC40;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC41;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC42;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC43;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC44;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC45;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC46;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC47;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC48;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC49;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC50;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC51;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC52;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC53;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC54;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC55;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC56;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC57;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC58;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC59;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC60;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC61;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC62;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
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
    } RSCFD0CFDRPGACC63;
};

#define RCANFD    (*(volatile struct st_rcanfd    *)0xE8020000)

#endif
