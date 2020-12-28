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

#ifndef RTC_IODEFINE_H
#define RTC_IODEFINE_H

struct st_rtc {
    union {
        unsigned char BYTE;
        struct {
            unsigned char F64HZ: 1;
            unsigned char F32HZ: 1;
            unsigned char F16HZ: 1;
            unsigned char F8HZ: 1;
            unsigned char F4HZ: 1;
            unsigned char F2HZ: 1;
            unsigned char F1HZ: 1;
            unsigned char : 1;
        } BIT;
    } R64CNT;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char SEC1: 4;
            unsigned char SEC10: 3;
            unsigned char : 1;
        } BIT;
    } RSECCNT;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MIN1: 4;
            unsigned char MIN10: 3;
            unsigned char : 1;
        } BIT;
    } RMINCNT;
    char           wk2[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char HR1: 4;
            unsigned char HR10: 2;
            unsigned char PM: 1;
            unsigned char : 1;
        } BIT;
    } RHRCNT;
    char           wk3[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DAYW: 3;
            unsigned char : 5;
        } BIT;
    } RWKCNT;
    char           wk4[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DATE1: 4;
            unsigned char DATE10: 2;
            unsigned char : 2;
        } BIT;
    } RDAYCNT;
    char           wk5[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MON1: 4;
            unsigned char MON10: 1;
            unsigned char : 3;
        } BIT;
    } RMONCNT;
    char           wk6[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short YR1: 4;
            unsigned short YR10: 4;
            unsigned short : 8;
        } BIT;
    } RYRCNT;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SEC1: 4;
            unsigned char SEC10: 3;
            unsigned char ENB: 1;
        } BIT;
    } RSECAR;
    char           wk7[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MIN1: 4;
            unsigned char MIN10: 3;
            unsigned char ENB: 1;
        } BIT;
    } RMINAR;
    char           wk8[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char HR1: 4;
            unsigned char HR10: 2;
            unsigned char PM: 1;
            unsigned char ENB: 1;
        } BIT;
    } RHRAR;
    char           wk9[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DAYW: 3;
            unsigned char : 4;
            unsigned char ENB: 1;
        } BIT;
    } RWKAR;
    char           wk10[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DATE1: 4;
            unsigned char DATE10: 2;
            unsigned char : 1;
            unsigned char ENB: 1;
        } BIT;
    } RDAYAR;
    char           wk11[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MON1: 4;
            unsigned char MON10: 1;
            unsigned char : 2;
            unsigned char ENB: 1;
        } BIT;
    } RMONAR;
    char           wk12[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short YR1: 4;
            unsigned short YR10: 4;
            unsigned short : 8;
        } BIT;
    } RYRAR;
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 7;
            unsigned char ENB: 1;
        } BIT;
    } RYRAREN;
    char           wk13[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char AF: 1;
            unsigned char CF: 1;
            unsigned char PF: 1;
            unsigned char : 5;
        } BIT;
    } RSR;
    char           wk14[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char AIE: 1;
            unsigned char CIE: 1;
            unsigned char PIE: 1;
            unsigned char : 1;
            unsigned char PES: 4;
        } BIT;
    } RCR1;
    char           wk15[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char START: 1;
            unsigned char RESET: 1;
            unsigned char ADJ30: 1;
            unsigned char : 1;
            unsigned char AADJE: 1;
            unsigned char AADJP: 1;
            unsigned char HR24: 1;
            unsigned char CNTMD: 1;
        } BIT;
    } RCR2;
    char           wk16[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RTCEN: 1;
            unsigned char : 7;
        } BIT;
    } RCR3;
    char           wk17[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RCKSEL: 1;
            unsigned char : 7;
        } BIT;
    } RCR4;
    char           wk18[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short RFC: 1;
            unsigned short : 15;
        } BIT;
    } RFRH;
    union {
        unsigned short WORD;
        struct {
            unsigned short RFC: 16;
        } BIT;
    } RFRL;
    union {
        unsigned char BYTE;
        struct {
            unsigned char ADJ: 6;
            unsigned char PMADJ: 2;
        } BIT;
    } RADJ;
    char           wk19[3];
    char           wk20[1];
    char           wk21[5];
    char           wk22[1];
    char           wk23[7];
    char           wk24[1];
    char           wk25[1];
    char           wk26[1];
    char           wk27[1];
    char           wk28[1];
    char           wk29[13];
    char           wk30[1];
    char           wk31[1];
    char           wk32[1];
    char           wk33[1];
    char           wk34[1];
    char           wk35[3];
    char           wk36[1];
    char           wk37[1];
    char           wk38[1];
    char           wk39[5];
    char           wk40[1];
    char           wk41[1];
    char           wk42[1];
    char           wk43[1];
    char           wk44[1];
    char           wk45[3];
    char           wk46[1];
    char           wk47[1];
    char           wk48[1];
    char           wk49[5];
    char           wk50[1];
    char           wk51[1];
    char           wk52[1];
    char           wk53[1];
    char           wk54[1];
    char           wk55[3];
    char           wk56[1];
    char           wk57[1];
    char           wk58[1];
};

