/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 Nuvoton
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
__NONSECURE_ENTRY
void pin_function_s(int32_t pin, int32_t data);

/* NOTE: time_t
 *
 * time_t may be defined to int32_t/int64_t dependent on compilers. For binary-compatible
 * among compilers, we must replace time_t with int32_t/int64_t explicitly in rtc secure
 * functions. We determine to choose int64_t rather than int32_t to avoid 'Year 2038 problem'.
 */

/* rtc_init (secure version) */
__NONSECURE_ENTRY
void rtc_init_s(void);

/* rtc_free (secure version) */
__NONSECURE_ENTRY
void rtc_free_s(void);

/* rtc_isenabled (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
__NONSECURE_ENTRY
int32_t rtc_isenabled_s(void);

/* rtc_read (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
__NONSECURE_ENTRY
int64_t rtc_read_s(void);

/* rtc_write (secure version)
 *
 * Its synopsis is the same as normal version except change of return/argument type for
 * binary-compatible across compilers.
 */
__NONSECURE_ENTRY
void rtc_write_s(int64_t t);

#ifdef __cplusplus
}
#endif

#endif
