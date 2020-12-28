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

#ifndef DMAC_IODEFINE_H
#define DMAC_IODEFINE_H

struct st_dmac0 {
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_7S;
    char           wk0[256];
    union {
        unsigned long LONG;
        struct {
            unsigned long PR: 1;
            unsigned long LVINT: 1;
            unsigned long : 14;
            unsigned long LDPR: 3;
            unsigned long : 1;
            unsigned long LDCA: 4;
            unsigned long LWPR: 3;
            unsigned long : 1;
            unsigned long LWCA: 4;
        } BIT;
    } DCTRL_0_7S;
    char           wk1[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN0: 1;
            unsigned long EN1: 1;
            unsigned long EN2: 1;
            unsigned long EN3: 1;
            unsigned long EN4: 1;
            unsigned long EN5: 1;
            unsigned long EN6: 1;
            unsigned long EN7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_EN_0_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ER0: 1;
            unsigned long ER1: 1;
            unsigned long ER2: 1;
            unsigned long ER3: 1;
            unsigned long ER4: 1;
            unsigned long ER5: 1;
            unsigned long ER6: 1;
            unsigned long ER7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_ER_0_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long END0: 1;
            unsigned long END1: 1;
            unsigned long END2: 1;
            unsigned long END3: 1;
            unsigned long END4: 1;
            unsigned long END5: 1;
            unsigned long END6: 1;
            unsigned long END7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_END_0_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TC0: 1;
            unsigned long TC1: 1;
            unsigned long TC2: 1;
            unsigned long TC3: 1;
            unsigned long TC4: 1;
            unsigned long TC5: 1;
            unsigned long TC6: 1;
            unsigned long TC7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_TC_0_7S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SUS0: 1;
            unsigned long SUS1: 1;
            unsigned long SUS2: 1;
            unsigned long SUS3: 1;
            unsigned long SUS4: 1;
            unsigned long SUS5: 1;
            unsigned long SUS6: 1;
            unsigned long SUS7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_SUS_0_7S;
    char           wk2[220];
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_8S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_9S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_10S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_11S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_12S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_13S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_14S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_15S;
    char           wk3[256];
    union {
        unsigned long LONG;
        struct {
            unsigned long PR: 1;
            unsigned long LVINT: 1;
            unsigned long : 14;
            unsigned long LDPR: 3;
            unsigned long : 1;
            unsigned long LDCA: 4;
            unsigned long LWPR: 3;
            unsigned long : 1;
            unsigned long LWCA: 4;
        } BIT;
    } DCTRL_8_15S;
    char           wk4[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN8: 1;
            unsigned long EN9: 1;
            unsigned long EN10: 1;
            unsigned long EN11: 1;
            unsigned long EN12: 1;
            unsigned long EN13: 1;
            unsigned long EN14: 1;
            unsigned long EN15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_EN_8_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long ER8: 1;
            unsigned long ER9: 1;
            unsigned long ER10: 1;
            unsigned long ER11: 1;
            unsigned long ER12: 1;
            unsigned long ER13: 1;
            unsigned long ER14: 1;
            unsigned long ER15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_ER_8_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long END8: 1;
            unsigned long END9: 1;
            unsigned long END10: 1;
            unsigned long END11: 1;
            unsigned long END12: 1;
            unsigned long END13: 1;
            unsigned long END14: 1;
            unsigned long END15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_END_8_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long TC8: 1;
            unsigned long TC9: 1;
            unsigned long TC10: 1;
            unsigned long TC11: 1;
            unsigned long TC12: 1;
            unsigned long TC13: 1;
            unsigned long TC14: 1;
            unsigned long TC15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_TC_8_15S;
    union {
        unsigned long LONG;
        struct {
            unsigned long SUS8: 1;
            unsigned long SUS9: 1;
            unsigned long SUS10: 1;
            unsigned long SUS11: 1;
            unsigned long SUS12: 1;
            unsigned long SUS13: 1;
            unsigned long SUS14: 1;
            unsigned long SUS15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_SUS_8_15S;
    char           wk5[349964508];
    union {
        unsigned long LONG;
        struct {
            unsigned long CH0_RID: 2;
            unsigned long CH0_MID: 8;
            unsigned long : 6;
            unsigned long CH1_RID: 2;
            unsigned long CH1_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS0S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH2_RID: 2;
            unsigned long CH2_MID: 8;
            unsigned long : 6;
            unsigned long CH3_RID: 2;
            unsigned long CH3_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS1S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH4_RID: 2;
            unsigned long CH4_MID: 8;
            unsigned long : 6;
            unsigned long CH5_RID: 2;
            unsigned long CH5_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS2S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH6_RID: 2;
            unsigned long CH6_MID: 8;
            unsigned long : 6;
            unsigned long CH7_RID: 2;
            unsigned long CH7_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS3S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH8_RID: 2;
            unsigned long CH8_MID: 8;
            unsigned long : 6;
            unsigned long CH9_RID: 2;
            unsigned long CH9_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS4S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH10_RID: 2;
            unsigned long CH10_MID: 8;
            unsigned long : 6;
            unsigned long CH11_RID: 2;
            unsigned long CH11_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS5S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH12_RID: 2;
            unsigned long CH12_MID: 8;
            unsigned long : 6;
            unsigned long CH13_RID: 2;
            unsigned long CH13_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS6S;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH14_RID: 2;
            unsigned long CH14_MID: 8;
            unsigned long : 6;
            unsigned long CH15_RID: 2;
            unsigned long CH15_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS7S;
};

struct st_dmac1 {
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_7;
    char           wk0[256];
    union {
        unsigned long LONG;
        struct {
            unsigned long PR: 1;
            unsigned long LVINT: 1;
            unsigned long : 14;
            unsigned long LDPR: 3;
            unsigned long : 1;
            unsigned long LDCA: 4;
            unsigned long LWPR: 3;
            unsigned long : 1;
            unsigned long LWCA: 4;
        } BIT;
    } DCTRL_0_7;
    char           wk1[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN0: 1;
            unsigned long EN1: 1;
            unsigned long EN2: 1;
            unsigned long EN3: 1;
            unsigned long EN4: 1;
            unsigned long EN5: 1;
            unsigned long EN6: 1;
            unsigned long EN7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_EN_0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long ER0: 1;
            unsigned long ER1: 1;
            unsigned long ER2: 1;
            unsigned long ER3: 1;
            unsigned long ER4: 1;
            unsigned long ER5: 1;
            unsigned long ER6: 1;
            unsigned long ER7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_ER_0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long END0: 1;
            unsigned long END1: 1;
            unsigned long END2: 1;
            unsigned long END3: 1;
            unsigned long END4: 1;
            unsigned long END5: 1;
            unsigned long END6: 1;
            unsigned long END7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_END_0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long TC0: 1;
            unsigned long TC1: 1;
            unsigned long TC2: 1;
            unsigned long TC3: 1;
            unsigned long TC4: 1;
            unsigned long TC5: 1;
            unsigned long TC6: 1;
            unsigned long TC7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_TC_0_7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SUS0: 1;
            unsigned long SUS1: 1;
            unsigned long SUS2: 1;
            unsigned long SUS3: 1;
            unsigned long SUS4: 1;
            unsigned long SUS5: 1;
            unsigned long SUS6: 1;
            unsigned long SUS7: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_SUS_0_7;
    char           wk2[220];
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N0SA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N0DA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N0TB_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SA: 32;
        } BIT;
    } N1SA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long DA: 32;
        } BIT;
    } N1DA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long TB: 32;
        } BIT;
    } N1TB_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRSA: 32;
        } BIT;
    } CRSA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRDA: 32;
        } BIT;
    } CRDA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRTB: 32;
        } BIT;
    } CRTB_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long EN: 1;
            unsigned long RQST: 1;
            unsigned long TACT: 1;
            unsigned long SUS: 1;
            unsigned long ER: 1;
            unsigned long END: 1;
            unsigned long TC: 1;
            unsigned long SR: 1;
            unsigned long DL: 1;
            unsigned long DW: 1;
            unsigned long DER: 1;
            unsigned long MODE: 1;
            unsigned long : 4;
            unsigned long INTMSK: 1;
            unsigned long : 15;
        } BIT;
    } CHSTAT_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SETEN: 1;
            unsigned long CLREN: 1;
            unsigned long STG: 1;
            unsigned long SWRST: 1;
            unsigned long CLRRQ: 1;
            unsigned long CLREND: 1;
            unsigned long CLRTC: 1;
            unsigned long : 1;
            unsigned long SETSUS: 1;
            unsigned long CLRSUS: 1;
            unsigned long : 6;
            unsigned long SETINTMSK: 1;
            unsigned long CLRINTMSK: 1;
            unsigned long : 14;
        } BIT;
    } CHCTRL_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SEL: 3;
            unsigned long REQD: 1;
            unsigned long LOEN: 1;
            unsigned long HIEN: 1;
            unsigned long LVL: 1;
            unsigned long : 1;
            unsigned long AM: 3;
            unsigned long : 1;
            unsigned long SDS: 4;
            unsigned long DDS: 4;
            unsigned long SAD: 1;
            unsigned long DAD: 1;
            unsigned long TM: 1;
            unsigned long : 1;
            unsigned long DEM: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SBE: 1;
            unsigned long RSEL: 1;
            unsigned long RSW: 1;
            unsigned long REN: 1;
            unsigned long DMS: 1;
        } BIT;
    } CHCFG_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long ITVL: 16;
            unsigned long : 16;
        } BIT;
    } CHITVL_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPR: 3;
            unsigned long : 1;
            unsigned long SCA: 4;
            unsigned long DPR: 3;
            unsigned long : 1;
            unsigned long DCA: 4;
            unsigned long : 16;
        } BIT;
    } CHEXT_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long NXLA: 32;
        } BIT;
    } NXLA_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long CRLA: 32;
        } BIT;
    } CRLA_15;
    char           wk3[256];
    union {
        unsigned long LONG;
        struct {
            unsigned long PR: 1;
            unsigned long LVINT: 1;
            unsigned long : 14;
            unsigned long LDPR: 3;
            unsigned long : 1;
            unsigned long LDCA: 4;
            unsigned long LWPR: 3;
            unsigned long : 1;
            unsigned long LWCA: 4;
        } BIT;
    } DCTRL_8_15;
    char           wk4[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long EN8: 1;
            unsigned long EN9: 1;
            unsigned long EN10: 1;
            unsigned long EN11: 1;
            unsigned long EN12: 1;
            unsigned long EN13: 1;
            unsigned long EN14: 1;
            unsigned long EN15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_EN_8_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long ER8: 1;
            unsigned long ER9: 1;
            unsigned long ER10: 1;
            unsigned long ER11: 1;
            unsigned long ER12: 1;
            unsigned long ER13: 1;
            unsigned long ER14: 1;
            unsigned long ER15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_ER_8_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long END8: 1;
            unsigned long END9: 1;
            unsigned long END10: 1;
            unsigned long END11: 1;
            unsigned long END12: 1;
            unsigned long END13: 1;
            unsigned long END14: 1;
            unsigned long END15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_END_8_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long TC8: 1;
            unsigned long TC9: 1;
            unsigned long TC10: 1;
            unsigned long TC11: 1;
            unsigned long TC12: 1;
            unsigned long TC13: 1;
            unsigned long TC14: 1;
            unsigned long TC15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_TC_8_15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SUS8: 1;
            unsigned long SUS9: 1;
            unsigned long SUS10: 1;
            unsigned long SUS11: 1;
            unsigned long SUS12: 1;
            unsigned long SUS13: 1;
            unsigned long SUS14: 1;
            unsigned long SUS15: 1;
            unsigned long : 24;
        } BIT;
    } DSTAT_SUS_8_15;
    char           wk5[349944028];
    union {
        unsigned long LONG;
        struct {
            unsigned long CH0_RID: 2;
            unsigned long CH0_MID: 8;
            unsigned long : 6;
            unsigned long CH1_RID: 2;
            unsigned long CH1_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS0;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH2_RID: 2;
            unsigned long CH2_MID: 8;
            unsigned long : 6;
            unsigned long CH3_RID: 2;
            unsigned long CH3_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH4_RID: 2;
            unsigned long CH4_MID: 8;
            unsigned long : 6;
            unsigned long CH5_RID: 2;
            unsigned long CH5_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS2;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH6_RID: 2;
            unsigned long CH6_MID: 8;
            unsigned long : 6;
            unsigned long CH7_RID: 2;
            unsigned long CH7_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS3;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH8_RID: 2;
            unsigned long CH8_MID: 8;
            unsigned long : 6;
            unsigned long CH9_RID: 2;
            unsigned long CH9_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS4;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH10_RID: 2;
            unsigned long CH10_MID: 8;
            unsigned long : 6;
            unsigned long CH11_RID: 2;
            unsigned long CH11_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS5;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH12_RID: 2;
            unsigned long CH12_MID: 8;
            unsigned long : 6;
            unsigned long CH13_RID: 2;
            unsigned long CH13_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS6;
    union {
        unsigned long LONG;
        struct {
            unsigned long CH14_RID: 2;
            unsigned long CH14_MID: 8;
            unsigned long : 6;
            unsigned long CH15_RID: 2;
            unsigned long CH15_MID: 8;
            unsigned long : 6;
        } BIT;
    } DMARS7;
};

#define DMAC0     (*(volatile struct st_dmac0     *)0xE8220000)
#define DMAC1     (*(volatile struct st_dmac1     *)0xE8226000)

#endif
