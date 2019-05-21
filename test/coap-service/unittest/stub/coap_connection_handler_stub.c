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

#include <string.h>
#include "coap_connection_handler.h"
#include "coap_security_handler.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "socket_api.h"
#include "net_interface.h"
#include "eventOS_callback_timer.h"
#include "coap_connection_handler_stub.h"

thread_conn_handler_stub_def thread_conn_handler_stub;

int coap_connection_handler_virtual_recv(coap_conn_handler_t *handler, uint8_t address[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len)
{
    return thread_conn_handler_stub.int_value;
}

coap_conn_handler_t *connection_handler_create(int (*recv_cb)(int8_t socket_id, int8_t recv_if_id, uint8_t src_address[static 16], uint16_t port, const uint8_t dst_address[static 16], unsigned char *, int),
                                               int (*send_cb)(int8_t socket_id, uint8_t const address[static 16], uint16_t port, const void *, int),
                                               int (*pw_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, coap_security_keys_t *security_ptr),
                                               void(*done_cb)(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static KEY_BLOCK_LEN]))
{
    thread_conn_handler_stub.send_to_sock_cb = send_cb;
    thread_conn_handler_stub.receive_from_sock_cb = recv_cb;
    thread_conn_handler_stub.get_passwd_cb = pw_cb;
    thread_conn_handler_stub.sec_done_cb = done_cb;
    thread_conn_handler_stub.cch_function_callback = NULL;
    return thread_conn_handler_stub.handler_obj;
}

void connection_handler_destroy(coap_conn_handler_t *handler, bool multicast_group_leave)
{

}
void connection_handler_close_secure_connection(coap_conn_handler_t *handler, uint8_t destination_addr_ptr[static 16], uint16_t port)
{

}

int coap_connection_handler_open_connection(coap_conn_handler_t *handler, uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool is_real_socket, bool bypassSec)
{
    return thread_conn_handler_stub.int_value;
}

int coap_connection_handler_send_data(coap_conn_handler_t *handler, const ns_address_t *dest_addr, const uint8_t dst_address[static 16], uint8_t *data_ptr, uint16_t data_len, bool bypass_link_sec)
{
    return thread_conn_handler_stub.int_value;
}

bool coap_connection_handler_socket_belongs_to(coap_conn_handler_t *handler, int8_t socket_id)
{
    return thread_conn_handler_stub.bool_value;
}

int8_t coap_connection_handler_set_timeout(coap_conn_handler_t *handler, uint32_t min, uint32_t max)
{
    return 0;
}

int8_t coap_connection_handler_handshake_limits_set(uint8_t handshakes_limit, uint8_t connections_limit)
{
    return 0;
}

void coap_connection_handler_exec(uint32_t time)
{
}

int coap_connection_handler_msg_prevalidate_callback_set(coap_conn_handler_t *handler, cch_func_cb *function_callback)
{
    thread_conn_handler_stub.cch_function_callback = function_callback;
    return 0;
}


cch_func_cb *coap_connection_handler_msg_prevalidate_callback_get(coap_conn_handler_t *handler, uint16_t *listen_socket_port)
{
    *listen_socket_port = 0;
    return thread_conn_handler_stub.cch_function_callback;
}

coap_conn_handler_t *coap_connection_handler_find_by_socket_port(uint16_t listen_port)
{
    return thread_conn_handler_stub.handler_obj;
}
