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

#ifndef JCU_IODEFINE_H
#define JCU_IODEFINE_H

struct st_jcu {
    union {
        unsigned char BYTE;
        struct {
            unsigned char REDU: 3;
            unsigned char DSP: 1;
            unsigned char : 4;
        } BIT;
    } JCMOD;
    union {
        unsigned char BYTE;
        struct {
            unsigned char JSRT: 1;
            unsigned char JRST: 1;
            unsigned char JEND: 1;
            unsigned char : 4;
            unsigned char BRST: 1;
        } BIT;
    } JCCMD;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char QT1: 2;
            unsigned char QT2: 2;
            unsigned char QT3: 2;
            unsigned char : 2;
        } BIT;
    } JCQTN;
    union {
        unsigned char BYTE;
        struct {
            unsigned char HTD1: 1;
            unsigned char HTA1: 1;
            unsigned char HTD2: 1;
            unsigned char HTA2: 1;
            unsigned char HTD3: 1;
            unsigned char HTA3: 1;
            unsigned char : 2;
        } BIT;
    } JCHTN;
    union {
        unsigned char BYTE;
        struct {
            unsigned char DRIU: 8;
        } BIT;
    } JCDRIU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char DRID: 8;
        } BIT;
    } JCDRID;
    union {
        unsigned char BYTE;
        struct {
            unsigned char VSZU: 8;
        } BIT;
    } JCVSZU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char VSZD: 8;
        } BIT;
    } JCVSZD;
    union {
        unsigned char BYTE;
        struct {
            unsigned char HSZU: 8;
        } BIT;
    } JCHSZU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char HSZD: 8;
        } BIT;
    } JCHSZD;
    union {
        unsigned char BYTE;
        struct {
            unsigned char DCU: 8;
        } BIT;
    } JCDTCU;
    union {
        unsigned char BYTE;
        struct {
            unsigned char DCM: 8;
        } BIT;
    } JCDTCM;
    union {
        unsigned char BYTE;
        struct {
            unsigned char DCD: 8;
        } BIT;
    } JCDTCD;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 3;
            unsigned char INT3: 1;
            unsigned char : 1;
            unsigned char INT5: 1;
            unsigned char INT6: 1;
            unsigned char INT7: 1;
        } BIT;
    } JINTE0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 3;
            unsigned char INS3: 1;
            unsigned char : 1;
            unsigned char INS5: 1;
            unsigned char INS6: 1;
            unsigned char : 1;
        } BIT;
    } JINTS0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char ERR: 4;
            unsigned char : 4;
        } BIT;
    } JCDERR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char RST: 1;
            unsigned char : 7;
        } BIT;
    } JCRST;
    char           wk1[1];
    char           wk2[1];
    char           wk3[44];
    union {
        unsigned long LONG;
        struct {
            unsigned long DINSWAP: 3;
            unsigned long : 1;
            unsigned long DINLC: 1;
            unsigned long DINRCMD: 1;
            unsigned long DINRINI: 1;
            unsigned long : 1;
            unsigned long JOUTSWAP: 3;
            unsigned long : 1;
            unsigned long JOUTC: 1;
            unsigned long JOUTRCMD: 1;
            unsigned long JOUTRINI: 1;
            unsigned long : 17;
        } BIT;
    } JIFECNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long ESA: 32;
        } BIT;
    } JIFESA;
    union {
        unsigned long LONG;
        struct {
            unsigned long ESMW: 15;
            unsigned long : 17;
        } BIT;
    } JIFESOFST;
    union {
        unsigned long LONG;
        struct {
            unsigned long EDA: 32;
        } BIT;
    } JIFEDA;
    union {
        unsigned long LONG;
        struct {
            unsigned long LINES: 16;
            unsigned long : 16;
        } BIT;
    } JIFESLC;
    union {
        unsigned long LONG;
        struct {
            unsigned long JDATAS: 16;
            unsigned long : 16;
        } BIT;
    } JIFEDDC;
    union {
        unsigned long LONG;
        struct {
            unsigned long DOUTSWAP: 3;
            unsigned long : 1;
            unsigned long DOUTLC: 1;
            unsigned long DOUTRCMD: 1;
            unsigned long DOUTRINI: 1;
            unsigned long : 1;
            unsigned long JINSWAP: 3;
            unsigned long : 1;
            unsigned long JINC: 1;
            unsigned long JINRCMD: 1;
            unsigned long JINRINI: 1;
            unsigned long : 9;
            unsigned long OPF: 2;
            unsigned long HINTER: 2;
            unsigned long VINTER: 2;
            unsigned long : 2;
        } BIT;
    } JIFDCNT;
    union {
        unsigned long LONG;
        struct {
            unsigned long DSA: 32;
        } BIT;
    } JIFDSA;
    union {
        unsigned long LONG;
        struct {
            unsigned long DDMW: 15;
            unsigned long : 17;
        } BIT;
    } JIFDDOFST;
    union {
        unsigned long LONG;
        struct {
            unsigned long DDA: 32;
        } BIT;
    } JIFDDA;
    union {
        unsigned long LONG;
        struct {
            unsigned long JDATAS: 16;
            unsigned long : 16;
        } BIT;
    } JIFDSDC;
    union {
        unsigned long LONG;
        struct {
            unsigned long LINES: 16;
            unsigned long : 16;
        } BIT;
    } JIFDDLC;
    union {
        unsigned long LONG;
        struct {
            unsigned long ALPHA: 8;
            unsigned long : 24;
        } BIT;
    } JIFDADT;
    char           wk4[4];
    char           wk5[4];
    char           wk6[4];
    char           wk7[4];
    char           wk8[4];
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DOUTLEN: 1;
            unsigned long JINEN: 1;
            unsigned long DBTEN: 1;
            unsigned long : 1;
            unsigned long JOUTEN: 1;
            unsigned long DINLEN: 1;
            unsigned long CBTEN: 1;
            unsigned long : 25;
        } BIT;
    } JINTE1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DOUTLF: 1;
            unsigned long JINF: 1;
            unsigned long DBTF: 1;
            unsigned long : 1;
            unsigned long JOUTF: 1;
            unsigned long DINLF: 1;
            unsigned long CBTF: 1;
            unsigned long : 25;
        } BIT;
    } JINTS1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 15;
            unsigned long DINYCHG: 1;
            unsigned long : 15;
            unsigned long : 1;
        } BIT;
    } JIFESVSZ;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 15;
            unsigned long DOUTYCHG: 1;
            unsigned long : 15;
            unsigned long : 1;
        } BIT;
    } JIFESHSZ;
    char           wk10[100];
    unsigned char  JCQTBL0;
    char           wk11[63];
    unsigned char  JCQTBL1;
    char           wk12[63];
    unsigned char  JCQTBL2;
    char           wk13[63];
    unsigned char  JCQTBL3;
    char           wk14[63];
    unsigned char  JCHTBD0;
    char           wk15[31];
    unsigned char  JCHTBA0;
    char           wk16[223];
    unsigned char  JCHTBD1;
    char           wk17[31];
    unsigned char  JCHTBA1;
};

#define JCU       (*(volatile struct st_jcu       *)0xE8017000)

#endif
