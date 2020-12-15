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

#ifndef PMG_IOBITMASK_H
#define PMG_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define PMG_RRAMKP_RRAMKP0                                                     (0x01u)
#define PMG_RRAMKP_RRAMKP0_SHIFT                                               (0u)
#define PMG_RRAMKP_RRAMKP1                                                     (0x02u)
#define PMG_RRAMKP_RRAMKP1_SHIFT                                               (1u)
#define PMG_RRAMKP_RRAMKP2                                                     (0x04u)
#define PMG_RRAMKP_RRAMKP2_SHIFT                                               (2u)
#define PMG_RRAMKP_RRAMKP3                                                     (0x08u)
#define PMG_RRAMKP_RRAMKP3_SHIFT                                               (3u)
#define PMG_DSCTR_RAMBOOT                                                      (0x40u)
#define PMG_DSCTR_RAMBOOT_SHIFT                                                (6u)
#define PMG_DSCTR_EBUSKEEPE                                                    (0x80u)
#define PMG_DSCTR_EBUSKEEPE_SHIFT                                              (7u)
#define PMG_DSSSR_P3_1                                                         (0x0001u)
#define PMG_DSSSR_P3_1_SHIFT                                                   (0u)
#define PMG_DSSSR_P3_3                                                         (0x0002u)
#define PMG_DSSSR_P3_3_SHIFT                                                   (1u)
#define PMG_DSSSR_P6_2                                                         (0x0004u)
#define PMG_DSSSR_P6_2_SHIFT                                                   (2u)
#define PMG_DSSSR_PE_1                                                         (0x0008u)
#define PMG_DSSSR_PE_1_SHIFT                                                   (3u)
#define PMG_DSSSR_PH_1                                                         (0x0010u)
#define PMG_DSSSR_PH_1_SHIFT                                                   (4u)
#define PMG_DSSSR_PG_2                                                         (0x0020u)
#define PMG_DSSSR_PG_2_SHIFT                                                   (5u)
#define PMG_DSSSR_RTCAR0                                                       (0x0040u)
#define PMG_DSSSR_RTCAR0_SHIFT                                                 (6u)
#define PMG_DSSSR_RTCAR1                                                       (0x0080u)
#define PMG_DSSSR_RTCAR1_SHIFT                                                 (7u)
#define PMG_DSSSR_NMI                                                          (0x0100u)
#define PMG_DSSSR_NMI_SHIFT                                                    (8u)
#define PMG_DSSSR_PG_6                                                         (0x0200u)
#define PMG_DSSSR_PG_6_SHIFT                                                   (9u)
#define PMG_DSSSR_PH_0                                                         (0x0400u)
#define PMG_DSSSR_PH_0_SHIFT                                                   (10u)
#define PMG_DSSSR_PJ_1                                                         (0x0800u)
#define PMG_DSSSR_PJ_1_SHIFT                                                   (11u)
#define PMG_DSSSR_PJ_5                                                         (0x1000u)
#define PMG_DSSSR_PJ_5_SHIFT                                                   (12u)
#define PMG_DSSSR_PK_2                                                         (0x2000u)
#define PMG_DSSSR_PK_2_SHIFT                                                   (13u)
#define PMG_DSSSR_PK_4                                                         (0x4000u)
#define PMG_DSSSR_PK_4_SHIFT                                                   (14u)
#define PMG_DSESR_P3_1E                                                        (0x0001u)
#define PMG_DSESR_P3_1E_SHIFT                                                  (0u)
#define PMG_DSESR_P3_3E                                                        (0x0002u)
#define PMG_DSESR_P3_3E_SHIFT                                                  (1u)
#define PMG_DSESR_P6_2E                                                        (0x0004u)
#define PMG_DSESR_P6_2E_SHIFT                                                  (2u)
#define PMG_DSESR_PE_1E                                                        (0x0008u)
#define PMG_DSESR_PE_1E_SHIFT                                                  (3u)
#define PMG_DSESR_PH_1E                                                        (0x0010u)
#define PMG_DSESR_PH_1E_SHIFT                                                  (4u)
#define PMG_DSESR_PG_2E                                                        (0x0020u)
#define PMG_DSESR_PG_2E_SHIFT                                                  (5u)
#define PMG_DSESR_NMIE                                                         (0x0100u)
#define PMG_DSESR_NMIE_SHIFT                                                   (8u)
#define PMG_DSESR_PG_6E                                                        (0x0200u)
#define PMG_DSESR_PG_6E_SHIFT                                                  (9u)
#define PMG_DSESR_PH_0E                                                        (0x0400u)
#define PMG_DSESR_PH_0E_SHIFT                                                  (10u)
#define PMG_DSESR_PJ_1E                                                        (0x0800u)
#define PMG_DSESR_PJ_1E_SHIFT                                                  (11u)
#define PMG_DSESR_PJ_5E                                                        (0x1000u)
#define PMG_DSESR_PJ_5E_SHIFT                                                  (12u)
#define PMG_DSESR_PK_2E                                                        (0x2000u)
#define PMG_DSESR_PK_2E_SHIFT                                                  (13u)
#define PMG_DSESR_PK_4E                                                        (0x4000u)
#define PMG_DSESR_PK_4E_SHIFT                                                  (14u)
#define PMG_DSFR_P3_1F                                                         (0x0001u)
#define PMG_DSFR_P3_1F_SHIFT                                                   (0u)
#define PMG_DSFR_P3_3F                                                         (0x0002u)
#define PMG_DSFR_P3_3F_SHIFT                                                   (1u)
#define PMG_DSFR_P6_2F                                                         (0x0004u)
#define PMG_DSFR_P6_2F_SHIFT                                                   (2u)
#define PMG_DSFR_PE_1F                                                         (0x0008u)
#define PMG_DSFR_PE_1F_SHIFT                                                   (3u)
#define PMG_DSFR_PH_1F                                                         (0x0010u)
#define PMG_DSFR_PH_1F_SHIFT                                                   (4u)
#define PMG_DSFR_PG_2F                                                         (0x0020u)
#define PMG_DSFR_PG_2F_SHIFT                                                   (5u)
#define PMG_DSFR_RTCARF0                                                       (0x0040u)
#define PMG_DSFR_RTCARF0_SHIFT                                                 (6u)
#define PMG_DSFR_RTCARF1                                                       (0x0080u)
#define PMG_DSFR_RTCARF1_SHIFT                                                 (7u)
#define PMG_DSFR_NMIF                                                          (0x0100u)
#define PMG_DSFR_NMIF_SHIFT                                                    (8u)
#define PMG_DSFR_PG_6F                                                         (0x0200u)
#define PMG_DSFR_PG_6F_SHIFT                                                   (9u)
#define PMG_DSFR_PH_0F                                                         (0x0400u)
#define PMG_DSFR_PH_0F_SHIFT                                                   (10u)
#define PMG_DSFR_PJ_1F                                                         (0x0800u)
#define PMG_DSFR_PJ_1F_SHIFT                                                   (11u)
#define PMG_DSFR_PJ_5F                                                         (0x1000u)
#define PMG_DSFR_PJ_5F_SHIFT                                                   (12u)
#define PMG_DSFR_PK_2F                                                         (0x2000u)
#define PMG_DSFR_PK_2F_SHIFT                                                   (13u)
#define PMG_DSFR_PK_4F                                                         (0x4000u)
#define PMG_DSFR_PK_4F_SHIFT                                                   (14u)
#define PMG_DSFR_IOKEEP                                                        (0x8000u)
#define PMG_DSFR_IOKEEP_SHIFT                                                  (15u)
#define PMG_DSCNT_CNTD                                                         (0x00FFu)
#define PMG_DSCNT_CNTD_SHIFT                                                   (0u)
#define PMG_XTALCTR_GAIN0                                                      (0x01u)
#define PMG_XTALCTR_GAIN0_SHIFT                                                (0u)
#define PMG_USBDSSSR_USBDSCE0                                                  (0x01u)
#define PMG_USBDSSSR_USBDSCE0_SHIFT                                            (0u)
#define PMG_USBDSSSR_USBDSCE1                                                  (0x02u)
#define PMG_USBDSSSR_USBDSCE1_SHIFT                                            (1u)
#define PMG_USBDSSSR_USBDSCE2                                                  (0x04u)
#define PMG_USBDSSSR_USBDSCE2_SHIFT                                            (2u)
#define PMG_USBDSSSR_USBDSCE3                                                  (0x08u)
#define PMG_USBDSSSR_USBDSCE3_SHIFT                                            (3u)
#define PMG_USBDSFR_USBDSF0                                                    (0x01u)
#define PMG_USBDSFR_USBDSF0_SHIFT                                              (0u)
#define PMG_USBDSFR_USBDSF1                                                    (0x02u)
#define PMG_USBDSFR_USBDSF1_SHIFT                                              (1u)
#define PMG_USBDSFR_USBDSF2                                                    (0x04u)
#define PMG_USBDSFR_USBDSF2_SHIFT                                              (2u)
#define PMG_USBDSFR_USBDSF3                                                    (0x08u)
#define PMG_USBDSFR_USBDSF3_SHIFT                                              (3u)
#define PMG_RTCXTALSEL_RTC0XT                                                  (0x0001u)
#define PMG_RTCXTALSEL_RTC0XT_SHIFT                                            (0u)
#define PMG_RTCXTALSEL_RTC1XT                                                  (0x0002u)
#define PMG_RTCXTALSEL_RTC1XT_SHIFT                                            (1u)

#endif
