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

#ifndef MTU_IODEFINE_H
#define MTU_IODEFINE_H

struct st_mtu {
    union {
        unsigned char BYTE;
        struct {
            unsigned char OE3B: 1;
            unsigned char OE4A: 1;
            unsigned char OE4B: 1;
            unsigned char OE3D: 1;
            unsigned char OE4C: 1;
            unsigned char OE4D: 1;
            unsigned char : 2;
        } BIT;
    } TOERA;
    char           wk0[1];
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char UF: 1;
            unsigned char VF: 1;
            unsigned char WF: 1;
            unsigned char FB: 1;
            unsigned char P: 1;
            unsigned char N: 1;
            unsigned char BDC: 1;
            unsigned char : 1;
        } BIT;
    } TGCRA;
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLSP: 1;
            unsigned char OLSN: 1;
            unsigned char TOCS: 1;
            unsigned char TOCL: 1;
            unsigned char : 2;
            unsigned char PSYE: 1;
            unsigned char : 1;
        } BIT;
    } TOCR1A;
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLS1P: 1;
            unsigned char OLS1N: 1;
            unsigned char OLS2P: 1;
            unsigned char OLS2N: 1;
            unsigned char OLS3P: 1;
            unsigned char OLS3N: 1;
            unsigned char BF: 2;
        } BIT;
    } TOCR2A;
    char           wk2[4];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCDRA: 16;
        } BIT;
    } TCDRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TDDRA: 16;
        } BIT;
    } TDDRA;
    char           wk3[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNTSA: 16;
        } BIT;
    } TCNTSA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TCBRA: 16;
        } BIT;
    } TCBRA;
    char           wk4[12];
    union {
        unsigned char BYTE;
        struct {
            unsigned char T4VCOR: 3;
            unsigned char T4VEN: 1;
            unsigned char T3ACOR: 3;
            unsigned char T3AEN: 1;
        } BIT;
    } TITCR1A;
    union {
        unsigned char BYTE;
        struct {
            unsigned char T4VCNT: 3;
            unsigned char : 1;
            unsigned char T3ACNT: 3;
            unsigned char : 1;
        } BIT;
    } TITCNT1A;
    union {
        unsigned char BYTE;
        struct {
            unsigned char BTE: 2;
            unsigned char : 6;
        } BIT;
    } TBTERA;
    char           wk5[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TDER: 1;
            unsigned char : 7;
        } BIT;
    } TDERA;
    char           wk6[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLS1P: 1;
            unsigned char OLS1N: 1;
            unsigned char OLS2P: 1;
            unsigned char OLS2N: 1;
            unsigned char OLS3P: 1;
            unsigned char OLS3N: 1;
            unsigned char : 2;
        } BIT;
    } TOLBRA;
    char           wk7[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TITM: 1;
            unsigned char : 7;
        } BIT;
    } TITMRA;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TRG4COR: 3;
            unsigned char : 5;
        } BIT;
    } TITCR2A;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TRG4CNT: 3;
            unsigned char : 5;
        } BIT;
    } TITCNT2A;
    char           wk8[35];
    union {
        unsigned char BYTE;
        struct {
            unsigned char WRE: 1;
            unsigned char SCC: 1;
            unsigned char : 5;
            unsigned char CCE: 1;
        } BIT;
    } TWCRA;
    char           wk9[15];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DRS: 1;
            unsigned char : 7;
        } BIT;
    } TMDR2A;
    char           wk10[15];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CST0: 1;
            unsigned char CST1: 1;
            unsigned char CST2: 1;
            unsigned char CST8: 1;
            unsigned char : 2;
            unsigned char CST3: 1;
            unsigned char CST4: 1;
        } BIT;
    } TSTRA;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SYNC0: 1;
            unsigned char SYNC1: 1;
            unsigned char SYNC2: 1;
            unsigned char : 3;
            unsigned char SYNC3: 1;
            unsigned char SYNC4: 1;
        } BIT;
    } TSYRA;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SCH7: 1;
            unsigned char SCH6: 1;
            unsigned char : 1;
            unsigned char SCH4: 1;
            unsigned char SCH3: 1;
            unsigned char SCH2: 1;
            unsigned char SCH1: 1;
            unsigned char SCH0: 1;
        } BIT;
    } TCSYSTR;
    char           wk11[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RWE: 1;
            unsigned char : 7;
        } BIT;
    } TRWERA;
    char           wk12[1925];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OE6B: 1;
            unsigned char OE7A: 1;
            unsigned char OE7B: 1;
            unsigned char OE6D: 1;
            unsigned char OE7C: 1;
            unsigned char OE7D: 1;
            unsigned char : 2;
        } BIT;
    } TOERB;
    char           wk13[1];
    char           wk14[1];
    char           wk15[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLSP: 1;
            unsigned char OLSN: 1;
            unsigned char TOCS: 1;
            unsigned char TOCL: 1;
            unsigned char : 2;
            unsigned char PSYE: 1;
            unsigned char : 1;
        } BIT;
    } TOCR1B;
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLS1P: 1;
            unsigned char OLS1N: 1;
            unsigned char OLS2P: 1;
            unsigned char OLS2N: 1;
            unsigned char OLS3P: 1;
            unsigned char OLS3N: 1;
            unsigned char BF: 2;
        } BIT;
    } TOCR2B;
    char           wk16[4];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCDRB: 16;
        } BIT;
    } TCDRB;
    union {
        unsigned short WORD;
        struct {
            unsigned short TDDRB: 16;
        } BIT;
    } TDDRB;
    char           wk17[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNTSB: 16;
        } BIT;
    } TCNTSB;
    union {
        unsigned short WORD;
        struct {
            unsigned short TCBRB: 16;
        } BIT;
    } TCBRB;
    char           wk18[12];
    union {
        unsigned char BYTE;
        struct {
            unsigned char T7VCOR: 3;
            unsigned char T7VEN: 1;
            unsigned char T6ACOR: 3;
            unsigned char T6AEN: 1;
        } BIT;
    } TITCR1B;
    union {
        unsigned char BYTE;
        struct {
            unsigned char T7VCNT: 3;
            unsigned char : 1;
            unsigned char T6ACNT: 3;
            unsigned char : 1;
        } BIT;
    } TITCNT1B;
    union {
        unsigned char BYTE;
        struct {
            unsigned char BTE: 2;
            unsigned char : 6;
        } BIT;
    } TBTERB;
    char           wk19[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TDER: 1;
            unsigned char : 7;
        } BIT;
    } TDERB;
    char           wk20[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OLS1P: 1;
            unsigned char OLS1N: 1;
            unsigned char OLS2P: 1;
            unsigned char OLS2N: 1;
            unsigned char OLS3P: 1;
            unsigned char OLS3N: 1;
            unsigned char : 2;
        } BIT;
    } TOLBRB;
    char           wk21[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TITM: 1;
            unsigned char : 7;
        } BIT;
    } TITMRB;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TRG7COR: 3;
            unsigned char : 5;
        } BIT;
    } TITCR2B;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TRG7CNT: 3;
            unsigned char : 5;
        } BIT;
    } TITCNT2B;
    char           wk22[35];
    union {
        unsigned char BYTE;
        struct {
            unsigned char WRE: 1;
            unsigned char SCC: 1;
            unsigned char : 5;
            unsigned char CCE: 1;
        } BIT;
    } TWCRB;
    char           wk23[15];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DRS: 1;
            unsigned char : 7;
        } BIT;
    } TMDR2B;
    char           wk24[15];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 6;
            unsigned char CST6: 1;
            unsigned char CST7: 1;
        } BIT;
    } TSTRB;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 6;
            unsigned char SYNC6: 1;
            unsigned char SYNC7: 1;
        } BIT;
    } TSYRB;
    char           wk25[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RWE: 1;
            unsigned char : 7;
        } BIT;
    } TRWERB;
};

