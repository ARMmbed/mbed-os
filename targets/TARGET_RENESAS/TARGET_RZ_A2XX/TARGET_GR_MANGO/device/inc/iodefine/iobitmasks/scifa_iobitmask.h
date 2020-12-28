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

#ifndef SCIFA_IOBITMASK_H
#define SCIFA_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SCIFA_SMR_CKS                                                          (0x0003u)
#define SCIFA_SMR_CKS_SHIFT                                                    (0u)
#define SCIFA_SMR_STOP                                                         (0x0008u)
#define SCIFA_SMR_STOP_SHIFT                                                   (3u)
#define SCIFA_SMR_PM                                                           (0x0010u)
#define SCIFA_SMR_PM_SHIFT                                                     (4u)
#define SCIFA_SMR_PE                                                           (0x0020u)
#define SCIFA_SMR_PE_SHIFT                                                     (5u)
#define SCIFA_SMR_CHR                                                          (0x0040u)
#define SCIFA_SMR_CHR_SHIFT                                                    (6u)
#define SCIFA_SMR_CM                                                           (0x0080u)
#define SCIFA_SMR_CM_SHIFT                                                     (7u)
#define SCIFA_MDDR_MDDR                                                        (0xFFu)
#define SCIFA_MDDR_MDDR_SHIFT                                                  (0u)
#define SCIFA_BRR_BRR                                                          (0xFFu)
#define SCIFA_BRR_BRR_SHIFT                                                    (0u)
#define SCIFA_SCR_CKE                                                          (0x0003u)
#define SCIFA_SCR_CKE_SHIFT                                                    (0u)
#define SCIFA_SCR_TEIE                                                         (0x0004u)
#define SCIFA_SCR_TEIE_SHIFT                                                   (2u)
#define SCIFA_SCR_REIE                                                         (0x0008u)
#define SCIFA_SCR_REIE_SHIFT                                                   (3u)
#define SCIFA_SCR_RE                                                           (0x0010u)
#define SCIFA_SCR_RE_SHIFT                                                     (4u)
#define SCIFA_SCR_TE                                                           (0x0020u)
#define SCIFA_SCR_TE_SHIFT                                                     (5u)
#define SCIFA_SCR_RIE                                                          (0x0040u)
#define SCIFA_SCR_RIE_SHIFT                                                    (6u)
#define SCIFA_SCR_TIE                                                          (0x0080u)
#define SCIFA_SCR_TIE_SHIFT                                                    (7u)
#define SCIFA_FTDR_FTDR                                                        (0xFFu)
#define SCIFA_FTDR_FTDR_SHIFT                                                  (0u)
#define SCIFA_FSR_DR                                                           (0x0001u)
#define SCIFA_FSR_DR_SHIFT                                                     (0u)
#define SCIFA_FSR_RDF                                                          (0x0002u)
#define SCIFA_FSR_RDF_SHIFT                                                    (1u)
#define SCIFA_FSR_PER                                                          (0x0004u)
#define SCIFA_FSR_PER_SHIFT                                                    (2u)
#define SCIFA_FSR_FER                                                          (0x0008u)
#define SCIFA_FSR_FER_SHIFT                                                    (3u)
#define SCIFA_FSR_BRK                                                          (0x0010u)
#define SCIFA_FSR_BRK_SHIFT                                                    (4u)
#define SCIFA_FSR_TDFE                                                         (0x0020u)
#define SCIFA_FSR_TDFE_SHIFT                                                   (5u)
#define SCIFA_FSR_TEND                                                         (0x0040u)
#define SCIFA_FSR_TEND_SHIFT                                                   (6u)
#define SCIFA_FSR_ER                                                           (0x0080u)
#define SCIFA_FSR_ER_SHIFT                                                     (7u)
#define SCIFA_FRDR_FRDR                                                        (0xFFu)
#define SCIFA_FRDR_FRDR_SHIFT                                                  (0u)
#define SCIFA_FCR_LOOP                                                         (0x0001u)
#define SCIFA_FCR_LOOP_SHIFT                                                   (0u)
#define SCIFA_FCR_RFRST                                                        (0x0002u)
#define SCIFA_FCR_RFRST_SHIFT                                                  (1u)
#define SCIFA_FCR_TFRST                                                        (0x0004u)
#define SCIFA_FCR_TFRST_SHIFT                                                  (2u)
#define SCIFA_FCR_MCE                                                          (0x0008u)
#define SCIFA_FCR_MCE_SHIFT                                                    (3u)
#define SCIFA_FCR_TTRG                                                         (0x0030u)
#define SCIFA_FCR_TTRG_SHIFT                                                   (4u)
#define SCIFA_FCR_RTRG                                                         (0x00C0u)
#define SCIFA_FCR_RTRG_SHIFT                                                   (6u)
#define SCIFA_FCR_RSTRG                                                        (0x0700u)
#define SCIFA_FCR_RSTRG_SHIFT                                                  (8u)
#define SCIFA_FDR_R                                                            (0x001Fu)
#define SCIFA_FDR_R_SHIFT                                                      (0u)
#define SCIFA_FDR_T                                                            (0x1F00u)
#define SCIFA_FDR_T_SHIFT                                                      (8u)
#define SCIFA_SPTR_SPB2DT                                                      (0x0001u)
#define SCIFA_SPTR_SPB2DT_SHIFT                                                (0u)
#define SCIFA_SPTR_SPB2IO                                                      (0x0002u)
#define SCIFA_SPTR_SPB2IO_SHIFT                                                (1u)
#define SCIFA_SPTR_SCKDT                                                       (0x0004u)
#define SCIFA_SPTR_SCKDT_SHIFT                                                 (2u)
#define SCIFA_SPTR_SCKIO                                                       (0x0008u)
#define SCIFA_SPTR_SCKIO_SHIFT                                                 (3u)
#define SCIFA_SPTR_CTS2DT                                                      (0x0010u)
#define SCIFA_SPTR_CTS2DT_SHIFT                                                (4u)
#define SCIFA_SPTR_CTS2IO                                                      (0x0020u)
#define SCIFA_SPTR_CTS2IO_SHIFT                                                (5u)
#define SCIFA_SPTR_RTS2DT                                                      (0x0040u)
#define SCIFA_SPTR_RTS2DT_SHIFT                                                (6u)
#define SCIFA_SPTR_RTS2IO                                                      (0x0080u)
#define SCIFA_SPTR_RTS2IO_SHIFT                                                (7u)
#define SCIFA_LSR_ORER                                                         (0x0001u)
#define SCIFA_LSR_ORER_SHIFT                                                   (0u)
#define SCIFA_LSR_FER                                                          (0x003Cu)
#define SCIFA_LSR_FER_SHIFT                                                    (2u)
#define SCIFA_LSR_PER                                                          (0x0F00u)
#define SCIFA_LSR_PER_SHIFT                                                    (8u)
#define SCIFA_SEMR_ABCS0                                                       (0x01u)
#define SCIFA_SEMR_ABCS0_SHIFT                                                 (0u)
#define SCIFA_SEMR_NFEN                                                        (0x04u)
#define SCIFA_SEMR_NFEN_SHIFT                                                  (2u)
#define SCIFA_SEMR_DIR                                                         (0x08u)
#define SCIFA_SEMR_DIR_SHIFT                                                   (3u)
#define SCIFA_SEMR_MDDRS                                                       (0x10u)
#define SCIFA_SEMR_MDDRS_SHIFT                                                 (4u)
#define SCIFA_SEMR_BRME                                                        (0x20u)
#define SCIFA_SEMR_BRME_SHIFT                                                  (5u)
#define SCIFA_SEMR_BGDM                                                        (0x80u)
#define SCIFA_SEMR_BGDM_SHIFT                                                  (7u)
#define SCIFA_FTCR_TFTC                                                        (0x001Fu)
#define SCIFA_FTCR_TFTC_SHIFT                                                  (0u)
#define SCIFA_FTCR_TTRGS                                                       (0x0080u)
#define SCIFA_FTCR_TTRGS_SHIFT                                                 (7u)
#define SCIFA_FTCR_RFTC                                                        (0x1F00u)
#define SCIFA_FTCR_RFTC_SHIFT                                                  (8u)
#define SCIFA_FTCR_RTRGS                                                       (0x8000u)
#define SCIFA_FTCR_RTRGS_SHIFT                                                 (15u)

#endif
