
/** \addtogroup netsocket */
/** @{*/
/* TCPServer
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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "netsocket/Socket.h"
#include "netsocket/TCPSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/NetworkInterface.h"
#include "rtos/Semaphore.h"


/** TCP socket server
  */
class TCPServer : public Socket {
public:
    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    TCPServer();

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *
     *  @param stack    Network stack as target for socket
     */
    template <typename S>
    TCPServer(S *stack)
        : _pending(0), _accept_sem(0)
    {
        open(stack);
    }

    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~TCPServer();

    /** Listen for connections on a TCP socket
     *
     *  Marks the socket as a passive socket that can be used to accept
     *  incoming connections.
     *
     *  @param backlog  Number of pending connections that can be queued
     *                  simultaneously, defaults to 1
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t listen(int backlog = 1);
    
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
     *  @param socket   TCPSocket instance that will handle the incoming connection.
     *  @param address  Destination for the remote address or NULL
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t accept(TCPSocket *connection, SocketAddress *address = NULL);

protected:
    virtual nsapi_protocol_t get_proto();
    virtual void event();

    volatile unsigned _pending;
    rtos::Semaphore _accept_sem;
};


#endif

/** @}*/
