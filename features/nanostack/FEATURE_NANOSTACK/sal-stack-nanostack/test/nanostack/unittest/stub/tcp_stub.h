/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#ifndef __TCP_STUB_H__
#define __TCP_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "Core/include/ns_buffer.h"
#include "socket.h"
#include "tcp.h"

typedef struct {
    int8_t int8_value;
    uint16_t uint16_value;
    tcp_error tcp_error_value;
    tcp_session_t *tcp_session_ptr;
    char *state;
    buffer_t *buffer_ptr;
} tcp_stub_data_t;

extern tcp_stub_data_t tcp_stub;


#ifdef __cplusplus
}
#endif

#endif //__SOCKET_STUB_H__
