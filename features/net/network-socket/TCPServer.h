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

#include "network-socket/Socket.h"
#include "network-socket/TCPSocket.h"
#include "network-socket/NetworkStack.h"
#include "network-socket/NetworkInterface.h"
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
    TCPServer(NetworkStack *stack);

    template <typename IF>
    TCPServer(IF *iface)
        : _pending(0), _accept_sem(0)
    {
        open(iface->get_stack());
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
    int listen(int backlog = 1);
    
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
     *  @return         0 on success, negative error code on failure
     */
    int accept(TCPSocket *connection);

protected:
    virtual nsapi_protocol_t get_proto();
    virtual void event();

    volatile unsigned _pending;
    rtos::Semaphore _accept_sem;
};


#endif
