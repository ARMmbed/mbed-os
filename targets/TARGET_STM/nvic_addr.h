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
#ifndef NVIC_ADDR_H
#define NVIC_ADDR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ICCARM__)
    #pragma section=".intvec"
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)__section_begin(".intvec"))
#elif defined(__CC_ARM)
    extern uint32_t Load$$LR$$LR_IROM1$$Base[];
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)Load$$LR$$LR_IROM1$$Base)
#elif defined(__GNUC__)
    extern uint32_t g_pfnVectors[];
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)g_pfnVectors)
#else
    #error "Flash vector address not set for this toolchain"
#endif

#ifdef __cplusplus
}
#endif

#endif
