/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#ifndef __SOCKET_STUB_H__
#define __SOCKET_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Core/include/ns_buffer.h"
#include "Core/include/socket.h"

typedef struct {
    uint8_t socket_id;  // socket id returned when socket created
    error_t err;        // socket error returned by the socket call
    socket_t *socket_ptr;
    buffer_t *socket_read_buffer;
} socket_stub_data_t;

typedef struct {
    buffer_t *buffer_ptr;
    void *void_ptr;
    protocol_interface_info_entry_t *entry_ptr;
    socket_t *socket_structure;
    int8_t socket_read_cnt;
    inet_pcb_t *inet_pcb;
} socket_stub_def;

extern socket_stub_def socket_stub;

socket_stub_data_t *socket_stub_get_test_data(void);
void socket_stub_clear_sockets();
void socket_stub_setup(void);
void socket_stub_teardown(void);

#ifdef __cplusplus
}
#endif

#endif //__SOCKET_STUB_H__
