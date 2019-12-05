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
#include "AT_CellularDevice.h"

using namespace mbed_cellular_util;
using namespace mbed;

AT_CellularStack::AT_CellularStack(ATHandler &at, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device) :
    _socket(NULL), _cid(cid),
    _stack_type(stack_type), _ip_ver_sendto(NSAPI_UNSPEC), _at(at), _device(device)
{
    memset(_ip, 0, PDP_IPV6_SIZE);

    MBED_ASSERT(_device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT) > 0);
    _socket = new CellularSocket *[_device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT)]();
}

AT_CellularStack::~AT_CellularStack()
{
    for (int i = 0; i < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT); i++) {
        if (_socket[i]) {
            delete _socket[i];
        }
    }
    delete [] _socket;
}

int AT_CellularStack::find_socket_index(nsapi_socket_t handle)
{
    if (!_socket) {
        return -1;
    }
    for (int i = 0; i < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT); i++) {
        if (_socket[i] == handle) {
            return i;
        }
    }
    return -1;
}

/** NetworkStack
 */
nsapi_error_t AT_CellularStack::get_ip_address(SocketAddress *address)
{
    if (!address) {
        return NSAPI_ERROR_PARAMETER;
    }
    _at.lock();

    bool ipv4 = false, ipv6 = false;

    _at.cmd_start_stop("+CGPADDR", "=", "%d", _cid);
    _at.resp_start("+CGPADDR:");

    if (_at.info_resp()) {
        _at.skip_param();

        if (_at.read_string(_ip, PDP_IPV6_SIZE) != -1) {
            convert_ipv6(_ip);
            address->set_ip_address(_ip);

            ipv4 = (address->get_ip_version() == NSAPI_IPv4);
            ipv6 = (address->get_ip_version() == NSAPI_IPv6);

            // Try to look for second address ONLY if modem has support for dual stack(can handle both IPv4 and IPv6 simultaneously).
            // Otherwise assumption is that second address is not reliable, even if network provides one.
            if ((_device.get_property(AT_CellularDevice::PROPERTY_IPV4V6_PDP_TYPE) && (_at.read_string(_ip, PDP_IPV6_SIZE) != -1))) {
                convert_ipv6(_ip);
                address->set_ip_address(_ip);
                ipv6 = (address->get_ip_version() == NSAPI_IPv6);
            }
        }
    }
    _at.resp_stop();
    _at.unlock();

    if (ipv4 && ipv6) {
        _stack_type = IPV4V6_STACK;
    } else if (ipv4) {
        _stack_type = IPV4_STACK;
    } else if (ipv6) {
        _stack_type = IPV6_STACK;
    }

    return (ipv4 || ipv6) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_ADDRESS;
}

void AT_CellularStack::set_cid(int cid)
{
    _cid = cid;
}

nsapi_error_t AT_CellularStack::socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto)
{
    if (!handle) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    if (proto == NSAPI_UDP) {
        if (!_device.get_property(AT_CellularDevice::PROPERTY_IP_UDP)) {
            return NSAPI_ERROR_UNSUPPORTED;
        }
    } else if (proto == NSAPI_TCP) {
        if (!_device.get_property(AT_CellularDevice::PROPERTY_IP_TCP)) {
            return NSAPI_ERROR_UNSUPPORTED;
        }
    } else {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _socket_mutex.lock();

    int index = find_socket_index(0);
    if (index == -1) {
        tr_warn("No free sockets!");
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
        return NSAPI_ERROR_NO_SOCKET;
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

    _socket_mutex.lock();
    _socket[index] = NULL;
    delete socket;
    _socket_mutex.unlock();

    _at.unlock();

    return err;
}

nsapi_error_t AT_CellularStack::socket_bind(nsapi_socket_t handle, const SocketAddress &addr)
{
    struct CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
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
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    socket->remoteAddress = addr;
    socket->connected = true;

    return NSAPI_ERROR_OK;
}

nsapi_error_t AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_size_or_error_t AT_CellularStack::socket_send(nsapi_socket_t handle, const void *data, unsigned size)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (!socket->connected) {
        return NSAPI_ERROR_NO_CONNECTION;
    }
    return socket_sendto(handle, socket->remoteAddress, data, size);
}

nsapi_size_or_error_t AT_CellularStack::socket_sendto(nsapi_socket_t handle, const SocketAddress &addr, const void *data, unsigned size)
{
    CellularSocket *socket = (CellularSocket *)handle;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (socket->closed && !socket->pending_bytes) {
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

        /* Check that stack type supports sendto address type*/
        if (!is_addr_stack_compatible(addr)) {
            return NSAPI_ERROR_PARAMETER;
        }

        _ip_ver_sendto = addr.get_ip_version();
        _at.lock();

        ret_val = create_socket_impl(socket);

        _at.unlock();
        if (ret_val != NSAPI_ERROR_OK) {
            tr_error("Socket %d create %s error %d", find_socket_index(socket), addr.get_ip_address(), ret_val);
            return ret_val;
        }
    }

    /* Check parameters - sendto address is valid and stack type supports sending to that address type*/
    if (!is_addr_stack_compatible(addr)) {
        return NSAPI_ERROR_PARAMETER;
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
        return NSAPI_ERROR_NO_SOCKET;
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
    for (int i = 0; i < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT); i++) {
        if (_socket[i] != 0) {
            if (_socket[i]->localAddress.get_port() == port) {
                return i;
            }
        }
    }
    return -1;
}

AT_CellularStack::CellularSocket *AT_CellularStack::find_socket(int sock_id)
{
    CellularSocket *sock = NULL;
    for (int i = 0; i < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT); i++) {
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

bool AT_CellularStack::is_addr_stack_compatible(const SocketAddress &addr)
{
    if ((addr.get_ip_version() == NSAPI_UNSPEC) ||
            (addr.get_ip_version() == NSAPI_IPv4 && _stack_type == IPV6_STACK) ||
            (addr.get_ip_version() == NSAPI_IPv6 && _stack_type == IPV4_STACK)) {
        return false;
    }
    return true;
}

