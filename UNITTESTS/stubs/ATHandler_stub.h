/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "stdint.h"
#include "stdbool.h"
#include <cstddef>
#include "nsapi_types.h"
#include "ATHandler.h"
#include "FileHandle_stub.h"
#include "Callback.h"
#include "vector"

#ifndef __AT_HANDLER_STUB_H__
#define __AT_HANDLER_STUB_H__

static const int kRead_string_table_size = 100;
static const int kRead_int_table_size = 100;
static const int kResp_stop_count_default = 100;
static const int kATHandler_urc_table_max_size = 10;
static const int kATHandler_urc_string_max_size = 16;

namespace ATHandler_stub {
extern nsapi_error_t nsapi_error_value;
extern uint8_t nsapi_error_ok_counter;
extern int int_value;
extern int timeout;
extern bool default_timeout;
extern bool debug_on;
extern ssize_t ssize_value;
extern const char *read_string_value;
extern size_t size_value;
extern size_t return_given_size;
extern bool bool_value;
extern uint8_t resp_info_true_counter;
extern uint8_t resp_info_true_counter2;
extern uint8_t resp_info_false_counter;
extern uint8_t info_elem_true_counter;
extern uint8_t uint8_value;
extern mbed::FileHandle_stub *fh_value;
extern mbed::device_err_t device_err_value;
extern bool call_immediately;
extern const char *read_string_table[kRead_string_table_size];
extern int read_string_index;
extern int int_valid_count_table[kRead_int_table_size];
extern int int_count;
extern int resp_stop_success_count;
extern bool process_oob_urc;

struct urc_handler {
    const char *urc;
    mbed::Callback<void()> cb;
};
extern std::vector<urc_handler> urc_handlers;

extern bool get_debug_flag;
bool is_get_debug_run();
void get_debug_clear();
extern uint8_t set_debug_call_count;
uint8_t set_debug_call_count_get();
void debug_call_count_clear();
}

#endif
