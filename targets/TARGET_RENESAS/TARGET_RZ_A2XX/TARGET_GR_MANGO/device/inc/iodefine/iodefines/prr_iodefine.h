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

#ifndef PRR_IODEFINE_H
#define PRR_IODEFINE_H

struct st_prr {
    char           wk0[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long DID: 32;
        } BIT;
    } BSID;
    char           wk1[4];
    char           wk2[4];
    char           wk3[4];
    char           wk4[4];
    char           wk5[488];
    char           wk6[4];
    char           wk7[4];
    char           wk8[4];
    char           wk9[4];
    char           wk10[16];
    char           wk11[4];
    char           wk12[4];
    char           wk13[4];
    char           wk14[4];
    char           wk15[4];
    char           wk16[4];
    char           wk17[4];
    char           wk18[4];
    char           wk19[4];
    char           wk20[4];
    char           wk21[4];
    char           wk22[4];
    char           wk23[4];
    char           wk24[4];
    char           wk25[4];
    char           wk26[4];
    char           wk27[4];
    char           wk28[4];
    char           wk29[4];
    char           wk30[4];
    char           wk31[4];
    char           wk32[4];
    char           wk33[4];
    char           wk34[4];
    char           wk35[4];
    char           wk36[4];
    char           wk37[4];
    char           wk38[4];
    char           wk39[4];
    char           wk40[4];
    char           wk41[4];
    char           wk42[4];
    char           wk43[352];
    char           wk44[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long ETHAXCACHE: 2;
            unsigned long : 14;
            unsigned long JCUAWCACHE: 4;
            unsigned long : 4;
            unsigned long JCUARCACHE: 4;
            unsigned long : 4;
        } BIT;
    } AXIBUSCTL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long VINAWCACHE: 4;
            unsigned long : 12;
            unsigned long IMR20AWCACHE: 4;
            unsigned long : 4;
            unsigned long IMR20ARCACHE: 4;
            unsigned long : 4;
        } BIT;
    } AXIBUSCTL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long CEUAWCACHE: 4;
            unsigned long : 28;
        } BIT;
    } AXIBUSCTL2;
    union {
        unsigned long LONG;
        struct {
            unsigned long SDMMC1AWCACHE: 4;
            unsigned long : 4;
            unsigned long SDMMC1ARCACHE: 4;
            unsigned long : 4;
            unsigned long SDMMC0AWCACHE: 4;
            unsigned long : 4;
            unsigned long SDMMC0ARCACHE: 4;
            unsigned long : 4;
        } BIT;
    } AXIBUSCTL3;
    union {
        unsigned long LONG;
        struct {
            unsigned long DRPAWCACHE: 4;
            unsigned long : 4;
            unsigned long DRPARCACHE: 4;
            unsigned long : 4;
            unsigned long NANDAWCACHE: 4;
            unsigned long : 4;
            unsigned long NANDARCACHE: 4;
            unsigned long : 4;
        } BIT;
    } AXIBUSCTL4;
    union {
        unsigned long LONG;
        struct {
            unsigned long D2D1AXCACHE: 2;
            unsigned long : 14;
            unsigned long D2D0AXCACHE: 2;
            unsigned long : 14;
        } BIT;
    } AXIBUSCTL5;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long VDC602ARCACHE: 4;
            unsigned long : 4;
            unsigned long VDC601AWCACHE: 4;
            unsigned long : 4;
            unsigned long VDC601ARCACHE: 4;
            unsigned long : 4;
        } BIT;
    } AXIBUSCTL6;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long VDC604ARCACHE: 4;
            unsigned long : 20;
        } BIT;
    } AXIBUSCTL7;
    char           wk45[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long CEURERREN: 1;
            unsigned long : 7;
            unsigned long VINRERREN: 1;
            unsigned long : 3;
            unsigned long IMR20RERREN: 1;
            unsigned long : 7;
            unsigned long JCURERREN: 1;
            unsigned long : 3;
        } BIT;
    } AXIRERRCTL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long DRPRERREN: 1;
            unsigned long : 3;
            unsigned long NANDRERREN: 1;
            unsigned long : 3;
            unsigned long SDMMC1RERREN: 1;
            unsigned long : 3;
            unsigned long SDMMC0RERREN: 1;
            unsigned long : 3;
        } BIT;
    } AXIRERRCTL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long VDC604RERREN: 1;
            unsigned long : 7;
            unsigned long VDC602RERREN: 1;
            unsigned long : 3;
            unsigned long VDC601RERREN: 1;
            unsigned long : 3;
        } BIT;
    } AXIRERRCTL2;
    char           wk46[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long CEUBRESP: 2;
            unsigned long : 6;
            unsigned long VINBRESP: 2;
            unsigned long : 2;
            unsigned long IMR20BRESP: 2;
            unsigned long IMR20RRESP: 2;
            unsigned long : 4;
            unsigned long JCUBRESP: 2;
            unsigned long JCURRESP: 2;
        } BIT;
    } AXIRERRST0;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long DRPBRESP: 2;
            unsigned long DRPRRESP: 2;
            unsigned long NANDBRESP: 2;
            unsigned long NANDRRESP: 2;
            unsigned long SDMMC1BRESP: 2;
            unsigned long SDMMC1RRESP: 2;
            unsigned long SDMMC0BRESP: 2;
            unsigned long SDMMC0RRESP: 2;
        } BIT;
    } AXIRERRST1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 18;
            unsigned long VDC604RRESP: 2;
            unsigned long : 6;
            unsigned long VDC602RRESP: 2;
            unsigned long VDC601BRESP: 2;
            unsigned long VDC601RRESP: 2;
        } BIT;
    } AXIRERRST2;
    char           wk47[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long CEUBRESPCLR: 1;
            unsigned long : 3;
            unsigned long SERBRESPCLR: 1;
            unsigned long : 1;
            unsigned long SERRRESPCLR: 1;
            unsigned long : 1;
            unsigned long VINBRESPCLR: 1;
            unsigned long : 3;
            unsigned long IMR20BRESPCLR: 1;
            unsigned long : 1;
            unsigned long IMR20RRESPCLR: 1;
            unsigned long : 5;
            unsigned long JCUBRESPCLR: 1;
            unsigned long : 1;
            unsigned long JCURRESPCLR: 1;
            unsigned long : 1;
        } BIT;
    } AXIRERRCLR0;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long DRPBRESPCLR: 1;
            unsigned long : 1;
            unsigned long DRPRRESPCLR: 1;
            unsigned long : 1;
            unsigned long NANDBRESPCLR: 1;
            unsigned long : 1;
            unsigned long NANDRRESPCLR: 1;
            unsigned long : 1;
            unsigned long SDMMC1BRESPCLR: 1;
            unsigned long : 1;
            unsigned long SDMMC1RRESPCLR: 1;
            unsigned long : 1;
            unsigned long SDMMC0BRESPCLR: 1;
            unsigned long : 1;
            unsigned long SDMMC0RRESPCLR: 1;
            unsigned long : 1;
        } BIT;
    } AXIRERRCLR1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 18;
            unsigned long VDC604RRESPCLR: 1;
            unsigned long : 7;
            unsigned long VDC602RRESPCLR: 1;
            unsigned long : 1;
            unsigned long VDC601BRESPCLR: 1;
            unsigned long : 1;
            unsigned long VDC601RRESPCLR: 1;
            unsigned long : 1;
        } BIT;
    } AXIRERRCLR2;
    char           wk48[4];
    char           wk49[4];
    char           wk50[2972];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long VINAWNS: 1;
            unsigned long : 7;
            unsigned long IMR20AWNS: 1;
            unsigned long : 3;
            unsigned long IMR20ARNS: 1;
            unsigned long : 3;
            unsigned long ETHAxNS: 1;
            unsigned long : 7;
            unsigned long JCUAWNS: 1;
            unsigned long : 3;
            unsigned long JCUARNS: 1;
            unsigned long : 2;
        } BIT;
    } MSTACCCTL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long SDMMC1AWNS: 1;
            unsigned long : 3;
            unsigned long SDMMC1ARNS: 1;
            unsigned long : 3;
            unsigned long SDMMC0AWNS: 1;
            unsigned long : 3;
            unsigned long SDMMC0ARNS: 1;
            unsigned long : 3;
            unsigned long CEUAWNS: 1;
            unsigned long : 14;
        } BIT;
    } MSTACCCTL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long D2D1AxNS: 1;
            unsigned long : 7;
            unsigned long D2D0AxNS: 1;
            unsigned long : 7;
            unsigned long DRPAWNS: 1;
            unsigned long : 3;
            unsigned long DRPARNS: 1;
            unsigned long : 3;
            unsigned long NANDAWNS: 1;
            unsigned long : 3;
            unsigned long NANDARNS: 1;
            unsigned long : 2;
        } BIT;
    } MSTACCCTL2;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 5;
            unsigned long VDC604ARNS: 1;
            unsigned long : 15;
            unsigned long VDC602ARNS: 1;
            unsigned long : 3;
            unsigned long VDC601AWNS: 1;
            unsigned long : 3;
            unsigned long VDC601ARNS: 1;
            unsigned long : 2;
        } BIT;
    } MSTACCCTL3;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long USB11AxNS: 1;
            unsigned long : 7;
            unsigned long USB10AxNS: 1;
            unsigned long : 7;
            unsigned long USB01AxNS: 1;
            unsigned long : 7;
            unsigned long USB00AxNS: 1;
            unsigned long : 6;
        } BIT;
    } MSTACCCTL4;
    char           wk51[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long WDTNS: 1;
            unsigned long : 1;
            unsigned long INTC2NS: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 3;
            unsigned long POEGNS: 1;
            unsigned long : 1;
            unsigned long POE3NS: 1;
            unsigned long : 1;
            unsigned long GPTNS: 1;
            unsigned long : 1;
            unsigned long MTU3NS: 1;
            unsigned long : 1;
            unsigned long IMR20NS: 1;
            unsigned long : 1;
            unsigned long VDC60NS: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SYSNS: 1;
            unsigned long : 1;
        } BIT;
    } SLVACCCTL0;
    union {
        unsigned long LONG;
        struct {
            unsigned long RSPINS: 1;
            unsigned long : 1;
            unsigned long JCUNS: 1;
            unsigned long : 1;
            unsigned long SCIFNS: 1;
            unsigned long : 1;
            unsigned long SCINS: 1;
            unsigned long : 1;
            unsigned long IRDANS: 1;
            unsigned long : 1;
            unsigned long ADNS: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long SENS: 1;
            unsigned long : 1;
            unsigned long RCANNS: 1;
            unsigned long : 1;
            unsigned long SPDIFNS: 1;
            unsigned long : 1;
            unsigned long SSIFNS: 1;
            unsigned long : 1;
            unsigned long OSTM2NS: 1;
            unsigned long : 1;
            unsigned long OSTM1NS: 1;
            unsigned long : 1;
            unsigned long OSTM0NS: 1;
            unsigned long : 1;
            unsigned long I2CNS: 1;
            unsigned long : 1;
            unsigned long GPIONS: 1;
            unsigned long : 1;
        } BIT;
    } SLVACCCTL1;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 2;
            unsigned long TSIPNS: 1;
            unsigned long : 1;
            unsigned long DRPNS: 1;
            unsigned long : 1;
            unsigned long CEUNS: 1;
            unsigned long : 1;
            unsigned long USB11NS: 1;
            unsigned long : 1;
            unsigned long USB10NS: 1;
            unsigned long : 1;
            unsigned long USB01NS: 1;
            unsigned long : 1;
            unsigned long USB00NS: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long VINNS: 1;
            unsigned long : 1;
            unsigned long MIPINS: 1;
            unsigned long : 1;
            unsigned long D2DNS: 1;
            unsigned long : 3;
            unsigned long ETHNS: 1;
            unsigned long : 1;
        } BIT;
    } SLVACCCTL2;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 20;
            unsigned long CSNS: 1;
            unsigned long : 3;
            unsigned long NANDNS: 1;
            unsigned long : 1;
            unsigned long SDMMC1NS: 1;
            unsigned long : 1;
            unsigned long SDMMC0NS: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 1;
        } BIT;
    } SLVACCCTL3;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 8;
            unsigned long VRAM4NS: 1;
            unsigned long : 1;
            unsigned long VRAM3NS: 1;
            unsigned long : 1;
            unsigned long VRAM2NS: 1;
            unsigned long : 1;
            unsigned long VRAM1NS: 1;
            unsigned long : 1;
            unsigned long VRAM0NS: 1;
            unsigned long : 1;
            unsigned long RRAMNS: 1;
            unsigned long : 1;
            unsigned long HYPRNS: 1;
            unsigned long : 1;
            unsigned long HYPNS: 1;
            unsigned long : 1;
            unsigned long OCTARNS: 1;
            unsigned long : 1;
            unsigned long OCTANS: 1;
            unsigned long : 1;
            unsigned long SPINS: 1;
            unsigned long : 1;
            unsigned long BSCNS: 1;
            unsigned long : 1;
        } BIT;
    } SLVACCCTL4;
    char           wk52[460];
    char           wk53[4];
};

#define PRR       (*(volatile struct st_prr       *)0xFCFE8000)

#endif
