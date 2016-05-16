/* NetworkStack
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

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "mbed.h"
#include "SocketAddress.h"


/** Enum of standardized error codes 
 *
 *  Valid error codes have negative values and may
 *  be returned by any network operation.
 *
 *  @enum nsapi_error_t
 */
enum nsapi_error_t {
    NSAPI_ERROR_WOULD_BLOCK   = -3001,     /*!< no data is not available but call is non-blocking */
    NSAPI_ERROR_UNSUPPORTED   = -3002,     /*!< unsupported functionality */
    NSAPI_ERROR_PARAMETER     = -3003,     /*!< invalid configuration */
    NSAPI_ERROR_NO_CONNECTION = -3004,     /*!< not connected to a network */
    NSAPI_ERROR_NO_SOCKET     = -3005,     /*!< socket not available for use */
    NSAPI_ERROR_NO_ADDRESS    = -3006,     /*!< IP address is not known */
    NSAPI_ERROR_NO_MEMORY     = -3007,     /*!< memory resource not available */
    NSAPI_ERROR_DNS_FAILURE   = -3008,     /*!< DNS failed to complete successfully */
    NSAPI_ERROR_DHCP_FAILURE  = -3009,     /*!< DHCP failed to complete successfully */
    NSAPI_ERROR_AUTH_FAILURE  = -3010,     /*!< connection to access point faield */
    NSAPI_ERROR_DEVICE_ERROR  = -3011,     /*!< failure interfacing with the network procesor */
};

/** Enum of socket protocols
 *
 *  The socket protocol specifies a particular protocol to
 *  be used with a newly created socket. 
 *
 *  @enum nsapi_protocol_t
 */
enum nsapi_protocol_t {
   NSAPI_TCP, /*!< Socket is of TCP type */
   NSAPI_UDP, /*!< Socket is of UDP type */
};

/*  Enum of standardized stack option levels
 *
 *  @enum nsapi_level_t
 */
enum nsapi_level_t {
    NSAPI_STACK,  /*!< Stack option level */
    NSAPI_SOCKET, /*!< Socket option level */
};

/*  Enum of standardized stack options
 *
 *  These options may not be supported on all stacks, in which
 *  case NSAPI_ERROR_UNSUPPORTED may be returned from setsockopt.
 *
 *  @enum nsapi_option_t
 */
enum nsapi_option_t {
    NSAPI_REUSEADDR, /*!< Allow bind to reuse local addresses */
    NSAPI_KEEPALIVE, /*!< Enables sending of keepalive messages */
    NSAPI_LINGER,    /*!< Keeps close from returning until queues empty */
    NSAPI_SNDBUF,    /*!< Sets send buffer size */
    NSAPI_RCVBUF,    /*!< Sets recv buffer size */
};


/** NetworkStack class
 *
 *  Common interface that is shared between hardware that
 *  can connect to a network over IP. By implementing the
 *  NetworkStack, a network stack can be used as a target
 *  for instantiating network sockets.
 */
class NetworkStack
{
public:
    virtual ~NetworkStack() {};

    /** Get the local IP address
     *
     *  @return         Null-terminated representation of the local IP address
     *                  or null if not yet connected
     */
    virtual const char *get_ip_address() = 0;

    /** Translates a hostname to an IP address
     *
     *  The hostname may be either a domain name or an IP address. If the
     *  hostname is an IP address, no network transactions will be performed.
     *  
     *  If no stack-specific DNS resolution is provided, the hostname
     *  will be resolve using a UDP socket on the stack. 
     *
     *  @param address  Destination for the host SocketAddress
     *  @param host     Hostname to resolve
     *  @return         0 on success, negative error code on failure
     */
    virtual int gethostbyname(SocketAddress *address, const char *host);

    /*  Set stack-specific stack options
     *
     *  The setstackopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the stack is unmodified.
     *
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    virtual int setstackopt(int level, int optname, const void *optval, unsigned optlen);

    /*  Get stack-specific stack options
     *
     *  The getstackopt allow an application to retrieve stack-specific hints
     *  from the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and optval is unmodified.
     *
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    virtual int getstackopt(int level, int optname, void *optval, unsigned *optlen);

protected:
    friend class Socket;
    friend class UDPSocket;
    friend class TCPSocket;
    friend class TCPServer;

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
    virtual int socket_open(void **handle, nsapi_protocol_t proto) = 0;

    /** Close the socket
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket.
     *
     *  @param handle   Socket handle
     *  @return         0 on success, negative error code on failure
     */
    virtual int socket_close(void *handle) = 0;

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param handle   Socket handle
     *  @param address  Local address to bind
     *  @return         0 on success, negative error code on failure.
     */
    virtual int socket_bind(void *handle, const SocketAddress &address) = 0;

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
    virtual int socket_listen(void *handle, int backlog) = 0;

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by the
     *  indicated address.
     *
     *  @param handle   Socket handle
     *  @param address  The SocketAddress of the remote host
     *  @return         0 on success, negative error code on failure
     */
    virtual int socket_connect(void *handle, const SocketAddress &address) = 0;

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
     *  @param handle   Destination for a handle to the newly created sockey
     *  @param server   Socket handle to server to accept from
     *  @return         0 on success, negative error code on failure
     */
    virtual int socket_accept(void **handle, void *server) = 0;

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
    virtual int socket_send(void *handle, const void *data, unsigned size) = 0;

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
    virtual int socket_recv(void *handle, void *data, unsigned size) = 0;

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
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size) = 0;

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
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size) = 0;

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
    virtual void socket_attach(void *handle, void (*callback)(void *), void *data) = 0;

    /*  Set stack-specific socket options
     *
     *  The setsockopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the socket is unmodified.
     *
     *  @param handle   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    virtual int setsockopt(void *handle, int level, int optname, const void *optval, unsigned optlen);

    /*  Get stack-specific socket options
     *
     *  The getstackopt allow an application to retrieve stack-specific hints
     *  from the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and optval is unmodified.
     *
     *  @param handle   Socket handle
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    virtual int getsockopt(void *handle, int level, int optname, void *optval, unsigned *optlen);
};

#endif
