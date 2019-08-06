/*
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

#include "nsconfig.h"
#include <string.h>
#include <stdio.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "6LoWPAN/ws/ws_config.h"
#include "ns_file_system.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsnv"

#define MAX_ROOT_PATH_LEN                200

static uint16_t ws_pae_nvm_store_path_len_get(const char *file_name);
static const char *ws_pae_nvm_store_get_root_path(void);
static int8_t ws_pae_nvm_store_root_path_valid(void);
static int8_t ws_pae_nvm_store_create_path(char *fast_data_path, const char *file_name);
static int8_t ws_pae_nvm_store_write(const char *file_name, nvm_tlv_list_t *tlv_list);
static int8_t ws_pae_nvm_store_read(const char *file_name, nvm_tlv_list_t *tlv_list);

int8_t ws_pae_nvm_store_tlv_file_write(const char *file, nvm_tlv_list_t *tlv_list)
{
    if (!ws_pae_nvm_store_root_path_valid()) {
        return PAE_NVM_FILE_ROOT_PATH_INVALID;
    }

    uint16_t path_len = ws_pae_nvm_store_path_len_get(file);

    char nw_info_path[path_len];

    ws_pae_nvm_store_create_path(nw_info_path, file);

    return ws_pae_nvm_store_write(nw_info_path, tlv_list);
}

int8_t ws_pae_nvm_store_tlv_file_read(const char *file, nvm_tlv_list_t *tlv_list)
{
    if (!ws_pae_nvm_store_root_path_valid()) {
        return PAE_NVM_FILE_ROOT_PATH_INVALID;
    }

    uint16_t path_len = ws_pae_nvm_store_path_len_get(file);

    char nw_info_path[path_len];

    ws_pae_nvm_store_create_path(nw_info_path, file);

    return ws_pae_nvm_store_read(nw_info_path, tlv_list);
}

static const char *ws_pae_nvm_store_get_root_path(void)
{
    char *path = ns_file_system_get_root_path();

    if (NULL == path) {
        return "";
    }
    return path;
}

static int8_t ws_pae_nvm_store_root_path_valid(void)
{
    if (NULL == ns_file_system_get_root_path()) {
        return 0;
    }
    int path_len = strlen(ws_pae_nvm_store_get_root_path());
    if (path_len == 0 || path_len > MAX_ROOT_PATH_LEN) {
        return 0;
    }
    return 1;
}

static uint16_t ws_pae_nvm_store_path_len_get(const char *file_name)
{
    return strlen(file_name) + strlen(ws_pae_nvm_store_get_root_path()) + 1;
}

static int8_t ws_pae_nvm_store_create_path(char *data_path, const char *file_name)
{
    strcpy(data_path, ws_pae_nvm_store_get_root_path());
    strcat(data_path, file_name);
    return 0;
}

static int8_t ws_pae_nvm_store_write(const char *file_name, nvm_tlv_list_t *tlv_list)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        tr_error("NVM open error: %s", file_name);
        return PAE_NVM_FILE_CANNOT_OPEN;
    }

    uint16_t list_count = ns_list_count(tlv_list);
    size_t n_bytes = fwrite(&list_count, 1, sizeof(uint16_t), fp);
    if (n_bytes != sizeof(uint16_t)) {
        tr_warning("NVM TLV list count write error");
        fclose(fp);
        return PAE_NVM_FILE_WRITE_ERROR;
    }

    bool failure = false;

    ns_list_foreach(nvm_tlv_entry_t, entry, tlv_list) {
        n_bytes = fwrite(&entry->tag, 1, entry->len + NVM_TLV_FIXED_LEN, fp);
        if (n_bytes != (size_t) entry->len + NVM_TLV_FIXED_LEN) {
            failure = true;
            break;
        }
    }

    fclose(fp);
    if (failure) {
        tr_error("NVM write error %s", file_name);
        return PAE_NVM_FILE_WRITE_ERROR;
    } else {
        return PAE_NVM_FILE_SUCCESS;
    }
}

static int8_t ws_pae_nvm_store_read(const char *file_name, nvm_tlv_list_t *tlv_list)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        tr_warning("File not found: %s", file_name);
        return PAE_NVM_FILE_CANNOT_OPEN;
    }

    uint16_t list_count;
    size_t n_bytes = fread(&list_count, 1, sizeof(uint16_t), fp);
    if (n_bytes != sizeof(uint16_t)) {
        tr_warning("NVM TLV list count read error %s", file_name);
        fclose(fp);
        return PAE_NVM_FILE_READ_ERROR;
    }

    bool failure = false;

    while (list_count-- > 0) {
        nvm_tlv_entry_t entry_header;
        memset(&entry_header, 0, sizeof(nvm_tlv_entry_t));
        n_bytes = fread(&entry_header.tag, 1, NVM_TLV_FIXED_LEN, fp);
        if (n_bytes != NVM_TLV_FIXED_LEN) {
            failure = true;
            break;
        }
        uint16_t len = entry_header.len;

        nvm_tlv_entry_t *entry = ns_dyn_mem_temporary_alloc(sizeof(nvm_tlv_entry_t) + len);
        if (!entry) {
            failure = true;
            break;
        }

        memcpy(&entry->tag, &entry_header.tag, NVM_TLV_FIXED_LEN);

        if (len > 0) {
            uint8_t *data_ptr = ((uint8_t *)&entry->tag) + NVM_TLV_FIXED_LEN;
            n_bytes = fread(data_ptr, 1, len, fp);
            if (n_bytes != len) {
                ns_dyn_mem_free(entry);
                failure = true;
                break;
            }
        }

        ns_list_add_to_end(tlv_list, entry);
    }

    fclose(fp);

    if (failure) {
        ns_list_foreach_safe(nvm_tlv_entry_t, entry, tlv_list) {
            ns_list_remove(tlv_list, entry);
            ns_dyn_mem_free(entry);
        }
        tr_error("NVM read error %s", file_name);
        return PAE_NVM_FILE_READ_ERROR;
    } else {
        return PAE_NVM_FILE_SUCCESS; // return how many bytes was written.
    }
}


#endif /* HAVE_WS */