struct st_rtc_bcnt {
    union {
        unsigned char BYTE;
        struct {
            unsigned char F64HZ: 1;
            unsigned char F32HZ: 1;
            unsigned char F16HZ: 1;
            unsigned char F8HZ: 1;
            unsigned char F4HZ: 1;
            unsigned char F2HZ: 1;
            unsigned char F1HZ: 1;
            unsigned char : 1;
        } BIT;
    } R64CNT;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNT: 8;
        } BIT;
    } BCNT0;
    char           wk1[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNT: 8;
        } BIT;
    } BCNT1;
    char           wk2[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNT: 8;
        } BIT;
    } BCNT2;
    char           wk3[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNT: 8;
        } BIT;
    } BCNT3;
    char           wk4[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char DATE1: 4;
            unsigned char DATE10: 2;
            unsigned char : 2;
        } BIT;
    } RDAYCNT;
    char           wk5[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MON1: 4;
            unsigned char MON10: 1;
            unsigned char : 3;
        } BIT;
    } RMONCNT;
    char           wk6[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short YR1: 4;
            unsigned short YR10: 4;
            unsigned short : 8;
        } BIT;
    } RYRCNT;
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNTAR: 8;
        } BIT;
    } BCNT0AR;
    char           wk7[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNTAR: 8;
        } BIT;
    } BCNT1AR;
    char           wk8[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNTAR: 8;
        } BIT;
    } BCNT2AR;
    char           wk9[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char BCNTAR: 8;
        } BIT;
    } BCNT3AR;
    char           wk10[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char ENB: 8;
        } BIT;
    } BCNT0AER;
    char           wk11[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char ENB: 8;
        } BIT;
    } BCNT1AER;
    char           wk12[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short ENB: 8;
            unsigned short : 8;
        } BIT;
    } BCNT2AER;
    union {
        unsigned char BYTE;
        struct {
            unsigned char ENB: 8;
        } BIT;
    } BCNT3AER;
    char           wk13[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char AF: 1;
            unsigned char CF: 1;
            unsigned char PF: 1;
            unsigned char : 5;
        } BIT;
    } RSR;
    char           wk14[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char AIE: 1;
            unsigned char CIE: 1;
            unsigned char PIE: 1;
            unsigned char : 1;
            unsigned char PES: 4;
        } BIT;
    } RCR1;
    char           wk15[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char START: 1;
            unsigned char RESET: 1;
            unsigned char ADJ30: 1;
            unsigned char : 1;
            unsigned char AADJE: 1;
            unsigned char AADJP: 1;
            unsigned char HR24: 1;
            unsigned char CNTMD: 1;
        } BIT;
    } RCR2;
    char           wk16[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RTCEN: 1;
            unsigned char : 7;
        } BIT;
    } RCR3;
    char           wk17[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RCKSEL: 1;
            unsigned char : 7;
        } BIT;
    } RCR4;
    char           wk18[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short RFC: 1;
            unsigned short : 15;
        } BIT;
    } RFRH;
    union {
        unsigned short WORD;
        struct {
            unsigned short RFC: 16;
        } BIT;
    } RFRL;
    union {
        unsigned char BYTE;
        struct {
            unsigned char ADJ: 6;
            unsigned char PMADJ: 2;
        } BIT;
    } RADJ;
    char           wk19[3];
    char           wk20[1];
    char           wk21[5];
    char           wk22[1];
    char           wk23[7];
    char           wk24[1];
    char           wk25[1];
    char           wk26[1];
    char           wk27[1];
    char           wk28[1];
    char           wk29[13];
    char           wk30[1];
    char           wk31[1];
    char           wk32[1];
    char           wk33[1];
    char           wk34[1];
    char           wk35[3];
    char           wk36[1];
    char           wk37[1];
    char           wk38[1];
    char           wk39[5];
    char           wk40[1];
    char           wk41[1];
    char           wk42[1];
    char           wk43[1];
    char           wk44[1];
    char           wk45[3];
    char           wk46[1];
    char           wk47[1];
    char           wk48[1];
    char           wk49[5];
    char           wk50[1];
    char           wk51[1];
    char           wk52[1];
    char           wk53[1];
    char           wk54[1];
    char           wk55[3];
    char           wk56[1];
    char           wk57[1];
    char           wk58[1];
};

#define RTC0      (*(volatile struct st_rtc       *)0xFCFFD000)
#define RTC1      (*(volatile struct st_rtc       *)0xFCFF1000)
#define RTC_BCNT0 (*(volatile struct st_rtc_bcnt  *)0xFCFFD000)
#define RTC_BCNT1 (*(volatile struct st_rtc_bcnt  *)0xFCFF1000)

#endif
