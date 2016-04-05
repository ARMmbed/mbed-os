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
#include <string.h>

Socket::Socket(NetworkInterface *iface, ns_protocol_t proto)
    : _iface(iface)
    , _proto(proto)
    , _socket(0)
{
    memset(_ip_address, 0, NS_IP_SIZE);
    _port = 0;
}

Socket::~Socket()
{
    if (_socket) {
        close();
    }
}


int32_t Socket::open(const char *address, uint16_t port)
{
    int32_t err;

    err = close();
    if (err) {
        return err;
    }

    err = _iface->getHostByName(address, _ip_address);
    _port = port;
    if (err) {
        return err;
    }

    _socket = _iface->createSocket(_proto);
    if (!_socket) {
        return NS_ERROR_NO_SOCKET;
    }

    err = _socket->open(_ip_address, _port);

    if (err) {
        _iface->destroySocket(_socket);
        _socket = 0;
    }

    return err;
}

int32_t Socket::close()
{
    if (!_socket) {
        return 0;
    }
    
    SocketInterface *socket = _socket;
    _socket = 0;

    int32_t err = socket->close();
    if (!err) {
        _iface->destroySocket(socket);
    }

    return err;
}

int32_t Socket::send(const void *data, uint32_t size)
{
    if (!_socket) {
        return NS_ERROR_NO_CONNECTION;
    }
    return _socket->send(data, size);
}

int32_t Socket::recv(void *data, uint32_t size, bool blocking)
{
    while (true) {
        if (!_socket) {
            return NS_ERROR_NO_CONNECTION;
        }
        
        int32_t recv = _socket->recv(data, size);

        if (recv != NS_ERROR_WOULD_BLOCK || !blocking) {
            return recv;
        }
    }
}


const char *Socket::getIPAddress() const
{
    if (_ip_address[0]) {
        return _ip_address;
    } else {
        return 0;
    }
}

uint16_t Socket::getPort() const
{
    return _port;
}

bool Socket::isConnected()
{
    if (!_socket) {
        return false;
    }

    return _socket->isConnected();
}

