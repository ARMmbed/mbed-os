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

#ifndef WDT_IODEFINE_H
#define WDT_IODEFINE_H

struct st_wdt {
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char CKS: 4;
            unsigned char : 1;
            unsigned char TME: 1;
            unsigned char WTIT: 1;
            unsigned char IOVF: 1;
            unsigned char : 8;
        } BIT;
    } WTCSR;
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char WTCNT: 8;
            unsigned char : 8;
        } BIT;
    } WTCNT;
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char : 6;
            unsigned char RSTE: 1;
            unsigned char WOVF: 1;
            unsigned char : 8;
        } BIT;
    } WRCSR;
    char           wk0[26];
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char PEE: 8;
            unsigned char : 8;
        } BIT;
    } PEER;
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char PERIE: 8;
            unsigned char : 8;
        } BIT;
    } PECR;
    union {
        unsigned short WORD;
        struct {
            unsigned char L;
            unsigned char H;
        } BYTE;
        struct {
            unsigned char PEF: 8;
            unsigned char : 8;
        } BIT;
    } PESR;
    char           wk1[10];
    char           wk2[2];
};

#define WDT       (*(volatile struct st_wdt       *)0xFCFE7000)

#endif
