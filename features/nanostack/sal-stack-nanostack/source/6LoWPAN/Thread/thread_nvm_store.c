/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * \file thread_nvm_store.c
 *
 */

#include "nsconfig.h"

#include <string.h>
#include <stdio.h>
#include "Core/include/ns_address_internal.h"
#include "ns_file_system.h"
#include "thread_config.h"
#include "thread_common.h"
#include "thread_nvm_store.h"
#include "ns_trace.h"

#define TRACE_GROUP "tnvm"
const char *FAST_DATA_FILE = "f_d";
#define FAST_DATA_VERSION 1
#define LINK_INFO_WRITE_DELAY 2
#define LINK_INFO_SHORT_ADDR_NOT_SET 0xffff
#define LINK_INFO_WRITE_DONE 0xffff

const char *LINK_INFO_FILE = "l_i";
#define LINK_INFO_DATA_VERSION 1

const char *LEADER_INFO_FILE = "ld_i";
#define LEADER_INFO_DATA_VERSION 1

typedef struct {
    uint8_t mac[8];
    uint16_t short_addr;
} nvm_link_info_t;

typedef struct {
    nvm_link_info_t nvm_link_info;
    uint16_t write_delay;
    bool loaded;
} thread_nvm_store_link_info_t;

const char *THREAD_NVM_ACTIVE_CONF_FILE = "a_c";
#define ACTIVE_CONF_DATA_VERSION 1

const char *DEVICE_CONF_FILE = "s_d";
#define DEVICE_CONF_VERSION 1

const char *THREAD_NVM_PENDING_CONF_FILE = "p_c";
#define PENDING_CONF_DATA_VERSION 1

static const char *thread_nvm_store_get_root_path(void);
static int root_path_valid(void);
static int thread_nvm_store_read(const char *file_name, void *data, uint32_t data_size, uint32_t *version);
static int thread_nvm_store_write(const char *file_name, void *data, uint32_t data_size, uint32_t version);
static void thread_nvm_store_create_path(char *fast_data_path, const char *file_name);
static int thread_nvm_store_fast_data_save(thread_nvm_fast_data_t *fast_data_to_set);
static int thread_nvm_store_all_counters_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t seq_counter);
static void thread_nvm_store_link_info_delayed_write(uint32_t seconds);

#define MAX_ROOT_PATH_LEN 200

#define FAST_DATA_STRING_LEN (strlen(FAST_DATA_FILE)+strlen(thread_nvm_store_get_root_path())+1)
#define ACTIVE_CONF_STRING_LEN (strlen(THREAD_NVM_ACTIVE_CONF_FILE)+strlen(thread_nvm_store_get_root_path())+1)
#define DEVICE_CONF_STRING_LEN (strlen(DEVICE_CONF_FILE)+strlen(thread_nvm_store_get_root_path())+1)
#define PENDING_CONF_STRING_LEN (strlen(THREAD_NVM_PENDING_CONF_FILE)+strlen(thread_nvm_store_get_root_path())+1)
#define LINK_INFO_STRING_LEN (strlen(LINK_INFO_FILE)+strlen(thread_nvm_store_get_root_path())+1)
#define LEADER_INFO_STRING_LEN (strlen(LEADER_INFO_FILE)+strlen(thread_nvm_store_get_root_path())+1)


thread_nvm_fast_data_t cached_fast_data;
thread_nvm_store_link_info_t cached_link_info = {
    .nvm_link_info.short_addr = LINK_INFO_SHORT_ADDR_NOT_SET,
    .nvm_link_info.mac = {0, 0, 0, 0, 0, 0, 0, 0},
    .write_delay = LINK_INFO_WRITE_DELAY,
    .loaded = false
};

static const char *thread_nvm_store_get_root_path(void)
{
    char *path = ns_file_system_get_root_path();
    if (NULL == path) {
        return "";
    }
    return path;
}

