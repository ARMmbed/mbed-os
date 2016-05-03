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

TCPSocket::TCPSocket(): _read_sem(0), _write_sem(0)
{
}

TCPSocket::TCPSocket(NetworkStack *iface): _read_sem(0), _write_sem(0)
{
    // TCPSocket::open is thread safe
    open(iface);
}

int TCPSocket::open(NetworkStack *iface)
{
    // Socket::open is thread safe
    return Socket::open(iface, NSAPI_TCP);
}

int TCPSocket::connect(const SocketAddress &addr)
{
    _lock.lock();

    int ret = NSAPI_ERROR_NO_SOCKET;
    if (_socket) {
        ret = _iface->socket_connect(_socket, addr);
    }

    _lock.unlock();
    return ret;
}

int TCPSocket::connect(const char *host, uint16_t port)
{
    _lock.lock();

    SocketAddress addr(_iface, host, port);
    int ret = NSAPI_ERROR_DNS_FAILURE;
    if (addr) {
        ret = connect(addr);
    }

    _lock.unlock();
    return ret;
}

int TCPSocket::send(const void *data, unsigned size)
{
    if (osOK != _write_lock.lock(_timeout)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    _lock.lock();

    int ret;
    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        int sent = _iface->socket_send(_socket, data, size);
        if ((0 == _timeout) || (NSAPI_ERROR_WOULD_BLOCK != sent)) {
            ret = sent;
            break;
        } else {
            int32_t count;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            count = _write_sem.wait(_timeout);
            _lock.lock();

            if (count < 1) {
                // Semaphore wait timed out so break out and return
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _lock.unlock();
    _write_lock.unlock();
    return ret;
}

int TCPSocket::recv(void *data, unsigned size)
{
    if (osOK != _read_lock.lock(_timeout)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    _lock.lock();

    int ret;
    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        int recv = _iface->socket_recv(_socket, data, size);
        if ((0 == _timeout) || (NSAPI_ERROR_WOULD_BLOCK != recv)) {
            ret = recv;
            break;
        } else {
            int32_t count;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            count = _read_sem.wait(_timeout);
            _lock.lock();

            if (count < 1) {
                // Semaphore wait timed out so break out and return
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _lock.unlock();
    _read_lock.unlock();
    return ret;
}

void TCPSocket::socket_event()
{
    int32_t count;
    count = _write_sem.wait(0);
    if (count <= 1) {
        _write_sem.release();
    }
    count = _read_sem.wait(0);
    if (count <= 1) {
        _read_sem.release();
    }

    Socket::socket_event();
}
