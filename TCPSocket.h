/* TCPSocket
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

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "NetworkSocketAPI/Socket.h"
#include "NetworkSocketAPI/NetworkStack.h"
#include "NetworkSocketAPI/NetworkInterface.h"
#include "rtos/Semaphore.h"


/** TCP socket connection
 */
class TCPSocket : public Socket {
public:
    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    TCPSocket();

    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~TCPSocket();

    /** Create a socket on a network stack
     *
     *  Creates and opens a socket on the specified network stack.
     *
     *  @param iface    Network stack as target for socket
     */
    TCPSocket(NetworkStack *iface);

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *
     *  @param iface    Network interface as target for socket
     */
    TCPSocket(NetworkInterface *iface);

    /** Opens a socket
     *
     *  Creates a network socket on the specified network stack.
     *  Not needed if stack is passed to the socket's constructor.
     *
     *  @param iface    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(NetworkStack *iface);

    /** Opens a socket
     *
     *  Creates a network socket on the network stack of the given
     *  network interface. Not needed if stack is passed to the
     *  socket's constructor.
     *
     *  @param iface    Network interface as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(NetworkInterface *iface);

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by either
     *  a domain name or an IP address and a port.
     *
     *  @param host     Hostname of the remote host
     *  @param port     Port of the remote host
     *  @return         0 on success, negative error code on failure
     */
    int connect(const char *host, uint16_t port);

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by the
     *  indicated address.
     *
     *  @param address  The SocketAddress of the remote host
     *  @return         0 on success, negative error code on failure
     */
    int connect(const SocketAddress &address);
    
    /** Send data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes sent from the buffer.
     *
     *  By default, send blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    int send(const void *data, unsigned size);
    
    /** Receive data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes received into the buffer.
     *
     *  By default, recv blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    int recv(void *data, unsigned size);

protected:
    virtual void event();
    volatile unsigned _pending;
    rtos::Semaphore _read_sem;
    rtos::Semaphore _write_sem;
    bool _read_in_progress;
    bool _write_in_progress;
    friend class TCPServer;
};


#endif
