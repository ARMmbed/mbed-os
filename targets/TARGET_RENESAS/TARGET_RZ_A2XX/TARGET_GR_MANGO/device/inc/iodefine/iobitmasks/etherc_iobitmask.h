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

#ifndef ETHERC_IOBITMASK_H
#define ETHERC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define ETHERC_ECMR_PRM                                                        (0x00000001u)
#define ETHERC_ECMR_PRM_SHIFT                                                  (0u)
#define ETHERC_ECMR_DM                                                         (0x00000002u)
#define ETHERC_ECMR_DM_SHIFT                                                   (1u)
#define ETHERC_ECMR_RTM                                                        (0x00000004u)
#define ETHERC_ECMR_RTM_SHIFT                                                  (2u)
#define ETHERC_ECMR_ILB                                                        (0x00000008u)
#define ETHERC_ECMR_ILB_SHIFT                                                  (3u)
#define ETHERC_ECMR_TE                                                         (0x00000020u)
#define ETHERC_ECMR_TE_SHIFT                                                   (5u)
#define ETHERC_ECMR_RE                                                         (0x00000040u)
#define ETHERC_ECMR_RE_SHIFT                                                   (6u)
#define ETHERC_ECMR_MPDE                                                       (0x00000200u)
#define ETHERC_ECMR_MPDE_SHIFT                                                 (9u)
#define ETHERC_ECMR_PRCEF                                                      (0x00001000u)
#define ETHERC_ECMR_PRCEF_SHIFT                                                (12u)
#define ETHERC_ECMR_TXF                                                        (0x00010000u)
#define ETHERC_ECMR_TXF_SHIFT                                                  (16u)
#define ETHERC_ECMR_RXF                                                        (0x00020000u)
#define ETHERC_ECMR_RXF_SHIFT                                                  (17u)
#define ETHERC_ECMR_PFR                                                        (0x00040000u)
#define ETHERC_ECMR_PFR_SHIFT                                                  (18u)
#define ETHERC_ECMR_ZPF                                                        (0x00080000u)
#define ETHERC_ECMR_ZPF_SHIFT                                                  (19u)
#define ETHERC_ECMR_TPC                                                        (0x00100000u)
#define ETHERC_ECMR_TPC_SHIFT                                                  (20u)
#define ETHERC_RFLR_RFL                                                        (0x00000FFFu)
#define ETHERC_RFLR_RFL_SHIFT                                                  (0u)
#define ETHERC_ECSR_ICD                                                        (0x00000001u)
#define ETHERC_ECSR_ICD_SHIFT                                                  (0u)
#define ETHERC_ECSR_MPD                                                        (0x00000002u)
#define ETHERC_ECSR_MPD_SHIFT                                                  (1u)
#define ETHERC_ECSR_LCHNG                                                      (0x00000004u)
#define ETHERC_ECSR_LCHNG_SHIFT                                                (2u)
#define ETHERC_ECSR_PSRTO                                                      (0x00000010u)
#define ETHERC_ECSR_PSRTO_SHIFT                                                (4u)
#define ETHERC_ECSR_BFR                                                        (0x00000020u)
#define ETHERC_ECSR_BFR_SHIFT                                                  (5u)
#define ETHERC_ECSIPR_ICDIP                                                    (0x00000001u)
#define ETHERC_ECSIPR_ICDIP_SHIFT                                              (0u)
#define ETHERC_ECSIPR_MPDIP                                                    (0x00000002u)
#define ETHERC_ECSIPR_MPDIP_SHIFT                                              (1u)
#define ETHERC_ECSIPR_LCHNGIP                                                  (0x00000004u)
#define ETHERC_ECSIPR_LCHNGIP_SHIFT                                            (2u)
#define ETHERC_ECSIPR_PSRTOIP                                                  (0x00000010u)
#define ETHERC_ECSIPR_PSRTOIP_SHIFT                                            (4u)
#define ETHERC_ECSIPR_BFSIPR                                                   (0x00000020u)
#define ETHERC_ECSIPR_BFSIPR_SHIFT                                             (5u)
#define ETHERC_PIR_MDC                                                         (0x00000001u)
#define ETHERC_PIR_MDC_SHIFT                                                   (0u)
#define ETHERC_PIR_MMD                                                         (0x00000002u)
#define ETHERC_PIR_MMD_SHIFT                                                   (1u)
#define ETHERC_PIR_MDO                                                         (0x00000004u)
#define ETHERC_PIR_MDO_SHIFT                                                   (2u)
#define ETHERC_PIR_MDI                                                         (0x00000008u)
#define ETHERC_PIR_MDI_SHIFT                                                   (3u)
#define ETHERC_PSR_LMON                                                        (0x00000001u)
#define ETHERC_PSR_LMON_SHIFT                                                  (0u)
#define ETHERC_RDMLR_RMD                                                       (0x000FFFFFu)
#define ETHERC_RDMLR_RMD_SHIFT                                                 (0u)
#define ETHERC_IPGR_IPG                                                        (0x0000001Fu)
#define ETHERC_IPGR_IPG_SHIFT                                                  (0u)
#define ETHERC_APR_AP                                                          (0x0000FFFFu)
#define ETHERC_APR_AP_SHIFT                                                    (0u)
#define ETHERC_MPR_MP                                                          (0x0000FFFFu)
#define ETHERC_MPR_MP_SHIFT                                                    (0u)
#define ETHERC_RFCF_RPAUSE                                                     (0x000000FFu)
#define ETHERC_RFCF_RPAUSE_SHIFT                                               (0u)
#define ETHERC_TPAUSER_TPAUSE                                                  (0x0000FFFFu)
#define ETHERC_TPAUSER_TPAUSE_SHIFT                                            (0u)
#define ETHERC_TPAUSECR_TXP                                                    (0x000000FFu)
#define ETHERC_TPAUSECR_TXP_SHIFT                                              (0u)
#define ETHERC_BCFRR_BCF                                                       (0x0000FFFFu)
#define ETHERC_BCFRR_BCF_SHIFT                                                 (0u)
#define ETHERC_MAHR_MAHR                                                       (0xFFFFFFFFu)
#define ETHERC_MAHR_MAHR_SHIFT                                                 (0u)
#define ETHERC_MALR_MALR                                                       (0x0000FFFFu)
#define ETHERC_MALR_MALR_SHIFT                                                 (0u)
#define ETHERC_TROCR_TROCR                                                     (0xFFFFFFFFu)
#define ETHERC_TROCR_TROCR_SHIFT                                               (0u)
#define ETHERC_CDCR_CDCR                                                       (0xFFFFFFFFu)
#define ETHERC_CDCR_CDCR_SHIFT                                                 (0u)
#define ETHERC_LCCR_LCCR                                                       (0xFFFFFFFFu)
#define ETHERC_LCCR_LCCR_SHIFT                                                 (0u)
#define ETHERC_CNDCR_CNDCR                                                     (0xFFFFFFFFu)
#define ETHERC_CNDCR_CNDCR_SHIFT                                               (0u)
#define ETHERC_CEFCR_CEFCR                                                     (0xFFFFFFFFu)
#define ETHERC_CEFCR_CEFCR_SHIFT                                               (0u)
#define ETHERC_FRECR_FRECR                                                     (0xFFFFFFFFu)
#define ETHERC_FRECR_FRECR_SHIFT                                               (0u)
#define ETHERC_TSFRCR_TSFRCR                                                   (0xFFFFFFFFu)
#define ETHERC_TSFRCR_TSFRCR_SHIFT                                             (0u)
#define ETHERC_TLFRCR_TLFRCR                                                   (0xFFFFFFFFu)
#define ETHERC_TLFRCR_TLFRCR_SHIFT                                             (0u)
#define ETHERC_RFCR_RFCR                                                       (0xFFFFFFFFu)
#define ETHERC_RFCR_RFCR_SHIFT                                                 (0u)
#define ETHERC_MAFCR_MAFCR                                                     (0xFFFFFFFFu)
#define ETHERC_MAFCR_MAFCR_SHIFT                                               (0u)

#endif
