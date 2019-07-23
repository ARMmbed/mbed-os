
/*
 * Copyright (c) 2015-2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MBED_UTIL_CRITICAL_H__
#define __MBED_UTIL_CRITICAL_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_critical critical section function
 * @{
 */

/** Determine the current interrupts enabled state
  *
  * This function can be called to determine whether or not interrupts are currently enabled.
  * @note
  * NOTE:
  * This function works for both cortex-A and cortex-M, although the underlying implementation
  * differs.
  * @return true if interrupts are enabled, false otherwise
  */
bool core_util_are_interrupts_enabled(void);

/** Determine if this code is executing from an interrupt
  *
  * This function can be called to determine if the code is running on interrupt context.
  * @note
  * NOTE:
  * This function works for both cortex-A and cortex-M, although the underlying implementation
  * differs.
  * @return true if in an isr, false otherwise
  */
bool core_util_is_isr_active(void);

/** Mark the start of a critical section
  *
  * This function should be called to mark the start of a critical section of code.
  * @note
  * NOTES:
  * 1) The use of this style of critical section is targetted at C based implementations.
  * 2) These critical sections can be nested.
  * 3) The interrupt enable state on entry to the first critical section (of a nested set, or single
  *    section) will be preserved on exit from the section.
  * 4) This implementation will currently only work on code running in privileged mode.
  */
void core_util_critical_section_enter(void);

/** Mark the end of a critical section
  *
  * This function should be called to mark the end of a critical section of code.
  * @note
  * NOTES:
  * 1) The use of this style of critical section is targetted at C based implementations.
  * 2) These critical sections can be nested.
  * 3) The interrupt enable state on entry to the first critical section (of a nested set, or single
  *    section) will be preserved on exit from the section.
  * 4) This implementation will currently only work on code running in privileged mode.
  */
void core_util_critical_section_exit(void);

/**
 * Determine if we are currently in a critical section
 *
 * @return true if in a critical section, false otherwise.
 */
bool core_util_in_critical_section(void);

/**@}*/

/**@}*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MBED_UTIL_CRITICAL_H__
