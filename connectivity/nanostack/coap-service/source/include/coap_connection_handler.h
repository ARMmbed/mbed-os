/*
 * Copyright (c) 2015-2018, Pelion and affiliates.
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

#ifndef __COAP_CONNECTION_HANDLER_H__
#define __COAP_CONNECTION_HANDLER_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include "ns_address.h"
#include "coap_service_api_internal.h"
#include "coap_security_handler.h"

#define MAX_SECURE_SESSION_COUNT 3
#define MAX_ONGOING_HANDSHAKES 2
#define CLOSED_SECURE_SESSION_TIMEOUT 3600          // Seconds
#define ONGOING_HANDSHAKE_TIMEOUT 600               // Seconds
#define OPEN_SECURE_SESSION_TIMEOUT 18000           // Seconds
#define SECURE_SESSION_CLEAN_INTERVAL 60            // Seconds

struct internal_socket_s;

typedef int send_to_socket_cb(int8_t socket_id, const uint8_t address[static 16], uint16_t port, const void *, int);
typedef int receive_from_socket_cb(int8_t socket_id, int8_t recv_if_id, uint8_t src_address[static 16], uint16_t port, const uint8_t dst_address[static 16], unsigned char *, int);
typedef int get_pw_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, coap_security_keys_t *security_ptr);
typedef void security_done_cb(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40]);
typedef void cch_func_cb(void);

typedef struct coap_conn_handler_s {
    struct internal_socket_s *socket;

    coap_security_keys_t *security_keys;
    receive_from_socket_cb *_recv_cb;
    send_to_socket_cb *_send_cb;
    get_pw_cb *_get_password_cb;
    security_done_cb *_security_done_cb;

    int8_t socket_interface_selection;
    bool registered_to_multicast;
} coap_conn_handler_t;

coap_conn_handler_t *connection_handler_create(receive_from_socket_cb *recv_from_cb,
                                               send_to_socket_cb *send_to_cb,
                                               get_pw_cb *pw_cb,
                                               security_done_cb *done_cb);

void connection_handler_destroy(coap_conn_handler_t *handler, bool multicast_group_leave);

void connection_handler_close_secure_connection(coap_conn_handler_t *handler, uint8_t destination_addr_ptr[static 16], uint16_t port);

int coap_connection_handler_open_connection(coap_conn_handler_t *handler, uint16_t listen_port, bool use_ephemeral_port, bool is_secure, bool real_socket, bool bypassSec);

//If returns -2, it means security was started and data was not send
/*
 * \return > 0 in OK
 * \return 0 Session started, data not send
 * \return -1 failure
 */
int coap_connection_handler_send_data(coap_conn_handler_t *handler, const ns_address_t *dest_addr, const uint8_t src_address[static 16], uint8_t *data_ptr, uint16_t data_len, bool bypass_link_sec);

int coap_connection_handler_virtual_recv(coap_conn_handler_t *handler, uint8_t address[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len);

bool coap_connection_handler_socket_belongs_to(coap_conn_handler_t *handler, int8_t socket_id);

int8_t coap_connection_handler_set_timeout(coap_conn_handler_t *handler, uint32_t min, uint32_t max);

int8_t coap_connection_handler_handshake_limits_set(uint8_t handshakes_limit, uint8_t connections_limit);

void coap_connection_handler_exec(uint32_t time);

coap_conn_handler_t *coap_connection_handler_find_by_socket_port(uint16_t listen_port);

int coap_connection_handler_msg_prevalidate_callback_set(coap_conn_handler_t *handler, cch_func_cb *function_callback);

cch_func_cb *coap_connection_handler_msg_prevalidate_callback_get(coap_conn_handler_t *handler, uint16_t *listen_socket_port);

#endif
