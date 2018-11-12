/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH

#include "fsl_flash.h"

int32_t flash_init(flash_t *obj)
{
    status_t result;

    /* Clean up Flash driver Structure*/
    memset(&obj->flash_config, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&obj->flash_config);
    if (kStatus_FLASH_Success != result) {
        return -1;
    } else {
        return 0;
    }
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    int status;

    /* We need to prevent flash accesses during erase operation */
    core_util_critical_section_enter();
    status = FLASH_Erase(&obj->flash_config, address, obj->flash_config.PFlashSectorSize, kFLASH_ApiEraseKey);

    if (status == kStatus_Success) {
        status = FLASH_VerifyErase(&obj->flash_config, address, obj->flash_config.PFlashSectorSize, kFLASH_MarginValueNormal);
    }
    core_util_critical_section_exit();

    if (status == kStatus_Success) {
        return 0;
    } else {
        return -1;
    }
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int status;

    /* We need to prevent flash accesses during program operation */
    core_util_critical_section_enter();
    status = FLASH_Program(&obj->flash_config, address, (uint32_t *)data, size);

    if (status == kStatus_Success) {
        // Must use kFlashMargin_User, or kFlashMargin_Factory for verify program
        status = FLASH_VerifyProgram(&obj->flash_config, address, size,
                              (uint32_t *)data, kFLASH_MarginValueUser,
                              NULL, NULL);
    }
    core_util_critical_section_exit();

    return status;

}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    uint32_t sectorsize = MBED_FLASH_INVALID_SIZE;
    uint32_t devicesize = 0;
    uint32_t startaddr = 0;

#if defined(TARGET_RAPIDIOT)
    startaddr = MBED_ROM_START;
    devicesize = MBED_ROM_SIZE;
#else
    FLASH_GetProperty((flash_config_t *)&obj->flash_config, kFLASH_PropertyPflashBlockBaseAddr, &startaddr);
    FLASH_GetProperty((flash_config_t *)&obj->flash_config, kFLASH_PropertyPflashTotalSize, &devicesize);
#endif

    if ((address >= startaddr) && (address < (startaddr + devicesize))) {
        FLASH_GetProperty((flash_config_t *)&obj->flash_config, kFLASH_PropertyPflashSectorSize, &sectorsize);
    }

    return sectorsize;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
#if defined(TARGET_RAPIDIOT)
    return MBED_ROM_START;
#else
    uint32_t startaddr = 0;

    FLASH_GetProperty((flash_config_t *)&obj->flash_config, kFLASH_PropertyPflashBlockBaseAddr, &startaddr);

    return startaddr;
#endif
}

uint32_t flash_get_size(const flash_t *obj)
{
#if defined(TARGET_RAPIDIOT)
    return MBED_ROM_SIZE;
#else
    uint32_t devicesize = 0;

    FLASH_GetProperty((flash_config_t *)&obj->flash_config, kFLASH_PropertyPflashTotalSize, &devicesize);

    return devicesize;
#endif
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}

#endif