struct st_mtu0 {
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR0;
    char           wk0[8];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCRC;
    char           wk1[102];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char BFE: 1;
            unsigned char : 1;
        } BIT;
    } TMDR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 2;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    char           wk2[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRC: 16;
        } BIT;
    } TGRC;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRD: 16;
        } BIT;
    } TGRD;
    char           wk3[16];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRE: 16;
        } BIT;
    } TGRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRF: 16;
        } BIT;
    } TGRF;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEE: 1;
            unsigned char TGIEF: 1;
            unsigned char : 5;
            unsigned char TTGE2: 1;
        } BIT;
    } TIER2;
    char           wk4[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TTSA: 1;
            unsigned char TTSB: 1;
            unsigned char TTSE: 1;
            unsigned char : 5;
        } BIT;
    } TBTM;
    char           wk5[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
};

struct st_mtu1 {
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR1;
    char           wk1[238];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char : 4;
        } BIT;
    } TMDR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIOR;
    char           wk2[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char : 2;
            unsigned char TCIEV: 1;
            unsigned char TCIEU: 1;
            unsigned char : 1;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    char           wk3[4];
    union {
        unsigned char BYTE;
        struct {
            unsigned char I1AE: 1;
            unsigned char I1BE: 1;
            unsigned char I2AE: 1;
            unsigned char I2BE: 1;
            unsigned char : 4;
        } BIT;
    } TICCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char LWA: 1;
            unsigned char PHCKSEL: 1;
            unsigned char : 6;
        } BIT;
    } TMDR3;
    char           wk4[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char PCB: 2;
            unsigned char : 3;
        } BIT;
    } TCR2;
    char           wk5[11];
    union {
        unsigned long LONG;
        struct {
            unsigned long TCNTLW: 32;
        } BIT;
    } TCNTLW;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRALW: 32;
        } BIT;
    } TGRALW;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRBLW: 32;
        } BIT;
    } TGRBLW;
};

