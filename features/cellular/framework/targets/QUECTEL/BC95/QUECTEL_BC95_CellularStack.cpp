/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "QUECTEL_BC95_CellularStack.h"
#include "CellularUtil.h"
#include "CellularLog.h"

#define PACKET_SIZE_MAX 1358

using namespace mbed;
using namespace mbed_cellular_util;

QUECTEL_BC95_CellularStack::QUECTEL_BC95_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    _at.set_urc_handler("+NSONMI:", mbed::Callback<void()>(this, &QUECTEL_BC95_CellularStack::urc_nsonmi));
    _at.set_urc_handler("+NSOCLI:", mbed::Callback<void()>(this, &QUECTEL_BC95_CellularStack::urc_nsocli));
}

QUECTEL_BC95_CellularStack::~QUECTEL_BC95_CellularStack()
{
    _at.set_urc_handler("+NSONMI:", NULL);
    _at.set_urc_handler("+NSOCLI:", NULL);
}

nsapi_error_t QUECTEL_BC95_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_BC95_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_BC95_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &address)
{
    CellularSocket *socket = (CellularSocket *)handle;

    _at.lock();
    if (socket->id == -1) {
        const nsapi_error_t error_create = create_socket_impl(socket);
        if (error_create != NSAPI_ERROR_OK) {
            return error_create;
        }
    }

    _at.cmd_start("AT+NSOCO=");
    _at.write_int(socket->id);
    _at.write_string(address.get_ip_address(), false);
    _at.write_int(address.get_port());
    _at.cmd_stop_read_resp();

    _at.unlock();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        socket->remoteAddress = address;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

void QUECTEL_BC95_CellularStack::urc_nsonmi()
{
    int sock_id = _at.read_int();

    for (int i = 0; i < get_max_socket_count(); i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock->id == sock_id) {
            if (sock->_cb) {
                sock->_cb(sock->_data);
            }
            break;
        }
    }
}

void QUECTEL_BC95_CellularStack::urc_nsocli()
{
    int sock_id = _at.read_int();

    const nsapi_error_t err = _at.get_last_error();

    if (err != NSAPI_ERROR_OK) {
        return;
    }

    CellularSocket *sock = find_socket(sock_id);

    if (sock) {
        sock->closed = true;
        if (sock->_cb) {
            sock->_cb(sock->_data);
        }
        tr_info("Socket closed %d", sock_id);
    }
}


int QUECTEL_BC95_CellularStack::get_max_socket_count()
{
    return BC95_SOCKET_MAX;
}

bool QUECTEL_BC95_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP || protocol == NSAPI_TCP);
}

nsapi_error_t QUECTEL_BC95_CellularStack::socket_close_impl(int sock_id)
{
    CellularSocket *sock = find_socket(sock_id);

    if (sock && sock->closed) {
        return NSAPI_ERROR_OK;
    }
    nsapi_error_t err = _at.at_cmd_discard("+NSOCL", "=", "%d", sock_id);

    tr_info("Close socket: %d error: %d", sock_id, err);

    return err;
}

nsapi_error_t QUECTEL_BC95_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int sock_id = -1;
    bool socketOpenWorking = false;

    if (socket->proto == NSAPI_UDP) {
        _at.cmd_start_stop("+NSOCR", "=DGRAM,", "%d%d%d%s", 17, socket->localAddress.get_port(), 1, ((_ip_ver_sendto == NSAPI_IPv4) ? "AF_INET" : "AF_INET6"));
    } else if (socket->proto == NSAPI_TCP) {
        _at.cmd_start_stop("+NSOCR", "=STREAM,", "%d%d%d%s", 6, socket->localAddress.get_port(), 1, ((_ip_ver_sendto == NSAPI_IPv4) ? "AF_INET" : "AF_INET6"));
    } else {
        return NSAPI_ERROR_PARAMETER;
    }
    _at.resp_start();
    sock_id = _at.read_int();
    _at.resp_stop();

    socketOpenWorking = (_at.get_last_error() == NSAPI_ERROR_OK);

    if (!socketOpenWorking || (sock_id == -1)) {
        tr_error("Socket create failed!");
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_info("Socket create id: %d", sock_id);

    socket->id = sock_id;

    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t QUECTEL_BC95_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                     const void *data, nsapi_size_t size)
{
    //AT_CellularStack::socket_sendto(...) will create a socket on modem if it wasn't
    // open already.
    MBED_ASSERT(socket->id != -1);

    if (_ip_ver_sendto != address.get_ip_version()) {
        _ip_ver_sendto =  address.get_ip_version();
        socket_close_impl(socket->id);
        create_socket_impl(socket);
    }

    int sent_len = 0;

    if (size > PACKET_SIZE_MAX) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (socket->proto == NSAPI_UDP) {
        _at.cmd_start("AT+NSOST=");
        _at.write_int(socket->id);
        _at.write_string(address.get_ip_address(), false);
        _at.write_int(address.get_port());
        _at.write_int(size);
    } else if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+NSOSD=");
        _at.write_int(socket->id);
        _at.write_int(size);
    } else {
        return NSAPI_ERROR_PARAMETER;
    }

    _at.write_hex_string((char *)data, size);
    _at.cmd_stop();
    _at.resp_start();
    // skip socket id
    _at.skip_param();
    sent_len = _at.read_int();
    _at.resp_stop();

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        return sent_len;
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t QUECTEL_BC95_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                       void *buffer, nsapi_size_t size)
{
    //AT_CellularStack::socket_recvfrom(...) will create a socket on modem if it wasn't
    // open already.
    MBED_ASSERT(socket->id != -1);

    nsapi_size_or_error_t recv_len = 0;
    int port;
    char ip_address[NSAPI_IP_SIZE];

    _at.cmd_start_stop("+NSORF", "=", "%d%d", socket->id, size < PACKET_SIZE_MAX ? size : PACKET_SIZE_MAX);

    _at.resp_start();
    // receiving socket id
    _at.skip_param();
    _at.read_string(ip_address, sizeof(ip_address));
    port = _at.read_int();
    recv_len = _at.read_int();
    int hexlen = _at.read_hex_string((char *)buffer, size);
    // remaining length
    _at.skip_param();
    _at.resp_stop();

    if (!recv_len || (recv_len == -1) || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (address) {
        address->set_ip_address(ip_address);
        address->set_port(port);
    }

    if (recv_len != hexlen) {
        tr_error("Not received as much data as expected. Should receive: %d bytes, received: %d bytes", recv_len, hexlen);
    }
    return recv_len;
}
