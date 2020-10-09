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
#include "test_coap_connection_handler.h"
#include <string.h>
#include <inttypes.h>
#include "nsdynmemLIB_stub.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "coap_connection_handler.h"
#include "coap_security_handler_stub.h"
#include "ns_timer_stub.h"
#include "socket_api.h"
#include "socket_api_stub.h"
#include "net_interface.h"

int send_to_sock_cb(int8_t socket_id, uint8_t address, uint16_t port, const unsigned char *a, int b)
{
    return 1;
}

int receive_from_sock_cb(int8_t socket_id, uint8_t address, uint16_t port, unsigned char *a, int b)
{
    return 1;
}

int get_passwd_cb(int8_t socket_id, uint8_t address, uint16_t port, uint8_t *pw_ptr, uint8_t *pw_len)
{
    return 0;
}

void sec_done_cb_test(int8_t socket_id, uint8_t address[static 16], uint16_t port, uint8_t keyblock[static 40])
{
    return 1;
}

bool test_connection_handler_create()
{
    coap_security_handler_stub.counter = -1;
    if (NULL != connection_handler_create(NULL, NULL, NULL, NULL)) {
        return false;
    }

    if (NULL != connection_handler_create(&receive_from_sock_cb, NULL, NULL, NULL)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, NULL, NULL, NULL);
    if (NULL == handler) {
        return false;
    }
    ns_dyn_mem_free(handler);
    return true;
}

bool test_connection_handler_destroy()
{
    coap_security_handler_stub.counter = -1;
    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, NULL, NULL, NULL);

    connection_handler_destroy(handler, false);
    return true;
}

bool test_coap_connection_handler_open_connection()
{
    coap_security_handler_stub.counter = -1;
    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, NULL, NULL, NULL);

    if (-1 != coap_connection_handler_open_connection(NULL, 0, false, false, false, false)) {
        return false;
    }

    if (-1 != coap_connection_handler_open_connection(handler, 0, false, false, false, false)) {
        return false;
    }

    ns_dyn_mem_free(handler);
    nsdynmemlib_stub.returnCounter = 1;
    handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    if (-1 != coap_connection_handler_open_connection(handler, 0, true, true, true, false)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 0, true, true, true, false)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, true)) {
        return false;
    }

    //open second one
    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler2 = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler2, 22, false, true, true, true)) {
        return false;
    }

    if (0 != coap_connection_handler_open_connection(handler, 23, false, false, false, false)) {
        return false;
    }

    connection_handler_destroy(handler2, false);
    connection_handler_destroy(handler, false);
    return true;
}

bool test_coap_connection_handler_send_data()
{
    coap_security_handler_stub.counter = -1;
    if (-1 != coap_connection_handler_send_data(NULL, NULL, ns_in6addr_any, NULL, 0, false)) {
        return false;
    }

    ns_address_t addr;
    memset(addr.address, 1, 16);
    addr.identifier = 22;

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, false, false)) {
        return false;
    }

    if (-1 != coap_connection_handler_send_data(handler, &addr, ns_in6addr_any, NULL, 0, true)) {
        return false;
    }

    connection_handler_destroy(handler, false);

    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    nsdynmemlib_stub.returnCounter = 1;
    handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 4;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, false, false)) {
        return false;
    }

    if (-1 != coap_connection_handler_send_data(handler, &addr, ns_in6addr_any, NULL, 0, true)) {
        return false;
    }

    if (-1 != coap_connection_handler_send_data(handler, &addr, ns_in6addr_any, NULL, 0, true)) {
        return false;
    }

    connection_handler_destroy(handler, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    //NON SECURE HERE -->
    nsdynmemlib_stub.returnCounter = 1;
    handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, false, false, false)) {
        return false;
    }


    if (1 != coap_connection_handler_send_data(handler, &addr, ns_in6addr_any, NULL, 0, true)) {
        return false;
    }
    connection_handler_destroy(handler, false);

    nsdynmemlib_stub.returnCounter = 1;
    handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, false, true, false)) {
        return false;
    }

    socket_api_stub.int8_value = 7;
    if (1 != coap_connection_handler_send_data(handler, &addr, ns_in6addr_any, NULL, 0, true)) {
        return false;
    }
    connection_handler_destroy(handler, false);

    //<-- NON SECURE HERE

    return true;
}

