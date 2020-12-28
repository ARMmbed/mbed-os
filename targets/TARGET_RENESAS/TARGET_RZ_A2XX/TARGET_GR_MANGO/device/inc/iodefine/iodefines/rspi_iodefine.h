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

#ifndef RSPI_IODEFINE_H
#define RSPI_IODEFINE_H

struct st_rspi {
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 2;
            unsigned char MODFEN: 1;
            unsigned char MSTR: 1;
            unsigned char SPEIE: 1;
            unsigned char SPTIE: 1;
            unsigned char SPE: 1;
            unsigned char SPRIE: 1;
        } BIT;
    } SPCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SSL0P: 1;
            unsigned char : 7;
        } BIT;
    } SSLP;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SPLP: 1;
            unsigned char : 3;
            unsigned char MOIFV: 1;
            unsigned char MOIFE: 1;
            unsigned char : 2;
        } BIT;
    } SPPCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char OVRF: 1;
            unsigned char : 1;
            unsigned char MODF: 1;
            unsigned char : 2;
            unsigned char SPTEF: 1;
            unsigned char TEND: 1;
            unsigned char SPRF: 1;
        } BIT;
    } SPSR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned char SPD0: 1;
            unsigned char SPD1: 1;
            unsigned char SPD2: 1;
            unsigned char SPD3: 1;
            unsigned char SPD4: 1;
            unsigned char SPD5: 1;
            unsigned char SPD6: 1;
            unsigned char SPD7: 1;
            unsigned char SPD8: 1;
            unsigned char SPD9: 1;
            unsigned char SPD10: 1;
            unsigned char SPD11: 1;
            unsigned char SPD12: 1;
            unsigned char SPD13: 1;
            unsigned char SPD14: 1;
            unsigned char SPD15: 1;
            unsigned char SPD16: 1;
            unsigned char SPD17: 1;
            unsigned char SPD18: 1;
            unsigned char SPD19: 1;
            unsigned char SPD20: 1;
            unsigned char SPD21: 1;
            unsigned char SPD22: 1;
            unsigned char SPD23: 1;
            unsigned char SPD24: 1;
            unsigned char SPD25: 1;
            unsigned char SPD26: 1;
            unsigned char SPD27: 1;
            unsigned char SPD28: 1;
            unsigned char SPD29: 1;
            unsigned char SPD30: 1;
            unsigned char SPD31: 1;
        } BIT;
    } SPDR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SPSLN0: 1;
            unsigned char SPSLN1: 1;
            unsigned char : 6;
        } BIT;
    } SPSCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SPCP0: 1;
            unsigned char SPCP1: 1;
            unsigned char : 6;
        } BIT;
    } SPSSR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SPR0: 1;
            unsigned char SPR1: 1;
            unsigned char SPR2: 1;
            unsigned char SPR3: 1;
            unsigned char SPR4: 1;
            unsigned char SPR5: 1;
            unsigned char SPR6: 1;
            unsigned char SPR7: 1;
        } BIT;
    } SPBR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 5;
            unsigned char SPLW0: 1;
            unsigned char SPLW1: 1;
            unsigned char TXDMY: 1;
        } BIT;
    } SPDCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SCKDL0: 1;
            unsigned char SCKDL1: 1;
            unsigned char SCKDL2: 1;
            unsigned char : 5;
        } BIT;
    } SPCKD;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SLNDL0: 1;
            unsigned char SLNDL1: 1;
            unsigned char SLNDL2: 1;
            unsigned char : 5;
        } BIT;
    } SSLND;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SPNDL0: 1;
            unsigned char SPNDL1: 1;
            unsigned char SPNDL2: 1;
            unsigned char : 5;
        } BIT;
    } SPND;
    char           wk0[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short CPHA: 1;
            unsigned short CPOL: 1;
            unsigned short BRDV0: 1;
            unsigned short BRDV1: 1;
            unsigned short : 3;
            unsigned short SSLKP: 1;
            unsigned short SPB0: 1;
            unsigned short SPB1: 1;
            unsigned short SPB2: 1;
            unsigned short SPB3: 1;
            unsigned short LSBF: 1;
            unsigned short SPNDEN: 1;
            unsigned short SLNDEN: 1;
            unsigned short SCKDEN: 1;
        } BIT;
    } SPCMD0;
    union {
        unsigned short WORD;
        struct {
            unsigned short CPHA: 1;
            unsigned short CPOL: 1;
            unsigned short BRDV0: 1;
            unsigned short BRDV1: 1;
            unsigned short : 3;
            unsigned short SSLKP: 1;
            unsigned short SPB0: 1;
            unsigned short SPB1: 1;
            unsigned short SPB2: 1;
            unsigned short SPB3: 1;
            unsigned short LSBF: 1;
            unsigned short SPNDEN: 1;
            unsigned short SLNDEN: 1;
            unsigned short SCKDEN: 1;
        } BIT;
    } SPCMD1;
    union {
        unsigned short WORD;
        struct {
            unsigned short CPHA: 1;
            unsigned short CPOL: 1;
            unsigned short BRDV0: 1;
            unsigned short BRDV1: 1;
            unsigned short : 3;
            unsigned short SSLKP: 1;
            unsigned short SPB0: 1;
            unsigned short SPB1: 1;
            unsigned short SPB2: 1;
            unsigned short SPB3: 1;
            unsigned short LSBF: 1;
            unsigned short SPNDEN: 1;
            unsigned short SLNDEN: 1;
            unsigned short SCKDEN: 1;
        } BIT;
    } SPCMD2;
    union {
        unsigned short WORD;
        struct {
            unsigned short CPHA: 1;
            unsigned short CPOL: 1;
            unsigned short BRDV0: 1;
            unsigned short BRDV1: 1;
            unsigned short : 3;
            unsigned short SSLKP: 1;
            unsigned short SPB0: 1;
            unsigned short SPB1: 1;
            unsigned short SPB2: 1;
            unsigned short SPB3: 1;
            unsigned short LSBF: 1;
            unsigned short SPNDEN: 1;
            unsigned short SLNDEN: 1;
            unsigned short SCKDEN: 1;
        } BIT;
    } SPCMD3;
    char           wk1[8];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RXTRG: 3;
            unsigned char : 1;
            unsigned char TXTRG: 2;
            unsigned char RXRST: 1;
            unsigned char TXRST: 1;
        } BIT;
    } SPBFCR;
    char           wk2[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short R: 6;
            unsigned short : 2;
            unsigned short T: 4;
            unsigned short : 4;
        } BIT;
    } SPBFDR;
};

#define RSPI0     (*(volatile struct st_rspi      *)0xE800C800)
#define RSPI1     (*(volatile struct st_rspi      *)0xE800D000)
#define RSPI2     (*(volatile struct st_rspi      *)0xE800D800)

#endif
