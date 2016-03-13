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

    /** Open the socket
     *  @param iface    Interface to open socket on
     */
    virtual int open(NetworkInterface *iface) = 0;
    
    /** Close the socket
     */
    int close();
    
    /** Bind a socket to a specific port
     *  @param port     The port to listen for incoming connections on
     *  @return         0 on success, negative on failure.
     */
    int bind(uint16_t port);

    /** Bind a socket to a specific port
     *  @param address  The null-terminated address to listen for incoming connections on
     *  @param port     The port to listen for incoming connections on
     *  @return         0 on success, negative on failure.
     */
    int bind(const char *address, uint16_t port);

    /** Bind a socket to a specific port
     *  @param address  The SocketAddress to listen for incoming connections on
     *  @return         0 on success, negative on failure.
     */
    int bind(const SocketAddress &address);
    
    /** Set blocking or non-blocking mode of the socket
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    void set_blocking(bool blocking);
    
    /** Set timeout on a socket operation if blocking behaviour is enabled
     *  @param timeout  timeout in ms
     */
    void set_timeout(unsigned int timeout);

    /*  Set socket options
     *  @param level    Option level
     *  @param optname  Option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */    
    int setsockopt(int level, int optname, const void *optval, unsigned optlen);

    /*  Get socket options
     *  @param level    Option level
     *  @param optname  Option identifier
     *  @param optval   Buffer where to write option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative on failure
     */    
    int getsockopt(int level, int optname, void *optval, unsigned *optlen);

    /** Register a callback on state change of the socket
     *  @param callback Function to call on state change
     *  @note Callback may be called in an interrupt context.
     *        The callback should not perform long operations 
     *        such as recv or send calls.
     */
    void attach(FunctionPointer callback);

    template <typename T, typename M>
    void attach(T *tptr, M mptr) {
        attach(FunctionPointer(tptr, mptr));
    }

protected:
    Socket();
    int open(NetworkInterface *iface, nsapi_protocol_t proto);

    static void thunk(void *);

    NetworkInterface *_iface;
    void *_socket;
    bool _blocking;
    unsigned _timeout;
    FunctionPointer _callback;
};

#endif
