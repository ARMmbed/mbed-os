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
#include "nsconfig.h"
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "CppUTest/TestHarness.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "../../../../../nanostack/socket_api.h"
#include "ns_error_types.h"

#include "../../stub/socket_stub.h"
#include "../../stub/address_stub.h"
#include "../../stub/buffer_dyn_stub.h"
#include "../../stub/protocol_core_stub.h"
#include "../../stub/tcp_stub.h"
#include "Common_Protocols/tcp.h"
#include "common_functions.h"

ns_address_t address = {ADDRESS_IPV6, {0}, 8080};

ns_address_t dst_address = {ADDRESS_IPV6, {0x20,0x01, 0x0d,0xb8}, 8080};

protocol_interface_info_entry_t interface;

void socket_callback(void *cb)
{
    //socket_callback_t *sock_cb = (socket_callback_t *) cb;
}

TEST_GROUP(SocketAPI)
{
    void setup() {
        socket_stub_setup();
    }

    void teardown() {
        socket_stub_teardown();
    }
};

TEST(SocketAPI, Create)
{

}

#if 1
TEST(SocketAPI, test_socketAPI_open)
{
    int8_t socket = -1;
    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;

    // socket open OK
    test_data->err = eOK;
    test_data->socket_id = 0;
    socket = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    // socket open OK (RAW socket)
    test_data->socket_id = 1;
    socket = socket_open(SOCKET_RAW, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    socket = socket_open(SOCKET_RAW, 0x100, socket_callback);
    BYTES_EQUAL(-1, socket);

    // Socket open fail, no function callback set
    test_data->err = eFALSE;
    test_data->socket_id = -1;
    socket = socket_open(SOCKET_TCP, 0, NULL);
    BYTES_EQUAL(test_data->socket_id, socket);

    // Socket open fail, busy
    test_data->err = eBUSY;
    test_data->socket_id = -2;
    socket = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    // Socket open failure, test default branch
    test_data->err = eSYSTEM;
    test_data->socket_id = -1;
    socket = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    // Socket type mismatch
    test_data->socket_id = -1;
    socket = socket_open(0, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);
}

TEST(SocketAPI, test_socketAPI_close)
{
    int8_t err;
    int8_t socket;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eFALSE;
    err = socket_close(-1);
    BYTES_EQUAL(-1, err);

    socket_stub_clear_sockets();

    // OK case
    test_data->err = eOK;
    socket_stub.socket_structure = test_data->socket_ptr;
    socket = socket_open(SOCKET_UDP, 0, socket_callback);
    err = socket_close(socket);
    BYTES_EQUAL(0, err);

    // fail case
    socket_stub.socket_structure = NULL;
    test_data->err = eFALSE;
    err = socket_close(0);
    BYTES_EQUAL(-1, err);
}

TEST(SocketAPI, test_socketAPI_connect)
{
    int8_t err;
    int8_t socket;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    // trying to connect UDP socket
    test_data->err = eOK;
    test_data->socket_id = 0;

    err = socket_connect(-1, &address, 0);
    BYTES_EQUAL(-1, err);

    err = socket_connect(2, &address, 0);
    BYTES_EQUAL(-1, err);

    socket_stub.socket_structure = test_data->socket_ptr;

    socket = socket_open(SOCKET_RAW, 0, socket_callback);
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-5, err);

    socket_stub_clear_sockets();

    socket = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);
    buffer_t buffer_ptr;
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    protocol_interface_info_entry_t entry;
    socket_stub.entry_ptr = &entry;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(0, err);

    // TCP socket
    test_data->err = eOK;
    test_data->socket_id = 0;
    socket = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    // socket is not valid
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    err = socket_connect(-1, &address, 0);
    BYTES_EQUAL(-1, err);

    // address is NULL
    socket_stub.socket_structure = test_data->socket_ptr;
    test_data->err = eOK;
    err = socket_connect(socket, NULL, 0);
    BYTES_EQUAL(-1, err);

    test_data->err = eOK;
    test_data->socket_id = 0;
    buffer_dyn_stub.buffer_ptr = NULL;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-2, err);
    socket_listen(socket, 5);
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    test_data->socket_ptr->flags = SOCKET_LISTEN_STATE;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-3, err);

    socket_stub_clear_sockets();
    test_data = socket_stub_get_test_data();

    // trying to connect TCP socket
    test_data->err = eOK;
    test_data->socket_id = 0;
    test_data->socket_ptr->flags = 0;
    socket = socket_open(SOCKET_TCP, 0, socket_callback);
    tcp_session_t tcp_session;
    test_data->socket_ptr->inet_pcb->session = &tcp_session;
    tcp_stub.tcp_error_value = TCP_ERROR_WRONG_STATE;
    test_data->socket_ptr->inet_pcb->flow_label == /*IPV6_FLOW_AUTOGENERATE*/ -2;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-4, err);
    test_data->socket_ptr->inet_pcb->session = NULL;
    tcp_stub.tcp_error_value = TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-2, err);
    tcp_stub.tcp_session_ptr = &tcp_session;

    tcp_stub.tcp_error_value = TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-2, err);

    tcp_stub.tcp_error_value = TCP_ERROR_NO_ERROR;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(0, err);
    socket = socket_open(SOCKET_UDP, 0, socket_callback);
    buffer_dyn_stub.buffer_ptr = NULL;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-4, err);
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    socket_stub.entry_ptr = NULL;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-4, err);
    socket_stub.entry_ptr = &entry;
    address_stub.int8_value = -1;
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-4, err);
    address_stub.int8_value = 0;
    address.identifier = 128;
    test_data->socket_ptr->inet_pcb->flow_label = (-2);
    err = socket_connect(socket, &address, 0);
    BYTES_EQUAL(-4, err);
}

