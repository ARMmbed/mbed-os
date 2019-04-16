/*
 * Copyright (c) 2017-2019 Arm Limited
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
 *
 */

#include "../TFM/flash_layout.h"

/*
 * This file is included from the linker script,
 * limited set of C constructs can be used here
 */

#define NS_ROM_ALIAS_BASE        (0x0A000000)
#define BL2_HEADER_SIZE          (0x400)
#define NS_RAM_ALIAS_BASE        (0x20000000)
#define TOTAL_RAM_SIZE           (0x00080000) /* 512 KB */
#define TOTAL_ROM_SIZE           FLASH_TOTAL_SIZE

#define NS_TOTAL_RAM_SIZE        (TOTAL_RAM_SIZE / 2)
#define S_TOTAL_RAM_SIZE         (TOTAL_RAM_SIZE / 2)
#define NS_RAM_ALIAS             (NS_RAM_ALIAS_BASE + S_TOTAL_RAM_SIZE)

#define NS_CODE_MEMORY_SIZE      (TOTAL_ROM_SIZE / 2)
#define TOTAL_CODE_MEMORY_SIZE   (NS_CODE_MEMORY_SIZE - BL2_HEADER_SIZE)
#define NS_CODE_MEMORY_ALIAS_BASE  \
        (NS_ROM_ALIAS_BASE + BL2_HEADER_SIZE +\
         FLASH_AREA_BL2_SIZE + SECURE_IMAGE_MAX_SIZE)

/* Non-Secure region */
#define CODE_START               NS_CODE_MEMORY_ALIAS_BASE
#define CODE_MAX_SIZE            TOTAL_CODE_MEMORY_SIZE
#define DATA_RAM_START           NS_RAM_ALIAS
#define DATA_RAM_SIZE            NS_TOTAL_RAM_SIZE
