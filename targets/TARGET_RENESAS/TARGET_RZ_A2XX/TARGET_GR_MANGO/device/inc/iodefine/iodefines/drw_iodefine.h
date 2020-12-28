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

#ifndef DRW_IODEFINE_H
#define DRW_IODEFINE_H

struct st_drw {
    union {
        unsigned long LONG;
        struct {
            unsigned long LIM1ENABLE: 1;
            unsigned long LIM2ENABLE: 1;
            unsigned long LIM3ENABLE: 1;
            unsigned long LIM4ENABLE: 1;
            unsigned long LIM5ENABLE: 1;
            unsigned long LIM6ENABLE: 1;
            unsigned long QUAD1ENABLE: 1;
            unsigned long QUAD2ENABLE: 1;
            unsigned long QUAD3ENABLE: 1;
            unsigned long LIM1THRESHOLD: 1;
            unsigned long LIM2THRESHOLD: 1;
            unsigned long LIM3THRESHOLD: 1;
            unsigned long LIM4THRESHOLD: 1;
            unsigned long LIM5THRESHOLD: 1;
            unsigned long LIM6THRESHOLD: 1;
            unsigned long BAND1ENABLE: 1;
            unsigned long BAND2ENABLE: 1;
            unsigned long UNION12: 1;
            unsigned long UNION34: 1;
            unsigned long UNION56: 1;
            unsigned long UNIONAB: 1;
            unsigned long UNIONCD: 1;
            unsigned long SPANABORT: 1;
            unsigned long SPANSTORE: 1;
            unsigned long : 8;
        } BIT;
    } CONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long PATTERNENABLE: 1;
            unsigned long TEXTUREENABLE: 1;
            unsigned long PATTERNSOURCEL5: 1;
            unsigned long USEACB: 1;
            unsigned long READFORMAT_3_2: 2;
            unsigned long BSFA: 1;
            unsigned long BDFA: 1;
            unsigned long WRITEFORMAT_2: 1;
            unsigned long BSF: 1;
            unsigned long BDF: 1;
            unsigned long BSI: 1;
            unsigned long BDI: 1;
            unsigned long BC2: 1;
            unsigned long TEXTURECLAMPX: 1;
            unsigned long TEXTURECLAMPY: 1;
            unsigned long TEXTUREFILTERX: 1;
            unsigned long TEXTUREFILTERY: 1;
            unsigned long READFORMAT_1_0: 2;
            unsigned long WRITEFORMAT_1_0: 2;
            unsigned long WRITEALPHA: 2;
            unsigned long RLEENABLE: 1;
            unsigned long CLUTENABLE: 1;
            unsigned long COLKEYENABLE: 1;
            unsigned long CLUTFORMAT: 1;
            unsigned long BSIA: 1;
            unsigned long BDIA: 1;
            unsigned long RLEPIXELWIDTH: 2;
        } BIT;
    } CONTROL2;
    char           wk0[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L1START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L2START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L3START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L4START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L5START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LSTART: 32;
        } BIT;
    } L6START;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L1XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L2XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L3XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L4XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L5XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LXADD: 32;
        } BIT;
    } L6XADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L1YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L2YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L3YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L4YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L5YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LYADD: 32;
        } BIT;
    } L6YADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LBAND: 32;
        } BIT;
    } L1BAND;
    union {
        unsigned long LONG;
        struct {
            unsigned long LBAND: 32;
        } BIT;
    } L2BAND;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long COLOR1B: 8;
            unsigned long COLOR1G: 8;
            unsigned long COLOR1R: 8;
            unsigned long COLOR1A: 8;
        } BIT;
    } COLOR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long COLOR2B: 8;
            unsigned long COLOR2G: 8;
            unsigned long COLOR2R: 8;
            unsigned long COLOR2A: 8;
        } BIT;
    } COLOR2;
    char           wk2[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long PATTERN: 8;
            unsigned long : 24;
        } BIT;
    } PATTERN;
    union {
        unsigned long LONG;
        struct {
            unsigned long SIZEX: 16;
            unsigned long SIZEY: 16;
        } BIT;
    } SIZE;
    union {
        unsigned long LONG;
        struct {
            unsigned long PITCH: 16;
            unsigned long SSD: 16;
        } BIT;
    } PITCH;
    union {
        unsigned long LONG;
        struct {
            unsigned long ORIGIN: 32;
        } BIT;
    } ORIGIN;
    char           wk3[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long LUSTART: 32;
        } BIT;
    } LUSTART;
    union {
        unsigned long LONG;
        struct {
            unsigned long LUXADD: 32;
        } BIT;
    } LUXADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LUYADD: 32;
        } BIT;
    } LUYADD;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVSTARTI: 32;
        } BIT;
    } LVSTARTI;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVSTARTF: 16;
            unsigned long : 16;
        } BIT;
    } LVSTARTF;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVXADDI: 32;
        } BIT;
    } LVXADDI;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVYADDI: 32;
        } BIT;
    } LVYADDI;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVXADDF: 16;
            unsigned long LVYADDF: 16;
        } BIT;
    } LVYXADDF;
    char           wk4[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long TEXPITCH: 11;
            unsigned long : 21;
        } BIT;
    } TEXPITCH;
    union {
        unsigned long LONG;
        struct {
            unsigned long TEXUMASK: 11;
            unsigned long TEXVMASK: 21;
        } BIT;
    } TEXMASK;
    union {
        unsigned long LONG;
        struct {
            unsigned long TEXORIGIN: 32;
        } BIT;
    } TEXORIGIN;
    union {
        unsigned long LONG;
        struct {
            unsigned long ENUMIRQEN: 1;
            unsigned long DLISTIRQEN: 1;
            unsigned long ENUMIRQCLR: 1;
            unsigned long DLISTIRQCLR: 1;
            unsigned long BUSIRQEN: 1;
            unsigned long BUSIRQCLR: 1;
            unsigned long : 26;
        } BIT;
    } IRQCTL;
    union {
        unsigned long LONG;
        struct {
            unsigned long CENABLEFX: 1;
            unsigned long CFLUSHFX: 1;
            unsigned long CENABLETX: 1;
            unsigned long CFLUSHTX: 1;
            unsigned long : 28;
        } BIT;
    } CACHECTL;
    union {
        unsigned long LONG;
        struct {
            unsigned long DLISTSTART: 32;
        } BIT;
    } DLISTSTART;
    union {
        unsigned long LONG;
        struct {
            unsigned long PERFCOUNT: 32;
        } BIT;
    } PERFCOUNT1;
    union {
        unsigned long LONG;
        struct {
            unsigned long PERFCOUNT: 32;
        } BIT;
    } PERFCOUNT2;
    union {
        unsigned long LONG;
        struct {
            unsigned long PERFTRIGGER1: 5;
            unsigned long : 11;
            unsigned long PERFTRIGGER2: 5;
            unsigned long : 11;
        } BIT;
    } PERFTRIGGER;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CLADDR: 8;
            unsigned long : 24;
        } BIT;
    } TEXCLADDR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CLDATA: 32;
        } BIT;
    } TEXCLDATA;
    union {
        unsigned long LONG;
        struct {
            unsigned long CLOFFSET: 8;
            unsigned long : 24;
        } BIT;
    } TEXCLOFFSET;
    union {
        unsigned long LONG;
        struct {
            unsigned long COLKEYB: 8;
            unsigned long COLKEYG: 8;
            unsigned long COLKEYR: 8;
            unsigned long : 8;
        } BIT;
    } COLKEY;
};

#define DRW       (*(volatile struct st_drw       *)0xE820A000)

#endif