TEST(SocketAPI, test_socketAPI_shutdown)
{
    int8_t status;
    int8_t sock_id;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 0;
    sock_id = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);
    socket_stub.socket_structure = test_data->socket_ptr;
    // test: If no active TCP session
    status = socket_shutdown(sock_id, SOCKET_SHUT_RDWR);
    BYTES_EQUAL(-2, status);
    socket_stub.socket_structure = NULL;
    status = socket_shutdown(2, SOCKET_SHUT_RDWR);
    // test: If no such socket ID is found
    BYTES_EQUAL(-1, status);

    tcp_stub.tcp_error_value = TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    socket_stub.socket_structure = test_data->socket_ptr;
    test_data->socket_ptr->inet_pcb->local_port = 10;
    status = socket_shutdown(sock_id, SOCKET_SHUT_RDWR);
    BYTES_EQUAL(-2, status);

    status = socket_shutdown(sock_id, NULL);
    BYTES_EQUAL(-2, status);

    tcp_session_t tcp_session;
    tcp_stub.tcp_error_value = TCP_ERROR_NO_ERROR;
    test_data->socket_ptr->inet_pcb->session = &tcp_session;
    status = socket_shutdown(sock_id, SOCKET_SHUT_RDWR);
    BYTES_EQUAL(-2, status);

    // test: close bad socket ID
    socket_stub.socket_structure = NULL;
    status = socket_shutdown(-1, SOCKET_SHUT_RDWR);
    BYTES_EQUAL(-1, status);

    //Test UDP close
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    test_data->socket_ptr->inet_pcb->local_port = 10;
    socket_stub.socket_structure = test_data->socket_ptr;
    status = socket_shutdown(sock_id, SOCKET_SHUT_RDWR);
    // Currently Socket_shutdown is not supporting UDP
    BYTES_EQUAL(-1, status);

    sock_id = socket_open(SOCKET_RAW, 0, socket_callback);
    socket_stub.socket_structure = test_data->socket_ptr;
    status = socket_shutdown(sock_id, SOCKET_SHUT_RDWR);
    BYTES_EQUAL(-1, status);

}

