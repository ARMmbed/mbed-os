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

#ifndef ADC_IOBITMASK_H
#define ADC_IOBITMASK_H


/* ==== Mask values for IO registers ==== */

#define ADC_ADCSR_DBLANS                                                       (0x001Fu)
#define ADC_ADCSR_DBLANS_SHIFT                                                 (0u)
#define ADC_ADCSR_GBADIE                                                       (0x0040u)
#define ADC_ADCSR_GBADIE_SHIFT                                                 (6u)
#define ADC_ADCSR_DBLE                                                         (0x0080u)
#define ADC_ADCSR_DBLE_SHIFT                                                   (7u)
#define ADC_ADCSR_EXTRG                                                        (0x0100u)
#define ADC_ADCSR_EXTRG_SHIFT                                                  (8u)
#define ADC_ADCSR_TRGE                                                         (0x0200u)
#define ADC_ADCSR_TRGE_SHIFT                                                   (9u)
#define ADC_ADCSR_ADIE                                                         (0x1000u)
#define ADC_ADCSR_ADIE_SHIFT                                                   (12u)
#define ADC_ADCSR_ADCS                                                         (0x6000u)
#define ADC_ADCSR_ADCS_SHIFT                                                   (13u)
#define ADC_ADCSR_ADST                                                         (0x8000u)
#define ADC_ADCSR_ADST_SHIFT                                                   (15u)
#define ADC_ADANSA0_ANSA0                                                      (0x00FFu)
#define ADC_ADANSA0_ANSA0_SHIFT                                                (0u)
#define ADC_ADADS0_ADS0                                                        (0x00FFu)
#define ADC_ADADS0_ADS0_SHIFT                                                  (0u)
#define ADC_ADADC_ADC_2_0                                                      (0x07u)
#define ADC_ADADC_ADC_2_0_SHIFT                                                (0u)
#define ADC_ADADC_AVEE                                                         (0x80u)
#define ADC_ADADC_AVEE_SHIFT                                                   (7u)
#define ADC_ADCER_ADPRC                                                        (0x0006u)
#define ADC_ADCER_ADPRC_SHIFT                                                  (1u)
#define ADC_ADCER_ACE                                                          (0x0020u)
#define ADC_ADCER_ACE_SHIFT                                                    (5u)
#define ADC_ADCER_DIAGVAL                                                      (0x0300u)
#define ADC_ADCER_DIAGVAL_SHIFT                                                (8u)
#define ADC_ADCER_DIAGLD                                                       (0x0400u)
#define ADC_ADCER_DIAGLD_SHIFT                                                 (10u)
#define ADC_ADCER_DIAGM                                                        (0x0800u)
#define ADC_ADCER_DIAGM_SHIFT                                                  (11u)
#define ADC_ADCER_ADRFMT                                                       (0x8000u)
#define ADC_ADCER_ADRFMT_SHIFT                                                 (15u)
#define ADC_ADSTRGR_TRSB                                                       (0x003Fu)
#define ADC_ADSTRGR_TRSB_SHIFT                                                 (0u)
#define ADC_ADSTRGR_TRSA                                                       (0x3F00u)
#define ADC_ADSTRGR_TRSA_SHIFT                                                 (8u)
#define ADC_ADANSB0_ANSB0                                                      (0x00FFu)
#define ADC_ADANSB0_ANSB0_SHIFT                                                (0u)
#define ADC_ADDBLDR_AD                                                         (0xFFFFu)
#define ADC_ADDBLDR_AD_SHIFT                                                   (0u)
#define ADC_ADRD_AD                                                            (0xFFFFu)
#define ADC_ADRD_AD_SHIFT                                                      (0u)
#define ADC_ADDR0_AD                                                           (0xFFFFu)
#define ADC_ADDR0_AD_SHIFT                                                     (0u)
#define ADC_ADDR1_AD                                                           (0xFFFFu)
#define ADC_ADDR1_AD_SHIFT                                                     (0u)
#define ADC_ADDR2_AD                                                           (0xFFFFu)
#define ADC_ADDR2_AD_SHIFT                                                     (0u)
#define ADC_ADDR3_AD                                                           (0xFFFFu)
#define ADC_ADDR3_AD_SHIFT                                                     (0u)
#define ADC_ADDR4_AD                                                           (0xFFFFu)
#define ADC_ADDR4_AD_SHIFT                                                     (0u)
#define ADC_ADDR5_AD                                                           (0xFFFFu)
#define ADC_ADDR5_AD_SHIFT                                                     (0u)
#define ADC_ADDR6_AD                                                           (0xFFFFu)
#define ADC_ADDR6_AD_SHIFT                                                     (0u)
#define ADC_ADDR7_AD                                                           (0xFFFFu)
#define ADC_ADDR7_AD_SHIFT                                                     (0u)
#define ADC_ADDISCR_ADNDIS                                                     (0x1Fu)
#define ADC_ADDISCR_ADNDIS_SHIFT                                               (0u)
#define ADC_ADGSPCR_PGS                                                        (0x0001u)
#define ADC_ADGSPCR_PGS_SHIFT                                                  (0u)
#define ADC_ADGSPCR_GBRSCN                                                     (0x0002u)
#define ADC_ADGSPCR_GBRSCN_SHIFT                                               (1u)
#define ADC_ADGSPCR_LGRRS                                                      (0x4000u)
#define ADC_ADGSPCR_LGRRS_SHIFT                                                (14u)
#define ADC_ADGSPCR_GBRP                                                       (0x8000u)
#define ADC_ADGSPCR_GBRP_SHIFT                                                 (15u)
#define ADC_ADDBLDRA_AD                                                        (0xFFFFu)
#define ADC_ADDBLDRA_AD_SHIFT                                                  (0u)
#define ADC_ADDBLDRB_AD                                                        (0xFFFFu)
#define ADC_ADDBLDRB_AD_SHIFT                                                  (0u)
#define ADC_ADWINMON_MONCOMB                                                   (0x01u)
#define ADC_ADWINMON_MONCOMB_SHIFT                                             (0u)
#define ADC_ADWINMON_MONCMPA                                                   (0x10u)
#define ADC_ADWINMON_MONCMPA_SHIFT                                             (4u)
#define ADC_ADWINMON_MONCMPB                                                   (0x20u)
#define ADC_ADWINMON_MONCMPB_SHIFT                                             (5u)
#define ADC_ADCMPCR_CMPBE                                                      (0x0200u)
#define ADC_ADCMPCR_CMPBE_SHIFT                                                (9u)
#define ADC_ADCMPCR_CMPAE                                                      (0x0800u)
#define ADC_ADCMPCR_CMPAE_SHIFT                                                (11u)
#define ADC_ADCMPCR_CMPBIE                                                     (0x2000u)
#define ADC_ADCMPCR_CMPBIE_SHIFT                                               (13u)
#define ADC_ADCMPCR_WCMPE                                                      (0x4000u)
#define ADC_ADCMPCR_WCMPE_SHIFT                                                (14u)
#define ADC_ADCMPCR_CMPAIE                                                     (0x8000u)
#define ADC_ADCMPCR_CMPAIE_SHIFT                                               (15u)
#define ADC_ADCMPANSR0_CMPCHA0                                                 (0x00FFu)
#define ADC_ADCMPANSR0_CMPCHA0_SHIFT                                           (0u)
#define ADC_ADCMPLR0_CMPLCHA0                                                  (0x00FFu)
#define ADC_ADCMPLR0_CMPLCHA0_SHIFT                                            (0u)
#define ADC_ADCMPDR0_CMPD0                                                     (0xFFFFu)
#define ADC_ADCMPDR0_CMPD0_SHIFT                                               (0u)
#define ADC_ADCMPDR1_CMPD1                                                     (0xFFFFu)
#define ADC_ADCMPDR1_CMPD1_SHIFT                                               (0u)
#define ADC_ADCMPSR0_CMPSTCHA0                                                 (0x00FFu)
#define ADC_ADCMPSR0_CMPSTCHA0_SHIFT                                           (0u)
#define ADC_ADCMPBNSR_CMPCHB                                                   (0x3Fu)
#define ADC_ADCMPBNSR_CMPCHB_SHIFT                                             (0u)
#define ADC_ADCMPBNSR_CMPLB                                                    (0x80u)
#define ADC_ADCMPBNSR_CMPLB_SHIFT                                              (7u)
#define ADC_ADWINLLB_CMPLLB                                                    (0xFFFFu)
#define ADC_ADWINLLB_CMPLLB_SHIFT                                              (0u)
#define ADC_ADWINULB_CMPULB                                                    (0xFFFFu)
#define ADC_ADWINULB_CMPULB_SHIFT                                              (0u)
#define ADC_ADCMPBSR_CMPSTB                                                    (0x01u)
#define ADC_ADCMPBSR_CMPSTB_SHIFT                                              (0u)
#define ADC_ADANSC0_ANSC0                                                      (0x00FFu)
#define ADC_ADANSC0_ANSC0_SHIFT                                                (0u)
#define ADC_ADGCTRGR_TRSC                                                      (0x3Fu)
#define ADC_ADGCTRGR_TRSC_SHIFT                                                (0u)
#define ADC_ADGCTRGR_GCADIE                                                    (0x40u)
#define ADC_ADGCTRGR_GCADIE_SHIFT                                              (6u)
#define ADC_ADGCTRGR_GRCE                                                      (0x80u)
#define ADC_ADGCTRGR_GRCE_SHIFT                                                (7u)
#define ADC_ADSSTR0_SST                                                        (0xFFu)
#define ADC_ADSSTR0_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR1_SST                                                        (0xFFu)
#define ADC_ADSSTR1_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR2_SST                                                        (0xFFu)
#define ADC_ADSSTR2_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR3_SST                                                        (0xFFu)
#define ADC_ADSSTR3_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR4_SST                                                        (0xFFu)
#define ADC_ADSSTR4_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR5_SST                                                        (0xFFu)
#define ADC_ADSSTR5_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR6_SST                                                        (0xFFu)
#define ADC_ADSSTR6_SST_SHIFT                                                  (0u)
#define ADC_ADSSTR7_SST                                                        (0xFFu)
#define ADC_ADSSTR7_SST_SHIFT                                                  (0u)

#endif
