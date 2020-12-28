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

#ifndef POE_IODEFINE_H
#define POE_IODEFINE_H

struct st_poe {
    union {
        unsigned short WORD;
        struct {
            unsigned short POE0M: 2;
            unsigned short : 6;
            unsigned short PIE1: 1;
            unsigned short : 3;
            unsigned short POE0F: 1;
            unsigned short : 3;
        } BIT;
    } ICSR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short OIE1: 1;
            unsigned short OCE1: 1;
            unsigned short : 5;
            unsigned short OSF1: 1;
        } BIT;
    } OCSR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short POE4M: 2;
            unsigned short : 6;
            unsigned short PIE2: 1;
            unsigned short : 3;
            unsigned short POE4F: 1;
            unsigned short : 3;
        } BIT;
    } ICSR2;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 8;
            unsigned short OIE2: 1;
            unsigned short OCE2: 1;
            unsigned short : 5;
            unsigned short OSF2: 1;
        } BIT;
    } OCSR2;
    union {
        unsigned short WORD;
        struct {
            unsigned short POE8M: 2;
            unsigned short : 6;
            unsigned short PIE3: 1;
            unsigned short POE8E: 1;
            unsigned short : 2;
            unsigned short POE8F: 1;
            unsigned short : 3;
        } BIT;
    } ICSR3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MTUCH34HIZ: 1;
            unsigned char MTUCH67HIZ: 1;
            unsigned char MTUCH0HIZ: 1;
            unsigned char : 5;
        } BIT;
    } SPOER;
    union {
        unsigned char BYTE;
        struct {
            unsigned char MTU0AZE: 1;
            unsigned char MTU0BZE: 1;
            unsigned char MTU0CZE: 1;
            unsigned char MTU0DZE: 1;
            unsigned char : 4;
        } BIT;
    } POECR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short MTU7BDZE: 1;
            unsigned short MTU7ACZE: 1;
            unsigned short MTU6BDZE: 1;
            unsigned short : 5;
            unsigned short MTU4BDZE: 1;
            unsigned short MTU4ACZE: 1;
            unsigned short MTU3BDZE: 1;
            unsigned short : 5;
        } BIT;
    } POECR2;
    char           wk0[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 2;
            unsigned short IC2ADDMT34ZE: 1;
            unsigned short IC3ADDMT34ZE: 1;
            unsigned short IC4ADDMT34ZE: 1;
            unsigned short : 4;
            unsigned short IC1ADDMT67ZE: 1;
            unsigned short : 1;
            unsigned short IC3ADDMT67ZE: 1;
            unsigned short IC4ADDMT67ZE: 1;
            unsigned short : 3;
        } BIT;
    } POECR4;
    union {
        unsigned short WORD;
        struct {
            unsigned short : 1;
            unsigned short IC1ADDMT0ZE: 1;
            unsigned short IC2ADDMT0ZE: 1;
            unsigned short : 1;
            unsigned short IC4ADDMT0ZE: 1;
            unsigned short : 11;
        } BIT;
    } POECR5;
    char           wk1[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short POE10M: 2;
            unsigned short : 6;
            unsigned short PIE4: 1;
            unsigned short POE10E: 1;
            unsigned short : 2;
            unsigned short POE10F: 1;
            unsigned short : 3;
        } BIT;
    } ICSR4;
    char           wk2[12];
    union {
        unsigned char BYTE;
        struct {
            unsigned char M0ASEL: 4;
            unsigned char M0BSEL: 4;
        } BIT;
    } M0SELR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char M0CSEL: 4;
            unsigned char M0DSEL: 4;
        } BIT;
    } M0SELR2;
};

#define POE3      (*(volatile struct st_poe       *)0xE8042000)

#endif
