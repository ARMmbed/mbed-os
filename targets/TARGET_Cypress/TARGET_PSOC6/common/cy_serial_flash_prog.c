/***************************************************************************//**
* \file cy_serial_flash_prog.c
*
* \brief
* Provides variables necessary to inform programming tools how to program the
* attached serial flash memory. The variables used here must be placed at
* specific locations in order to be detected and used by programming tools
* to know that there is an attached memory and its characteristics. Uses the
* configuration provided as part of BSP.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

/**
* \addtogroup group_serial_flash Serial Flash
* \{
* Variables for informing programming tools that there is an attached memory device and what
* its characteristics are so it can be programmed just like the on-chip memory.
*
* \defgroup group_serial_flash_variables Variables
*/

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CY_ENABLE_XIP_PROGRAM)

#include "cycfg_qspi_memslot.h"

typedef struct
{
    const cy_stc_smif_block_config_t * smifCfg; /* Pointer to SMIF top-level configuration */
    const uint32_t null_t; /* NULL termination */
} stc_smif_ipblocks_arr_t;

/**
* \addtogroup group_serial_flash_variables
* \{
*/

/**
 * This data can be placed anywhere in the internal memory, but it must be at a location that
 * can be determined and used for the calculation of the CRC16 checksum in the cyToc below. There
 * are multiple ways this can be accomplished including:
 * 1) Placing it in a dedicated memory block with a known address. (as done here)
 * 2) Placing it at an absolute location via a the linker script
 * 3) Using 'cymcuelftool -S' to recompute the checksum and patch the elf file after linking
 */
CY_SECTION(".cy_sflash_user_data") __attribute__( (used) )
const stc_smif_ipblocks_arr_t smifIpBlocksArr = {&smifBlockConfig, 0x00000000};

/**
 * This data is used to populate the table of contents part 2. When present, it is used by the boot
 * process and programming tools to determine key characteristics about the memory usage including
 * where the boot process should start the application from and what external memories are connected
 * (if any). This must consume a full row of flash memory row. The last entry is a checksum of the
 * other values in the ToC which must be updated if any other value changes. This can be done manually
 * or by running 'cymcuelftool -S' to recompute the checksum.
 */
CY_SECTION(".cy_toc_part2") __attribute__( (used) )
const uint32_t cyToc[128] =
{
    0x200-4,                /* Offset=0x0000: Object Size, bytes */
    0x01211220,             /* Offset=0x0004: Magic Number (TOC Part 2, ID) */
    0,                      /* Offset=0x0008: Key Storage Address */
    (int)&smifIpBlocksArr,  /* Offset=0x000C: This points to a null terminated array of SMIF structures. */
    0x10000000u,            /* Offset=0x0010: App image start address */
                            /* Offset=0x0014-0x01F7: Reserved */
    [126] =  0x000002C2,    /* Offset=0x01F8: Bits[ 1: 0] CLOCK_CONFIG (0=8MHz, 1=25MHz, 2=50MHz, 3=100MHz)
                                              Bits[ 4: 2] LISTEN_WINDOW (0=20ms, 1=10ms, 2=1ms, 3=0ms, 4=100ms)
                                              Bits[ 6: 5] SWJ_PINS_CTL (0/1/3=Disable SWJ, 2=Enable SWJ)
                                              Bits[ 8: 7] APP_AUTHENTICATION (0/2/3=Enable, 1=Disable)
                                              Bits[10: 9] FB_BOOTLOADER_CTL: UNUSED */
    [127] =  0x3BB30000     /* Offset=0x01FC: CRC16-CCITT (the upper 2 bytes contain the CRC and the lower 2 bytes are 0) */
};

/** \} group_serial_flash_variables */

#endif /* defined(CY_ENABLE_XIP_PROGRAM) */

#if defined(__cplusplus)
}
#endif

/** \} group_serial_flash */
