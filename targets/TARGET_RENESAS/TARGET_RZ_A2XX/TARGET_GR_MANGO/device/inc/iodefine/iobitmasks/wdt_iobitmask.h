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

#ifndef WDT_IOBITMASK_H
#define WDT_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define WDT_WTCSR_CKS                                                          (0x000Fu)
#define WDT_WTCSR_CKS_SHIFT                                                    (0u)
#define WDT_WTCSR_TME                                                          (0x0020u)
#define WDT_WTCSR_TME_SHIFT                                                    (5u)
#define WDT_WTCSR_WTIT                                                         (0x0040u)
#define WDT_WTCSR_WTIT_SHIFT                                                   (6u)
#define WDT_WTCSR_IOVF                                                         (0x0080u)
#define WDT_WTCSR_IOVF_SHIFT                                                   (7u)
#define WDT_WTCNT_WTCNT                                                        (0x00FFu)
#define WDT_WTCNT_WTCNT_SHIFT                                                  (0u)
#define WDT_WRCSR_RSTE                                                         (0x0040u)
#define WDT_WRCSR_RSTE_SHIFT                                                   (6u)
#define WDT_WRCSR_WOVF                                                         (0x0080u)
#define WDT_WRCSR_WOVF_SHIFT                                                   (7u)
#define WDT_PEER_PEE                                                           (0x00FFu)
#define WDT_PEER_PEE_SHIFT                                                     (0u)
#define WDT_PECR_PERIE                                                         (0x00FFu)
#define WDT_PECR_PERIE_SHIFT                                                   (0u)
#define WDT_PESR_PEF                                                           (0x00FFu)
#define WDT_PESR_PEF_SHIFT                                                     (0u)

#endif
