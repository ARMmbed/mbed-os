/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "thread_nvm_store.h"
#include "ns_trace.h"
#include <string.h>
#include <stdio.h>
#include "ns_file_system.h"
#include "thread_config.h"

#define TRACE_GROUP "tnvm"
#define FAST_DATA_FILE "f_d"
#define FAST_DATA_WORD_SIZE 4
#define FAST_DATA_VERSION 1

static const char* thread_nvm_store_get_root_path(void);
static int root_path_valid();
static int thread_nvm_store_fast_data_read_version(uint32_t* version);
static int thread_nvm_store_fast_data_create(void);
static int thread_nvm_store_read(const char *file_name, void *data, uint32_t data_size);
static int thread_nvm_store_write(const char *file_name, void *data, uint32_t data_size);
static void create_fast_data_path (char* fast_data_path);
static int thread_nvm_store_fast_data_save(thread_nvm_fast_data_t* fast_data_to_set);
static int thread_nvm_store_all_counters_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t seq_counter);

#define MAX_ROOT_PATH_LEN 100

#define FAST_DATA_STRING_LEN (strlen(FAST_DATA_FILE)+strlen(thread_nvm_store_get_root_path())+1)

thread_nvm_fast_data_t cached_fast_data;

static const char* thread_nvm_store_get_root_path(void)
{
    char* path = ns_file_system_get_root_path();
    if (NULL==path) {
        return "";
    }
    return path;
}

static int root_path_valid() {
    if (NULL==thread_nvm_store_get_root_path())
        return 0;
    int path_len = strlen(thread_nvm_store_get_root_path());
    if(path_len==0 || path_len>MAX_ROOT_PATH_LEN) {
        return 0;
    }
    return 1;
}

int thread_nvm_store_init(void)
{
    uint32_t version=0;

    if (!root_path_valid()) {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }

    int ret = thread_nvm_store_fast_data_read_version(&version);
    if(THREAD_NVM_FILE_READ_ERROR==ret) {
        tr_info("need to create a new fastdata");
        return thread_nvm_store_fast_data_create();
    }

    if (version!=FAST_DATA_VERSION) {
        //handle new version format here
        //if version is different do the reformatting and save new format.....
        tr_info("Fast data version mismatch %u\n", (unsigned int)version);
    }
    return 0;
}

int thread_nvm_store_seq_counter_store(uint32_t network_seq_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if (cached_fast_data.seq_counter!=network_seq_counter) {
        ret = thread_nvm_store_all_counters_store(cached_fast_data.mac_frame_counter, cached_fast_data.mle_frame_counter, network_seq_counter);
        cached_fast_data.seq_counter=network_seq_counter;
    }
    return ret;
}

int thread_nvm_store_fast_data_check_and_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if( ((int)(mac_frame_counter - cached_fast_data.mac_frame_counter) > MAC_FRAME_COUNTER_LIMIT) ||
        ((int)(mle_frame_counter - cached_fast_data.mle_frame_counter) > MLE_FRAME_COUNTER_LIMIT) ||
        cached_fast_data.seq_counter!=network_seq_counter) {
            ret = thread_nvm_store_all_counters_store(mac_frame_counter, mle_frame_counter, network_seq_counter);
            cached_fast_data.mac_frame_counter = mac_frame_counter;
            cached_fast_data.mle_frame_counter = mle_frame_counter;
            cached_fast_data.seq_counter=network_seq_counter;
    }
    return ret;
}


