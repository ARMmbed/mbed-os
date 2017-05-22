/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

#define NVIC_USER_IRQ_OFFSET 16
#define NVIC_USER_IRQ_NUMBER 64
#define NVIC_NUM_VECTORS     (NVIC_USER_IRQ_OFFSET + NVIC_USER_IRQ_NUMBER)

#if defined(__CC_ARM)
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &Image$$ER_IRAMVEC$$ZI$$Base)
#elif defined(__ICCARM__)
#   pragma section = "IRAMVEC"
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) __section_begin("IRAMVEC"))
#elif defined(__GNUC__)
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &__start_vector_table__)
#endif

#if defined(__CC_ARM)
    extern uint32_t Load$$LR$$LR_IROM1$$Base[];
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)Load$$LR$$LR_IROM1$$Base)
#elif defined(__ICCARM__)
    #pragma section=".intvec"
    #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)__section_begin(".intvec"))
#elif defined(__GNUC__)
      extern uint32_t __vector_table;
      #define NVIC_FLASH_VECTOR_ADDRESS   ((uint32_t)&__vector_table)
#else
    #error "Flash vector address not set for this toolchain"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Set the ISR for IRQn
 *
 * Sets an Interrupt Service Routine vector for IRQn; if the feature is available, the vector table is relocated to SRAM
 * the first time this function is called
 * @param[in] IRQn   The Interrupt Request number for which a vector will be registered
 * @param[in] vector The ISR vector to register for IRQn
 */
void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);

/** Get the ISR registered for IRQn
 *
 * Reads the Interrupt Service Routine currently registered for IRQn
 * @param[in] IRQn   The Interrupt Request number the vector of which will be read
 * @return           Returns the ISR registered for IRQn
 */
uint32_t NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
