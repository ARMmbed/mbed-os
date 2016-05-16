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
    , _timeout(osWaitForever)
{
}

Socket::~Socket()
{
    // Underlying close is thread safe
    close();
}

int Socket::open(NetworkStack *iface, nsapi_protocol_t proto)
{
    _lock.lock();

    if (_iface != NULL) {
        _lock.unlock();
        return NSAPI_ERROR_PARAMETER;
    }
    _iface = iface;

    void *socket;
    int err = _iface->socket_open(&socket, proto);
    if (err) {
        _lock.unlock();
        return err;
    }

    _socket = socket;
    _iface->socket_attach(_socket, &Socket::thunk, this);

    _lock.unlock();

    return 0;
}

int Socket::close()
{
    _lock.lock();

    int ret = 0;
    if (_socket) {
        _iface->socket_attach(_socket, 0, 0);

        void * socket = _socket;
        _socket = 0;
        ret = _iface->socket_close(socket);
    }

    // Wakeup anything in a blocking operation
    // on this socket
    socket_event();

    _lock.unlock();
    return ret;
}

int Socket::bind(uint16_t port)
{
    // Underlying bind is thread safe
    SocketAddress addr(0, port);
    return bind(addr);
}

int Socket::bind(const char *address, uint16_t port)
{
    // Underlying bind is thread safe
    SocketAddress addr(address, port);
    return bind(addr);
}

int Socket::bind(const SocketAddress &address)
{
    _lock.lock();

    int ret = NSAPI_ERROR_NO_SOCKET;
    if (_socket) {
        ret = _iface->socket_bind(_socket, address);
    }

    _lock.unlock();
    return ret;
}

void Socket::set_blocking(bool blocking)
{
    // Socket::set_timeout is thread safe
    set_timeout(blocking ? -1 : 0);
}

void Socket::set_timeout(int timeout)
{
    _lock.lock();

    if (timeout >= 0) {
        _timeout = (uint32_t)timeout;
    } else {
        _timeout = osWaitForever;
    }

    _lock.unlock();
}

int Socket::setsockopt(int level, int optname, const void *optval, unsigned optlen)
{
    _lock.lock();

    int ret = NSAPI_ERROR_NO_SOCKET;
    if (_socket) {
        ret = _iface->setsockopt(_socket, level, optname, optval, optlen);
    }

    _lock.unlock();
    return ret;
}

int Socket::getsockopt(int level, int optname, void *optval, unsigned *optlen)
{
    _lock.lock();

    int ret = NSAPI_ERROR_NO_SOCKET;
    if (_socket) {
        ret = _iface->getsockopt(_socket, level, optname, optval, optlen);
    }

    _lock.unlock();
    return ret;

}

void Socket::attach(Callback<void()> callback)
{
    _lock.lock();

    _callback = callback;

    _lock.unlock();
}

void Socket::thunk(void *data)
{
    Socket *self = (Socket *)data;
    self->socket_event();
}

void Socket::socket_event(void)
{
    if (_callback) {
        _callback();
    }
}