TEST(SocketAPI, test_socketAPI_send)
{
    int8_t status;
    int8_t sock_id;
    uint8_t buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // connect socket to address
    test_data->err = eOK;
    socket_stub.socket_structure = test_data->socket_ptr;
    buffer_dyn_stub.buffer_ptr = NULL;
    tcp_stub.tcp_error_value = TCP_ERROR_BUFFER_ALLOCATION_ERROR;
    status = socket_connect(sock_id, &address, 0);
    BYTES_EQUAL(-2, status);

    // test length zero
    buffer_t buffer_ptr;
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    status = socket_send(sock_id, (uint8_t *)&buf, 0);
    BYTES_EQUAL(0, status);

    // test with valid params,
    status = socket_send(sock_id, (uint8_t *)&buf, sizeof(buf));
    BYTES_EQUAL(0, status);

    tcp_session_t tcp_session;
    test_data->socket_ptr->inet_pcb->session = &tcp_session;
    //status = socket_send(sock_id, (uint8_t *)&buf, sizeof(buf));
    //BYTES_EQUAL(-3, status);
    protocol_interface_info_entry_t entry;
    protocol_core_stub.entry_ptr = &entry;
    address_stub.uint8_ptr = NULL;
    //status = socket_send(sock_id, (uint8_t *)&buf, sizeof(buf));
    //BYTES_EQUAL(-3, status);
    uint8_t temp_address[16];
    address_stub.uint8_ptr = temp_address;
    buffer_dyn_stub.buffer_ptr = NULL;
    //status = socket_send(sock_id, (uint8_t *)&buf, sizeof(buf));
    //BYTES_EQUAL(-2, status);
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    //status = socket_send(sock_id, (uint8_t *)&buf, sizeof(buf));
    //BYTES_EQUAL(0, status);

    // test sending data to UDP socket
    uint8_t sock_udp_id = socket_open(SOCKET_UDP, 0, socket_callback);
//    status = socket_send(sock_udp_id, (uint8_t *)&buf, sizeof(buf));
//    BYTES_EQUAL(-5, status);
//    test_data->socket_ptr->inet_pcb->remote_port = 10;
    //Buffer get fail
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    status = socket_send(sock_udp_id, (uint8_t *)&buf, sizeof(buf));
    BYTES_EQUAL(0, status);

}

TEST(SocketAPI, test_socketAPI_read)
{
    int8_t status;
    int8_t sock_id;
    uint8_t buf[10];

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // test reading from unknow socket
    socket_stub.socket_structure = NULL;
    status = socket_read(sock_id, &address, (uint8_t *)&buf, 10);
    BYTES_EQUAL(-1, status);

    socket_stub.socket_structure = test_data->socket_ptr;

    // test NULL address
    status = socket_read(sock_id, NULL, (uint8_t *)&buf, 10);
    BYTES_EQUAL(0, status);

    // test NULL buffer
    socket_stub.socket_read_cnt = 0;
    status = socket_read(sock_id, &address, NULL, 10);
    BYTES_EQUAL(0, status);

    socket_stub.socket_read_cnt = 2;
    status = socket_read(sock_id, &address, NULL, 10);
    BYTES_EQUAL(0, status);

    status = socket_read(sock_id, &address, NULL, 10);
    BYTES_EQUAL(0, status);
    socket_stub.socket_structure = NULL;
}

TEST(SocketAPI, test_socketAPI_bind2addrsel)
{
    int8_t status;
    int8_t sock_id;
    uint8_t buf[10];

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 0;

    status = socket_bind2addrsel(-1,NULL);
    BYTES_EQUAL(-1, status);

    status = socket_bind2addrsel(100,NULL);
    BYTES_EQUAL(-1, status);

    status = socket_bind2addrsel(2,NULL);
    BYTES_EQUAL(-1, status);

    socket_stub_clear_sockets();
    socket_stub.socket_structure = test_data->socket_ptr;
    test_data->err = eOK;
    sock_id = socket_open(SOCKET_RAW, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);
    status = socket_bind2addrsel(sock_id,NULL);
    BYTES_EQUAL(-1, status);
    status = socket_bind2addrsel(sock_id,&dst_address);
    BYTES_EQUAL(-6, status);
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    // Fails due to buffer_get failing
    status = socket_bind2addrsel(sock_id,&address);
    BYTES_EQUAL(-2, status);

    buffer_t buffer_ptr;
    buffer_dyn_stub.buffer_ptr = NULL;
    status = socket_bind2addrsel(sock_id,&address);
    BYTES_EQUAL(-2, status);
    buffer_dyn_stub.buffer_ptr = &buffer_ptr;
    socket_stub.entry_ptr = NULL;
    status = socket_bind2addrsel(sock_id,&address);
    BYTES_EQUAL(-4, status);
    protocol_interface_info_entry_t entry;
    socket_stub.entry_ptr = &entry;
    address_stub.int8_value = -1;
    status = socket_bind2addrsel(sock_id,&address);
    BYTES_EQUAL(-5, status);
    address_stub.int8_value = 0;
    status = socket_bind2addrsel(sock_id,&address);
    BYTES_EQUAL(0, status);

}

#endif

