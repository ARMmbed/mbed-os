/*
 * Copyright (c) 2015, 2017, Arm Limited and affiliates.
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
#include "test_thread_nvm_store.h"
#include <string.h>
#include <stdio.h>
#include "6LoWPAN/Thread/thread_nvm_store.h"
#include "ns_file_system.h"
#include "thread_config.h"


const char *ROOT_FILE_PATH = "/tmp/";

bool test_thread_nvm_store_init()
{
    int ret = ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(ret)
        return 0;

    ret = thread_nvm_store_init();
    // this is for clearing memory
    ns_file_system_set_root_path(NULL);

    if(!ret)
        return 1;
    else
        return 0;
}

bool test_thread_nvm_store_init_failed()
{
    ns_file_system_set_root_path(NULL);
    int ret = thread_nvm_store_init();
    if (ret<0)
        return 1;

    return 0;
}

/** first stores data successfully and then read fails, but it should
 * return (write) cached values.
 *
 */
bool test_thread_nvm_store_init_failed_read()
{
    ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(thread_nvm_store_init()<0)
        return 0;

    thread_nvm_fast_data_t f_data;
    f_data.mac_frame_counter=2;
    f_data.mle_frame_counter=3;
    f_data.seq_counter=5;
    // data goes to write cache
    thread_nvm_store_fast_data_store(&f_data);
    ns_file_system_set_root_path(NULL);

    thread_nvm_fast_data_t f_data2;
    if(thread_nvm_store_fast_data_read(&f_data2)<0)
        return 0;
    if(f_data2.mac_frame_counter!=2)
        return 0;
    if(f_data2.seq_counter!=5)
        return 0;
    if(f_data2.mle_frame_counter!=3)
        return 0;

    return 1;
}

/** Storing fails, but read should
 * return (write) cached values.
 *
 */
bool test_thread_nvm_store_init_failed_write()
{
    ns_file_system_set_root_path(NULL);
    if(thread_nvm_store_init()>-1)
        return 0;

    if(thread_nvm_store_frame_counters_check_and_store(MAC_FRAME_COUNTER_LIMIT+7,MLE_FRAME_COUNTER_LIMIT+7)>-1)
        return 0;

    // using here different value, as the nvm data was not cleaned.
    if(thread_nvm_store_seq_counter_store(87)>-1)
        return 0;

    thread_nvm_fast_data_t f_data;
    thread_nvm_store_fast_data_read(&f_data);
    if(f_data.mac_frame_counter!=MAC_FRAME_COUNTER_LIMIT+7)
        return 0;
    if(f_data.seq_counter!=87)
        return 0;
    if(f_data.mle_frame_counter!=MLE_FRAME_COUNTER_LIMIT+7)
        return 0;

    return 1;
}

// tests seq counter. First stores to file and reads checks all values
// then stores only seq counter and reads it back and checks the value.
// After it stores all values and reads back and checks all values.
bool test_thread_nvm_store_seq_counter()
{
    ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(thread_nvm_store_init()<0)
        return 0;

    thread_nvm_fast_data_t f_data;
    f_data.mac_frame_counter=2;
    f_data.mle_frame_counter=3;
    f_data.seq_counter=5;
    thread_nvm_store_fast_data_store(&f_data);

    thread_nvm_fast_data_t f_data2;
    thread_nvm_store_fast_data_read(&f_data2);
    if(f_data2.mac_frame_counter!=2)
        return 0;
    if(f_data2.seq_counter!=5)
        return 0;
    if(f_data2.mle_frame_counter!=3)
        return 0;

    if(thread_nvm_store_seq_counter_store(7)<0)
        return 0;

    thread_nvm_store_fast_data_read(&f_data);

    if (f_data.seq_counter!=7)
        return 0;

    if(thread_nvm_store_fast_data_check_and_store(2,3,8)<0)
        return 0;

    thread_nvm_store_fast_data_read(&f_data);

    if(f_data.mac_frame_counter!=2)
        return 0;
    if(f_data.seq_counter!=8)
        return 0;
    if(f_data.mle_frame_counter!=3)
        return 0;
    // this is for clearing memory
    ns_file_system_set_root_path(NULL);

    return 1;
}

bool test_thread_nvm_store_mac_frame_counter()
{
    ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(thread_nvm_store_init()<0)
        return 0;

    thread_nvm_fast_data_t f_data;
    f_data.mac_frame_counter=2;
    f_data.mle_frame_counter=3;
    f_data.seq_counter=5;
    thread_nvm_store_fast_data_store(&f_data);

    thread_nvm_fast_data_t f_data2;
    thread_nvm_store_fast_data_read(&f_data2);
    if(f_data2.mac_frame_counter!=2)
        return 0;
    if(f_data2.seq_counter!=5)
        return 0;
    if(f_data2.mle_frame_counter!=3)
        return 0;

    thread_nvm_store_frame_counters_check_and_store(MAC_FRAME_COUNTER_LIMIT+17, 0);
    thread_nvm_store_fast_data_read(&f_data);
    // this is for clearing memory
    ns_file_system_set_root_path(NULL);
    if (f_data.mac_frame_counter==17+MAC_FRAME_COUNTER_LIMIT)
        return 1;

    return 0;
}

bool test_thread_nvm_store_mle_frame_counter()
{
    ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(thread_nvm_store_init()<0)
        return 0;

    thread_nvm_fast_data_t f_data;
    f_data.mac_frame_counter=2;
    f_data.mle_frame_counter=3;
    f_data.seq_counter=5;
    thread_nvm_store_fast_data_store(&f_data);

    thread_nvm_fast_data_t f_data2;
    thread_nvm_store_fast_data_read(&f_data2);
    if(f_data2.mac_frame_counter!=2)
        return 0;
    if(f_data2.seq_counter!=5)
        return 0;
    if(f_data2.mle_frame_counter!=3)
        return 0;

    thread_nvm_store_frame_counters_check_and_store(0, 77+MLE_FRAME_COUNTER_LIMIT);

    thread_nvm_store_fast_data_read(&f_data);
    // this is for clearing memory
    ns_file_system_set_root_path(NULL);
    if (f_data.mle_frame_counter==77+MLE_FRAME_COUNTER_LIMIT)
        return 1;

    return 0;
}

bool test_thread_nvm_store_read_cache()
{
    ns_file_system_set_root_path(ROOT_FILE_PATH);
    if(thread_nvm_store_init()<0)
        return 0;

    thread_nvm_fast_data_t f_data;
    f_data.mac_frame_counter=2;
    f_data.mle_frame_counter=3;
    f_data.seq_counter=5;
    thread_nvm_store_fast_data_store(&f_data);

    thread_nvm_fast_data_t f_data2;
    thread_nvm_store_fast_data_read(&f_data2);
    if(f_data2.mac_frame_counter!=2)
        return 0;
    if(f_data2.seq_counter!=5)
        return 0;
    if(f_data2.mle_frame_counter!=3)
        return 0;

    thread_nvm_store_frame_counters_check_and_store(10, 10);
    thread_nvm_store_fast_data_read(&f_data);
    // this is for clearing memory
    ns_file_system_set_root_path(NULL);

    if (f_data.mac_frame_counter!=2)
        return 0;
    if (f_data.mle_frame_counter!=3)
        return 0;
    if (f_data.seq_counter!=5)
        return 0;

    return 1;
}