struct st_mtu2 {
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR2;
    char           wk0[365];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char : 4;
        } BIT;
    } TMDR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIOR;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char : 2;
            unsigned char TCIEV: 1;
            unsigned char TCIEU: 1;
            unsigned char : 1;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char PCB: 2;
            unsigned char : 3;
        } BIT;
    } TCR2;
};

struct st_mtu3 {
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char : 2;
        } BIT;
    } TMDR1;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    char           wk2[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 2;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    char           wk3[7];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    char           wk4[6];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    char           wk5[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRC: 16;
        } BIT;
    } TGRC;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRD: 16;
        } BIT;
    } TGRD;
    char           wk6[4];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    char           wk7[11];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TTSA: 1;
            unsigned char TTSB: 1;
            unsigned char : 6;
        } BIT;
    } TBTM;
    char           wk8[19];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
    char           wk9[37];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRE: 16;
        } BIT;
    } TGRE;
    char           wk10[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR3;
};

struct st_mtu4 {
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char : 2;
        } BIT;
    } TMDR1;
    char           wk2[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    char           wk3[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 1;
            unsigned char TTGE2: 1;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    char           wk4[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    char           wk5[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    char           wk6[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRC: 16;
        } BIT;
    } TGRC;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRD: 16;
        } BIT;
    } TGRD;
    char           wk7[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    char           wk8[11];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TTSA: 1;
            unsigned char TTSB: 1;
            unsigned char : 6;
        } BIT;
    } TBTM;
    char           wk9[6];
    union {
        unsigned short WORD;
        struct {
            unsigned short ITB4VE: 1;
            unsigned short ITB3AE: 1;
            unsigned short ITA4VE: 1;
            unsigned short ITA3AE: 1;
            unsigned short DT4BE: 1;
            unsigned short UT4BE: 1;
            unsigned short DT4AE: 1;
            unsigned short UT4AE: 1;
            unsigned short : 6;
            unsigned short BF: 2;
        } BIT;
    } TADCR;
    char           wk10[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCORA: 16;
        } BIT;
    } TADCORA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCORB: 16;
        } BIT;
    } TADCORB;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCOBRA: 16;
        } BIT;
    } TADCOBRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCOBRB: 16;
        } BIT;
    } TADCOBRB;
    char           wk11[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
    char           wk12[38];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRE: 16;
        } BIT;
    } TGRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRF: 16;
        } BIT;
    } TGRF;
    char           wk13[28];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR4;
};

