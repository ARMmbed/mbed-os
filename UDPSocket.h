/* UDPSocket
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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "Socket.h"
#include "NetworkStack.h"

/** UDP socket
 */
class UDPSocket : public Socket {
public:
    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    UDPSocket();

    /** Create a socket on a network stack
     *
     *  Creates and opens a socket on the specified network stack.
     *
     *  @param iface    Network stack as target for socket
     */
    UDPSocket(NetworkStack *iface);

    /** Opens a socket
     *
     *  Creates a network socket on the specified network stack.
     *  Not needed if stack is passed to the socket's constructor.
     *
     *  @param iface    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(NetworkStack *iface);

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address specified by either a domain name
     *  or an IP address and port. Returns the number of bytes sent from the
     *  buffer.
     *
     *  By default, sendto blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param host     Hostname of the remote host
     *  @param port     Port of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    int sendto(const char *host, uint16_t port, const void *data, unsigned size);

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address. Returns the number of bytes
     *  sent from the buffer.
     *
     *  By default, sendto blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param address  The SocketAddress of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    int sendto(const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet over a UDP socket
     *
     *  Receives data and stores the source address in address if address
     *  is not NULL. Returns the number of bytes received into the buffer.
     *
     *  By default, recvfrom blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param address  Destination for the source address or NULL
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    int recvfrom(SocketAddress *address, void *data, unsigned size);
};

#endif
