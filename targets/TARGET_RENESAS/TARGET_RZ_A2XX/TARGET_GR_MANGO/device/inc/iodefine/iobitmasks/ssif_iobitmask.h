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

#ifndef SSIF_IOBITMASK_H
#define SSIF_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SSIF_SSICR_REN                                                         (0x00000001u)
#define SSIF_SSICR_REN_SHIFT                                                   (0u)
#define SSIF_SSICR_TEN                                                         (0x00000002u)
#define SSIF_SSICR_TEN_SHIFT                                                   (1u)
#define SSIF_SSICR_MUEN                                                        (0x00000008u)
#define SSIF_SSICR_MUEN_SHIFT                                                  (3u)
#define SSIF_SSICR_CKDV                                                        (0x000000F0u)
#define SSIF_SSICR_CKDV_SHIFT                                                  (4u)
#define SSIF_SSICR_DEL                                                         (0x00000100u)
#define SSIF_SSICR_DEL_SHIFT                                                   (8u)
#define SSIF_SSICR_PDTA                                                        (0x00000200u)
#define SSIF_SSICR_PDTA_SHIFT                                                  (9u)
#define SSIF_SSICR_SDTA                                                        (0x00000400u)
#define SSIF_SSICR_SDTA_SHIFT                                                  (10u)
#define SSIF_SSICR_SPDP                                                        (0x00000800u)
#define SSIF_SSICR_SPDP_SHIFT                                                  (11u)
#define SSIF_SSICR_LRCKP                                                       (0x00001000u)
#define SSIF_SSICR_LRCKP_SHIFT                                                 (12u)
#define SSIF_SSICR_BCKP                                                        (0x00002000u)
#define SSIF_SSICR_BCKP_SHIFT                                                  (13u)
#define SSIF_SSICR_MST                                                         (0x00004000u)
#define SSIF_SSICR_MST_SHIFT                                                   (14u)
#define SSIF_SSICR_SWL                                                         (0x00070000u)
#define SSIF_SSICR_SWL_SHIFT                                                   (16u)
#define SSIF_SSICR_DWL                                                         (0x00380000u)
#define SSIF_SSICR_DWL_SHIFT                                                   (19u)
#define SSIF_SSICR_FRM                                                         (0x00C00000u)
#define SSIF_SSICR_FRM_SHIFT                                                   (22u)
#define SSIF_SSICR_IIEN                                                        (0x02000000u)
#define SSIF_SSICR_IIEN_SHIFT                                                  (25u)
#define SSIF_SSICR_ROIEN                                                       (0x04000000u)
#define SSIF_SSICR_ROIEN_SHIFT                                                 (26u)
#define SSIF_SSICR_RUIEN                                                       (0x08000000u)
#define SSIF_SSICR_RUIEN_SHIFT                                                 (27u)
#define SSIF_SSICR_TOIEN                                                       (0x10000000u)
#define SSIF_SSICR_TOIEN_SHIFT                                                 (28u)
#define SSIF_SSICR_TUIEN                                                       (0x20000000u)
#define SSIF_SSICR_TUIEN_SHIFT                                                 (29u)
#define SSIF_SSICR_CKS                                                         (0x40000000u)
#define SSIF_SSICR_CKS_SHIFT                                                   (30u)
#define SSIF_SSISR_IIRQ                                                        (0x02000000u)
#define SSIF_SSISR_IIRQ_SHIFT                                                  (25u)
#define SSIF_SSISR_ROIRQ                                                       (0x04000000u)
#define SSIF_SSISR_ROIRQ_SHIFT                                                 (26u)
#define SSIF_SSISR_RUIRQ                                                       (0x08000000u)
#define SSIF_SSISR_RUIRQ_SHIFT                                                 (27u)
#define SSIF_SSISR_TOIRQ                                                       (0x10000000u)
#define SSIF_SSISR_TOIRQ_SHIFT                                                 (28u)
#define SSIF_SSISR_TUIRQ                                                       (0x20000000u)
#define SSIF_SSISR_TUIRQ_SHIFT                                                 (29u)
#define SSIF_SSIFCR_RFRST                                                      (0x00000001u)
#define SSIF_SSIFCR_RFRST_SHIFT                                                (0u)
#define SSIF_SSIFCR_TFRST                                                      (0x00000002u)
#define SSIF_SSIFCR_TFRST_SHIFT                                                (1u)
#define SSIF_SSIFCR_RIE                                                        (0x00000004u)
#define SSIF_SSIFCR_RIE_SHIFT                                                  (2u)
#define SSIF_SSIFCR_TIE                                                        (0x00000008u)
#define SSIF_SSIFCR_TIE_SHIFT                                                  (3u)
#define SSIF_SSIFCR_RXDNCE                                                     (0x00000100u)
#define SSIF_SSIFCR_RXDNCE_SHIFT                                               (8u)
#define SSIF_SSIFCR_LRCKNCE                                                    (0x00000200u)
#define SSIF_SSIFCR_LRCKNCE_SHIFT                                              (9u)
#define SSIF_SSIFCR_BCKNCE                                                     (0x00000400u)
#define SSIF_SSIFCR_BCKNCE_SHIFT                                               (10u)
#define SSIF_SSIFCR_BSW                                                        (0x00000800u)
#define SSIF_SSIFCR_BSW_SHIFT                                                  (11u)
#define SSIF_SSIFCR_SSIRST                                                     (0x00010000u)
#define SSIF_SSIFCR_SSIRST_SHIFT                                               (16u)
#define SSIF_SSIFCR_AUCKE                                                      (0x80000000u)
#define SSIF_SSIFCR_AUCKE_SHIFT                                                (31u)
#define SSIF_SSIFSR_RDF                                                        (0x00000001u)
#define SSIF_SSIFSR_RDF_SHIFT                                                  (0u)
#define SSIF_SSIFSR_RDC                                                        (0x00003F00u)
#define SSIF_SSIFSR_RDC_SHIFT                                                  (8u)
#define SSIF_SSIFSR_TDE                                                        (0x00010000u)
#define SSIF_SSIFSR_TDE_SHIFT                                                  (16u)
#define SSIF_SSIFSR_TDC                                                        (0x3F000000u)
#define SSIF_SSIFSR_TDC_SHIFT                                                  (24u)
#define SSIF_SSIFTDR_SSIFTDR                                                   (0xFFFFFFFFu)
#define SSIF_SSIFTDR_SSIFTDR_SHIFT                                             (0u)
#define SSIF_SSIFRDR_SSIFRDR                                                   (0xFFFFFFFFu)
#define SSIF_SSIFRDR_SSIFRDR_SHIFT                                             (0u)
#define SSIF_SSIOFR_OMOD                                                       (0x00000003u)
#define SSIF_SSIOFR_OMOD_SHIFT                                                 (0u)
#define SSIF_SSIOFR_LRCONT                                                     (0x00000100u)
#define SSIF_SSIOFR_LRCONT_SHIFT                                               (8u)
#define SSIF_SSIOFR_BCKASTP                                                    (0x00000200u)
#define SSIF_SSIOFR_BCKASTP_SHIFT                                              (9u)
#define SSIF_SSISCR_RDFS                                                       (0x0000001Fu)
#define SSIF_SSISCR_RDFS_SHIFT                                                 (0u)
#define SSIF_SSISCR_TDES                                                       (0x00001F00u)
#define SSIF_SSISCR_TDES_SHIFT                                                 (8u)

#endif
