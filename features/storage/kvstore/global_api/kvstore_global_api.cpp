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
#include "kvstore_global_api.h"

#include "kv_config.h"
#include "KVMap.h"
#include "KVStore.h"
#include "mbed_error.h"

using namespace mbed;

// iterator handle
struct _opaque_kv_key_iterator {
    bool iterator_is_open;
    KVStore *kvstore_intance;
    KVStore::iterator_t *iterator_handle;
    char *path;
};

int kv_set(const char *full_name_key, const void *buffer, size_t size, uint32_t create_flags)
{
    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    uint32_t flags_mask = 0;
    size_t key_index = 0;
    ret = kv_map.lookup(full_name_key, &kv_instance, &key_index, &flags_mask);
    if (ret != MBED_SUCCESS) {
        return ret;
    }

    ret = kv_instance->set(full_name_key + key_index, buffer, size, create_flags & flags_mask);
    return ret;
}

int kv_get(const char *full_name_key, void *buffer, size_t buffer_size, size_t *actual_size)
{
    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    size_t key_index = 0;
    ret = kv_map.lookup(full_name_key, &kv_instance, &key_index);
    if (ret != MBED_SUCCESS) {
        return ret;
    }

    return kv_instance->get(full_name_key + key_index, buffer, buffer_size, actual_size);
}

int kv_get_info(const char *full_name_key, kv_info_t *info)
{
    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    size_t key_index = 0;
    ret = kv_map.lookup(full_name_key, &kv_instance, &key_index);
    if (ret != MBED_SUCCESS) {
        return ret;
    }

    KVStore::info_t inner_info;
    ret =  kv_instance->get_info(full_name_key + key_index, &inner_info);
    if (MBED_SUCCESS != ret) {
        return ret;
    }
    info->flags = inner_info.flags;
    info->size =  inner_info.size;
    return ret;
}

int kv_remove(const char *full_name_key)
{
    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    size_t key_index = 0;
    ret  = kv_map.lookup(full_name_key, &kv_instance, &key_index);
    if (ret != MBED_SUCCESS) {
        return ret;
    }

    return kv_instance->remove(full_name_key + key_index);
}

int kv_iterator_open(kv_iterator_t *it, const char *full_prefix)
{
    if (it == NULL) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    (*it) = new _opaque_kv_key_iterator;
    if (*it == NULL) {
        return MBED_ERROR_FAILED_OPERATION;
    }
    (*it)->iterator_is_open = false;

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    size_t key_index = 0;
    ret  = kv_map.lookup(full_prefix, &kv_instance, &key_index);
    if (ret != MBED_SUCCESS) {
        delete (*it);
        return ret;
    }

    (*it)->kvstore_intance = kv_instance;
    KVStore::iterator_t *inner_it = new KVStore::iterator_t;
    ret = kv_instance->iterator_open(inner_it, full_prefix + key_index);
    if (MBED_SUCCESS != ret) {
        delete inner_it;
        delete (*it);
        return ret;
    }

    (*it)->iterator_handle = inner_it;
    (*it)->iterator_is_open = true;
    (*it)->path = new char[key_index + 1];
    strncpy((*it)->path, full_prefix, key_index);
    (*it)->path[key_index] = '\0';
    return ret;

}

int kv_iterator_next(kv_iterator_t it, char *key, size_t key_size)
{
    if (!it->iterator_is_open) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    strcpy(key, it->path);
    int path_len = strlen(key);
    return it->kvstore_intance->iterator_next(*it->iterator_handle, key + path_len, key_size - path_len);
}

int kv_iterator_close(kv_iterator_t it)
{
    if (!it->iterator_is_open) {
        return MBED_ERROR_INVALID_ARGUMENT;
    }

    int ret = it->kvstore_intance->iterator_close(*it->iterator_handle);

    delete it->iterator_handle;
    delete[] it->path;
    delete it;

    return ret;
}

int kv_reset(const char *kvstore_name)
{
    int ret = kv_init_storage_config();
    if (MBED_SUCCESS != ret) {
        return ret;
    }

    KVMap &kv_map = KVMap::get_instance();
    KVStore *kv_instance = NULL;
    size_t key_index = 0;
    ret  = kv_map.lookup(kvstore_name, &kv_instance, &key_index);
    if (ret != MBED_SUCCESS) {
        return ret;
    }

    ret = kv_instance->reset();

    return ret;

}

