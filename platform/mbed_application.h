
/** \addtogroup platform */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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
#ifndef MBED_APPLICATION_H
#define MBED_APPLICATION_H

#include<stdint.h>

#if defined(__CORTEX_M3) || defined(__CORTEX_M4) || defined(__CORTEX_M7)
#define MBED_APPLICATION_SUPPORT 1
#else
#define MBED_APPLICATION_SUPPORT 0
#endif

#if MBED_APPLICATION_SUPPORT
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Start the application at the given address. This function does
 *  not return. It is the applications responsibility for flushing to
 *  or powering down external components such as filesystems or
 *  socket connections before calling this function. For Cortex-M
 *  devices this function powers down generic system components such as
 *  the NVIC and set the vector table to that of the new image followed
 *  by jumping to the reset handler of the new image.
 *
 *  @param address    Starting address of next application to run
 */
void mbed_start_application(uintptr_t address);

#ifdef __cplusplus
}
#endif
#endif

#endif

/** @}*/
