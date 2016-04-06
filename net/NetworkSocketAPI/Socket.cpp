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

Socket::Socket(NetworkInterface *iface, nsapi_protocol_t proto)
    : _iface(iface)
    , _blocking(true)
    , _timeout(0)
{
    _socket = _iface->socket_create(proto);
}

Socket::~Socket()
{
    if (_socket) {
        close(false);
    }
}

void Socket::set_blocking(bool blocking)
{
    _blocking = blocking;
}

void Socket::set_timeout(unsigned timeout)
{
    _timeout = timeout;
}

int Socket::set_option(int optname, const void *optval, unsigned int optlen)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_set_option(_socket, optname, optval, optlen);
}

int Socket::get_option(int optname, void *optval, unsigned int *optlen)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_get_option(_socket, optname, optval, optlen);
}

int Socket::close(bool shutdown)
{
    if (!_socket) {
        return 0;
    }

    int err = _iface->socket_close(_socket, shutdown);
    if (!err) {
        void *socket = _socket;
        _socket = 0;
        _iface->socket_destroy(socket);
    }

    return err;
}

void Socket::thunk(void *p) 
{
    mbed::FuncPtr<void()> *fptr = (mbed::FuncPtr<void()> *)p;
    (*fptr)();
}
