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

#ifndef CEU_IODEFINE_H
#define CEU_IODEFINE_H

struct st_ceu {
    union {
        unsigned long LONG;
        struct {
            unsigned long CE: 1;
            unsigned long : 15;
            unsigned long CPKIL: 1;
            unsigned long : 15;
        } BIT;
    } CAPSR;
    union {
        unsigned long LONG;
        struct {
            unsigned long : 16;
            unsigned long CTNCP: 1;
            unsigned long : 3;
            unsigned long MTCM: 2;
            unsigned long : 2;
            unsigned long FDRP: 8;
        } BIT;
    } CAPCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HDPOL: 1;
            unsigned long VDPOL: 1;
            unsigned long : 2;
            unsigned long JPG: 2;
            unsigned long : 2;
            unsigned long DTARY: 2;
            unsigned long : 2;
            unsigned long DTIF: 1;
            unsigned long : 3;
            unsigned long FLDPOL: 1;
            unsigned long : 7;
            unsigned long DSEL: 1;
            unsigned long FLDSEL: 1;
            unsigned long HDSEL: 1;
            unsigned long VDSEL: 1;
            unsigned long : 4;
        } BIT;
    } CAMCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HCYL: 14;
            unsigned long : 2;
            unsigned long VCYL: 14;
            unsigned long : 2;
        } BIT;
    } CMCYR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HOFST: 13;
            unsigned long : 3;
            unsigned long VOFST: 12;
            unsigned long : 4;
        } BIT;
    } CAMOR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long HWDTH: 13;
            unsigned long : 3;
            unsigned long VWDTH: 12;
            unsigned long : 4;
        } BIT;
    } CAPWR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long FCI: 2;
            unsigned long : 2;
            unsigned long CIM: 1;
            unsigned long : 3;
            unsigned long IFS: 1;
            unsigned long : 23;
        } BIT;
    } CAIFR;
    char           wk0[4];
    char           wk1[4];
    char           wk2[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long RC: 1;
            unsigned long RS: 1;
            unsigned long : 2;
            unsigned long RVS: 1;
            unsigned long : 27;
        } BIT;
    } CRCNTR;
    union {
        unsigned long LONG;
        struct {
            unsigned long RA: 1;
            unsigned long : 31;
        } BIT;
    } CRCMPR;
    union {
        unsigned long LONG;
        struct {
            unsigned long HFRAC: 12;
            unsigned long HMANT: 4;
            unsigned long VFRAC: 12;
            unsigned long VMANT: 4;
        } BIT;
    } CFLCR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long HFCLP: 12;
            unsigned long : 4;
            unsigned long VFCLP: 12;
            unsigned long : 4;
        } BIT;
    } CFSZR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CHDW: 13;
            unsigned long : 19;
        } BIT;
    } CDWDR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR: 32;
        } BIT;
    } CDAYR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR: 32;
        } BIT;
    } CDACR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR: 32;
        } BIT;
    } CDBYR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR: 32;
        } BIT;
    } CDBCR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBVS: 23;
            unsigned long : 9;
        } BIT;
    } CBDSR_A;
    char           wk3[12];
    union {
        unsigned long LONG;
        struct {
            unsigned long FWE: 1;
            unsigned long : 4;
            unsigned long FWV: 27;
        } BIT;
    } CFWCR;
    union {
        unsigned long LONG;
        struct {
            unsigned long LPF: 1;
            unsigned long : 31;
        } BIT;
    } CLFCR_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long COBS: 1;
            unsigned long COWS: 1;
            unsigned long COLS: 1;
            unsigned long : 1;
            unsigned long CDS: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long CBE: 1;
            unsigned long : 15;
        } BIT;
    } CDOCR_A;
    char           wk4[4];
    char           wk5[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CPEIE: 1;
            unsigned long CFEIE: 1;
            unsigned long : 2;
            unsigned long IGRWIE: 1;
            unsigned long : 3;
            unsigned long HDIE: 1;
            unsigned long VDIE: 1;
            unsigned long : 2;
            unsigned long CPBE1IE: 1;
            unsigned long CPBE2IE: 1;
            unsigned long CPBE3IE: 1;
            unsigned long CPBE4IE: 1;
            unsigned long CDTOFIE: 1;
            unsigned long IGHSIE: 1;
            unsigned long IGVSIE: 1;
            unsigned long : 1;
            unsigned long VBPIE: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long FWFIE: 1;
            unsigned long NHDIE: 1;
            unsigned long NVDIE: 1;
            unsigned long : 6;
        } BIT;
    } CEIER;
    union {
        unsigned long LONG;
        struct {
            unsigned long CPE: 1;
            unsigned long CFE: 1;
            unsigned long : 2;
            unsigned long IGRW: 1;
            unsigned long : 3;
            unsigned long HD: 1;
            unsigned long VD: 1;
            unsigned long : 2;
            unsigned long CPBE1: 1;
            unsigned long CPBE2: 1;
            unsigned long CPBE3: 1;
            unsigned long CPBE4: 1;
            unsigned long CDTOF: 1;
            unsigned long IGHS: 1;
            unsigned long IGVS: 1;
            unsigned long : 1;
            unsigned long VBP: 1;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long FWF: 1;
            unsigned long NHD: 1;
            unsigned long NVD: 1;
            unsigned long : 6;
        } BIT;
    } CETCR;
    char           wk6[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CPTON: 1;
            unsigned long : 7;
            unsigned long : 1;
            unsigned long : 1;
            unsigned long : 6;
            unsigned long CPFLD: 1;
            unsigned long : 7;
            unsigned long CRST: 1;
            unsigned long : 7;
        } BIT;
    } CSTSR;
    char           wk7[4];
    union {
        unsigned long LONG;
        struct {
            unsigned long CDSS: 32;
        } BIT;
    } CDSSR;
    char           wk8[8];
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR2: 32;
        } BIT;
    } CDAYR2_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR2: 32;
        } BIT;
    } CDACR2_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR2: 32;
        } BIT;
    } CDBYR2_A;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR2: 32;
        } BIT;
    } CDBCR2_A;
    char           wk9[3952];
    union {
        unsigned long LONG;
        struct {
            unsigned long HOFST: 13;
            unsigned long : 3;
            unsigned long VOFST: 12;
            unsigned long : 4;
        } BIT;
    } CAMOR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long HWDTH: 13;
            unsigned long : 3;
            unsigned long VWDTH: 12;
            unsigned long : 4;
        } BIT;
    } CAPWR_B;
    char           wk10[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long HFRAC: 12;
            unsigned long HMANT: 4;
            unsigned long VFRAC: 12;
            unsigned long VMANT: 4;
        } BIT;
    } CFLCR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long HFCLP: 12;
            unsigned long : 4;
            unsigned long VFCLP: 12;
            unsigned long : 4;
        } BIT;
    } CFSZR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CHDW: 13;
            unsigned long : 19;
        } BIT;
    } CDWDR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR: 32;
        } BIT;
    } CDAYR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR: 32;
        } BIT;
    } CDACR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR: 32;
        } BIT;
    } CDBYR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR: 32;
        } BIT;
    } CDBCR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBVS: 23;
            unsigned long : 9;
        } BIT;
    } CBDSR_B;
    char           wk11[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long LPF: 1;
            unsigned long : 31;
        } BIT;
    } CLFCR_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long COBS: 1;
            unsigned long COWS: 1;
            unsigned long COLS: 1;
            unsigned long : 1;
            unsigned long CDS: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long CBE: 1;
            unsigned long : 15;
        } BIT;
    } CDOCR_B;
    char           wk12[4];
    char           wk13[4];
    char           wk14[32];
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR2: 32;
        } BIT;
    } CDAYR2_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR2: 32;
        } BIT;
    } CDACR2_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR2: 32;
        } BIT;
    } CDBYR2_B;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR2: 32;
        } BIT;
    } CDBCR2_B;
    char           wk15[3952];
    union {
        unsigned long LONG;
        struct {
            unsigned long HOFST: 13;
            unsigned long : 3;
            unsigned long VOFST: 12;
            unsigned long : 4;
        } BIT;
    } CAMOR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long HWDTH: 13;
            unsigned long : 3;
            unsigned long VWDTH: 12;
            unsigned long : 4;
        } BIT;
    } CAPWR_M;
    char           wk16[24];
    union {
        unsigned long LONG;
        struct {
            unsigned long HFRAC: 12;
            unsigned long HMANT: 4;
            unsigned long VFRAC: 12;
            unsigned long VMANT: 4;
        } BIT;
    } CFLCR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long HFCLP: 12;
            unsigned long : 4;
            unsigned long VFCLP: 12;
            unsigned long : 4;
        } BIT;
    } CFSZR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CHDW: 13;
            unsigned long : 19;
        } BIT;
    } CDWDR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR: 32;
        } BIT;
    } CDAYR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR: 32;
        } BIT;
    } CDACR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR: 32;
        } BIT;
    } CDBYR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR: 32;
        } BIT;
    } CDBCR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBVS: 23;
            unsigned long : 9;
        } BIT;
    } CBDSR_M;
    char           wk17[16];
    union {
        unsigned long LONG;
        struct {
            unsigned long LPF: 1;
            unsigned long : 31;
        } BIT;
    } CLFCR_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long COBS: 1;
            unsigned long COWS: 1;
            unsigned long COLS: 1;
            unsigned long : 1;
            unsigned long CDS: 1;
            unsigned long : 3;
            unsigned long : 1;
            unsigned long : 7;
            unsigned long CBE: 1;
            unsigned long : 15;
        } BIT;
    } CDOCR_M;
    char           wk18[4];
    char           wk19[4];
    char           wk20[32];
    union {
        unsigned long LONG;
        struct {
            unsigned long CAYR2: 32;
        } BIT;
    } CDAYR2_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CACR2: 32;
        } BIT;
    } CDACR2_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBYR2: 32;
        } BIT;
    } CDBYR2_M;
    union {
        unsigned long LONG;
        struct {
            unsigned long CBCR2: 32;
        } BIT;
    } CDBCR2_M;
};

#define CEU       (*(volatile struct st_ceu       *)0xE8210000)

#endif
