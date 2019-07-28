/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

// CORE:            16 vectors
// MCU Peripherals: 41 vectors
// Total:           57 vectors = 228 bytes to be reserved in RAM

// For MSP432 we do not copy the IRQ vectors to save some bytes in RAM.
// Remember to change the linker script if the below lines are uncommented!

//#define NVIC_NUM_VECTORS 57
//#define NVIC_RAM_VECTOR_ADDRESS 0x20000000 // Copy irq vectors to start of RAM

#endif
