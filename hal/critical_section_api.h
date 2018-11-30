/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#ifndef MBED_CRITICAL_SECTION_API_H
#define MBED_CRITICAL_SECTION_API_H

#include <stdbool.h>

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
 * This function will be called by core_util_critical_section_enter() each time
 * the application requests to enter a critical section. The purpose of the
 * critical section is to ensure mutual-exclusion synchronisation of the
 * processor by preventing any change in processor control, the default
 * behaviour requires storing the state of interrupts in the system before
 * disabling them.
 *
 * The critical section code supports nesting. When a thread has entered a
 * critical section it can make additional calls to
 * core_util_critical_section_enter() without deadlocking itself. The critical
 * section driver API tracks the number of nested calls to the critical section.
 * The critical section will only be exited when
 * core_util_critical_section_exit() has been called once for each time it
 * entered the critical section.
 *
 * On the first call to enter a critical section this function MUST store the
 * state of any interrupts or other application settings it will modify to
 * facilitate the critical section.
 *
 * Each successive call to enter the critical section MUST ignore storing or
 * modifying any application state.
 *
 * The default implementation of this function which will save the current state
 * of interrupts before disabling them. This implementation can be found in
 * mbed_critical_section_api.c. This behaviour is can be overridden on a per
 * platform basis by providing a different implementation within the correct
 * targets directory.
 */
void hal_critical_section_enter(void);


/** Mark the end of a critical section.
 *
 * The purpose of this function is to restore any state that was modified upon
 * entering the critical section, allowing other threads or interrupts to change
 * the processor control.
 *
 * This function will be called once by core_util_critical_section_exit() per
 * critical section on last call to exit. When called, the application MUST
 * restore the saved interrupt/application state that was saved when entering
 * the critical section.
 *
 * There is a default implementation of this function, it will restore the state
 * of interrupts that were previously saved when hal_critical_section_enter was
 * first called, this implementation can be found in
 * mbed_critical_section_api.c. This behaviour is overridable by providing a
 * different function implementation within the correct targets directory.
 */
void hal_critical_section_exit(void);


/** Determine if the application is currently running in a critical section
 *
 * The purpose of this function is to inform the caller whether or not the
 * application is running in a critical section. This is done by checking if
 * the current interrupt state has been saved in the underlying implementation,
 * this could also be done by checking the state of the interrupts at the time
 * of calling.
 *
 *  @return  True if running in a critical section, false if not.
 */
bool hal_in_critical_section(void);


/**@}*/

#ifdef __cplusplus
}
#endif

#endif // MBED_CRITICAL_SECTION_API_H

/** @}*/
