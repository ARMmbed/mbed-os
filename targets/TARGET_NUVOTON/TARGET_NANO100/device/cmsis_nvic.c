/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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
#include "cmsis_nvic.h"
#include "platform/mbed_error.h"

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
    // NOTE: On NANO130, relocating vector table is not supported due to just 16KB small SRAM.
    //       Add guard code to prevent from unsupported relocating.
    uint32_t vector_static = NVIC_GetVector(IRQn);
    if (vector_static != vector) {
        error("No support for relocating vector table");
    }
}

uint32_t NVIC_GetVector(IRQn_Type IRQn)
{
    uint32_t *vectors = (uint32_t*) NVIC_FLASH_VECTOR_ADDRESS;

    // Return the vector
    return vectors[IRQn + 16];
}
