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

/* Flash layout on Musca with BL2 (single image boot, mandatory):
 *
 * 0x0020_0000 BL2 - MCUBoot (128 KB)
 * 0x0022_0000 Primary image area (1 MB):
 *    0x0022_0000 Secure     image primary
 *    0x002A_0000 Non-secure image primary
 * 0x0032_0000 Secondary image area (1 MB):
 *    0x0032_0000 Secure     image secondary
 *    0x003A_0000 Non-secure image secondary
 * 0x0042_0000 Secure Storage Area (8 KB)
 * 0x0042_2000 Internal Trusted Storage Area (8 KB)
 * 0x0042_4000 NV counters area (4 KB)
 * 0x0042_5000 Unused
 */

/* Code SRAM layout on Musca (with BL2, which is mandatory) after the newest
 * image has been copied to SRAM:
 * 0x0000_0000 BL2 - MCUBoot (128 KB)
 * 0x0002_0000 Flash_area_newest_image (1 MB)
 *    0x0002_0000 Secure     image primary
 *    0x000A_0000 Non-secure image primary
 * 0x0012_00000 Unused
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          (0x80000) /* S partition: 512 KB */
#define FLASH_NS_PARTITION_SIZE         (0x80000) /* NS partition: 512 KB */
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE :    \
                                         FLASH_NS_PARTITION_SIZE)

/* Sector size of the flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)   /* 4 KB */
#define FLASH_TOTAL_SIZE                (0x800000) /* 8 MB */

/* Flash layout info for BL2 bootloader */
/* Same as MPC_QSPI_RANGE_BASE_S */
#define FLASH_BASE_ADDRESS              (0x10200000)

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      (0x0)
#define FLASH_AREA_BL2_SIZE        (0x20000) /* 128KB */

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Not used, only the RAM loading firmware upgrade operation
 * is supported on Musca-A.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS       ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#elif (MCUBOOT_IMAGE_NUMBER == 2)
/* Secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Not used, only the RAM loading firmware upgrade operation
 * is supported on Musca-A.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS       (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Not used, only the RAM loading firmware upgrade operation
 * is supported on Musca-A. The maximum number of status entries
 * supported by the bootloader.
 */
#define BOOT_STATUS_MAX_ENTRIES         (0)

/* Secure Storage (SST) Service definitions */
#define FLASH_SST_AREA_OFFSET           (FLASH_AREA_SCRATCH_OFFSET + \
                                         FLASH_AREA_SCRATCH_SIZE)
#define FLASH_SST_AREA_SIZE             (0x2000)   /* 8 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET           (FLASH_SST_AREA_OFFSET + \
                                         FLASH_SST_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE             (0x2000)   /* 8 KB */

/* NV Counters definitions */
#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_ITS_AREA_OFFSET + \
                                         FLASH_ITS_AREA_SIZE)
#define FLASH_NV_COUNTERS_AREA_SIZE     (FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Image load address used by imgtool.py */
#define IMAGE_LOAD_ADDRESS              (S_SRAM_ALIAS_BASE + \
                                         FLASH_AREA_BL2_SIZE)

/* Define where executable memory for the images starts and ends */
#define IMAGE_EXECUTABLE_RAM_START      (IMAGE_LOAD_ADDRESS)
#define IMAGE_EXECUTABLE_RAM_SIZE       (FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE)


#define S_QSPI_ALIAS_BASE               (0x10200000)
#define NS_QSPI_ALIAS_BASE              (0x00200000)

#define S_SRAM_ALIAS_BASE               (0x10000000)
#define NS_SRAM_ALIAS_BASE              (0x00000000)

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0

/* Secure Storage (SST) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M SST Integration Guide.
 */
#define SST_FLASH_DEV_NAME Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define SST_FLASH_AREA_ADDR     FLASH_SST_AREA_OFFSET
/* Dedicated flash area for SST */
#define SST_FLASH_AREA_SIZE     FLASH_SST_AREA_SIZE
#define SST_SECTOR_SIZE         FLASH_AREA_IMAGE_SECTOR_SIZE
/* Number of SST_SECTOR_SIZE per block */
#define SST_SECTORS_PER_BLOCK   (0x1)
/* Specifies the smallest flash programmable unit in bytes */
#define SST_FLASH_PROGRAM_UNIT  (0x1)
/* The maximum asset size to be stored in the SST area */
#define SST_MAX_ASSET_SIZE      (512)
/* The maximum number of assets to be stored in the SST area */
#define SST_NUM_ASSETS          (10)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide. The ITS should be in the internal flash, but is
 * allocated in the external flash just for development platforms that don't
 * have internal flash available.
 */
#define ITS_FLASH_DEV_NAME Driver_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define ITS_FLASH_AREA_ADDR     FLASH_ITS_AREA_OFFSET
/* Dedicated flash area for ITS */
#define ITS_FLASH_AREA_SIZE     FLASH_ITS_AREA_SIZE
#define ITS_SECTOR_SIZE         FLASH_AREA_IMAGE_SECTOR_SIZE
/* Number of ITS_SECTOR_SIZE per block */
#define ITS_SECTORS_PER_BLOCK   (0x1)
/* Specifies the smallest flash programmable unit in bytes */
#define ITS_FLASH_PROGRAM_UNIT  (0x1)
/* The maximum asset size to be stored in the ITS area */
#define ITS_MAX_ASSET_SIZE      (512)
/* The maximum number of assets to be stored in the ITS area */
#define ITS_NUM_ASSETS          (10)

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    (0x18) /* 24 Bytes */
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_AREA_IMAGE_SECTOR_SIZE

/* Use QSPI Flash memory to store Code data */
#define S_ROM_ALIAS_BASE  (0x10200000)
#define NS_ROM_ALIAS_BASE (0x00200000)

/* FIXME: Use SRAM2 memory to store RW data */
#define S_RAM_ALIAS_BASE  (0x30000000)
#define NS_RAM_ALIAS_BASE (0x20000000)

#define TOTAL_ROM_SIZE (0x200000) /* 2 MB */
#define TOTAL_RAM_SIZE (0x20000) /* 128 KB */

#endif /* __FLASH_LAYOUT_H__ */
