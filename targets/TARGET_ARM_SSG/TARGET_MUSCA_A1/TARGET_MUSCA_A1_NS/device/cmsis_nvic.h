/* mbed Microcontroller Library
 * Copyright (c) 2019 Arm Limited
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

/*
 * This file is included from the linker script,
 * limited set of C constructs can be used here
 */

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "../../partition/region_defs.h"

#define NVIC_NUM_VECTORS        (16 + 76)
/** Location of vectors to move in RAM */
#define NVIC_RAM_VECTOR_ADDRESS NS_DATA_START
#define NVIC_RAM_VECTOR_SIZE    (NVIC_NUM_VECTORS * 4)
#define NVIC_RAM_VECTOR_LIMIT   (NVIC_RAM_VECTOR_ADDRESS + NVIC_RAM_VECTOR_SIZE)

#endif
