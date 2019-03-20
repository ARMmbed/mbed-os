/*
 * Copyright (c) 2018 Arm Limited. All rights reserved.
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
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* Flash layout on LPC55S69 without BL2:
 *
 * 0x0000_0000 Secure     image primary
 * 0x0002_8000 SST / ITS
 * 0x0003_0000 Non-secure
 */

/* This header file is included from linker scatter file as well, where only a
 * limited C constructs are allowed. Therefore it is not possible to include
 * here the platform_retarget.h to access flash related defines. To resolve this
 * some of the values are redefined here with different names, these are marked
 * with comment.
 */

/* Sector size of the flash hardware; same as FLASH0_SECTOR_SIZE */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (0x8000)     /* 32 kB */
/* Same as FLASH0_SIZE */
#define FLASH_TOTAL_SIZE                (0x00098000) /* 608 kB */

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              (0x10000000) /* same as FLASH0_BASE_S */

/* Offset and size definitions of the flash partitions that are handled by thes
 * bootloader. The image swapping is done between IMAGE_0 and IMAGE_1, SCRATCH
 * is used as a temporary storage during image swapping.
 */
#define FLASH_AREA_BL2_OFFSET           (0x0)
#ifdef BL2
#define FLASH_AREA_BL2_SIZE             (0x10000)  /* 64kB */
#else
#define FLASH_AREA_BL2_SIZE             (0x0)
#endif

/* Secure image */
#define FLASH_AREA_IMAGE_0_OFFSET       (FLASH_AREA_BL2_OFFSET + \
                                         FLASH_AREA_BL2_SIZE) // 0
#define FLASH_AREA_IMAGE_0_SIZE         (0x00028000 - FLASH_AREA_BL2_SIZE) // 0x00028000

#define FLASH_SST_AREA_OFFSET           (FLASH_AREA_IMAGE_0_OFFSET + \
                                         FLASH_AREA_IMAGE_0_SIZE)  // 0x00028000
#define FLASH_SST_AREA_SIZE             (0x00008000)   /* 32 KB */

/* Non-secure image */
#define FLASH_AREA_IMAGE_1_OFFSET       (FLASH_SST_AREA_OFFSET +\
                                         FLASH_SST_AREA_SIZE) // 0x00030000
#define FLASH_AREA_IMAGE_1_SIZE         (0x00068000)

#define FLASH_AREA_IMAGE_SCRATCH_OFFSET (FLASH_AREA_IMAGE_1_OFFSET +\
                                         FLASH_AREA_IMAGE_1_SIZE)
#define FLASH_AREA_IMAGE_SCRATCH_SIZE   (0)

/* Offset and size definition in flash area, used by assemble.py */
#define SECURE_IMAGE_OFFSET             0x0
#define SECURE_IMAGE_MAX_SIZE           0x00028000

#define NON_SECURE_IMAGE_OFFSET         0x00030000
#define NON_SECURE_IMAGE_MAX_SIZE       0x00068000

/* Flash device name used by BL2 and SST
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME Driver_FLASH0

/* Secure Storage (SST) Service definitions
 * Note: Further documentation of these definitions can be found in the
 * TF-M SST Integration Guide.
 */
/* In this target the CMSIS driver requires only the offset from the base
 * address instead of the full memory address.
 */
#define SST_FLASH_AREA_ADDR  FLASH_SST_AREA_OFFSET
#define SST_SECTOR_SIZE      FLASH_AREA_IMAGE_SECTOR_SIZE
/* The sectors must be in consecutive memory location */
#define SST_NBR_OF_SECTORS  (FLASH_SST_AREA_SIZE / SST_SECTOR_SIZE)
/* Specifies the smallest flash programmable unit in bytes */
#define SST_FLASH_PROGRAM_UNIT  (512)
/* The maximum asset size to be stored in the SST area */
#define SST_MAX_ASSET_SIZE      2048
/* The maximum number of assets to be stored in the SST area */
#define SST_NUM_ASSETS          10

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
