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

#ifndef SDMMC_IODEFINE_H
#define SDMMC_IODEFINE_H

struct st_sdmmc0 {
    union {
        unsigned long LONG;
        struct {
            unsigned long TAPEN: 1;
            unsigned long : 15;
            unsigned long TAPNUM: 8;
            unsigned long : 8;
        } BIT;
    } SCC_DTCNTL;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TAPSET: 8;
            unsigned long : 24;
        } BIT;
    } SCC_TAPSET;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DT2NSSET: 8;
            unsigned long DT2NESET: 8;
            unsigned long : 16;
        } BIT;
    } SCC_DT2FF;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DTSEL: 1;
            unsigned long : 31;
        } BIT;
    } SCC_CKSEL;
    char           wk3[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RVSEN: 1;
            unsigned long RVSW: 1;
            unsigned long : 6;
            unsigned long TAPSEL: 8;
            unsigned long : 16;
        } BIT;
    } SCC_RVSCNTL;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long REQTAPDWN: 1;
            unsigned long REQTAPUP: 1;
            unsigned long RVSERR: 1;
            unsigned long : 29;
        } BIT;
    } SCC_RVSREQ;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CMPNGD: 9;
            unsigned long : 7;
            unsigned long CMPNGU: 9;
            unsigned long : 7;
        } BIT;
    } SCC_SMPCMP;
    char           wk6[4044];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 6;
            unsigned long long C0: 1;
            unsigned long long C1: 1;
            unsigned long long MD0: 1;
            unsigned long long MD1: 1;
            unsigned long long MD2: 1;
            unsigned long long MD3: 1;
            unsigned long long MD4: 1;
            unsigned long long MD5: 1;
            unsigned long long MD6: 1;
            unsigned long long MD7: 1;
            unsigned long long : 48;
        } BIT;
    } SD_CMD;
    char           wk7[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 32;
            unsigned long long : 32;
        } BIT;
    } SD_ARG;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 16;
            unsigned long long : 48;
        } BIT;
    } SD_ARG1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long STP: 1;
            unsigned long long : 7;
            unsigned long long SEC: 1;
            unsigned long long : 7;
            unsigned long long HPICMD: 1;
            unsigned long long HPIMODE: 1;
            unsigned long long : 46;
        } BIT;
    } SD_STOP;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CNT: 32;
            unsigned long long : 32;
        } BIT;
    } SD_SECCNT;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 64;
        } BIT;
    } SD_RSP10;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 32;
            unsigned long long : 32;
        } BIT;
    } SD_RSP32;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP3;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 56;
            unsigned long long : 8;
        } BIT;
    } SD_RSP54;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP5;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 24;
            unsigned long long : 40;
        } BIT;
    } SD_RSP76;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 8;
            unsigned long long : 56;
        } BIT;
    } SD_RSP7;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long INFO0: 1;
            unsigned long long : 1;
            unsigned long long INFO2: 1;
            unsigned long long INFO3: 1;
            unsigned long long INFO4: 1;
            unsigned long long INFO5: 1;
            unsigned long long : 1;
            unsigned long long INFO7: 1;
            unsigned long long INFO8: 1;
            unsigned long long INFO9: 1;
            unsigned long long INFO10: 1;
            unsigned long long : 5;
            unsigned long long HPIRES: 1;
            unsigned long long : 47;
        } BIT;
    } SD_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long ERR0: 1;
            unsigned long long ERR1: 1;
            unsigned long long ERR2: 1;
            unsigned long long ERR3: 1;
            unsigned long long ERR4: 1;
            unsigned long long ERR5: 1;
            unsigned long long ERR6: 1;
            unsigned long long DAT0: 1;
            unsigned long long BRE: 1;
            unsigned long long BWE: 1;
            unsigned long long : 3;
            unsigned long long SCLKDIVEN: 1;
            unsigned long long CBSY: 1;
            unsigned long long ILA: 1;
            unsigned long long : 48;
        } BIT;
    } SD_INFO2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IMASK0: 1;
            unsigned long long : 1;
            unsigned long long IMASK2: 1;
            unsigned long long IMASK3: 1;
            unsigned long long IMASK4: 1;
            unsigned long long : 3;
            unsigned long long IMASK8: 1;
            unsigned long long IMASK9: 1;
            unsigned long long : 6;
            unsigned long long IMASK16: 1;
            unsigned long long : 47;
        } BIT;
    } SD_INFO1_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long EMASK0: 1;
            unsigned long long EMASK1: 1;
            unsigned long long EMASK2: 1;
            unsigned long long EMASK3: 1;
            unsigned long long EMASK4: 1;
            unsigned long long EMASK5: 1;
            unsigned long long EMASK6: 1;
            unsigned long long : 1;
            unsigned long long BMASK0: 1;
            unsigned long long BMASK1: 1;
            unsigned long long : 5;
            unsigned long long IMASK: 1;
            unsigned long long : 48;
        } BIT;
    } SD_INFO2_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DIV: 8;
            unsigned long long SCLKEN: 1;
            unsigned long long SDCLKOFFEN: 1;
            unsigned long long : 54;
        } BIT;
    } SD_CLK_CTRL;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long LEN: 10;
            unsigned long long : 54;
        } BIT;
    } SD_SIZE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CTOP21: 1;
            unsigned long long CTOP22: 1;
            unsigned long long CTOP23: 1;
            unsigned long long CTOP24: 1;
            unsigned long long TOP24: 1;
            unsigned long long TOP25: 1;
            unsigned long long TOP26: 1;
            unsigned long long TOP27: 1;
            unsigned long long TOUTMASK: 1;
            unsigned long long EXTOP: 1;
            unsigned long long : 3;
            unsigned long long WIDTH8: 1;
            unsigned long long : 1;
            unsigned long long WIDTH: 1;
            unsigned long long : 48;
        } BIT;
    } SD_OPTION;
    char           wk8[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long E0: 1;
            unsigned long long E1: 1;
            unsigned long long E2: 1;
            unsigned long long E3: 1;
            unsigned long long E4: 1;
            unsigned long long E5: 1;
            unsigned long long : 2;
            unsigned long long E8: 1;
            unsigned long long E9: 1;
            unsigned long long E10: 1;
            unsigned long long E11: 1;
            unsigned long long E12: 1;
            unsigned long long E13: 1;
            unsigned long long E14: 1;
            unsigned long long : 49;
        } BIT;
    } SD_ERR_STS1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long E0: 1;
            unsigned long long E1: 1;
            unsigned long long E2: 1;
            unsigned long long E3: 1;
            unsigned long long E4: 1;
            unsigned long long E5: 1;
            unsigned long long E6: 1;
            unsigned long long : 57;
        } BIT;
    } SD_ERR_STS2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long BUF: 64;
        } BIT;
    } SD_BUF0;
    char           wk9[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOMOD: 1;
            unsigned long long : 1;
            unsigned long long RWREQ: 1;
            unsigned long long : 5;
            unsigned long long IOABT: 1;
            unsigned long long C52PUB: 1;
            unsigned long long : 54;
        } BIT;
    } SDIO_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOIRQ: 1;
            unsigned long long : 13;
            unsigned long long EXPUB52: 1;
            unsigned long long EXWT: 1;
            unsigned long long : 48;
        } BIT;
    } SDIO_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOMSK: 1;
            unsigned long long : 13;
            unsigned long long MEXPUB52: 1;
            unsigned long long MEXWT: 1;
            unsigned long long : 48;
        } BIT;
    } SDIO_INFO1_MASK;
    char           wk10[632];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 1;
            unsigned long long DMASDRW: 1;
            unsigned long long : 62;
        } BIT;
    } CC_EXT_MODE;
    char           wk11[24];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SDRST: 1;
            unsigned long long : 63;
        } BIT;
    } SOFT_RST;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IP: 8;
            unsigned long long UR: 8;
            unsigned long long : 48;
        } BIT;
    } VERSION;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long WMODE: 1;
            unsigned long long ENDIAN: 1;
            unsigned long long : 6;
            unsigned long long BUSWIDTH: 1;
            unsigned long long : 55;
        } BIT;
    } HOST_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DDR: 1;
            unsigned long long : 7;
            unsigned long long NOCHKCR: 1;
            unsigned long long : 55;
        } BIT;
    } SDIF_MODE;
    char           wk12[40];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 1;
            unsigned long long SD_RST: 1;
            unsigned long long : 62;
        } BIT;
    } SD_STATUS;
    char           wk13[1080];
    char           wk14[8];
    char           wk15[16];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 4;
            unsigned long long BUS_WIDTH: 2;
            unsigned long long : 10;
            unsigned long long CH_NUM: 2;
            unsigned long long : 46;
        } BIT;
    } DM_CM_DTRAN_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DM_START: 1;
            unsigned long long : 63;
        } BIT;
    } DM_CM_DTRAN_CTRL;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQRST: 1;
            unsigned long long : 7;
            unsigned long long DTRANRST0: 1;
            unsigned long long DTRANRST1: 1;
            unsigned long long : 54;
        } BIT;
    } DM_CM_RST;
    char           wk16[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQEND: 1;
            unsigned long long : 15;
            unsigned long long DTRANEND0: 1;
            unsigned long long : 3;
            unsigned long long DTRANEND1: 1;
            unsigned long long : 43;
        } BIT;
    } DM_CM_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQEND_MASK: 1;
            unsigned long long : 15;
            unsigned long long DTRANEND0_MASK: 1;
            unsigned long long : 3;
            unsigned long long DTRANEND1_MASK: 1;
            unsigned long long : 43;
        } BIT;
    } DM_CM_INFO1_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQERR: 1;
            unsigned long long : 15;
            unsigned long long DTRANERR0: 1;
            unsigned long long DTRANERR1: 1;
            unsigned long long : 46;
        } BIT;
    } DM_CM_INFO2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQERR_MASK: 1;
            unsigned long long : 15;
            unsigned long long DTRANERR0_MASK: 1;
            unsigned long long DTRANERR1_MASK: 1;
            unsigned long long : 46;
        } BIT;
    } DM_CM_INFO2_MASK;
    char           wk17[32];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 3;
            unsigned long long DADDR: 29;
            unsigned long long : 32;
        } BIT;
    } DM_DTRAN_ADDR;
};

