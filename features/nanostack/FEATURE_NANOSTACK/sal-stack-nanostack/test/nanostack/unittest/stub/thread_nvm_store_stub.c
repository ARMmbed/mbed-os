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

static const char* thread_nvm_store_get_root_path(void)
{    
    return "";    
}

int thread_nvm_store_init(void)
{
    
    return 0;
}


int thread_nvm_store_fast_data_read(thread_nvm_fast_data_t* fast_data)
{
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_fast_data_store(thread_nvm_fast_data_t* fast_data)
{
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_seq_counter_store(uint32_t network_seq_counter)
{
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_frame_counters_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter)
{
    return THREAD_NVM_FILE_SUCCESS;
}

int thread_nvm_store_fast_data_check_and_store(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter)
{
    return THREAD_NVM_FILE_SUCCESS;
}
