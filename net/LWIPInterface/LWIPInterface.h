/* LWIP implementation of NetworkInterfaceAPI
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

#ifndef LWIP_INTERFACE_H
#define LWIP_INTERFACE_H

#include "EthernetInterface.h"
#include "rtos.h"
#include "lwip/netif.h"


/** LWIPInterface class
 *  Implementation of the NetworkInterface for LWIP
 */
class LWIPInterface : public EthernetInterface
{
public:
    /** Start the interface
     *  @return     0 on success, negative on failure
     */
    virtual int connect();

    /** Stop the interface
     *  @return     0 on success, negative on failure
     */
    virtual int disconnect();

    /** Get the internally stored IP address
    /return     IP address of the interface or null if not yet connected
    */
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
    /return     MAC address of the interface
    */
    virtual const char *get_mac_address();

protected:
    /** Create a socket
    /param proto    The type of socket to open, TCP or UDP
    /return         The alocated socket or null on failure
    */
    virtual void *socket_create(nsapi_protocol_t proto);

    /** Destroy a socket
    /param socket   Previously allocated socket
    */
    virtual void socket_destroy(void *handle);

    /** Set socket options
    \param handle   Socket handle
    \param optname  Option ID
    \param optval   Option value
    \param optlen   Length of the option value
    \return         0 on success, negative on failure
    */    
    virtual int socket_set_option(void *handle, int optname, const void *optval, unsigned int optlen);

    /** Get socket options
    \param handle   Socket handle
    \param optname  Option ID
    \param optval   Buffer pointer where to write the option value
    \param optlen   Length of the option value
    \return         0 on success, negative on failure
    */
    virtual int socket_get_option(void *handle, int optname, void *optval, unsigned int *optlen);

    /** Bind a server socket to a specific port
    \param handle   Socket handle
    \param port     The port to listen for incoming connections on
    \return         0 on success, negative on failure.
    */
    virtual int socket_bind(void *handle, int port);

    /** Start listening for incoming connections
    \param handle   Socket handle
    \param backlog  Number of pending connections that can be queued up at any
                    one time [Default: 1]
    \return         0 on success, negative on failure
    */
    virtual int socket_listen(void *handle, int backlog);

    /** Connects this TCP socket to the server
    \param handle   Socket handle
    \param address  SocketAddress to connect to
    \return         0 on success, negative on failure
    */
    virtual int socket_connect(void *handle, const SocketAddress &address);
    
    /** Check if the socket is connected
    \param handle   Socket handle
    \return         true if connected, false otherwise
    */
    virtual bool socket_is_connected(void *handle);

    /** Accept a new connection.
    \param handle   Socket handle
    \param socket   A TCPSocket instance that will handle the incoming connection.
    \return         0 on success, negative on failure.
    \note This call is not-blocking, if this call would block, must
          immediately return NSAPI_ERROR_WOULD_WAIT
    */
    virtual int socket_accept(void *handle, void **connection);

    /** Send data to the remote host
    \param handle   Socket handle
    \param data     The buffer to send to the host
    \param size     The length of the buffer to send
    \return         Number of written bytes on success, negative on failure
    \note This call is not-blocking, if this call would block, must
          immediately return NSAPI_ERROR_WOULD_WAIT
    */
    virtual int socket_send(void *handle, const void *data, unsigned size);

    /** Receive data from the remote host
    \param handle   Socket handle
    \param data     The buffer in which to store the data received from the host
    \param size     The maximum length of the buffer
    \return         Number of received bytes on success, negative on failure
    \note This call is not-blocking, if this call would block, must
          immediately return NSAPI_ERROR_WOULD_WAIT
    */
    virtual int socket_recv(void *handle, void *data, unsigned size);

    /** Send a packet to a remote endpoint
    \param handle   Socket handle
    \param address  The remote SocketAddress
    \param data     The packet to be sent
    \param size     The length of the packet to be sent
    \return the number of written bytes on success, negative on failure
    \note This call is not-blocking, if this call would block, must
          immediately return NSAPI_ERROR_WOULD_WAIT
    */
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet from a remote endpoint
    \param handle   Socket handle
    \param address  Destination for the remote SocketAddress or null
    \param buffer   The buffer for storing the incoming packet data
                    If a packet is too long to fit in the supplied buffer,
                    excess bytes are discarded
    \param size     The length of the buffer
    \return the number of received bytes on success, negative on failure
    \note This call is not-blocking, if this call would block, must
          immediately return NSAPI_ERROR_WOULD_WAIT
    */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);

    /** Close the socket
    \param handle   Socket handle
    \param shutdown  free the left-over data in message queues
    */
    virtual int socket_close(void *handle, bool shutdown);

    /** Register a callback on when a new connection is ready
    \param handle   Socket handle
    \param callback Function to call when accept will succeed, may be called in
                    interrupt context.
    \param id       Argument to pass to callback
    */
    virtual void socket_attach_accept(void *handle, void (*callback)(void *), void *id);

    /** Register a callback on when send is ready
    \param handle   Socket handle
    \param callback Function to call when accept will succeed, may be called in
                    interrupt context.
    \param id       Argument to pass to callback
    */
    virtual void socket_attach_send(void *handle, void (*callback)(void *), void *id);

    /** Register a callback on when recv is ready
    \param handle   Socket handle
    \param callback Function to call when accept will succeed, may be called in
                    interrupt context.
    \param id       Argument to pass to callback
    */
    virtual void socket_attach_recv(void *handle, void (*callback)(void *), void *id);
};

#endif
