/* mbed Microcontroller Library
 * Copyright (c) 2020 Arm Limited
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

#include "device.h"
#include "flash_layout.h"
#include "flash_api.h"

#if DEVICE_FLASH

#define FLASH_DEV          MT25QL_DEV_S

int32_t flash_init(flash_t *obj)
{
    (void)(obj);
    enum mt25ql_error_t err = MT25QL_ERR_NONE;

    qspi_ip6514e_enable(FLASH_DEV.controller);

    /* Configure QSPI Flash controller to operate in single SPI mode and
     * to use fast Flash commands */
    err = mt25ql_config_mode(&FLASH_DEV, MT25QL_FUNC_STATE_FAST);
    if (err != MT25QL_ERR_NONE) {
        return -1;
    }

    return 0;
}

int32_t flash_free(flash_t *obj)
{
    (void)(obj);
    enum mt25ql_error_t err = MT25QL_ERR_NONE;

    /* Restores the QSPI Flash controller and MT25QL to reset state */
    err = mt25ql_restore_reset_state(&FLASH_DEV);
    if (err != MT25QL_ERR_NONE) {
        return -1;
    }

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    (void)(obj);
    enum mt25ql_error_t err = MT25QL_ERR_NONE;

    address -= FLASH_DEV.direct_access_start_addr;

    err = mt25ql_erase(&FLASH_DEV, address, MT25QL_ERASE_SUBSECTOR_4K);
    if (err != MT25QL_ERR_NONE) {
        return -1;
    }

    return 0;
}

int32_t flash_read(flash_t *obj, uint32_t address,
                   uint8_t *data, uint32_t size)
{
    (void)obj;
    enum mt25ql_error_t err = MT25QL_ERR_NONE;

    address -= FLASH_DEV.direct_access_start_addr;

    err = mt25ql_command_read(&FLASH_DEV, address, data, size);
    if (err != MT25QL_ERR_NONE) {
        return -1;
    }

    return 0;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    (void)(obj);
    enum mt25ql_error_t err = MT25QL_ERR_NONE;

    address -= FLASH_DEV.direct_access_start_addr;

    err = mt25ql_command_write(&FLASH_DEV, address, data, size);
    if (err != MT25QL_ERR_NONE) {
        return -1;
    }

    return 0;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    (void)(obj);
    if ((address >= S_QSPI_ALIAS_BASE) && (address < S_QSPI_ALIAS_BASE + QSPI_FLASH_TOTAL_SIZE)) {
        return SUBSECTOR_4KB;
    }

    return MBED_FLASH_INVALID_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    (void)(obj);
    return FLASH_PAGE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    (void)(obj);
    return S_QSPI_ALIAS_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    (void)(obj);
    return QSPI_FLASH_TOTAL_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;
    return 0xFF;
}

#endif // DEVICE_FLASH
