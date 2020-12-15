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

#ifndef POE3_IOBITMASK_H
#define POE3_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define POE3_ICSR1_POE0M                                                       (0x0003u)
#define POE3_ICSR1_POE0M_SHIFT                                                 (0u)
#define POE3_ICSR1_PIE1                                                        (0x0100u)
#define POE3_ICSR1_PIE1_SHIFT                                                  (8u)
#define POE3_ICSR1_POE0F                                                       (0x1000u)
#define POE3_ICSR1_POE0F_SHIFT                                                 (12u)
#define POE3_OCSR1_OIE1                                                        (0x0100u)
#define POE3_OCSR1_OIE1_SHIFT                                                  (8u)
#define POE3_OCSR1_OCE1                                                        (0x0200u)
#define POE3_OCSR1_OCE1_SHIFT                                                  (9u)
#define POE3_OCSR1_OSF1                                                        (0x8000u)
#define POE3_OCSR1_OSF1_SHIFT                                                  (15u)
#define POE3_ICSR2_POE4M                                                       (0x0003u)
#define POE3_ICSR2_POE4M_SHIFT                                                 (0u)
#define POE3_ICSR2_PIE2                                                        (0x0100u)
#define POE3_ICSR2_PIE2_SHIFT                                                  (8u)
#define POE3_ICSR2_POE4F                                                       (0x1000u)
#define POE3_ICSR2_POE4F_SHIFT                                                 (12u)
#define POE3_OCSR2_OIE2                                                        (0x0100u)
#define POE3_OCSR2_OIE2_SHIFT                                                  (8u)
#define POE3_OCSR2_OCE2                                                        (0x0200u)
#define POE3_OCSR2_OCE2_SHIFT                                                  (9u)
#define POE3_OCSR2_OSF2                                                        (0x8000u)
#define POE3_OCSR2_OSF2_SHIFT                                                  (15u)
#define POE3_ICSR3_POE8M                                                       (0x0003u)
#define POE3_ICSR3_POE8M_SHIFT                                                 (0u)
#define POE3_ICSR3_PIE3                                                        (0x0100u)
#define POE3_ICSR3_PIE3_SHIFT                                                  (8u)
#define POE3_ICSR3_POE8E                                                       (0x0200u)
#define POE3_ICSR3_POE8E_SHIFT                                                 (9u)
#define POE3_ICSR3_POE8F                                                       (0x1000u)
#define POE3_ICSR3_POE8F_SHIFT                                                 (12u)
#define POE3_SPOER_MTUCH34HIZ                                                  (0x01u)
#define POE3_SPOER_MTUCH34HIZ_SHIFT                                            (0u)
#define POE3_SPOER_MTUCH67HIZ                                                  (0x02u)
#define POE3_SPOER_MTUCH67HIZ_SHIFT                                            (1u)
#define POE3_SPOER_MTUCH0HIZ                                                   (0x04u)
#define POE3_SPOER_MTUCH0HIZ_SHIFT                                             (2u)
#define POE3_POECR1_MTU0AZE                                                    (0x01u)
#define POE3_POECR1_MTU0AZE_SHIFT                                              (0u)
#define POE3_POECR1_MTU0BZE                                                    (0x02u)
#define POE3_POECR1_MTU0BZE_SHIFT                                              (1u)
#define POE3_POECR1_MTU0CZE                                                    (0x04u)
#define POE3_POECR1_MTU0CZE_SHIFT                                              (2u)
#define POE3_POECR1_MTU0DZE                                                    (0x08u)
#define POE3_POECR1_MTU0DZE_SHIFT                                              (3u)
#define POE3_POECR2_MTU7BDZE                                                   (0x0001u)
#define POE3_POECR2_MTU7BDZE_SHIFT                                             (0u)
#define POE3_POECR2_MTU7ACZE                                                   (0x0002u)
#define POE3_POECR2_MTU7ACZE_SHIFT                                             (1u)
#define POE3_POECR2_MTU6BDZE                                                   (0x0004u)
#define POE3_POECR2_MTU6BDZE_SHIFT                                             (2u)
#define POE3_POECR2_MTU4BDZE                                                   (0x0100u)
#define POE3_POECR2_MTU4BDZE_SHIFT                                             (8u)
#define POE3_POECR2_MTU4ACZE                                                   (0x0200u)
#define POE3_POECR2_MTU4ACZE_SHIFT                                             (9u)
#define POE3_POECR2_MTU3BDZE                                                   (0x0400u)
#define POE3_POECR2_MTU3BDZE_SHIFT                                             (10u)
#define POE3_POECR4_IC2ADDMT34ZE                                               (0x0004u)
#define POE3_POECR4_IC2ADDMT34ZE_SHIFT                                         (2u)
#define POE3_POECR4_IC3ADDMT34ZE                                               (0x0008u)
#define POE3_POECR4_IC3ADDMT34ZE_SHIFT                                         (3u)
#define POE3_POECR4_IC4ADDMT34ZE                                               (0x0010u)
#define POE3_POECR4_IC4ADDMT34ZE_SHIFT                                         (4u)
#define POE3_POECR4_IC1ADDMT67ZE                                               (0x0200u)
#define POE3_POECR4_IC1ADDMT67ZE_SHIFT                                         (9u)
#define POE3_POECR4_IC3ADDMT67ZE                                               (0x0800u)
#define POE3_POECR4_IC3ADDMT67ZE_SHIFT                                         (11u)
#define POE3_POECR4_IC4ADDMT67ZE                                               (0x1000u)
#define POE3_POECR4_IC4ADDMT67ZE_SHIFT                                         (12u)
#define POE3_POECR5_IC1ADDMT0ZE                                                (0x0002u)
#define POE3_POECR5_IC1ADDMT0ZE_SHIFT                                          (1u)
#define POE3_POECR5_IC2ADDMT0ZE                                                (0x0004u)
#define POE3_POECR5_IC2ADDMT0ZE_SHIFT                                          (2u)
#define POE3_POECR5_IC4ADDMT0ZE                                                (0x0010u)
#define POE3_POECR5_IC4ADDMT0ZE_SHIFT                                          (4u)
#define POE3_ICSR4_POE10M                                                      (0x0003u)
#define POE3_ICSR4_POE10M_SHIFT                                                (0u)
#define POE3_ICSR4_PIE4                                                        (0x0100u)
#define POE3_ICSR4_PIE4_SHIFT                                                  (8u)
#define POE3_ICSR4_POE10E                                                      (0x0200u)
#define POE3_ICSR4_POE10E_SHIFT                                                (9u)
#define POE3_ICSR4_POE10F                                                      (0x1000u)
#define POE3_ICSR4_POE10F_SHIFT                                                (12u)
#define POE3_M0SELR1_M0ASEL                                                    (0x0Fu)
#define POE3_M0SELR1_M0ASEL_SHIFT                                              (0u)
#define POE3_M0SELR1_M0BSEL                                                    (0xF0u)
#define POE3_M0SELR1_M0BSEL_SHIFT                                              (4u)
#define POE3_M0SELR2_M0CSEL                                                    (0x0Fu)
#define POE3_M0SELR2_M0CSEL_SHIFT                                              (0u)
#define POE3_M0SELR2_M0DSEL                                                    (0xF0u)
#define POE3_M0SELR2_M0DSEL_SHIFT                                              (4u)

#endif
