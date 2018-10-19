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

#ifndef STDDRIVER_SECURE_H
#define STDDRIVER_SECURE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Secure SYS_ResetModule
 *
 * Guard access to secure module from non-secure domain before SYS_ResetModule.
 * Its synopsis is the same as SYS_ResetModule.
 */
__NONSECURE_ENTRY
void SYS_ResetModule_S(uint32_t u32ModuleIndex);

/* Secure CLK_SetModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_SetModuleClock.
 * Its synopsis is the same as CLK_SetModuleClock.
 */
__NONSECURE_ENTRY
void CLK_SetModuleClock_S(uint32_t u32ModuleIndex, uint32_t u32ClkSrc, uint32_t u32ClkDiv);

/* Secure CLK_EnableModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_EnableModuleClock.
 * Its synopsis is the same as CLK_EnableModuleClock.
 */
__NONSECURE_ENTRY
void CLK_EnableModuleClock_S(uint32_t u32ModuleIndex);

/* Secure CLK_DisableModuleClock
 *
 * Guard access to secure module from non-secure domain before CLK_DisableModuleClock.
 * Its synopsis is the same as CLK_DisableModuleClock.
 */
__NONSECURE_ENTRY
void CLK_DisableModuleClock_S(uint32_t u32ModuleIndex);

/* Secure SYS_LockReg
 *
 * Guard access to secure module from non-secure domain before SYS_LockReg.
 * Its synopsis is the same as SYS_LockReg.
 */
__NONSECURE_ENTRY
void SYS_LockReg_S(void);

/* Secure SYS_UnlockReg
 *
 * Guard access to secure module from non-secure domain before SYS_UnlockReg.
 * Its synopsis is the same as SYS_UnlockReg.
 */
__NONSECURE_ENTRY
void SYS_UnlockReg_S(void);

/* Secure CLK_Idle */
__NONSECURE_ENTRY
void CLK_Idle_S(void);

/* Secure CLK_PowerDown */
__NONSECURE_ENTRY
void CLK_PowerDown_S(void);

#ifdef __cplusplus
}
#endif

#endif