TEST(SocketAPI, socket_getpeername)
{
    ns_address_t addr;
    int8_t socket = -1;
    int8_t err;
    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;

    // socket open OK
    test_data->err = eOK;
    test_data->socket_id = 0;
    socket = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, socket);

    socket_stub.socket_structure = test_data->socket_ptr;
    err = socket_getpeername(test_data->socket_id, &addr);
    BYTES_EQUAL(-2, err);

    err = socket_getpeername(test_data->socket_id, &addr);
    BYTES_EQUAL(-2, err);
    // fail, bad socket
    socket_stub.socket_structure = NULL;
    err = socket_getpeername(-1, &addr);
    BYTES_EQUAL(-1, err);

    // fail, bad socket in socket range
    err = socket_getpeername(2, &addr);
    BYTES_EQUAL(-1, err);
}

TEST(SocketAPI, socket_sendto)
{
    int8_t status;
    int8_t sock_id;
    uint8_t buf[] = "abcd";
    ns_address_t addr;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);
    socket_stub.socket_structure = test_data->socket_ptr;

    // Tx with length 0
    status = socket_sendto(sock_id, &address, (uint8_t *)&buf, sizeof(buf));
    BYTES_EQUAL(0, status);
    // Tx with length 0
    status = socket_sendto(sock_id, &address, (uint8_t *)&buf, 0);
    BYTES_EQUAL(0, status);

    status = socket_sendmsg(sock_id, NULL, 0);
    BYTES_EQUAL(-1, status);

    buffer_dyn_stub.buffer_ptr = NULL;

}

TEST(SocketAPI, socket_bind)
{
    int8_t status;
    int8_t sock_id;
    ns_address_t addr = {ADDRESS_IPV6, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 0};

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;

    sock_id = socket_open(SOCKET_RAW, 0, socket_callback);

    socket_stub.socket_structure = test_data->socket_ptr;
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(-5, status);

    sock_id = socket_open(SOCKET_UDP, 8080, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // fail, bad socket
    socket_stub.socket_structure = NULL;
    status = socket_bind(-1, &addr);
    BYTES_EQUAL(-1, status);

    // fail, bad socket
    status = socket_bind(2, &addr);
    BYTES_EQUAL(-1, status);

    // fail, NULL address
    socket_stub.socket_structure = test_data->socket_ptr;
    status = socket_bind(sock_id, NULL);
    BYTES_EQUAL(-1, status);

    // OK
    addr.identifier = 8080;
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(0, status);

    // fail, port already chosen
    addr.identifier = 8081;
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(-4, status);

    // create new socket
    test_data->err = eOK;
    test_data->socket_id = 2;
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    socket_stub.socket_structure = test_data->socket_ptr;
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // fail, port validation fails
    addr.identifier = 8081;
    test_data->socket_ptr->inet_pcb->local_port = 0;
    test_data->err = eFALSE;
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(-2, status);

    test_data->err = eOK;
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(0, status);
}

TEST(SocketAPI, socket_listen)
{
    int8_t status;
    int8_t sock_id;
    ns_address_t addr = {ADDRESS_IPV6, {0}, 8080};

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // fail, bad socket id
    status =  socket_listen(-1, 5);
    BYTES_EQUAL(-1, status);

    // fail, bad socket id but in socket range
    status =  socket_listen(2, 5);
    BYTES_EQUAL(-1, status);

    // fail, listening UDP socket
    socket_stub.socket_structure = test_data->socket_ptr;
    status =  socket_listen(sock_id, 5);
    BYTES_EQUAL(-1, status);

    socket_stub_clear_sockets();

    // create TCP socket
    test_data = socket_stub_get_test_data();
    test_data->err = eOK;
    test_data->socket_id = 2;
    sock_id = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);
    socket_stub.socket_structure = test_data->socket_ptr;

    // fail, not bound to a port
    status =  socket_listen(sock_id, 5);
    BYTES_EQUAL(-1, status);

    // bind the socket to a port
    status = socket_bind(sock_id, &addr);
    BYTES_EQUAL(0, status)

    // OK
    test_data->socket_ptr->u.live.fptr = socket_callback;
    test_data->socket_ptr->type = SOCKET_TYPE_STREAM;
    status =  socket_listen(sock_id, 5);
    BYTES_EQUAL(0, status);
}

