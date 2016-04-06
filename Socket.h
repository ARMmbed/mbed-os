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

#include "SocketAddress.h"
#include "NetworkInterface.h"

/** Abstract socket class
 */
class Socket {
public:
    /** Socket lifetime
     */
    virtual ~Socket();
    
    /** Set blocking or non-blocking mode of the socket
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    void set_blocking(bool blocking);
    
    /** Set timeout on a socket operation if blocking behaviour is enabled
     *  @param timeout  timeout in ms
     */
    void set_timeout(unsigned int timeout);

    /*  Set socket options
     *  @param optname  Option ID
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */
    int set_option(int optname, const void *optval, unsigned optlen);
    
    /*  Get socket options
     *  @param optname  Option ID
     *  @param optval   Buffer pointer where to write the option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */
    int get_option(int optname, void *optval, unsigned *optlen);
    
    /** Close the socket
     *  @param shutdown free the left-over data in message queues
     */
    int close(bool shutdown=true);

protected:
    Socket(NetworkInterface *iface, nsapi_protocol_t proto);

    static void thunk(void *);

    NetworkInterface *_iface;
    void *_socket;
    bool _blocking;
    unsigned _timeout;
};

#endif
