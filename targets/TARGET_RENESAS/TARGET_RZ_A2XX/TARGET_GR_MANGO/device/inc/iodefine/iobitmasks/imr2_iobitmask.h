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

#ifndef IMR2_IOBITMASK_H
#define IMR2_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define IMR2_CR_RS                                                             (0x00000001u)
#define IMR2_CR_RS_SHIFT                                                       (0u)
#define IMR2_CR_ARS                                                            (0x00000002u)
#define IMR2_CR_ARS_SHIFT                                                      (1u)
#define IMR2_CR_SFE                                                            (0x00000004u)
#define IMR2_CR_SFE_SHIFT                                                      (2u)
#define IMR2_CR_SWRST                                                          (0x00008000u)
#define IMR2_CR_SWRST_SHIFT                                                    (15u)
#define IMR2_SR_TRA                                                            (0x00000001u)
#define IMR2_SR_TRA_SHIFT                                                      (0u)
#define IMR2_SR_IER                                                            (0x00000002u)
#define IMR2_SR_IER_SHIFT                                                      (1u)
#define IMR2_SR_INT                                                            (0x00000004u)
#define IMR2_SR_INT_SHIFT                                                      (2u)
#define IMR2_SR_DSA                                                            (0x00000040u)
#define IMR2_SR_DSA_SHIFT                                                      (6u)
#define IMR2_SR_SFS                                                            (0x00000080u)
#define IMR2_SR_SFS_SHIFT                                                      (7u)
#define IMR2_SRCR_TRACLR                                                       (0x00000001u)
#define IMR2_SRCR_TRACLR_SHIFT                                                 (0u)
#define IMR2_SRCR_IERCLR                                                       (0x00000002u)
#define IMR2_SRCR_IERCLR_SHIFT                                                 (1u)
#define IMR2_SRCR_INTCLR                                                       (0x00000004u)
#define IMR2_SRCR_INTCLR_SHIFT                                                 (2u)
#define IMR2_ICR_TRAENB                                                        (0x00000001u)
#define IMR2_ICR_TRAENB_SHIFT                                                  (0u)
#define IMR2_ICR_IERENB                                                        (0x00000002u)
#define IMR2_ICR_IERENB_SHIFT                                                  (1u)
#define IMR2_ICR_INTENB                                                        (0x00000004u)
#define IMR2_ICR_INTENB_SHIFT                                                  (2u)
#define IMR2_IMR_TEAM                                                          (0x00000001u)
#define IMR2_IMR_TEAM_SHIFT                                                    (0u)
#define IMR2_IMR_IEM                                                           (0x00000002u)
#define IMR2_IMR_IEM_SHIFT                                                     (1u)
#define IMR2_IMR_INM                                                           (0x00000004u)
#define IMR2_IMR_INM_SHIFT                                                     (2u)
#define IMR2_DLPR_DLP                                                          (0xFFFFFFFFu)
#define IMR2_DLPR_DLP_SHIFT                                                    (0u)
#define IMR2_DLSAR_DLSA                                                        (0xFFFFFFF8u)
#define IMR2_DLSAR_DLSA_SHIFT                                                  (3u)
#define IMR2_DSAR_DSA                                                          (0xFFFFFFE0u)
#define IMR2_DSAR_DSA_SHIFT                                                    (5u)
#define IMR2_DSTR_DST                                                          (0x00003FFFu)
#define IMR2_DSTR_DST_SHIFT                                                    (0u)
#define IMR2_DSAR2_DSA2                                                        (0xFFFFFFE0u)
#define IMR2_DSAR2_DSA2_SHIFT                                                  (5u)
#define IMR2_DLSAR2_DLSA2                                                      (0xFFFFFFF8u)
#define IMR2_DLSAR2_DLSA2_SHIFT                                                (3u)
#define IMR2_TRIMR_TME                                                         (0x00000001u)
#define IMR2_TRIMR_TME_SHIFT                                                   (0u)
#define IMR2_TRIMR_BFE                                                         (0x00000002u)
#define IMR2_TRIMR_BFE_SHIFT                                                   (1u)
#define IMR2_TRIMR_AUTODG                                                      (0x00000004u)
#define IMR2_TRIMR_AUTODG_SHIFT                                                (2u)
#define IMR2_TRIMR_AUTOSG                                                      (0x00000008u)
#define IMR2_TRIMR_AUTOSG_SHIFT                                                (3u)
#define IMR2_TRIMR_DXDYM                                                       (0x00000010u)
#define IMR2_TRIMR_DXDYM_SHIFT                                                 (4u)
#define IMR2_TRIMR_DUDVM                                                       (0x00000020u)
#define IMR2_TRIMR_DUDVM_SHIFT                                                 (5u)
#define IMR2_TRIMR_TCM                                                         (0x00000040u)
#define IMR2_TRIMR_TCM_SHIFT                                                   (6u)
#define IMR2_TRIMSR_TMES                                                       (0x00000001u)
#define IMR2_TRIMSR_TMES_SHIFT                                                 (0u)
#define IMR2_TRIMSR_BFES                                                       (0x00000002u)
#define IMR2_TRIMSR_BFES_SHIFT                                                 (1u)
#define IMR2_TRIMSR_AUTODGS                                                    (0x00000004u)
#define IMR2_TRIMSR_AUTODGS_SHIFT                                              (2u)
#define IMR2_TRIMSR_AUTOSGS                                                    (0x00000008u)
#define IMR2_TRIMSR_AUTOSGS_SHIFT                                              (3u)
#define IMR2_TRIMSR_DXDYMS                                                     (0x00000010u)
#define IMR2_TRIMSR_DXDYMS_SHIFT                                               (4u)
#define IMR2_TRIMSR_DUDVMS                                                     (0x00000020u)
#define IMR2_TRIMSR_DUDVMS_SHIFT                                               (5u)
#define IMR2_TRIMSR_TCMS                                                       (0x00000040u)
#define IMR2_TRIMSR_TCMS_SHIFT                                                 (6u)
#define IMR2_TRIMCR_TMEC                                                       (0x00000001u)
#define IMR2_TRIMCR_TMEC_SHIFT                                                 (0u)
#define IMR2_TRIMCR_BFEC                                                       (0x00000002u)
#define IMR2_TRIMCR_BFEC_SHIFT                                                 (1u)
#define IMR2_TRIMCR_AUTODGC                                                    (0x00000004u)
#define IMR2_TRIMCR_AUTODGC_SHIFT                                              (2u)
#define IMR2_TRIMCR_AUTOSGC                                                    (0x00000008u)
#define IMR2_TRIMCR_AUTOSGC_SHIFT                                              (3u)
#define IMR2_TRIMCR_DXDYMC                                                     (0x00000010u)
#define IMR2_TRIMCR_DXDYMC_SHIFT                                               (4u)
#define IMR2_TRIMCR_DUDVMC                                                     (0x00000020u)
#define IMR2_TRIMCR_DUDVMC_SHIFT                                               (5u)
#define IMR2_TRIMCR_TCMC                                                       (0x00000040u)
#define IMR2_TRIMCR_TCMC_SHIFT                                                 (6u)
#define IMR2_TRICR_TCY                                                         (0x000000FFu)
#define IMR2_TRICR_TCY_SHIFT                                                   (0u)
#define IMR2_TRICR_TCU                                                         (0x0000FF00u)
#define IMR2_TRICR_TCU_SHIFT                                                   (8u)
#define IMR2_TRICR_TCV                                                         (0x00FF0000u)
#define IMR2_TRICR_TCV_SHIFT                                                   (16u)
#define IMR2_TRICR_YCFORM                                                      (0x80000000u)
#define IMR2_TRICR_YCFORM_SHIFT                                                (31u)
#define IMR2_UVDPOR_UVDPO                                                      (0x00000007u)
#define IMR2_UVDPOR_UVDPO_SHIFT                                                (0u)
#define IMR2_UVDPOR_DDP                                                        (0x00000100u)
#define IMR2_UVDPOR_DDP_SHIFT                                                  (8u)
#define IMR2_SUSR_SVW                                                          (0x000007FFu)
#define IMR2_SUSR_SVW_SHIFT                                                    (0u)
#define IMR2_SUSR_SUW                                                          (0x07FF0000u)
#define IMR2_SUSR_SUW_SHIFT                                                    (16u)
#define IMR2_SVSR_SVS                                                          (0x000007FFu)
#define IMR2_SVSR_SVS_SHIFT                                                    (0u)
#define IMR2_XMINR_XMIN                                                        (0x00001FFFu)
#define IMR2_XMINR_XMIN_SHIFT                                                  (0u)
#define IMR2_YMINR_YMIN                                                        (0x00001FFFu)
#define IMR2_YMINR_YMIN_SHIFT                                                  (0u)
#define IMR2_XMAXR_XMAX                                                        (0x00001FFFu)
#define IMR2_XMAXR_XMAX_SHIFT                                                  (0u)
#define IMR2_YMAXR_YMAX                                                        (0x00001FFFu)
#define IMR2_YMAXR_YMAX_SHIFT                                                  (0u)
#define IMR2_AMXSR_AMXS                                                        (0x00001FFFu)
#define IMR2_AMXSR_AMXS_SHIFT                                                  (0u)
#define IMR2_AMYSR_AMYS                                                        (0x00001FFFu)
#define IMR2_AMYSR_AMYS_SHIFT                                                  (0u)
#define IMR2_AMXOR_AMXO                                                        (0x00001FFFu)
#define IMR2_AMXOR_AMXO_SHIFT                                                  (0u)
#define IMR2_AMYOR_AMYO                                                        (0x00001FFFu)
#define IMR2_AMYOR_AMYO_SHIFT                                                  (0u)
#define IMR2_MACR1_EMAM                                                        (0x00001000u)
#define IMR2_MACR1_EMAM_SHIFT                                                  (12u)
#define IMR2_LSPR_LSPR                                                         (0x000003FFu)
#define IMR2_LSPR_LSPR_SHIFT                                                   (0u)
#define IMR2_LEPR_LEPR                                                         (0x000003FFu)
#define IMR2_LEPR_LEPR_SHIFT                                                   (0u)
#define IMR2_LMSR_LMSR                                                         (0x00000007u)
#define IMR2_LMSR_LMSR_SHIFT                                                   (0u)
#define IMR2_LMSPPCR_SPPC                                                      (0x000007FFu)
#define IMR2_LMSPPCR_SPPC_SHIFT                                                (0u)
#define IMR2_LMEPPCR_EPPC                                                      (0x000007FFu)
#define IMR2_LMEPPCR_EPPC_SHIFT                                                (0u)

#endif
