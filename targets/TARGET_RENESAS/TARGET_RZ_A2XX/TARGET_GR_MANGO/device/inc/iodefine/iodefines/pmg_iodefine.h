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

#ifndef PMG_IODEFINE_H
#define PMG_IODEFINE_H

struct st_pmg {
    union {
        unsigned char BYTE;
        struct {
            unsigned char RRAMKP0: 1;
            unsigned char RRAMKP1: 1;
            unsigned char RRAMKP2: 1;
            unsigned char RRAMKP3: 1;
            unsigned char : 3;
            unsigned char : 1;
        } BIT;
    } RRAMKP;
    char           wk0[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char : 6;
            unsigned char RAMBOOT: 1;
            unsigned char EBUSKEEPE: 1;
        } BIT;
    } DSCTR;
    char           wk1[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short P3_1: 1;
            unsigned short P3_3: 1;
            unsigned short P6_2: 1;
            unsigned short PE_1: 1;
            unsigned short PH_1: 1;
            unsigned short PG_2: 1;
            unsigned short RTCAR0: 1;
            unsigned short RTCAR1: 1;
            unsigned short NMI: 1;
            unsigned short PG_6: 1;
            unsigned short PH_0: 1;
            unsigned short PJ_1: 1;
            unsigned short PJ_5: 1;
            unsigned short PK_2: 1;
            unsigned short PK_4: 1;
            unsigned short : 1;
        } BIT;
    } DSSSR;
    union {
        unsigned short WORD;
        struct {
            unsigned short P3_1E: 1;
            unsigned short P3_3E: 1;
            unsigned short P6_2E: 1;
            unsigned short PE_1E: 1;
            unsigned short PH_1E: 1;
            unsigned short PG_2E: 1;
            unsigned short : 2;
            unsigned short NMIE: 1;
            unsigned short PG_6E: 1;
            unsigned short PH_0E: 1;
            unsigned short PJ_1E: 1;
            unsigned short PJ_5E: 1;
            unsigned short PK_2E: 1;
            unsigned short PK_4E: 1;
            unsigned short : 1;
        } BIT;
    } DSESR;
    union {
        unsigned short WORD;
        struct {
            unsigned short P3_1F: 1;
            unsigned short P3_3F: 1;
            unsigned short P6_2F: 1;
            unsigned short PE_1F: 1;
            unsigned short PH_1F: 1;
            unsigned short PG_2F: 1;
            unsigned short RTCARF0: 1;
            unsigned short RTCARF1: 1;
            unsigned short NMIF: 1;
            unsigned short PG_6F: 1;
            unsigned short PH_0F: 1;
            unsigned short PJ_1F: 1;
            unsigned short PJ_5F: 1;
            unsigned short PK_2F: 1;
            unsigned short PK_4F: 1;
            unsigned short IOKEEP: 1;
        } BIT;
    } DSFR;
    char           wk2[2];
    char           wk3[1];
    char           wk4[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short CNTD: 8;
            unsigned short : 6;
            unsigned short : 1;
            unsigned short : 1;
        } BIT;
    } DSCNT;
    union {
        unsigned char BYTE;
        struct {
            unsigned char GAIN0: 1;
            unsigned char : 1;
            unsigned char : 6;
        } BIT;
    } XTALCTR;
    char           wk5[15];
    union {
        unsigned char BYTE;
        struct {
            unsigned char USBDSCE0: 1;
            unsigned char USBDSCE1: 1;
            unsigned char USBDSCE2: 1;
            unsigned char USBDSCE3: 1;
            unsigned char : 4;
        } BIT;
    } USBDSSSR;
    char           wk6[3];
    union {
        unsigned char BYTE;
        struct {
            unsigned char USBDSF0: 1;
            unsigned char USBDSF1: 1;
            unsigned char USBDSF2: 1;
            unsigned char USBDSF3: 1;
            unsigned char : 4;
        } BIT;
    } USBDSFR;
    char           wk7[11];
    char           wk8[2];
    char           wk9[2];
    char           wk10[2];
    char           wk11[10];
    union {
        unsigned short WORD;
        struct {
            unsigned short RTC0XT: 1;
            unsigned short RTC1XT: 1;
            unsigned short : 14;
        } BIT;
    } RTCXTALSEL;
};

#define PMG       (*(volatile struct st_pmg       *)0xFCFFC000)

#endif
