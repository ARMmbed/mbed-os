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

#ifndef HYPER_IOBITMASK_H
#define HYPER_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define HYPER_CSR_RACT                                                         (0x00000001u)
#define HYPER_CSR_RACT_SHIFT                                                   (0u)
#define HYPER_CSR_RDECERR                                                      (0x00000100u)
#define HYPER_CSR_RDECERR_SHIFT                                                (8u)
#define HYPER_CSR_RTRSERR                                                      (0x00000200u)
#define HYPER_CSR_RTRSERR_SHIFT                                                (9u)
#define HYPER_CSR_RRSTOERR                                                     (0x00000400u)
#define HYPER_CSR_RRSTOERR_SHIFT                                               (10u)
#define HYPER_CSR_RDSSTALL                                                     (0x00000800u)
#define HYPER_CSR_RDSSTALL_SHIFT                                               (11u)
#define HYPER_CSR_WACT                                                         (0x00010000u)
#define HYPER_CSR_WACT_SHIFT                                                   (16u)
#define HYPER_CSR_WDECERR                                                      (0x01000000u)
#define HYPER_CSR_WDECERR_SHIFT                                                (24u)
#define HYPER_CSR_WTRSERR                                                      (0x02000000u)
#define HYPER_CSR_WTRSERR_SHIFT                                                (25u)
#define HYPER_CSR_WRSTOERR                                                     (0x04000000u)
#define HYPER_CSR_WRSTOERR_SHIFT                                               (26u)
#define HYPER_IEN_RPCINTE                                                      (0x00000001u)
#define HYPER_IEN_RPCINTE_SHIFT                                                (0u)
#define HYPER_IEN_INTP                                                         (0x80000000u)
#define HYPER_IEN_INTP_SHIFT                                                   (31u)
#define HYPER_ISR_RPCINTS                                                      (0x00000001u)
#define HYPER_ISR_RPCINTS_SHIFT                                                (0u)
#define HYPER_MCR0_MAXLEN                                                      (0x07FC0000u)
#define HYPER_MCR0_MAXLEN_SHIFT                                                (18u)
#define HYPER_MCR0_MAXEN                                                       (0x80000000u)
#define HYPER_MCR0_MAXEN_SHIFT                                                 (31u)
#define HYPER_MCR1_DEVTYPE                                                     (0x00000010u)
#define HYPER_MCR1_DEVTYPE_SHIFT                                               (4u)
#define HYPER_MCR1_CRT                                                         (0x00000020u)
#define HYPER_MCR1_CRT_SHIFT                                                   (5u)
#define HYPER_MCR1_MAXLEN                                                      (0x07FC0000u)
#define HYPER_MCR1_MAXLEN_SHIFT                                                (18u)
#define HYPER_MCR1_MAXEN                                                       (0x80000000u)
#define HYPER_MCR1_MAXEN_SHIFT                                                 (31u)
#define HYPER_MTR0_WCSH                                                        (0x00000F00u)
#define HYPER_MTR0_WCSH_SHIFT                                                  (8u)
#define HYPER_MTR0_RCSH                                                        (0x0000F000u)
#define HYPER_MTR0_RCSH_SHIFT                                                  (12u)
#define HYPER_MTR0_WCSS                                                        (0x000F0000u)
#define HYPER_MTR0_WCSS_SHIFT                                                  (16u)
#define HYPER_MTR0_RCSS                                                        (0x00F00000u)
#define HYPER_MTR0_RCSS_SHIFT                                                  (20u)
#define HYPER_MTR0_WCSHI                                                       (0x0F000000u)
#define HYPER_MTR0_WCSHI_SHIFT                                                 (24u)
#define HYPER_MTR0_RCSHI                                                       (0xF0000000u)
#define HYPER_MTR0_RCSHI_SHIFT                                                 (28u)
#define HYPER_MTR1_LTCY                                                        (0x0000000Fu)
#define HYPER_MTR1_LTCY_SHIFT                                                  (0u)
#define HYPER_MTR1_WCSH                                                        (0x00000F00u)
#define HYPER_MTR1_WCSH_SHIFT                                                  (8u)
#define HYPER_MTR1_RCSH                                                        (0x0000F000u)
#define HYPER_MTR1_RCSH_SHIFT                                                  (12u)
#define HYPER_MTR1_WCSS                                                        (0x000F0000u)
#define HYPER_MTR1_WCSS_SHIFT                                                  (16u)
#define HYPER_MTR1_RCSS                                                        (0x00F00000u)
#define HYPER_MTR1_RCSS_SHIFT                                                  (20u)
#define HYPER_MTR1_WCSHI                                                       (0x0F000000u)
#define HYPER_MTR1_WCSHI_SHIFT                                                 (24u)
#define HYPER_MTR1_RCSHI                                                       (0xF0000000u)
#define HYPER_MTR1_RCSHI_SHIFT                                                 (28u)

#endif
