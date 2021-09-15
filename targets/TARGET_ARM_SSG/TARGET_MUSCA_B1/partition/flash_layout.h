/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
 * Copyright (c) 2020 Cypress Semiconductor Corporation. All rights reserved.
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

#ifndef FORWARD_PROT_MSG
/* Flash layouts if FORWARD_PROT_MSG is OFF
 *
 *
 * Flash layout on Musca-B1 with BL2 (multiple image boot, boot from eFlash 0):
 *
 * 0x0A00_0000 BL2 - MCUBoot (128 KB)
 * 0x0A02_0000 Secure image     primary slot (384 KB)
 * 0x0A08_0000 Non-secure image primary slot (512 KB)
 * 0x0A10_0000 Secure image     secondary slot (384 KB)
 * 0x0A16_0000 Non-secure image secondary slot (512 KB)
 * 0x0A1E_0000 Scratch area (64 KB)
 * 0x0A1F_0000 Internal Trusted Storage Area (32 KB)
 * 0x0A1F_8000 NV counters area (16 KB)
 * 0x0A1F_C000 Unused (32 KB)
 *
 * Flash layout on Musca-B1 with BL2 (single image boot):
 *
 * 0x0A00_0000 BL2 - MCUBoot (128 KB)
 * 0x0A02_0000 Primary image area (896 KB):
 *    0x0A02_0000 Secure     image primary (384 KB)
 *    0x0A08_0000 Non-secure image primary (512 KB)
 * 0x0A10_0000 Secondary image area (896 KB):
 *    0x0A10_0000 Secure     image secondary (384 KB)
 *    0x0A16_0000 Non-secure image secondary (512 KB)
 * 0x0A1E_0000 Scratch area (64 KB)
 * 0x0A1F_0000 Internal Trusted Storage Area (32 KB)
 * 0x0A1F_8000 NV counters area (16 KB)
 * 0x0A1F_C000 Unused (32 KB)
 *
 * Note: As eFlash is written at runtime, the eFlash driver code is placed
 * in code SRAM to avoid any interference.
 *
 * Flash layout on Musca-B1 without BL2:
 * 0x0A00_0000 Secure     image
 * 0x0A08_0000 Non-secure image
 *
 * QSPI Flash layout
 * 0x0000_0000 Protected Storage Area (20 KB)
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          (0x60000) /* S partition: 384 KB */
#define FLASH_NS_PARTITION_SIZE         (0x80000) /* NS partition: 512 KB */
#define FLASH_MAX_PARTITION_SIZE        ((FLASH_S_PARTITION_SIZE >   \
                                          FLASH_NS_PARTITION_SIZE) ? \
                                         FLASH_S_PARTITION_SIZE :    \
                                         FLASH_NS_PARTITION_SIZE)

/* Sector size of the embedded flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x4000)   /* 16 KB */
#define FLASH_TOTAL_SIZE                (0x200000) /* 2 MB */

