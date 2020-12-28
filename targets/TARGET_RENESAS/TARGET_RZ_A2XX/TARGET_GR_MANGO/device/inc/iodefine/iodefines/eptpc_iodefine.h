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

#ifndef EPTPC_IODEFINE_H
#define EPTPC_IODEFINE_H

struct st_eptpc {
    union {
        unsigned long LONG;
        struct {
            unsigned long RESET: 1;
            unsigned long : 31;
        } BIT;
    } PTRSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long SCLKSEL: 3;
            unsigned long : 21;
        } BIT;
    } STCSELR;
    union {
        unsigned long LONG;
        struct {
            unsigned long BYPASS0: 1;
            unsigned long : 15;
            unsigned long BYPASS1: 1;
            unsigned long : 15;
        } BIT;
    } BYPASS;
    char           wk0[2804];
    union {
        unsigned long LONG;
        struct {
            unsigned long ST: 1;
            unsigned long SY0: 1;
            unsigned long SY1: 1;
            unsigned long PRC: 1;
            unsigned long : 12;
            unsigned long CYC0: 1;
            unsigned long CYC1: 1;
            unsigned long CYC2: 1;
            unsigned long CYC3: 1;
            unsigned long CYC4: 1;
            unsigned long CYC5: 1;
            unsigned long : 10;
        } BIT;
    } MIESR;
    union {
        unsigned long LONG;
        struct {
            unsigned long ST: 1;
            unsigned long SY0: 1;
            unsigned long SY1: 1;
            unsigned long PR: 1;
            unsigned long : 12;
            unsigned long CYC0: 1;
            unsigned long CYC1: 1;
            unsigned long CYC2: 1;
            unsigned long CYC3: 1;
            unsigned long CYC4: 1;
            unsigned long CYC5: 1;
            unsigned long : 10;
        } BIT;
    } MIEIPR;
    char           wk1[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long PLSP: 1;
            unsigned long : 7;
            unsigned long PLSN: 1;
            unsigned long : 7;
        } BIT;
    } ELIPPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long PLSP: 1;
            unsigned long : 7;
            unsigned long PLSN: 1;
            unsigned long : 7;
        } BIT;
    } ELIPACR;
    char           wk2[40];
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNC: 1;
            unsigned long SYNCOUT: 1;
            unsigned long : 1;
            unsigned long SYNTOUT: 1;
            unsigned long W10D: 1;
            unsigned long : 27;
        } BIT;
    } STSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNC: 1;
            unsigned long SYNCOUT: 1;
            unsigned long : 1;
            unsigned long SYNTOUT: 1;
            unsigned long W10D: 1;
            unsigned long : 27;
        } BIT;
    } STIPR;
    char           wk3[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long STCF: 2;
            unsigned long : 30;
        } BIT;
    } STCFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long WINT: 8;
            unsigned long : 5;
            unsigned long CMOD: 1;
            unsigned long : 1;
            unsigned long W10S: 1;
            unsigned long SYTH: 4;
            unsigned long DVTH: 4;
            unsigned long : 4;
            unsigned long ALEN0: 1;
            unsigned long ALEN1: 1;
            unsigned long : 2;
        } BIT;
    } STMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNTOR: 32;
        } BIT;
    } SYNTOR;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long IPTSEL0: 1;
            unsigned long IPTSEL1: 1;
            unsigned long IPTSEL2: 1;
            unsigned long IPTSEL3: 1;
            unsigned long IPTSEL4: 1;
            unsigned long IPTSEL5: 1;
            unsigned long : 26;
        } BIT;
    } IPTSELR;
    union {
        unsigned long LONG;
        struct {
            unsigned long MINTEN0: 1;
            unsigned long MINTEN1: 1;
            unsigned long MINTEN2: 1;
            unsigned long MINTEN3: 1;
            unsigned long MINTEN4: 1;
            unsigned long MINTEN5: 1;
            unsigned long : 26;
        } BIT;
    } MITSELR;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SYSEL: 1;
            unsigned long : 31;
        } BIT;
    } STCHSELR;
    char           wk6[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long STR: 1;
            unsigned long : 31;
        } BIT;
    } SYNSTARTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LOAD: 1;
            unsigned long : 31;
        } BIT;
    } LCIVLDR;
    char           wk7[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNTDARU: 32;
        } BIT;
    } SYNTDARU;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNTDARL: 32;
        } BIT;
    } SYNTDARL;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNTDBRU: 32;
        } BIT;
    } SYNTDBRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYNTDBRL: 32;
        } BIT;
    } SYNTDBRL;
    char           wk8[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long LCIVRU: 16;
            unsigned long : 16;
        } BIT;
    } LCIVRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long LCIVRM: 16;
            unsigned long : 16;
        } BIT;
    } LCIVRM;
    union {
        unsigned long LONG;
        struct {
            unsigned long LCIVRL: 16;
            unsigned long : 16;
        } BIT;
    } LCIVRL;
    char           wk9[104];
    union {
        unsigned long LONG;
        struct {
            unsigned long GW10: 1;
            unsigned long : 31;
        } BIT;
    } GETW10R;
    union {
        unsigned long LONG;
        struct {
            unsigned long PLIMITRU: 31;
            unsigned long : 1;
        } BIT;
    } PLIMITRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PLIMITRM: 32;
        } BIT;
    } PLIMITRM;
    union {
        unsigned long LONG;
        struct {
            unsigned long PLIMITRL: 32;
        } BIT;
    } PLIMITRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long MLIMITRU: 31;
            unsigned long : 1;
        } BIT;
    } MLIMITRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long MLIMITRM: 32;
        } BIT;
    } MLIMITRM;
    union {
        unsigned long LONG;
        struct {
            unsigned long MLIMITRL: 32;
        } BIT;
    } MLIMITRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long INFO: 1;
            unsigned long : 31;
        } BIT;
    } GETINFOR;
    char           wk10[44];
    union {
        unsigned long LONG;
        struct {
            unsigned long LCCVRU: 16;
            unsigned long : 16;
        } BIT;
    } LCCVRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long LCCVRM: 32;
        } BIT;
    } LCCVRM;
    union {
        unsigned long LONG;
        struct {
            unsigned long LCCVRL: 32;
        } BIT;
    } LCCVRL;
    char           wk11[148];
    union {
        unsigned long LONG;
        struct {
            unsigned long PW10VRU: 32;
        } BIT;
    } PW10VRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PW10VRM: 32;
        } BIT;
    } PW10VRM;
    union {
        unsigned long LONG;
        struct {
            unsigned long PW10VRL: 32;
        } BIT;
    } PW10VRL;
    char           wk12[180];
    union {
        unsigned long LONG;
        struct {
            unsigned long MW10RU: 32;
        } BIT;
    } MW10RU;
    union {
        unsigned long LONG;
        struct {
            unsigned long MW10RM: 32;
        } BIT;
    } MW10RM;
    union {
        unsigned long LONG;
        struct {
            unsigned long MW10RL: 32;
        } BIT;
    } MW10RL;
    char           wk13[36];
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU0: 32;
        } BIT;
    } TMSTTRU0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL0: 32;
        } BIT;
    } TMSTTRL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR0: 32;
        } BIT;
    } TMCYCR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR0: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU1: 32;
        } BIT;
    } TMSTTRU1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL1: 32;
        } BIT;
    } TMSTTRL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR1: 32;
        } BIT;
    } TMCYCR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR1: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU2: 32;
        } BIT;
    } TMSTTRU2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL2: 32;
        } BIT;
    } TMSTTRL2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR2: 32;
        } BIT;
    } TMCYCR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR2: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU3: 32;
        } BIT;
    } TMSTTRU3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL3: 32;
        } BIT;
    } TMSTTRL3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR3: 32;
        } BIT;
    } TMCYCR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR3: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU4: 32;
        } BIT;
    } TMSTTRU4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL4: 32;
        } BIT;
    } TMSTTRL4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR4: 32;
        } BIT;
    } TMCYCR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR4: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRU5: 32;
        } BIT;
    } TMSTTRU5;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMSTTRL5: 32;
        } BIT;
    } TMSTTRL5;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMCYCR5: 32;
        } BIT;
    } TMCYCR5;
    union {
        unsigned long LONG;
        struct {
            unsigned long TMPLSR5: 29;
            unsigned long : 3;
        } BIT;
    } TMPLSR5;
    char           wk14[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN0: 1;
            unsigned long EN1: 1;
            unsigned long EN2: 1;
            unsigned long EN3: 1;
            unsigned long EN4: 1;
            unsigned long EN5: 1;
            unsigned long : 26;
        } BIT;
    } TMSTARTR;
    char           wk15[128];
    union {
        unsigned long LONG;
        struct {
            unsigned long OVRE0: 1;
            unsigned long OVRE1: 1;
            unsigned long OVRE2: 1;
            unsigned long OVRE3: 1;
            unsigned long : 4;
            unsigned long MACE: 1;
            unsigned long : 19;
            unsigned long URE0: 1;
            unsigned long URE1: 1;
            unsigned long : 2;
        } BIT;
    } PRSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OVRE0: 1;
            unsigned long OVRE1: 1;
            unsigned long OVRE2: 1;
            unsigned long OVRE3: 1;
            unsigned long : 4;
            unsigned long MACE: 1;
            unsigned long : 19;
            unsigned long URE0: 1;
            unsigned long URE1: 1;
            unsigned long : 2;
        } BIT;
    } PRIPR;
    char           wk16[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long PRMACRU0: 24;
            unsigned long : 8;
        } BIT;
    } PRMACRU0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PRMACRL0: 24;
            unsigned long : 8;
        } BIT;
    } PRMACRL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long PRMACRU1: 24;
            unsigned long : 8;
        } BIT;
    } PRMACRU1;
    union {
        unsigned long LONG;
        struct {
            unsigned long PRMACRL1: 24;
            unsigned long : 8;
        } BIT;
    } PRMACRL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TDIS: 2;
            unsigned long : 30;
        } BIT;
    } TRNDISR;
    char           wk17[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long MOD: 1;
            unsigned long : 7;
            unsigned long FWD0: 1;
            unsigned long FWD1: 1;
            unsigned long : 22;
        } BIT;
    } TRNMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long THVAL: 11;
            unsigned long : 21;
        } BIT;
    } TRNCTTDR;
};

