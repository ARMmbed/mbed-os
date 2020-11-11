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

#ifndef SCIM_IOBITMASK_H
#define SCIM_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SCIM_SMR_CKS                                                           (0x03u)
#define SCIM_SMR_CKS_SHIFT                                                     (0u)
#define SCIM_SMR_MP                                                            (0x04u)
#define SCIM_SMR_MP_SHIFT                                                      (2u)
#define SCIM_SMR_STOP                                                          (0x08u)
#define SCIM_SMR_STOP_SHIFT                                                    (3u)
#define SCIM_SMR_PM                                                            (0x10u)
#define SCIM_SMR_PM_SHIFT                                                      (4u)
#define SCIM_SMR_PE                                                            (0x20u)
#define SCIM_SMR_PE_SHIFT                                                      (5u)
#define SCIM_SMR_CHR                                                           (0x40u)
#define SCIM_SMR_CHR_SHIFT                                                     (6u)
#define SCIM_SMR_CM                                                            (0x80u)
#define SCIM_SMR_CM_SHIFT                                                      (7u)
#define SCIM_BRR_BRR                                                           (0xFFu)
#define SCIM_BRR_BRR_SHIFT                                                     (0u)
#define SCIM_SCR_CKE                                                           (0x03u)
#define SCIM_SCR_CKE_SHIFT                                                     (0u)
#define SCIM_SCR_TEIE                                                          (0x04u)
#define SCIM_SCR_TEIE_SHIFT                                                    (2u)
#define SCIM_SCR_MPIE                                                          (0x08u)
#define SCIM_SCR_MPIE_SHIFT                                                    (3u)
#define SCIM_SCR_RE                                                            (0x10u)
#define SCIM_SCR_RE_SHIFT                                                      (4u)
#define SCIM_SCR_TE                                                            (0x20u)
#define SCIM_SCR_TE_SHIFT                                                      (5u)
#define SCIM_SCR_RIE                                                           (0x40u)
#define SCIM_SCR_RIE_SHIFT                                                     (6u)
#define SCIM_SCR_TIE                                                           (0x80u)
#define SCIM_SCR_TIE_SHIFT                                                     (7u)
#define SCIM_TDR_TDR                                                           (0xFFu)
#define SCIM_TDR_TDR_SHIFT                                                     (0u)
#define SCIM_SSR_MPBT                                                          (0x01u)
#define SCIM_SSR_MPBT_SHIFT                                                    (0u)
#define SCIM_SSR_MPB                                                           (0x02u)
#define SCIM_SSR_MPB_SHIFT                                                     (1u)
#define SCIM_SSR_TEND                                                          (0x04u)
#define SCIM_SSR_TEND_SHIFT                                                    (2u)
#define SCIM_SSR_PER                                                           (0x08u)
#define SCIM_SSR_PER_SHIFT                                                     (3u)
#define SCIM_SSR_FER                                                           (0x10u)
#define SCIM_SSR_FER_SHIFT                                                     (4u)
#define SCIM_SSR_ORER                                                          (0x20u)
#define SCIM_SSR_ORER_SHIFT                                                    (5u)
#define SCIM_SSR_RDRF                                                          (0x40u)
#define SCIM_SSR_RDRF_SHIFT                                                    (6u)
#define SCIM_SSR_TDRE                                                          (0x80u)
#define SCIM_SSR_TDRE_SHIFT                                                    (7u)
#define SCIM_RDR_RDR                                                           (0xFFu)
#define SCIM_RDR_RDR_SHIFT                                                     (0u)
#define SCIM_SCMR_SMIF                                                         (0x01u)
#define SCIM_SCMR_SMIF_SHIFT                                                   (0u)
#define SCIM_SCMR_SINV                                                         (0x04u)
#define SCIM_SCMR_SINV_SHIFT                                                   (2u)
#define SCIM_SCMR_SDIR                                                         (0x08u)
#define SCIM_SCMR_SDIR_SHIFT                                                   (3u)
#define SCIM_SCMR_CHR1                                                         (0x10u)
#define SCIM_SCMR_CHR1_SHIFT                                                   (4u)
#define SCIM_SCMR_BCP2                                                         (0x80u)
#define SCIM_SCMR_BCP2_SHIFT                                                   (7u)
#define SCIM_SEMR_ACS0                                                         (0x01u)
#define SCIM_SEMR_ACS0_SHIFT                                                   (0u)
#define SCIM_SEMR_BRME                                                         (0x04u)
#define SCIM_SEMR_BRME_SHIFT                                                   (2u)
#define SCIM_SEMR_ABCS                                                         (0x10u)
#define SCIM_SEMR_ABCS_SHIFT                                                   (4u)
#define SCIM_SEMR_NFEN                                                         (0x20u)
#define SCIM_SEMR_NFEN_SHIFT                                                   (5u)
#define SCIM_SEMR_BGDM                                                         (0x40u)
#define SCIM_SEMR_BGDM_SHIFT                                                   (6u)
#define SCIM_SEMR_RXDESEL                                                      (0x80u)
#define SCIM_SEMR_RXDESEL_SHIFT                                                (7u)
#define SCIM_SNFR_NFCS                                                         (0x07u)
#define SCIM_SNFR_NFCS_SHIFT                                                   (0u)
#define SCIM_SECR_CTSE                                                         (0x02u)
#define SCIM_SECR_CTSE_SHIFT                                                   (1u)
#define SCIM_TDRHL_TDRHL                                                       (0xFFFFu)
#define SCIM_TDRHL_TDRHL_SHIFT                                                 (0u)
#define SCIM_TDRH_TDRH                                                         (0xFFu)
#define SCIM_TDRH_TDRH_SHIFT                                                   (0u)
#define SCIM_TDRL_TDRL                                                         (0xFFu)
#define SCIM_TDRL_TDRL_SHIFT                                                   (0u)
#define SCIM_RDRHL_RDRHL                                                       (0xFFFFu)
#define SCIM_RDRHL_RDRHL_SHIFT                                                 (0u)
#define SCIM_RDRH_RDRH                                                         (0xFFu)
#define SCIM_RDRH_RDRH_SHIFT                                                   (0u)
#define SCIM_RDRL_RDRL                                                         (0xFFu)
#define SCIM_RDRL_RDRL_SHIFT                                                   (0u)
#define SCIM_MDDR_MDDR                                                         (0xFFu)
#define SCIM_MDDR_MDDR_SHIFT                                                   (0u)

#endif
