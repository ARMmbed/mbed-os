/* Copyright (c) 2018 ARM Limited
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

#include <cstring>
#include "KVStore.h"
#include "TDBStore.h"
#include "psa/internal_trusted_storage.h"
#include "pits_impl.h"
#include "mbed_error.h"
#include "mbed_toolchain.h"
#include "FlashIAP.h"
#include "FlashIAPBlockDevice.h"

using namespace mbed;

static KVStore *internal_store = NULL;
static bool is_tfm_kv_initialized = false;

static inline uint32_t align_up(uint64_t val, uint64_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}

static BlockDevice *_get_blockdevice(bd_addr_t start_address, bd_size_t size)
{
    int ret = MBED_SUCCESS;
    bd_addr_t flash_end_address;
    bd_addr_t flash_start_address;
    bd_addr_t aligned_start_address;
    bd_addr_t aligned_end_address;
    bd_addr_t end_address;
    FlashIAP flash;

    ret = flash.init();
    if (ret != 0) {
        return NULL;
    }

    //Get flash parameters before starting
    flash_start_address = flash.get_flash_start();
    flash_end_address = flash_start_address + flash.get_flash_size();;

    aligned_start_address = align_down(start_address, flash.get_sector_size(start_address));
    if (start_address != aligned_start_address) {
        flash.deinit();
        return NULL;
    }

    end_address = start_address + size;
    if (end_address > flash_end_address) {
        flash.deinit();
        return NULL;
    }

    aligned_end_address = align_up(end_address, flash.get_sector_size(end_address - 1));
    if (end_address != aligned_end_address) {
        flash.deinit();
        return NULL;
    }

    static FlashIAPBlockDevice bd(start_address, size);
    flash.deinit();
    return &bd;
}

static int _calculate_blocksize_match_tdbstore(BlockDevice *bd)
{
    bd_size_t size = bd->size();
    bd_size_t erase_size = bd->get_erase_size();
    bd_size_t number_of_sector = size / erase_size;

    if (number_of_sector < 2) {
        return -1;
    }

    return 0;
}

static int tfm_kv_init(void)
{
    int ret = MBED_SUCCESS;
    bd_size_t internal_size = MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE;
    bd_addr_t internal_start_address = MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS;

    //Get internal memory FLASHIAP block device.
    BlockDevice *internal_bd = _get_blockdevice(internal_start_address, internal_size);
    if (internal_bd == NULL) {
        return -1; // TODO: Error code
    }

    ret = internal_bd->init();
    if (ret != 0) {
        return ret;
    }

    //Check that internal flash has 2 or more sectors
    if (_calculate_blocksize_match_tdbstore(internal_bd) != 0) {
        return -1; // TODO: Error code
    }

    //Deinitialize internal block device and TDB will reinitialize and take control on it.
    ret = internal_bd->deinit();
    if (ret != 0) {
        return ret;
    }

    //Create a TDBStore in the internal FLASHIAP block device.
    static TDBStore tdb_internal(internal_bd);
    internal_store = &tdb_internal;

    ret = internal_store->init();

    return ret;
}

/*
 * \brief Get default KVStore instance for internal flesh storage
 *
 * \return valid pointer to KVStore
 */

KVStore *get_its_kvstore_instance(void)
{
    return internal_store;
}
extern "C" int kv_init_storage_config()
{
    int ret = MBED_SUCCESS;

    if (!is_tfm_kv_initialized) {
        ret = tfm_kv_init();
    }

    is_tfm_kv_initialized = (ret == MBED_SUCCESS) ? true : false;
    return ret;
}


