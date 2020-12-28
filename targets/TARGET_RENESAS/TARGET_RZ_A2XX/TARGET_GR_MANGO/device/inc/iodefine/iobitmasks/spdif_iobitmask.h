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

#ifndef SPDIF_IOBITMASK_H
#define SPDIF_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SPDIF_TLCA_TLCA                                                        (0x00FFFFFFu)
#define SPDIF_TLCA_TLCA_SHIFT                                                  (0u)
#define SPDIF_TRCA_TRCA                                                        (0x00FFFFFFu)
#define SPDIF_TRCA_TRCA_SHIFT                                                  (0u)
#define SPDIF_TLCS_CTL                                                         (0x0000003Eu)
#define SPDIF_TLCS_CTL_SHIFT                                                   (1u)
#define SPDIF_TLCS_CATCD                                                       (0x0000FF00u)
#define SPDIF_TLCS_CATCD_SHIFT                                                 (8u)
#define SPDIF_TLCS_SRCNO                                                       (0x000F0000u)
#define SPDIF_TLCS_SRCNO_SHIFT                                                 (16u)
#define SPDIF_TLCS_CHNO                                                        (0x00F00000u)
#define SPDIF_TLCS_CHNO_SHIFT                                                  (20u)
#define SPDIF_TLCS_FS                                                          (0x0F000000u)
#define SPDIF_TLCS_FS_SHIFT                                                    (24u)
#define SPDIF_TLCS_CLAC                                                        (0x30000000u)
#define SPDIF_TLCS_CLAC_SHIFT                                                  (28u)
#define SPDIF_TRCS_CTL                                                         (0x0000003Eu)
#define SPDIF_TRCS_CTL_SHIFT                                                   (1u)
#define SPDIF_TRCS_CATCD                                                       (0x0000FF00u)
#define SPDIF_TRCS_CATCD_SHIFT                                                 (8u)
#define SPDIF_TRCS_SRCNO                                                       (0x000F0000u)
#define SPDIF_TRCS_SRCNO_SHIFT                                                 (16u)
#define SPDIF_TRCS_CHNO                                                        (0x00F00000u)
#define SPDIF_TRCS_CHNO_SHIFT                                                  (20u)
#define SPDIF_TRCS_FS                                                          (0x0F000000u)
#define SPDIF_TRCS_FS_SHIFT                                                    (24u)
#define SPDIF_TRCS_CLAC                                                        (0x30000000u)
#define SPDIF_TRCS_CLAC_SHIFT                                                  (28u)
#define SPDIF_TUI_TUI                                                          (0xFFFFFFFFu)
#define SPDIF_TUI_TUI_SHIFT                                                    (0u)
#define SPDIF_RLCA_RLCA                                                        (0x00FFFFFFu)
#define SPDIF_RLCA_RLCA_SHIFT                                                  (0u)
#define SPDIF_RRCA_RRCA                                                        (0x00FFFFFFu)
#define SPDIF_RRCA_RRCA_SHIFT                                                  (0u)
#define SPDIF_RLCS_CTL                                                         (0x0000003Eu)
#define SPDIF_RLCS_CTL_SHIFT                                                   (1u)
#define SPDIF_RLCS_CATCD                                                       (0x0000FF00u)
#define SPDIF_RLCS_CATCD_SHIFT                                                 (8u)
#define SPDIF_RLCS_SRCNO                                                       (0x000F0000u)
#define SPDIF_RLCS_SRCNO_SHIFT                                                 (16u)
#define SPDIF_RLCS_CHNO                                                        (0x00F00000u)
#define SPDIF_RLCS_CHNO_SHIFT                                                  (20u)
#define SPDIF_RLCS_FS                                                          (0x0F000000u)
#define SPDIF_RLCS_FS_SHIFT                                                    (24u)
#define SPDIF_RLCS_CLAC                                                        (0x30000000u)
#define SPDIF_RLCS_CLAC_SHIFT                                                  (28u)
#define SPDIF_RRCS_CTL                                                         (0x0000003Eu)
#define SPDIF_RRCS_CTL_SHIFT                                                   (1u)
#define SPDIF_RRCS_CATCD                                                       (0x0000FF00u)
#define SPDIF_RRCS_CATCD_SHIFT                                                 (8u)
#define SPDIF_RRCS_SRCNO                                                       (0x000F0000u)
#define SPDIF_RRCS_SRCNO_SHIFT                                                 (16u)
#define SPDIF_RRCS_CHNO                                                        (0x00F00000u)
#define SPDIF_RRCS_CHNO_SHIFT                                                  (20u)
#define SPDIF_RRCS_FS                                                          (0x0F000000u)
#define SPDIF_RRCS_FS_SHIFT                                                    (24u)
#define SPDIF_RRCS_CLAC                                                        (0x30000000u)
#define SPDIF_RRCS_CLAC_SHIFT                                                  (28u)
#define SPDIF_RUI_RUI                                                          (0xFFFFFFFFu)
#define SPDIF_RUI_RUI_SHIFT                                                    (0u)
#define SPDIF_CTRL_TCBI                                                        (0x00000001u)
#define SPDIF_CTRL_TCBI_SHIFT                                                  (0u)
#define SPDIF_CTRL_TCSI                                                        (0x00000002u)
#define SPDIF_CTRL_TCSI_SHIFT                                                  (1u)
#define SPDIF_CTRL_RCBI                                                        (0x00000004u)
#define SPDIF_CTRL_RCBI_SHIFT                                                  (2u)
#define SPDIF_CTRL_RCSI                                                        (0x00000008u)
#define SPDIF_CTRL_RCSI_SHIFT                                                  (3u)
#define SPDIF_CTRL_TUII                                                        (0x00000010u)
#define SPDIF_CTRL_TUII_SHIFT                                                  (4u)
#define SPDIF_CTRL_RUII                                                        (0x00000020u)
#define SPDIF_CTRL_RUII_SHIFT                                                  (5u)
#define SPDIF_CTRL_ABUI                                                        (0x00000040u)
#define SPDIF_CTRL_ABUI_SHIFT                                                  (6u)
#define SPDIF_CTRL_ABOI                                                        (0x00000080u)
#define SPDIF_CTRL_ABOI_SHIFT                                                  (7u)
#define SPDIF_CTRL_CSEI                                                        (0x00000100u)
#define SPDIF_CTRL_CSEI_SHIFT                                                  (8u)
#define SPDIF_CTRL_PREI                                                        (0x00000200u)
#define SPDIF_CTRL_PREI_SHIFT                                                  (9u)
#define SPDIF_CTRL_PAEI                                                        (0x00000400u)
#define SPDIF_CTRL_PAEI_SHIFT                                                  (10u)
#define SPDIF_CTRL_CREI                                                        (0x00000800u)
#define SPDIF_CTRL_CREI_SHIFT                                                  (11u)
#define SPDIF_CTRL_UBUI                                                        (0x00001000u)
#define SPDIF_CTRL_UBUI_SHIFT                                                  (12u)
#define SPDIF_CTRL_UBOI                                                        (0x00002000u)
#define SPDIF_CTRL_UBOI_SHIFT                                                  (13u)
#define SPDIF_CTRL_TEIE                                                        (0x00004000u)
#define SPDIF_CTRL_TEIE_SHIFT                                                  (14u)
#define SPDIF_CTRL_REIE                                                        (0x00008000u)
#define SPDIF_CTRL_REIE_SHIFT                                                  (15u)
#define SPDIF_CTRL_TME                                                         (0x00010000u)
#define SPDIF_CTRL_TME_SHIFT                                                   (16u)
#define SPDIF_CTRL_RME                                                         (0x00020000u)
#define SPDIF_CTRL_RME_SHIFT                                                   (17u)
#define SPDIF_CTRL_AOS                                                         (0x00040000u)
#define SPDIF_CTRL_AOS_SHIFT                                                   (18u)
#define SPDIF_CTRL_NCSI                                                        (0x00080000u)
#define SPDIF_CTRL_NCSI_SHIFT                                                  (19u)
#define SPDIF_CTRL_TDE                                                         (0x00100000u)
#define SPDIF_CTRL_TDE_SHIFT                                                   (20u)
#define SPDIF_CTRL_RDE                                                         (0x00200000u)
#define SPDIF_CTRL_RDE_SHIFT                                                   (21u)
#define SPDIF_CTRL_TASS                                                        (0x00C00000u)
#define SPDIF_CTRL_TASS_SHIFT                                                  (22u)
#define SPDIF_CTRL_RASS                                                        (0x03000000u)
#define SPDIF_CTRL_RASS_SHIFT                                                  (24u)
#define SPDIF_CTRL_PB                                                          (0x04000000u)
#define SPDIF_CTRL_PB_SHIFT                                                    (26u)
#define SPDIF_CTRL_CKS                                                         (0x10000000u)
#define SPDIF_CTRL_CKS_SHIFT                                                   (28u)
#define SPDIF_STAT_CBTX                                                        (0x00000001u)
#define SPDIF_STAT_CBTX_SHIFT                                                  (0u)
#define SPDIF_STAT_CSTX                                                        (0x00000002u)
#define SPDIF_STAT_CSTX_SHIFT                                                  (1u)
#define SPDIF_STAT_CBRX                                                        (0x00000004u)
#define SPDIF_STAT_CBRX_SHIFT                                                  (2u)
#define SPDIF_STAT_CSRX                                                        (0x00000008u)
#define SPDIF_STAT_CSRX_SHIFT                                                  (3u)
#define SPDIF_STAT_TUIR                                                        (0x00000010u)
#define SPDIF_STAT_TUIR_SHIFT                                                  (4u)
#define SPDIF_STAT_RUIR                                                        (0x00000020u)
#define SPDIF_STAT_RUIR_SHIFT                                                  (5u)
#define SPDIF_STAT_ABU                                                         (0x00000040u)
#define SPDIF_STAT_ABU_SHIFT                                                   (6u)
#define SPDIF_STAT_ABO                                                         (0x00000080u)
#define SPDIF_STAT_ABO_SHIFT                                                   (7u)
#define SPDIF_STAT_CSE                                                         (0x00000100u)
#define SPDIF_STAT_CSE_SHIFT                                                   (8u)
#define SPDIF_STAT_PREE                                                        (0x00000200u)
#define SPDIF_STAT_PREE_SHIFT                                                  (9u)
#define SPDIF_STAT_PARE                                                        (0x00000400u)
#define SPDIF_STAT_PARE_SHIFT                                                  (10u)
#define SPDIF_STAT_CE                                                          (0x00000800u)
#define SPDIF_STAT_CE_SHIFT                                                    (11u)
#define SPDIF_STAT_UBU                                                         (0x00001000u)
#define SPDIF_STAT_UBU_SHIFT                                                   (12u)
#define SPDIF_STAT_UBO                                                         (0x00002000u)
#define SPDIF_STAT_UBO_SHIFT                                                   (13u)
#define SPDIF_STAT_TIS                                                         (0x00004000u)
#define SPDIF_STAT_TIS_SHIFT                                                   (14u)
#define SPDIF_STAT_RIS                                                         (0x00008000u)
#define SPDIF_STAT_RIS_SHIFT                                                   (15u)
#define SPDIF_STAT_CMD                                                         (0x00010000u)
#define SPDIF_STAT_CMD_SHIFT                                                   (16u)
#define SPDIF_TDAD_TDAD                                                        (0x00FFFFFFu)
#define SPDIF_TDAD_TDAD_SHIFT                                                  (0u)
#define SPDIF_RDAD_RDAD                                                        (0x00FFFFFFu)
#define SPDIF_RDAD_RDAD_SHIFT                                                  (0u)

#endif
