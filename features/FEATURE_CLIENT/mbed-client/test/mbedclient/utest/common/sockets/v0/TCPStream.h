/*
 * PackageLicenseDeclared: Apache-2.0
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
#ifndef __MBED_NET_SOCKETS_V0_TCPSTREAM_H__
#define __MBED_NET_SOCKETS_V0_TCPSTREAM_H__
#include <stddef.h>
#include <stdint.h>
#include "Ticker.h"
#include "TCPAsynch.h"

namespace mbed {
namespace Sockets {
namespace v0 {

class TCPStream: public TCPAsynch {
public:
    typedef FunctionPointer1<void, TCPStream *> ConnectHandler_t;
    typedef FunctionPointer1<void, TCPStream *> DisconnectHandler_t;
    /**
     * TCP socket constructor.
     * Does not allocate an underlying TCP Socket instance.
     * @param[in] stack The network stack to use for this socket.
     */
    TCPStream(const socket_stack_t stack);
    /**
     * TCP socket constructor.
     * Does not allocate an underlying TCP Socket instance. This version is for use with
     * TCPListener::accept(). The struct socket instance passed into this constructor should
     * be a fully initialized socket, with an initialized impl field.  TCPStream will copy
     * the stack, API, protocol family, and impl pointer from sock.
     * @param[in] sock The TCP socket instance to use for this TCP socket.
     */
    TCPStream(const struct socket * sock);
    /**
     * TCP socket destructor
     */
    ~TCPStream();
    /**
     * Connect to a remote host.
     * Initates the TCP Connection process.  onConnect is called when the connection
     * is acknowledged.
     * @param[in] address The remote host to connect to
     * @param[in] port The remote port to connect to
     * @param[in] onConnect
     * @return SOCKET_ERROR_NONE on success, or an error code on failure
     */
    virtual socket_error_t connect(const SocketAddr &address, const uint16_t port,
            const ConnectHandler_t &onConnect);
    /**
     * Set a disconnect handler
     * This handler only needs to be configured once onConnect has been called
     * @param[in] h the handler to call when a connection is disconnected
     */
    virtual void setOnDisconnect(const DisconnectHandler_t &h) { _onDisconnect = h; }

protected:
    /**
     * Internal event handler
     * @param[in] ev the event to handle
     */
    void _eventHandler(struct socket_event *ev);

protected:
    ConnectHandler_t _onConnect;
    DisconnectHandler_t _onDisconnect;
};
} // namespace v0
} // namespace Sockets
} // namespace mbed
#endif // __MBED_NET_SOCKETS_V0_TCPSTREAM_H__