struct st_mtu5 {
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFUEN: 1;
            unsigned char NFVEN: 1;
            unsigned char NFWEN: 1;
            unsigned char : 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR5;
    char           wk1[490];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNTU: 16;
        } BIT;
    } TCNTU;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRU: 16;
        } BIT;
    } TGRU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 2;
            unsigned char : 6;
        } BIT;
    } TCRU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char CKEG: 2;
            unsigned char : 3;
        } BIT;
    } TCR2U;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 5;
            unsigned char : 3;
        } BIT;
    } TIORU;
    char           wk2[9];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNTV: 16;
        } BIT;
    } TCNTV;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRV: 16;
        } BIT;
    } TGRV;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 2;
            unsigned char : 6;
        } BIT;
    } TCRV;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char CKEG: 2;
            unsigned char : 3;
        } BIT;
    } TCR2V;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 5;
            unsigned char : 3;
        } BIT;
    } TIORV;
    char           wk3[9];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNTW: 16;
        } BIT;
    } TCNTW;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRW: 16;
        } BIT;
    } TGRW;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 2;
            unsigned char : 6;
        } BIT;
    } TCRW;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char CKEG: 2;
            unsigned char : 3;
        } BIT;
    } TCR2W;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 5;
            unsigned char : 3;
        } BIT;
    } TIORW;
    char           wk4[9];
    char           wk5[1];
    char           wk6[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIE5W: 1;
            unsigned char TGIE5V: 1;
            unsigned char TGIE5U: 1;
            unsigned char : 5;
        } BIT;
    } TIER;
    char           wk7[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CSTW5: 1;
            unsigned char CSTV5: 1;
            unsigned char CSTU5: 1;
            unsigned char : 5;
        } BIT;
    } TSTR;
    char           wk8[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CMPCLR5W: 1;
            unsigned char CMPCLR5V: 1;
            unsigned char CMPCLR5U: 1;
            unsigned char : 5;
        } BIT;
    } TCNTCMPCLR;
};

struct st_mtu6 {
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char : 2;
        } BIT;
    } TMDR1;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    char           wk2[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 2;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    char           wk3[7];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    char           wk4[6];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    char           wk5[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRC: 16;
        } BIT;
    } TGRC;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRD: 16;
        } BIT;
    } TGRD;
    char           wk6[4];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    char           wk7[11];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TTSA: 1;
            unsigned char TTSB: 1;
            unsigned char : 6;
        } BIT;
    } TBTM;
    char           wk8[19];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
    char           wk9[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CE2B: 1;
            unsigned char CE2A: 1;
            unsigned char CE1B: 1;
            unsigned char CE1A: 1;
            unsigned char CE0D: 1;
            unsigned char CE0C: 1;
            unsigned char CE0B: 1;
            unsigned char CE0A: 1;
        } BIT;
    } TSYCR;
    char           wk10[33];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRE: 16;
        } BIT;
    } TGRE;
    char           wk11[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR6;
};

