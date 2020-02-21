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

TCPServer::TCPServer()
{
    _socket_stats.stats_update_proto(this, NSAPI_TCP);
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

        core_util_atomic_flag_clear(&_pending);
        void *socket;
        ret = _stack->socket_accept(_socket, &socket, address);

        if (0 == ret) {
            connection->_lock.lock();

            if (connection->_socket) {
                connection->close();
            }

            connection->_stack = _stack;
            connection->_socket = socket;
            connection->_event = { connection, &TCPSocket::event };
            _stack->socket_attach(socket, connection->_event.thunk, &connection->_event);
            _socket_stats.stats_update_peer(connection, *address);
            _socket_stats.stats_update_socket_state(connection, SOCK_CONNECTED);
            connection->_lock.unlock();
            break;
        } else if ((_timeout == 0) || (ret != NSAPI_ERROR_WOULD_BLOCK)) {
            break;
        } else {
            uint32_t flag;

            // Release lock before blocking so other threads
            // accessing this object aren't blocked
            _lock.unlock();
            flag = _event_flag.wait_any(READ_FLAG, _timeout);
            _lock.lock();

            if (flag & osFlagsError) {
                // Timeout break
                ret = NSAPI_ERROR_WOULD_BLOCK;
                break;
            }
        }
    }

    _lock.unlock();
    return ret;
}
