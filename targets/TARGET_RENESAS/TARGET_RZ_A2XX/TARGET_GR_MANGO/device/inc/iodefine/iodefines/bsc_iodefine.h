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

#ifndef BSC_IODEFINE_H
#define BSC_IODEFINE_H

struct st_bsc {
    union {
        unsigned long LONG;
        struct {
            unsigned long HIZCNT: 1;
            unsigned long HIZMEM: 1;
            unsigned long : 7;
            unsigned long DPRTY: 2;
            unsigned long : 13;
            unsigned long AL0: 1;
            unsigned long : 3;
            unsigned long TL0: 1;
            unsigned long : 3;
        } BIT;
    } CMNCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS0BCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS1BCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS2BCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS3BCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS4BCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 9;
            unsigned long BSZ: 2;
            unsigned long : 1;
            unsigned long TYPE: 3;
            unsigned long : 1;
            unsigned long IWRRS: 3;
            unsigned long IWRRD: 3;
            unsigned long IWRWS: 3;
            unsigned long IWRWD: 3;
            unsigned long IWW: 3;
            unsigned long : 1;
        } BIT;
    } CS5BCR;
    char           wk0[4];
    char           wk1[4];
    char           wk2[4];
    union {
        union {
            unsigned long LONG;
            struct {
                unsigned long HW: 2;
                unsigned long : 4;
                unsigned long WM: 1;
                unsigned long WR: 4;
                unsigned long SW: 2;
                unsigned long : 7;
                unsigned long BAS: 1;
                unsigned long : 11;
            } BIT;
        } CS0WCR_0;
        union {
            unsigned long LONG;
            struct {
                unsigned long : 6;
                unsigned long WM: 1;
                unsigned long W: 4;
                unsigned long : 5;
                unsigned long BW: 2;
                unsigned long : 2;
                unsigned long BST: 2;
                unsigned long : 10;
            } BIT;
        } CS0WCR_1;
        union {
            unsigned long LONG;
            struct {
                unsigned long : 6;
                unsigned long WM: 1;
                unsigned long W: 4;
                unsigned long : 5;
                unsigned long BW: 2;
                unsigned long : 14;
            } BIT;
        } CS0WCR_2;
    } CS0WCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HW: 2;
            unsigned long : 4;
            unsigned long WM: 1;
            unsigned long WR: 4;
            unsigned long SW: 2;
            unsigned long : 3;
            unsigned long WW: 3;
            unsigned long : 1;
            unsigned long BAS: 1;
            unsigned long : 11;
        } BIT;
    } CS1WCR_0;
    union {
        union {
            unsigned long LONG;
            struct {
                unsigned long : 6;
                unsigned long WM: 1;
                unsigned long WR: 4;
                unsigned long : 9;
                unsigned long BAS: 1;
                unsigned long : 11;
            } BIT;
        } CS2WCR_0;
        union {
            unsigned long LONG;
            struct {
                unsigned long : 7;
                unsigned long A2CL: 2;
                unsigned long : 23;
            } BIT;
        } CS2WCR_1;
    } CS2WCR;
    union {
        union {
            unsigned long LONG;
            struct {
                unsigned long WTRC: 2;
                unsigned long : 1;
                unsigned long TRWL: 2;
                unsigned long : 2;
                unsigned long A3CL: 2;
                unsigned long : 1;
                unsigned long WTRCD: 2;
                unsigned long : 1;
                unsigned long WTRP: 2;
                unsigned long : 17;
            } BIT;
        } CS3WCR_1;
        union {
            unsigned long LONG;
            struct {
                unsigned long : 6;
                unsigned long WM: 1;
                unsigned long WR: 4;
                unsigned long : 9;
                unsigned long BAS: 1;
                unsigned long : 11;
            } BIT;
        } CS3WCR_0;
    } CS3WCR;
    union {
        union {
            unsigned long LONG;
            struct {
                unsigned long HW: 2;
                unsigned long : 4;
                unsigned long WM: 1;
                unsigned long WR: 4;
                unsigned long SW: 2;
                unsigned long : 3;
                unsigned long WW: 3;
                unsigned long : 1;
                unsigned long BAS: 1;
                unsigned long : 11;
            } BIT;
        } CS4WCR_0;
        union {
            unsigned long LONG;
            struct {
                unsigned long HW: 2;
                unsigned long : 4;
                unsigned long WM: 1;
                unsigned long W: 4;
                unsigned long SW: 2;
                unsigned long : 3;
                unsigned long BW: 2;
                unsigned long : 2;
                unsigned long BST: 2;
                unsigned long : 10;
            } BIT;
        } CS4WCR_1;
    } CS4WCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HW: 2;
            unsigned long : 4;
            unsigned long WM: 1;
            unsigned long WR: 4;
            unsigned long SW: 2;
            unsigned long : 3;
            unsigned long WW: 3;
            unsigned long : 1;
            unsigned long MPXWBAS: 1;
            unsigned long SZSEL: 1;
            unsigned long : 10;
        } BIT;
    } CS5WCR_0;
    char           wk3[4];
    char           wk4[4];
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long A3COL: 2;
            unsigned long : 1;
            unsigned long A3ROW: 2;
            unsigned long : 3;
            unsigned long BACTV: 1;
            unsigned long PDOWN: 1;
            unsigned long RMODE: 1;
            unsigned long RFSH: 1;
            unsigned long : 1;
            unsigned long DEEP: 1;
            unsigned long : 2;
            unsigned long A2COL: 2;
            unsigned long : 1;
            unsigned long A2ROW: 2;
            unsigned long : 11;
        } BIT;
    } SDCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RRC: 3;
            unsigned long CKS: 3;
            unsigned long CMIE: 1;
            unsigned long CMF: 1;
            unsigned long : 24;
        } BIT;
    } RTCSR;
    unsigned long  RTCNT;
    unsigned long  RTCOR;
    char           wk6[4];
    unsigned long  TOSCOR0;
    unsigned long  TOSCOR1;
    unsigned long  TOSCOR2;
    unsigned long  TOSCOR3;
    unsigned long  TOSCOR4;
    unsigned long  TOSCOR5;
    char           wk7[4];
    char           wk8[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CS0TOSTF: 1;
            unsigned long CS1TOSTF: 1;
            unsigned long CS2TOSTF: 1;
            unsigned long CS3TOSTF: 1;
            unsigned long CS4TOSTF: 1;
            unsigned long CS5TOSTF: 1;
            unsigned long : 26;
        } BIT;
    } TOSTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long CS0TOEN: 1;
            unsigned long CS1TOEN: 1;
            unsigned long CS2TOEN: 1;
            unsigned long CS3TOEN: 1;
            unsigned long CS4TOEN: 1;
            unsigned long CS5TOEN: 1;
            unsigned long : 26;
        } BIT;
    } TOENR;
    char           wk9[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long SDRIDLY: 4;
            unsigned long : 12;
            unsigned long SDRODLY: 4;
            unsigned long : 12;
        } BIT;
    } ACADJ;
    char           wk10[2924];
    char           wk11[4];
    char           wk12[4];
    char           wk13[4];
    char           wk14[4];
    char           wk15[4];
    char           wk16[4];
    char           wk17[228];
    char           wk18[1];
};

#define BSC       (*(volatile struct st_bsc       *)0x1F000000)

#endif
