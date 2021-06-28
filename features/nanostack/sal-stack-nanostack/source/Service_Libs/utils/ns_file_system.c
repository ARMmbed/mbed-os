/*
 * Copyright (c) 2017-2020, Pelion and affiliates.
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

#include <string.h>
#include <stdio.h>
#include "ns_types.h"
#include "nsdynmemLIB.h"
#include "ns_file_system.h"
#include "ns_file.h"

static char *file_system_root;

static ns_file_open file_open_cb = NULL;
static ns_file_close file_close_cb = NULL;
static ns_file_remove file_remove_cb = NULL;
static ns_file_write file_write_cb = NULL;
static ns_file_read file_read_cb = NULL;
static ns_file_size file_size_cb = NULL;

int ns_file_system_set_root_path(const char *root_path)
{
    char *new_root_path;

    if (root_path == NULL) {
        // File system usage disabled
        ns_dyn_mem_free(file_system_root);
        file_system_root = NULL;
        return 0;
    }

    new_root_path = ns_dyn_mem_alloc(strlen(root_path) + 1);
    if (!new_root_path) {
        // mem alloc failed
        return -2;
    }

    ns_dyn_mem_free(file_system_root);
    file_system_root = new_root_path;
    strcpy(file_system_root, root_path);

    return 0;
}

char *ns_file_system_get_root_path(void)
{
    return file_system_root;
}

void ns_file_system_callbacks_set(ns_file_open open, ns_file_close close, ns_file_remove remove, ns_file_write write, ns_file_read read, ns_file_size size)
{
    file_open_cb = open;
    file_close_cb = close;
    file_remove_cb = remove;
    file_write_cb = write;
    file_read_cb = read;
    file_size_cb = size;
}

NS_FILE ns_fopen(const char *file_name, const char *mode)
{
    if (!file_name || !mode || (*mode != 'r' && *mode != 'w')) {
        return NULL;
    }

    if (file_open_cb) {
        return file_open_cb(file_name, mode);
    }

    FILE *file = fopen(file_name, mode);
    if (file == NULL) {
        return NULL;
    }

    return (NS_FILE) file;
}

int ns_fclose(NS_FILE *ns_handle)
{
    if (!ns_handle) {
        return -1;
    }

    if (file_close_cb) {
        return file_close_cb(ns_handle);
    }
    fclose((FILE *) ns_handle);
    return 0;
}

int ns_fremove(const char *file_name)
{
    if (file_remove_cb) {
        return file_remove_cb(file_name);
    }

    if (!file_name) {
        return -1;
    }

    return remove(file_name);
}

size_t ns_fwrite(NS_FILE *ns_handle, const void *buffer, size_t size)
{
    if (!ns_handle || !buffer || size == 0) {
        return 0;
    }

    if (file_write_cb) {
        return file_write_cb(ns_handle, buffer, size);
    }

    rewind((FILE *) ns_handle);
    return fwrite(buffer, 1, size, (FILE *) ns_handle);
}

size_t ns_fread(NS_FILE *ns_handle, void *buffer, size_t size)
{
    if (!ns_handle || !buffer || size == 0) {
        return 0;
    }

    if (file_read_cb) {
        return file_read_cb(ns_handle, buffer, size);
    }

    rewind((FILE *) ns_handle);
    return fread(buffer, 1, size, (FILE *) ns_handle);
}

int ns_fsize(NS_FILE *ns_handle, size_t *size)
{
    if (!ns_handle || !size) {
        return 0;
    }

    if (file_size_cb) {
        return file_size_cb(ns_handle, size);
    }

    fseek((FILE *) ns_handle, 0L, SEEK_END);
    return ftell((FILE *) ns_handle);
}
