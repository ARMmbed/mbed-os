/* SocketInterface Base Class
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

#ifndef SOCKET_INTERFACE_H
#define SOCKET_INTERFACE_H

#include "stdint.h"


/**
 *  @enum ns_protocol_t
 *  @brief enum of socket protocols
 */
enum ns_protocol_t {
    NS_TCP,     /*!< Socket is of TCP type */
    NS_UDP,     /*!< Socket is of UDP type */
};


/** SocketInterface class
 *  Common interface for implementation specific sockets created through
 *  network interfaces. This class is used internally by the
 *  TCPSocket and UDPSocket classes
 */
class SocketInterface
{
public:

    virtual ~SocketInterface() {}

    /** Open a connection to the underlying address
     *  @param ip IP address to connect to
     *  @param port Port to connect to
     *  @return 0 on success
     */
    virtual int32_t open(const char *ip, uint16_t port) = 0;

    /** Close an open connection
     *  @return 0 on success
     */
    virtual int32_t close() = 0;

    /** Send data
     *  @param data Buffer of data to send
     *  @param size Size of data to send
     *  @return Number of bytes received or a negative value on success
     */
    virtual int32_t send(const void *data, uint32_t size) = 0;

    /** Receive data
     *  @note
     *      This call should return immediately with a value of 
     *      NS_ERROR_WOULD_BOCK if no data is available.
     *
     *  @param data A buffer to store the data in
     *  @param size Size of buffer
     *  @return Number of bytes received or a negative value on failure
     */
    virtual int32_t recv(void *data, uint32_t size) = 0;

    /** Status of the socket
     *  @return True if connected
     */
    virtual bool isConnected() {
        // By default return true if socket was created successfully
        return true;
    }
};

#endif
