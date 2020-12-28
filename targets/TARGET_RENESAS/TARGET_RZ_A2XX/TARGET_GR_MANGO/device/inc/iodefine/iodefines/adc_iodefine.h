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

#ifndef ADC_IODEFINE_H
#define ADC_IODEFINE_H

struct st_adc {
    union {
        unsigned short WORD;
        struct {
            unsigned short DBLANS: 5;
            unsigned short : 1;
            unsigned short GBADIE: 1;
            unsigned short DBLE: 1;
            unsigned short EXTRG: 1;
            unsigned short TRGE: 1;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short ADIE: 1;
            unsigned short ADCS: 2;
            unsigned short ADST: 1;
        } BIT;
    } ADCSR;
    char           wk0[1];
    char           wk1[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short ANSA0: 8;
            unsigned short : 8;
        } BIT;
    } ADANSA0;
    char           wk2[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short ADS0: 8;
            unsigned short : 8;
        } BIT;
    } ADADS0;
    char           wk3[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char ADC_2_0: 3;
            unsigned char : 4;
            unsigned char AVEE: 1;
        } BIT;
    } ADADC;
    char           wk4[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 1;
            unsigned short ADPRC: 2;
            unsigned short : 1;
            unsigned short : 1;
            unsigned short ACE: 1;
            unsigned short : 2;
            unsigned short DIAGVAL: 2;
            unsigned short DIAGLD: 1;
            unsigned short DIAGM: 1;
            unsigned short : 3;
            unsigned short ADRFMT: 1;
        } BIT;
    } ADCER;
    union {
        unsigned short WORD;
        struct {
            unsigned short TRSB: 6;
            unsigned short : 2;
            unsigned short TRSA: 6;
            unsigned short : 2;
        } BIT;
    } ADSTRGR;
    char           wk5[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short ANSB0: 8;
            unsigned short : 8;
        } BIT;
    } ADANSB0;
    char           wk6[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDBLDR;
    char           wk7[2];
    char           wk8[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADRD;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR0;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR2;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR3;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR4;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR5;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR6;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDR7;
    char           wk9[2];
    char           wk10[2];
    char           wk11[2];
    char           wk12[2];
    char           wk13[2];
    char           wk14[2];
    char           wk15[2];
    char           wk16[2];
    char           wk17[2];
    char           wk18[2];
    char           wk19[2];
    char           wk20[2];
    char           wk21[2];
    char           wk22[2];
    char           wk23[2];
    char           wk24[2];
    char           wk25[2];
    char           wk26[2];
    char           wk27[2];
    char           wk28[2];
    char           wk29[2];
    char           wk30[2];
    char           wk31[2];
    char           wk32[2];
    char           wk33[2];
    char           wk34[1];
    char           wk35[1];
    char           wk36[2];
    char           wk37[2];
    char           wk38[2];
    char           wk39[2];
    char           wk40[2];
    char           wk41[2];
    char           wk42[2];
    char           wk43[2];
    char           wk44[2];
    char           wk45[2];
    char           wk46[2];
    union {
        unsigned char BYTE;
        struct {
            unsigned char ADNDIS: 5;
            unsigned char : 3;
        } BIT;
    } ADDISCR;
    char           wk47[1];
    char           wk48[1];
    char           wk49[1];
    char           wk50[1];
    char           wk51[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short PGS: 1;
            unsigned short GBRSCN: 1;
            unsigned short : 6;
            unsigned short : 1;
            unsigned short : 5;
            unsigned short LGRRS: 1;
            unsigned short GBRP: 1;
        } BIT;
    } ADGSPCR;
    char           wk52[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDBLDRA;
    union {
        unsigned short WORD;
        struct {
            unsigned short AD: 16;
        } BIT;
    } ADDBLDRB;
    char           wk53[1];
    char           wk54[1];
    char           wk55[1];
    char           wk56[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char MONCOMB: 1;
            unsigned char : 3;
            unsigned char MONCMPA: 1;
            unsigned char MONCMPB: 1;
            unsigned char : 2;
        } BIT;
    } ADWINMON;
    char           wk57[3];
    union {
        unsigned short WORD;
        struct {
            unsigned short : 2;
            unsigned short : 7;
            unsigned short CMPBE: 1;
            unsigned short : 1;
            unsigned short CMPAE: 1;
            unsigned short : 1;
            unsigned short CMPBIE: 1;
            unsigned short WCMPE: 1;
            unsigned short CMPAIE: 1;
        } BIT;
    } ADCMPCR;
    char           wk58[1];
    char           wk59[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPCHA0: 8;
            unsigned short : 8;
        } BIT;
    } ADCMPANSR0;
    char           wk60[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPLCHA0: 8;
            unsigned short : 8;
        } BIT;
    } ADCMPLR0;
    char           wk61[2];
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPD0: 16;
        } BIT;
    } ADCMPDR0;
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPD1: 16;
        } BIT;
    } ADCMPDR1;
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPSTCHA0: 8;
            unsigned short : 8;
        } BIT;
    } ADCMPSR0;
    char           wk62[2];
    char           wk63[1];
    char           wk64[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char CMPCHB: 6;
            unsigned char : 1;
            unsigned char CMPLB: 1;
        } BIT;
    } ADCMPBNSR;
    char           wk65[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPLLB: 16;
        } BIT;
    } ADWINLLB;
    union {
        unsigned short WORD;
        struct {
            unsigned short CMPULB: 16;
        } BIT;
    } ADWINULB;
    union {
        unsigned char BYTE;
        struct {
            unsigned char CMPSTB: 1;
            unsigned char : 7;
        } BIT;
    } ADCMPBSR;
    char           wk66[3];
    char           wk67[2];
    char           wk68[2];
    char           wk69[2];
    char           wk70[2];
    char           wk71[2];
    char           wk72[2];
    char           wk73[2];
    char           wk74[2];
    char           wk75[2];
    char           wk76[2];
    char           wk77[2];
    char           wk78[2];
    char           wk79[2];
    char           wk80[2];
    char           wk81[2];
    char           wk82[2];
    char           wk83[1];
    char           wk84[1];
    char           wk85[1];
    char           wk86[1];
    union {
        unsigned short WORD;
        struct {
            unsigned short ANSC0: 8;
            unsigned short : 8;
        } BIT;
    } ADANSC0;
    char           wk87[2];
    char           wk88[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char TRSC: 6;
            unsigned char GCADIE: 1;
            unsigned char GRCE: 1;
        } BIT;
    } ADGCTRGR;
    char           wk89[3];
    char           wk90[1];
    char           wk91[1];
    char           wk92[1];
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR0;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR1;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR2;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR3;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR4;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR5;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR6;
    union {
        unsigned char BYTE;
        struct {
            unsigned char SST: 8;
        } BIT;
    } ADSSTR7;
    char           wk93[1];
    char           wk94[1];
    char           wk95[1];
    char           wk96[1];
    char           wk97[1];
    char           wk98[1];
    char           wk99[1];
    char           wk100[1];
    char           wk101[176];
    char           wk102[2];
    char           wk103[2];
    char           wk104[12];
    char           wk105[2];
    char           wk106[2];
    char           wk107[1];
    char           wk108[1];
    char           wk109[42];
    char           wk110[1];
};

#define ADC       (*(volatile struct st_adc       *)0xE8005800)

#endif
