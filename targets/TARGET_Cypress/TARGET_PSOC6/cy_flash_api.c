/* mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "flash_api.h"
#include "cyhal_flash.h"

#if DEVICE_FLASH

#ifdef __cplusplus
extern "C" {
#endif

int32_t flash_init(flash_t *obj)
{
    if (CY_RSLT_SUCCESS != cyhal_flash_init(&(obj->flash))) {
        return -1;
    }
    cyhal_flash_get_info(&(obj->flash), &(obj->info));
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    cyhal_flash_free(&(obj->flash));
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    return CY_RSLT_SUCCESS == cyhal_flash_erase(&(obj->flash), address) ? 0 : -1;
}

int32_t flash_read(flash_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    return CY_RSLT_SUCCESS == cyhal_flash_read(&(obj->flash), address, data, size) ? 0 : -1;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    MBED_ASSERT(0 == (address % obj->info.blocks[0].page_size));
    MBED_ASSERT(0 == (size % obj->info.blocks[0].page_size));
    for (uint32_t offset = 0; offset < size; offset += obj->info.blocks[0].page_size) {
        if (CY_RSLT_SUCCESS != cyhal_flash_program(&(obj->flash), address + offset, (const uint32_t *)(data + offset))) {
            return -1;
        }
    }
    return 0;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (address < obj->info.blocks[0].start_address || address >= obj->info.blocks[0].start_address + obj->info.blocks[0].size) {
        return MBED_FLASH_INVALID_SIZE;
    }
    return obj->info.blocks[0].sector_size;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return obj->info.blocks[0].page_size;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return obj->info.blocks[0].start_address;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return obj->info.blocks[0].size;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    return obj->info.blocks[0].erase_value;
}

#ifdef __cplusplus
}
#endif

#endif
