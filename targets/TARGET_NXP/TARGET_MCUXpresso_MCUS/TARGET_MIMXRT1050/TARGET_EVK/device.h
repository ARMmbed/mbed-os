// The 'features' section in 'target.json' is now used to create the device's hardware preprocessor switches.
// Check the 'features' section of the target description in 'targets.json' for more details.
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#define DEVICE_ID_LENGTH       24

#ifdef HYPERFLASH_BOOT
/* 64MB HyperFlash, 4MB reserved for mbed-os */
#define BOARD_FLASH_SIZE             (0x4000000U)
#define BOARD_FLASH_START_ADDR       (0x60000000U)
#define BOARD_FLASHIAP_SIZE          (0x3C00000U)
#define BOARD_FLASHIAP_START_ADDR    (0x60400000U)
#define BOARD_FLASH_PAGE_SIZE        (512)
#define BOARD_FLASH_SECTOR_SIZE      (262144)
#else
/* 8MB QSPI Flash, 64KB reserved for mbed_bootloader */
#define BOARD_FLASH_SIZE             (0x800000U)
#define BOARD_FLASH_START_ADDR       (0x60000000U)
#define BOARD_FLASHIAP_SIZE          (0x7F0000U)
#define BOARD_FLASHIAP_START_ADDR    (0x60010000U)
#define BOARD_FLASH_PAGE_SIZE        (256)
#define BOARD_FLASH_SECTOR_SIZE      (4096)
#endif

#define BOARD_ENET_PHY_ADDR    (2)

#include "objects.h"

#endif
