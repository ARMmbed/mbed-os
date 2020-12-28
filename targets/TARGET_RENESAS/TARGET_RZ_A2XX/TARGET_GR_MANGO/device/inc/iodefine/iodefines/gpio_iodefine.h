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

#ifndef GPIO_IODEFINE_H
#define GPIO_IODEFINE_H

struct st_gpio {
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P00PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P01PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P02PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P03PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P04PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P05PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P06PFS;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P10PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P11PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P12PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P13PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P14PFS;
    char           wk1[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P20PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P21PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P22PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P23PFS;
    char           wk2[4];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P30PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P31PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P32PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P33PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P34PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P35PFS;
    char           wk3[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P40PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P41PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P42PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P43PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P44PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P45PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P46PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P47PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P50PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P51PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P52PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P53PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P54PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P55PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P56PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P57PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P60PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P61PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P62PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P63PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P64PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P65PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P66PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P67PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P70PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P71PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P72PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P73PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P74PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P75PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P76PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P77PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P80PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P81PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P82PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P83PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P84PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P85PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P86PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P87PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P90PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P91PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P92PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P93PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P94PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P95PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P96PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } P97PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PA7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PB5PFS;
    char           wk4[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PC7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PD7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PE6PFS;
    char           wk5[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PF7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PG7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PH6PFS;
    char           wk6[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ5PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ6PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PJ7PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK4PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PK5PFS;
    char           wk7[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PL0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PL1PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PL2PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PL3PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PL4PFS;
    char           wk8[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PM0PFS;
    union {
        unsigned char BYTE;
        struct {
            unsigned char PSEL: 3;
            unsigned char : 3;
            unsigned char ISEL: 1;
            unsigned char : 1;
        } BIT;
    } PM1PFS;
    char           wk9[85];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 6;
            unsigned char PFSWE: 1;
            unsigned char B0WI: 1;
        } BIT;
    } PWPR;
    char           wk10[1312];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PHYMODE0: 1;
            unsigned char PHYMODE1: 1;
            unsigned char : 6;
        } BIT;
    } PFENET;
    char           wk11[223];
    union {
        unsigned long LONG;
        struct {
            unsigned long POC0: 1;
            unsigned long : 1;
            unsigned long POC2: 1;
            unsigned long POC3: 1;
            unsigned long : 4;
            unsigned long POCSEL0: 1;
            unsigned long : 1;
            unsigned long : 22;
        } BIT;
    } PPOC;
    char           wk12[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long SD0_CLK_DRV: 2;
            unsigned long SD0_CMD_DRV: 2;
            unsigned long SD0_DAT0_DRV: 2;
            unsigned long SD0_DAT1_DRV: 2;
            unsigned long SD0_DAT2_DRV: 2;
            unsigned long SD0_DAT3_DRV: 2;
            unsigned long SD0_CLK_TDSEL: 2;
            unsigned long : 2;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 1;
        } BIT;
    } PSDMMC0;
    char           wk13[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long SD0_DAT4_DRV: 2;
            unsigned long SD0_DAT5_DRV: 2;
            unsigned long SD0_DAT6_DRV: 2;
            unsigned long SD0_DAT7_DRV: 2;
            unsigned long SD0_RSTN_DRV: 2;
            unsigned long : 6;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 1;
        } BIT;
    } PSDMMC1;
    char           wk14[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long SD1_CLK_DRV: 2;
            unsigned long SD1_CMD_DRV: 2;
            unsigned long SD1_DAT0_DRV: 2;
            unsigned long SD1_DAT1_DRV: 2;
            unsigned long SD1_DAT2_DRV: 2;
            unsigned long SD1_DAT3_DRV: 2;
            unsigned long SD1_CLK_TDSEL: 2;
            unsigned long : 2;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 3;
            unsigned long : 1;
        } BIT;
    } PSDMMC2;
    char           wk15[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long QSPI0_SPCLK_DRV: 2;
            unsigned long QSPI0_IO0_DRV: 2;
            unsigned long QSPI0_IO1_DRV: 2;
            unsigned long QSPI0_IO2_DRV: 2;
            unsigned long QSPI0_IO3_DRV: 2;
            unsigned long QSPI0_SSL_DRV: 2;
            unsigned long RPC_RESETN_DRV: 2;
            unsigned long RPC_WPN_DRV: 2;
            unsigned long QSPI1_SPCLK_DRV: 2;
            unsigned long QSPI1_IO0_DRV: 2;
            unsigned long QSPI1_IO1_DRV: 2;
            unsigned long QSPI1_IO2_DRV: 2;
            unsigned long QSPI1_IO3_DRV: 2;
            unsigned long QSPI1_SSL_DRV: 2;
            unsigned long : 4;
        } BIT;
    } PSPIBSC;
    char           wk16[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long HOSEL: 1;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 1;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
            unsigned long : 2;
        } BIT;
    } PHMOM0;
    char           wk17[28];
    char           wk18[4];
    char           wk19[28];
    union {
        unsigned long LONG;
        struct {
            unsigned long ET0_EXOUT_SEL: 1;
            unsigned long ET1_EXOUT_SEL: 1;
            unsigned long VBUS0_SEL: 1;
            unsigned long VBUS1_SEL: 1;
            unsigned long : 28;
        } BIT;
    } PMODEPFS;
    char           wk20[12];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CKIO_DRV: 2;
            unsigned char : 6;
        } BIT;
    } PCKIO;
};

