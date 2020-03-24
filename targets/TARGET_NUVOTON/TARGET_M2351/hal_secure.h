/*
 * Copyright (c) 2017-2018, Nuvoton Technology Corporation
 *
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

#ifndef HAL_SECURE_H
#define HAL_SECURE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pin_function (secure version)
 *
 * Guard access to secure GPIO from non-secure domain.
 * 
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void pin_function_s(int32_t pin, int32_t data);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t pin_function_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* NOTE: time_t
 *
 * time_t may be defined to int32_t/int64_t dependent on compilers. For binary-compatible
 * among compilers, we must replace time_t with int32_t/int64_t explicitly in rtc secure
 * functions. We determine to choose int64_t rather than int32_t to avoid 'Year 2038 problem'.
 */

/* rtc_init (secure version) */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_init_s(void);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t rtc_init_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif


/* rtc_free (secure version) */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_free_s(void);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t rtc_free_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* rtc_isenabled (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
int32_t rtc_isenabled_s(void);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t rtc_isenabled_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* rtc_read (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
int64_t rtc_read_s(void);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t rtc_read_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* rtc_write (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void rtc_write_s(int64_t t);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t rtc_write_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* trng_init (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void trng_init_s(void *obj);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t trng_init_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* trng_free (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
void trng_free_s(void *obj);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t trng_free_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

/* trng_get_bytes (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && (TFM_LVL == 0)
__NONSECURE_ENTRY
#endif
int32_t trng_get_bytes_s(void *obj, uint8_t *output, uint32_t length, uint32_t *output_length);
#if (TFM_LVL > 0)
__NONSECURE_ENTRY
int32_t trng_get_bytes_veneer(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3);
#endif

#ifdef __cplusplus
}
#endif

#endif
