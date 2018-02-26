/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef TABLE_CRC_H
#define TABLE_CRC_H

#include <stdint.h>

namespace mbed {
/** \addtogroup drivers */
/** @{*/

#define MBED_CRC_TABLE_SIZE     256

extern const uint8_t Table_CRC_7Bit_SD[MBED_CRC_TABLE_SIZE];
extern const uint8_t Table_CRC_8bit_CCITT[MBED_CRC_TABLE_SIZE];
extern const uint16_t Table_CRC_16bit_CCITT[MBED_CRC_TABLE_SIZE];
extern const uint16_t Table_CRC_16bit_IBM[MBED_CRC_TABLE_SIZE];
extern const uint32_t Table_CRC_32bit_ANSI[MBED_CRC_TABLE_SIZE];

/** @}*/
} // namespace mbed

#endif
