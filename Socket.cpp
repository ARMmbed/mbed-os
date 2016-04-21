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

#include "Socket.h"

Socket::Socket()
    : _iface(0)
    , _socket(0)
    , _timeout(-1)
{
}

Socket::~Socket()
{
    if (_socket) {
        close();
    }
}

int Socket::open(NetworkStack *iface, nsapi_protocol_t proto)
{
    _iface = iface;

    void *socket;
    int err = _iface->socket_open(&socket, proto);
    if (err) {
        return err;
    }

    _socket = socket;
    _iface->socket_attach(_socket, &Socket::thunk, this);

    return 0;
}

int Socket::close()
{
    if (!_socket) {
        return 0;
    }
    
    _iface->socket_attach(_socket, 0, 0);
    
    void *volatile socket = _socket;
    _socket = 0;
    return _iface->socket_close(socket);
}

int Socket::bind(uint16_t port)
{
    SocketAddress addr(0, port);
    return bind(addr);
}

int Socket::bind(const char *address, uint16_t port)
{
    SocketAddress addr(address, port);
    return bind(addr);
}

int Socket::bind(const SocketAddress &address)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_bind(_socket, address);
}

void Socket::set_blocking(bool blocking)
{
    set_timeout(blocking ? -1 : 0);
}

void Socket::set_timeout(int timeout)
{
    _timeout = timeout;
}

int Socket::setsockopt(int level, int optname, const void *optval, unsigned optlen)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->setsockopt(_socket, level, optname, optval, optlen);
}

int Socket::getsockopt(int level, int optname, void *optval, unsigned *optlen)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->getsockopt(_socket, level, optname, optval, optlen);

}

void Socket::wakeup()
{
}

void Socket::thunk(void *data)
{
    Socket *self = (Socket *)data;
    if (self->_callback) {
        self->_callback();
    }
}

void Socket::attach(FunctionPointer callback)
{
    _callback = callback;
}