struct st_eptpc0 {
    union {
        unsigned long LONG;
        struct {
            unsigned long OFMUD: 1;
            unsigned long INTCHG: 1;
            unsigned long MPDUD: 1;
            unsigned long : 1;
            unsigned long DRPTO: 1;
            unsigned long INTDEV: 1;
            unsigned long DRQOVR: 1;
            unsigned long : 5;
            unsigned long RECLP: 1;
            unsigned long : 1;
            unsigned long INFABT: 1;
            unsigned long : 1;
            unsigned long RESDN: 1;
            unsigned long GENDN: 1;
            unsigned long : 14;
        } BIT;
    } SYSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OFMUD: 1;
            unsigned long INTCHG: 1;
            unsigned long MPDUD: 1;
            unsigned long : 1;
            unsigned long DRPTO: 1;
            unsigned long INTDEV: 1;
            unsigned long DRQOVR: 1;
            unsigned long : 5;
            unsigned long RECLP: 1;
            unsigned long : 1;
            unsigned long INFABT: 1;
            unsigned long : 1;
            unsigned long RESDN: 1;
            unsigned long GENDN: 1;
            unsigned long : 14;
        } BIT;
    } SYIPR;
    char           wk0[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long SYMACRU: 24;
            unsigned long : 8;
        } BIT;
    } SYMACRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYMACRL: 24;
            unsigned long : 8;
        } BIT;
    } SYMACRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long CTL: 8;
            unsigned long : 24;
        } BIT;
    } SYLLCCTLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYIPADDRR: 32;
        } BIT;
    } SYIPADDRR;
    char           wk1[32];
    union {
        unsigned long LONG;
        struct {
            unsigned long VER: 4;
            unsigned long TRSP: 4;
            unsigned long : 24;
        } BIT;
    } SYSPVRR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DNUM: 8;
            unsigned long : 24;
        } BIT;
    } SYDOMR;
    char           wk2[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long FLAG0: 1;
            unsigned long FLAG1: 1;
            unsigned long FLAG2: 1;
            unsigned long FLAG3: 1;
            unsigned long FLAG4: 1;
            unsigned long FLAG5: 1;
            unsigned long : 2;
            unsigned long FLAG8: 1;
            unsigned long : 1;
            unsigned long FLAG10: 1;
            unsigned long : 2;
            unsigned long FLAG13: 1;
            unsigned long FLAG14: 1;
            unsigned long : 17;
        } BIT;
    } ANFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 10;
            unsigned long FLAG10: 1;
            unsigned long : 2;
            unsigned long FLAG13: 1;
            unsigned long FLAG14: 1;
            unsigned long : 17;
        } BIT;
    } SYNFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 10;
            unsigned long FLAG10: 1;
            unsigned long : 2;
            unsigned long FLAG13: 1;
            unsigned long FLAG14: 1;
            unsigned long : 17;
        } BIT;
    } DYRQFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long FLAG8: 1;
            unsigned long FLAG9: 1;
            unsigned long FLAG10: 1;
            unsigned long : 2;
            unsigned long FLAG13: 1;
            unsigned long FLAG14: 1;
            unsigned long : 17;
        } BIT;
    } DYRPFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYCIDRL: 32;
        } BIT;
    } SYCIDRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long SYCIDRU: 32;
        } BIT;
    } SYCIDRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PNUM: 16;
            unsigned long : 16;
        } BIT;
    } SYPNUMR;
    char           wk3[20];
    union {
        unsigned long LONG;
        struct {
            unsigned long BMUP: 1;
            unsigned long STUP: 1;
            unsigned long ANUP: 1;
            unsigned long : 29;
        } BIT;
    } SYRVLDR;
    char           wk4[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long ANCE: 2;
            unsigned long : 2;
            unsigned long SYNC: 3;
            unsigned long : 1;
            unsigned long FUP: 3;
            unsigned long : 1;
            unsigned long DRQ: 3;
            unsigned long : 1;
            unsigned long DRP: 3;
            unsigned long : 1;
            unsigned long PDRQ: 3;
            unsigned long : 1;
            unsigned long PDRP: 3;
            unsigned long : 1;
            unsigned long PDFUP: 3;
            unsigned long : 1;
        } BIT;
    } SYRFL1R;
    union {
        unsigned long LONG;
        struct {
            unsigned long MAN: 2;
            unsigned long : 2;
            unsigned long SIG: 2;
            unsigned long : 22;
            unsigned long ILL: 2;
            unsigned long : 2;
        } BIT;
    } SYRFL2R;
    union {
        unsigned long LONG;
        struct {
            unsigned long ANCE: 1;
            unsigned long : 3;
            unsigned long SYNC: 1;
            unsigned long : 3;
            unsigned long DRQ: 1;
            unsigned long : 3;
            unsigned long PDRQ: 1;
            unsigned long : 19;
        } BIT;
    } SYTRENR;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long MTCIDL: 32;
        } BIT;
    } MTCIDL;
    union {
        unsigned long LONG;
        struct {
            unsigned long MTCIDU: 32;
        } BIT;
    } MTCIDU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PNUM: 16;
            unsigned long : 16;
        } BIT;
    } MTPID;
    char           wk6[20];
    union {
        unsigned long LONG;
        struct {
            unsigned long ANCE: 8;
            unsigned long SYNC: 8;
            unsigned long DREQ: 8;
            unsigned long : 8;
        } BIT;
    } SYTLIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long ANCE: 8;
            unsigned long SYNC: 8;
            unsigned long DRESP: 8;
            unsigned long : 8;
        } BIT;
    } SYRLIR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OFMRL: 32;
        } BIT;
    } OFMRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long OFMRU: 32;
        } BIT;
    } OFMRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long MPDRU: 32;
        } BIT;
    } MPDRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long MPDRL: 32;
        } BIT;
    } MPDRL;
    char           wk7[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long GMPR2: 8;
            unsigned long : 8;
            unsigned long GMPR1: 8;
            unsigned long : 8;
        } BIT;
    } GMPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GMCQR: 32;
        } BIT;
    } GMCQR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GMIDRU: 32;
        } BIT;
    } GMIDRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long GMIDRL: 32;
        } BIT;
    } GMIDRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long TSRC: 8;
            unsigned long : 8;
            unsigned long CUTO: 16;
        } BIT;
    } CUOTSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SRMV: 16;
            unsigned long : 16;
        } BIT;
    } SRR;
    char           wk8[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long PPMACRU: 24;
            unsigned long : 8;
        } BIT;
    } PPMACRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PPMACRL: 24;
            unsigned long : 8;
        } BIT;
    } PPMACRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long PDMACRU: 24;
            unsigned long : 8;
        } BIT;
    } PDMACRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long PDMACRL: 24;
            unsigned long : 8;
        } BIT;
    } PDMACRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long TYPE: 16;
            unsigned long : 16;
        } BIT;
    } PETYPER;
    char           wk9[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long PPIPR: 32;
        } BIT;
    } PPIPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PDIPR: 32;
        } BIT;
    } PDIPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long EVTO: 8;
            unsigned long : 24;
        } BIT;
    } PETOSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GETO: 8;
            unsigned long : 24;
        } BIT;
    } PGTOSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PRTL: 8;
            unsigned long : 24;
        } BIT;
    } PPTTLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PDTL: 8;
            unsigned long : 24;
        } BIT;
    } PDTTLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long EVUPT: 16;
            unsigned long : 16;
        } BIT;
    } PEUDPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GEUPT: 16;
            unsigned long : 16;
        } BIT;
    } PGUDPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 1;
            unsigned long PRT: 1;
            unsigned long ENB: 1;
            unsigned long : 13;
            unsigned long EXTPRM: 1;
            unsigned long : 15;
        } BIT;
    } FFLTR;
    char           wk10[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long FMAC0RU: 24;
            unsigned long : 8;
        } BIT;
    } FMAC0RU;
    union {
        unsigned long LONG;
        struct {
            unsigned long FMAC0RL: 24;
            unsigned long : 8;
        } BIT;
    } FMAC0RL;
    union {
        unsigned long LONG;
        struct {
            unsigned long FMAC1RU: 24;
            unsigned long : 8;
        } BIT;
    } FMAC1RU;
    union {
        unsigned long LONG;
        struct {
            unsigned long FMAC1RL: 24;
            unsigned long : 8;
        } BIT;
    } FMAC1RL;
    char           wk11[80];
    union {
        unsigned long LONG;
        struct {
            unsigned long DASYMRU: 16;
            unsigned long : 16;
        } BIT;
    } DASYMRU;
    union {
        unsigned long LONG;
        struct {
            unsigned long DASYMRL: 32;
        } BIT;
    } DASYMRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long EGP: 16;
            unsigned long INGP: 16;
        } BIT;
    } TSLATR;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCYC: 8;
            unsigned long : 4;
            unsigned long SBDIS: 1;
            unsigned long : 3;
            unsigned long FILDIS: 1;
            unsigned long : 3;
            unsigned long TCMOD: 1;
            unsigned long : 11;
        } BIT;
    } SYCONFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long FORM0: 1;
            unsigned long FORM1: 1;
            unsigned long : 30;
        } BIT;
    } SYFORMR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RSTOUTR: 32;
        } BIT;
    } RSTOUTR;
};

#define EPTPC     (*(volatile struct st_eptpc     *)0xE8204500)
#define EPTPC0    (*(volatile struct st_eptpc0    *)0xE8205800)
#define EPTPC1    (*(volatile struct st_eptpc0    *)0xE8205C00)

#endif
