/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ----------------------------------------------------------- Includes -----------------------------------------------------------
#if DEVICE_FLASH
#include "DirectAccessDevicekey.h"
#include <string.h>
#include <stdio.h>
#include "mbed_error.h"
#include "MbedCRC.h"
#include "mbed_trace.h"
#define TRACE_GROUP "DADK"

using namespace mbed;

// --------------------------------------------------------- Definitions ----------------------------------------------------------
typedef struct {
    uint32_t address;
    size_t   size;
} tdbstore_area_data_t;

typedef struct {
    uint16_t trailer_size;
    uint16_t data_size;
    uint32_t crc;
} reserved_trailer_t;

#define TDBSTORE_NUMBER_OF_AREAS 2
#define MAX_DEVICEKEY_DATA_SIZE 64
#define RESERVED_AREA_SIZE (MAX_DEVICEKEY_DATA_SIZE+sizeof(reserved_trailer_t)) /* DeviceKey Max Data size + metadata trailer */

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

// -------------------------------------------------- Local Functions Declaration ----------------------------------------------------
static int calc_area_params(FlashIAP *flash, uint32_t out_tdb_start_offset, uint32_t tdb_end_offset,
                            tdbstore_area_data_t *area_params);
static int reserved_data_get(FlashIAP *flash, tdbstore_area_data_t *area_params, void *reserved_data_buf,
                             size_t reserved_data_buf_size, size_t *actual_data_size_ptr);
static inline uint32_t align_up(uint64_t val, uint64_t size);
static inline uint32_t align_down(uint64_t val, uint64_t size);
static uint32_t calc_crc(uint32_t init_crc, uint32_t data_size, const void *data_buf);

// -------------------------------------------------- API Functions Implementation ----------------------------------------------------
int direct_access_to_devicekey(uint32_t tdb_start_offset, uint32_t tdb_end_offset, void *data_buf,
                               size_t data_buf_size, size_t *actual_data_size_ptr)
{
    int status = MBED_ERROR_INVALID_ARGUMENT;
    FlashIAP flash;
    uint8_t active_area = 0;
    tdbstore_area_data_t area_params[TDBSTORE_NUMBER_OF_AREAS];
    memset(area_params, 0, sizeof(area_params));
    bool is_flash_init = false;

    if (NULL == data_buf) {
        tr_error("Invalid Data Buf Argument");
        goto exit_point;
    }

    status = flash.init();
    if (status != 0) {
        tr_error("FlashIAP init failed - err: %d", status);
        status = MBED_ERROR_FAILED_OPERATION;
        goto exit_point;
    }

    is_flash_init = true;

    status = calc_area_params(&flash, tdb_start_offset, tdb_end_offset, area_params);
    if (status != MBED_SUCCESS) {
        tr_error("Couldn't calulate Area Params - err: %d", status);
        goto exit_point;
    }

    /* DeviceKey data can be found either in first or second Flash Area */
    /* Loop through Areas to find valid DeviceKey data */
    for (active_area = 0; active_area < TDBSTORE_NUMBER_OF_AREAS; active_area++) {
        status = reserved_data_get(&flash, &area_params[active_area], data_buf, data_buf_size, actual_data_size_ptr);
        if (status == MBED_SUCCESS) {
            break;
        }
    }

    if (status != MBED_SUCCESS) {
        status = MBED_ERROR_ITEM_NOT_FOUND;
        tr_error("Couldn't find valid DeviceKey - err: %d", status);
    }

exit_point:
    if (true == is_flash_init) {
        flash.deinit();
    }

    return status;
}

int  get_expected_internal_TDBStore_position(uint32_t *out_tdb_start_offset, uint32_t *out_tdb_end_offset)
{
    uint32_t flash_end_address;
    uint32_t flash_start_address;
    uint32_t aligned_start_address;
    uint32_t aligned_end_address;
    FlashIAP flash;
    bool is_default_configuration = false;
    uint32_t tdb_size;

    if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "FILESYSTEM") == 0) {
#ifndef MBED_CONF_STORAGE_FILESYSTEM_INTERNAL_BASE_ADDRESS
        return MBED_ERROR_ITEM_NOT_FOUND;
#else
        *out_tdb_start_offset =  MBED_CONF_STORAGE_FILESYSTEM_INTERNAL_BASE_ADDRESS;
        tdb_size = MBED_CONF_STORAGE_FILESYSTEM_RBP_INTERNAL_SIZE;
#endif

    } else if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "TDB_EXTERNAL") == 0) {
#ifndef MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS
        return MBED_ERROR_ITEM_NOT_FOUND;
#else
        *out_tdb_start_offset =  MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS;
        tdb_size = MBED_CONF_STORAGE_TDB_EXTERNAL_RBP_INTERNAL_SIZE;
#endif

    } else if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "TDB_INTERNAL") == 0) {
#ifndef MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS
        return MBED_ERROR_ITEM_NOT_FOUND;
#else
        *out_tdb_start_offset =  MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS;
        tdb_size = MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE;
#endif

    } else if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "default") == 0) {
#ifndef MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS
        return MBED_ERROR_ITEM_NOT_FOUND;
#else
#if COMPONENT_QSPIF || COMPONENT_SPIF || COMPONENT_DATAFLASH
        *out_tdb_start_offset =  MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS;
        tdb_size = MBED_CONF_STORAGE_TDB_EXTERNAL_RBP_INTERNAL_SIZE;
#elif COMPONENT_SD
        tdb_size = MBED_CONF_STORAGE_FILESYSTEM_RBP_INTERNAL_SIZE;
#else
        return MBED_ERROR_UNSUPPORTED;
#endif // COMPONENT_QSPIF || COMPONENT_SPIF || COMPONENT_DATAFLASH
#endif // MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS
    } else {
        return MBED_ERROR_UNSUPPORTED;
    }

    *out_tdb_end_offset = (*out_tdb_start_offset) + tdb_size;

    if ((*out_tdb_start_offset == 0) && (tdb_size == 0)) {
        is_default_configuration = true;
    } else if ((*out_tdb_start_offset == 0) || (tdb_size == 0)) {
        return MBED_ERROR_UNSUPPORTED;
    }

    int ret = flash.init();
    if (ret != 0) {
        return MBED_ERROR_FAILED_OPERATION;
    }

    uint32_t flash_first_writable_sector_address = (uint32_t)(align_up(FLASHIAP_APP_ROM_END_ADDR,
                                                                       flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR)));

    //Get flash parameters before starting
    flash_start_address = flash.get_flash_start();
    flash_end_address = flash_start_address + flash.get_flash_size();

    if (!is_default_configuration) {
        aligned_start_address = align_down(*out_tdb_start_offset, flash.get_sector_size(*out_tdb_start_offset));
        aligned_end_address = align_up(*out_tdb_end_offset, flash.get_sector_size(*out_tdb_end_offset - 1));
        if ((*out_tdb_start_offset != aligned_start_address) || (*out_tdb_end_offset != aligned_end_address)
                || (*out_tdb_end_offset > flash_end_address)) {
            flash.deinit();
            return MBED_ERROR_INVALID_OPERATION;
        }
    } else {
        aligned_start_address = flash_end_address - (flash.get_sector_size(flash_end_address - 1) * 2);
        if (aligned_start_address < flash_first_writable_sector_address) {
            flash.deinit();
            return MBED_ERROR_INVALID_OPERATION;
        }
        *out_tdb_start_offset = aligned_start_address;
        *out_tdb_end_offset = flash_end_address;
    }

    flash.deinit();

    return MBED_SUCCESS;
}

