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

#include "UDPSocket.h"
#include "Timer.h"

UDPSocket::UDPSocket(NetworkInterface *iface)
    : Socket(iface, NSAPI_UDP)
{
}

int UDPSocket::bind(uint16_t port)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_bind(_socket, port);
}

int UDPSocket::sendto(const SocketAddress &address, const void *data, unsigned size)
{
    mbed::Timer timer;
    timer.start();

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    
        int sent = _iface->socket_sendto(_socket, address, data, size);
        if (sent != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return sent;
        }
    }
}

int UDPSocket::recvfrom(SocketAddress *address, void *buffer, unsigned size)
{
    mbed::Timer timer;
    timer.start();

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    
        int recv = _iface->socket_recvfrom(_socket, address, buffer, size);
        if (recv != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return recv;
        }
    }
}


void UDPSocket::attach_send(FunctionPointer callback)
{
    _send_cb = callback;
    if (_socket && _send_cb) {
        return _iface->socket_attach_send(_socket, Socket::thunk, &_send_cb);
    } else if (_socket) {
        return _iface->socket_attach_send(_socket, 0, 0);
    }
}

void UDPSocket::attach_recv(FunctionPointer callback)
{
    _recv_cb = callback;
    if (_socket && _recv_cb) {
        return _iface->socket_attach_recv(_socket, Socket::thunk, &_recv_cb);
    } else if (_socket) {
        return _iface->socket_attach_recv(_socket, 0, 0);
    }
}

UDPSocket::~UDPSocket()
{
    if (_socket && _send_cb) {
        _iface->socket_attach_send(_socket, 0, 0);
    }

    if (_socket && _recv_cb) {
        _iface->socket_attach_recv(_socket, 0, 0);
    }
}
