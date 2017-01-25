/** \addtogroup hal */
/** @{*/

/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_FLASH_API_H
#define MBED_FLASH_API_H

#include "device.h"
#include <stdint.h>

#if DEVICE_FLASH

typedef struct flash_s flash_t;

#ifdef __cplusplus
extern "C" {
#endif

// maps to FlashAlgo Init
int32_t flash_init(flash_t *obj);

// maps to FlashAlgo UnInit
int32_t flash_free(flash_t *obj);

// maps to FlashAlgo EraseSector
int32_t flash_erase_sector(flash_t *obj, uint32_t address);

// maps FlashAlgo to ProgramPage
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