static int root_path_valid(void)
{
    if (NULL == ns_file_system_get_root_path()) {
        return 0;
    }
    int path_len = strlen(thread_nvm_store_get_root_path());
    if (path_len == 0 || path_len > MAX_ROOT_PATH_LEN) {
        return 0;
    }
    return 1;
}

int thread_nvm_store_mleid_rloc_map_write(thread_nvm_mleid_rloc_map *mleid_rloc_map)
{
    char lc_data_path[LEADER_INFO_STRING_LEN];
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    thread_nvm_store_create_path(lc_data_path, LEADER_INFO_FILE);
    tr_debug("writing to store rloc mapping info");
    return thread_nvm_store_write(lc_data_path, mleid_rloc_map, sizeof(thread_nvm_mleid_rloc_map), LEADER_INFO_DATA_VERSION);
}

int thread_nvm_store_mleid_rloc_map_read(thread_nvm_mleid_rloc_map *mleid_rloc_map)
{
    char lc_data_path[LEADER_INFO_STRING_LEN];
    uint32_t version;
    if (NULL == mleid_rloc_map) {
        return THREAD_NVM_FILE_PARAMETER_INVALID;
    }
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    thread_nvm_store_create_path(lc_data_path, LEADER_INFO_FILE);

    int ret = thread_nvm_store_read(lc_data_path, mleid_rloc_map, sizeof(thread_nvm_mleid_rloc_map), &version);

    if (THREAD_NVM_FILE_SUCCESS != ret) {
        tr_info("Leader data map read failed");
        thread_nvm_store_mleid_rloc_map_remove();
        return ret;
    }

    if (LEADER_INFO_DATA_VERSION != version) {
        tr_info("Leader data map version mismatch %"PRIu32, version);
        thread_nvm_store_mleid_rloc_map_remove();
        return THREAD_NVM_FILE_VERSION_WRONG;
    }

    return ret;
}

int thread_nvm_store_mleid_rloc_map_remove(void)
{
    int status;
    tr_info("thread_nvm_store_leader_info_remove");

    if (!ns_file_system_get_root_path()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }

    char lc_data_path[LEADER_INFO_STRING_LEN];
    thread_nvm_store_create_path(lc_data_path, LEADER_INFO_FILE);
    status = remove(lc_data_path);
    if (status != 0) {
        return THREAD_NVM_FILE_REMOVE_ERROR;
    }
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_device_configuration_write(uint8_t *mac_ptr, uint8_t *mleid_ptr)
{
    thread_nvm_device_conf_t d_c;
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    memcpy(d_c.mac, mac_ptr, sizeof(d_c.mac));
    memcpy(d_c.mle_id, mleid_ptr, sizeof(d_c.mle_id));
    char device_conf_path[DEVICE_CONF_STRING_LEN];
    thread_nvm_store_create_path(device_conf_path, DEVICE_CONF_FILE);
    return thread_nvm_store_write(device_conf_path, &d_c, sizeof(thread_nvm_device_conf_t), DEVICE_CONF_VERSION);
}

int thread_nvm_store_device_configuration_read(uint8_t *mac_ptr, uint8_t *mleid_ptr)
{
    int ret = THREAD_NVM_FILE_READ_ERROR;
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    char device_conf_path[DEVICE_CONF_STRING_LEN];
    thread_nvm_store_create_path(device_conf_path, DEVICE_CONF_FILE);
    uint32_t version;
    thread_nvm_device_conf_t d_c;

    ret = thread_nvm_store_read(device_conf_path, &d_c, sizeof(thread_nvm_device_conf_t), &version);
    if (THREAD_NVM_FILE_SUCCESS == ret) {
        if (THREAD_NVM_FILE_SUCCESS == ret && DEVICE_CONF_VERSION != version) {
            tr_info("fast data version mismatch %"PRIu32, version);
            ret = THREAD_NVM_FILE_VERSION_WRONG;
        } else {
            memcpy(mac_ptr, d_c.mac, sizeof(d_c.mac));
            memcpy(mleid_ptr, d_c.mle_id, sizeof(d_c.mle_id));
        }
    }
    return ret;
}

int thread_nvm_store_pending_configuration_write(void *data, uint16_t size)
{
    char pc_data_path[PENDING_CONF_STRING_LEN];
    if (NULL == data) {
        return THREAD_NVM_FILE_PARAMETER_INVALID;
    }
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    thread_nvm_store_create_path(pc_data_path, THREAD_NVM_PENDING_CONF_FILE);
    return thread_nvm_store_write(pc_data_path, data, size, PENDING_CONF_DATA_VERSION);
}

int thread_nvm_store_pending_configuration_read(void *data, uint16_t size)
{
    char pc_data_path[PENDING_CONF_STRING_LEN];
    uint32_t version;
    if (NULL == data) {
        return THREAD_NVM_FILE_PARAMETER_INVALID;
    }
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    thread_nvm_store_create_path(pc_data_path, THREAD_NVM_PENDING_CONF_FILE);

    int ret = thread_nvm_store_read(pc_data_path, data, size, &version);
    if (THREAD_NVM_FILE_SUCCESS == ret && PENDING_CONF_DATA_VERSION != version) {
        tr_info("Pending configuration version mismatch %"PRIu32, version);
        return THREAD_NVM_FILE_VERSION_WRONG;
    }
    return ret;
}

int thread_nvm_store_active_configuration_write(void *data, uint16_t data_size)
{
    char ac_data_path[ACTIVE_CONF_STRING_LEN];
    if (NULL == data) {
        return THREAD_NVM_FILE_PARAMETER_INVALID;
    }
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }

    thread_nvm_store_create_path(ac_data_path, THREAD_NVM_ACTIVE_CONF_FILE);
    return thread_nvm_store_write(ac_data_path, data, data_size, ACTIVE_CONF_DATA_VERSION);
}

