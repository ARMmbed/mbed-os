
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

#ifndef INTERNETSOCKET_H
#define INTERNETSOCKET_H

#include "netsocket/Socket.h"
#include "netsocket/NetworkStack.h"
#include "rtos/Mutex.h"
#include "rtos/EventFlags.h"
#include "Callback.h"
#include "mbed_toolchain.h"

/** Socket implementation that uses IP network stack.
 * Not to be directly used by applications. Cannot be directly instantiated.
 */
class InternetSocket : public Socket {
public:
    /** Destroy the socket.
     *
     *  @note Closes socket if it's still open.
     */
    virtual ~InternetSocket();

    /** Open a network socket on the network stack of the given
     *  network interface.
     *
     *  @note Not needed if stack is passed to the socket's constructor.
     *
     *  @param stack    Network stack as target for socket.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    nsapi_error_t open(NetworkStack *stack);

#if !defined(DOXYGEN_ONLY)
    template <typename S>
    nsapi_error_t open(S *stack)
    {
        return open(nsapi_create_stack(stack));
    }
#endif //!defined(DOXYGEN_ONLY)

    /** Close any open connection, and deallocate any memory associated
     *  with the socket. Called from destructor if socket is not closed.
     *
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    virtual nsapi_error_t close();

    /** Subscribe to an IP multicast group.
     *
     * @param address   Multicast group IP address.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    int join_multicast_group(const SocketAddress &address);

    /** Leave an IP multicast group.
     *
     * @param address   Multicast group IP address.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    int leave_multicast_group(const SocketAddress &address);

    /** Bind the socket to a port on which to receive data.
     *
     *  @param port     Local port to bind.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    nsapi_error_t bind(uint16_t port);

    /** Bind the socket to a specific address and port on which to receive
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Null-terminated local address to bind.
     *  @param port     Local port to bind.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    nsapi_error_t bind(const char *address, uint16_t port);

    /** Bind the socket to a specific address and port on which to receive
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Local SocketAddress to bind, which includes the address and port.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    virtual nsapi_error_t bind(const SocketAddress &address);

    /** Set blocking or nonblocking mode of the socket.
     *
     *  Initially all sockets are in blocking mode. In nonblocking mode,
     *  blocking operations, such as send/recv/accept, return
     *  NSAPI_ERROR_WOULD_BLOCK if they cannot continue.
     *
     *  @note set_blocking(false) is equivalent to set_timeout(-1) and
     *  set_blocking(true) is equivalent to set_timeout(0).
     *
     *  @param blocking Use true for blocking mode, false for nonblocking mode.
     */
    virtual void set_blocking(bool blocking);

    /** Set timeout on blocking socket operations.
     *
     *  Initially all sockets have unbounded timeouts. NSAPI_ERROR_WOULD_BLOCK
     *  is returned if a blocking operation takes longer than the specified
     *  timeout. A timeout of 0 removes the timeout from the socket. A negative
     *  value give the socket an unbounded timeout.
     *
     *  @note set_timeout(0) is equivalent to set_blocking(false) and
     *  set_timeout(-1) is equivalent to set_blocking(true).
     *
     *  @param timeout  Timeout in milliseconds.
     */
    virtual void set_timeout(int timeout);

    /** Pass stack-specific options to the underlying stack using stack-specific
     *  level and option names, or request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned,
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t.
     *  @param optname  Level-specific option name.
     *  @param optval   Option value.
     *  @param optlen   Length of the option value.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    virtual nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);

    /** Retrieve stack-specific options from the underlying stack using
     *  stack-specific level and option names, or request generic options
     *  using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned,
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t.
     *  @param optname  Level-specific option name.
     *  @param optval   Destination for option value.
     *  @param optlen   Length of the option value.
     *  @return         0 on success, negative error code on failure (@see nsapi_types.h).
     */
    virtual nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen);

    /** Register a callback on state change of the socket.
     *
     *  The specified callback is called on state changes, such as when
     *  the socket can recv/send/accept successfully and when an error
     *  occurs. The callback may also be called spuriously without a reason.
     *
     *  The callback may be called in an interrupt context and should not
     *  perform expensive operations, such as recv/send calls.
     *
     *  @note This is not intended as a replacement for a poll or attach-like
     *  asynchronous API, but rather as a building block for constructing
     *  such functionality. The exact timing of when the registered function
     *  is called is not guaranteed and is susceptible to change.
     *
     *  @param func     Function to call on state change.
     */
    virtual void sigio(mbed::Callback<void()> func);

    /** Register a callback on state change of the socket.
     *
     *  @see Socket::sigio
     *  @deprecated
     *      The behavior of Socket::attach differs from other attach functions in
     *      Mbed OS and has been known to cause confusion. Replaced by Socket::sigio.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
                          "The behavior of Socket::attach differs from other attach functions in "
                          "Mbed OS and has been known to cause confusion. Replaced by Socket::sigio.")
    void attach(mbed::Callback<void()> func);

    /** Register a callback on state change of the socket.
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
    void attach(T *obj, M method)
    {
        attach(mbed::callback(obj, method));
    }

#if !defined(DOXYGEN_ONLY)

protected:
    InternetSocket();
    virtual nsapi_protocol_t get_proto() = 0;
    virtual void event();
    int modify_multicast_group(const SocketAddress &address, nsapi_socket_option_t socketopt);

    NetworkStack *_stack;
    nsapi_socket_t _socket;
    uint32_t _timeout;
    mbed::Callback<void()> _event;
    mbed::Callback<void()> _callback;
    rtos::EventFlags _event_flag;
    rtos::Mutex _lock;
    SocketAddress _remote_peer;
    uint8_t _readers;
    uint8_t _writers;
    volatile unsigned _pending;
    bool _factory_allocated;

    // Event flags
    static const int READ_FLAG     = 0x1u;
    static const int WRITE_FLAG    = 0x2u;
    static const int FINISHED_FLAG = 0x3u;

#endif //!defined(DOXYGEN_ONLY)
};

#endif // INTERNETSOCKET_H

/** @}*/