/* Sector size of the QSPI flash hardware */
#define QSPI_FLASH_AREA_IMAGE_SECTOR_SIZE (0x1000)   /* 4 KB */
#define QSPI_FLASH_TOTAL_SIZE             (0x800000) /* 8 MB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x1A000000)

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      (0x0)
#define FLASH_AREA_BL2_SIZE        (0x20000) /* 128 KB */

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
/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (4 * FLASH_AREA_IMAGE_SECTOR_SIZE)
/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    ((FLASH_S_PARTITION_SIZE + \
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
/* Scratch area */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (4 * FLASH_AREA_IMAGE_SECTOR_SIZE)
/* The maximum number of status entries supported by the bootloader. */
#define MCUBOOT_STATUS_MAX_ENTRIES (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Internal Trusted Storage (ITS) Service definitions (32 KB) */
#define FLASH_ITS_AREA_OFFSET           (FLASH_AREA_SCRATCH_OFFSET + \
                                         FLASH_AREA_SCRATCH_SIZE)
#define FLASH_ITS_AREA_SIZE             (2 * FLASH_AREA_IMAGE_SECTOR_SIZE)

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

/* Protected Storage (PS) Service definitions size is 20 KB. */
/* Same as MUSCA_B1_QSPI_FLASH_S_BASE */
#define QSPI_FLASH_BASE_ADDRESS         (0x10000000)
#define FLASH_PS_AREA_OFFSET            (0x0)
#define FLASH_PS_AREA_SIZE              (5 * QSPI_FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Flash device name used by BL2
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_EFLASH0

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER Driver_QSPI_FLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR    FLASH_PS_AREA_OFFSET
/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE    FLASH_PS_AREA_SIZE
#define PS_RAM_FS_SIZE                TFM_HAL_PS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT       (0x1)

/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M ITS Integration Guide.
 */
#define TFM_HAL_ITS_FLASH_DRIVER Driver_EFLASH0

/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR    FLASH_ITS_AREA_OFFSET
/* Size of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_SIZE    FLASH_ITS_AREA_SIZE
#define ITS_RAM_FS_SIZE                TFM_HAL_ITS_FLASH_AREA_SIZE
/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK  (1)
/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT       (0x4)

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    (0x18) /* 24 Bytes */
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_NV_COUNTERS_AREA_SIZE

/* Use eFlash 0 memory to store Code data */
#define S_ROM_ALIAS_BASE  (0x1A000000)
#define NS_ROM_ALIAS_BASE (0x0A000000)

/* FIXME: Use SRAM2 memory to store RW data */
#define S_RAM_ALIAS_BASE  (0x30000000)
#define NS_RAM_ALIAS_BASE (0x20000000)

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
#define TOTAL_RAM_SIZE (0x80000)     /* 512 KB */

#else /* FORWARD_PROT_MSG */

/* Flash layout information if FORWARD_PROT_MSG is ON.
 * For information you can check Musca-B1 Secure Enclave's flash_layout.h
 */

#define FLASH_S_PARTITION_SIZE          (0x30000) /* S partition: 192 KB */
#define FLASH_NS_PARTITION_SIZE         (0x50000) /* NS partition: 320 KB */

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             (0x0)
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Image placed in eFlash 1 */
#define FLASH_BASE_ADDRESS              (0x1A200000)

#if (MCUBOOT_IMAGE_NUMBER != 1)
#error "If FORWARD_PROT_MSG is ON MCUBOOT_IMAGE_NUMBER must be 1"
#endif

/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (0x60000) /* Address comes from SE */
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (0x160000) /* Address comes from SE */
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)

/* Image placed in eFlash 1 */
#define S_ROM_ALIAS_BASE    (0x1A200000)
#define NS_ROM_ALIAS_BASE   (0x0A200000)

#define S_RAM_ALIAS_BASE    (0x30000000)
#define NS_RAM_ALIAS_BASE   (0x20000000)

#define TOTAL_RAM_SIZE      (0x80000)     /* 512 KB */

/* Macros needed for BL2 build with dummy values.
 * This BL2 instance is not used, but the BL2 macro needs to be set, and this
 * macro and BL2 build is entangled. If this is fixed the following macros can
 * be deleted.
 */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    0

#define MCUBOOT_STATUS_MAX_ENTRIES      0
#define MCUBOOT_MAX_IMG_SECTORS         32

#define FLASH_AREA_SCRATCH_ID           0
#define FLASH_AREA_SCRATCH_OFFSET       0
#define FLASH_AREA_SCRATCH_SIZE         0

#define FLASH_DEV_NAME                  Driver_EFLASH0
#define FLASH_AREA_BL2_OFFSET           0
#define FLASH_AREA_BL2_SIZE             0x20000

#define TFM_NV_COUNTERS_AREA_ADDR       0
#define TFM_NV_COUNTERS_AREA_SIZE       8
#define TFM_NV_COUNTERS_SECTOR_ADDR     0
#define TFM_NV_COUNTERS_SECTOR_SIZE     8

#endif /* FORWARD_PROT_MSG */

#endif /* __FLASH_LAYOUT_H__ */
