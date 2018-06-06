/* mbed Microcontroller Library
 * Copyright (c) 2015-2018 ARM Limited
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

/*
 * CMSIS-style functionality to support dynamic vectors
 *
 * This file is included in ARM and GCC_ARM linker scripts.
 *
 * WARNING: IAR does not include this file and re-define these values in
 * MPS2.icf file. Please make sure that the two files share the same values.
 */

#include "memory_zones.h"

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

/*
 * 16 vectors for initial stack pointer and internal exceptions (defined in
 * Armv7-M ARM).
 * 57 vectors for external interrupts (defined in CM3DS Eval RTL and Testbench
 * User Guide).
 */
#define NVIC_NUM_VECTORS        (16 + 57)

/*
 * Location of vectors in RAM, they are copied at boot from adress 0x00000000 to
 * that address.
 */
#define NVIC_RAM_VECTOR_ADDRESS ZBT_SSRAM23_START

/*
 * Size of the whole vector table in bytes. Each vector is on 32 bits.
 */
#define NVIC_VECTORS_SIZE       (NVIC_NUM_VECTORS * 4)

#endif /* MBED_CMSIS_NVIC_H */
