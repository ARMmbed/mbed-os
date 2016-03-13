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

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "mbed.h"
#include "SocketAddress.h"

/** Enum of standardized error codes
 *  @enum ns_error_t
 */
enum nsapi_error_t {
    NSAPI_ERROR_WOULD_BLOCK   = -3001,     /*!< no data is not available but call is non-blocking */
    NSAPI_ERROR_UNSUPPORTED   = -3002,     /*!< unsupported configuration */
    NSAPI_ERROR_NO_CONNECTION = -3003,     /*!< not connected to a network */
    NSAPI_ERROR_NO_SOCKET     = -3004,     /*!< socket not available for use */
    NSAPI_ERROR_NO_ADDRESS    = -3005,     /*!< IP address is not known */
    NSAPI_ERROR_NO_MEMORY     = -3006,     /*!< memory resource not available */
    NSAPI_ERROR_DNS_FAILURE   = -3007,     /*!< DNS failed to complete successfully */
    NSAPI_ERROR_DHCP_FAILURE  = -3008,     /*!< DHCP failed to complete successfully */
    NSAPI_ERROR_AUTH_FAILURE  = -3009,     /*!< connection to access point faield */
    NSAPI_ERROR_DEVICE_ERROR  = -3010,     /*!< failure interfacing with the network procesor */
};
   
/** Enum of available options
 *  @enum ns_opt_t
 */
enum ns_opt_t {
};

/** Enum of socket protocols
 *  @enum protocol_t
 */
enum nsapi_protocol_t {
   NSAPI_TCP, /*!< Socket is of TCP type */
   NSAPI_UDP, /*!< Socket is of UDP type */
};

/** NetworkInterface class
 *  Common interface that is shared between all hardware that
 *  can connect to a network over IP.
 */
class NetworkInterface
{
public:
    virtual ~NetworkInterface() {};

    /** Get the internally stored IP address
     *  @return         IP address of the interface or null if not yet connected
     */
    virtual const char *get_ip_address() = 0;

    /** Get the internally stored MAC address
     *  @return         MAC address of the interface
     */
    virtual const char *get_mac_address() = 0;

    /** Get the current status of the interface
     *  @return         true if connected
     */
    virtual bool is_connected() {
        return get_ip_address() != NULL;
    }

    /** Looks up the specified host's IP address
     *  @param name     Hostname to lookup
     *  @param dest     Destination for IP address, must have space for SocketAddress::IP_SIZE
     *  @return         0 on success, negative on failure
     */
    virtual int gethostbyname(const char *name, char *dest);

protected:
    friend class Socket;
    friend class UDPSocket;
    friend class TCPSocket;
    friend class TCPServer;

    /** Create a socket
     *  @param proto    The type of socket to open, TCP or UDP
     *  @return         The alocated socket or null on failure
     */
    virtual void *socket_create(nsapi_protocol_t proto) = 0;

    /** Destroy a socket
     *  @param socket     Previously allocated socket
     */
    virtual void socket_destroy(void *handle) = 0;

    /** Set socket options
     *  @param handle   Socket handle
     *  @param optname  Option ID
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */    
    virtual int socket_set_option(void *handle, int optname, const void *optval, unsigned int optlen) = 0;

    /** Get socket options
     *  @param handle   Socket handle
     *  @param optname  Option ID
     *  @param optval   Buffer pointer where to write the option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */
    virtual int socket_get_option(void *handle, int optname, void *optval, unsigned int *optlen) = 0;

    /** Bind a server socket to a specific port
     *  @param handle   Socket handle
     *  @param port     The port to listen for incoming connections on
     *  @return         0 on success, negative on failure.
     */
    virtual int socket_bind(void *handle, int port) = 0;

    /** Start listening for incoming connections
     *  @param handle   Socket handle
     *  @param backlog  Number of pending connections that can be queued up at any
     *                  one time [Default: 1]
     *  @return         0 on success, negative on failure
     */
    virtual int socket_listen(void *handle, int backlog) = 0;

    /** Connects this TCP socket to the server
     *  @param handle   Socket handle
     *  @param address  SocketAddress to connect to
     *  @return         0 on success, negative on failure
     */
    virtual int socket_connect(void *handle, const SocketAddress &address) = 0;
    
    /** Check if the socket is connected
     *  @param handle   Socket handle
     *  @return         true if connected, false otherwise
     */
    virtual bool socket_is_connected(void *handle) = 0;

    /** Accept a new connection.
     *  @param handle   Socket handle
     *  @param socket   A TCPSocket instance that will handle the incoming connection.
     *  @return         0 on success, negative on failure.
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_accept(void *handle, void **connection) = 0;

    /** Send data to the remote host
     *  @param handle   Socket handle
     *  @param data     The buffer to send to the host
     *  @param size     The length of the buffer to send
     *  @return         Number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_send(void *handle, const void *data, unsigned size) = 0;

    /** Receive data from the remote host
     *  @param handle   Socket handle
     *  @param data     The buffer in which to store the data received from the host
     *  @param size     The maximum length of the buffer
     *  @return         Number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recv(void *handle, void *data, unsigned size) = 0;

    /** Send a packet to a remote endpoint
     *  @param handle   Socket handle
     *  @param address  The remote SocketAddress
     *  @param data     The packet to be sent
     *  @param size     The length of the packet to be sent
     *  @return         the number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size) = 0;

    /** Receive a packet from a remote endpoint
     *  @param handle   Socket handle
     *  @param address  Destination for the remote SocketAddress or null
     *  @param buffer   The buffer for storing the incoming packet data
     *                  If a packet is too long to fit in the supplied buffer,
     *                  excess bytes are discarded
     *  @param size     The length of the buffer
     *  @return         the number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size) = 0;

    /** Close the socket
     *  @param handle   Socket handle
     */
    virtual int socket_close(void *handle) = 0;

    /** Register a callback on when a new connection is ready
     *  @param handle   Socket handle
     *  @param callback Function to call when accept will succeed, may be called in
     *                  interrupt context.
     *  @param id       Argument to pass to callback
     */
    virtual void socket_attach_accept(void *handle, void (*callback)(void *), void *id) = 0;

    /** Register a callback on when send is ready
     *  @param handle   Socket handle
     *  @param callback Function to call when accept will succeed, may be called in
     *                  interrupt context.
     *  @param id       Argument to pass to callback
     */
    virtual void socket_attach_send(void *handle, void (*callback)(void *), void *id) = 0;

    /** Register a callback on when recv is ready
     *  @param handle   Socket handle
     *  @param callback Function to call when accept will succeed, may be called in
     *                  interrupt context.
     *  @param id       Argument to pass to callback
     */
    virtual void socket_attach_recv(void *handle, void (*callback)(void *), void *id) = 0;
};

#endif
