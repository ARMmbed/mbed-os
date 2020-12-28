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

#ifndef POEG_IODEFINE_H
#define POEG_IODEFINE_H

struct st_poeg {
    union {
        unsigned long LONG;
        struct {
            unsigned long PIDF: 1;
            unsigned long IOCF: 1;
            unsigned long : 1;
            unsigned long SSF: 1;
            unsigned long PIDE: 1;
            unsigned long IOCE: 1;
            unsigned long : 10;
            unsigned long ST: 1;
            unsigned long : 11;
            unsigned long INV: 1;
            unsigned long NFEN: 1;
            unsigned long NFCS: 2;
        } BIT;
    } POEGGA;
    char           wk0[2044];
    union {
        unsigned long LONG;
        struct {
            unsigned long PIDF: 1;
            unsigned long IOCF: 1;
            unsigned long : 1;
            unsigned long SSF: 1;
            unsigned long PIDE: 1;
            unsigned long IOCE: 1;
            unsigned long : 10;
            unsigned long ST: 1;
            unsigned long : 11;
            unsigned long INV: 1;
            unsigned long NFEN: 1;
            unsigned long NFCS: 2;
        } BIT;
    } POEGGB;
    char           wk1[2044];
    union {
        unsigned long LONG;
        struct {
            unsigned long PIDF: 1;
            unsigned long IOCF: 1;
            unsigned long : 1;
            unsigned long SSF: 1;
            unsigned long PIDE: 1;
            unsigned long IOCE: 1;
            unsigned long : 10;
            unsigned long ST: 1;
            unsigned long : 11;
            unsigned long INV: 1;
            unsigned long NFEN: 1;
            unsigned long NFCS: 2;
        } BIT;
    } POEGGC;
    char           wk2[2044];
    union {
        unsigned long LONG;
        struct {
            unsigned long PIDF: 1;
            unsigned long IOCF: 1;
            unsigned long : 1;
            unsigned long SSF: 1;
            unsigned long PIDE: 1;
            unsigned long IOCE: 1;
            unsigned long : 10;
            unsigned long ST: 1;
            unsigned long : 11;
            unsigned long INV: 1;
            unsigned long NFEN: 1;
            unsigned long NFCS: 2;
        } BIT;
    } POEGGD;
};

#define POEG      (*(volatile struct st_poeg      *)0xE8044000)

#endif
