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

#ifndef GPT_IODEFINE_H
#define GPT_IODEFINE_H

struct st_gpt {
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 7;
            unsigned char EVCON: 1;
        } BIT;
    } GTECR;
    char           wk0[15];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR0;
    char           wk1[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR1;
    char           wk2[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR2;
    char           wk3[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR3;
    char           wk4[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR4;
    char           wk5[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR5;
    char           wk6[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR6;
    char           wk7[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short EVS: 7;
            unsigned short : 9;
        } BIT;
    } GTESR7;
};

struct st_gpt32e {
    union {
        unsigned long LONG;
        struct {
            unsigned long WP: 1;
            unsigned long : 7;
            unsigned long PRKEY: 8;
            unsigned long : 16;
        } BIT;
    } GTWP;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSTRT0: 1;
            unsigned long CSTRT1: 1;
            unsigned long CSTRT2: 1;
            unsigned long CSTRT3: 1;
            unsigned long CSTRT4: 1;
            unsigned long CSTRT5: 1;
            unsigned long CSTRT6: 1;
            unsigned long CSTRT7: 1;
            unsigned long : 24;
        } BIT;
    } GTSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSTOP0: 1;
            unsigned long CSTOP1: 1;
            unsigned long CSTOP2: 1;
            unsigned long CSTOP3: 1;
            unsigned long CSTOP4: 1;
            unsigned long CSTOP5: 1;
            unsigned long CSTOP6: 1;
            unsigned long CSTOP7: 1;
            unsigned long : 24;
        } BIT;
    } GTSTP;
    union {
        unsigned long LONG;
        struct {
            unsigned long CCLR0: 1;
            unsigned long CCLR1: 1;
            unsigned long CCLR2: 1;
            unsigned long CCLR3: 1;
            unsigned long CCLR4: 1;
            unsigned long CCLR5: 1;
            unsigned long CCLR6: 1;
            unsigned long CCLR7: 1;
            unsigned long : 24;
        } BIT;
    } GTCLR;
    union {
        unsigned long LONG;
        struct {
            unsigned long SSGTRGAR: 1;
            unsigned long SSGTRGAF: 1;
            unsigned long SSGTRGBR: 1;
            unsigned long SSGTRGBF: 1;
            unsigned long SSGTRGCR: 1;
            unsigned long SSGTRGCF: 1;
            unsigned long SSGTRGDR: 1;
            unsigned long SSGTRGDF: 1;
            unsigned long SSCARBL: 1;
            unsigned long SSCARBH: 1;
            unsigned long SSCAFBL: 1;
            unsigned long SSCAFBH: 1;
            unsigned long SSCBRAL: 1;
            unsigned long SSCBRAH: 1;
            unsigned long SSCBFAL: 1;
            unsigned long SSCBFAH: 1;
            unsigned long SSEVTA: 1;
            unsigned long SSEVTB: 1;
            unsigned long SSEVTC: 1;
            unsigned long SSEVTD: 1;
            unsigned long SSEVTE: 1;
            unsigned long SSEVTF: 1;
            unsigned long SSEVTG: 1;
            unsigned long SSEVTH: 1;
            unsigned long : 7;
            unsigned long CSTRT: 1;
        } BIT;
    } GTSSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long PSGTRGAR: 1;
            unsigned long PSGTRGAF: 1;
            unsigned long PSGTRGBR: 1;
            unsigned long PSGTRGBF: 1;
            unsigned long PSGTRGCR: 1;
            unsigned long PSGTRGCF: 1;
            unsigned long PSGTRGDR: 1;
            unsigned long PSGTRGDF: 1;
            unsigned long PSCARBL: 1;
            unsigned long PSCARBH: 1;
            unsigned long PSCAFBL: 1;
            unsigned long PSCAFBH: 1;
            unsigned long PSCBRAL: 1;
            unsigned long PSCBRAH: 1;
            unsigned long PSCBFAL: 1;
            unsigned long PSCBFAH: 1;
            unsigned long PSEVTA: 1;
            unsigned long PSEVTB: 1;
            unsigned long PSEVTC: 1;
            unsigned long PSEVTD: 1;
            unsigned long PSEVTE: 1;
            unsigned long PSEVTF: 1;
            unsigned long PSEVTG: 1;
            unsigned long PSEVTH: 1;
            unsigned long : 7;
            unsigned long CSTOP: 1;
        } BIT;
    } GTPSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CSGTRGAR: 1;
            unsigned long CSGTRGAF: 1;
            unsigned long CSGTRGBR: 1;
            unsigned long CSGTRGBF: 1;
            unsigned long CSGTRGCR: 1;
            unsigned long CSGTRGCF: 1;
            unsigned long CSGTRGDR: 1;
            unsigned long CSGTRGDF: 1;
            unsigned long CSCARBL: 1;
            unsigned long CSCARBH: 1;
            unsigned long CSCAFBL: 1;
            unsigned long CSCAFBH: 1;
            unsigned long CSCBRAL: 1;
            unsigned long CSCBRAH: 1;
            unsigned long CSCBFAL: 1;
            unsigned long CSCBFAH: 1;
            unsigned long CSEVTA: 1;
            unsigned long CSEVTB: 1;
            unsigned long CSEVTC: 1;
            unsigned long CSEVTD: 1;
            unsigned long CSEVTE: 1;
            unsigned long CSEVTF: 1;
            unsigned long CSEVTG: 1;
            unsigned long CSEVTH: 1;
            unsigned long : 7;
            unsigned long CCLR: 1;
        } BIT;
    } GTCSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long USGTRGAR: 1;
            unsigned long USGTRGAF: 1;
            unsigned long USGTRGBR: 1;
            unsigned long USGTRGBF: 1;
            unsigned long USGTRGCR: 1;
            unsigned long USGTRGCF: 1;
            unsigned long USGTRGDR: 1;
            unsigned long USGTRGDF: 1;
            unsigned long USCARBL: 1;
            unsigned long USCARBH: 1;
            unsigned long USCAFBL: 1;
            unsigned long USCAFBH: 1;
            unsigned long USCBRAL: 1;
            unsigned long USCBRAH: 1;
            unsigned long USCBFAL: 1;
            unsigned long USCBFAH: 1;
            unsigned long USEVTA: 1;
            unsigned long USEVTB: 1;
            unsigned long USEVTC: 1;
            unsigned long USEVTD: 1;
            unsigned long USEVTE: 1;
            unsigned long USEVTF: 1;
            unsigned long USEVTG: 1;
            unsigned long USEVTH: 1;
            unsigned long : 8;
        } BIT;
    } GTUPSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long DSGTRGAR: 1;
            unsigned long DSGTRGAF: 1;
            unsigned long DSGTRGBR: 1;
            unsigned long DSGTRGBF: 1;
            unsigned long DSGTRGCR: 1;
            unsigned long DSGTRGCF: 1;
            unsigned long DSGTRGDR: 1;
            unsigned long DSGTRGDF: 1;
            unsigned long DSCARBL: 1;
            unsigned long DSCARBH: 1;
            unsigned long DSCAFBL: 1;
            unsigned long DSCAFBH: 1;
            unsigned long DSCBRAL: 1;
            unsigned long DSCBRAH: 1;
            unsigned long DSCBFAL: 1;
            unsigned long DSCBFAH: 1;
            unsigned long DSEVTA: 1;
            unsigned long DSEVTB: 1;
            unsigned long DSEVTC: 1;
            unsigned long DSEVTD: 1;
            unsigned long DSEVTE: 1;
            unsigned long DSEVTF: 1;
            unsigned long DSEVTG: 1;
            unsigned long DSEVTH: 1;
            unsigned long : 8;
        } BIT;
    } GTDNSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long ASGTRGAR: 1;
            unsigned long ASGTRGAF: 1;
            unsigned long ASGTRGBR: 1;
            unsigned long ASGTRGBF: 1;
            unsigned long ASGTRGCR: 1;
            unsigned long ASGTRGCF: 1;
            unsigned long ASGTRGDR: 1;
            unsigned long ASGTRGDF: 1;
            unsigned long ASCARBL: 1;
            unsigned long ASCARBH: 1;
            unsigned long ASCAFBL: 1;
            unsigned long ASCAFBH: 1;
            unsigned long ASCBRAL: 1;
            unsigned long ASCBRAH: 1;
            unsigned long ASCBFAL: 1;
            unsigned long ASCBFAH: 1;
            unsigned long ASEVTA: 1;
            unsigned long ASEVTB: 1;
            unsigned long ASEVTC: 1;
            unsigned long ASEVTD: 1;
            unsigned long ASEVTE: 1;
            unsigned long ASEVTF: 1;
            unsigned long ASEVTG: 1;
            unsigned long ASEVTH: 1;
            unsigned long : 8;
        } BIT;
    } GTICASR;
    union {
        unsigned long LONG;
        struct {
            unsigned long BSGTRGAR: 1;
            unsigned long BSGTRGAF: 1;
            unsigned long BSGTRGBR: 1;
            unsigned long BSGTRGBF: 1;
            unsigned long BSGTRGCR: 1;
            unsigned long BSGTRGCF: 1;
            unsigned long BSGTRGDR: 1;
            unsigned long BSGTRGDF: 1;
            unsigned long BSCARBL: 1;
            unsigned long BSCARBH: 1;
            unsigned long BSCAFBL: 1;
            unsigned long BSCAFBH: 1;
            unsigned long BSCBRAL: 1;
            unsigned long BSCBRAH: 1;
            unsigned long BSCBFAL: 1;
            unsigned long BSCBFAH: 1;
            unsigned long BSEVTA: 1;
            unsigned long BSEVTB: 1;
            unsigned long BSEVTC: 1;
            unsigned long BSEVTD: 1;
            unsigned long BSEVTE: 1;
            unsigned long BSEVTF: 1;
            unsigned long BSEVTG: 1;
            unsigned long BSEVTH: 1;
            unsigned long : 8;
        } BIT;
    } GTICBSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CST: 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 5;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long MD: 3;
            unsigned long : 5;
            unsigned long TPCS: 3;
            unsigned long : 5;
        } BIT;
    } GTCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long UD: 1;
            unsigned long UDF: 1;
            unsigned long : 14;
            unsigned long OADTY: 2;
            unsigned long OADTYF: 1;
            unsigned long OADTYR: 1;
            unsigned long : 4;
            unsigned long OBDTY: 2;
            unsigned long OBDTYF: 1;
            unsigned long OBDTYR: 1;
            unsigned long : 4;
        } BIT;
    } GTUDDTYC;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTIOA: 5;
            unsigned long : 1;
            unsigned long OADFLT: 1;
            unsigned long OAHLD: 1;
            unsigned long OAE: 1;
            unsigned long OADF: 2;
            unsigned long : 2;
            unsigned long NFAEN: 1;
            unsigned long NFCSA: 2;
            unsigned long GTIOB: 5;
            unsigned long : 1;
            unsigned long OBDFLT: 1;
            unsigned long OBHLD: 1;
            unsigned long OBE: 1;
            unsigned long OBDF: 2;
            unsigned long : 2;
            unsigned long NFBEN: 1;
            unsigned long NFCSB: 2;
        } BIT;
    } GTIOR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTINTA: 1;
            unsigned long GTINTB: 1;
            unsigned long GTINTC: 1;
            unsigned long GTINTD: 1;
            unsigned long GTINTE: 1;
            unsigned long GTINTF: 1;
            unsigned long GTINTPR: 2;
            unsigned long : 8;
            unsigned long ADTRAUEN: 1;
            unsigned long ADTRADEN: 1;
            unsigned long ADTRBUEN: 1;
            unsigned long ADTRBDEN: 1;
            unsigned long : 4;
            unsigned long GRP: 2;
            unsigned long : 2;
            unsigned long GRPDTE: 1;
            unsigned long GRPABH: 1;
            unsigned long GRPABL: 1;
            unsigned long : 1;
        } BIT;
    } GTINTAD;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCFA: 1;
            unsigned long TCFB: 1;
            unsigned long TCFC: 1;
            unsigned long TCFD: 1;
            unsigned long TCFE: 1;
            unsigned long TCFF: 1;
            unsigned long TCFPO: 1;
            unsigned long TCFPU: 1;
            unsigned long ITCNT: 3;
            unsigned long : 4;
            unsigned long TUCF: 1;
            unsigned long ADTRAUF: 1;
            unsigned long ADTRADF: 1;
            unsigned long ADTRBUF: 1;
            unsigned long ADTRBDF: 1;
            unsigned long : 4;
            unsigned long ODF: 1;
            unsigned long : 3;
            unsigned long DTEF: 1;
            unsigned long OABHF: 1;
            unsigned long OABLF: 1;
            unsigned long : 1;
        } BIT;
    } GTST;
    union {
        unsigned long LONG;
        struct {
            unsigned long BD: 4;
            unsigned long : 12;
            unsigned long CCRA: 2;
            unsigned long CCRB: 2;
            unsigned long PR: 2;
            unsigned long CCRSWT: 1;
            unsigned long : 1;
            unsigned long ADTTA: 2;
            unsigned long ADTDA: 1;
            unsigned long : 1;
            unsigned long ADTTB: 2;
            unsigned long ADTDB: 1;
            unsigned long : 1;
        } BIT;
    } GTBER;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITLA: 1;
            unsigned long ITLB: 1;
            unsigned long ITLC: 1;
            unsigned long ITLD: 1;
            unsigned long ITLE: 1;
            unsigned long ITLF: 1;
            unsigned long IVTC: 2;
            unsigned long IVTT: 3;
            unsigned long : 1;
            unsigned long ADTAL: 1;
            unsigned long : 1;
            unsigned long ADTBL: 1;
            unsigned long : 17;
        } BIT;
    } GTITC;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCNT: 32;
        } BIT;
    } GTCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRA: 32;
        } BIT;
    } GTCCRA;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRB: 32;
        } BIT;
    } GTCCRB;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRC: 32;
        } BIT;
    } GTCCRC;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRE: 32;
        } BIT;
    } GTCCRE;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRD: 32;
        } BIT;
    } GTCCRD;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTCCRF: 32;
        } BIT;
    } GTCCRF;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTPR: 32;
        } BIT;
    } GTPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTPBR: 32;
        } BIT;
    } GTPBR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTPDBR: 32;
        } BIT;
    } GTPDBR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTRA: 32;
        } BIT;
    } GTADTRA;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTBRA: 32;
        } BIT;
    } GTADTBRA;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTDBRA: 32;
        } BIT;
    } GTADTDBRA;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTRB: 32;
        } BIT;
    } GTADTRB;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTBRB: 32;
        } BIT;
    } GTADTBRB;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTADTDBRB: 32;
        } BIT;
    } GTADTDBRB;
    union {
        unsigned long LONG;
        struct {
            unsigned long TDE: 1;
            unsigned long : 3;
            unsigned long TDBUE: 1;
            unsigned long TDBDE: 1;
            unsigned long : 2;
            unsigned long TDFER: 1;
            unsigned long : 23;
        } BIT;
    } GTDTCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTDVU: 32;
        } BIT;
    } GTDVU;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTDVD: 32;
        } BIT;
    } GTDVD;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTDBU: 32;
        } BIT;
    } GTDBU;
    union {
        unsigned long LONG;
        struct {
            unsigned long GTDBD: 32;
        } BIT;
    } GTDBD;
    union {
        unsigned long LONG;
        struct {
            unsigned long SOS: 2;
            unsigned long : 6;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 22;
        } BIT;
    } GTSOS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SOTR: 1;
            unsigned long : 31;
        } BIT;
    } GTSOTR;
};

#define GPT       (*(volatile struct st_gpt       *)0xE8043800)
#define GPT32E0   (*(volatile struct st_gpt32e    *)0xE8043000)
#define GPT32E1   (*(volatile struct st_gpt32e    *)0xE8043100)
#define GPT32E2   (*(volatile struct st_gpt32e    *)0xE8043200)
#define GPT32E3   (*(volatile struct st_gpt32e    *)0xE8043300)
#define GPT32E4   (*(volatile struct st_gpt32e    *)0xE8043400)
#define GPT32E5   (*(volatile struct st_gpt32e    *)0xE8043500)
#define GPT32E6   (*(volatile struct st_gpt32e    *)0xE8043600)
#define GPT32E7   (*(volatile struct st_gpt32e    *)0xE8043700)

#endif
