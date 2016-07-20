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
#include "mbed_assert.h"

UDPSocket::UDPSocket()
    : _pending(0), _read_sem(0), _write_sem(0),
      _read_in_progress(false), _write_in_progress(false)
{
}

UDPSocket::~UDPSocket()
{
    close();
}

nsapi_protocol_t UDPSocket::get_proto()
{
    return NSAPI_UDP;
}

int UDPSocket::sendto(const char *host, uint16_t port, const void *data, unsigned size)
{
    SocketAddress address(_stack, host, port);
    if (!address) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    // sendto is thread safe
    return sendto(address, data, size);
}

int UDPSocket::sendto(const SocketAddress &address, const void *data, unsigned size)
{
    _lock.lock();
    int ret;

    // If this assert is hit then there are two threads
    // performing a send at the same time which is undefined
    // behavior
    MBED_ASSERT(!_write_in_progress);
    _write_in_progress = true;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        _pending = 0;
        int sent = _stack->socket_sendto(_socket, address, data, size);
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

    _write_in_progress = false;
    _lock.unlock();
    return ret;
}

int UDPSocket::recvfrom(SocketAddress *address, void *buffer, unsigned size)
{
    _lock.lock();
    int ret;

    // If this assert is hit then there are two threads
    // performing a recv at the same time which is undefined
    // behavior
    MBED_ASSERT(!_read_in_progress);
    _read_in_progress = true;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        }

        _pending = 0;
        int recv = _stack->socket_recvfrom(_socket, address, buffer, size);
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

    _read_in_progress = false;
    _lock.unlock();
    return ret;
}

void UDPSocket::event()
{
    int32_t wcount = _write_sem.wait(0);
    if (wcount <= 1) {
        _write_sem.release();
    }
    int32_t rcount = _read_sem.wait(0);
    if (rcount <= 1) {
        _read_sem.release();
    }

    _pending += 1;
    if (_callback && _pending == 1) {
        _callback();
    }
}
