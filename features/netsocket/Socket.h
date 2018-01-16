
/** \addtogroup netsocket */
/** @{*/
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

#include "netsocket/SocketAddress.h"
#include "netsocket/NetworkStack.h"
#include "rtos/Mutex.h"
#include "Callback.h"
#include "mbed_toolchain.h"


/** Abstract socket class
 */
class Socket {
public:
    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~Socket() {}

    /** Opens a socket
     *
     *  Creates a network socket on the network stack of the given
     *  network interface. Not needed if stack is passed to the
     *  socket's constructor.
     *
     *  @param stack    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t open(NetworkStack *stack);

    template <typename S>
    nsapi_error_t open(S *stack) {
        return open(nsapi_create_stack(stack));
    }
    
    /** Close the socket
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket. Called from destructor if socket is not closed.
     *
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t close();
    
    /** Subscribes to an IP multicast group
     *
     * @param address   Multicast group IP address
     * @return          Negative error code on failure
     */
    int join_multicast_group(const SocketAddress &address);

    /** Leave an IP multicast group
     *
     * @param address   Multicast group IP address
     * @return          Negative error code on failure
     */
    int leave_multicast_group(const SocketAddress &address);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to receive
     *  data.
     *
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t bind(uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to receive
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Null-terminated local address to bind
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t bind(const char *address, uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to receive
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Local address to bind
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t bind(const SocketAddress &address);
    
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
     *  timeout. A timeout of 0 removes the timeout from the socket. A negative
     *  value give the socket an unbounded timeout.
     *
     *  set_timeout(0) is equivalent to set_blocking(false)
     *  set_timeout(-1) is equivalent to set_blocking(true)
     *
     *  @param timeout  Timeout in milliseconds
     */
    void set_timeout(int timeout);

    /*  Set socket options
     *
     *  setsockopt allows an application to pass stack-specific options
     *  to the underlying stack using stack-specific level and option names,
     *  or to request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t
     *  @param optname  Level-specific option name
     *  @param optval   Option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);

    /*  Get socket options
     *
     *  getsockopt allows an application to retrieve stack-specific options
     *  from the underlying stack using stack-specific level and option names,
     *  or to request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t
     *  @param optname  Level-specific option name
     *  @param optval   Destination for option value
     *  @param optlen   Length of the option value
     *  @return         0 on success, negative error code on failure
     */    
    nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  Note! This is not intended as a replacement for a poll or attach-like
     *  asynchronous api, but rather as a building block for constructing
     *  such functionality. The exact timing of when the registered function
     *  is called is not guaranteed and susceptible to change.
     *
     *  @param func     Function to call on state change
     */
    void sigio(mbed::Callback<void()> func);

    /** Register a callback on state change of the socket
     *
     *  @see Socket::sigio
     *  @deprecated
     *      The behaviour of Socket::attach differs from other attach functions in
     *      mbed OS and has been known to cause confusion. Replaced by Socket::sigio.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "The behaviour of Socket::attach differs from other attach functions in "
        "mbed OS and has been known to cause confusion. Replaced by Socket::sigio.")
    void attach(mbed::Callback<void()> func);

    /** Register a callback on state change of the socket
     *
     *  @see Socket::sigio
     *  @deprecated
     *      The attach function does not support cv-qualifiers. Replaced by
     *      attach(callback(obj, method)).
     */
    template <typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "The attach function does not support cv-qualifiers. Replaced by "
        "attach(callback(obj, method)).")
    void attach(T *obj, M method) {
        attach(mbed::callback(obj, method));
    }

protected:
    Socket();
    virtual nsapi_protocol_t get_proto() = 0;
    virtual void event() = 0;
    int modify_multicast_group(const SocketAddress &address, nsapi_socket_option_t socketopt);

    NetworkStack *_stack;
    nsapi_socket_t _socket;
    uint32_t _timeout;
    mbed::Callback<void()> _event;
    mbed::Callback<void()> _callback;
    rtos::Mutex _lock;
};


#endif

/** @}*/
