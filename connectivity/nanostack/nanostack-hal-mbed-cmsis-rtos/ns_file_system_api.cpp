/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#include <new>
#include <string.h>
#include "mbed_error.h"
#include "kvstore_global_api.h"
#include "kv_config.h"
#if MBED_CONF_NANOSTACK_HAL_USE_KVSTORE == 1
#include "ns_file_system.h"
#endif

#define TRACE_GROUP "kvs"

#if MBED_CONF_NANOSTACK_HAL_USE_KVSTORE == 1

static NS_FILE ns_file_system_api_open(const char *filename, const char *mode);
static int ns_file_system_api_close(NS_FILE *handle);
static int ns_file_system_api_remove(const char *filename);
static size_t ns_file_system_api_write(NS_FILE *handle, const void *buffer, size_t size);
static size_t ns_file_system_api_read(NS_FILE *handle, void *buffer, size_t size);
static int ns_file_system_api_size(NS_FILE *handle, size_t *size);

extern "C" {
    void ns_file_system_api_init(void)
    {
        ns_file_system_set_root_path(MBED_CONF_NANOSTACK_HAL_KVSTORE_PATH);
        ns_file_system_callbacks_set(ns_file_system_api_open, ns_file_system_api_close, ns_file_system_api_remove, ns_file_system_api_write, ns_file_system_api_read, ns_file_system_api_size);
    }
}

static NS_FILE ns_file_system_api_open(const char *filename, const char *mode)
{
    if (!filename || (*mode != 'r' && *mode != 'w')) {
        return NULL;
    }

    uint16_t file_name_len = strlen(filename);
    if (file_name_len == 0) {
        return NULL;
    }

    char *full_name_key = new (std::nothrow) char[file_name_len + sizeof(char)];
    if (!full_name_key) {
        return NULL;
    }

    strcpy(full_name_key, filename);

    return (NS_FILE) full_name_key;
}

static int ns_file_system_api_close(NS_FILE *handle)
{
    if (!handle) {
        return -1;
    }
    char *full_name_key = (char *) handle;
    delete full_name_key;
    return 0;
}

static int ns_file_system_api_remove(const char *filename)
{
    if (!filename) {
        return -1;
    }

    int ret = kv_remove(filename);

    if (ret == MBED_SUCCESS) {
        return 0;
    }
    return -1;
}

static size_t ns_file_system_api_write(NS_FILE *handle, const void *buffer, size_t size)
{
    if (!handle || !buffer || size == 0) {
        return -1;
    }

    char *full_name_key = (char *) handle;
    int ret = kv_set(full_name_key, buffer, size, 0);

    if (ret == MBED_SUCCESS) {
        return size;
    }
    return 0;
}

static size_t ns_file_system_api_read(NS_FILE *handle, void *buffer, size_t size)
{
    if (!handle || !buffer || size == 0) {
        return -1;
    }

    char *full_name_key = (char *) handle;
    size_t read_size;
    int ret = kv_get(full_name_key, buffer, size, &read_size);

    if (ret == MBED_SUCCESS) {
        return read_size;
    }
    return 0;
}

static int ns_file_system_api_size(NS_FILE *handle, size_t *size)
{
    if (!handle || !size) {
        return -1;
    }
    char *full_name_key = (char *) handle;

    kv_info_t info;
    int ret = kv_get_info(full_name_key, &info);

    if (ret == MBED_SUCCESS) {
        *size = info.size;
        return 0;
    }
    return -1;
}

#endif
