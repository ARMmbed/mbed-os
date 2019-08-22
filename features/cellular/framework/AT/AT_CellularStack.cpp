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
#include "ThisThread.h"

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularStack::AT_CellularStack(ATHandler &at, int cid, nsapi_ip_stack_t stack_type) : AT_CellularBase(at), _socket(NULL), _socket_count(0), _cid(cid), _stack_type(stack_type)
{
    memset(_ip, 0, PDP_IPV6_SIZE);
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

int AT_CellularStack::find_socket_index(nsapi_socket_t handle)
{
    int max_socket_count = get_max_socket_count();
    for (int i = 0; i < max_socket_count; i++) {
        if (_socket[i] == handle) {
            return i;
        }
    }
    return -1;
}

/** NetworkStack
 */
const char *AT_CellularStack::get_ip_address()
{
    _at.lock();

    _at.cmd_start_stop("+CGPADDR", "=", "%d", _cid);
    _at.resp_start("+CGPADDR:");

    int len = -1;
    if (_at.info_resp()) {
        _at.skip_param();

        len = _at.read_string(_ip, PDP_IPV6_SIZE);

        if (len != -1 && _stack_type != IPV4_STACK) {
            // in case stack type is not IPV4 only, try to look also for IPV6 address
            (void)_at.read_string(_ip, PDP_IPV6_SIZE);
        }
    }
    _at.resp_stop();
    _at.unlock();

    // we have at least IPV4 address
    convert_ipv6(_ip);

    return len != -1 ? _ip : NULL;
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

    _socket_mutex.lock();

    if (!_socket) {
        if (socket_stack_init() != NSAPI_ERROR_OK) {
            _socket_mutex.unlock();
            return NSAPI_ERROR_NO_SOCKET;
        }

        _socket = new CellularSocket*[max_socket_count];
        _socket_count = max_socket_count;
        for (int i = 0; i < max_socket_count; i++) {
            _socket[i] = 0;
        }
    }

    int index = find_socket_index(0);
    if (index == -1) {
        tr_error("No free sockets!");
        _socket_mutex.unlock();
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_info("Socket %d open", index);
    // create local socket structure, socket on modem is created when app calls sendto/recvfrom
    // Do not assign a socket ID yet. Socket is not created at the Modem yet.
    // create_socket_impl(handle) will assign the correct socket ID.
    _socket[index] = new CellularSocket;
    CellularSocket *psock = _socket[index];
    SocketAddress addr(0, get_dynamic_ip_port());

    psock->localAddress = addr;
    psock->proto = proto;
    *handle = psock;

    _socket_mutex.unlock();

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_close(nsapi_socket_t handle)
{
    int err = NSAPI_ERROR_DEVICE_ERROR;

    struct CellularSocket *socket = (struct CellularSocket *)handle;
    if (!socket) {
        return err;
    }
    int sock_id = socket->id;

    int index = find_socket_index(handle);
    if (index == -1) {
        tr_error("No socket found to be closed");
        return err;
    }

    err = NSAPI_ERROR_OK;

    // Close the socket on the modem if it was created
    _at.lock();
    if (sock_id > -1) {
        err = socket_close_impl(sock_id);
    }

    if (!err) {
        tr_info("Socket %d closed", index);
    } else {
        tr_info("Socket %d close (id %d, started %d, error %d)", index, sock_id, socket->started, err);
    }

    _socket[index] = NULL;
    delete socket;

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
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _at.lock();

    uint16_t port = addr.get_port();
    if (port != socket->localAddress.get_port()) {
        if (port && (get_socket_index_by_port(port) == -1)) {
            socket->localAddress.set_port(port);
        } else {
            _at.unlock();
            return NSAPI_ERROR_PARAMETER;
        }
    }

    if (socket->id == -1) {
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

    if (socket->closed && !socket->rx_avail) {
        tr_info("sendto socket %d closed", socket->id);
        return NSAPI_ERROR_NO_CONNECTION;
    }

    if (size == 0) {
        if (socket->proto == NSAPI_UDP) {
            return NSAPI_ERROR_UNSUPPORTED;
        } else if (socket->proto == NSAPI_TCP) {
            return 0;
        }
    }

    nsapi_size_or_error_t ret_val = NSAPI_ERROR_OK;

    if (socket->id == -1) {
        _at.lock();

        ret_val = create_socket_impl(socket);

        _at.unlock();
        if (ret_val != NSAPI_ERROR_OK) {
            tr_error("Socket %d create %s error %d", find_socket_index(socket), addr.get_ip_address(), ret_val);
            return ret_val;
        }
    }

    /* Check parameters */
    if (addr.get_ip_version() == NSAPI_UNSPEC) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _at.lock();

    ret_val = socket_sendto_impl(socket, addr, data, size);

    _at.unlock();

    if (ret_val >= 0) {
        tr_info("Socket %d sent %d bytes to %s port %d", find_socket_index(socket), ret_val, addr.get_ip_address(), addr.get_port());
    } else if (ret_val != NSAPI_ERROR_WOULD_BLOCK) {
        tr_error("Socket %d sendto %s error %d", find_socket_index(socket), addr.get_ip_address(), ret_val);
    }

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

    if (socket->closed) {
        tr_info("recvfrom socket %d closed", socket->id);
        return 0;
    }

    nsapi_size_or_error_t ret_val = NSAPI_ERROR_OK;

    if (socket->id == -1) {
        _at.lock();

        ret_val = create_socket_impl(socket);

        _at.unlock();
        if (ret_val != NSAPI_ERROR_OK) {
            tr_error("Socket %d create error %d", find_socket_index(socket), ret_val);
            return ret_val;
        }
    }

    _at.lock();

    ret_val = socket_recvfrom_impl(socket, addr, buffer, size);

    _at.unlock();

    if (socket->closed) {
        tr_info("recvfrom socket %d closed", socket->id);
        return 0;
    }

    if (ret_val >= 0) {
        if (addr) {
            tr_info("Socket %d recv %d bytes from %s port %d", find_socket_index(socket), ret_val, addr->get_ip_address(), addr->get_port());
        } else {
            tr_info("Socket %d recv %d bytes", find_socket_index(socket), ret_val);
        }
    } else if (ret_val != NSAPI_ERROR_WOULD_BLOCK) {
        tr_error("Socket %d recv error %d", find_socket_index(socket), ret_val);
    }

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

int AT_CellularStack::get_socket_index_by_port(uint16_t port)
{
    int max_socket_count = get_max_socket_count();
    for (int i = 0; i < max_socket_count; i++) {
        if (_socket[i]->localAddress.get_port() == port) {
            return i;
        }
    }
    return -1;
}

AT_CellularStack::CellularSocket *AT_CellularStack::find_socket(int sock_id)
{
    CellularSocket *sock = NULL;
    for (int i = 0; i < _socket_count; i++) {
        if (_socket[i] && _socket[i]->id == sock_id) {
            sock = _socket[i];
            break;
        }
    }
    if (!sock) {
        tr_error("Socket not found %d", sock_id);
    }
    return sock;
}