int thread_nvm_store_active_configuration_read(void *data, uint16_t data_size)
{
    char ac_data_path[ACTIVE_CONF_STRING_LEN];
    uint32_t version;
    if (NULL == data) {
        return THREAD_NVM_FILE_PARAMETER_INVALID;
    }
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    thread_nvm_store_create_path(ac_data_path, THREAD_NVM_ACTIVE_CONF_FILE);

    int ret = thread_nvm_store_read(ac_data_path, data, data_size, &version);
    if (THREAD_NVM_FILE_SUCCESS == ret && ACTIVE_CONF_DATA_VERSION != version) {
        tr_info("active configuration version mismatch %"PRIu32, version);
        return THREAD_NVM_FILE_VERSION_WRONG;
    }
    return ret;
}

int thread_nvm_store_active_configuration_remove(void)
{
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    char ac_data_path[ACTIVE_CONF_STRING_LEN];
    thread_nvm_store_create_path(ac_data_path, THREAD_NVM_ACTIVE_CONF_FILE);
    int status = remove(ac_data_path);
    if (status != 0) {
        return THREAD_NVM_FILE_REMOVE_ERROR;
    }
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_pending_configuration_remove(void)
{
    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
    char ac_data_path[PENDING_CONF_STRING_LEN];
    thread_nvm_store_create_path(ac_data_path, THREAD_NVM_PENDING_CONF_FILE);
    int status = remove(ac_data_path);
    if (status != 0) {
        return THREAD_NVM_FILE_REMOVE_ERROR;
    }
    return THREAD_NVM_FILE_SUCCESS;
}


int thread_nvm_store_seq_counter_write(uint32_t network_seq_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if (cached_fast_data.seq_counter != network_seq_counter) {
        ret = thread_nvm_store_all_counters_store(cached_fast_data.mac_frame_counter, cached_fast_data.mle_frame_counter, network_seq_counter);
        cached_fast_data.seq_counter = network_seq_counter;
    }
    return ret;
}

int thread_nvm_store_fast_data_check_and_write(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if (((int)(mac_frame_counter - cached_fast_data.mac_frame_counter) > MAC_FRAME_COUNTER_LIMIT) ||
            ((int)(mle_frame_counter - cached_fast_data.mle_frame_counter) > MLE_FRAME_COUNTER_LIMIT) ||
            cached_fast_data.seq_counter != network_seq_counter) {
        ret = thread_nvm_store_all_counters_store(mac_frame_counter, mle_frame_counter, network_seq_counter);
        cached_fast_data.mac_frame_counter = mac_frame_counter;
        cached_fast_data.mle_frame_counter = mle_frame_counter;
        cached_fast_data.seq_counter = network_seq_counter;
    }
    return ret;
}

int thread_nvm_store_fast_data_write_all(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter)
{
    int ret;
    ret = thread_nvm_store_all_counters_store(mac_frame_counter, mle_frame_counter, network_seq_counter);
    cached_fast_data.mac_frame_counter = mac_frame_counter;
    cached_fast_data.mle_frame_counter = mle_frame_counter;
    cached_fast_data.seq_counter = network_seq_counter;
    return ret;
}

int thread_nvm_store_frame_counters_check_and_write(uint32_t mac_frame_counter, uint32_t mle_frame_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if (((int)(mac_frame_counter - cached_fast_data.mac_frame_counter) > MAC_FRAME_COUNTER_LIMIT) ||
            ((int)(mle_frame_counter - cached_fast_data.mle_frame_counter) > MLE_FRAME_COUNTER_LIMIT)) {
        ret = thread_nvm_store_all_counters_store(mac_frame_counter, mle_frame_counter, cached_fast_data.seq_counter);
        cached_fast_data.mac_frame_counter = mac_frame_counter;
        cached_fast_data.mle_frame_counter = mle_frame_counter;
    }
    return ret;
}

static int thread_nvm_store_all_counters_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter)
{
    thread_nvm_fast_data_t fast_data;
    fast_data.mac_frame_counter = mac_frame_counter;
    fast_data.mle_frame_counter = mle_frame_counter;
    fast_data.seq_counter = network_seq_counter;
    if (root_path_valid()) {
        return thread_nvm_store_fast_data_save(&fast_data);
    } else {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
}

int thread_nvm_store_fast_data_write(thread_nvm_fast_data_t *fast_data)
{
    cached_fast_data.mac_frame_counter = fast_data->mac_frame_counter;
    cached_fast_data.mle_frame_counter = fast_data->mle_frame_counter;
    cached_fast_data.seq_counter = fast_data->seq_counter;

    if (root_path_valid()) {
        return thread_nvm_store_fast_data_save(fast_data);
    } else {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
}

static void thread_nvm_store_create_path(char *fast_data_path, const char *file_name)
{
    strcpy(fast_data_path, thread_nvm_store_get_root_path());
    strcat(fast_data_path, file_name);
}

int thread_nvm_store_fast_data_read(thread_nvm_fast_data_t *fast_data)
{
    int ret = THREAD_NVM_FILE_SUCCESS;

    if (root_path_valid()) {
        char fast_data_path[FAST_DATA_STRING_LEN];
        thread_nvm_store_create_path(fast_data_path, FAST_DATA_FILE);
        uint32_t version;
        ret = thread_nvm_store_read(fast_data_path, fast_data, sizeof(thread_nvm_fast_data_t), &version);
        if (THREAD_NVM_FILE_SUCCESS == ret && FAST_DATA_VERSION != version) {
            tr_info("fast data version mismatch %"PRIu32, version);
            return THREAD_NVM_FILE_VERSION_WRONG;
        }
    } else {
        fast_data->mac_frame_counter = cached_fast_data.mac_frame_counter;
        fast_data->mle_frame_counter = cached_fast_data.mle_frame_counter;
        fast_data->seq_counter = cached_fast_data.seq_counter;
    }
    return ret;
}

static int thread_nvm_store_fast_data_save(thread_nvm_fast_data_t *fast_data_to_set)
{
    char fast_data_path[FAST_DATA_STRING_LEN];
    thread_nvm_store_create_path(fast_data_path, FAST_DATA_FILE);
    return thread_nvm_store_write(fast_data_path, fast_data_to_set, sizeof(thread_nvm_fast_data_t), FAST_DATA_VERSION);
}

static int thread_nvm_store_write(const char *file_name, void *data, uint32_t data_size, uint32_t version)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        tr_error("NVM open error: %s", file_name);
        return THREAD_NVM_FILE_CANNOT_OPEN;
    }

    size_t n_bytes = fwrite(&version, 1, sizeof(uint32_t), fp);
    if (n_bytes != sizeof(uint32_t)) {
        tr_warning("NVM version write error");
        fclose(fp);
        return THREAD_NVM_FILE_WRITE_ERROR;
    }

    n_bytes = fwrite(data, 1, data_size, fp);
    fclose(fp);
    if (n_bytes != data_size) {
        tr_error("NVM write error %s", file_name);
        return THREAD_NVM_FILE_WRITE_ERROR;
    } else {
        return THREAD_NVM_FILE_SUCCESS;
    }
}