bool test_coap_connection_handler_virtual_recv()
{
    coap_security_handler_stub.counter = -1;
    uint8_t buf[16];
    memset(&buf, 1, 16);
    if (-1 != coap_connection_handler_virtual_recv(NULL, buf, 12, NULL, 0)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        return false;
    }

    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, NULL, 0)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, NULL, 0)) {
        return false;
    }

    ns_timer_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 3;
    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, &buf, 1)) {
        return false;
    }

    //handler->socket->data still in memory
    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    ns_timer_stub.int8_value = -1;
    nsdynmemlib_stub.returnCounter = 3;
    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, &buf, 1)) {
        return false;
    }

    ns_timer_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 3;
    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, &buf, 1)) {
        return false;
    }

    connection_handler_destroy(handler, false);

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler2 = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, &get_passwd_cb, &sec_done_cb_test);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler2, 24, false, true, true, false)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    if (0 != coap_connection_handler_virtual_recv(handler2, buf, 12, &buf, 1)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    coap_security_handler_stub.int_value = 0;
    if (0 != coap_connection_handler_virtual_recv(handler2, buf, 12, &buf, 1)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if (0 != coap_connection_handler_virtual_recv(handler2, buf, 12, &buf, 1)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    coap_security_handler_stub.int_value = MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY;
    if (0 != coap_connection_handler_virtual_recv(handler2, buf, 12, &buf, 1)) {
        return false;
    }

    connection_handler_destroy(handler2, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler3 = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, &get_passwd_cb, &sec_done_cb_test);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler3, 26, false, false, true, false)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    if (0 != coap_connection_handler_virtual_recv(handler3, buf, 12, &buf, 1)) {
        return false;
    }

    connection_handler_destroy(handler3, false);

    return true;
}

bool test_coap_connection_handler_socket_belongs_to()
{
    coap_security_handler_stub.counter = -1;
    if (false != coap_connection_handler_socket_belongs_to(NULL, 2)) {
        return false;
    }

    socket_api_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        return false;
    }

    if (true != coap_connection_handler_socket_belongs_to(handler, 0)) {
        return false;
    }

    if (false != coap_connection_handler_socket_belongs_to(handler, 3)) {
        return false;
    }
    connection_handler_destroy(handler, false);

    nsdynmemlib_stub.returnCounter = 0;
    return true;
}

bool test_timer_callbacks()
{
    coap_security_handler_stub.counter = -1;
    uint8_t buf[16];
    memset(&buf, 1, 16);

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        return false;
    }

    //handler->socket->data still in memory
    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    ns_timer_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 3;
    ns_timer_stub.int8_value = 5;
    if (-1 != coap_connection_handler_virtual_recv(handler, buf, 12, &buf, 1)) {
        return false;
    }

    //Note next tests will affect ns_timer test (cycle & cycle_count
    if (coap_security_handler_stub.start_timer_cb) {
        coap_security_handler_stub.start_timer_cb(1, 0, 0);

        coap_security_handler_stub.start_timer_cb(1, 1, 2);
    }

    if (coap_security_handler_stub.timer_status_cb) {
        if (-1 != coap_security_handler_stub.timer_status_cb(4)) {
            return false;
        }

        if (0 != coap_security_handler_stub.timer_status_cb(1)) {
            return false;
        }
    }

    if (ns_timer_stub.cb) {
        ns_timer_stub.cb(4, 0);

        ns_timer_stub.cb(5, 0);

        coap_security_handler_stub.int_value = MBEDTLS_ERR_SSL_TIMEOUT;

        ns_timer_stub.cb(5, 0);
        coap_security_handler_stub.int_value = 0;
    }

    connection_handler_destroy(handler, false);
    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;
    return true;
}

