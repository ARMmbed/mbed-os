/*
 * Copyright (c) 2019-2020, Pelion and affiliates.
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
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "6LoWPAN/ws/ws_config.h"
#include "ns_file_system.h"
#include "Service_Libs/utils/ns_file.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "ns_file_system.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsnv"

#define MAX_ROOT_PATH_LEN                200

static uint16_t ws_pae_nvm_store_path_len_get(const char *file_name);
static const char *ws_pae_nvm_store_get_root_path(void);
static int8_t ws_pae_nvm_store_root_path_valid(void);
static int8_t ws_pae_nvm_store_create_path(char *fast_data_path, const char *file_name);
static int8_t ws_pae_nvm_store_write(const char *file_name, nvm_tlv_t *tlv);
static int8_t ws_pae_nvm_store_read(const char *file_name, nvm_tlv_t *tlv);

int8_t ws_pae_nvm_store_tlv_file_write(const char *file, nvm_tlv_t *tlv)
{
    if (!ws_pae_nvm_store_root_path_valid()) {
        return PAE_NVM_FILE_ROOT_PATH_INVALID;
    }

    uint16_t path_len = ws_pae_nvm_store_path_len_get(file);

    char nw_info_path[path_len];

    ws_pae_nvm_store_create_path(nw_info_path, file);

    return ws_pae_nvm_store_write(nw_info_path, tlv);
}

int8_t ws_pae_nvm_store_tlv_file_read(const char *file, nvm_tlv_t *tlv)
{
    if (!ws_pae_nvm_store_root_path_valid()) {
        return PAE_NVM_FILE_ROOT_PATH_INVALID;
    }

    uint16_t path_len = ws_pae_nvm_store_path_len_get(file);

    char nw_info_path[path_len];

    ws_pae_nvm_store_create_path(nw_info_path, file);

    return ws_pae_nvm_store_read(nw_info_path, tlv);
}

int8_t ws_pae_nvm_store_tlv_file_remove(const char *file)
{
    if (!ws_pae_nvm_store_root_path_valid()) {
        return PAE_NVM_FILE_ROOT_PATH_INVALID;
    }

    uint16_t path_len = ws_pae_nvm_store_path_len_get(file);

    char nw_info_path[path_len];

    ws_pae_nvm_store_create_path(nw_info_path, file);

    int ret = ns_fremove(nw_info_path);
    if (ret < 0) {
        return -1;
    }

    return 0;
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

static int8_t ws_pae_nvm_store_write(const char *file_name, nvm_tlv_t *tlv)
{
    if (!file_name || !tlv) {
        return -1;
    }

    NS_FILE *fp = ns_fopen(file_name, "w");
    if (fp == NULL) {
        tr_error("NVM open error: %s", file_name);
        return PAE_NVM_FILE_CANNOT_OPEN;
    }

    size_t n_bytes = ns_fwrite(fp, tlv, tlv->len + sizeof(nvm_tlv_t));
    ns_fclose(fp);
    if (n_bytes != tlv->len + sizeof(nvm_tlv_t)) {
        tr_error("NVM write error %s", file_name);
        return PAE_NVM_FILE_WRITE_ERROR;
    }

    return PAE_NVM_FILE_SUCCESS;
}

static int8_t ws_pae_nvm_store_read(const char *file_name, nvm_tlv_t *tlv)
{
    if (!file_name || !tlv) {
        return -1;
    }

    NS_FILE *fp = ns_fopen(file_name, "r");
    if (fp == NULL) {
        tr_warning("File not found: %s", file_name);
        return PAE_NVM_FILE_CANNOT_OPEN;
    }

    size_t n_bytes = ns_fread(fp, tlv, tlv->len + sizeof(nvm_tlv_t));
    ns_fclose(fp);
    if (n_bytes != tlv->len + sizeof(nvm_tlv_t)) {
        tr_warning("File not found or cannot be read: %s", file_name);
        return PAE_NVM_FILE_READ_ERROR;
    }

    return PAE_NVM_FILE_SUCCESS;
}

#endif /* HAVE_WS */

