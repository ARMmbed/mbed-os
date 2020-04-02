/* mbed Microcontroller Library
 *
 * Copyright (C) 2019, Toshiba Electronic Device Solutions Corporation
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

#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#if defined(TARGET_TMPM46B)

#ifndef INITIAL_SP
#define INITIAL_SP                        (0x20080000UL)
#endif
#ifdef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#undef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#endif
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE 3072

#endif

#if defined(TARGET_TMPM4G9)

#ifndef INITIAL_SP
#define INITIAL_SP                        (0x20030000UL)
#endif

#endif

#endif  // MBED_MBED_RTX_H