struct st_port0 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short : 2;
        } BIT;
    } PDR;
    char           wk0[62];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char : 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char : 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char : 1;
        } BIT;
    } PMR;
    char           wk3[191];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short : 2;
        } BIT;
    } DSCR;
};

struct st_port1 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short : 6;
        } BIT;
    } PDR;
    char           wk0[61];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char : 3;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char : 3;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char : 3;
        } BIT;
    } PMR;
    char           wk3[192];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short : 6;
        } BIT;
    } DSCR;
};

struct st_port2 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short : 8;
        } BIT;
    } PDR;
    char           wk0[60];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char : 4;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char : 4;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char : 4;
        } BIT;
    } PMR;
    char           wk3[193];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short : 8;
        } BIT;
    } DSCR;
};

struct st_port3 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short : 4;
        } BIT;
    } PDR;
    char           wk0[59];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char : 2;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char : 2;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char : 2;
        } BIT;
    } PMR;
    char           wk3[194];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short : 4;
        } BIT;
    } DSCR;
};

struct st_port4 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[58];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[195];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_port5 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[57];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[196];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_port6 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[56];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[197];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_port7 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[55];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[198];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_port8 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[54];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[199];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_port9 {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[53];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[200];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_porta {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[52];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[201];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_portb {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short : 4;
        } BIT;
    } PDR;
    char           wk0[51];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char : 2;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char : 2;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char : 2;
        } BIT;
    } PMR;
    char           wk3[202];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short : 4;
        } BIT;
    } DSCR;
};

struct st_portc {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[50];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[203];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_portd {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[49];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[204];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_porte {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short : 2;
        } BIT;
    } PDR;
    char           wk0[48];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char : 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char : 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char : 1;
        } BIT;
    } PMR;
    char           wk3[205];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short : 2;
        } BIT;
    } DSCR;
};

struct st_portf {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[47];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[206];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_portg {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[46];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[207];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_porth {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short : 2;
        } BIT;
    } PDR;
    char           wk0[45];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char : 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char : 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char : 1;
        } BIT;
    } PMR;
    char           wk3[208];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short : 2;
        } BIT;
    } DSCR;
};

struct st_portj {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short PDR6: 2;
            unsigned short PDR7: 2;
        } BIT;
    } PDR;
    char           wk0[44];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char PODR6: 1;
            unsigned char PODR7: 1;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char PIDR6: 1;
            unsigned char PIDR7: 1;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char PMR6: 1;
            unsigned char PMR7: 1;
        } BIT;
    } PMR;
    char           wk3[209];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short DSCR6: 2;
            unsigned short DSCR7: 2;
        } BIT;
    } DSCR;
};

