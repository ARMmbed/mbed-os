/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef QUECTEL_BC95_CELLULARSTACK_H_
#define QUECTEL_BC95_CELLULARSTACK_H_

#include "AT_CellularStack.h"

#define BC95_SOCKET_MAX 7
#define BC95_MAX_PACKET_SIZE 512

namespace mbed {

class QUECTEL_BC95_CellularStack : public AT_CellularStack
{
public:
    QUECTEL_BC95_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type);
    virtual ~QUECTEL_BC95_CellularStack();

public: // NetworkStack

    /** Listen for connections on a TCP socket
     *
     *  Marks the socket as a passive socket that can be used to accept
     *  incoming connections.
     *
     *  @param handle   Socket handle
     *  @param backlog  Number of pending connections that can be queued
     *                  simultaneously
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    /** Accepts a connection on a TCP socket
     *
     *  The server socket must be bound and set to listen for connections.
     *  On a new connection, creates a network socket and stores it in the
     *  specified handle. The handle must be passed to following calls on
     *  the socket.
     *
     *  A stack may have a finite number of sockets, in this case
     *  NSAPI_ERROR_NO_SOCKET is returned if no socket is available.
     *
     *  This call is non-blocking. If accept would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param server   Socket handle to server to accept from
     *  @param handle   Destination for a handle to the newly created socket
     *  @param address  Destination for the remote address or NULL
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address=0);

protected:

    /**
    * Gets maximum number of sockets modem supports
    */
    virtual int get_max_socket_count();

    /**
    * Gets maximum packet size
    */
    virtual int get_max_packet_size();

    /**
    * Checks if modem supports given protocol
    *
    * @param protocol   Protocol type
    */
    virtual bool is_protocol_supported(nsapi_protocol_t protocol);

    /**
    * Implements modem specific AT command set for socket closing
    *
    * @param sock_id   Socket id
    */
    virtual nsapi_error_t socket_close_impl(int sock_id);

    /**
    * Implements modem specific AT command set for creating socket
    *
    * @param socket   Cellular socket handle
    */
    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    /**
    * Implements modem specific AT command set for sending data
    *
    * @param socket   Cellular socket handle
    * @param address  The SocketAddress of the remote host
    * @param data     Buffer of data to send to the host
    * @param size     Size of the buffer in bytes
    * @return         Number of sent bytes on success, negative error
    *                 code on failure
    */
    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
            const void *data, nsapi_size_t size);

    /**
     *  Implements modem specific AT command set for receiving data
     *
     *  @param handle   Socket handle
     *  @param address  Destination for the source address or NULL
     *  @param buffer   Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
            void *buffer, nsapi_size_t size);

private:
    // URC handlers
    void urc_nsonmi();
};
} // namespace mbed
#endif /* QUECTEL_BC95_CELLULARSTACK_H_ */
