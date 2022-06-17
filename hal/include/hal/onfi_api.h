/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#ifndef MBED_ONFI_API_H
#define MBED_ONFI_API_H

#include <stdint.h>
#include "device.h"

#if DEVICE_ONFI

typedef struct onfi_s onfi_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_onfi ONFI API
 * @{
 */

/** Initialize the ONFI NAND Flash peripheral and the onfi_t object
 *
 * @param obj The onfi object
 * @param d0 1st IO pin used for sending/receiving data during data phase of a transaction
 * @param d1 2nd IO pin used for sending/receiving data during data phase of a transaction
 * @param d2 3rd IO pin used for sending/receiving data during data phase of a transaction
 * @param d3 4th IO pin used for sending/receiving data during data phase of a transaction
 * @param d4 5th IO pin used for sending/receiving data during data phase of a transaction
 * @param d5 6th IO pin used for sending/receiving data during data phase of a transaction
 * @param d6 7th IO pin used for sending/receiving data during data phase of a transaction
 * @param d7 8th IO pin used for sending/receiving data during data phase of a transaction
 * @param add09 1st IO pin used for sending address during address phase
 * @param add11 2nd IO pin used for sending address during address phase
 * @param add12 3rd IO pin used for sending address during address phase
 * @param ba1 SEMC_BA1
 * @param rdy pin used for checking Ready/Busy status
 * @param csx chip select pin
 * @return 0 for success, -1 for error
 */
int32_t onfi_init(onfi_t *obj, PinName d0, PinName d1, PinName d2, PinName d3, PinName d4, PinName d5,
                  PinName d6, PinName d7, PinName add09, PinName add11, PinName add12, PinName ba1, PinName rdy, PinName csx);

/** Uninitialize the ONFI NAND Flash peripheral and the onfi_t object
 *
 * @return 0 for success, -1 for error
 */
int32_t onfi_free(onfi_t *obj);

/** Erase blocks starting at defined address
 *
 *  The state of an erased block is undefined until it has been programmed
 *
 *  @param obj The onfi object
 *  @param addr     Address of block to begin erasing
 *  @param size     Size to erase in bytes, must be a multiple of erase block size
 *  @return 0 for success, -1 for error
*/
int32_t onfi_erase_block(onfi_t *obj, uint32_t addr, uint32_t size);

/** Program pages starting at defined address
 *
 *  The blocks must have been erased prior to being programmed
 *
 *  @param obj The onfi object
 *  @param buffer   Buffer of data to write to
 *  @param addr     Address of block to begin writing to
 *  @param size     Size to write in bytes
 *  @return 0 for success, -1 for error
*/
int32_t onfi_write(onfi_t *obj, uint32_t addr, const void *buffer, uint32_t size);

/** Read data starting at defined address
 *
 *  @param obj The onfi object
 *  @param buffer   Buffer to read data into
 *  @param addr     Address of block to begin reading from
 *  @param size     Size to read in bytes
 *  @return 0 for success, -1 for error
*/
int32_t onfi_read(onfi_t *obj, uint32_t addr, uint8_t *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif

#endif
