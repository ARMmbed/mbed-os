
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
#include "toolchain.h"


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
    
    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data.
     *
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t bind(uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Null-terminated local address to bind
     *  @param port     Local port to bind
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t bind(const char *address, uint16_t port);

    /** Bind a specific address to a socket
     *
     *  Binding a socket specifies the address and port on which to recieve
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
    nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);

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
     *  @param func     Function to call on state change
     */
    void attach(mbed::Callback<void()> func);

    /** Register a callback on state change of the socket
     *
     *  The specified callback will be called on state changes such as when
     *  the socket can recv/send/accept successfully and on when an error
     *  occurs. The callback may also be called spuriously without reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations such as recv/send calls.
     *
     *  @param obj      Pointer to object to call method on
     *  @param method   Method to call on state change
     *
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

    NetworkStack *_stack;
    nsapi_socket_t _socket;
    uint32_t _timeout;
    mbed::Callback<void()> _event;
    mbed::Callback<void()> _callback;
    rtos::Mutex _lock;
};


#endif

/** @}*/
