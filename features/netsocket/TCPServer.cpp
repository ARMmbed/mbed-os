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
#include "mbed.h"

TCPServer::TCPServer()
    : _pending(0), _accept_sem(0)
{
}

TCPServer::~TCPServer()
{
    close();
}

nsapi_protocol_t TCPServer::get_proto()
{
    return NSAPI_TCP;
}

nsapi_error_t TCPServer::listen(int backlog)
{
    _lock.lock();
    nsapi_error_t ret;

    if (!_socket) {
        ret = NSAPI_ERROR_NO_SOCKET;
    } else {
        ret = _stack->socket_listen(_socket, backlog);
    }

    _lock.unlock();
    return ret;
}

nsapi_error_t TCPServer::accept(TCPSocket *connection, SocketAddress *address)
{
    _lock.lock();
    nsapi_error_t ret;

    while (true) {
        if (!_socket) {
            ret = NSAPI_ERROR_NO_SOCKET;
            break;
        } 

        _pending = 0;
        void *socket;
        ret = _stack->socket_accept(_socket, &socket, address);

        if (0 == ret) {
            connection->_lock.lock();

            if (connection->_socket) {
                connection->close();
            }

            connection->_stack = _stack;
            connection->_socket = socket;
            connection->_event = Callback<void()>(connection, &TCPSocket::event);
            _stack->socket_attach(socket, &Callback<void()>::thunk, &connection->_event);

            connection->_lock.unlock();
            break;
        } else if (NSAPI_ERROR_WOULD_BLOCK != ret) {
            break;
        } else {
            int32_t count;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            count = _accept_sem.wait(_timeout);
            _lock.lock();

            if (count < 1) {
                // Semaphore wait timed out so break out and return
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _lock.unlock();
    return ret;
}

void TCPServer::event()
{
    int32_t acount = _accept_sem.wait(0);
    if (acount <= 1) {
        _accept_sem.release();
    }

    _pending += 1;
    if (_callback && _pending == 1) {
        _callback();
    }
}
