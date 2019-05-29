/*
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

/** @file TCPServer.h Deprecated TCPServer class */
/** \addtogroup netsocket
 * @{*/

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "netsocket/InternetSocket.h"
#include "netsocket/TCPSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/NetworkInterface.h"


/** TCP socket server
 */
class TCPServer : public TCPSocket {
public:
    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.10",
                          "TCPServer is deprecated, use TCPSocket")
    TCPServer();

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *
     *  @param stack    Network stack as target for socket
     */
    template <typename S>
    MBED_DEPRECATED_SINCE("mbed-os-5.10",
                          "TCPServer is deprecated, use TCPSocket")
    TCPServer(S *stack)
    {
        open(stack);
    }

    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~TCPServer();

    // Allow legacy TCPServer::accept() to override inherited Socket::accept()
    using TCPSocket::accept;

    /** Accepts a connection on a TCP socket
     *
     *  The server socket must be bound and set to listen for connections.
     *  On a new connection, creates a network socket using the specified
     *  socket instance.
     *
     *  By default, accept blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param connection TCPSocket instance that will handle the incoming connection.
     *  @param address    Destination for the remote address or NULL
     *  @return           0 on success, negative error code on failure
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.10",
                          "TCPServer::accept() is deprecated, use Socket *Socket::accept() instead")
    nsapi_error_t accept(TCPSocket *connection, SocketAddress *address = NULL);
};

#endif

/** @} */
