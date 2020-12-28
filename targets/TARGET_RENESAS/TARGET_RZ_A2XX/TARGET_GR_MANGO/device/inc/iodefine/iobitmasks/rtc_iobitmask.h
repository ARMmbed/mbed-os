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

#ifndef RTC_IOBITMASK_H
#define RTC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define RTC_R64CNT_F64HZ                                                       (0x01u)
#define RTC_R64CNT_F64HZ_SHIFT                                                 (0u)
#define RTC_R64CNT_F32HZ                                                       (0x02u)
#define RTC_R64CNT_F32HZ_SHIFT                                                 (1u)
#define RTC_R64CNT_F16HZ                                                       (0x04u)
#define RTC_R64CNT_F16HZ_SHIFT                                                 (2u)
#define RTC_R64CNT_F8HZ                                                        (0x08u)
#define RTC_R64CNT_F8HZ_SHIFT                                                  (3u)
#define RTC_R64CNT_F4HZ                                                        (0x10u)
#define RTC_R64CNT_F4HZ_SHIFT                                                  (4u)
#define RTC_R64CNT_F2HZ                                                        (0x20u)
#define RTC_R64CNT_F2HZ_SHIFT                                                  (5u)
#define RTC_R64CNT_F1HZ                                                        (0x40u)
#define RTC_R64CNT_F1HZ_SHIFT                                                  (6u)
#define RTC_RSECCNT_SEC1                                                       (0x0Fu)
#define RTC_RSECCNT_SEC1_SHIFT                                                 (0u)
#define RTC_RSECCNT_SEC10                                                      (0x70u)
#define RTC_RSECCNT_SEC10_SHIFT                                                (4u)
#define RTC_RMINCNT_MIN1                                                       (0x0Fu)
#define RTC_RMINCNT_MIN1_SHIFT                                                 (0u)
#define RTC_RMINCNT_MIN10                                                      (0x70u)
#define RTC_RMINCNT_MIN10_SHIFT                                                (4u)
#define RTC_RHRCNT_HR1                                                         (0x0Fu)
#define RTC_RHRCNT_HR1_SHIFT                                                   (0u)
#define RTC_RHRCNT_HR10                                                        (0x30u)
#define RTC_RHRCNT_HR10_SHIFT                                                  (4u)
#define RTC_RHRCNT_PM                                                          (0x40u)
#define RTC_RHRCNT_PM_SHIFT                                                    (6u)
#define RTC_RWKCNT_DAYW                                                        (0x07u)
#define RTC_RWKCNT_DAYW_SHIFT                                                  (0u)
#define RTC_RDAYCNT_DATE1                                                      (0x0Fu)
#define RTC_RDAYCNT_DATE1_SHIFT                                                (0u)
#define RTC_RDAYCNT_DATE10                                                     (0x30u)
#define RTC_RDAYCNT_DATE10_SHIFT                                               (4u)
#define RTC_RMONCNT_MON1                                                       (0x0Fu)
#define RTC_RMONCNT_MON1_SHIFT                                                 (0u)
#define RTC_RMONCNT_MON10                                                      (0x10u)
#define RTC_RMONCNT_MON10_SHIFT                                                (4u)
#define RTC_RYRCNT_YR1                                                         (0x000Fu)
#define RTC_RYRCNT_YR1_SHIFT                                                   (0u)
#define RTC_RYRCNT_YR10                                                        (0x00F0u)
#define RTC_RYRCNT_YR10_SHIFT                                                  (4u)
#define RTC_RSECAR_SEC1                                                        (0x0Fu)
#define RTC_RSECAR_SEC1_SHIFT                                                  (0u)
#define RTC_RSECAR_SEC10                                                       (0x70u)
#define RTC_RSECAR_SEC10_SHIFT                                                 (4u)
#define RTC_RSECAR_ENB                                                         (0x80u)
#define RTC_RSECAR_ENB_SHIFT                                                   (7u)
#define RTC_RMINAR_MIN1                                                        (0x0Fu)
#define RTC_RMINAR_MIN1_SHIFT                                                  (0u)
#define RTC_RMINAR_MIN10                                                       (0x70u)
#define RTC_RMINAR_MIN10_SHIFT                                                 (4u)
#define RTC_RMINAR_ENB                                                         (0x80u)
#define RTC_RMINAR_ENB_SHIFT                                                   (7u)
#define RTC_RHRAR_HR1                                                          (0x0Fu)
#define RTC_RHRAR_HR1_SHIFT                                                    (0u)
#define RTC_RHRAR_HR10                                                         (0x30u)
#define RTC_RHRAR_HR10_SHIFT                                                   (4u)
#define RTC_RHRAR_PM                                                           (0x40u)
#define RTC_RHRAR_PM_SHIFT                                                     (6u)
#define RTC_RHRAR_ENB                                                          (0x80u)
#define RTC_RHRAR_ENB_SHIFT                                                    (7u)
#define RTC_RWKAR_DAYW                                                         (0x07u)
#define RTC_RWKAR_DAYW_SHIFT                                                   (0u)
#define RTC_RWKAR_ENB                                                          (0x80u)
#define RTC_RWKAR_ENB_SHIFT                                                    (7u)
#define RTC_RDAYAR_DATE1                                                       (0x0Fu)
#define RTC_RDAYAR_DATE1_SHIFT                                                 (0u)
#define RTC_RDAYAR_DATE10                                                      (0x30u)
#define RTC_RDAYAR_DATE10_SHIFT                                                (4u)
#define RTC_RDAYAR_ENB                                                         (0x80u)
#define RTC_RDAYAR_ENB_SHIFT                                                   (7u)
#define RTC_RMONAR_MON1                                                        (0x0Fu)
#define RTC_RMONAR_MON1_SHIFT                                                  (0u)
#define RTC_RMONAR_MON10                                                       (0x10u)
#define RTC_RMONAR_MON10_SHIFT                                                 (4u)
#define RTC_RMONAR_ENB                                                         (0x80u)
#define RTC_RMONAR_ENB_SHIFT                                                   (7u)
#define RTC_RYRAR_YR1                                                          (0x000Fu)
#define RTC_RYRAR_YR1_SHIFT                                                    (0u)
#define RTC_RYRAR_YR10                                                         (0x00F0u)
#define RTC_RYRAR_YR10_SHIFT                                                   (4u)
#define RTC_RYRAREN_ENB                                                        (0x80u)
#define RTC_RYRAREN_ENB_SHIFT                                                  (7u)
#define RTC_RSR_AF                                                             (0x01u)
#define RTC_RSR_AF_SHIFT                                                       (0u)
#define RTC_RSR_CF                                                             (0x02u)
#define RTC_RSR_CF_SHIFT                                                       (1u)
#define RTC_RSR_PF                                                             (0x04u)
#define RTC_RSR_PF_SHIFT                                                       (2u)
#define RTC_RCR1_AIE                                                           (0x01u)
#define RTC_RCR1_AIE_SHIFT                                                     (0u)
#define RTC_RCR1_CIE                                                           (0x02u)
#define RTC_RCR1_CIE_SHIFT                                                     (1u)
#define RTC_RCR1_PIE                                                           (0x04u)
#define RTC_RCR1_PIE_SHIFT                                                     (2u)
#define RTC_RCR1_PES                                                           (0xF0u)
#define RTC_RCR1_PES_SHIFT                                                     (4u)
#define RTC_RCR2_START                                                         (0x01u)
#define RTC_RCR2_START_SHIFT                                                   (0u)
#define RTC_RCR2_RESET                                                         (0x02u)
#define RTC_RCR2_RESET_SHIFT                                                   (1u)
#define RTC_RCR2_ADJ30                                                         (0x04u)
#define RTC_RCR2_ADJ30_SHIFT                                                   (2u)
#define RTC_RCR2_AADJE                                                         (0x10u)
#define RTC_RCR2_AADJE_SHIFT                                                   (4u)
#define RTC_RCR2_AADJP                                                         (0x20u)
#define RTC_RCR2_AADJP_SHIFT                                                   (5u)
#define RTC_RCR2_HR24                                                          (0x40u)
#define RTC_RCR2_HR24_SHIFT                                                    (6u)
#define RTC_RCR2_CNTMD                                                         (0x80u)
#define RTC_RCR2_CNTMD_SHIFT                                                   (7u)
#define RTC_RCR3_RTCEN                                                         (0x01u)
#define RTC_RCR3_RTCEN_SHIFT                                                   (0u)
#define RTC_RCR4_RCKSEL                                                        (0x01u)
#define RTC_RCR4_RCKSEL_SHIFT                                                  (0u)
#define RTC_RFRH_RFC                                                           (0x0001u)
#define RTC_RFRH_RFC_SHIFT                                                     (0u)
#define RTC_RFRL_RFC                                                           (0xFFFFu)
#define RTC_RFRL_RFC_SHIFT                                                     (0u)
#define RTC_RADJ_ADJ                                                           (0x3Fu)
#define RTC_RADJ_ADJ_SHIFT                                                     (0u)
#define RTC_RADJ_PMADJ                                                         (0xC0u)
#define RTC_RADJ_PMADJ_SHIFT                                                   (6u)
#define RTC_BCNT0_BCNT                                                         (0xFFu)
#define RTC_BCNT0_BCNT_SHIFT                                                   (0u)
#define RTC_BCNT1_BCNT                                                         (0xFFu)
#define RTC_BCNT1_BCNT_SHIFT                                                   (0u)
#define RTC_BCNT2_BCNT                                                         (0xFFu)
#define RTC_BCNT2_BCNT_SHIFT                                                   (0u)
#define RTC_BCNT3_BCNT                                                         (0xFFu)
#define RTC_BCNT3_BCNT_SHIFT                                                   (0u)
#define RTC_BCNT0AR_BCNTAR                                                     (0xFFu)
#define RTC_BCNT0AR_BCNTAR_SHIFT                                               (0u)
#define RTC_BCNT1AR_BCNTAR                                                     (0xFFu)
#define RTC_BCNT1AR_BCNTAR_SHIFT                                               (0u)
#define RTC_BCNT2AR_BCNTAR                                                     (0xFFu)
#define RTC_BCNT2AR_BCNTAR_SHIFT                                               (0u)
#define RTC_BCNT3AR_BCNTAR                                                     (0xFFu)
#define RTC_BCNT3AR_BCNTAR_SHIFT                                               (0u)
#define RTC_BCNT0AER_ENB                                                       (0xFFu)
#define RTC_BCNT0AER_ENB_SHIFT                                                 (0u)
#define RTC_BCNT1AER_ENB                                                       (0xFFu)
#define RTC_BCNT1AER_ENB_SHIFT                                                 (0u)
#define RTC_BCNT2AER_ENB                                                       (0x00FFu)
#define RTC_BCNT2AER_ENB_SHIFT                                                 (0u)
#define RTC_BCNT3AER_ENB                                                       (0xFFu)
#define RTC_BCNT3AER_ENB_SHIFT                                                 (0u)

#endif