TEST(SocketAPI, socket_setsockopt)
{
    int8_t status;
    int8_t sock_id;
    int8_t sock_id_tcp;
    socket_t *sock_tcp;
    socket_t *sock_udp;
    int8_t opt_int8 = 0;
    int16_t opt_int16 = 0;
    int32_t opt_int32 = 0;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // fail, bad socket id
    status =  socket_setsockopt(-1, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(-1, status);

    // fail, bad protocol level
    sock_udp = test_data->socket_ptr;
    socket_stub.socket_structure = test_data->socket_ptr;
    status =  socket_setsockopt(sock_id, 0, SOCKET_LINK_LAYER_SECURITY, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(-2, status);

    // fail, bad option name
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, 0, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(-2, status);

    // fail, NULL option
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, NULL, sizeof(int8_t));
    BYTES_EQUAL(-1, status);

    // OK
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    // fail, bad size
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &opt_int8, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // OK, SOCKET_LINK_LAYER_SECURITY
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_LINK_LAYER_SECURITY
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &opt_int8, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // OK, SOCKET_IPV6_USE_MIN_MTU
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_USE_MIN_MTU, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_IPV6_USE_MIN_MTU
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_USE_MIN_MTU, &opt_int8, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // fail, bad value SOCKET_IPV6_USE_MIN_MTU
    opt_int8 = 2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_USE_MIN_MTU, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(-3, status);
    opt_int8 = 0;

    // OK, SOCKET_IPV6_DONTFRAG
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_DONTFRAG, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_IPV6_DONTFRAG
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_DONTFRAG, &opt_int8, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // fail, bad value SOCKET_IPV6_DONTFRAG
    opt_int8 = 2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_DONTFRAG, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(-3, status);

    // INT16
    test_data->err = eOK;
    test_data->socket_id = 2;
    sock_id_tcp = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id_tcp);
    sock_tcp = test_data->socket_ptr;
    socket_stub.socket_structure = sock_udp;
    // OK, SOCKET_IPV6_TCLASS
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_IPV6_DONTFRAG
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &opt_int16, sizeof(int8_t));
    BYTES_EQUAL(-3, status);

    socket_stub.socket_structure = sock_tcp;
    // fail, bad socket type SOCKET_IPV6_DONTFRAG
    status =  socket_setsockopt(sock_id_tcp, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(-2, status);

    // OK, special value -1
    socket_stub.socket_structure = sock_udp;
    opt_int16 = -1;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // fail, invalid value -2
    opt_int16 = -2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(-3, status);
    opt_int16 = 0;

    // OK, SOCKET_IPV6_UNICAST_HOPS
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_IPV6_UNICAST_HOPS
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &opt_int16, sizeof(int8_t));
    BYTES_EQUAL(-3, status);

    // OK, special value -2
    opt_int16 = -2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // OK, SOCKET_IPV6_MULTICAST_HOPS
    opt_int16 = 0;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // fail, bad size SOCKET_IPV6_MULTICAST_HOPS
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &opt_int16, sizeof(int8_t));
    BYTES_EQUAL(-3, status);

    // OK, special value -1
    opt_int16 = -1;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // fail, special value -2
    opt_int16 = -2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // OK, SOCKET_IPV6_ADDRESS_SELECT
    opt_int16 = SOCKET_SRC_ADDRESS_MODE_PRIMARY;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDRESS_SELECT, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // OK, SOCKET_IPV6_ADDRESS_SELECT
    opt_int16 = SOCKET_SRC_ADDRESS_MODE_SECONDARY;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDRESS_SELECT, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    // Fail, bad value in options
    opt_int16 = -2;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDRESS_SELECT, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    // fail, bad size SOCKET_IPV6_ADDRESS_SELECT
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDRESS_SELECT, &opt_int16, sizeof(int8_t));
    BYTES_EQUAL(-3, status);

    // INT32 options

    // OK, SOCKET_IPV6_ADDR_PREFERENCES
    opt_int32 = SOCKET_IPV6_PREFER_SRC_PUBLIC;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_int32, sizeof(int32_t));
    BYTES_EQUAL(0, status);

    // fail, bad option SOCKET_IPV6_ADDR_PREFERENCES
    opt_int32 = 0xffffffff;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_int32, sizeof(int32_t));
    BYTES_EQUAL(-3, status);

    // fail, bad len SOCKET_IPV6_ADDR_PREFERENCES
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &opt_int32, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    //fail by length
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_BROADCAST_PAN, &opt_int8, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_BROADCAST_PAN, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    //fail by length
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_FLOW_LABEL, &opt_int32, sizeof(int16_t));
    BYTES_EQUAL(-3, status);

    opt_int32 = 0x100000;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_FLOW_LABEL, &opt_int32, sizeof(int32_t));
    BYTES_EQUAL(-3, status);

    opt_int32 = 1;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_FLOW_LABEL, &opt_int32, sizeof(int32_t));
    BYTES_EQUAL(0, status);


    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVHOPLIMIT, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);


    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVTCLASS, &opt_int8, sizeof(int8_t));
    BYTES_EQUAL(0, status);

}

