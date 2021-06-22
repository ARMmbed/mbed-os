/*
 * Copyright (c) 2015-2017, Pelion and affiliates.
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
#ifndef __SOCKET_API_STUB_H__
#define __SOCKET_API_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
    void (*recv_cb)(void *);
    int8_t int8_value;
    int counter;
    int values[10];
} socket_api_stub_data_t;

extern socket_api_stub_data_t socket_api_stub;

#ifdef __cplusplus
}
#endif

#endif //__SOCKET_API_STUB_H__





