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

#ifndef OSTM_IODEFINE_H
#define OSTM_IODEFINE_H

struct st_ostm {
    union {
        unsigned long LONG;
        struct {
            unsigned long OSTMnCMP: 32;
        } BIT;
    } OSTMnCMP;
    union {
        unsigned long LONG;
        struct {
            unsigned long OSTMnCNT: 32;
        } BIT;
    } OSTMnCNT;
    char           wk0[1];
    char           wk1[3];
    char           wk2[1];
    char           wk3[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OSTMnTE: 1;
            unsigned char : 7;
        } BIT;
    } OSTMnTE;
    char           wk4[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OSTMnTS: 1;
            unsigned char : 7;
        } BIT;
    } OSTMnTS;
    char           wk5[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OSTMnTT: 1;
            unsigned char : 7;
        } BIT;
    } OSTMnTT;
    char           wk6[7];
    union {
        unsigned char BYTE;
        struct {
            unsigned char OSTMnMD0: 1;
            unsigned char OSTMnMD1: 1;
            unsigned char : 6;
        } BIT;
    } OSTMnCTL;
    char           wk7[3];
    char           wk8[1];
};

#define OSTM0     (*(volatile struct st_ostm      *)0xE803B000)
#define OSTM1     (*(volatile struct st_ostm      *)0xE803C000)
#define OSTM2     (*(volatile struct st_ostm      *)0xE803D000)

#endif
