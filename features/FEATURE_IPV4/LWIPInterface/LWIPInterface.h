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
 *  Implementation of the NetworkStack for LWIP
 */
class LWIPInterface : public NetworkStack, public EthernetInterface
{
public:
    /** Start the interface
     *  @return             0 on success, negative on failure
     */
    virtual int connect();

    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    virtual int disconnect();

    /** Get the internally stored IP address
     *  @return             IP address of the interface or null if not yet connected
     */
    virtual const char *get_ip_address();

    /** Get the internally stored MAC address
     *  @return             MAC address of the interface
     */
    virtual const char *get_mac_address();

protected:
    /** Open a socket
     *  @param handle       Handle in which to store new socket
     *  @param proto        Type of socket to open, NSAPI_TCP or NSAPI_UDP
     *  @return             0 on success, negative on failure
     */
    virtual int socket_open(void **handle, nsapi_protocol_t proto);

    /** Close the socket
     *  @param handle       Socket handle
     *  @return             0 on success, negative on failure
     *  @note On failure, any memory associated with the socket must still 
     *        be cleaned up
     */
    virtual int socket_close(void *handle);

    /** Bind a server socket to a specific port
     *  @param handle       Socket handle
     *  @param address      Local address to listen for incoming connections on 
     *  @return             0 on success, negative on failure.
     */
    virtual int socket_bind(void *handle, const SocketAddress &address);

    /** Start listening for incoming connections
     *  @param handle       Socket handle
     *  @param backlog      Number of pending connections that can be queued up at any
     *                      one time [Default: 1]
     *  @return             0 on success, negative on failure
     */
    virtual int socket_listen(void *handle, int backlog);

    /** Connects this TCP socket to the server
     *  @param handle       Socket handle
     *  @param address      SocketAddress to connect to
     *  @return             0 on success, negative on failure
     */
    virtual int socket_connect(void *handle, const SocketAddress &address);

    /** Accept a new connection.
     *  @param handle       Handle in which to store new socket
     *  @param server       Socket handle to server to accept from
     *  @return             0 on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_accept(void **handle, void *server);

    /** Send data to the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer to send to the host
     *  @param size         The length of the buffer to send
     *  @return             Number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_send(void *handle, const void *data, unsigned size);

    /** Receive data from the remote host
     *  @param handle       Socket handle
     *  @param data         The buffer in which to store the data received from the host
     *  @param size         The maximum length of the buffer
     *  @return             Number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recv(void *handle, void *data, unsigned size);

    /** Send a packet to a remote endpoint
     *  @param handle       Socket handle
     *  @param address      The remote SocketAddress
     *  @param data         The packet to be sent
     *  @param size         The length of the packet to be sent
     *  @return the         number of written bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_sendto(void *handle, const SocketAddress &address, const void *data, unsigned size);

    /** Receive a packet from a remote endpoint
     *  @param handle       Socket handle
     *  @param address      Destination for the remote SocketAddress or null
     *  @param buffer       The buffer for storing the incoming packet data
     *                      If a packet is too long to fit in the supplied buffer,
     *                      excess bytes are discarded
     *  @param size         The length of the buffer
     *  @return the         number of received bytes on success, negative on failure
     *  @note This call is not-blocking, if this call would block, must
     *        immediately return NSAPI_ERROR_WOULD_WAIT
     */
    virtual int socket_recvfrom(void *handle, SocketAddress *address, void *buffer, unsigned size);

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

    /** Register a callback on state change of the socket
     *  @param handle       Socket handle
     *  @param callback     Function to call on state change
     *  @param data         Argument to pass to callback
     *  @note Callback may be called in an interrupt context.
     */
    virtual void socket_attach(void *handle, void (*callback)(void *), void *data);
};

#endif
