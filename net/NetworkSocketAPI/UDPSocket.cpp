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

UDPSocket::UDPSocket()
{
}

UDPSocket::UDPSocket(NetworkStack *iface)
{
    open(iface);
}

int UDPSocket::open(NetworkStack *iface)
{
    return Socket::open(iface, NSAPI_UDP);
}

int UDPSocket::sendto(const char *host, uint16_t port, const void *data, unsigned size)
{
    SocketAddress addr(_iface, host, port);
    if (!addr) {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    return sendto(addr, data, size);
}

int UDPSocket::sendto(const SocketAddress &address, const void *data, unsigned size)
{
    mbed::Timer timer;
    timer.start();
    mbed::Timeout timeout;
    if (_timeout >= 0) {
        timeout.attach_us(&Socket::wakeup, _timeout * 1000);
    }

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    
        int sent = _iface->socket_sendto(_socket, address, data, size);
        if (sent != NSAPI_ERROR_WOULD_BLOCK
            || (_timeout >= 0 && timer.read_ms() >= _timeout)) {
            return sent;
        }

        __WFI();
    }
}

int UDPSocket::recvfrom(SocketAddress *address, void *buffer, unsigned size)
{
    mbed::Timer timer;
    timer.start();
    mbed::Timeout timeout;
    if (_timeout >= 0) {
        timeout.attach_us(&Socket::wakeup, _timeout * 1000);
    }

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    
        int recv = _iface->socket_recvfrom(_socket, address, buffer, size);
        if (recv != NSAPI_ERROR_WOULD_BLOCK
            || (_timeout >= 0 && timer.read_ms() >= _timeout)) {
            return recv;
        }

        __WFI();
    }
}
