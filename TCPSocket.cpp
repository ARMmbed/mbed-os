/* Socket
 * Copyright (c) 2015 ARM Limited
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

#include "TCPSocket.h"
#include "Timer.h"

TCPSocket::TCPSocket(NetworkInterface *iface)
    : Socket(iface, NSAPI_TCP)
{
}

int TCPSocket::connect(const SocketAddress &addr)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_connect(_socket, addr);
}

int TCPSocket::connect(const char *host, uint16_t port)
{
    SocketAddress addr(_iface, host, port);
    if (!addr.get_ip_address()) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    return connect(addr);
}

bool TCPSocket::is_connected()
{
    return _socket && _iface->socket_is_connected(_socket);
}

int TCPSocket::send(const void *data, unsigned size)
{
    mbed::Timer timer;
    timer.start();

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }

        int sent = _iface->socket_send(_socket, data, size);
        if (sent != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return sent;
        }
    }
}

int TCPSocket::recv(void *data, unsigned size)
{
    mbed::Timer timer;
    timer.start();

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    
        int recv = _iface->socket_recv(_socket, data, size);
        if (recv != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return recv;
        }
    }
}


void TCPSocket::attach_send(FunctionPointer callback)
{
    _send_cb = callback;

    if (_socket && _send_cb) {
        return _iface->socket_attach_send(_socket, Socket::thunk, &_send_cb);
    } else if (_socket) {
        return _iface->socket_attach_send(_socket, 0, 0);
    }
}

void TCPSocket::attach_recv(FunctionPointer callback)
{
    _recv_cb = callback;

    if (_socket && _recv_cb) {
        return _iface->socket_attach_recv(_socket, Socket::thunk, &_recv_cb);
    } else if (_socket) {
        return _iface->socket_attach_recv(_socket, 0, 0);
    }
}

TCPSocket::~TCPSocket()
{
    if (_socket && _send_cb) {
        _iface->socket_attach_send(_socket, 0, 0);
    }

    if (_socket && _recv_cb) {
        _iface->socket_attach_recv(_socket, 0, 0);
    }
}
