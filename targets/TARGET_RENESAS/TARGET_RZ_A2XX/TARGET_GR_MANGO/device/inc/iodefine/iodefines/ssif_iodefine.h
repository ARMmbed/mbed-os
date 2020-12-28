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

#ifndef SSIF_IODEFINE_H
#define SSIF_IODEFINE_H

struct st_ssif {
    union {
        unsigned long LONG;
        struct {
            unsigned long REN: 1;
            unsigned long TEN: 1;
            unsigned long : 1;
            unsigned long MUEN: 1;
            unsigned long CKDV: 4;
            unsigned long DEL: 1;
            unsigned long PDTA: 1;
            unsigned long SDTA: 1;
            unsigned long SPDP: 1;
            unsigned long LRCKP: 1;
            unsigned long BCKP: 1;
            unsigned long MST: 1;
            unsigned long : 1;
            unsigned long SWL: 3;
            unsigned long DWL: 3;
            unsigned long FRM: 2;
            unsigned long : 1;
            unsigned long IIEN: 1;
            unsigned long ROIEN: 1;
            unsigned long RUIEN: 1;
            unsigned long TOIEN: 1;
            unsigned long TUIEN: 1;
            unsigned long CKS: 1;
            unsigned long : 1;
        } BIT;
    } SSICR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 25;
            unsigned long IIRQ: 1;
            unsigned long ROIRQ: 1;
            unsigned long RUIRQ: 1;
            unsigned long TOIRQ: 1;
            unsigned long TUIRQ: 1;
            unsigned long : 2;
        } BIT;
    } SSISR;
    char           wk0[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long RFRST: 1;
            unsigned long TFRST: 1;
            unsigned long RIE: 1;
            unsigned long TIE: 1;
            unsigned long : 4;
            unsigned long RXDNCE: 1;
            unsigned long LRCKNCE: 1;
            unsigned long BCKNCE: 1;
            unsigned long BSW: 1;
            unsigned long : 4;
            unsigned long SSIRST: 1;
            unsigned long : 13;
            unsigned long : 1;
            unsigned long AUCKE: 1;
        } BIT;
    } SSIFCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RDF: 1;
            unsigned long : 7;
            unsigned long RDC: 6;
            unsigned long : 2;
            unsigned long TDE: 1;
            unsigned long : 7;
            unsigned long TDC: 6;
            unsigned long : 2;
        } BIT;
    } SSIFSR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long SSIFTDR: 32;
        } BIT;
    } SSIFTDR;
    union {
        unsigned long LONG;
        struct {
            unsigned short L;
            unsigned short H;
        } WORD;
        struct {
            unsigned char LL;
            unsigned char LH;
            unsigned char HL;
            unsigned char HH;
        } BYTE;
        struct {
            unsigned long SSIFRDR: 32;
        } BIT;
    } SSIFRDR;
    union {
        unsigned long LONG;
        struct {
            unsigned long OMOD: 2;
            unsigned long : 6;
            unsigned long LRCONT: 1;
            unsigned long BCKASTP: 1;
            unsigned long : 22;
        } BIT;
    } SSIOFR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RDFS: 5;
            unsigned long : 3;
            unsigned long TDES: 5;
            unsigned long : 19;
        } BIT;
    } SSISCR;
    char           wk1[16];
    char           wk2[4];
    char           wk3[4];
};

#define SSIF0     (*(volatile struct st_ssif      *)0xE8048000)
#define SSIF1     (*(volatile struct st_ssif      *)0xE8048800)
#define SSIF2     (*(volatile struct st_ssif      *)0xE8049000)
#define SSIF3     (*(volatile struct st_ssif      *)0xE8049800)

#endif
