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

#ifndef HYPER_IODEFINE_H
#define HYPER_IODEFINE_H

struct st_hyper {
    union {
        unsigned long LONG;
        struct {
            unsigned long RACT: 1;
            unsigned long : 7;
            unsigned long RDECERR: 1;
            unsigned long RTRSERR: 1;
            unsigned long RRSTOERR: 1;
            unsigned long RDSSTALL: 1;
            unsigned long : 4;
            unsigned long WACT: 1;
            unsigned long : 7;
            unsigned long WDECERR: 1;
            unsigned long WTRSERR: 1;
            unsigned long WRSTOERR: 1;
            unsigned long : 5;
        } BIT;
    } CSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RPCINTE: 1;
            unsigned long : 30;
            unsigned long INTP: 1;
        } BIT;
    } IEN;
    union {
        unsigned long LONG;
        struct {
            unsigned long RPCINTS: 1;
            unsigned long : 31;
        } BIT;
    } ISR;
    char           wk0[4];
    char           wk1[4];
    char           wk2[4];
    char           wk3[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 10;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long MAXLEN: 9;
            unsigned long : 4;
            unsigned long MAXEN: 1;
        } BIT;
    } MCR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 2;
            unsigned long : 2;
            unsigned long DEVTYPE: 1;
            unsigned long CRT: 1;
            unsigned long : 10;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long MAXLEN: 9;
            unsigned long : 4;
            unsigned long MAXEN: 1;
        } BIT;
    } MCR1;
    char           wk4[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 4;
            unsigned long : 4;
            unsigned long WCSH: 4;
            unsigned long RCSH: 4;
            unsigned long WCSS: 4;
            unsigned long RCSS: 4;
            unsigned long WCSHI: 4;
            unsigned long RCSHI: 4;
        } BIT;
    } MTR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long LTCY: 4;
            unsigned long : 4;
            unsigned long WCSH: 4;
            unsigned long RCSH: 4;
            unsigned long WCSS: 4;
            unsigned long RCSS: 4;
            unsigned long WCSHI: 4;
            unsigned long RCSHI: 4;
        } BIT;
    } MTR1;
    char           wk5[12];
    char           wk6[4];
    char           wk7[4];
    char           wk8[4];
    char           wk9[4];
};

#define HYPER     (*(volatile struct st_hyper     *)0x1F400000)

#endif