bool test_socket_api_callbacks()
{
    coap_security_handler_stub.counter = -1;
    uint8_t buf[16];
    memset(&buf, 1, 16);

    socket_callback_t *sckt_data = (socket_callback_t *)malloc(sizeof(socket_callback_t));
    memset(sckt_data, 0, sizeof(socket_callback_t));

    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    socket_api_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, false, true, false)) {
        free(sckt_data);
        return false;
    }

    if (socket_api_stub.recv_cb) {
        sckt_data->event_type = SOCKET_DATA;
        sckt_data->d_len = 1;
        socket_api_stub.int8_value = -1;
        socket_api_stub.recv_cb(sckt_data);

        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.recv_cb(sckt_data);

        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.int8_value = 1;
        socket_api_stub.recv_cb(sckt_data);
    }

    connection_handler_destroy(handler, false);

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler2 = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, &get_passwd_cb, &sec_done_cb_test);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler2, 22, false, true, true, false)) {
        return false;
    }

    if (socket_api_stub.recv_cb) {
        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 1;
        socket_api_stub.recv_cb(sckt_data);

        nsdynmemlib_stub.returnCounter = 2;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 1;
        socket_api_stub.recv_cb(sckt_data);

        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 1;
        socket_api_stub.recv_cb(sckt_data);

        coap_security_handler_stub.int_value = 4;
        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 1;
        socket_api_stub.recv_cb(sckt_data);

        coap_security_handler_stub.int_value = MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY;
        nsdynmemlib_stub.returnCounter = 1;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 1;
        socket_api_stub.recv_cb(sckt_data);
    }

    connection_handler_destroy(handler2, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    free(sckt_data);
    sckt_data = NULL;
    return true;
}

bool test_security_callbacks()
{
    coap_security_handler_stub.counter = -1;
    uint8_t buf[16];
    memset(&buf, 1, 16);

    socket_callback_t *sckt_data = (socket_callback_t *)malloc(sizeof(socket_callback_t));
    memset(sckt_data, 0, sizeof(socket_callback_t));

    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        free(sckt_data);
        return false;
    }

    if (socket_api_stub.recv_cb) {
        sckt_data->event_type = SOCKET_DATA;
        sckt_data->d_len = 1;
        nsdynmemlib_stub.returnCounter = 2;
        socket_api_stub.int8_value = 1;
        sckt_data->socket_id = 0;
        socket_api_stub.recv_cb(sckt_data);
    }

    if (coap_security_handler_stub.send_cb) {
        coap_security_handler_stub.send_cb(0, buf, 22, &buf, 16);
    }
    if (coap_security_handler_stub.receive_cb) {
        coap_security_handler_stub.receive_cb(0, &buf, 16);
    }

    connection_handler_destroy(handler, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    free(sckt_data);
    sckt_data = NULL;
    return true;
}

bool test_coap_connection_handler_msg_prevalidate_cb_read_and_set()
{
    coap_security_handler_stub.counter = -1;

    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        return false;
    }

    if (-1 != coap_connection_handler_msg_prevalidate_callback_set(NULL, 1000)) {
        return false;
    }

    if (0 != coap_connection_handler_msg_prevalidate_callback_set(handler, 1000)) {
        return false;
    }

    uint16_t listen_socket_port;
    if (NULL != coap_connection_handler_msg_prevalidate_callback_get(NULL, &listen_socket_port)) {
        return false;
    }

    if (1000 != coap_connection_handler_msg_prevalidate_callback_get(handler, &listen_socket_port)) {
        return false;
    }

    connection_handler_destroy(handler, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    return true;
}

bool test_coap_connection_handler_find_by_socket_port()
{
    coap_conn_handler_t *handler_ref;
    coap_security_handler_stub.counter = -1;

    coap_security_handler_stub.sec_obj = coap_security_handler_stub_alloc();

    nsdynmemlib_stub.returnCounter = 1;
    coap_conn_handler_t *handler = connection_handler_create(&receive_from_sock_cb, &send_to_sock_cb, NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != coap_connection_handler_open_connection(handler, 22, false, true, true, false)) {
        return false;
    }

    handler_ref = coap_connection_handler_find_by_socket_port(1000);
    if (NULL != handler_ref) {
        return false;
    }

    handler_ref = coap_connection_handler_find_by_socket_port(22);
    if (handler_ref->_recv_cb != receive_from_sock_cb) {
        return false;
    }

    connection_handler_destroy(handler, false);

    free(coap_security_handler_stub.sec_obj);
    coap_security_handler_stub.sec_obj = NULL;

    return true;
}