int thread_nvm_store_frame_counters_check_and_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if( ((int)(mac_frame_counter - cached_fast_data.mac_frame_counter) > MAC_FRAME_COUNTER_LIMIT) ||
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
    fast_data.version = FAST_DATA_VERSION;
    if (root_path_valid()) {
        return thread_nvm_store_fast_data_save(&fast_data);
    }
    else{
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
}

int thread_nvm_store_fast_data_store(thread_nvm_fast_data_t* fast_data)
{
    cached_fast_data.mac_frame_counter = fast_data->mac_frame_counter;
    cached_fast_data.mle_frame_counter = fast_data->mle_frame_counter;
    cached_fast_data.seq_counter = fast_data->seq_counter;

    if (root_path_valid()) {
        fast_data->version=FAST_DATA_VERSION;
        return thread_nvm_store_fast_data_save(fast_data);
    }
    else {
        return THREAD_NVM_FILE_ROOT_PATH_INVALID;
    }
}

static void create_fast_data_path (char* fast_data_path)
{
    strcpy(fast_data_path, thread_nvm_store_get_root_path());
    strcat(fast_data_path, FAST_DATA_FILE);
}

static int thread_nvm_store_fast_data_read_version(uint32_t* version)
{
    thread_nvm_fast_data_t fast_data;
    char fast_data_path[FAST_DATA_STRING_LEN];
    create_fast_data_path(fast_data_path);

    int ret = thread_nvm_store_read(fast_data_path, &fast_data, FAST_DATA_WORD_SIZE*4);

    if(ret<0) {
        return THREAD_NVM_FILE_READ_ERROR;
    }
    *version = fast_data.version;
    return THREAD_NVM_FILE_SUCCESS;
}

static int thread_nvm_store_fast_data_create(void)
{
    thread_nvm_fast_data_t fast_data;
    memset(&fast_data, 0, FAST_DATA_WORD_SIZE*4);
    fast_data.version = FAST_DATA_VERSION;
    char fast_data_path[FAST_DATA_STRING_LEN];
    create_fast_data_path(fast_data_path);
    return thread_nvm_store_write(fast_data_path, &fast_data, FAST_DATA_WORD_SIZE*4);
}

int thread_nvm_store_fast_data_read(thread_nvm_fast_data_t* fast_data)
{
    int ret = THREAD_NVM_FILE_SUCCESS;
    if (root_path_valid()) {
        char fast_data_path[FAST_DATA_STRING_LEN];
        create_fast_data_path(fast_data_path);
        ret = thread_nvm_store_read(fast_data_path, fast_data, FAST_DATA_WORD_SIZE*4);
    }
    else {
        fast_data->mac_frame_counter = cached_fast_data.mac_frame_counter;
        fast_data->mle_frame_counter = cached_fast_data.mle_frame_counter;
        fast_data->seq_counter = cached_fast_data.seq_counter;
    }
    return ret;
}

static int thread_nvm_store_fast_data_save(thread_nvm_fast_data_t* fast_data_to_set)
{
    char fast_data_path[FAST_DATA_STRING_LEN];
    create_fast_data_path(fast_data_path);
    return thread_nvm_store_write(fast_data_path, fast_data_to_set, FAST_DATA_WORD_SIZE*4);
}

static int thread_nvm_store_write(const char *file_name, void *data, uint32_t data_size)
{
    FILE *fp = fopen(file_name, "w");
    if(fp == NULL) {
        return THREAD_NVM_FILE_CANNOT_OPEN;
    }
    size_t n_bytes = fwrite(data, 1, data_size, fp);
    fclose(fp);
    if (n_bytes!=data_size) {
        tr_error("NVM write failed");
        return THREAD_NVM_FILE_WRITE_ERROR;
    }
    else {
        return THREAD_NVM_FILE_SUCCESS;
    }
}

// returns 0 when ok
static int thread_nvm_store_read(const char *file_name, void *data, uint32_t data_size)
{
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        return THREAD_NVM_FILE_CANNOT_OPEN;
    }
    size_t n_bytes = fread(data, 1, data_size, fp);
    fclose(fp);
    if (n_bytes!=data_size) {
        tr_error("NVM read failed");
        return THREAD_NVM_FILE_READ_ERROR;
    }
    else {
        return THREAD_NVM_FILE_SUCCESS; // return how many bytes was written.
    }
}
