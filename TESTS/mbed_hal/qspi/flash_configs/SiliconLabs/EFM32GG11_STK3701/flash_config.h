/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
#ifndef MBED_QSPI_FLASH_CONFIG_H
#define MBED_QSPI_FLASH_CONFIG_H

#include "../../MX25RXX35F_config.h"

#ifdef QSPI_SECTOR_COUNT
#undef QSPI_SECTOR_COUNT
#define QSPI_SECTOR_COUNT                       1024 // for MX25R3235F
#endif

#define QSPI_MIN_FREQUENCY  2000000

#endif // MBED_QSPI_FLASH_CONFIG_H
