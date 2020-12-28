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

#ifndef POEG_IOBITMASK_H
#define POEG_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define POEG_POEGGA_PIDF                                                       (0x00000001u)
#define POEG_POEGGA_PIDF_SHIFT                                                 (0u)
#define POEG_POEGGA_IOCF                                                       (0x00000002u)
#define POEG_POEGGA_IOCF_SHIFT                                                 (1u)
#define POEG_POEGGA_SSF                                                        (0x00000008u)
#define POEG_POEGGA_SSF_SHIFT                                                  (3u)
#define POEG_POEGGA_PIDE                                                       (0x00000010u)
#define POEG_POEGGA_PIDE_SHIFT                                                 (4u)
#define POEG_POEGGA_IOCE                                                       (0x00000020u)
#define POEG_POEGGA_IOCE_SHIFT                                                 (5u)
#define POEG_POEGGA_ST                                                         (0x00010000u)
#define POEG_POEGGA_ST_SHIFT                                                   (16u)
#define POEG_POEGGA_INV                                                        (0x10000000u)
#define POEG_POEGGA_INV_SHIFT                                                  (28u)
#define POEG_POEGGA_NFEN                                                       (0x20000000u)
#define POEG_POEGGA_NFEN_SHIFT                                                 (29u)
#define POEG_POEGGA_NFCS                                                       (0xC0000000u)
#define POEG_POEGGA_NFCS_SHIFT                                                 (30u)
#define POEG_POEGGB_PIDF                                                       (0x00000001u)
#define POEG_POEGGB_PIDF_SHIFT                                                 (0u)
#define POEG_POEGGB_IOCF                                                       (0x00000002u)
#define POEG_POEGGB_IOCF_SHIFT                                                 (1u)
#define POEG_POEGGB_SSF                                                        (0x00000008u)
#define POEG_POEGGB_SSF_SHIFT                                                  (3u)
#define POEG_POEGGB_PIDE                                                       (0x00000010u)
#define POEG_POEGGB_PIDE_SHIFT                                                 (4u)
#define POEG_POEGGB_IOCE                                                       (0x00000020u)
#define POEG_POEGGB_IOCE_SHIFT                                                 (5u)
#define POEG_POEGGB_ST                                                         (0x00010000u)
#define POEG_POEGGB_ST_SHIFT                                                   (16u)
#define POEG_POEGGB_INV                                                        (0x10000000u)
#define POEG_POEGGB_INV_SHIFT                                                  (28u)
#define POEG_POEGGB_NFEN                                                       (0x20000000u)
#define POEG_POEGGB_NFEN_SHIFT                                                 (29u)
#define POEG_POEGGB_NFCS                                                       (0xC0000000u)
#define POEG_POEGGB_NFCS_SHIFT                                                 (30u)
#define POEG_POEGGC_PIDF                                                       (0x00000001u)
#define POEG_POEGGC_PIDF_SHIFT                                                 (0u)
#define POEG_POEGGC_IOCF                                                       (0x00000002u)
#define POEG_POEGGC_IOCF_SHIFT                                                 (1u)
#define POEG_POEGGC_SSF                                                        (0x00000008u)
#define POEG_POEGGC_SSF_SHIFT                                                  (3u)
#define POEG_POEGGC_PIDE                                                       (0x00000010u)
#define POEG_POEGGC_PIDE_SHIFT                                                 (4u)
#define POEG_POEGGC_IOCE                                                       (0x00000020u)
#define POEG_POEGGC_IOCE_SHIFT                                                 (5u)
#define POEG_POEGGC_ST                                                         (0x00010000u)
#define POEG_POEGGC_ST_SHIFT                                                   (16u)
#define POEG_POEGGC_INV                                                        (0x10000000u)
#define POEG_POEGGC_INV_SHIFT                                                  (28u)
#define POEG_POEGGC_NFEN                                                       (0x20000000u)
#define POEG_POEGGC_NFEN_SHIFT                                                 (29u)
#define POEG_POEGGC_NFCS                                                       (0xC0000000u)
#define POEG_POEGGC_NFCS_SHIFT                                                 (30u)
#define POEG_POEGGD_PIDF                                                       (0x00000001u)
#define POEG_POEGGD_PIDF_SHIFT                                                 (0u)
#define POEG_POEGGD_IOCF                                                       (0x00000002u)
#define POEG_POEGGD_IOCF_SHIFT                                                 (1u)
#define POEG_POEGGD_SSF                                                        (0x00000008u)
#define POEG_POEGGD_SSF_SHIFT                                                  (3u)
#define POEG_POEGGD_PIDE                                                       (0x00000010u)
#define POEG_POEGGD_PIDE_SHIFT                                                 (4u)
#define POEG_POEGGD_IOCE                                                       (0x00000020u)
#define POEG_POEGGD_IOCE_SHIFT                                                 (5u)
#define POEG_POEGGD_ST                                                         (0x00010000u)
#define POEG_POEGGD_ST_SHIFT                                                   (16u)
#define POEG_POEGGD_INV                                                        (0x10000000u)
#define POEG_POEGGD_INV_SHIFT                                                  (28u)
#define POEG_POEGGD_NFEN                                                       (0x20000000u)
#define POEG_POEGGD_NFEN_SHIFT                                                 (29u)
#define POEG_POEGGD_NFCS                                                       (0xC0000000u)
#define POEG_POEGGD_NFCS_SHIFT                                                 (30u)

#endif
