/*
 * Copyright (c) 2018-2020 Arm Limited. All rights reserved.
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

/* Flash layout on Musca-B1 with BL2(single image boot, boot from eFlash 0):
 *
 * 0x0A00_0000 BL2 - MCUBoot (128 KB)
 * 0x0A02_0000 Primary image area (832 KB):
 *    0x0A02_0000 Secure     image primary (320 KB)
 *    0x0A07_0000 Non-secure image primary (512 KB)
 * 0x0A0F_0000 Secondary image area (832 KB):
 *    0x0A0F_0000 Secure     image secondary (320 KB)
 *    0x0A14_0000 Non-secure image secondary (512 KB)
 * 0x0A1C_0000 Secure Storage Area (0.02 MB)
 * 0x0A1C_5000 Internal Trusted Storage Area (0.02 MB)
 * 0x0A1C_A000 NV counters area (4 KB)
 * 0x0A1C_B000 Unused
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are
 * marked with comment.
 */

/* Size of a Secure and of a Non-secure image */
#define FLASH_S_PARTITION_SIZE          (0x50000) /* S partition: 320 KB */
#define FLASH_NS_PARTITION_SIZE         (0x80000) /* NS partition: 512 KB */

/* Sector size of the flash hardware */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x1000)   /* 4 KB */
#define FLASH_TOTAL_SIZE                (0x200000) /* 2 MB */

/* Flash layout info for BL2 bootloader */
/* Same as MUSCA_B1_EFLASH0_S_BASE */
#define FLASH_BASE_ADDRESS              (0x1A000000)

/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#define FLASH_AREA_BL2_SIZE             (0x20000) /* 128 KB */

#define FLASH_AREA_IMAGE_0_OFFSET       (FLASH_AREA_BL2_OFFSET + \
                                         FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_IMAGE_0_SIZE         (FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE)

#define FLASH_AREA_IMAGE_1_OFFSET       (FLASH_AREA_IMAGE_0_OFFSET + \
                                         FLASH_AREA_IMAGE_0_SIZE)
#define FLASH_AREA_IMAGE_1_SIZE         (FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE)

/* Not used, only the Non-swapping firmware upgrade operation
 * is supported on Musca-B1
 */
#define FLASH_AREA_IMAGE_SCRATCH_OFFSET (FLASH_AREA_IMAGE_1_OFFSET + \
                                         FLASH_AREA_IMAGE_1_SIZE)
#define FLASH_AREA_IMAGE_SCRATCH_SIZE   (0)

/* Not used, only the Non-swapping firmware upgrade operation
 * is supported on Musca-B1. The maximum number of status entries
 * supported by the bootloader.
 */
#define BOOT_STATUS_MAX_ENTRIES         (0)

/* Maximum number of image sectors supported by the bootloader. */
#define BOOT_MAX_IMG_SECTORS            ((FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE) / \
                                         FLASH_AREA_IMAGE_SECTOR_SIZE)

/* NV Counters definitions */
#define FLASH_NV_COUNTERS_AREA_OFFSET   (FLASH_AREA_IMAGE_SCRATCH_OFFSET + \
                                         FLASH_AREA_IMAGE_SCRATCH_SIZE)
#define FLASH_NV_COUNTERS_AREA_SIZE     (0x14)     /* 20 Bytes */

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET             0x0
/* Value comes from FLASH_S_PARTITION_SIZE */
#define SECURE_IMAGE_MAX_SIZE           0x50000

#define NON_SECURE_IMAGE_OFFSET         0x50000
/* Comes from FLASH_NS_PARTITION_SIZE */
#define NON_SECURE_IMAGE_MAX_SIZE       0x80000

/* Concatenated binary size used by imgtool.py */
#define SIGN_BIN_SIZE                   (FLASH_S_PARTITION_SIZE + \
                                         FLASH_NS_PARTITION_SIZE)

/* NV Counters definitions */
#define TFM_NV_COUNTERS_AREA_ADDR    FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_AREA_SIZE    FLASH_NV_COUNTERS_AREA_SIZE
#define TFM_NV_COUNTERS_SECTOR_ADDR  FLASH_NV_COUNTERS_AREA_OFFSET
#define TFM_NV_COUNTERS_SECTOR_SIZE  FLASH_AREA_IMAGE_SECTOR_SIZE

/* Use eFlash 0 memory to store Code data */
#define S_ROM_ALIAS_BASE             (0x1A000000)
#define NS_ROM_ALIAS_BASE            (0x0A000000)
#define TOTAL_ROM_SIZE               FLASH_TOTAL_SIZE

/* Internal SRAM memory is used to store RW data */
#define S_RAM_ALIAS_BASE             (0x30000000)
#define NS_RAM_ALIAS_BASE            (0x20000000)
#define TOTAL_RAM_SIZE               (0x80000)     /* 512 KB */

/* QSPi flash area is used by the Flash API */
#define S_QSPI_ALIAS_BASE            (0x10000000)
#define NS_QSPI_ALIAS_BASE           (0x00000000)
#define QSPI_FLASH_TOTAL_SIZE        (0x00800000)  /* 8MB */

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the RAM, it is overlapping
 * with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE S_RAM_ALIAS_BASE
#define BOOT_TFM_SHARED_DATA_SIZE (0x400)

#endif /* __FLASH_LAYOUT_H__ */
