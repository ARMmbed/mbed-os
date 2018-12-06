/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2016 u-blox
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

#if defined(TARGET_MCU_PSOC6_M0)

#ifdef TARGET_PSA
#ifndef INITIAL_SP
#define INITIAL_SP              (PSA_SECURE_RAM_START + PSA_SECURE_RAM_SIZE)
#endif // INITIAL_SP
#else
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08000000 + 0x00010000)  // Ram origin + length
#endif // INITIAL_SP

#endif // TARGET_PSA
#elif defined(TARGET_MCU_PSOC6_M4)

#ifdef TARGET_PSA
#ifndef INITIAL_SP
#define INITIAL_SP              (PSA_NON_SECURE_RAM_START + PSA_NON_SECURE_RAM_SIZE)
#endif // INITIAL_SP
#else
#ifndef INITIAL_SP
#define INITIAL_SP              (0x08010000 + 0x00037800)  // Ram origin + length
#endif // INITIAL_SP
#endif // TARGET_PSA

#else

#error "Unknown board"

#endif

#endif  // MBED_MBED_RTX_H
