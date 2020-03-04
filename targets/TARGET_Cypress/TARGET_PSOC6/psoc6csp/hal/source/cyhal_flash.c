/***************************************************************************//**
* \file cyhal_flash.c
*
* Description:
* Provides a high level interface for interacting with the Cypress Flash. This
* is wrapper around the lower level PDL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include "cyhal_hwmgr.h"
#include "cyhal_hw_types.h"
#include "cyhal_flash.h"
#include <string.h>

#ifdef CY_IP_MXS40SRSS

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

typedef cy_en_flashdrv_status_t (*flash_operation)(uint32_t rowAddr, const uint32_t* data);

static const cyhal_flash_block_info_t FLASH_BLOCKS[2] =
{
    // Main Flash
    {
        .start_address = CY_FLASH_BASE,
        .size = CY_FLASH_SIZE,
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

static uint8_t writeBuf[CY_FLASH_SIZEOF_ROW];

static inline bool is_in_flash(uint32_t address)
{
    return ((CY_FLASH_BASE <= address) && (address < (CY_FLASH_BASE + CY_FLASH_SIZE))) ||
           ((CY_EM_EEPROM_BASE <= address) && (address < (CY_EM_EEPROM_BASE + CY_EM_EEPROM_SIZE)));
}
static inline bool is_in_sram(uint32_t address)
{
    return (CY_SRAM_BASE <= address) && (address < (CY_SRAM_BASE + CY_SRAM_SIZE));
}

static cy_rslt_t run_flash_operation(
    flash_operation operation, uint32_t address, const uint32_t* data, bool clearCache)
{
    const uint32_t* buffer;
    if (is_in_sram((uint32_t)data))
    {
        buffer = data;
    }
    else
    {
        memcpy(writeBuf, (const void *)data, CY_FLASH_SIZEOF_ROW);
        buffer = (const uint32_t*)writeBuf;
    }

    cy_en_flashdrv_status_t status =  operation(address, buffer);
    if (clearCache)
    {
        Cy_SysLib_ClearFlashCacheAndBuffer();
    }

    return (cy_rslt_t)status;
}

cy_rslt_t cyhal_flash_init(cyhal_flash_t *obj)
{
    CY_ASSERT(NULL != obj);
    return CY_RSLT_SUCCESS;
}

void cyhal_flash_free(cyhal_flash_t *obj)
{
    CY_ASSERT(NULL != obj);
}

void cyhal_flash_get_info(const cyhal_flash_t *obj, cyhal_flash_info_t *info)
{
    CY_ASSERT(NULL != obj);

    info->block_count = 2;
    info->blocks = &FLASH_BLOCKS[0];
}

cy_rslt_t cyhal_flash_read(cyhal_flash_t *obj, uint32_t address, uint8_t *data, size_t size)
{
    CY_ASSERT(NULL != obj);
    if (!is_in_flash(address) || !is_in_flash(address + size - 1))
    {
        return CYHAL_FLASH_RSLT_ERR_ADDRESS;
    }
    memcpy(data, (void *)address, size);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_flash_erase(cyhal_flash_t *obj, uint32_t address)
{
    CY_ASSERT(NULL != obj);
    cy_rslt_t status = CYHAL_FLASH_RSLT_ERR_ADDRESS;

    if (is_in_flash(address))
    {
        status = Cy_Flash_EraseRow(address);
        Cy_SysLib_ClearFlashCacheAndBuffer();
    }

    return (status);
}

cy_rslt_t cyhal_flash_write(cyhal_flash_t *obj, uint32_t address, const uint32_t* data)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t status = is_in_flash(address)
        ? run_flash_operation(Cy_Flash_WriteRow, address, data, true)
        : CYHAL_FLASH_RSLT_ERR_ADDRESS;

    return (status);
}

cy_rslt_t cyhal_flash_program(cyhal_flash_t *obj, uint32_t address, const uint32_t *data)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t status = is_in_flash(address)
        ? run_flash_operation(Cy_Flash_ProgramRow, address, data, true)
        : CYHAL_FLASH_RSLT_ERR_ADDRESS;

    return (status);
}

cy_rslt_t cyhal_flash_start_erase(cyhal_flash_t *obj, uint32_t address)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t status = is_in_flash(address)
        ? Cy_Flash_StartEraseRow(address)
        : CYHAL_FLASH_RSLT_ERR_ADDRESS;

    return (status);
}

cy_rslt_t cyhal_flash_start_write(cyhal_flash_t *obj, uint32_t address, const uint32_t* data)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t status = is_in_flash(address)
        ? run_flash_operation(Cy_Flash_StartWrite, address, data, false)
        : CYHAL_FLASH_RSLT_ERR_ADDRESS;

    return (status);
}

cy_rslt_t cyhal_flash_start_program(cyhal_flash_t *obj, uint32_t address, const uint32_t* data)
{
    CY_ASSERT(NULL != obj);

    cy_rslt_t status = is_in_flash(address)
        ? run_flash_operation(Cy_Flash_StartProgram, address, data, false)
        : CYHAL_FLASH_RSLT_ERR_ADDRESS;

    return (status);
}

bool cyhal_flash_is_operation_complete(cyhal_flash_t *obj)
{
    CY_ASSERT(NULL != obj);

    bool complete;
    cy_rslt_t status = Cy_Flash_IsOperationComplete();
    if (CY_FLASH_DRV_SUCCESS == status)
    {
        Cy_SysLib_ClearFlashCacheAndBuffer();
        complete = true;
    }
    else
    {
        complete = false;
    }

    return complete;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CY_IP_MXS40SRSS */
