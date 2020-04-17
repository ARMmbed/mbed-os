/*
 * Copyright (c) 2017-2019 ARM Limited. All rights reserved.
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

#define TOTAL_ROM_SIZE FLASH_TOTAL_SIZE
/* 2KB of RAM (at the end of the SRAM) are reserved for system use. Using
 * this memory region for other purposes will lead to unexpected behavior.
 * 94KB of RAM (just before the memory reserved for system use) are
 * allocated and protected by Cypress Bootloader */
/* FixMe: confirm exact available amount of RAM based on the actual
   system allocation */
#define TOTAL_RAM_SIZE (0x000E8000) /* CY_SRAM_SIZE - 96KB */

#define BL2_HEAP_SIZE           0x0001000
#define BL2_MSP_STACK_SIZE      0x0001000

#define S_HEAP_SIZE             0x0001000
#define S_MSP_STACK_SIZE_INIT   0x0000400
#define S_MSP_STACK_SIZE        0x0000800
#define S_PSP_STACK_SIZE        0x0000800

#define NS_HEAP_SIZE            0x0001000
#define NS_MSP_STACK_SIZE       0x0000400
#define NS_PSP_STACK_SIZE       0x0000C00

/* Relocation of vectors to RAM support */
/* #define RAM_VECTORS_SUPPORT */

/*
 * This size of buffer is big enough to store an attestation
 * token produced by initial attestation service
 */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   0x250

/*
 * MPC granularity is 128 KB on AN519 MPS2 FPGA image. Alignment
 * of partitions is defined in accordance with this constraint.
 */

#ifdef BL2
#error "BL2 configuration is not supported"
#else
#define S_IMAGE_PRIMARY_PARTITION_OFFSET  SECURE_IMAGE_OFFSET
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET NON_SECURE_IMAGE_OFFSET
#endif /* BL2 */

/* TFM PSoC6 CY8CKIT_064 RAM layout:
 *
 * 0x0800_0000 - 0x0802_FFFF Secure (192KB)
 *    0x0800_0000 - 0x0800_FFFF Secure unprivileged data (S_UNPRIV_DATA_SIZE, 64KB)
 *    0x0801_0000 - 0x0802_EFFF Secure priviliged data (S_PRIV_DATA_SIZE, 124KB)
 *    0x0802_F000 - 0x0802_FFFF Secure priv code executable from RAM (S_RAM_CODE_SIZE, 4KB)
 *
 * 0x0803_0000 - 0x080E_7FFF Non-secure (736KB)
 *    0x0803_0000 - 0x080E_6FFF Non-secure OS/App (732KB)
 *    0x080E_7000 - 0x080E_7FFF Shared memory (NS_DATA_SHARED_SIZE, 4KB)
 * 0x080E_8000 - 0x080F_FFFF System reserved memory (96KB)
 * 0x0810_0000 End of RAM
 */

/*
 * Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x1_FC00 Trailer
 */
/* Image code size is the space available for the software binary image.
 * It is less than the FLASH_S_PARTITION_SIZE and FLASH_NS_PARTITION_SIZE
 * because we reserve space for the image header and trailer introduced by the
 * bootloader.
 */
#ifdef BL2
#error "BL2 configuration is not supported"
#else
/* Even though TFM BL2 is excluded from the build,
 * CY BL built externally is used and it needs offsets for header and trailer
 * to be taken in account.
 * */
#define BL2_HEADER_SIZE      (0x400)
#define BL2_TRAILER_SIZE     (0x400)

#endif /* BL2 */

#define IMAGE_S_CODE_SIZE \
            (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#define IMAGE_NS_CODE_SIZE \
            (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)  (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x) (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)  (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x) (NS_RAM_ALIAS_BASE + (x))

/* Secure regions */
#define S_IMAGE_PRIMARY_AREA_OFFSET \
             (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define S_CODE_START    (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))
#define S_CODE_SIZE     IMAGE_S_CODE_SIZE
#define S_CODE_LIMIT    (S_CODE_START + S_CODE_SIZE - 1)

#define S_DATA_START    (S_RAM_ALIAS(0))
#define S_UNPRIV_DATA_SIZE  0x10000
#define S_PRIV_DATA_SIZE    0x1F000
/* Reserve 4KB for RAM-based executable code */
#define S_RAM_CODE_SIZE     0x1000

/* Secure data area */
#define S_DATA_SIZE  (S_UNPRIV_DATA_SIZE + S_PRIV_DATA_SIZE + S_RAM_CODE_SIZE)
#define S_DATA_LIMIT (S_DATA_START + S_DATA_SIZE - 1)

/* We need the privileged data area to be aligned so that an SMPU
 * region can cover it.
 */
/* TODO It would be nice to figure this out automatically.
 * In theory, in the linker script, we could determine the amount
 * of secure data space available after all the unprivileged data,
 * round that down to a power of 2 to get the actual size we want
 * to use for privileged data, and then determine this value from
 * that. We'd also potentially have to update the configs for SMPU9
 * and SMPU10.
 * Leave the SMPU alignment check in SMPU configuration file.
 */
#define S_DATA_UNPRIV_OFFSET (0)
#define S_DATA_UNPRIV_START  S_RAM_ALIAS(S_DATA_UNPRIV_OFFSET)

#define S_DATA_PRIV_OFFSET   (S_DATA_UNPRIV_OFFSET + S_UNPRIV_DATA_SIZE)
#define S_DATA_PRIV_START    S_RAM_ALIAS(S_DATA_PRIV_OFFSET)

/* Reserve area for RAM-based executable code right after secure unprivilaged
 * and privilaged data areas*/
#define S_RAM_CODE_OFFSET    (S_DATA_PRIV_OFFSET + S_PRIV_DATA_SIZE)
#define S_RAM_CODE_START     S_RAM_ALIAS(S_RAM_CODE_OFFSET)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET \
                        (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE    IMAGE_NS_CODE_SIZE
#define NS_CODE_LIMIT   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START   (S_RAM_ALIAS(S_DATA_SIZE))
#define NS_DATA_SIZE    (TOTAL_RAM_SIZE - S_DATA_SIZE)
#define NS_DATA_LIMIT   (NS_DATA_START + NS_DATA_SIZE - 1)

/* Shared memory */
#define NS_DATA_SHARED_SIZE  0x1000
#define NS_DATA_SHARED_START (NS_DATA_START + NS_DATA_SIZE - \
                              NS_DATA_SHARED_SIZE)
#define NS_DATA_SHARED_LIMIT (NS_DATA_SHARED_START + NS_DATA_SHARED_SIZE - 1)

/* Shared variables addresses */
/* ipcWaitMessageStc, cy_flash.c */
#define IPC_WAIT_MESSAGE_STC_SIZE 4
#define IPC_WAIT_MESSAGE_STC_ADDR (NS_DATA_SHARED_START + \
                                   NS_DATA_SHARED_SIZE - \
                                   IPC_WAIT_MESSAGE_STC_SIZE)

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START \
            (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))

#define NS_PARTITION_SIZE (FLASH_NS_PARTITION_SIZE)

#ifdef BL2
#error "BL2 configuration is not supported"
#endif /* BL2 */

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the privileged data area,
 * it is overlapping with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE (S_RAM_ALIAS(S_DATA_PRIV_OFFSET))
#define BOOT_TFM_SHARED_DATA_SIZE 0x400

#endif /* __REGION_DEFS_H__ */

