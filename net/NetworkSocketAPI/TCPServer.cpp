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

#include "TCPServer.h"
#include "Timer.h"

TCPServer::TCPServer()
{
}

TCPServer::TCPServer(NetworkInterface *iface)
{
    open(iface);
}

int TCPServer::open(NetworkInterface *iface)
{
    return Socket::open(iface, NSAPI_TCP);
}

int TCPServer::bind(uint16_t port)
{
    SocketAddress addr(0, port);
    return bind(addr);
}

int TCPServer::bind(const char *address, uint16_t port)
{
    SocketAddress addr(address, port);
    return bind(addr);
}

int TCPServer::bind(const SocketAddress &address)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return _iface->socket_bind(_socket, address);
}

int TCPServer::listen(int backlog)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;   
    }

    return _iface->socket_listen(_socket, backlog);
}

int TCPServer::accept(TCPSocket *connection)
{
    mbed::Timer timer;
    timer.start();

    if (connection->_socket) {
        connection->close();
    }

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;   
        }

        void *socket;
        int err = _iface->socket_accept(&socket, _socket);
        if (!err) {
            connection->_socket = socket;
        }

        if (err != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return err;
        }
    }
}
