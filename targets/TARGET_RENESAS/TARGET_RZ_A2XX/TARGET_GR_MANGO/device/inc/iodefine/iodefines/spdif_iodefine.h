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

#ifndef SPDIF_IODEFINE_H
#define SPDIF_IODEFINE_H

struct st_spdif {
    union {
        unsigned long LONG;
        struct {
            unsigned long TLCA: 24;
            unsigned long : 8;
        } BIT;
    } TLCA;
    union {
        unsigned long LONG;
        struct {
            unsigned long TRCA: 24;
            unsigned long : 8;
        } BIT;
    } TRCA;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long CTL: 5;
            unsigned long : 2;
            unsigned long CATCD: 8;
            unsigned long SRCNO: 4;
            unsigned long CHNO: 4;
            unsigned long FS: 4;
            unsigned long CLAC: 2;
            unsigned long : 2;
        } BIT;
    } TLCS;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long CTL: 5;
            unsigned long : 2;
            unsigned long CATCD: 8;
            unsigned long SRCNO: 4;
            unsigned long CHNO: 4;
            unsigned long FS: 4;
            unsigned long CLAC: 2;
            unsigned long : 2;
        } BIT;
    } TRCS;
    union {
        unsigned long LONG;
        struct {
            unsigned long TUI: 32;
        } BIT;
    } TUI;
    union {
        unsigned long LONG;
        struct {
            unsigned long RLCA: 24;
            unsigned long : 8;
        } BIT;
    } RLCA;
    union {
        unsigned long LONG;
        struct {
            unsigned long RRCA: 24;
            unsigned long : 8;
        } BIT;
    } RRCA;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long CTL: 5;
            unsigned long : 2;
            unsigned long CATCD: 8;
            unsigned long SRCNO: 4;
            unsigned long CHNO: 4;
            unsigned long FS: 4;
            unsigned long CLAC: 2;
            unsigned long : 2;
        } BIT;
    } RLCS;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long CTL: 5;
            unsigned long : 2;
            unsigned long CATCD: 8;
            unsigned long SRCNO: 4;
            unsigned long CHNO: 4;
            unsigned long FS: 4;
            unsigned long CLAC: 2;
            unsigned long : 2;
        } BIT;
    } RRCS;
    union {
        unsigned long LONG;
        struct {
            unsigned long RUI: 32;
        } BIT;
    } RUI;
    union {
        unsigned long LONG;
        struct {
            unsigned long TCBI: 1;
            unsigned long TCSI: 1;
            unsigned long RCBI: 1;
            unsigned long RCSI: 1;
            unsigned long TUII: 1;
            unsigned long RUII: 1;
            unsigned long ABUI: 1;
            unsigned long ABOI: 1;
            unsigned long CSEI: 1;
            unsigned long PREI: 1;
            unsigned long PAEI: 1;
            unsigned long CREI: 1;
            unsigned long UBUI: 1;
            unsigned long UBOI: 1;
            unsigned long TEIE: 1;
            unsigned long REIE: 1;
            unsigned long TME: 1;
            unsigned long RME: 1;
            unsigned long AOS: 1;
            unsigned long NCSI: 1;
            unsigned long TDE: 1;
            unsigned long RDE: 1;
            unsigned long TASS: 2;
            unsigned long RASS: 2;
            unsigned long PB: 1;
            unsigned long : 1;
            unsigned long CKS: 1;
            unsigned long : 3;
        } BIT;
    } CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBTX: 1;
            unsigned long CSTX: 1;
            unsigned long CBRX: 1;
            unsigned long CSRX: 1;
            unsigned long TUIR: 1;
            unsigned long RUIR: 1;
            unsigned long ABU: 1;
            unsigned long ABO: 1;
            unsigned long CSE: 1;
            unsigned long PREE: 1;
            unsigned long PARE: 1;
            unsigned long CE: 1;
            unsigned long UBU: 1;
            unsigned long UBO: 1;
            unsigned long TIS: 1;
            unsigned long RIS: 1;
            unsigned long CMD: 1;
            unsigned long : 15;
        } BIT;
    } STAT;
    union {
        unsigned long LONG;
        struct {
            unsigned long TDAD: 24;
            unsigned long : 8;
        } BIT;
    } TDAD;
    union {
        unsigned long LONG;
        struct {
            unsigned long RDAD: 24;
            unsigned long : 8;
        } BIT;
    } RDAD;
};

#define SPDIF     (*(volatile struct st_spdif     *)0xE804F000)

#endif
