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
* Description : IO bitmask header
*******************************************************************************/

#ifndef OCTA_IOBITMASK_H
#define OCTA_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define OCTA_DCR_DVCMD0                                                        (0x000000FFu)
#define OCTA_DCR_DVCMD0_SHIFT                                                  (0u)
#define OCTA_DCR_DVCMD1                                                        (0x0000FF00u)
#define OCTA_DCR_DVCMD1_SHIFT                                                  (8u)
#define OCTA_DAR_DVAD0                                                         (0x000000FFu)
#define OCTA_DAR_DVAD0_SHIFT                                                   (0u)
#define OCTA_DAR_DVAD1                                                         (0x0000FF00u)
#define OCTA_DAR_DVAD1_SHIFT                                                   (8u)
#define OCTA_DAR_DVAD2                                                         (0x00FF0000u)
#define OCTA_DAR_DVAD2_SHIFT                                                   (16u)
#define OCTA_DAR_DVAD3                                                         (0xFF000000u)
#define OCTA_DAR_DVAD3_SHIFT                                                   (24u)
#define OCTA_DCSR_DALEN                                                        (0x000000FFu)
#define OCTA_DCSR_DALEN_SHIFT                                                  (0u)
#define OCTA_DCSR_DMLEN                                                        (0x0000FF00u)
#define OCTA_DCSR_DMLEN_SHIFT                                                  (8u)
#define OCTA_DCSR_ACDV                                                         (0x00080000u)
#define OCTA_DCSR_ACDV_SHIFT                                                   (19u)
#define OCTA_DCSR_CMDLEN                                                       (0x00700000u)
#define OCTA_DCSR_CMDLEN_SHIFT                                                 (20u)
#define OCTA_DCSR_DAOR                                                         (0x00800000u)
#define OCTA_DCSR_DAOR_SHIFT                                                   (23u)
#define OCTA_DCSR_ADLEN                                                        (0x07000000u)
#define OCTA_DCSR_ADLEN_SHIFT                                                  (24u)
#define OCTA_DCSR_DOPI                                                         (0x08000000u)
#define OCTA_DCSR_DOPI_SHIFT                                                   (27u)
#define OCTA_DCSR_ACDA                                                         (0x10000000u)
#define OCTA_DCSR_ACDA_SHIFT                                                   (28u)
#define OCTA_DSR0_DV0SZ                                                        (0x3FFFFFFFu)
#define OCTA_DSR0_DV0SZ_SHIFT                                                  (0u)
#define OCTA_DSR0_DV0TYP                                                       (0xC0000000u)
#define OCTA_DSR0_DV0TYP_SHIFT                                                 (30u)
#define OCTA_DSR1_DV1SZ                                                        (0x3FFFFFFFu)
#define OCTA_DSR1_DV1SZ_SHIFT                                                  (0u)
#define OCTA_DSR1_DV1TYP                                                       (0xC0000000u)
#define OCTA_DSR1_DV1TYP_SHIFT                                                 (30u)
#define OCTA_MDTR_DV0DEL                                                       (0x000000FFu)
#define OCTA_MDTR_DV0DEL_SHIFT                                                 (0u)
#define OCTA_MDTR_DQSERAM                                                      (0x00000F00u)
#define OCTA_MDTR_DQSERAM_SHIFT                                                (8u)
#define OCTA_MDTR_DQSESOPI                                                     (0x0000F000u)
#define OCTA_MDTR_DQSESOPI_SHIFT                                               (12u)
#define OCTA_MDTR_DV1DEL                                                       (0x00FF0000u)
#define OCTA_MDTR_DV1DEL_SHIFT                                                 (16u)
#define OCTA_MDTR_DQSEDOPI                                                     (0x0F000000u)
#define OCTA_MDTR_DQSEDOPI_SHIFT                                               (24u)
#define OCTA_ACTR_CTP                                                          (0xFFFFFFFFu)
#define OCTA_ACTR_CTP_SHIFT                                                    (0u)
#define OCTA_ACAR0_CAD0                                                        (0xFFFFFFFFu)
#define OCTA_ACAR0_CAD0_SHIFT                                                  (0u)
#define OCTA_ACAR1_CAD1                                                        (0xFFFFFFFFu)
#define OCTA_ACAR1_CAD1_SHIFT                                                  (0u)
#define OCTA_DRCSTR_CTRW0                                                      (0x0000007Fu)
#define OCTA_DRCSTR_CTRW0_SHIFT                                                (0u)
#define OCTA_DRCSTR_CTR0                                                       (0x00000080u)
#define OCTA_DRCSTR_CTR0_SHIFT                                                 (7u)
#define OCTA_DRCSTR_DVRDCMD0                                                   (0x00000700u)
#define OCTA_DRCSTR_DVRDCMD0_SHIFT                                             (8u)
#define OCTA_DRCSTR_DVRDHI0                                                    (0x00003800u)
#define OCTA_DRCSTR_DVRDHI0_SHIFT                                              (11u)
#define OCTA_DRCSTR_DVRDLO0                                                    (0x0000C000u)
#define OCTA_DRCSTR_DVRDLO0_SHIFT                                              (14u)
#define OCTA_DRCSTR_DVRDCMD1                                                   (0x07000000u)
#define OCTA_DRCSTR_DVRDCMD1_SHIFT                                             (24u)
#define OCTA_DRCSTR_DVRDHI1                                                    (0x38000000u)
#define OCTA_DRCSTR_DVRDHI1_SHIFT                                              (27u)
#define OCTA_DRCSTR_DVRDLO1                                                    (0xC0000000u)
#define OCTA_DRCSTR_DVRDLO1_SHIFT                                              (30u)
#define OCTA_DWCSTR_DVWCMD0                                                    (0x00000700u)
#define OCTA_DWCSTR_DVWCMD0_SHIFT                                              (8u)
#define OCTA_DWCSTR_DVWHI0                                                     (0x00003800u)
#define OCTA_DWCSTR_DVWHI0_SHIFT                                               (11u)
#define OCTA_DWCSTR_DVWLO0                                                     (0x0000C000u)
#define OCTA_DWCSTR_DVWLO0_SHIFT                                               (14u)
#define OCTA_DWCSTR_DVWCMD1                                                    (0x07000000u)
#define OCTA_DWCSTR_DVWCMD1_SHIFT                                              (24u)
#define OCTA_DWCSTR_DVWHI1                                                     (0x38000000u)
#define OCTA_DWCSTR_DVWHI1_SHIFT                                               (27u)
#define OCTA_DWCSTR_DVWLO1                                                     (0xC0000000u)
#define OCTA_DWCSTR_DVWLO1_SHIFT                                               (30u)
#define OCTA_DCSTR_DVSELCMD                                                    (0x00000700u)
#define OCTA_DCSTR_DVSELCMD_SHIFT                                              (8u)
#define OCTA_DCSTR_DVSELHI                                                     (0x00003800u)
#define OCTA_DCSTR_DVSELHI_SHIFT                                               (11u)
#define OCTA_DCSTR_DVSELLO                                                     (0x0000C000u)
#define OCTA_DCSTR_DVSELLO_SHIFT                                               (14u)
#define OCTA_CDSR_DV0TTYP                                                      (0x00000003u)
#define OCTA_CDSR_DV0TTYP_SHIFT                                                (0u)
#define OCTA_CDSR_DV1TTYP                                                      (0x0000000Cu)
#define OCTA_CDSR_DV1TTYP_SHIFT                                                (2u)
#define OCTA_CDSR_DV0PC                                                        (0x00000010u)
#define OCTA_CDSR_DV0PC_SHIFT                                                  (4u)
#define OCTA_CDSR_DV1PC                                                        (0x00000020u)
#define OCTA_CDSR_DV1PC_SHIFT                                                  (5u)
#define OCTA_CDSR_ACMEME                                                       (0x00000C00u)
#define OCTA_CDSR_ACMEME_SHIFT                                                 (10u)
#define OCTA_CDSR_ACMODE                                                       (0x00003000u)
#define OCTA_CDSR_ACMODE_SHIFT                                                 (12u)
#define OCTA_CDSR_DLFT                                                         (0x80000000u)
#define OCTA_CDSR_DLFT_SHIFT                                                   (31u)
#define OCTA_MDLR_DV0RDL                                                       (0x000000FFu)
#define OCTA_MDLR_DV0RDL_SHIFT                                                 (0u)
#define OCTA_MDLR_DV0WDL                                                       (0x0000FF00u)
#define OCTA_MDLR_DV0WDL_SHIFT                                                 (8u)
#define OCTA_MDLR_DV1RDL                                                       (0x00FF0000u)
#define OCTA_MDLR_DV1RDL_SHIFT                                                 (16u)
#define OCTA_MDLR_DV1WDL                                                       (0xFF000000u)
#define OCTA_MDLR_DV1WDL_SHIFT                                                 (24u)
#define OCTA_MRWCR0_D0MRCMD0                                                   (0x000000FFu)
#define OCTA_MRWCR0_D0MRCMD0_SHIFT                                             (0u)
#define OCTA_MRWCR0_D0MRCMD1                                                   (0x0000FF00u)
#define OCTA_MRWCR0_D0MRCMD1_SHIFT                                             (8u)
#define OCTA_MRWCR0_D0MWCMD0                                                   (0x00FF0000u)
#define OCTA_MRWCR0_D0MWCMD0_SHIFT                                             (16u)
#define OCTA_MRWCR0_D0MWCMD1                                                   (0xFF000000u)
#define OCTA_MRWCR0_D0MWCMD1_SHIFT                                             (24u)
#define OCTA_MRWCR1_D1MRCMD0                                                   (0x000000FFu)
#define OCTA_MRWCR1_D1MRCMD0_SHIFT                                             (0u)
#define OCTA_MRWCR1_D1MRCMD1                                                   (0x0000FF00u)
#define OCTA_MRWCR1_D1MRCMD1_SHIFT                                             (8u)
#define OCTA_MRWCR1_D1MWCMD0                                                   (0x00FF0000u)
#define OCTA_MRWCR1_D1MWCMD0_SHIFT                                             (16u)
#define OCTA_MRWCR1_D1MWCMD1                                                   (0xFF000000u)
#define OCTA_MRWCR1_D1MWCMD1_SHIFT                                             (24u)
#define OCTA_MRWCSR_MRAL0                                                      (0x00000007u)
#define OCTA_MRWCSR_MRAL0_SHIFT                                                (0u)
#define OCTA_MRWCSR_MRCL0                                                      (0x00000038u)
#define OCTA_MRWCSR_MRCL0_SHIFT                                                (3u)
#define OCTA_MRWCSR_MRO0                                                       (0x00000040u)
#define OCTA_MRWCSR_MRO0_SHIFT                                                 (6u)
#define OCTA_MRWCSR_MWAL0                                                      (0x00000700u)
#define OCTA_MRWCSR_MWAL0_SHIFT                                                (8u)
#define OCTA_MRWCSR_MWCL0                                                      (0x00003800u)
#define OCTA_MRWCSR_MWCL0_SHIFT                                                (11u)
#define OCTA_MRWCSR_MWO0                                                       (0x00004000u)
#define OCTA_MRWCSR_MWO0_SHIFT                                                 (14u)
#define OCTA_MRWCSR_MRAL1                                                      (0x00070000u)
#define OCTA_MRWCSR_MRAL1_SHIFT                                                (16u)
#define OCTA_MRWCSR_MRCL1                                                      (0x00380000u)
#define OCTA_MRWCSR_MRCL1_SHIFT                                                (19u)
#define OCTA_MRWCSR_MRO1                                                       (0x00400000u)
#define OCTA_MRWCSR_MRO1_SHIFT                                                 (22u)
#define OCTA_MRWCSR_MWAL1                                                      (0x07000000u)
#define OCTA_MRWCSR_MWAL1_SHIFT                                                (24u)
#define OCTA_MRWCSR_MWCL1                                                      (0x38000000u)
#define OCTA_MRWCSR_MWCL1_SHIFT                                                (27u)
#define OCTA_MRWCSR_MWO1                                                       (0x40000000u)
#define OCTA_MRWCSR_MWO1_SHIFT                                                 (30u)
#define OCTA_ESR_MRESR                                                         (0x000000FFu)
#define OCTA_ESR_MRESR_SHIFT                                                   (0u)
#define OCTA_ESR_MWESR                                                         (0x0000FF00u)
#define OCTA_ESR_MWESR_SHIFT                                                   (8u)
#define OCTA_CWDR_WD0                                                          (0x000000FFu)
#define OCTA_CWDR_WD0_SHIFT                                                    (0u)
#define OCTA_CWDR_WD1                                                          (0x0000FF00u)
#define OCTA_CWDR_WD1_SHIFT                                                    (8u)
#define OCTA_CWDR_WD2                                                          (0x00FF0000u)
#define OCTA_CWDR_WD2_SHIFT                                                    (16u)
#define OCTA_CWDR_WD3                                                          (0xFF000000u)
#define OCTA_CWDR_WD3_SHIFT                                                    (24u)
#define OCTA_CRR_RD0                                                           (0x000000FFu)
#define OCTA_CRR_RD0_SHIFT                                                     (0u)
#define OCTA_CRR_RD1                                                           (0x0000FF00u)
#define OCTA_CRR_RD1_SHIFT                                                     (8u)
#define OCTA_CRR_RD2                                                           (0x00FF0000u)
#define OCTA_CRR_RD2_SHIFT                                                     (16u)
#define OCTA_CRR_RD3                                                           (0xFF000000u)
#define OCTA_CRR_RD3_SHIFT                                                     (24u)

#endif
