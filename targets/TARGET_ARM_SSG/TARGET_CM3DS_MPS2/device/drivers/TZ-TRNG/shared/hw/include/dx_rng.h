/******************************************************************************
 * Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
 * SPDX-License-Identifier: Apache-2.0                                         *
 *                                                                             *
 * Licensed under the Apache License, Version 2.0 (the "License");             *
 * you may not use this file except in compliance with the License.            *
 *                                                                             *
 * You may obtain a copy of the License at                                     *
 * http://www.apache.org/licenses/LICENSE-2.0                                  *
 *                                                                             *
 * Unless required by applicable law or agreed to in writing, software         *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
 *                                                                             *
 * See the License for the specific language governing permissions and         *
 * limitations under the License.                                              *
 ******************************************************************************/

 #ifndef __DX_RNG_H__
#define __DX_RNG_H__

// --------------------------------------
// BLOCK: RNG
// --------------------------------------
#define DX_RNG_IMR_REG_OFFSET 	0x100UL 
#define DX_RNG_IMR_EHR_VALID_INT_MASK_BIT_SHIFT 	0x0UL
#define DX_RNG_IMR_EHR_VALID_INT_MASK_BIT_SIZE 	0x1UL
#define DX_RNG_IMR_AUTOCORR_ERR_INT_MASK_BIT_SHIFT 	0x1UL
#define DX_RNG_IMR_AUTOCORR_ERR_INT_MASK_BIT_SIZE 	0x1UL
#define DX_RNG_IMR_CRNGT_ERR_INT_MASK_BIT_SHIFT 	0x2UL
#define DX_RNG_IMR_CRNGT_ERR_INT_MASK_BIT_SIZE 	0x1UL
#define DX_RNG_IMR_VN_ERR_INT_MASK_BIT_SHIFT 	0x3UL
#define DX_RNG_IMR_VN_ERR_INT_MASK_BIT_SIZE 	0x1UL
#define DX_RNG_ISR_REG_OFFSET 	0x104UL 
#define DX_RNG_ISR_EHR_VALID_BIT_SHIFT 	0x0UL
#define DX_RNG_ISR_EHR_VALID_BIT_SIZE 	0x1UL
#define DX_RNG_ISR_AUTOCORR_ERR_BIT_SHIFT 	0x1UL
#define DX_RNG_ISR_AUTOCORR_ERR_BIT_SIZE 	0x1UL
#define DX_RNG_ISR_CRNGT_ERR_BIT_SHIFT 	0x2UL
#define DX_RNG_ISR_CRNGT_ERR_BIT_SIZE 	0x1UL
#define DX_RNG_ISR_VN_ERR_BIT_SHIFT 	0x3UL
#define DX_RNG_ISR_VN_ERR_BIT_SIZE 	0x1UL
#define DX_RNG_ICR_REG_OFFSET 	0x108UL 
#define DX_RNG_ICR_EHR_VALID_BIT_SHIFT 	0x0UL
#define DX_RNG_ICR_EHR_VALID_BIT_SIZE 	0x1UL
#define DX_RNG_ICR_AUTOCORR_ERR_BIT_SHIFT 	0x1UL
#define DX_RNG_ICR_AUTOCORR_ERR_BIT_SIZE 	0x1UL
#define DX_RNG_ICR_CRNGT_ERR_BIT_SHIFT 	0x2UL
#define DX_RNG_ICR_CRNGT_ERR_BIT_SIZE 	0x1UL
#define DX_RNG_ICR_VN_ERR_BIT_SHIFT 	0x3UL
#define DX_RNG_ICR_VN_ERR_BIT_SIZE 	0x1UL
#define DX_TRNG_CONFIG_REG_OFFSET 	0x10CUL 
#define DX_TRNG_CONFIG_RND_SRC_SEL_BIT_SHIFT 	0x0UL
#define DX_TRNG_CONFIG_RND_SRC_SEL_BIT_SIZE 	0x2UL
#define DX_TRNG_VALID_REG_OFFSET 	0x110UL 
#define DX_TRNG_VALID_VALUE_BIT_SHIFT 	0x0UL
#define DX_TRNG_VALID_VALUE_BIT_SIZE 	0x1UL
#define DX_EHR_DATA_0_REG_OFFSET 	0x114UL 
#define DX_EHR_DATA_0_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_0_VALUE_BIT_SIZE 	0x20UL
#define DX_EHR_DATA_1_REG_OFFSET 	0x118UL 
#define DX_EHR_DATA_1_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_1_VALUE_BIT_SIZE 	0x20UL
#define DX_EHR_DATA_2_REG_OFFSET 	0x11CUL 
#define DX_EHR_DATA_2_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_2_VALUE_BIT_SIZE 	0x20UL
#define DX_EHR_DATA_3_REG_OFFSET 	0x120UL 
#define DX_EHR_DATA_3_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_3_VALUE_BIT_SIZE 	0x20UL
#define DX_EHR_DATA_4_REG_OFFSET 	0x124UL 
#define DX_EHR_DATA_4_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_4_VALUE_BIT_SIZE 	0x20UL
#define DX_EHR_DATA_5_REG_OFFSET 	0x128UL 
#define DX_EHR_DATA_5_VALUE_BIT_SHIFT 	0x0UL
#define DX_EHR_DATA_5_VALUE_BIT_SIZE 	0x20UL
#define DX_RND_SOURCE_ENABLE_REG_OFFSET 	0x12CUL 
#define DX_RND_SOURCE_ENABLE_VALUE_BIT_SHIFT 	0x0UL
#define DX_RND_SOURCE_ENABLE_VALUE_BIT_SIZE 	0x1UL
#define DX_SAMPLE_CNT1_REG_OFFSET 	0x130UL 
#define DX_SAMPLE_CNT1_VALUE_BIT_SHIFT 	0x0UL
#define DX_SAMPLE_CNT1_VALUE_BIT_SIZE 	0x20UL
#define DX_AUTOCORR_STATISTIC_REG_OFFSET 	0x134UL 
#define DX_AUTOCORR_STATISTIC_AUTOCORR_TRYS_BIT_SHIFT 	0x0UL
#define DX_AUTOCORR_STATISTIC_AUTOCORR_TRYS_BIT_SIZE 	0xEUL
#define DX_AUTOCORR_STATISTIC_AUTOCORR_FAILS_BIT_SHIFT 	0xEUL
#define DX_AUTOCORR_STATISTIC_AUTOCORR_FAILS_BIT_SIZE 	0x8UL
#define DX_TRNG_DEBUG_CONTROL_REG_OFFSET 	0x138UL 
#define DX_TRNG_DEBUG_CONTROL_VNC_BYPASS_BIT_SHIFT 	0x1UL
#define DX_TRNG_DEBUG_CONTROL_VNC_BYPASS_BIT_SIZE 	0x1UL
#define DX_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_BIT_SHIFT 	0x2UL
#define DX_TRNG_DEBUG_CONTROL_TRNG_CRNGT_BYPASS_BIT_SIZE 	0x1UL
#define DX_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_BIT_SHIFT 	0x3UL
#define DX_TRNG_DEBUG_CONTROL_AUTO_CORRELATE_BYPASS_BIT_SIZE 	0x1UL
#define DX_RNG_SW_RESET_REG_OFFSET 	0x140UL 
#define DX_RNG_SW_RESET_VALUE_BIT_SHIFT 	0x0UL
#define DX_RNG_SW_RESET_VALUE_BIT_SIZE 	0x1UL
#define DX_RNG_BUSY_REG_OFFSET 	0x1B8UL 
#define DX_RNG_BUSY_RNG_BUSY_BIT_SHIFT 	0x0UL
#define DX_RNG_BUSY_RNG_BUSY_BIT_SIZE 	0x1UL
#define DX_RST_BITS_COUNTER_REG_OFFSET 	0x1BCUL 
#define DX_RST_BITS_COUNTER_VALUE_BIT_SHIFT 	0x0UL
#define DX_RST_BITS_COUNTER_VALUE_BIT_SIZE 	0x1UL
#define DX_RNG_BIST_CNTR_0_REG_OFFSET 	0x1E0UL 
#define DX_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_BIT_SHIFT 	0x0UL
#define DX_RNG_BIST_CNTR_0_ROSC_CNTR_VAL_BIT_SIZE 	0x16UL
#define DX_RNG_BIST_CNTR_1_REG_OFFSET 	0x1E4UL 
#define DX_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_BIT_SHIFT 	0x0UL
#define DX_RNG_BIST_CNTR_1_ROSC_CNTR_VAL_BIT_SIZE 	0x16UL
#define DX_RNG_BIST_CNTR_2_REG_OFFSET 	0x1E8UL 
#define DX_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_BIT_SHIFT 	0x0UL
#define DX_RNG_BIST_CNTR_2_ROSC_CNTR_VAL_BIT_SIZE 	0x16UL
#endif //__DX_RNG_H__
