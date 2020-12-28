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

#ifndef LVDS_IODEFINE_H
#define LVDS_IODEFINE_H

struct st_lvds {
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long LVDS_UPDATE: 1;
            unsigned long : 15;
        } BIT;
    } LVDS_UPDATE;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVDS_SEL0: 4;
            unsigned long LVDS_SEL1: 4;
            unsigned long LVDS_SEL2: 4;
            unsigned long : 4;
            unsigned long : 4;
            unsigned long : 2;
            unsigned long SYNC_POL: 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long SYNC_MODE: 1;
            unsigned long : 3;
        } BIT;
    } LVDSFCL;
    char           wk0[4];
    char           wk1[4];
    char           wk2[4];
    char           wk3[4];
    char           wk4[4];
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long LVDS_CLK_EN: 1;
            unsigned long : 3;
            unsigned long LVDS_ODIV_SET: 2;
            unsigned long LVDSPLL_TST: 6;
            unsigned long : 2;
            unsigned long : 6;
            unsigned long LVDS_IN_CLK_SEL: 2;
            unsigned long : 1;
            unsigned long : 5;
        } BIT;
    } LCLKSELR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LVDSPLL_PD: 1;
            unsigned long : 3;
            unsigned long LVDSPLL_OD: 2;
            unsigned long : 2;
            unsigned long LVDSPLL_RD: 3;
            unsigned long : 2;
            unsigned long : 3;
            unsigned long LVDSPLL_FD: 7;
            unsigned long : 4;
            unsigned long : 4;
            unsigned long : 1;
        } BIT;
    } LPLLSETR;
    char           wk6[4];
    char           wk7[4];
};

#define LVDS      (*(volatile struct st_lvds      *)0xFCFF7A30)

#endif
