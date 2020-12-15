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

#ifndef SPIBSC_IOBITMASK_H
#define SPIBSC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define SPIBSC_CMNCR_BSZ                                                       (0x00000003u)
#define SPIBSC_CMNCR_BSZ_SHIFT                                                 (0u)
#define SPIBSC_CMNCR_IO0FV                                                     (0x00000300u)
#define SPIBSC_CMNCR_IO0FV_SHIFT                                               (8u)
#define SPIBSC_CMNCR_IO2FV                                                     (0x00003000u)
#define SPIBSC_CMNCR_IO2FV_SHIFT                                               (12u)
#define SPIBSC_CMNCR_IO3FV                                                     (0x0000C000u)
#define SPIBSC_CMNCR_IO3FV_SHIFT                                               (14u)
#define SPIBSC_CMNCR_MOIIO0                                                    (0x00030000u)
#define SPIBSC_CMNCR_MOIIO0_SHIFT                                              (16u)
#define SPIBSC_CMNCR_MOIIO1                                                    (0x000C0000u)
#define SPIBSC_CMNCR_MOIIO1_SHIFT                                              (18u)
#define SPIBSC_CMNCR_MOIIO2                                                    (0x00300000u)
#define SPIBSC_CMNCR_MOIIO2_SHIFT                                              (20u)
#define SPIBSC_CMNCR_MOIIO3                                                    (0x00C00000u)
#define SPIBSC_CMNCR_MOIIO3_SHIFT                                              (22u)
#define SPIBSC_CMNCR_MD                                                        (0x80000000u)
#define SPIBSC_CMNCR_MD_SHIFT                                                  (31u)
#define SPIBSC_SSLDR_SCKDL                                                     (0x00000007u)
#define SPIBSC_SSLDR_SCKDL_SHIFT                                               (0u)
#define SPIBSC_SSLDR_SLNDL                                                     (0x00000700u)
#define SPIBSC_SSLDR_SLNDL_SHIFT                                               (8u)
#define SPIBSC_SSLDR_SPNDL                                                     (0x00070000u)
#define SPIBSC_SSLDR_SPNDL_SHIFT                                               (16u)
#define SPIBSC_DRCR_SSLE                                                       (0x00000001u)
#define SPIBSC_DRCR_SSLE_SHIFT                                                 (0u)
#define SPIBSC_DRCR_RBE                                                        (0x00000100u)
#define SPIBSC_DRCR_RBE_SHIFT                                                  (8u)
#define SPIBSC_DRCR_RCF                                                        (0x00000200u)
#define SPIBSC_DRCR_RCF_SHIFT                                                  (9u)
#define SPIBSC_DRCR_RBURST                                                     (0x001F0000u)
#define SPIBSC_DRCR_RBURST_SHIFT                                               (16u)
#define SPIBSC_DRCR_SSLN                                                       (0x01000000u)
#define SPIBSC_DRCR_SSLN_SHIFT                                                 (24u)
#define SPIBSC_DRCMR_OCMD                                                      (0x000000FFu)
#define SPIBSC_DRCMR_OCMD_SHIFT                                                (0u)
#define SPIBSC_DRCMR_CMD                                                       (0x00FF0000u)
#define SPIBSC_DRCMR_CMD_SHIFT                                                 (16u)
#define SPIBSC_DREAR_EAC                                                       (0x00000007u)
#define SPIBSC_DREAR_EAC_SHIFT                                                 (0u)
#define SPIBSC_DREAR_EAV                                                       (0x00FF0000u)
#define SPIBSC_DREAR_EAV_SHIFT                                                 (16u)
#define SPIBSC_DROPR_OPD0                                                      (0x000000FFu)
#define SPIBSC_DROPR_OPD0_SHIFT                                                (0u)
#define SPIBSC_DROPR_OPD1                                                      (0x0000FF00u)
#define SPIBSC_DROPR_OPD1_SHIFT                                                (8u)
#define SPIBSC_DROPR_OPD2                                                      (0x00FF0000u)
#define SPIBSC_DROPR_OPD2_SHIFT                                                (16u)
#define SPIBSC_DROPR_OPD3                                                      (0xFF000000u)
#define SPIBSC_DROPR_OPD3_SHIFT                                                (24u)
#define SPIBSC_DRENR_OPDE                                                      (0x000000F0u)
#define SPIBSC_DRENR_OPDE_SHIFT                                                (4u)
#define SPIBSC_DRENR_ADE                                                       (0x00000F00u)
#define SPIBSC_DRENR_ADE_SHIFT                                                 (8u)
#define SPIBSC_DRENR_OCDE                                                      (0x00001000u)
#define SPIBSC_DRENR_OCDE_SHIFT                                                (12u)
#define SPIBSC_DRENR_CDE                                                       (0x00004000u)
#define SPIBSC_DRENR_CDE_SHIFT                                                 (14u)
#define SPIBSC_DRENR_DME                                                       (0x00008000u)
#define SPIBSC_DRENR_DME_SHIFT                                                 (15u)
#define SPIBSC_DRENR_DRDB                                                      (0x00030000u)
#define SPIBSC_DRENR_DRDB_SHIFT                                                (16u)
#define SPIBSC_DRENR_OPDB                                                      (0x00300000u)
#define SPIBSC_DRENR_OPDB_SHIFT                                                (20u)
#define SPIBSC_DRENR_ADB                                                       (0x03000000u)
#define SPIBSC_DRENR_ADB_SHIFT                                                 (24u)
#define SPIBSC_DRENR_OCDB                                                      (0x30000000u)
#define SPIBSC_DRENR_OCDB_SHIFT                                                (28u)
#define SPIBSC_DRENR_CDB                                                       (0xC0000000u)
#define SPIBSC_DRENR_CDB_SHIFT                                                 (30u)
#define SPIBSC_SMCR_SPIE                                                       (0x00000001u)
#define SPIBSC_SMCR_SPIE_SHIFT                                                 (0u)
#define SPIBSC_SMCR_SPIWE                                                      (0x00000002u)
#define SPIBSC_SMCR_SPIWE_SHIFT                                                (1u)
#define SPIBSC_SMCR_SPIRE                                                      (0x00000004u)
#define SPIBSC_SMCR_SPIRE_SHIFT                                                (2u)
#define SPIBSC_SMCR_SSLKP                                                      (0x00000100u)
#define SPIBSC_SMCR_SSLKP_SHIFT                                                (8u)
#define SPIBSC_SMCMR_OCMD                                                      (0x000000FFu)
#define SPIBSC_SMCMR_OCMD_SHIFT                                                (0u)
#define SPIBSC_SMCMR_CMD                                                       (0x00FF0000u)
#define SPIBSC_SMCMR_CMD_SHIFT                                                 (16u)
#define SPIBSC_SMADR_ADR                                                       (0xFFFFFFFFu)
#define SPIBSC_SMADR_ADR_SHIFT                                                 (0u)
#define SPIBSC_SMOPR_OPD0                                                      (0x000000FFu)
#define SPIBSC_SMOPR_OPD0_SHIFT                                                (0u)
#define SPIBSC_SMOPR_OPD1                                                      (0x0000FF00u)
#define SPIBSC_SMOPR_OPD1_SHIFT                                                (8u)
#define SPIBSC_SMOPR_OPD2                                                      (0x00FF0000u)
#define SPIBSC_SMOPR_OPD2_SHIFT                                                (16u)
#define SPIBSC_SMOPR_OPD3                                                      (0xFF000000u)
#define SPIBSC_SMOPR_OPD3_SHIFT                                                (24u)
#define SPIBSC_SMENR_SPIDE                                                     (0x0000000Fu)
#define SPIBSC_SMENR_SPIDE_SHIFT                                               (0u)
#define SPIBSC_SMENR_OPDE                                                      (0x000000F0u)
#define SPIBSC_SMENR_OPDE_SHIFT                                                (4u)
#define SPIBSC_SMENR_ADE                                                       (0x00000F00u)
#define SPIBSC_SMENR_ADE_SHIFT                                                 (8u)
#define SPIBSC_SMENR_OCDE                                                      (0x00001000u)
#define SPIBSC_SMENR_OCDE_SHIFT                                                (12u)
#define SPIBSC_SMENR_CDE                                                       (0x00004000u)
#define SPIBSC_SMENR_CDE_SHIFT                                                 (14u)
#define SPIBSC_SMENR_DME                                                       (0x00008000u)
#define SPIBSC_SMENR_DME_SHIFT                                                 (15u)
#define SPIBSC_SMENR_SPIDB                                                     (0x00030000u)
#define SPIBSC_SMENR_SPIDB_SHIFT                                               (16u)
#define SPIBSC_SMENR_OPDB                                                      (0x00300000u)
#define SPIBSC_SMENR_OPDB_SHIFT                                                (20u)
#define SPIBSC_SMENR_ADB                                                       (0x03000000u)
#define SPIBSC_SMENR_ADB_SHIFT                                                 (24u)
#define SPIBSC_SMENR_OCDB                                                      (0x30000000u)
#define SPIBSC_SMENR_OCDB_SHIFT                                                (28u)
#define SPIBSC_SMENR_CDB                                                       (0xC0000000u)
#define SPIBSC_SMENR_CDB_SHIFT                                                 (30u)
#define SPIBSC_SMRDR0_RDATA0                                                   (0xFFFFFFFFu)
#define SPIBSC_SMRDR0_RDATA0_SHIFT                                             (0u)
#define SPIBSC_SMRDR1_RDATA1                                                   (0xFFFFFFFFu)
#define SPIBSC_SMRDR1_RDATA1_SHIFT                                             (0u)
#define SPIBSC_SMWDR0_WDATA0                                                   (0xFFFFFFFFu)
#define SPIBSC_SMWDR0_WDATA0_SHIFT                                             (0u)
#define SPIBSC_SMWDR1_WDATA1                                                   (0xFFFFFFFFu)
#define SPIBSC_SMWDR1_WDATA1_SHIFT                                             (0u)
#define SPIBSC_CMNSR_TEND                                                      (0x00000001u)
#define SPIBSC_CMNSR_TEND_SHIFT                                                (0u)
#define SPIBSC_CMNSR_SSLF                                                      (0x00000002u)
#define SPIBSC_CMNSR_SSLF_SHIFT                                                (1u)
#define SPIBSC_DRDMCR_DMCYC                                                    (0x0000001Fu)
#define SPIBSC_DRDMCR_DMCYC_SHIFT                                              (0u)
#define SPIBSC_DRDRENR_DRDRE                                                   (0x00000001u)
#define SPIBSC_DRDRENR_DRDRE_SHIFT                                             (0u)
#define SPIBSC_DRDRENR_OPDRE                                                   (0x00000010u)
#define SPIBSC_DRDRENR_OPDRE_SHIFT                                             (4u)
#define SPIBSC_DRDRENR_ADDRE                                                   (0x00000100u)
#define SPIBSC_DRDRENR_ADDRE_SHIFT                                             (8u)
#define SPIBSC_DRDRENR_HYPE                                                    (0x00007000u)
#define SPIBSC_DRDRENR_HYPE_SHIFT                                              (12u)
#define SPIBSC_SMDMCR_DMCYC                                                    (0x0000001Fu)
#define SPIBSC_SMDMCR_DMCYC_SHIFT                                              (0u)
#define SPIBSC_SMDRENR_SPIDRE                                                  (0x00000001u)
#define SPIBSC_SMDRENR_SPIDRE_SHIFT                                            (0u)
#define SPIBSC_SMDRENR_OPDRE                                                   (0x00000010u)
#define SPIBSC_SMDRENR_OPDRE_SHIFT                                             (4u)
#define SPIBSC_SMDRENR_ADDRE                                                   (0x00000100u)
#define SPIBSC_SMDRENR_ADDRE_SHIFT                                             (8u)
#define SPIBSC_SMDRENR_HYPE                                                    (0x00007000u)
#define SPIBSC_SMDRENR_HYPE_SHIFT                                              (12u)
#define SPIBSC_PHYADJ1_ADJ1                                                    (0xFFFFFFFFu)
#define SPIBSC_PHYADJ1_ADJ1_SHIFT                                              (0u)
#define SPIBSC_PHYADJ2_ADJ2                                                    (0xFFFFFFFFu)
#define SPIBSC_PHYADJ2_ADJ2_SHIFT                                              (0u)
#define SPIBSC_PHYCNT_PHYMEM                                                   (0x00000003u)
#define SPIBSC_PHYCNT_PHYMEM_SHIFT                                             (0u)
#define SPIBSC_PHYCNT_WBUF                                                     (0x00000004u)
#define SPIBSC_PHYCNT_WBUF_SHIFT                                               (2u)
#define SPIBSC_PHYCNT_WBUF2                                                    (0x00000010u)
#define SPIBSC_PHYCNT_WBUF2_SHIFT                                              (4u)
#define SPIBSC_PHYCNT_CKSEL                                                    (0x00030000u)
#define SPIBSC_PHYCNT_CKSEL_SHIFT                                              (16u)
#define SPIBSC_PHYCNT_HS                                                       (0x00040000u)
#define SPIBSC_PHYCNT_HS_SHIFT                                                 (18u)
#define SPIBSC_PHYCNT_OCT                                                      (0x00100000u)
#define SPIBSC_PHYCNT_OCT_SHIFT                                                (20u)
#define SPIBSC_PHYCNT_EXDS                                                     (0x00200000u)
#define SPIBSC_PHYCNT_EXDS_SHIFT                                               (21u)
#define SPIBSC_PHYCNT_OCTA_1_0                                                 (0x00C00000u)
#define SPIBSC_PHYCNT_OCTA_1_0_SHIFT                                           (22u)
#define SPIBSC_PHYCNT_ALT_ALIGN                                                (0x40000000u)
#define SPIBSC_PHYCNT_ALT_ALIGN_SHIFT                                          (30u)
#define SPIBSC_PHYCNT_CAL                                                      (0x80000000u)
#define SPIBSC_PHYCNT_CAL_SHIFT                                                (31u)
#define SPIBSC_PHYOFFSET1_DDRTMG                                               (0x30000000u)
#define SPIBSC_PHYOFFSET1_DDRTMG_SHIFT                                         (28u)
#define SPIBSC_PHYOFFSET2_OCTTMG                                               (0x00000700u)
#define SPIBSC_PHYOFFSET2_OCTTMG_SHIFT                                         (8u)
#define SPIBSC_PHYINT_INT                                                      (0x00000001u)
#define SPIBSC_PHYINT_INT_SHIFT                                                (0u)
#define SPIBSC_PHYINT_WPVAL                                                    (0x00000002u)
#define SPIBSC_PHYINT_WPVAL_SHIFT                                              (1u)
#define SPIBSC_PHYINT_RSTVAL                                                   (0x00000004u)
#define SPIBSC_PHYINT_RSTVAL_SHIFT                                             (2u)
#define SPIBSC_PHYINT_INTEN                                                    (0x01000000u)
#define SPIBSC_PHYINT_INTEN_SHIFT                                              (24u)
#define SPIBSC_PHYINT_WPEN                                                     (0x02000000u)
#define SPIBSC_PHYINT_WPEN_SHIFT                                               (25u)
#define SPIBSC_PHYINT_RSTEN                                                    (0x04000000u)
#define SPIBSC_PHYINT_RSTEN_SHIFT                                              (26u)

#endif
