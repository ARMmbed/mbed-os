/*
 * Copyright (c) 2009-2011 ARM Limited. All rights reserved.
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

#define NVIC_NUM_VECTORS        (16 + EXT_IRQ_COUNT)     // CORE + MCU Peripherals

/* For GCC, use dynamic vector table placement since otherwise we run into an alignment conflict */
#if (defined (__GNUC__) && (!defined(__CC_ARM) && (!defined(__ARMCC_VERSION))))
extern uint32_t __start_vector_table__;       // Dynamic vector positioning in GCC
#define NVIC_RAM_VECTOR_ADDRESS (&__start_vector_table__)
#else
#define NVIC_RAM_VECTOR_ADDRESS 0x20000000    // Vectors positioned at start of RAM
#endif

#endif /* MBED_CMSIS_NVIC_H */
