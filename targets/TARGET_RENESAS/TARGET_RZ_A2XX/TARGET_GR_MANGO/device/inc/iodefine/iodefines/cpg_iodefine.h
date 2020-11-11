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

#ifndef CPG_IODEFINE_H
#define CPG_IODEFINE_H

struct st_cpg {
    union {
        unsigned short WORD;
        struct {
            unsigned short PFC: 2;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short BFC: 2;
            unsigned short : 2;
            unsigned short IFC: 2;
            unsigned short : 2;
            unsigned short CKOEN: 2;
            unsigned short CKOEN2: 1;
            unsigned short : 1;
        } BIT;
    } FRQCR;
    char           wk0[6];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 2;
            unsigned char ISBUSY: 1;
            unsigned char : 3;
        } BIT;
    } CPUSTS;
    char           wk1[7];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 6;
            unsigned char DEEP: 1;
            unsigned char STBY: 1;
        } BIT;
    } STBCR1;
    char           wk2[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP20: 1;
            unsigned char : 6;
            unsigned char HIZ: 1;
        } BIT;
    } STBCR2;
    char           wk3[11];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBRQ10: 1;
            unsigned char STBRQ11: 1;
            unsigned char STBRQ12: 1;
            unsigned char STBRQ13: 1;
            unsigned char : 1;
            unsigned char STBRQ15: 1;
            unsigned char : 1;
            unsigned char : 1;
        } BIT;
    } STBREQ1;
    char           wk4[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBRQ20: 1;
            unsigned char STBRQ21: 1;
            unsigned char STBRQ22: 1;
            unsigned char STBRQ23: 1;
            unsigned char STBRQ24: 1;
            unsigned char STBRQ25: 1;
            unsigned char STBRQ26: 1;
            unsigned char STBRQ27: 1;
        } BIT;
    } STBREQ2;
    char           wk5[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBRQ30: 1;
            unsigned char STBRQ31: 1;
            unsigned char STBRQ32: 1;
            unsigned char STBRQ33: 1;
            unsigned char : 4;
        } BIT;
    } STBREQ3;
    char           wk6[7];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBAK10: 1;
            unsigned char STBAK11: 1;
            unsigned char STBAK12: 1;
            unsigned char STBAK13: 1;
            unsigned char : 1;
            unsigned char STBAK15: 1;
            unsigned char : 1;
            unsigned char : 1;
        } BIT;
    } STBACK1;
    char           wk7[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBAK20: 1;
            unsigned char STBAK21: 1;
            unsigned char STBAK22: 1;
            unsigned char STBAK23: 1;
            unsigned char STBAK24: 1;
            unsigned char STBAK25: 1;
            unsigned char STBAK26: 1;
            unsigned char STBAK27: 1;
        } BIT;
    } STBACK2;
    char           wk8[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char STBAK30: 1;
            unsigned char STBAK31: 1;
            unsigned char STBAK32: 1;
            unsigned char STBAK33: 1;
            unsigned char : 4;
        } BIT;
    } STBACK3;
    char           wk9[183];
    union {
        unsigned short WORD;
        struct {
            unsigned short CKIOSEL: 2;
            unsigned short : 14;
        } BIT;
    } CKIOSEL;
    char           wk10[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short SPICR: 2;
            unsigned short : 2;
            unsigned short HYMCR: 2;
            unsigned short : 2;
            unsigned short OCTCR: 2;
            unsigned short : 6;
        } BIT;
    } SCLKSEL;
    char           wk11[762];
    union {
        unsigned char BYTE;
        struct {
            unsigned char VRAME0: 1;
            unsigned char VRAME1: 1;
            unsigned char VRAME2: 1;
            unsigned char VRAME3: 1;
            unsigned char VRAME4: 1;
            unsigned char : 3;
        } BIT;
    } SYSCR1;
    char           wk12[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char VRAMWE0: 1;
            unsigned char VRAMWE1: 1;
            unsigned char VRAMWE2: 1;
            unsigned char VRAMWE3: 1;
            unsigned char VRAMWE4: 1;
            unsigned char : 3;
        } BIT;
    } SYSCR2;
    char           wk13[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char RRAMWE0: 1;
            unsigned char RRAMWE1: 1;
            unsigned char RRAMWE2: 1;
            unsigned char RRAMWE3: 1;
            unsigned char : 4;
        } BIT;
    } SYSCR3;
    char           wk14[23];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP30: 1;
            unsigned char : 1;
            unsigned char MSTP32: 1;
            unsigned char MSTP33: 1;
            unsigned char MSTP34: 1;
            unsigned char MSTP35: 1;
            unsigned char MSTP36: 1;
            unsigned char : 1;
        } BIT;
    } STBCR3;
    char           wk15[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP40: 1;
            unsigned char MSTP41: 1;
            unsigned char MSTP42: 1;
            unsigned char MSTP43: 1;
            unsigned char MSTP44: 1;
            unsigned char MSTP45: 1;
            unsigned char MSTP46: 1;
            unsigned char MSTP47: 1;
        } BIT;
    } STBCR4;
    char           wk16[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char MSTP51: 1;
            unsigned char MSTP52: 1;
            unsigned char MSTP53: 1;
            unsigned char : 2;
            unsigned char MSTP56: 1;
            unsigned char MSTP57: 1;
        } BIT;
    } STBCR5;
    char           wk17[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP60: 1;
            unsigned char MSTP61: 1;
            unsigned char MSTP62: 1;
            unsigned char MSTP63: 1;
            unsigned char MSTP64: 1;
            unsigned char MSTP65: 1;
            unsigned char MSTP66: 1;
            unsigned char : 1;
        } BIT;
    } STBCR6;
    char           wk18[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP70: 1;
            unsigned char MSTP71: 1;
            unsigned char MSTP72: 1;
            unsigned char MSTP73: 1;
            unsigned char : 1;
            unsigned char MSTP75: 1;
            unsigned char MSTP76: 1;
            unsigned char MSTP77: 1;
        } BIT;
    } STBCR7;
    char           wk19[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char MSTP81: 1;
            unsigned char : 1;
            unsigned char MSTP83: 1;
            unsigned char MSTP84: 1;
            unsigned char MSTP85: 1;
            unsigned char MSTP86: 1;
            unsigned char MSTP87: 1;
        } BIT;
    } STBCR8;
    char           wk20[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP90: 1;
            unsigned char MSTP91: 1;
            unsigned char MSTP92: 1;
            unsigned char MSTP93: 1;
            unsigned char : 1;
            unsigned char MSTP95: 1;
            unsigned char MSTP96: 1;
            unsigned char MSTP97: 1;
        } BIT;
    } STBCR9;
    char           wk21[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MSTP100: 1;
            unsigned char MSTP101: 1;
            unsigned char MSTP102: 1;
            unsigned char MSTP103: 1;
            unsigned char MSTP104: 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char MSTP107: 1;
        } BIT;
    } STBCR10;
    char           wk22[3];
    char           wk23[1];
    char           wk24[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char SRST10: 1;
            unsigned char SRST11: 1;
            unsigned char SRST12: 1;
            unsigned char SRST13: 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char : 1;
            unsigned char AXTALE: 1;
        } BIT;
    } SWRSTCR1;
    char           wk25[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 1;
            unsigned char SRST21: 1;
            unsigned char SRST22: 1;
            unsigned char SRST23: 1;
            unsigned char SRST24: 1;
            unsigned char SRST25: 1;
            unsigned char SRST26: 1;
            unsigned char : 1;
        } BIT;
    } SWRSTCR2;
};

#define CPG       (*(volatile struct st_cpg       *)0xFCFE0010)

#endif
