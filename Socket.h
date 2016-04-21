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
#include "NetworkStack.h"

/** Abstract socket class
 */
class Socket {
public:
    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~Socket();

    /** Opens a socket
     *
     *  Creates a network socket on the specified network stack.
     *  Not needed if stack is passed to the socket's constructor.
     *
     *  @param iface    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(NetworkStack *iface) = 0;
    
    /** Close the socket
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket. Called from destructor if socket is not closed.
     *
     *  @return         0 on success, negative error code on failure
     */
    int close();
    
    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data.
     *
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    int bind(uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Null-terminated local address to bind
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    int bind(const char *address, uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Local address to bind
     *  @return         0 on success, negative error code on failure.
     */
    int bind(const SocketAddress &address);
    
    /** Set blocking or non-blocking mode of the socket
     *
     *  Initially all sockets are in blocking mode. In non-blocking mode
     *  blocking operations such as send/recv/accept return
     *  NSAPI_ERROR_WOULD_BLOCK if they can not continue.
     *
     *  set_blocking(false) is equivalent to set_timeout(-1)
     *  set_blocking(true) is equivalent to set_timeout(0)
     *
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    void set_blocking(bool blocking);
    
    /** Set timeout on blocking socket operations
     *
     *  Initially all sockets have unbounded timeouts. NSAPI_ERROR_WOULD_BLOCK
     *  is returned if a blocking operation takes longer than the specified
     *  timeout. A timeout of -1 removes the timeout from the socket.
     *
     *  set_timeout(-1) is equivalent to set_blocking(false)
     *  set_timeout(0) is equivalent to set_blocking(true)
     *
     *  @param timeout  Timeout in milliseconds
     */
    void set_timeout(int timeout);

    /*  Set stack-specific socket options
     *
     *  The setsockopt allow an application to pass stack-specific hints
     *  to the underlying stack. For unsupported options,
     *  NSAPI_ERROR_UNSUPPORTED is returned and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level
     *  @param optname  Stack-specific option identifier
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    int setsockopt(int level, int optname, const void *optval, unsigned optlen);

    /*  Get stack-specific socket options
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
    int getsockopt(int level, int optname, void *optval, unsigned *optlen);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  @param callback Function to call on state change
     */
    void attach(FunctionPointer callback);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  @param tptr     Pointer to object to call method on
     *  @param mptr     Method to call on state change
     */
    template <typename T, typename M>
    void attach(T *tptr, M mptr) {
        attach(FunctionPointer(tptr, mptr));
    }

protected:
    Socket();
    int open(NetworkStack *iface, nsapi_protocol_t proto);

    static void thunk(void *);
    static void wakeup();

    NetworkStack *_iface;
    void *_socket;
    int _timeout;
    FunctionPointer _callback;
};

#endif
