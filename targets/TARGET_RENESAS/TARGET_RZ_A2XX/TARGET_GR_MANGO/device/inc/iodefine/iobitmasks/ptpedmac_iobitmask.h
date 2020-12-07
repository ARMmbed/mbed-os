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

#ifndef PTPEDMAC_IOBITMASK_H
#define PTPEDMAC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define PTPEDMAC_EDMR_SWR                                                      (0x00000001u)
#define PTPEDMAC_EDMR_SWR_SHIFT                                                (0u)
#define PTPEDMAC_EDMR_DL                                                       (0x00000030u)
#define PTPEDMAC_EDMR_DL_SHIFT                                                 (4u)
#define PTPEDMAC_EDMR_DE                                                       (0x00000040u)
#define PTPEDMAC_EDMR_DE_SHIFT                                                 (6u)
#define PTPEDMAC_EDTRR_TR                                                      (0x00000001u)
#define PTPEDMAC_EDTRR_TR_SHIFT                                                (0u)
#define PTPEDMAC_EDRRR_RR                                                      (0x00000001u)
#define PTPEDMAC_EDRRR_RR_SHIFT                                                (0u)
#define PTPEDMAC_TDLAR_TDLAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_TDLAR_TDLAR_SHIFT                                             (0u)
#define PTPEDMAC_RDLAR_RDLAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_RDLAR_RDLAR_SHIFT                                             (0u)
#define PTPEDMAC_EESR_TYPE                                                     (0x0000000Fu)
#define PTPEDMAC_EESR_TYPE_SHIFT                                               (0u)
#define PTPEDMAC_EESR_PVER                                                     (0x00000010u)
#define PTPEDMAC_EESR_PVER_SHIFT                                               (4u)
#define PTPEDMAC_EESR_RPORT                                                    (0x00000080u)
#define PTPEDMAC_EESR_RPORT_SHIFT                                              (7u)
#define PTPEDMAC_EESR_MACE                                                     (0x00000100u)
#define PTPEDMAC_EESR_MACE_SHIFT                                               (8u)
#define PTPEDMAC_EESR_RFOF                                                     (0x00010000u)
#define PTPEDMAC_EESR_RFOF_SHIFT                                               (16u)
#define PTPEDMAC_EESR_RDE                                                      (0x00020000u)
#define PTPEDMAC_EESR_RDE_SHIFT                                                (17u)
#define PTPEDMAC_EESR_FR                                                       (0x00040000u)
#define PTPEDMAC_EESR_FR_SHIFT                                                 (18u)
#define PTPEDMAC_EESR_TFUF                                                     (0x00080000u)
#define PTPEDMAC_EESR_TFUF_SHIFT                                               (19u)
#define PTPEDMAC_EESR_TDE                                                      (0x00100000u)
#define PTPEDMAC_EESR_TDE_SHIFT                                                (20u)
#define PTPEDMAC_EESR_TC                                                       (0x00200000u)
#define PTPEDMAC_EESR_TC_SHIFT                                                 (21u)
#define PTPEDMAC_EESR_RFCOF                                                    (0x01000000u)
#define PTPEDMAC_EESR_RFCOF_SHIFT                                              (24u)
#define PTPEDMAC_EESR_TABT                                                     (0x04000000u)
#define PTPEDMAC_EESR_TABT_SHIFT                                               (26u)
#define PTPEDMAC_EESR_TWB                                                      (0x40000000u)
#define PTPEDMAC_EESR_TWB_SHIFT                                                (30u)
#define PTPEDMAC_EESIPR_PVERIP                                                 (0x00000010u)
#define PTPEDMAC_EESIPR_PVERIP_SHIFT                                           (4u)
#define PTPEDMAC_EESIPR_RPORTIP                                                (0x00000080u)
#define PTPEDMAC_EESIPR_RPORTIP_SHIFT                                          (7u)
#define PTPEDMAC_EESIPR_MACEIP                                                 (0x00000100u)
#define PTPEDMAC_EESIPR_MACEIP_SHIFT                                           (8u)
#define PTPEDMAC_EESIPR_RFOFIP                                                 (0x00010000u)
#define PTPEDMAC_EESIPR_RFOFIP_SHIFT                                           (16u)
#define PTPEDMAC_EESIPR_RDEIP                                                  (0x00020000u)
#define PTPEDMAC_EESIPR_RDEIP_SHIFT                                            (17u)
#define PTPEDMAC_EESIPR_FRIP                                                   (0x00040000u)
#define PTPEDMAC_EESIPR_FRIP_SHIFT                                             (18u)
#define PTPEDMAC_EESIPR_TFUFIP                                                 (0x00080000u)
#define PTPEDMAC_EESIPR_TFUFIP_SHIFT                                           (19u)
#define PTPEDMAC_EESIPR_TDEIP                                                  (0x00100000u)
#define PTPEDMAC_EESIPR_TDEIP_SHIFT                                            (20u)
#define PTPEDMAC_EESIPR_TCIP                                                   (0x00200000u)
#define PTPEDMAC_EESIPR_TCIP_SHIFT                                             (21u)
#define PTPEDMAC_EESIPR_RFCOFIP                                                (0x01000000u)
#define PTPEDMAC_EESIPR_RFCOFIP_SHIFT                                          (24u)
#define PTPEDMAC_EESIPR_TABTIP                                                 (0x04000000u)
#define PTPEDMAC_EESIPR_TABTIP_SHIFT                                           (26u)
#define PTPEDMAC_EESIPR_TWBIP                                                  (0x40000000u)
#define PTPEDMAC_EESIPR_TWBIP_SHIFT                                            (30u)
#define PTPEDMAC_RMFCR_MFC                                                     (0x0000FFFFu)
#define PTPEDMAC_RMFCR_MFC_SHIFT                                               (0u)
#define PTPEDMAC_TFTR_TFT                                                      (0x000007FFu)
#define PTPEDMAC_TFTR_TFT_SHIFT                                                (0u)
#define PTPEDMAC_FDR_RFD                                                       (0x0000001Fu)
#define PTPEDMAC_FDR_RFD_SHIFT                                                 (0u)
#define PTPEDMAC_FDR_TFD                                                       (0x00001F00u)
#define PTPEDMAC_FDR_TFD_SHIFT                                                 (8u)
#define PTPEDMAC_RMCR_RNR                                                      (0x00000001u)
#define PTPEDMAC_RMCR_RNR_SHIFT                                                (0u)
#define PTPEDMAC_TFUCR_UNDER                                                   (0x0000FFFFu)
#define PTPEDMAC_TFUCR_UNDER_SHIFT                                             (0u)
#define PTPEDMAC_RFOCR_OVER                                                    (0x0000FFFFu)
#define PTPEDMAC_RFOCR_OVER_SHIFT                                              (0u)
#define PTPEDMAC_FCFTR_RFDO                                                    (0x00000007u)
#define PTPEDMAC_FCFTR_RFDO_SHIFT                                              (0u)
#define PTPEDMAC_FCFTR_RFFO                                                    (0x00070000u)
#define PTPEDMAC_FCFTR_RFFO_SHIFT                                              (16u)
#define PTPEDMAC_RPADIR_PADR                                                   (0x0000003Fu)
#define PTPEDMAC_RPADIR_PADR_SHIFT                                             (0u)
#define PTPEDMAC_RPADIR_PADS                                                   (0x00030000u)
#define PTPEDMAC_RPADIR_PADS_SHIFT                                             (16u)
#define PTPEDMAC_TRIMD_TIS                                                     (0x00000001u)
#define PTPEDMAC_TRIMD_TIS_SHIFT                                               (0u)
#define PTPEDMAC_TRIMD_TIM                                                     (0x00000010u)
#define PTPEDMAC_TRIMD_TIM_SHIFT                                               (4u)
#define PTPEDMAC_RBWAR_RBWAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_RBWAR_RBWAR_SHIFT                                             (0u)
#define PTPEDMAC_RDFAR_RDFAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_RDFAR_RDFAR_SHIFT                                             (0u)
#define PTPEDMAC_TBRAR_TBRAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_TBRAR_TBRAR_SHIFT                                             (0u)
#define PTPEDMAC_TDFAR_TDFAR                                                   (0xFFFFFFFFu)
#define PTPEDMAC_TDFAR_TDFAR_SHIFT                                             (0u)

#endif
