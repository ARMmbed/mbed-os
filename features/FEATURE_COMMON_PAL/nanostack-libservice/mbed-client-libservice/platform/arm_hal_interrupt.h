/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ARM_HAL_API_H_
#define ARM_HAL_API_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

/**
 * \brief This function disable global interrupts.
 */
extern void platform_enter_critical(void);
/**
 * \brief This function enable global interrupts.
 */
extern void platform_exit_critical(void);

/**
 * \brief This function increments the disable IRQ counter.
 * The function must be called inside interrupt routines
 * before any routine that uses platform_enter_critical()
 * is called.
 *
 * This routine may not be need to do anything on some platforms,
 * but requiring the call to be made before using
 * platform_enter_critical() in an interrupt can simplify some
 * implementations of platform_enter/exit_critical().
 */
extern void platform_interrupts_disabled(void);

/**
 * \brief This function decrements the disable IRQ counter.
 */
extern void platform_interrupts_enabling(void);

#ifdef __cplusplus
}
#endif

#endif /* ARM_HAL_API_H_ */
