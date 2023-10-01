// The 'features' section in 'target.json' is now used to create the device's hardware preprocessor switches.
// Check the 'features' section of the target description in 'targets.json' for more details.
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#include <stdint.h>

#define DEVICE_ID_LENGTH       24

#define BOARD_ENET_PHY_ADDR    (2)

/* CMSIS defines this, we use it as linker symbol, undefined it and let a linker symbol
 to be as vector table */
#undef __VECTOR_TABLE

// The MIMXRT linker script provides the __USED_FLASH_END symbol to
// indicate where application data ends in flash
extern uint8_t __USED_FLASH_END[0];
#define FLASHIAP_APP_ROM_END_ADDR ((uint32_t)__USED_FLASH_END)

#include "mimxrt_memory_info.h"
#include "objects.h"

#endif