struct st_portk {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short PDR5: 2;
            unsigned short : 4;
        } BIT;
    } PDR;
    char           wk0[43];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char PODR5: 1;
            unsigned char : 2;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char PIDR5: 1;
            unsigned char : 2;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char PMR5: 1;
            unsigned char : 2;
        } BIT;
    } PMR;
    char           wk3[210];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short DSCR5: 2;
            unsigned short : 4;
        } BIT;
    } DSCR;
};

struct st_portl {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short PDR2: 2;
            unsigned short PDR3: 2;
            unsigned short PDR4: 2;
            unsigned short : 6;
        } BIT;
    } PDR;
    char           wk0[42];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char PODR2: 1;
            unsigned char PODR3: 1;
            unsigned char PODR4: 1;
            unsigned char : 3;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char PIDR2: 1;
            unsigned char PIDR3: 1;
            unsigned char PIDR4: 1;
            unsigned char : 3;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char PMR2: 1;
            unsigned char PMR3: 1;
            unsigned char PMR4: 1;
            unsigned char : 3;
        } BIT;
    } PMR;
    char           wk3[211];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short DSCR2: 2;
            unsigned short DSCR3: 2;
            unsigned short DSCR4: 2;
            unsigned short : 6;
        } BIT;
    } DSCR;
};

struct st_portm {
    union {
        unsigned short WORD;
        struct {
            unsigned short PDR0: 2;
            unsigned short PDR1: 2;
            unsigned short : 12;
        } BIT;
    } PDR;
    char           wk0[41];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PODR0: 1;
            unsigned char PODR1: 1;
            unsigned char : 6;
        } BIT;
    } PODR;
    char           wk1[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PIDR0: 1;
            unsigned char PIDR1: 1;
            unsigned char : 6;
        } BIT;
    } PIDR;
    char           wk2[31];
    union {
        unsigned char BYTE;
        struct {
            unsigned char PMR0: 1;
            unsigned char PMR1: 1;
            unsigned char : 6;
        } BIT;
    } PMR;
    char           wk3[212];
    union {
        unsigned short WORD;
        struct {
            unsigned short DSCR0: 2;
            unsigned short DSCR1: 2;
            unsigned short : 12;
        } BIT;
    } DSCR;
};

#define GPIO      (*(volatile struct st_gpio      *)0xFCFFE200)
#define PORT0     (*(volatile struct st_port0     *)0xFCFFE000)
#define PORT1     (*(volatile struct st_port1     *)0xFCFFE002)
#define PORT2     (*(volatile struct st_port2     *)0xFCFFE004)
#define PORT3     (*(volatile struct st_port3     *)0xFCFFE006)
#define PORT4     (*(volatile struct st_port4     *)0xFCFFE008)
#define PORT5     (*(volatile struct st_port5     *)0xFCFFE00A)
#define PORT6     (*(volatile struct st_port6     *)0xFCFFE00C)
#define PORT7     (*(volatile struct st_port7     *)0xFCFFE00E)
#define PORT8     (*(volatile struct st_port8     *)0xFCFFE010)
#define PORT9     (*(volatile struct st_port9     *)0xFCFFE012)
#define PORTA     (*(volatile struct st_porta     *)0xFCFFE014)
#define PORTB     (*(volatile struct st_portb     *)0xFCFFE016)
#define PORTC     (*(volatile struct st_portc     *)0xFCFFE018)
#define PORTD     (*(volatile struct st_portd     *)0xFCFFE01A)
#define PORTE     (*(volatile struct st_porte     *)0xFCFFE01C)
#define PORTF     (*(volatile struct st_portf     *)0xFCFFE01E)
#define PORTG     (*(volatile struct st_portg     *)0xFCFFE020)
#define PORTH     (*(volatile struct st_porth     *)0xFCFFE022)
#define PORTJ     (*(volatile struct st_portj     *)0xFCFFE024)
#define PORTK     (*(volatile struct st_portk     *)0xFCFFE026)
#define PORTL     (*(volatile struct st_portl     *)0xFCFFE028)
#define PORTM     (*(volatile struct st_portm     *)0xFCFFE02A)

#endif
