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

#include "features/storage/kvstore/include/KVStore.h"
#include "features/storage/kvstore/kv_map/KVMap.h"
#include "features/storage/kvstore/conf/kv_config.h"
#include <stdlib.h>
#include "string.h"
#include "mbed_error.h"

namespace mbed {

KVMap::~KVMap()
{
    deinit();
}

int KVMap::init()
{
    int ret = MBED_SUCCESS;

    _mutex->lock();

    if (_is_initialized) {
        goto exit;
    }

    _kv_num_attached_kvs = 0;
    memset(&_kv_map_table, 0, sizeof(_kv_map_table));

    _is_initialized = 1;

exit:
    _mutex->unlock();
    return ret;
}

int KVMap::attach(const char *partition_name, kvstore_config_t *kv_config)
{
    int ret = MBED_SUCCESS;
    char *kv_partition_name = NULL;

    _mutex->lock();

    if (!_is_initialized) {
        ret = MBED_ERROR_NOT_READY;
        goto exit;
    }

    if (_kv_num_attached_kvs >= MAX_ATTACHED_KVS) {
        ret =  MBED_ERROR_OUT_OF_MEMORY;
        goto exit;
    }

    kv_partition_name = new char[strlen(partition_name) + 1];
    strcpy(kv_partition_name, partition_name);
    _kv_map_table[_kv_num_attached_kvs].partition_name = kv_partition_name;
    _kv_map_table[_kv_num_attached_kvs].kv_config = kv_config;
    _kv_num_attached_kvs++;

exit:
    _mutex->unlock();
    return ret;
}

void KVMap::deinit_partition(kv_map_entry_t *partition)
{
    if (partition->kv_config == NULL) {
        return;
    }

    if (partition->kv_config->external_store != NULL) {
        partition->kv_config->external_store->deinit();
    }

    // TODO: this should be removed after FS APIs are standardized
    if (partition->kv_config->external_fs != NULL) {
        partition->kv_config->external_fs->unmount();
    }

    if (partition->kv_config->internal_store != NULL) {
        partition->kv_config->internal_store->deinit();
    }

    if (partition->kv_config->kvstore_main_instance != NULL) {
        partition->kv_config->kvstore_main_instance->deinit();
    }

    delete [] partition->partition_name;
    partition->partition_name = NULL;
    partition->kv_config = NULL;
}


int KVMap::detach(const char *partition_name)
{
    int ret = MBED_SUCCESS;

    _mutex->lock();

    if (!_is_initialized) {
        ret = MBED_ERROR_NOT_READY;
        goto exit;
    }

    ret = MBED_ERROR_ITEM_NOT_FOUND;
    for (int i = 0; i < _kv_num_attached_kvs; i++) {

        if (strcmp(partition_name, _kv_map_table[i].partition_name) != 0) {
            continue;
        }

        deinit_partition(&_kv_map_table[i]);

        memcpy(&_kv_map_table[i], &_kv_map_table[i + 1], sizeof(kv_map_entry_t) * (MAX_ATTACHED_KVS - i - 1));
        _kv_map_table[MAX_ATTACHED_KVS - 1].partition_name = NULL;
        _kv_map_table[MAX_ATTACHED_KVS - 1].kv_config->kvstore_main_instance = NULL;
        _kv_num_attached_kvs--;
        ret = MBED_SUCCESS;
        break;
    }

exit:
    _mutex->unlock();
    return ret;
}

int KVMap::deinit()
{
    int ret = MBED_SUCCESS;

    _mutex->lock();

    if (!_is_initialized) {
        ret = MBED_ERROR_NOT_READY;
        goto exit;
    }

    for (int i = 0; i < _kv_num_attached_kvs; i++) {

        if (_kv_map_table[i].kv_config->kvstore_main_instance == NULL) {
            goto exit;
        }

        deinit_partition(&_kv_map_table[i]);
    }

exit:
    _kv_num_attached_kvs = 0;
    _mutex->unlock();
    return ret;
}

// Full name lookup and then break it into KVStore instance and key
int KVMap::lookup(const char *full_name, KVStore **kv_instance, size_t *key_index, uint32_t *flags_mask)
{
    _mutex->lock();

    kvstore_config_t *kv_config;
    int ret = config_lookup(full_name, &kv_config, key_index);
    if (ret != MBED_SUCCESS) {
        goto exit;
    }

    *kv_instance = kv_config->kvstore_main_instance;
    if (flags_mask != NULL) {
        *flags_mask = kv_config->flags_mask;
    }

exit:
    _mutex->unlock();
    return ret;
}

// Full name lookup and then break it into KVStore configuration struct and key
int KVMap::config_lookup(const char *full_name, kvstore_config_t **kv_config, size_t *key_index)
{
    int ret = MBED_SUCCESS;
    int delimiter_index;
    int i;
    const char *delimiter_position;

    const char *temp_str = full_name;

    if (!_is_initialized) {
        ret = MBED_ERROR_NOT_READY;
        goto exit;
    }

    if (temp_str != NULL) {
        *key_index = 0;
        if (*temp_str == '/') {
            temp_str++;
            (*key_index)++;
        }

        delimiter_position = strchr(temp_str, '/');
        if (delimiter_position == NULL) {  //delimiter not found
            delimiter_index = -1;
            *kv_config = _kv_map_table[0].kv_config;
            goto exit;
        }
    } else {
        delimiter_index = -1;
        *kv_config = _kv_map_table[0].kv_config;
        goto exit;
    }


    delimiter_index = delimiter_position - temp_str;
    for (i = 0; i < _kv_num_attached_kvs; i++) {

        if (strncmp(temp_str, _kv_map_table[i].partition_name, delimiter_index) != 0) {
            continue;
        }

        *kv_config = _kv_map_table[i].kv_config;
        break;
    }
    if (i == _kv_num_attached_kvs) {
        ret = MBED_ERROR_ITEM_NOT_FOUND;
        goto exit;
    }
exit:
    if (ret == MBED_SUCCESS) {
        //if success extract the key
        *key_index = *key_index + delimiter_index + 1;
    }
    return ret;
}

KVStore *KVMap::get_internal_kv_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->internal_store;
}

KVStore *KVMap::get_external_kv_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->external_store;
}

KVStore *KVMap::get_main_kv_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->kvstore_main_instance;
}

BlockDevice *KVMap::get_internal_blockdevice_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->internal_bd;
}

BlockDevice *KVMap::get_external_blockdevice_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->external_bd;
}

FileSystem *KVMap::get_external_filesystem_instance(const char *name)
{

    _mutex->lock();

    kvstore_config_t *kv_config;
    size_t key_index = 0;

    int ret = config_lookup(name, &kv_config, &key_index);

    _mutex->unlock();

    return ret != MBED_SUCCESS ? NULL : kv_config->external_fs;
}

} // namespace mbed

