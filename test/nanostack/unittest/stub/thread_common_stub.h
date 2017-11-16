/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#ifndef __THREAD_COMMON_STUB_H__
#define __THREAD_COMMON_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"
#include "thread_common.h"

typedef struct {
    bool bool_value;
    int int_value;
    int8_t int8_value;
    uint8_t uint8_value;
    uint8_t *uint8_ptr;
    uint16_t uint16_value;
    thread_parent_info_t *info_ptr;
    thread_leader_data_t *data_ptr;
    thread_pending_child_id_req_t *req_ptr;
} thread_stub_def;

extern thread_stub_def thread_stub;

#ifdef __cplusplus
}
#endif

#endif // __THREAD_COMMON_STUB_H__
