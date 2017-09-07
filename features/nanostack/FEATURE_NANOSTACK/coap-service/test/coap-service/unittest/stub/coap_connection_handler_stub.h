/*
 * Copyright (c) 2015-2017 ARM Limited. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __COAP_CONNECTION_HANDLER_STUB_H__
#define __COAP_CONNECTION_HANDLER_STUB_H__

#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "coap_connection_handler.h"

typedef struct {
    int int_value;
    bool bool_value;
    coap_conn_handler_t *handler_obj;

    int (*send_to_sock_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, const void *, int);
    int (*receive_from_sock_cb)(int8_t socket_id, uint8_t src_address[static 16], uint16_t port, const uint8_t dst_address[static 16], unsigned char *data, int len);
    int (*get_passwd_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t *pw_ptr, uint8_t *pw_len);
    void (*sec_done_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40]);

} thread_conn_handler_stub_def;

extern thread_conn_handler_stub_def thread_conn_handler_stub;

#ifdef __cplusplus
}
#endif


#endif
