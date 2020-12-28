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

#ifndef SCIFA_IODEFINE_H
#define SCIFA_IODEFINE_H

struct st_scifa {
    union {
        unsigned short WORD;
        struct {
            unsigned short CKS: 2;
            unsigned short : 1;
            unsigned short STOP: 1;
            unsigned short PM: 1;
            unsigned short PE: 1;
            unsigned short CHR: 1;
            unsigned short CM: 1;
            unsigned short : 8;
        } BIT;
    } SMR;
    union {
        union {
            unsigned char BYTE;
            struct {
                unsigned char MDDR: 8;
            } BIT;
        } MDDR;
        union {
            unsigned char BYTE;
            struct {
                unsigned char BRR: 8;
            } BIT;
        } BRR;
    } BRR_MDDR;
    char           wk0[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short CKE: 2;
            unsigned short TEIE: 1;
            unsigned short REIE: 1;
            unsigned short RE: 1;
            unsigned short TE: 1;
            unsigned short RIE: 1;
            unsigned short TIE: 1;
            unsigned short : 8;
        } BIT;
    } SCR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char FTDR: 8;
        } BIT;
    } FTDR;
    char           wk1[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short DR: 1;
            unsigned short RDF: 1;
            unsigned short PER: 1;
            unsigned short FER: 1;
            unsigned short BRK: 1;
            unsigned short TDFE: 1;
            unsigned short TEND: 1;
            unsigned short ER: 1;
            unsigned short : 8;
        } BIT;
    } FSR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char FRDR: 8;
        } BIT;
    } FRDR;
    char           wk2[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short LOOP: 1;
            unsigned short RFRST: 1;
            unsigned short TFRST: 1;
            unsigned short MCE: 1;
            unsigned short TTRG: 2;
            unsigned short RTRG: 2;
            unsigned short RSTRG: 3;
            unsigned short : 5;
        } BIT;
    } FCR;
    union {
        unsigned short WORD;
        struct {
            unsigned short R: 5;
            unsigned short : 3;
            unsigned short T: 5;
            unsigned short : 3;
        } BIT;
    } FDR;
    union {
        unsigned short WORD;
        struct {
            unsigned short SPB2DT: 1;
            unsigned short SPB2IO: 1;
            unsigned short SCKDT: 1;
            unsigned short SCKIO: 1;
            unsigned short CTS2DT: 1;
            unsigned short CTS2IO: 1;
            unsigned short RTS2DT: 1;
            unsigned short RTS2IO: 1;
            unsigned short : 8;
        } BIT;
    } SPTR;
    union {
        unsigned short WORD;
        struct {
            unsigned short ORER: 1;
            unsigned short : 1;
            unsigned short FER: 4;
            unsigned short : 2;
            unsigned short PER: 4;
            unsigned short : 4;
        } BIT;
    } LSR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char ABCS0: 1;
            unsigned char : 1;
            unsigned char NFEN: 1;
            unsigned char DIR: 1;
            unsigned char MDDRS: 1;
            unsigned char BRME: 1;
            unsigned char : 1;
            unsigned char BGDM: 1;
        } BIT;
    } SEMR;
    char           wk3[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short TFTC: 5;
            unsigned short : 2;
            unsigned short TTRGS: 1;
            unsigned short RFTC: 5;
            unsigned short : 2;
            unsigned short RTRGS: 1;
        } BIT;
    } FTCR;
};

#define SCIFA0    (*(volatile struct st_scifa     *)0xE8007000)
#define SCIFA1    (*(volatile struct st_scifa     *)0xE8007800)
#define SCIFA2    (*(volatile struct st_scifa     *)0xE8008000)
#define SCIFA3    (*(volatile struct st_scifa     *)0xE8008800)
#define SCIFA4    (*(volatile struct st_scifa     *)0xE8009000)

#endif
