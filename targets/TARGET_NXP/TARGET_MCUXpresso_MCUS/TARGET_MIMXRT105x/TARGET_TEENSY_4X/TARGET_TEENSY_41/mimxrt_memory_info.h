/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
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

#ifndef MBED_OS_MIMXRT_MEMORY_INFO_H
#define MBED_OS_MIMXRT_MEMORY_INFO_H

/* 8MB QSPI Flash */
#define BOARD_FLASH_SIZE             (0x7C0000) // 7936k
#define BOARD_FLASH_START_ADDR       (0x60000000)
#define BOARD_FLASH_PAGE_SIZE        (256)
#define BOARD_FLASH_SECTOR_SIZE      (4096)

// Unless the user overrides it, define the app to use the entire flash space.
#ifndef MBED_APP_START
#define MBED_APP_START BOARD_FLASH_START_ADDR
#endif

#ifndef MBED_APP_SIZE
#define MBED_APP_SIZE BOARD_FLASH_SIZE
#endif

#define MIMXRT105X_BOARD_HAS_EXTERNAL_RAM 0

#endif //MBED_OS_MIMXRT_MEMORY_INFO_H