TEST(SocketAPI, socket_getsockopt)
{
    int8_t status;
    int8_t sock_id;
    void *option;
    uint16_t option_len = 4;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_TCP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);
    socket_stub.socket_structure = NULL;
    // fail, bad socket id
    status =  socket_getsockopt(-1, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &option, &option_len);
    BYTES_EQUAL(-1, status);
    socket_stub.socket_structure = test_data->socket_ptr;
    // fail, bad protocol level
    status =  socket_getsockopt(sock_id, 0, SOCKET_LINK_LAYER_SECURITY, &option, &option_len);
    BYTES_EQUAL(-2, status);

    // fail, bad option name
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, 0, &option, &option_len);
    BYTES_EQUAL(-2, status);

    // fail, NULL option
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, NULL, &option_len);
    BYTES_EQUAL(-1, status);

    //OK cases
    socket_stub.socket_structure = test_data->socket_ptr;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_TCLASS, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(2, option_len);
    option_len = 4;

    // OK, set SOCKET_IPV6_UNICAST_HOPS first
    int16_t opt_int16 = 64;
    status =  socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &opt_int16, sizeof(int16_t));
    BYTES_EQUAL(0, status);

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(64, option);
    BYTES_EQUAL(2, option_len);
    option_len = 4;

    tcp_session_t tcp_session;
    protocol_interface_info_entry_t entry;
    tcp_session.interface = &entry;
    entry.cur_hop_limit = 64;
    test_data->socket_ptr->inet_pcb->session = &tcp_session;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(entry.cur_hop_limit, option);
    BYTES_EQUAL(2, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(2, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(4, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_USE_MIN_MTU, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);
    option_len = 4;

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_DONTFRAG, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);

    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_BROADCAST_PAN, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);

    option_len = 8;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_FLOW_LABEL, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(4, option_len);
    option_len = 8;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);
    option_len = 8;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVHOPLIMIT, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);
    option_len = 8;
    status =  socket_getsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVTCLASS, &option, &option_len);
    BYTES_EQUAL(0, status);
    BYTES_EQUAL(1, option_len);

}

//Test properly socket_readmsg()

