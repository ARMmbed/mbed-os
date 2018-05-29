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

#include "AT_CellularStack.h"
#include "CellularUtil.h"
#include "CellularLog.h"

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularStack::AT_CellularStack(ATHandler &at, int cid, nsapi_ip_stack_t stack_type) : AT_CellularBase(at), _socket(NULL),_socket_count(0),_cid(cid), _stack_type(stack_type)
{
    memset(_ip,0, PDP_IPV6_SIZE);
}

AT_CellularStack::~AT_CellularStack()
{
    for (int i = 0; i < _socket_count; i++) {
        if (_socket[i]) {
            delete _socket[i];
            _socket[i] = NULL;
        }
    }
    _socket_count = 0;

    delete [] _socket;
    _socket = NULL;
}

/** NetworkStack
 */

const char * AT_CellularStack::get_ip_address()
{
    _at.lock();

    _at.cmd_start("AT+CGPADDR=");
    _at.write_int(_cid);
    _at.cmd_stop();

    _at.resp_start("+CGPADDR:");

    if (_at.info_resp()) {

        _at.skip_param();

        int len = _at.read_string(_ip, NSAPI_IPv4_SIZE-1);
        if (len == -1) {
            _ip[0] = '\0';
            _at.unlock();
            // no IPV4 address, return
            return NULL;
        }

        // in case stack type is not IPV4 only, try to look also for IPV6 address
        if (_stack_type != IPV4_STACK) {
            (void)_at.read_string(_ip, PDP_IPV6_SIZE-1);
        }
    }

    _at.resp_stop();
    _at.unlock();

    // we have at least IPV4 address
    convert_ipv6(_ip);

    return _ip;
}

nsapi_error_t AT_CellularStack::socket_stack_init()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    if (!is_protocol_supported(proto) || !handle) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    int max_socket_count = get_max_socket_count();

    if (!_socket) {
        if (socket_stack_init() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_NO_SOCKET;
        }

        _socket = new CellularSocket*[max_socket_count];
        if (!_socket) {
            tr_error("No memory to open socket!");
            return NSAPI_ERROR_NO_SOCKET;
        }
        _socket_count = max_socket_count;
        for (int i = 0; i < max_socket_count; i++) {
            _socket[i] = 0;
        }
    }

    int index = -1;
    for (int i = 0; i < max_socket_count; i++) {
        if (!_socket[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        tr_error("No socket found!");
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_info("Socket open index: %d", index);
    // create local socket structure, socket on modem is created when app calls sendto/recvfrom
    _socket[index] = new CellularSocket;
    CellularSocket *psock;
    psock = _socket[index];
    memset(psock, 0, sizeof(CellularSocket));
    SocketAddress addr(0, get_dynamic_ip_port());
    psock->id = index;
    psock->localAddress = addr;
    psock->proto = proto;
    *handle = psock;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_close(nsapi_socket_t handle)
{
    int err = NSAPI_ERROR_DEVICE_ERROR;

    struct CellularSocket *socket = (struct CellularSocket *)handle;
    if (!socket){
        return err;
    }
    int sock_id = socket->id;
    bool sock_created = socket->created;
    int max_socket_count = get_max_socket_count();

    int index = -1;
    for (int i = 0; i < max_socket_count; i++) {
        if (_socket[i] == socket) {
            index = i;
            break;
        }
    }

    tr_info("Close socket index: %d id: %d created: %d", index, sock_id, socket->created);

    if (index == -1) {
        tr_error("No socket found to be closed");
        return err;
    }

    _socket[index] = NULL;
    delete socket;
    err = NSAPI_ERROR_OK;

    // Close the socket on the modem if it was created
    _at.lock();
    if (sock_created) {
        err = socket_close_impl(sock_id);
    }
    _at.unlock();

    return err;
}

nsapi_error_t AT_CellularStack::socket_bind(nsapi_socket_t handle, const SocketAddress &addr)
{
    struct CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (addr) {
        socket->localAddress.set_addr(addr.get_addr());
    }

    if (addr.get_port()) {
        socket->localAddress.set_port(addr.get_port());
    }

    _at.lock();

    if (!socket->created) {
        create_socket_impl(socket);
    }

    return _at.unlock_return_error();
}

nsapi_error_t AT_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;;
}

nsapi_error_t AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    socket->remoteAddress = addr;
    socket->connected = true;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;;
}

nsapi_size_or_error_t AT_CellularStack::socket_send(nsapi_socket_t handle, const void *data, unsigned size)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket || !socket->connected) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    return socket_sendto(handle, socket->remoteAddress, data, size);
}

nsapi_size_or_error_t AT_CellularStack::socket_sendto(nsapi_socket_t handle, const SocketAddress &addr, const void *data, unsigned size)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    nsapi_size_or_error_t ret_val = NSAPI_ERROR_OK;

    if (!socket->created) {
        _at.lock();

        ret_val = create_socket_impl(socket);

        _at.unlock();
        if (ret_val != NSAPI_ERROR_OK) {
            tr_error("Error creating socket to send to: %s error code: %d", addr.get_ip_address(), ret_val);
            return ret_val;
        } else {
            tr_info("Success creating socket to send to: %s", addr.get_ip_address());
        }
    }

    unsigned max_packet_size = get_max_packet_size();

    /* Check parameters */
    if (addr.get_ip_version() == NSAPI_UNSPEC ||
            size > max_packet_size) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _at.lock();

    ret_val = socket_sendto_impl(socket, addr, data, size);
    
    if (ret_val <= 0) {
        tr_error("Error sending to: %s error code: %d", addr.get_ip_address(), ret_val);
    } else {
        tr_info("Success sending %d Bytes to: %s", ret_val, addr.get_ip_address());
    }

    _at.unlock();

    return ret_val;
}

nsapi_size_or_error_t AT_CellularStack::socket_recv(nsapi_socket_t handle, void *data, unsigned size)
{
    return socket_recvfrom(handle, NULL, data, size);
}

nsapi_size_or_error_t AT_CellularStack::socket_recvfrom(nsapi_socket_t handle, SocketAddress *addr, void *buffer, unsigned size)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    nsapi_size_or_error_t ret_val = NSAPI_ERROR_OK;

    if (!socket->created) {
        _at.lock();

        ret_val = create_socket_impl(socket);

        _at.unlock();
        if (ret_val != NSAPI_ERROR_OK) {
            return ret_val;
        }
    }

    _at.lock();

    ret_val = socket_recvfrom_impl(socket, addr, buffer, size);

    _at.unlock();

    return ret_val;
}

void AT_CellularStack::socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return;
    }
    socket->_cb = callback;
    socket->_data = data;
}
