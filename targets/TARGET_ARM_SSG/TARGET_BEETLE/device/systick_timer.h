/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#ifndef _SYSTICK_TIMER_H
#define _SYSTICK_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SysTick_Initialize(): Initializes the SysTick timer
 */
void SysTick_Initialize(void);

/*
 * SysTick_Disable(): Disables the SysTick timer
 */
void SysTick_Disable(void);

/*
 * SysTick_Read(): Read SysTick Value
 * @return: the SysTick VALUE
 */
uint32_t SysTick_Read(void);

/*
 * SysTick_Overflow(): Read SysTick Overflow Value
 * @return: the SysTick Overflow VALUE
 */
uint32_t SysTick_Overflow(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTICK_TIMER_H */
