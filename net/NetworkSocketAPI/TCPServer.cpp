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

TCPServer::TCPServer(NetworkInterface *iface)
    : Socket(iface, NSAPI_TCP)
{
}

int TCPServer::bind(uint16_t port)
{
    if (!_socket) {
        return NSAPI_ERROR_NO_SOCKET;   
    }

    return _iface->socket_bind(_socket, port);
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

    void *socket = connection->_socket;
    connection->_socket = 0;
    _iface->socket_destroy(socket);

    while (true) {
        if (!_socket) {
            return NSAPI_ERROR_NO_SOCKET;   
        }

        int err = _iface->socket_accept(_socket, &socket);

        if (err > 0) {
            connection->_socket = socket;
        }

        if (err != NSAPI_ERROR_WOULD_BLOCK || !_blocking || 
            (_timeout && timer.read_ms() > _timeout)) {
            return err;
        }
    }
}


void TCPServer::attach_accept(FunctionPointer callback)
{
    _accept_cb = callback;

    if (_socket && _accept_cb) {
        return _iface->socket_attach_accept(_socket, Socket::thunk, &_accept_cb);
    } else if (_socket) {
        return _iface->socket_attach_accept(_socket, 0, 0);
    }
}

TCPServer::~TCPServer()
{
    if (_socket && _accept_cb) {
        _iface->socket_attach_accept(_socket, 0, 0);
    }
}
