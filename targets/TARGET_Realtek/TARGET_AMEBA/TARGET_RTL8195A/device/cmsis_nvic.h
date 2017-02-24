/* mbed Microcontroller Library
 * CMSIS-style functionality to support dynamic vectors
 ******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 *
 ******************************************************************************/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

// CORE: 64 vectors = 64 bytes from 0x00 to 0x3F
// MCU Peripherals: 85 vectors = 340 bytes from 0x40 to ...
// Total: 128 vectors = 512 bytes (0x200) to be reserved in RAM
#define NVIC_NUM_VECTORS      128
#define NVIC_USER_IRQ_OFFSET  64

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

void NVIC_SetVector(IRQn_Type IRQn, uint32_t vector);
uint32_t NVIC_GetVector(IRQn_Type IRQn);

#ifdef __cplusplus
}
#endif

#endif