// returns 0 when ok
static int thread_nvm_store_read(const char *file_name, void *data, uint32_t data_size, uint32_t *version)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        tr_warning("File not found: %s", file_name);
        return THREAD_NVM_FILE_CANNOT_OPEN;
    }

    size_t n_bytes = fread(version, 1, sizeof(uint32_t), fp);
    if (n_bytes != sizeof(uint32_t)) {
        tr_warning("NVM version read error %s", file_name);
        fclose(fp);
        return THREAD_NVM_FILE_READ_ERROR;
    }

    n_bytes = fread(data, 1, data_size, fp);
    fclose(fp);
    if (n_bytes != data_size) {
        tr_error("NVM read error %s", file_name);
        return THREAD_NVM_FILE_READ_ERROR;
    } else {
        return THREAD_NVM_FILE_SUCCESS; // return how many bytes was written.
    }
}

int thread_nvm_store_link_info_read(void)
{
    nvm_link_info_t nvm_link_info_tmp;
    int status;

    if (!ns_file_system_get_root_path()) {
        if (!memcmp(cached_link_info.nvm_link_info.mac, ADDR_UNSPECIFIED, 8) &&
                cached_link_info.nvm_link_info.short_addr == LINK_INFO_SHORT_ADDR_NOT_SET) {
            tr_info("link info not cached");
            return THREAD_NVM_FILE_READ_ERROR;
        }
    }
    cached_link_info.loaded = true;
    char link_info_path[LINK_INFO_STRING_LEN];
    strcpy(link_info_path, thread_nvm_store_get_root_path());
    strcat(link_info_path, LINK_INFO_FILE);

    uint32_t version = 0;
    status = thread_nvm_store_read(link_info_path, &nvm_link_info_tmp, sizeof(nvm_link_info_t), &version);

    if (status != THREAD_NVM_FILE_SUCCESS) {
        if (!memcmp(cached_link_info.nvm_link_info.mac, ADDR_UNSPECIFIED, 8) &&
                cached_link_info.nvm_link_info.short_addr == LINK_INFO_SHORT_ADDR_NOT_SET) {
            tr_info("link info not cached and read error %d", status);
            cached_link_info.loaded = false;
            return THREAD_NVM_FILE_READ_ERROR;
        }
        return status;
    } else if (ACTIVE_CONF_DATA_VERSION != version) {
        tr_info("link info version mismatch %"PRIu32, version);
        return THREAD_NVM_FILE_VERSION_WRONG;
    }
    memcpy(cached_link_info.nvm_link_info.mac, nvm_link_info_tmp.mac, 8);
    cached_link_info.nvm_link_info.short_addr = nvm_link_info_tmp.short_addr;
    tr_info("info read: %s parent short addr: %"PRIu16, trace_array(cached_link_info.nvm_link_info.mac, 8), cached_link_info.nvm_link_info.short_addr);
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_link_info_get(uint8_t *parent_mac64, uint16_t *my_short_address)
{
    if (!memcmp(cached_link_info.nvm_link_info.mac, ADDR_UNSPECIFIED, 8) &&
            cached_link_info.nvm_link_info.short_addr == LINK_INFO_SHORT_ADDR_NOT_SET) {
        tr_info("thread_nvm_store_link_info_get addr zeros");
        return THREAD_NVM_FILE_READ_ERROR;
    }

    if (!cached_link_info.loaded) {
        return THREAD_NVM_FILE_READ_ERROR;
    }
    // read data from cache if cached data is available
    if (parent_mac64) {
        memcpy(parent_mac64, cached_link_info.nvm_link_info.mac, 8);
    }
    if (my_short_address) {
        *my_short_address = cached_link_info.nvm_link_info.short_addr;
    }
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_link_info_clear(void)
{
    int status;
    tr_info("thread_nvm_store_link_info_clear");
    memset(cached_link_info.nvm_link_info.mac, 0, 8);
    cached_link_info.nvm_link_info.short_addr = LINK_INFO_SHORT_ADDR_NOT_SET;

    cached_link_info.loaded = false;

    if (!ns_file_system_get_root_path()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }

    char link_info_path[LINK_INFO_STRING_LEN];
    strcpy(link_info_path, thread_nvm_store_get_root_path());
    strcat(link_info_path, LINK_INFO_FILE);

    status = remove(link_info_path);

    if (status != 0) {
        return THREAD_NVM_FILE_REMOVE_ERROR;
    }

    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_link_info_write(uint8_t *parent_mac, uint16_t short_addr)
{
    //tr_info("write mac: %s parent short addr: %"PRIu16, trace_array(parent_mac, 8), short_addr);
    if (parent_mac) {
        memcpy(cached_link_info.nvm_link_info.mac, parent_mac, 8);
    } else {
        memset(cached_link_info.nvm_link_info.mac, 0, 8);
        tr_info("setting to zero");
    }
    // when router parent is zeros, but my short address is the actual routing address.
    cached_link_info.nvm_link_info.short_addr = short_addr;

    if (cached_link_info.write_delay == LINK_INFO_WRITE_DONE) {
        cached_link_info.write_delay = LINK_INFO_WRITE_DELAY; // delay writing some seconds
    }

    if (!ns_file_system_get_root_path()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }

    return THREAD_NVM_FILE_SUCCESS;
}

static void thread_nvm_store_link_info_delayed_write(uint32_t seconds)
{
    if (cached_link_info.write_delay == LINK_INFO_WRITE_DONE) {
        return;
    } else if (cached_link_info.write_delay > seconds) {
        cached_link_info.write_delay -= seconds;
        return;
    }
    cached_link_info.write_delay = LINK_INFO_WRITE_DONE;

    if (!ns_file_system_get_root_path()) {
        return;
    }

    char link_info_path[LINK_INFO_STRING_LEN];
    strcpy(link_info_path, thread_nvm_store_get_root_path());
    strcat(link_info_path, LINK_INFO_FILE);
    tr_info("link info write parent mac: %s parent short addr: %"PRIu16, trace_array(cached_link_info.nvm_link_info.mac, 8), cached_link_info.nvm_link_info.short_addr);
    thread_nvm_store_write(link_info_path, &cached_link_info.nvm_link_info, sizeof(nvm_link_info_t), LINK_INFO_DATA_VERSION);
}

void thread_nvm_store_seconds_timer(uint32_t seconds)
{
    thread_nvm_store_link_info_delayed_write(seconds);
}
