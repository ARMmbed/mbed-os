/*
 * Copyright (c) 2017-2018 Arm Limited
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

#include "..\TFM\flash_layout.h"

/*
 * This file is included from the linker script,
 * limited set of C constructs can be used here
 */

/* Internal data SRAM
 * Whole RAM is 128KB, but TFM used first 64KB. Then, RAM SIZE available to
 * mbed would be 64KB
 */
#define NS_RAM_ALIAS_BASE        (0x20000000)
#define TOTAL_RAM_SIZE           (0x00020000) /* 128KB */

#define NS_TOTAL_RAM_SIZE        (TOTAL_RAM_SIZE / 2) /* 64 kB */
#define S_TOTAL_RAM_SIZE         (TOTAL_RAM_SIZE / 2) /* 64 kB */
#define NS_RAM_ALIAS             (NS_RAM_ALIAS_BASE + S_TOTAL_RAM_SIZE)

/* Code SRAM area
 * Code SRAM is 2MB, but TFM used first 1MB
 */
#define TOTAL_ROM_SIZE           (0x00200000) /* 2 MB */
#define BL2_HEADER_SIZE          (0x400)

#define NS_CODE_SIZE             (TOTAL_ROM_SIZE / 2)
#define TOTAL_CODE_SRAM_SIZE     (NS_CODE_SIZE - BL2_HEADER_SIZE)
#define NS_CODE_SRAM_ALIAS_BASE  \
        (FLASH_AREA_BL2_SIZE + SECURE_IMAGE_MAX_SIZE + BL2_HEADER_SIZE)

/* Non-Secure region */
#define CODE_RAM_START           NS_CODE_SRAM_ALIAS_BASE
#define DATA_RAM_START           NS_RAM_ALIAS
