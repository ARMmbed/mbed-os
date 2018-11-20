/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "flash_api.h"
#include "mbed_critical.h"

#if DEVICE_FLASH

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#include <arm_cmse.h>
#include "fsl_iap.h"
#include "partition/region_defs.h"

static flash_config_t flash_config;

/* Check if address range [start_addr, end_addr] is in non-secure flash
 *
 *  @param obj          The flash object
 *  @param start_addr   Start address to check
 *  @param end_addr     End address to check. Could be the same as start_addr to just check start_addr
 *                      for e.g. flash_erase_sector.
 *  @return             0 for success, -1 for error
 */
static int32_t flash_check_nonsecure(flash_t *obj, uint32_t start_addr, uint32_t end_addr)
{
    /* Check if end address wraps around */
    if (end_addr < start_addr) {
        return -1;
    }

    /* Check if start address is in non-secure flash */
    if ((start_addr < FLASH_AREA_IMAGE_1_OFFSET) ||
        (start_addr >= (FLASH_AREA_IMAGE_1_OFFSET + NS_CODE_SIZE))) {
        return -1;
    }

    /* Check if end address is in non-secure flash */
    if (end_addr != start_addr) {
        if ((end_addr < FLASH_AREA_IMAGE_1_OFFSET) ||
            (end_addr >= (FLASH_AREA_IMAGE_1_OFFSET + NS_CODE_SIZE))) {
            return -1;
        }
    }

    return 0;
}

MBED_NONSECURE_ENTRY int32_t flash_init(flash_t *obj)
{
    if (FLASH_Init(&flash_config) != kStatus_Success) {
        return -1;
    } else {
        return 0;
    }
}

MBED_NONSECURE_ENTRY int32_t flash_free(flash_t *obj)
{
    return 0;
}

MBED_NONSECURE_ENTRY int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    if (cmse_nonsecure_caller()) {
        // Confine non-secure access to non-secure flash
        if (flash_check_nonsecure(obj, address, address)) {
            return -1;
        }
    }

    core_util_critical_section_enter();

    uint32_t status;

    status = FLASH_Erase(&flash_config, address, flash_config.PFlashPageSize, kFLASH_ApiEraseKey);
    if (status == kStatus_Success) {
        status = FLASH_VerifyErase(&flash_config, address, flash_config.PFlashPageSize);
    }
    core_util_critical_section_exit();

    if (status != kStatus_Success) {
        return -1;
    } else {
        return 0;
    }
}

MBED_NONSECURE_ENTRY int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    if (cmse_nonsecure_caller()) {
        // Confine non-secure access to non-secure flash
        if (flash_check_nonsecure(obj, address, address + size - 1)) {
            return -1;
        }
    }

    core_util_critical_section_enter();

    uint32_t n;
    uint32_t status;
    uint32_t failedAddress, failedData;

    status = FLASH_Program(&flash_config, address, data, size);
    if (status == kStatus_Success) {
        status = FLASH_VerifyProgram(&flash_config, address, size, data, &failedAddress, &failedData);
    }
    core_util_critical_section_exit();

    if (status != kStatus_Success) {
        return -1;
    } else {
        return 0;
    }
}

MBED_NONSECURE_ENTRY uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (cmse_nonsecure_caller()) {
        if ((address >= FLASH_AREA_IMAGE_1_OFFSET) &&
            (address < (FLASH_AREA_IMAGE_1_OFFSET + NS_CODE_SIZE))) {
            return flash_config.PFlashPageSize;
        }

        return MBED_FLASH_INVALID_SIZE;
    }

    if ((address >= FLASH_AREA_IMAGE_0_OFFSET) &&
        (address < (FLASH_AREA_IMAGE_0_OFFSET + 0x40000))) {
        return flash_config.PFlashPageSize;
    }

    return MBED_FLASH_INVALID_SIZE;
}

MBED_NONSECURE_ENTRY int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    if (cmse_nonsecure_caller()) {
        // Confine non-secure access to non-secure flash
        if (flash_check_nonsecure(obj, address, address + size - 1)) {
            return -1;
        }
    }

    /* Check if flash is empty */
    if (FLASH_VerifyErase(&flash_config, address, size) == kStatus_FLASH_Success) {
        memset(data, 0x0, size);
        return 0;
    }

    memcpy(data, (const void *)address, size);
    return 0;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_page_size(const flash_t *obj)
{
    return flash_config.PFlashPageSize;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_start_address(const flash_t *obj)
{
    if (cmse_nonsecure_caller()) {
        return FLASH_AREA_IMAGE_1_OFFSET;
    }

    return FLASH_AREA_IMAGE_0_OFFSET;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_size(const flash_t *obj)
{
    if (cmse_nonsecure_caller()) {
        return NS_CODE_SIZE;
    }

    return 0x40000;
}

MBED_NONSECURE_ENTRY uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0x0;
}

#endif  // #if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#endif

