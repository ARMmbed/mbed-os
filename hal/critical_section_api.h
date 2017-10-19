/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#ifndef MBED_CRITICAL_SECTION_API_H
#define MBED_CRITICAL_SECTION_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_critical Critical Section HAL functions
 * @{
 */

/**
  * Mark the start of a critical section
  *
  * This function is called directly by core_util_critical_section_enter on
  * first entrance to a critical section.
  *
  * The default behavior of this function is to save the current state of
  * interrupts before disabling them.
  *
  * The function is only called once per critical section by
  * core_util_critical_section_enter. When implementing this function for a
  * platform you must store any state that you intend to alter within this
  * function so it can be restored when exiting the critical section.
  *
  */
void hal_critical_section_enter(void);

/** Mark the end of a critical section
  *
  * This function is called directly by core_util_critical_section_exit on the
  * final exit from a critical section.
  *
  * The default behavior of this function is to restore the state of interrupts
  * as they were prior to entering this critical section.
  *
  * This function is only called once per critical section. When implemented
  * for a specific platform it must restore any state that was saved upon
  * entering the current critical section.
  *
  */
void hal_critical_section_exit(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // MBED_CRITICAL_SECTION_API_H

/** @}*/
