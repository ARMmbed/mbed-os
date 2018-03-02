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

#ifndef M2351_STDDRIVER_SUP_H
#define M2351_STDDRIVER_SUP_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Secure SYS_ResetModule
 *
 * Guard access to secure module from non-secure domain before SYS_ResetModule.
 * Its synopsis is the same as SYS_ResetModule.
 */
void SYS_ResetModule_S(uint32_t u32ModuleIndex);

/* Secure CLK_SetModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_SetModuleClock.
 * Its synopsis is the same as CLK_SetModuleClock.
 */
void CLK_SetModuleClock_S(uint32_t u32ModuleIndex, uint32_t u32ClkSrc, uint32_t u32ClkDiv);

/* Secure CLK_EnableModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_EnableModuleClock.
 * Its synopsis is the same as CLK_EnableModuleClock.
 */
void CLK_EnableModuleClock_S(uint32_t u32ModuleIndex);

/* Secure CLK_DisableModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_DisableModuleClock.
 * Its synopsis is the same as CLK_DisableModuleClock.
 */
void CLK_DisableModuleClock_S(uint32_t u32ModuleIndex);

#ifdef __cplusplus
}
#endif

#endif