// -------------------------------------------------- Local Functions Implementation ----------------------------------------------------
static int calc_area_params(FlashIAP *flash, uint32_t out_tdb_start_offset, uint32_t tdb_end_offset,
                            tdbstore_area_data_t *area_params)
{
    uint32_t bd_size = 0;
    uint32_t initial_erase_size = flash->get_sector_size(out_tdb_start_offset);
    uint32_t erase_unit_size = initial_erase_size;
    size_t cur_area_size = 0;

    if ((tdb_end_offset < (out_tdb_start_offset + 2 * RESERVED_AREA_SIZE - 1)) ||
            (tdb_end_offset > (flash->get_flash_start() + flash->get_flash_size()))) {
        tr_error("calc_area_params failed - Invalid input addresses");
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Entire TDBStore can't exceed 32 bits
    bd_size = (tdb_end_offset - out_tdb_start_offset + 1);

    while (cur_area_size < bd_size / 2) {
        erase_unit_size = flash->get_sector_size(out_tdb_start_offset + cur_area_size);
        cur_area_size += erase_unit_size;
    }

    area_params[0].address = out_tdb_start_offset;
    area_params[0].size = cur_area_size;
    area_params[1].address = out_tdb_start_offset + cur_area_size;
    area_params[1].size = bd_size - cur_area_size;

    return MBED_SUCCESS;
}

static int reserved_data_get(FlashIAP *flash, tdbstore_area_data_t *area_params, void *reserved_data_buf,
                             size_t reserved_data_buf_size, size_t *actual_data_size_ptr)
{
    int status = MBED_SUCCESS;
    reserved_trailer_t trailer;
    uint8_t *buf;
    int ret = MBED_SUCCESS;
    bool erased = true;
    size_t actual_size = 0;
    uint32_t initial_crc = 0xFFFFFFFF;
    uint32_t crc = initial_crc;
    uint8_t blank = flash->get_erase_value();

    /* Read Into trailer deviceKey metadata */
    ret = flash->read(&trailer, area_params->address + MAX_DEVICEKEY_DATA_SIZE, sizeof(trailer));
    if (ret != MBED_SUCCESS) {
        status =  MBED_ERROR_READ_FAILED;
        goto exit_point;
    }

    buf = reinterpret_cast <uint8_t *>(&trailer);
    for (uint32_t i = 0; i < sizeof(trailer); i++) {
        if (buf[i] != blank) {
            erased = false;
            break;
        }
    }

    if (true == erased) {
        /* Metadata is erased , DeviceKey Data is NOT in this Area */
        status =  MBED_ERROR_ITEM_NOT_FOUND;
        goto exit_point;
    }

    actual_size = trailer.data_size;
    if (actual_data_size_ptr != NULL) {
        *actual_data_size_ptr = actual_size;
    }
    if (reserved_data_buf_size < actual_size) {
        status =  MBED_ERROR_INVALID_SIZE;
        goto exit_point;
    }

    buf = reinterpret_cast <uint8_t *>(reserved_data_buf);

    /* Read DeviceKey Data */
    ret = flash->read(buf, area_params->address, (uint32_t)actual_size);
    if (ret != MBED_SUCCESS) {
        status = MBED_ERROR_READ_FAILED;
        goto exit_point;
    }

    crc = calc_crc(crc, (uint32_t)actual_size, buf);
    if (crc != trailer.crc) {
        status = MBED_ERROR_INVALID_DATA_DETECTED;
    }

exit_point:

    return status;
}

static inline uint32_t align_up(uint64_t val, uint64_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}

static uint32_t calc_crc(uint32_t init_crc, uint32_t data_size, const void *data_buf)
{
    uint32_t crc;
    MbedCRC<POLY_32BIT_ANSI, 32> ct(init_crc, 0x0, true, false);
    ct.compute(const_cast<void *>(data_buf), data_size, &crc);
    return crc;
}
#endif // DEVICE_FLASH
