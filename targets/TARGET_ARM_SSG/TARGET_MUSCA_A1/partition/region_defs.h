/*
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#define TOTAL_ROM_SIZE (0x00200000) /* 2 MB */
#define TOTAL_RAM_SIZE (0x00020000) /* 128KB */

#define BL2_HEAP_SIZE           0x0001000
#define BL2_MSP_STACK_SIZE      0x0001000

#define S_HEAP_SIZE             0x0004000
#define S_MSP_STACK_SIZE_INIT   0x0000400
#define S_MSP_STACK_SIZE        0x0000800
#define S_PSP_STACK_SIZE        0x0000800

#define NS_HEAP_SIZE            0x0001000
#define NS_MSP_STACK_SIZE       0x0000400
#define NS_PSP_STACK_SIZE       0x0000C00

/*
 * MPC granularity is 128 KB on Musca. Alignment
 * of partitions is defined in accordance with this constraint.
 */
#define S_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_IMAGE_0_OFFSET)
#define S_IMAGE_SECONDARY_PARTITION_OFFSET (FLASH_AREA_IMAGE_1_OFFSET)

#define NS_IMAGE_PRIMARY_PARTITION_OFFSET (S_IMAGE_PRIMARY_PARTITION_OFFSET + \
                                           FLASH_PARTITION_SIZE)

/*
 * Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x1_FC00 Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image.
 * It is less than the FLASH_PARTITION_SIZE because we reserve space
 * for the image header and trailer introduced by the bootloader.
 */
#define BL2_HEADER_SIZE      (0x400)
#define BL2_TRAILER_SIZE     (0x400)

#define IMAGE_CODE_SIZE \
            (FLASH_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define CMSE_VENEER_REGION_SIZE     (0x000001C0)

/*
 * Since we enable/disable flash during s/ns code copy to code sram we cannot
 * access bl2 code from flash, hence we need to copy the bl2 code to code sram
 */
#define BL2_CODE_SRAM_ALIAS_BASE (S_SRAM_ALIAS_BASE)
#define BL2_CODE_SRAM_ALIAS(x) (BL2_CODE_SRAM_ALIAS_BASE + x)
#define BL2_CODE_SRAM_BASE  (BL2_CODE_SRAM_ALIAS(FLASH_AREA_BL2_OFFSET))

/* Use QSPI Flash memory to store Code data */
#define S_ROM_ALIAS_BASE  (0x10200000)
#define NS_ROM_ALIAS_BASE (0x00200000)

#define S_RAM_ALIAS_BASE  (0x30000000)
#define NS_RAM_ALIAS_BASE (0x20000000)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)  (S_SRAM_ALIAS_BASE + x)
#define NS_ROM_ALIAS(x) (NS_SRAM_ALIAS_BASE + x)

#define S_RAM_ALIAS(x)  (S_RAM_ALIAS_BASE + x)
#define NS_RAM_ALIAS(x) (NS_RAM_ALIAS_BASE + x)

/* Secure regions */
#define  S_IMAGE_PRIMARY_AREA_OFFSET \
             (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define S_CODE_START    (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))
#define S_CODE_SIZE     (IMAGE_CODE_SIZE - CMSE_VENEER_REGION_SIZE)
#define S_CODE_LIMIT    (S_CODE_START + S_CODE_SIZE - 1)

#define S_DATA_START    (S_RAM_ALIAS(0x0))

#if defined(TEST_FRAMEWORK_S) || defined(TEST_FRAMEWORK_NS)
/* Increase secure DATA area to run the regression tests on Musca A1 */
#define S_DATA_SIZE     ((TOTAL_RAM_SIZE / 4) * 3)
#else
#define S_DATA_SIZE     ((TOTAL_RAM_SIZE / 2) + 0x1000)
#endif

#define S_DATA_LIMIT    (S_DATA_START + S_DATA_SIZE - 1)

/* CMSE Veneers region */
#define CMSE_VENEER_REGION_START  (S_CODE_LIMIT + 1)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET \
                        (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE    (IMAGE_CODE_SIZE - FLASH_AREA_BL2_SIZE)
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START   (NS_RAM_ALIAS(S_DATA_SIZE))
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE - S_DATA_SIZE)

#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START \
            (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))

#define NS_PARTITION_SIZE (FLASH_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START \
            (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))

#define SECONDARY_PARTITION_SIZE (FLASH_AREA_IMAGE_1_SIZE)

/* Code SRAM area */
#define TOTAL_CODE_SRAM_SIZE     (TOTAL_ROM_SIZE)
#define S_CODE_SRAM_ALIAS_BASE   (0x10000000)
#define NS_CODE_SRAM_ALIAS_BASE  (0x00000000)

#define BL2_CODE_SRAM_EXEC_BASE  (S_CODE_SRAM_ALIAS_BASE)
#define S_CODE_SRAM_EXEC_BASE    (S_CODE_SRAM_ALIAS_BASE)
#define S_CODE_SRAM_EXEC_LIMIT   (S_CODE_SRAM_EXEC_BASE + \
                                 (TOTAL_CODE_SRAM_SIZE / 2) - 1)
#define NS_CODE_SRAM_EXEC_BASE   (NS_CODE_SRAM_ALIAS_BASE + \
                                 (TOTAL_CODE_SRAM_SIZE / 2))
#define NS_CODE_SRAM_EXEC_LIMIT  (NS_CODE_SRAM_EXEC_BASE + \
                                 (TOTAL_CODE_SRAM_SIZE / 2) - 1)

/* Bootloader regions */
#define BL2_CODE_START    (S_QSPI_ALIAS_BASE)
#define BL2_CODE_SIZE     (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_RAM_ALIAS(0x0))
#define BL2_DATA_SIZE     (TOTAL_RAM_SIZE)
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)

#endif /* __REGION_DEFS_H__ */
