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

#ifndef SOCKET_H
#define SOCKET_H

#include "NetworkInterface.h"

/** Abstract socket class
 *  API for handling general sockets. Supports IP address operations
 *  and sending/recieving data.
 */
class Socket
{
public:
    ~Socket();

    /** Open a connection to the underlying address
     *  @param address URL or IP address to connect to
     *  @param port Port to connect to
     *  @return 0 on success
     */
    int32_t open(const char *address, uint16_t port);

    /** Close an open connection
     *  @return 0 on success
     */
    int32_t close();

    /** Send data over the socket
     *  @param data Buffer of data to send
     *  @param size Size of data to send
     *  @return Number of bytes sent or a negative value on failure
     */
    int32_t send(const void *data, uint32_t size);

    /** Recieve data over the socket
     *  @param data Buffer to store recieved data
     *  @param size Size of provided buffer
     *  @param blocking If true wait for data, otherwise return NS_ERROR_WOULD_BLOCK
     *  @return Number of bytes recieved or a negative value on failure
     */
    int32_t recv(void *data, uint32_t size, bool blocking = true);

    /** Gets the IP address
     *  @return IP address to connect to
     */
    const char *getIPAddress() const;

    /** Gets the port
     *  @return Port to connect to
     */
    uint16_t getPort() const;

    /** Returns status of socket
     *  @return true if connected
     */
    bool isConnected();

protected:
    Socket(NetworkInterface *iface, ns_protocol_t proto);

private:
    NetworkInterface *_iface;
    ns_protocol_t _proto;
    SocketInterface *_socket;

    char _ip_address[NS_IP_SIZE];
    uint16_t _port;
};

#endif