struct st_mtu7 {
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char : 2;
        } BIT;
    } TMDR1;
    char           wk2[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    char           wk3[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 1;
            unsigned char TTGE2: 1;
            unsigned char TTGE: 1;
        } BIT;
    } TIER;
    char           wk4[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TCNT: 16;
        } BIT;
    } TCNT;
    char           wk5[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRA: 16;
        } BIT;
    } TGRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRB: 16;
        } BIT;
    } TGRB;
    char           wk6[8];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRC: 16;
        } BIT;
    } TGRC;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRD: 16;
        } BIT;
    } TGRD;
    char           wk7[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char TCFD: 1;
        } BIT;
    } TSR;
    char           wk8[11];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TTSA: 1;
            unsigned char TTSB: 1;
            unsigned char : 6;
        } BIT;
    } TBTM;
    char           wk9[6];
    union {
        unsigned short WORD;
        struct {
            unsigned short ITB7VE: 1;
            unsigned short ITB6AE: 1;
            unsigned short ITA7VE: 1;
            unsigned short ITA6AE: 1;
            unsigned short DT7BE: 1;
            unsigned short UT7BE: 1;
            unsigned short DT7AE: 1;
            unsigned short UT7AE: 1;
            unsigned short : 6;
            unsigned short BF: 2;
        } BIT;
    } TADCR;
    char           wk10[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCORA: 16;
        } BIT;
    } TADCORA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCORB: 16;
        } BIT;
    } TADCORB;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCOBRA: 16;
        } BIT;
    } TADCOBRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short TADCOBRB: 16;
        } BIT;
    } TADCOBRB;
    char           wk11[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
    char           wk12[38];
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRE: 16;
        } BIT;
    } TGRE;
    union {
        unsigned short WORD;
        struct {
            unsigned short TGRF: 16;
        } BIT;
    } TGRF;
    char           wk13[28];
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR7;
};

struct st_mtu8 {
    union {
        unsigned char BYTE;
        struct {
            unsigned char NFAEN: 1;
            unsigned char NFBEN: 1;
            unsigned char NFCEN: 1;
            unsigned char NFDEN: 1;
            unsigned char NFCS: 2;
            unsigned char : 2;
        } BIT;
    } NFCR8;
    char           wk0[871];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC: 3;
            unsigned char CKEG: 2;
            unsigned char CCLR: 3;
        } BIT;
    } TCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MD: 4;
            unsigned char BFA: 1;
            unsigned char BFB: 1;
            unsigned char : 2;
        } BIT;
    } TMDR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOA: 4;
            unsigned char IOB: 4;
        } BIT;
    } TIORH;
    union {
        unsigned char BYTE;
        struct {
            unsigned char IOC: 4;
            unsigned char IOD: 4;
        } BIT;
    } TIORL;
    union {
        unsigned char BYTE;
        struct {
            unsigned char TGIEA: 1;
            unsigned char TGIEB: 1;
            unsigned char TGIEC: 1;
            unsigned char TGIED: 1;
            unsigned char TCIEV: 1;
            unsigned char : 3;
        } BIT;
    } TIER;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TPSC2: 3;
            unsigned char : 5;
        } BIT;
    } TCR2;
    char           wk2[1];
    union {
        unsigned long LONG;
        struct {
            unsigned long TCNT: 32;
        } BIT;
    } TCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRA: 32;
        } BIT;
    } TGRA;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRB: 32;
        } BIT;
    } TGRB;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRC: 32;
        } BIT;
    } TGRC;
    union {
        unsigned long LONG;
        struct {
            unsigned long TGRD: 32;
        } BIT;
    } TGRD;
};

struct st_mtut {
    char           wk0[1];
    char           wk1[15];
    char           wk2[1];
    char           wk3[1];
    char           wk4[1];
    char           wk5[13];
    char           wk6[1];
    char           wk7[1];
    char           wk8[1];
    char           wk9[13];
    char           wk10[1];
    char           wk11[1];
    char           wk12[1];
};

#define MTU       (*(volatile struct st_mtu       *)0xE804120A)
#define MTU0      (*(volatile struct st_mtu0      *)0xE8041290)
#define MTU1      (*(volatile struct st_mtu1      *)0xE8041290)
#define MTU2      (*(volatile struct st_mtu2      *)0xE8041292)
#define MTU3      (*(volatile struct st_mtu3      *)0xE8041200)
#define MTU4      (*(volatile struct st_mtu4      *)0xE8041200)
#define MTU5      (*(volatile struct st_mtu5      *)0xE8041A94)
#define MTU6      (*(volatile struct st_mtu6      *)0xE8041A00)
#define MTU7      (*(volatile struct st_mtu7      *)0xE8041A00)
#define MTU8      (*(volatile struct st_mtu8      *)0xE8041298)
#define MTUT      (*(volatile struct st_mtut      *)0xE8041D00)

#endif
