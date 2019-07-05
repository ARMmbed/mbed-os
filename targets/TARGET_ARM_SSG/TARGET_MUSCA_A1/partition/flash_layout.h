/*
 * Copyright (c) 2018-2019 Arm Limited. All rights reserved.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* Flash layout on Musca with BL2:
 * 0x0020_0000 BL2 - MCUBoot(128 KB)
 * 0x0022_0000 Flash_area_image_0(1 MB)
 *    0x0022_0000 Secure     image primary
 *    0x002A_0000 Non-secure image primary
 * 0x0032_0000 Flash_area_image_1(1 MB)
 *    0x0032_0000 Secure     image secondary
 *    0x003A_0000 Non-secure image secondary
 * 0x0042_0000 Secure Storage Area(0.02 MB)
 * 0x0042_5000 NV counters area(16 Bytes)
 * 0x0042_5010 Unused
 */

/* Code SRAM layout on Musca (with BL2, which is mandatory) after the newest
 * image has been copied to SRAM:
 * 0x0000_0000 BL2 - MCUBoot(128 KB)
 * 0x0002_0000 Flash_area_newest_image(1 MB)
 *    0x0002_0000 Secure     image primary
 *    0x000A_0000 Non-secure image primary
 * 0x0012_00000 Unused
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* The size of a partition. This should be large enough to contain a S or NS
 * sw binary. Each FLASH_AREA_IMAGE contains two partitions. See Flash layout
 * above.
 */
#define FLASH_PARTITION_SIZE            (0x80000) /* 512KB */

/* Sector size of the flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)   /* 4KB */
#define FLASH_TOTAL_SIZE                (0x800000) /* 8MB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x10200000) /* same as FLASH0_BASE_S */

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#define FLASH_AREA_BL2_SIZE             (0x20000) /* 128KB */

#define FLASH_AREA_IMAGE_0_OFFSET       (0x20000)
#define FLASH_AREA_IMAGE_0_SIZE         (2 * FLASH_PARTITION_SIZE)

#define FLASH_AREA_IMAGE_1_OFFSET       (0x120000)
#define FLASH_AREA_IMAGE_1_SIZE         (2 * FLASH_PARTITION_SIZE)

/* Not used, only RAM loading is supported on Musca A1 */
#define FLASH_AREA_IMAGE_SCRATCH_OFFSET (0x220000)
#define FLASH_AREA_IMAGE_SCRATCH_SIZE   (0)

/*
 * Not used, only RAM loading is supported on Musca A1.
 * The maximum number of status entries supported by the bootloader.
 */
#define BOOT_STATUS_MAX_ENTRIES         (0)

/** Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS            ((2 * FLASH_PARTITION_SIZE) / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)

#define FLASH_SST_AREA_OFFSET           (FLASH_AREA_IMAGE_SCRATCH_OFFSET + \
                                         FLASH_AREA_IMAGE_SCRATCH_SIZE)
#define FLASH_SST_AREA_SIZE             (0x5000)   /* 20 KB */

#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_SST_AREA_OFFSET + \
                                         FLASH_SST_AREA_SIZE)
#define FLASH_NV_COUNTERS_AREA_SIZE     (0x10)     /* 16 Bytes */

/* Offset and size definition in flash area, used by assemble.py */
#define SECURE_IMAGE_OFFSET             0x0
#define SECURE_IMAGE_MAX_SIZE           0x80000

#define NON_SECURE_IMAGE_OFFSET         0x80000
#define NON_SECURE_IMAGE_MAX_SIZE       0x80000

#define S_QSPI_ALIAS_BASE               (0x10200000)
#define NS_QSPI_ALIAS_BASE              (0x00200000)

#define S_SRAM_ALIAS_BASE               (0x10000000)
#define NS_SRAM_ALIAS_BASE              (0x00000000)

/* Address that tells the bootloader where in SRAM to copy the image from flash
 * E.g. Lowest address =  S_SRAM_ALIAS_BASE
 *                        + FLASH_AREA_BL2_SIZE
 *                        = 0x10000000 + 0x20000 = 0x10020000
 * Please make sure the value is in the form of hex. here
 */
#define IMAGE_LOAD_ADDRESS            0x10020000

/* Flash device name used by BL2 and SST
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0

/* Secure Storage (SST) Service definitions */
/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define SST_FLASH_AREA_ADDR  FLASH_SST_AREA_OFFSET
#define SST_SECTOR_SIZE      FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define SST_NBR_OF_SECTORS  (FLASH_SST_AREA_SIZE / SST_SECTOR_SIZE)
/* Specifies the smallest flash programmable unit in bytes */
#define SST_FLASH_PROGRAM_UNIT  0x1

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    FLASH_NV_COUNTERS_AREA_SIZE
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_AREA_IMAGE_SECTOR_SIZE

/* FIXME: Use SRAM2 memory to store RW data */
#define S_RAM_ALIAS_BASE  (0x30000000)
#define NS_RAM_ALIAS_BASE (0x20000000)

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the RAM, it is overlapping
 * with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE S_RAM_ALIAS_BASE
#define BOOT_TFM_SHARED_DATA_SIZE 0x400

#endif /* __FLASH_LAYOUT_H__ */
