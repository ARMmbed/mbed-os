/*
 * Copyright (c) 2017-2019 Arm Limited. All rights reserved.
 * Copyright (c) 2019, Cypress Semiconductor Corporation. All rights reserved.
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

/* Flash layout with BL2:
 *
 * TBD
 *
 *
 * Flash layout if BL2 not defined:
 *
 * 0x1000_0000 Non-secure image primary (256 KB)
 * 0x1004_0000 Secure/Non-secure image secondary (256 KB)
 *  Note that only one image can be upgraded per boot
 * 0x1008_0000 Secure     image primary (256 KB)
 * 0x100c_0000 Secure Storage Area (20 KB)
 * 0x100c_5000 NV counters area (24 Bytes)
 * 0x100c_5018 Unused (almost 44 KB)
 * 0x100d_0000 Reserved (192 KB)
 *
 */

#define MAX(X, Y)                       (((X) > (Y)) ? (X) : (Y))

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* The size of S partition */
#define FLASH_S_PARTITION_SIZE          0x40000      /* 256 KB */
/* The size of NS partition */
#define FLASH_NS_PARTITION_SIZE         0x40000      /* 256 KB */

/*
 * Each FLASH_AREA_IMAGE contains NS and S partitions.
 * See Flash layout above.
 */
#define FLASH_PARTITION_SIZE            (FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE)
#define FLASH_MAX_PARTITION_SIZE        MAX(FLASH_S_PARTITION_SIZE, \
                                            FLASH_NS_PARTITION_SIZE)

/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x200)      /* 512 B */
/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00100000) /* 1 MB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x10000000U) /* same as FLASH0_BASE */

#ifdef BL2
/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY.
 * SCRATCH is used as a temporary storage during image swapping.
 */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#define FLASH_AREA_BL2_SIZE             (0x20000)  /* 128 kB */

#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BL2_OFFSET + \
                                         FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE               FLASH_PARTITION_SIZE

/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_0_OFFSET + \
                                         FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE               FLASH_PARTITION_SIZE

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_AREA_2_OFFSET + \
                                         FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE         (0x8000)   /* 32 KB */

/* Maximum number of status entries supported by the bootloader. */
#define BOOT_STATUS_MAX_ENTRIES         MAX(2, FLASH_PARTITION_SIZE / \
                                               FLASH_AREA_SCRATCH_SIZE)

/** Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS            (FLASH_PARTITION_SIZE / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)
#elif (MCUBOOT_IMAGE_NUMBER == 2)

/* Secure image primary slot */
#define FLASH_AREA_0_ID                 (1)
#define FLASH_AREA_0_OFFSET             (FLASH_AREA_BL2_OFFSET + \
                                         FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE               (FLASH_S_PARTITION_SIZE)

/* Non-secure image primary slot */
#define FLASH_AREA_1_ID                 (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET             (FLASH_AREA_0_OFFSET + \
                                         FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE               (FLASH_NS_PARTITION_SIZE)

/* Secure image secondary slot */
#define FLASH_AREA_2_ID                 (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET             (FLASH_AREA_1_OFFSET + \
                                         FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE               (FLASH_S_PARTITION_SIZE)

/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID                 (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET             (FLASH_AREA_2_OFFSET + \
                                         FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE               (FLASH_NS_PARTITION_SIZE)

/* Scratch area */
#define FLASH_AREA_SCRATCH_ID           (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET       (FLASH_AREA_3_OFFSET + \
                                         FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE         (0x8000)   /* 32 KB */

/* The maximum number of status entries supported by the bootloader. */
/* The maximum number of status entries must be at least 2. For more
 * information see the MCUBoot issue:
 * https://github.com/JuulLabs-OSS/mcuboot/issues/427.
 */
#define BOOT_STATUS_MAX_ENTRIES         MAX(2, FLASH_MAX_PARTITION_SIZE / \
                                               FLASH_AREA_SCRATCH_SIZE)
/* Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS            (FLASH_MAX_PARTITION_SIZE / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

#else /* BL2 */
/* SST area follows scratch area */
#define FLASH_AREA_SCRATCH_OFFSET       (SECURE_IMAGE_OFFSET + \
                                         FLASH_S_PARTITION_SIZE)
#define FLASH_AREA_SCRATCH_SIZE         (0)   /* None */
#endif /* BL2 */

#define FLASH_SST_AREA_OFFSET           (FLASH_AREA_SCRATCH_OFFSET + \
                                         FLASH_AREA_SCRATCH_SIZE)
#define FLASH_SST_AREA_SIZE             (0x5000)   /* 20 KB */

#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_SST_AREA_OFFSET + \
                                         FLASH_SST_AREA_SIZE)
#define FLASH_NV_COUNTERS_AREA_SIZE     (0x18)     /* 24 Bytes */

/* Offset and size definition in flash area, used by assemble.py */
#ifdef BL2
#define SECURE_IMAGE_OFFSET             0x0
#else
#define SECURE_IMAGE_OFFSET             (NON_SECURE_IMAGE_OFFSET + \
                                         FLASH_NS_PARTITION_SIZE + \
                                         FLASH_MAX_PARTITION_SIZE)
#endif
#define SECURE_IMAGE_MAX_SIZE           FLASH_S_PARTITION_SIZE

#ifdef BL2
#define NON_SECURE_IMAGE_OFFSET         (SECURE_IMAGE_OFFSET + \
                                         SECURE_IMAGE_MAX_SIZE)
#else
#define NON_SECURE_IMAGE_OFFSET         0x0
#endif
#define NON_SECURE_IMAGE_MAX_SIZE       FLASH_NS_PARTITION_SIZE

/* Flash device name used by BL2 and SST
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
#define SST_SECTORS_PER_BLOCK   0x8
/* Specifies the smallest flash programmable unit in bytes */
#define SST_FLASH_PROGRAM_UNIT  0x1
/* The maximum asset size to be stored in the SST area */
#define SST_MAX_ASSET_SIZE      2048
/* The maximum number of assets to be stored in the SST area */
#define SST_NUM_ASSETS          10

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    FLASH_NV_COUNTERS_AREA_SIZE
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_AREA_IMAGE_SECTOR_SIZE

/* Use Flash to store Code data */
#define S_ROM_ALIAS_BASE  (0x10000000)
#define NS_ROM_ALIAS_BASE (0x10000000)

/* Use SRAM to store RW data */
#define S_RAM_ALIAS_BASE  (0x08000000)
#define NS_RAM_ALIAS_BASE (0x08000000)

#endif /* __FLASH_LAYOUT_H__ */
