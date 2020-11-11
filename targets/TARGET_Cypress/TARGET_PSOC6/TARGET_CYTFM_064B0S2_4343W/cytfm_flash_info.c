/***************************************************************************//**
* \file cytfm_flash_info.c
*
* Description:
* Provides Flash characteristics information for target CYTFM_064B0S2_4343W
* Cypress board.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
#include "cyhal_flash.h"
#include "region_defs.h"
#include "cytfm_flash_info.h"

#if DEVICE_FLASH && defined(TARGET_TFM) && MBED_CONF_PSA_PRESENT

#ifdef __cplusplus
extern "C" {
#endif

/* Flash layout for App on TARGET_CYTFM_064B0S2_4343W
 *   +--------------------------+
 *   |                          |
 *   |         KVSTORE          |  }+ MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE
 *   |                          |
 *   +--------------------------+
 *   |                          |
 *   |                          |
 *   |  NS partition for App    |  }+ NS_PARTITION_SIZE
 *   |                          |
 *   |                          |
 *   +--------------------------+ <-+ NS_PARTITION_START
 */

static const cyhal_flash_block_info_t CYTFM_FLASH_BLOCKS[2] =
{
    // Main Flash
    {
        .start_address = NS_PARTITION_START,
        .size = NS_PARTITION_SIZE + MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE,
        .sector_size = CY_FLASH_SIZEOF_ROW,
        .page_size = CY_FLASH_SIZEOF_ROW,
        .erase_value = 0x00U,
    },
    // Working Flash
    {
        .start_address = CY_EM_EEPROM_BASE,
        .size = CY_EM_EEPROM_SIZE,
        .sector_size = CY_FLASH_SIZEOF_ROW,
        .page_size = CY_FLASH_SIZEOF_ROW,
        .erase_value = 0x00U,
    },
};

void cytfm_flash_get_info(const cyhal_flash_t *obj, cyhal_flash_info_t *info)
{
    CY_UNUSED_PARAMETER(obj);
    CY_ASSERT(NULL != obj);

    info->block_count =
          sizeof(CYTFM_FLASH_BLOCKS) / sizeof(cyhal_flash_block_info_t);
    info->blocks = CYTFM_FLASH_BLOCKS;
}

#ifdef __cplusplus
}
#endif

#endif
