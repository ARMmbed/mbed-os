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

#ifndef PL_IODEFINE_H
#define PL_IODEFINE_H

struct st_pl {
    union {
        unsigned long LONG;
        struct {
            unsigned long RTLrelease: 6;
            unsigned long Partnumber: 4;
            unsigned long CACHEID: 6;
            unsigned long : 8;
            unsigned long Implementer: 8;
        } BIT;
    } REG0_CACHE_ID;
    union {
        unsigned long LONG;
        struct {
            unsigned long L2cachelinelength1: 2;
            unsigned long : 4;
            unsigned long L2associativity1: 1;
            unsigned long Isize: 5;
            unsigned long L2cachelinelength0: 2;
            unsigned long : 4;
            unsigned long L2associativity0: 1;
            unsigned long Dsize: 5;
            unsigned long H: 1;
            unsigned long ctype: 4;
            unsigned long : 2;
            unsigned long Databanking: 1;
        } BIT;
    } REG0_CACHE_TYPE;
    char           wk0[248];
    union {
        unsigned long LONG;
        struct {
            unsigned long L2Cacheenable: 1;
            unsigned long : 31;
        } BIT;
    } REG1_CONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long FullLineofZeroEnable: 1;
            unsigned long : 9;
            unsigned long HighPriorityforSOandDevReadsEnable: 1;
            unsigned long StorebufferdevicelimitationEnable: 1;
            unsigned long Exclusivecacheconfiguration: 1;
            unsigned long SharedAttributeInvalidateEnable: 1;
            unsigned long : 2;
            unsigned long Associativity: 1;
            unsigned long Waysize: 3;
            unsigned long Eventmonitorbusenable: 1;
            unsigned long Parityenable: 1;
            unsigned long Sharedattributeoverrideenable: 1;
            unsigned long Forcewriteallocate: 2;
            unsigned long Cachereplacementpolicy: 1;
            unsigned long Nonsecurelockdownenable: 1;
            unsigned long Nonsecureinterruptaccesscontrol: 1;
            unsigned long Dataprefetchenable: 1;
            unsigned long Instructionprefetchenable: 1;
            unsigned long EarlyBRESPenable: 1;
            unsigned long : 1;
        } BIT;
    } REG1_AUX_CONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long RAMsetuplatency: 3;
            unsigned long : 1;
            unsigned long RAMreadaccesslatency: 3;
            unsigned long : 1;
            unsigned long RAMwriteaccesslatency: 3;
            unsigned long : 21;
        } BIT;
    } REG1_TAG_RAM_CONTROL;
    union {
        unsigned long LONG;
        struct {
            unsigned long RAMsetuplatency: 3;
            unsigned long : 1;
            unsigned long RAMreadaccesslatency: 3;
            unsigned long : 1;
            unsigned long RAMwriteaccesslatency: 3;
            unsigned long : 21;
        } BIT;
    } REG1_DATA_RAM_CONTROL;
    char           wk1[240];
    union {
        unsigned long LONG;
        struct {
            unsigned long Eventcounterenable: 1;
            unsigned long Counterreset: 2;
            unsigned long : 29;
        } BIT;
    } REG2_EV_COUNTER_CTRL;
    union {
        unsigned long LONG;
        struct {
            unsigned long Eventcounterinterruptgeneration: 2;
            unsigned long Countereventsource: 4;
            unsigned long : 26;
        } BIT;
    } REG2_EV_COUNTER1_CFG;
    union {
        unsigned long LONG;
        struct {
            unsigned long Eventcounterinterruptgeneration: 2;
            unsigned long Countereventsource: 4;
            unsigned long : 26;
        } BIT;
    } REG2_EV_COUNTER0_CFG;
    union {
        unsigned long LONG;
        struct {
            unsigned long Countervalue: 32;
        } BIT;
    } REG2_EV_COUNTER1;
    union {
        unsigned long LONG;
        struct {
            unsigned long Countervalue: 32;
        } BIT;
    } REG2_EV_COUNTER0;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECNTR: 1;
            unsigned long PARRT: 1;
            unsigned long PARRD: 1;
            unsigned long ERRWT: 1;
            unsigned long ERRWD: 1;
            unsigned long ERRRT: 1;
            unsigned long ERRRD: 1;
            unsigned long SLVERR: 1;
            unsigned long DECERR: 1;
            unsigned long : 23;
        } BIT;
    } REG2_INT_MASK;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECNTR: 1;
            unsigned long PARRT: 1;
            unsigned long PARRD: 1;
            unsigned long ERRWT: 1;
            unsigned long ERRWD: 1;
            unsigned long ERRRT: 1;
            unsigned long ERRRD: 1;
            unsigned long SLVERR: 1;
            unsigned long DECERR: 1;
            unsigned long : 23;
        } BIT;
    } REG2_INT_MASK_STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECNTR: 1;
            unsigned long PARRT: 1;
            unsigned long PARRD: 1;
            unsigned long ERRWT: 1;
            unsigned long ERRWD: 1;
            unsigned long ERRRT: 1;
            unsigned long ERRRD: 1;
            unsigned long SLVERR: 1;
            unsigned long DECERR: 1;
            unsigned long : 23;
        } BIT;
    } REG2_INT_RAW_STATUS;
    union {
        unsigned long LONG;
        struct {
            unsigned long ECNTR: 1;
            unsigned long PARRT: 1;
            unsigned long PARRD: 1;
            unsigned long ERRWT: 1;
            unsigned long ERRWD: 1;
            unsigned long ERRRT: 1;
            unsigned long ERRRD: 1;
            unsigned long SLVERR: 1;
            unsigned long DECERR: 1;
            unsigned long : 23;
        } BIT;
    } REG2_INT_CLEAR;
    char           wk2[1292];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 31;
        } BIT;
    } REG7_CACHE_SYNC;
    char           wk3[60];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 4;
            unsigned long INDEX: 9;
            unsigned long TAG: 18;
        } BIT;
    } REG7_INV_PA;
    char           wk4[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long Way_bits: 8;
            unsigned long : 24;
        } BIT;
    } REG7_INV_WAY;
    char           wk5[48];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 4;
            unsigned long INDEX: 9;
            unsigned long TAG: 18;
        } BIT;
    } REG7_CLEAN_PA;
    char           wk6[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 4;
            unsigned long INDEX: 9;
            unsigned long : 14;
            unsigned long Way: 3;
            unsigned long : 1;
        } BIT;
    } REG7_CLEAN_INDEX;
    union {
        unsigned long LONG;
        struct {
            unsigned long Way_bits: 8;
            unsigned long : 24;
        } BIT;
    } REG7_CLEAN_WAY;
    char           wk7[48];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 4;
            unsigned long INDEX: 9;
            unsigned long TAG: 18;
        } BIT;
    } REG7_CLEAN_INV_PA;
    char           wk8[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long C: 1;
            unsigned long : 4;
            unsigned long INDEX: 9;
            unsigned long : 14;
            unsigned long Way: 3;
            unsigned long : 1;
        } BIT;
    } REG7_CLEAN_INV_INDEX;
    union {
        unsigned long LONG;
        struct {
            unsigned long Way_bits: 8;
            unsigned long : 24;
        } BIT;
    } REG7_CLEAN_INV_WAY;
    char           wk9[256];
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK000: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN0;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK000: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN0;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK001: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN1;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK001: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN1;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK002: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN2;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK002: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN2;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK003: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN3;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK003: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN3;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK004: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN4;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK004: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN4;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK005: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN5;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK005: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN5;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK006: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN6;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK006: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN6;
    union {
        unsigned long LONG;
        struct {
            unsigned long DATALOCK007: 8;
            unsigned long : 24;
        } BIT;
    } REG9_D_LOCKDOWN7;
    union {
        unsigned long LONG;
        struct {
            unsigned long INSTRLOCK007: 8;
            unsigned long : 24;
        } BIT;
    } REG9_I_LOCKDOWN7;
    char           wk10[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long lockdown_by_line_enable: 1;
            unsigned long : 31;
        } BIT;
    } REG9_LOCK_LINE_EN;
    union {
        unsigned long LONG;
        struct {
            unsigned long unlock_all_lines_by_way_operation: 8;
            unsigned long : 24;
        } BIT;
    } REG9_UNLOCK_WAY;
    char           wk11[680];
    union {
        unsigned long LONG;
        struct {
            unsigned long address_filtering_enable: 1;
            unsigned long : 19;
            unsigned long address_filtering_start: 12;
        } BIT;
    } REG12_ADDR_FILTERING_START;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 20;
            unsigned long address_filtering_end: 12;
        } BIT;
    } REG12_ADDR_FILTERING_END;
    char           wk12[824];
    union {
        unsigned long LONG;
        struct {
            unsigned long DCL: 1;
            unsigned long DWB: 1;
            unsigned long SPNIDEN: 1;
            unsigned long : 29;
        } BIT;
    } REG15_DEBUG_CTRL;
    char           wk13[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long Prefetchoffset: 5;
            unsigned long : 16;
            unsigned long NotsameIDonexclusivesequenceenable: 1;
            unsigned long : 1;
            unsigned long IncrdoubleLinefillenable: 1;
            unsigned long Prefetchdropenable: 1;
            unsigned long : 2;
            unsigned long DoublelinefillonWRAPreaddisable: 1;
            unsigned long Dataprefetchenable: 1;
            unsigned long Instructionprefetchenable: 1;
            unsigned long Doublelinefillenable: 1;
            unsigned long : 1;
        } BIT;
    } REG15_PREFETCH_CTRL;
    char           wk14[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long standby_mode_en: 1;
            unsigned long dynamic_clk_gating_en: 1;
            unsigned long : 30;
        } BIT;
    } REG15_POWER_CTRL;
};

#define pl310     (*(volatile struct st_pl        *)0x1F003000)

#endif