TEST(SocketAPI, socket_recvmsg)
{
    int8_t status;
    int8_t sock_id;
    uint8_t buf[10];

    ns_iovec_t msg_iov;
    ns_msghdr_t msghdr;

    //Init message payload vector
    msg_iov.iov_base = buf;
    msg_iov.iov_len = sizeof(buf);

    //Set messages name buffer
    msghdr.msg_name = &address;
    msghdr.msg_namelen = sizeof(ns_address_t);
    msghdr.msg_control = NULL;
    msghdr.msg_controllen = 0;
    msghdr.msg_flags = 0;
    msghdr.msg_iov = &msg_iov;
    msghdr.msg_iovlen = 1;

    socket_stub_data_t *test_data = socket_stub_get_test_data();
    socket_stub.socket_structure = NULL;
    test_data->err = eOK;
    test_data->socket_id = 1;
    sock_id = socket_open(SOCKET_UDP, 0, socket_callback);
    BYTES_EQUAL(test_data->socket_id, sock_id);

    // test reading from unknow socket
    socket_stub.socket_structure = NULL;
    status = socket_recvmsg(sock_id, NULL, 0);
    BYTES_EQUAL(-1, status);

    socket_stub.socket_read_cnt = 0;

    status = socket_recvmsg(sock_id, &msghdr, 0);
    BYTES_EQUAL(-1, status);

    socket_stub.socket_structure = test_data->socket_ptr;
    status = socket_recvmsg(sock_id, &msghdr, 0);
    /*Would block*/
    BYTES_EQUAL(-100, status);
    
    socket_stub.socket_read_cnt = 10;
    msg_iov.iov_base = buf;
    buffer_t *read_buffer = test_data->socket_read_buffer;
    uint8_t *ptr;
    ptr = buffer_data_pointer(read_buffer);
    ptr = common_write_16_bit(1000, ptr);
    read_buffer->buf_end = ptr - read_buffer->buf;
    
    read_buffer->interface = &interface;
    read_buffer->interface->id = 2;
    read_buffer->size = 4;

    memset(read_buffer->dst_sa.address, 4, 16);
    read_buffer->options.hop_limit = 32;
    read_buffer->options.traffic_class = 14;

    //sockbuf_init(&test_data->socket_ptr->rcvq);
    //sockbuf_append(&test_data->socket_ptr->rcvq, read_buffer);
    ns_list_init(&test_data->socket_ptr->rcvq.bufs);
    ns_list_add_to_end(&test_data->socket_ptr->rcvq.bufs, read_buffer);

    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    msghdr.msg_namelen = 2;
    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(-1, status);
    //Set back to real address
    msghdr.msg_namelen = sizeof(ns_address_t);

    //Test receive ancillary data
    uint8_t ancillary_databuffer[NS_CMSG_SPACE(sizeof(ns_in6_pktinfo_t)) + NS_CMSG_SPACE(sizeof(int16_t)) + NS_CMSG_SPACE(sizeof(int16_t))];
    msghdr.msg_control = ancillary_databuffer;
    msghdr.msg_controllen = sizeof(ancillary_databuffer);
    //Test to give buffer without order data
    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(0, msghdr.msg_controllen);
    // Now set the ancillary data size
    msghdr.msg_controllen = sizeof(ancillary_databuffer);

    //Request PKtInfo
    int8_t recvr_enable = true;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &recvr_enable, sizeof(int8_t));
    BYTES_EQUAL(0, status);

    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(NS_CMSG_SPACE(sizeof(ns_in6_pktinfo_t)), msghdr.msg_controllen);

    ns_cmsghdr_t *cmsg = NS_CMSG_FIRSTHDR(&msghdr);
    uint8_t *temp_ptr;
    ns_in6_pktinfo_t *packetinfo;
    int status_compare;
    int16_t data_int16;
    uint8_t data_u8;
    //Stay at while when full ancillary data is analyzed
    while (cmsg ) {
        BYTES_EQUAL(SOCKET_IPPROTO_IPV6, cmsg->cmsg_level);

        temp_ptr = NS_CMSG_DATA(cmsg);
        switch (cmsg->cmsg_type) {
            case SOCKET_IPV6_PKTINFO:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(ns_in6_pktinfo_t)), cmsg->cmsg_len);
                packetinfo = (ns_in6_pktinfo_t*)temp_ptr;

                status_compare = memcmp(packetinfo->ipi6_addr, read_buffer->dst_sa.address, 16);

                BYTES_EQUAL(0, status_compare);
                BYTES_EQUAL(read_buffer->interface->id, packetinfo->ipi6_ifindex);
                break;

            default:
                break;
        }

        cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
    }
    msghdr.msg_controllen = sizeof(ancillary_databuffer);
    recvr_enable = false;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &recvr_enable, sizeof(int8_t));
    recvr_enable = true;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVTCLASS, &recvr_enable, sizeof(int8_t));
    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(NS_CMSG_SPACE(sizeof(int16_t)), msghdr.msg_controllen);

    cmsg = NS_CMSG_FIRSTHDR(&msghdr);
    //Stay at while when full ancillary data is analyzed
    while (cmsg ) {
        BYTES_EQUAL(SOCKET_IPPROTO_IPV6, cmsg->cmsg_level);

        temp_ptr = NS_CMSG_DATA(cmsg);
        switch (cmsg->cmsg_type) {
            case SOCKET_IPV6_TCLASS: {
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = *(int16_t*)temp_ptr;
                data_u8 = (uint8_t)data_int16;
                BYTES_EQUAL(read_buffer->options.traffic_class, data_u8);
                break;
            }

            default:
                break;
        }

        cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
    }

    recvr_enable = false;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVTCLASS, &recvr_enable, sizeof(int8_t));
    recvr_enable = true;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVHOPLIMIT, &recvr_enable, sizeof(int8_t));

    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(NS_CMSG_SPACE(sizeof(int16_t)), msghdr.msg_controllen);

    cmsg = NS_CMSG_FIRSTHDR(&msghdr);
    //Stay at while when full ancillary data is analyzed
    while (cmsg ) {
        BYTES_EQUAL(SOCKET_IPPROTO_IPV6, cmsg->cmsg_level);

        temp_ptr = NS_CMSG_DATA(cmsg);
        switch (cmsg->cmsg_type) {
            case SOCKET_IPV6_RECVHOPLIMIT:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = common_read_16_bit(temp_ptr);
                data_u8 = data_int16;
                BYTES_EQUAL(read_buffer->options.hop_limit, data_u8);
                break;

            default:
                break;
        }

        cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
    }

    recvr_enable = true;
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVTCLASS, &recvr_enable, sizeof(int8_t));
    status = socket_setsockopt(sock_id, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_RECVPKTINFO, &recvr_enable, sizeof(int8_t));
    msghdr.msg_controllen = sizeof(ancillary_databuffer);
    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(sizeof(ancillary_databuffer), msghdr.msg_controllen);

    cmsg = NS_CMSG_FIRSTHDR(&msghdr);
    //Stay at while when full ancillary data is analyzed
    while (cmsg ) {
        BYTES_EQUAL(SOCKET_IPPROTO_IPV6, cmsg->cmsg_level);

        temp_ptr = NS_CMSG_DATA(cmsg);
        switch (cmsg->cmsg_type) {
            case SOCKET_IPV6_RECVHOPLIMIT:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = *(int16_t*)temp_ptr;
                data_u8 = data_int16;
                BYTES_EQUAL(read_buffer->options.hop_limit, data_u8);
                break;
            case SOCKET_IPV6_TCLASS:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = *(int16_t*)temp_ptr;
                data_u8 = data_int16;
                BYTES_EQUAL(read_buffer->options.traffic_class, data_u8);
                break;
            case SOCKET_IPV6_PKTINFO:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(ns_in6_pktinfo_t)), cmsg->cmsg_len);
                packetinfo = (ns_in6_pktinfo_t*)temp_ptr;

                status_compare = memcmp(packetinfo->ipi6_addr, read_buffer->dst_sa.address, 16);

                BYTES_EQUAL(0, status_compare);
                BYTES_EQUAL(read_buffer->interface->id, packetinfo->ipi6_ifindex);
                break;

            default:
                break;
        }

        cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
    }

    msghdr.msg_controllen = sizeof(ancillary_databuffer) - 1;
    status = socket_recvmsg(sock_id, &msghdr, NS_MSG_PEEK);

    BYTES_EQUAL(NS_MSG_CTRUNC, msghdr.msg_flags);
    BYTES_EQUAL(2, status);
    BYTES_EQUAL(NS_CMSG_SPACE(sizeof(ns_in6_pktinfo_t)) + NS_CMSG_SPACE(sizeof(int16_t)), msghdr.msg_controllen);

    cmsg = NS_CMSG_FIRSTHDR(&msghdr);
    //Stay at while when full ancillary data is analyzed
    while (cmsg ) {
        BYTES_EQUAL(SOCKET_IPPROTO_IPV6, cmsg->cmsg_level);

        temp_ptr = NS_CMSG_DATA(cmsg);
        switch (cmsg->cmsg_type) {
            case SOCKET_IPV6_RECVHOPLIMIT:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = *(int16_t*)temp_ptr;
                data_u8 = data_int16;
                BYTES_EQUAL(read_buffer->options.hop_limit, data_u8);
                break;
            case SOCKET_IPV6_TCLASS:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(int16_t)), cmsg->cmsg_len);
                data_int16 = *(int16_t*)temp_ptr;
                data_u8 = data_int16;
                BYTES_EQUAL(read_buffer->options.traffic_class, data_u8);
                break;
            case SOCKET_IPV6_PKTINFO:
                BYTES_EQUAL(NS_CMSG_LEN(sizeof(ns_in6_pktinfo_t)), cmsg->cmsg_len);
                packetinfo = (ns_in6_pktinfo_t*)temp_ptr;

                status_compare = memcmp(packetinfo->ipi6_addr, read_buffer->dst_sa.address, 16);

                BYTES_EQUAL(0, status_compare);
                BYTES_EQUAL(read_buffer->interface->id, packetinfo->ipi6_ifindex);
                break;

            default:
                break;
        }

        cmsg = NS_CMSG_NXTHDR(&msghdr, cmsg);
    }

}


