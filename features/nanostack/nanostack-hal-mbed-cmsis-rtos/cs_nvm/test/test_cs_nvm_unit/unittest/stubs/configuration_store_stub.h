/*
 * Copyright (c) 2016, 2018, Arm Limited and affiliates.
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
#ifndef __CONFIGURATION_STORE_STUB_H__
#define __CONFIGURATION_STORE_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    int32_t ret_val;
    int32_t delete_ret_val;
    int32_t read_ret_val;
    int32_t close_ret_val;
    int32_t write_ret_val;
    ARM_CFSTORE_OPCODE cmd_code;
    void *client_context;
    ARM_CFSTORE_HANDLE handle;
    ARM_CFSTORE_CALLBACK callback;
} configuration_store_stub_data_t;

extern configuration_store_stub_data_t cfstore_stub;

void test_cfstore_callback_trigger(void);

#ifdef __cplusplus
}
#endif

#endif