struct st_sdmmc1 {
    union {
        unsigned long LONG;
        struct {
            unsigned long TAPEN: 1;
            unsigned long : 15;
            unsigned long TAPNUM: 8;
            unsigned long : 8;
        } BIT;
    } SCC_DTCNTL;
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TAPSET: 8;
            unsigned long : 24;
        } BIT;
    } SCC_TAPSET;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DT2NSSET: 8;
            unsigned long DT2NESET: 8;
            unsigned long : 16;
        } BIT;
    } SCC_DT2FF;
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DTSEL: 1;
            unsigned long : 31;
        } BIT;
    } SCC_CKSEL;
    char           wk3[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RVSEN: 1;
            unsigned long RVSW: 1;
            unsigned long : 6;
            unsigned long TAPSEL: 8;
            unsigned long : 16;
        } BIT;
    } SCC_RVSCNTL;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long REQTAPDWN: 1;
            unsigned long REQTAPUP: 1;
            unsigned long RVSERR: 1;
            unsigned long : 29;
        } BIT;
    } SCC_RVSREQ;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CMPNGD: 9;
            unsigned long : 7;
            unsigned long CMPNGU: 9;
            unsigned long : 7;
        } BIT;
    } SCC_SMPCMP;
    char           wk6[4044];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 6;
            unsigned long long C0: 1;
            unsigned long long C1: 1;
            unsigned long long MD0: 1;
            unsigned long long MD1: 1;
            unsigned long long MD2: 1;
            unsigned long long MD3: 1;
            unsigned long long MD4: 1;
            unsigned long long MD5: 1;
            unsigned long long MD6: 1;
            unsigned long long MD7: 1;
            unsigned long long : 48;
        } BIT;
    } SD_CMD;
    char           wk7[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 32;
            unsigned long long : 32;
        } BIT;
    } SD_ARG;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CF: 16;
            unsigned long long : 48;
        } BIT;
    } SD_ARG1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long STP: 1;
            unsigned long long : 7;
            unsigned long long SEC: 1;
            unsigned long long : 7;
            unsigned long long HPICMD: 1;
            unsigned long long HPIMODE: 1;
            unsigned long long : 46;
        } BIT;
    } SD_STOP;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CNT: 32;
            unsigned long long : 32;
        } BIT;
    } SD_SECCNT;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 64;
        } BIT;
    } SD_RSP10;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 32;
            unsigned long long : 32;
        } BIT;
    } SD_RSP32;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP3;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 56;
            unsigned long long : 8;
        } BIT;
    } SD_RSP54;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 16;
            unsigned long long : 48;
        } BIT;
    } SD_RSP5;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 24;
            unsigned long long : 40;
        } BIT;
    } SD_RSP76;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long R: 8;
            unsigned long long : 56;
        } BIT;
    } SD_RSP7;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long INFO0: 1;
            unsigned long long : 1;
            unsigned long long INFO2: 1;
            unsigned long long INFO3: 1;
            unsigned long long INFO4: 1;
            unsigned long long INFO5: 1;
            unsigned long long : 1;
            unsigned long long INFO7: 1;
            unsigned long long INFO8: 1;
            unsigned long long INFO9: 1;
            unsigned long long INFO10: 1;
            unsigned long long : 5;
            unsigned long long HPIRES: 1;
            unsigned long long : 47;
        } BIT;
    } SD_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long ERR0: 1;
            unsigned long long ERR1: 1;
            unsigned long long ERR2: 1;
            unsigned long long ERR3: 1;
            unsigned long long ERR4: 1;
            unsigned long long ERR5: 1;
            unsigned long long ERR6: 1;
            unsigned long long DAT0: 1;
            unsigned long long BRE: 1;
            unsigned long long BWE: 1;
            unsigned long long : 3;
            unsigned long long SCLKDIVEN: 1;
            unsigned long long CBSY: 1;
            unsigned long long ILA: 1;
            unsigned long long : 48;
        } BIT;
    } SD_INFO2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IMASK0: 1;
            unsigned long long : 1;
            unsigned long long IMASK2: 1;
            unsigned long long IMASK3: 1;
            unsigned long long IMASK4: 1;
            unsigned long long : 3;
            unsigned long long IMASK8: 1;
            unsigned long long IMASK9: 1;
            unsigned long long : 6;
            unsigned long long IMASK16: 1;
            unsigned long long : 47;
        } BIT;
    } SD_INFO1_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long EMASK0: 1;
            unsigned long long EMASK1: 1;
            unsigned long long EMASK2: 1;
            unsigned long long EMASK3: 1;
            unsigned long long EMASK4: 1;
            unsigned long long EMASK5: 1;
            unsigned long long EMASK6: 1;
            unsigned long long : 1;
            unsigned long long BMASK0: 1;
            unsigned long long BMASK1: 1;
            unsigned long long : 5;
            unsigned long long IMASK: 1;
            unsigned long long : 48;
        } BIT;
    } SD_INFO2_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DIV: 8;
            unsigned long long SCLKEN: 1;
            unsigned long long SDCLKOFFEN: 1;
            unsigned long long : 54;
        } BIT;
    } SD_CLK_CTRL;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long LEN: 10;
            unsigned long long : 54;
        } BIT;
    } SD_SIZE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long CTOP21: 1;
            unsigned long long CTOP22: 1;
            unsigned long long CTOP23: 1;
            unsigned long long CTOP24: 1;
            unsigned long long TOP24: 1;
            unsigned long long TOP25: 1;
            unsigned long long TOP26: 1;
            unsigned long long TOP27: 1;
            unsigned long long TOUTMASK: 1;
            unsigned long long EXTOP: 1;
            unsigned long long : 3;
            unsigned long long WIDTH8: 1;
            unsigned long long : 1;
            unsigned long long WIDTH: 1;
            unsigned long long : 48;
        } BIT;
    } SD_OPTION;
    char           wk8[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long E0: 1;
            unsigned long long E1: 1;
            unsigned long long E2: 1;
            unsigned long long E3: 1;
            unsigned long long E4: 1;
            unsigned long long E5: 1;
            unsigned long long : 2;
            unsigned long long E8: 1;
            unsigned long long E9: 1;
            unsigned long long E10: 1;
            unsigned long long E11: 1;
            unsigned long long E12: 1;
            unsigned long long E13: 1;
            unsigned long long E14: 1;
            unsigned long long : 49;
        } BIT;
    } SD_ERR_STS1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long E0: 1;
            unsigned long long E1: 1;
            unsigned long long E2: 1;
            unsigned long long E3: 1;
            unsigned long long E4: 1;
            unsigned long long E5: 1;
            unsigned long long E6: 1;
            unsigned long long : 57;
        } BIT;
    } SD_ERR_STS2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long BUF: 64;
        } BIT;
    } SD_BUF0;
    char           wk9[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOMOD: 1;
            unsigned long long : 1;
            unsigned long long RWREQ: 1;
            unsigned long long : 5;
            unsigned long long IOABT: 1;
            unsigned long long C52PUB: 1;
            unsigned long long : 54;
        } BIT;
    } SDIO_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOIRQ: 1;
            unsigned long long : 13;
            unsigned long long EXPUB52: 1;
            unsigned long long EXWT: 1;
            unsigned long long : 48;
        } BIT;
    } SDIO_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IOMSK: 1;
            unsigned long long : 13;
            unsigned long long MEXPUB52: 1;
            unsigned long long MEXWT: 1;
            unsigned long long : 48;
        } BIT;
    } SDIO_INFO1_MASK;
    char           wk10[632];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 1;
            unsigned long long DMASDRW: 1;
            unsigned long long : 62;
        } BIT;
    } CC_EXT_MODE;
    char           wk11[24];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SDRST: 1;
            unsigned long long : 63;
        } BIT;
    } SOFT_RST;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long IP: 8;
            unsigned long long UR: 8;
            unsigned long long : 48;
        } BIT;
    } VERSION;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long WMODE: 1;
            unsigned long long ENDIAN: 1;
            unsigned long long : 6;
            unsigned long long BUSWIDTH: 1;
            unsigned long long : 55;
        } BIT;
    } HOST_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DDR: 1;
            unsigned long long : 7;
            unsigned long long NOCHKCR: 1;
            unsigned long long : 55;
        } BIT;
    } SDIF_MODE;
    char           wk12[1128];
    char           wk13[8];
    char           wk14[16];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 4;
            unsigned long long BUS_WIDTH: 2;
            unsigned long long : 10;
            unsigned long long CH_NUM: 2;
            unsigned long long : 46;
        } BIT;
    } DM_CM_DTRAN_MODE;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long DM_START: 1;
            unsigned long long : 63;
        } BIT;
    } DM_CM_DTRAN_CTRL;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQRST: 1;
            unsigned long long : 7;
            unsigned long long DTRANRST0: 1;
            unsigned long long DTRANRST1: 1;
            unsigned long long : 54;
        } BIT;
    } DM_CM_RST;
    char           wk15[8];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQEND: 1;
            unsigned long long : 15;
            unsigned long long DTRANEND0: 1;
            unsigned long long : 3;
            unsigned long long DTRANEND1: 1;
            unsigned long long : 43;
        } BIT;
    } DM_CM_INFO1;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQEND_MASK: 1;
            unsigned long long : 15;
            unsigned long long DTRANEND0_MASK: 1;
            unsigned long long : 3;
            unsigned long long DTRANEND1_MASK: 1;
            unsigned long long : 43;
        } BIT;
    } DM_CM_INFO1_MASK;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQERR: 1;
            unsigned long long : 15;
            unsigned long long DTRANERR0: 1;
            unsigned long long DTRANERR1: 1;
            unsigned long long : 46;
        } BIT;
    } DM_CM_INFO2;
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long SEQERR_MASK: 1;
            unsigned long long : 15;
            unsigned long long DTRANERR0_MASK: 1;
            unsigned long long DTRANERR1_MASK: 1;
            unsigned long long : 46;
        } BIT;
    } DM_CM_INFO2_MASK;
    char           wk16[32];
    union {
        unsigned long long LONGLONG;
        struct {
            unsigned long L;
            unsigned long H;
        } LONG;
        struct {
            unsigned short LL;
            unsigned short LH;
            unsigned short HL;
            unsigned short HH;
        } WORD;
        struct {
            unsigned long long : 3;
            unsigned long long DADDR: 29;
            unsigned long long : 32;
        } BIT;
    } DM_DTRAN_ADDR;
};

#define SDMMC0    (*(volatile struct st_sdmmc0    *)0xE8227000)
#define SDMMC1    (*(volatile struct st_sdmmc1    *)0xE8229000)

#endif
