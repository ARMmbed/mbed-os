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

#ifndef AT_CELLULAR_STACK_H_
#define AT_CELLULAR_STACK_H_

#include "AT_CellularBase.h"
#include "NetworkStack.h"

namespace mbed {

// <PDP_addr_1> and <PDP_addr_2>: each is a string type that identifies the MT in the address space applicable to the PDP.
// The string is given as dot-separated numeric (0-255) parameter of the form:
// a1.a2.a3.a4 for IPv4 and
// a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16 for IPv6.
#define PDP_IPV6_SIZE 63+1

/**
 * Class AT_CellularStack.
 *
 * Implements NetworkStack and introduces interface for modem specific stack implementations.
 */
class AT_CellularStack : public NetworkStack, public AT_CellularBase
{

public:
    AT_CellularStack(ATHandler &at, int cid, nsapi_ip_stack_t stack_type);
    virtual ~AT_CellularStack();

public: // NetworkStack

    /** Get the local IP address
     *
     *  @return Null-terminated representation of the local IP address
     *          or null if not yet connected
     */
    virtual const char *get_ip_address();

    /** Opens a socket
     *
     *  Creates a network socket and stores it in the specified handle.
     *  The handle must be passed to following calls on the socket.
     *
     *  A stack may have a finite number of sockets, in this case
     *  NSAPI_ERROR_NO_SOCKET is returned if no socket is available.
     *
     *  @param handle   Destination for the handle to a newly created socket
     *  @param proto    Protocol of socket to open, NSAPI_TCP or NSAPI_UDP
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t socket_open(nsapi_socket_t *handle, nsapi_protocol_t proto);

    /** Close the socket
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket.
     *
     *  @param handle   Socket handle
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t socket_close(nsapi_socket_t handle);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param handle   Socket handle
     *  @param address  Local address to bind
     *  @return         0 on success, negative error code on failure.
     */
    virtual nsapi_error_t socket_bind(nsapi_socket_t handle, const SocketAddress &address);

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

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by the
     *  indicated address.
     *
     *  @param handle   Socket handle
     *  @param address  The SocketAddress of the remote host
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address);

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

    /** Send data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes sent from the buffer.
     *
     *  This call is non-blocking. If send would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param handle   Socket handle
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_send(nsapi_socket_t handle,
            const void *data, nsapi_size_t size);

    /** Receive data over a TCP socket
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes received into the buffer.
     *
     *  This call is non-blocking. If recv would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param handle   Socket handle
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_recv(nsapi_socket_t handle,
            void *data, nsapi_size_t size);

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address. Returns the number of bytes
     *  sent from the buffer.
     *
     *  This call is non-blocking. If sendto would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param handle   Socket handle
     *  @param address  The SocketAddress of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_sendto(nsapi_socket_t handle, const SocketAddress &address,
            const void *data, nsapi_size_t size);

    /** Receive a packet over a UDP socket
     *
     *  Receives data and stores the source address in address if address
     *  is not NULL. Returns the number of bytes received into the buffer.
     *
     *  This call is non-blocking. If recvfrom would block,
     *  NSAPI_ERROR_WOULD_BLOCK is returned immediately.
     *
     *  @param handle   Socket handle
     *  @param address  Destination for the source address or NULL
     *  @param buffer   Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_recvfrom(nsapi_socket_t handle, SocketAddress *address,
            void *buffer, nsapi_size_t size);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  @param handle   Socket handle
     *  @param callback Function to call on state change
     *  @param data     Argument to pass to callback
     */
    virtual void socket_attach(nsapi_socket_t handle, void (*callback)(void *), void *data);

protected:

    class CellularSocket
    {
    public:
        // Socket id from cellular device
        int id;
        // Being connected means remote ip address and port are set
        bool connected;
        nsapi_protocol_t proto;
        SocketAddress remoteAddress;
        SocketAddress localAddress;
        void (*_cb)(void *);
        void *_data;
        bool created;
        bool rx_avail; // used to synchronise reading from modem
    };

    /**
    * Gets maximum number of sockets modem supports
    */
    virtual int get_max_socket_count() = 0;

    /**
    * Gets maximum packet size
    */
    virtual int get_max_packet_size() = 0;

    /**
    * Checks if modem supports the given protocol
    *
    * @param protocol   Protocol type
    */
    virtual bool is_protocol_supported(nsapi_protocol_t protocol) = 0;

    /**
    * Implements modem specific AT command set for socket closing
    *
    * @param sock_id   Socket id
    */
    virtual nsapi_error_t socket_close_impl(int sock_id) = 0;

    /**
    * Implements modem specific AT command set for creating socket
    *
    * @param socket   Cellular socket handle
    */
    virtual nsapi_error_t create_socket_impl(CellularSocket *socket) = 0;

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
            const void *data, nsapi_size_t size) = 0;

    /**
     *  Implements modem specific AT command set for receiving data
     *
     *  @param socket   Socket handle
     *  @param address  Destination for the source address or NULL
     *  @param buffer   Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
            void *buffer, nsapi_size_t size) = 0;

    // socket container
    CellularSocket **_socket;

    // number of socket slots allocated in socket container
    int _socket_count;

    // IP address
    char _ip[PDP_IPV6_SIZE];

    // PDP context id
    int _cid;

    // stack type from PDP context
    nsapi_ip_stack_t _stack_type;
};

} // namespace mbed

#endif // AT_CELLULAR_STACK_H_
