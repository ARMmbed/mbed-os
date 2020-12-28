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

#ifndef SPRITE_IODEFINE_H
#define SPRITE_IODEFINE_H

struct st_sprite {
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RBUSSEL: 1;
            unsigned long : 31;
        } BIT;
    } SPEA0RLSL;
    char           wk0[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RSTA0: 32;
        } BIT;
    } SPEA0STA0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RPHA0: 32;
        } BIT;
    } SPEA0PHA0;
    char           wk1[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RCM0: 2;
            unsigned long : 30;
        } BIT;
    } SPEA0RCM0;
    char           wk2[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RUP0: 1;
            unsigned long : 1;
            unsigned long : 30;
        } BIT;
    } SPEA0RUP;
    char           wk3[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0RDTH: 3;
            unsigned long : 1;
            unsigned long SPEA0RLEN: 3;
            unsigned long : 25;
        } BIT;
    } SPEA0RCFG;
    char           wk4[180];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0EN0: 1;
            unsigned long SPEA0S0EN1: 1;
            unsigned long SPEA0S0EN2: 1;
            unsigned long SPEA0S0EN3: 1;
            unsigned long SPEA0S0EN4: 1;
            unsigned long SPEA0S0EN5: 1;
            unsigned long SPEA0S0EN6: 1;
            unsigned long SPEA0S0EN7: 1;
            unsigned long SPEA0S0EN8: 1;
            unsigned long SPEA0S0EN9: 1;
            unsigned long SPEA0S0EN10: 1;
            unsigned long SPEA0S0EN11: 1;
            unsigned long SPEA0S0EN12: 1;
            unsigned long SPEA0S0EN13: 1;
            unsigned long SPEA0S0EN14: 1;
            unsigned long SPEA0S0EN15: 1;
            unsigned long : 16;
        } BIT;
    } SPEA0S0EN;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DS0: 1;
            unsigned long SPEA0S0DS1: 1;
            unsigned long SPEA0S0DS2: 1;
            unsigned long SPEA0S0DS3: 1;
            unsigned long SPEA0S0DS4: 1;
            unsigned long SPEA0S0DS5: 1;
            unsigned long SPEA0S0DS6: 1;
            unsigned long SPEA0S0DS7: 1;
            unsigned long SPEA0S0DS8: 1;
            unsigned long SPEA0S0DS9: 1;
            unsigned long SPEA0S0DS10: 1;
            unsigned long SPEA0S0DS11: 1;
            unsigned long SPEA0S0DS12: 1;
            unsigned long SPEA0S0DS13: 1;
            unsigned long SPEA0S0DS14: 1;
            unsigned long SPEA0S0DS15: 1;
            unsigned long : 16;
        } BIT;
    } SPEA0S0DS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0UP0: 1;
            unsigned long : 31;
        } BIT;
    } SPEA0S0UP;
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1EN0: 1;
            unsigned long SPEA0S1EN1: 1;
            unsigned long SPEA0S1EN2: 1;
            unsigned long SPEA0S1EN3: 1;
            unsigned long SPEA0S1EN4: 1;
            unsigned long SPEA0S1EN5: 1;
            unsigned long SPEA0S1EN6: 1;
            unsigned long SPEA0S1EN7: 1;
            unsigned long SPEA0S1EN8: 1;
            unsigned long SPEA0S1EN9: 1;
            unsigned long SPEA0S1EN10: 1;
            unsigned long SPEA0S1EN11: 1;
            unsigned long SPEA0S1EN12: 1;
            unsigned long SPEA0S1EN13: 1;
            unsigned long SPEA0S1EN14: 1;
            unsigned long SPEA0S1EN15: 1;
            unsigned long : 16;
        } BIT;
    } SPEA0S1EN;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DS0: 1;
            unsigned long SPEA0S1DS1: 1;
            unsigned long SPEA0S1DS2: 1;
            unsigned long SPEA0S1DS3: 1;
            unsigned long SPEA0S1DS4: 1;
            unsigned long SPEA0S1DS5: 1;
            unsigned long SPEA0S1DS6: 1;
            unsigned long SPEA0S1DS7: 1;
            unsigned long SPEA0S1DS8: 1;
            unsigned long SPEA0S1DS9: 1;
            unsigned long SPEA0S1DS10: 1;
            unsigned long SPEA0S1DS11: 1;
            unsigned long SPEA0S1DS12: 1;
            unsigned long SPEA0S1DS13: 1;
            unsigned long SPEA0S1DS14: 1;
            unsigned long SPEA0S1DS15: 1;
            unsigned long : 16;
        } BIT;
    } SPEA0S1DS;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1UP0: 1;
            unsigned long : 31;
        } BIT;
    } SPEA0S1UP;
    char           wk6[740];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA0: 32;
        } BIT;
    } SPEA0S0DA0;
    char           wk7[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH0: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW0: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY0: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX0: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS0;
    char           wk8[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA1: 32;
        } BIT;
    } SPEA0S0DA1;
    char           wk9[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH1: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW1: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY1: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX1: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS1;
    char           wk10[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA2: 32;
        } BIT;
    } SPEA0S0DA2;
    char           wk11[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH2: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW2: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY2: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX2: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS2;
    char           wk12[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA3: 32;
        } BIT;
    } SPEA0S0DA3;
    char           wk13[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH3: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW3: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY3: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX3: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS3;
    char           wk14[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA4: 32;
        } BIT;
    } SPEA0S0DA4;
    char           wk15[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH4: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW4: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY4: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX4: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS4;
    char           wk16[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA5: 32;
        } BIT;
    } SPEA0S0DA5;
    char           wk17[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH5: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW5: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY5: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX5: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS5;
    char           wk18[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA6: 32;
        } BIT;
    } SPEA0S0DA6;
    char           wk19[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH6: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW6: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY6: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX6: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS6;
    char           wk20[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA7: 32;
        } BIT;
    } SPEA0S0DA7;
    char           wk21[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH7: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW7: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY7: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX7: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS7;
    char           wk22[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA8: 32;
        } BIT;
    } SPEA0S0DA8;
    char           wk23[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH8: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW8: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY8: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX8: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS8;
    char           wk24[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA9: 32;
        } BIT;
    } SPEA0S0DA9;
    char           wk25[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH9: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW9: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY9: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX9: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS9;
    char           wk26[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA10: 32;
        } BIT;
    } SPEA0S0DA10;
    char           wk27[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH10: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW10: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY10: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX10: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS10;
    char           wk28[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA11: 32;
        } BIT;
    } SPEA0S0DA11;
    char           wk29[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH11: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW11: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY11: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX11: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS11;
    char           wk30[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA12: 32;
        } BIT;
    } SPEA0S0DA12;
    char           wk31[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH12: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW12: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY12: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX12: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS12;
    char           wk32[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA13: 32;
        } BIT;
    } SPEA0S0DA13;
    char           wk33[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH13: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW13: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY13: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX13: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS13;
    char           wk34[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA14: 32;
        } BIT;
    } SPEA0S0DA14;
    char           wk35[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH14: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW14: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY14: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX14: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS14;
    char           wk36[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0DA15: 32;
        } BIT;
    } SPEA0S0DA15;
    char           wk37[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0LYH15: 11;
            unsigned long : 6;
            unsigned long SPEA0S0LYW15: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0LY15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S0PSY15: 13;
            unsigned long : 4;
            unsigned long SPEA0S0PSX15: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S0PS15;
    char           wk38[528];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA0: 32;
        } BIT;
    } SPEA0S1DA0;
    char           wk39[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH0: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW0: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY0;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY0: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX0: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS0;
    char           wk40[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA1: 32;
        } BIT;
    } SPEA0S1DA1;
    char           wk41[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH1: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW1: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY1;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY1: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX1: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS1;
    char           wk42[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA2: 32;
        } BIT;
    } SPEA0S1DA2;
    char           wk43[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH2: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW2: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY2: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX2: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS2;
    char           wk44[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA3: 32;
        } BIT;
    } SPEA0S1DA3;
    char           wk45[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH3: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW3: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY3;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY3: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX3: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS3;
    char           wk46[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA4: 32;
        } BIT;
    } SPEA0S1DA4;
    char           wk47[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH4: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW4: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY4;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY4: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX4: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS4;
    char           wk48[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA5: 32;
        } BIT;
    } SPEA0S1DA5;
    char           wk49[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH5: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW5: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY5;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY5: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX5: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS5;
    char           wk50[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA6: 32;
        } BIT;
    } SPEA0S1DA6;
    char           wk51[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH6: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW6: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY6;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY6: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX6: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS6;
    char           wk52[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA7: 32;
        } BIT;
    } SPEA0S1DA7;
    char           wk53[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH7: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW7: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY7;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY7: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX7: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS7;
    char           wk54[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA8: 32;
        } BIT;
    } SPEA0S1DA8;
    char           wk55[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH8: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW8: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY8;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY8: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX8: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS8;
    char           wk56[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA9: 32;
        } BIT;
    } SPEA0S1DA9;
    char           wk57[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH9: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW9: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY9;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY9: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX9: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS9;
    char           wk58[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA10: 32;
        } BIT;
    } SPEA0S1DA10;
    char           wk59[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH10: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW10: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY10;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY10: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX10: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS10;
    char           wk60[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA11: 32;
        } BIT;
    } SPEA0S1DA11;
    char           wk61[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH11: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW11: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY11;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY11: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX11: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS11;
    char           wk62[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA12: 32;
        } BIT;
    } SPEA0S1DA12;
    char           wk63[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH12: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW12: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY12;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY12: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX12: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS12;
    char           wk64[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA13: 32;
        } BIT;
    } SPEA0S1DA13;
    char           wk65[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH13: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW13: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY13;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY13: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX13: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS13;
    char           wk66[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA14: 32;
        } BIT;
    } SPEA0S1DA14;
    char           wk67[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH14: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW14: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY14;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY14: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX14: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS14;
    char           wk68[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1DA15: 32;
        } BIT;
    } SPEA0S1DA15;
    char           wk69[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1LYH15: 11;
            unsigned long : 6;
            unsigned long SPEA0S1LYW15: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1LY15;
    union {
        unsigned long LONG;
        struct {
            unsigned long SPEA0S1PSY15: 13;
            unsigned long : 4;
            unsigned long SPEA0S1PSX15: 10;
            unsigned long : 5;
        } BIT;
    } SPEA0S1PS15;
};

#define SPRITE    (*(volatile struct st_sprite    *)0xE803E010)

#endif
