
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
#include "mbed_atomic.h"
#include "mbed_toolchain.h"
#include "SocketStats.h"

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
     *  @retval         NSAPI_ERROR_OK on success.
     *  @retval         NSAPI_ERROR_PARAMETER in case the provided stack was invalid
     *                  or a stack was already created and socket opened successfully.
     *  @retval         int negative error codes for stack-related failures.
     *                  See @ref NetworkStack::socket_open.
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
     *  @retval         NSAPI_ERROR_OK on success.
     *  @retval         NSAPI_ERROR_NO_SOCKET if socket is not open.
     *  @retval         int negative error codes for stack-related failures.
     *                  See @ref NetworkStack::socket_close.
     */
    virtual nsapi_error_t close();

    /** Subscribe to an IP multicast group.
     *
     * @param address   Multicast group IP address.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure (@see InternetSocket::setsockopt).
     */
    int join_multicast_group(const SocketAddress &address);

    /** Leave an IP multicast group.
     *
     * @param address   Multicast group IP address.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure (@see InternetSocket::setsockopt).
     */
    int leave_multicast_group(const SocketAddress &address);

    /** Bind the socket to a port on which to receive data.
     *
     *  @param port     Local port to bind.
     *  @retval         NSAPI_ERROR_OK on success.
     *  @retval         NSAPI_ERROR_NO_SOCKET if socket is not open.
     *  @retval         int negative error codes for stack-related failures.
     *                  See @ref NetworkStack::socket_bind.
     */
    nsapi_error_t bind(uint16_t port);

    /** @copydoc Socket::bind
     */
    virtual nsapi_error_t bind(const SocketAddress &address);

    /** @copydoc Socket::set_blocking
     */
    virtual void set_blocking(bool blocking);

    /** @copydoc Socket::set_timeout
     */
    virtual void set_timeout(int timeout);

    /** @copydoc Socket::setsockopt
     */
    virtual nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);

    /** @copydoc Socket::getsockopt
     */
    virtual nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen);

    /** @copydoc Socket::sigio
     */
    virtual void sigio(mbed::Callback<void()> func);

    /** @copydoc Socket::getpeername
     */
    virtual nsapi_error_t getpeername(SocketAddress *address);

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
    char _interface_name[NSAPI_INTERFACE_NAME_MAX_SIZE];
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
    core_util_atomic_flag _pending;
    bool _factory_allocated;

    // Event flags
    static const int READ_FLAG     = 0x1u;
    static const int WRITE_FLAG    = 0x2u;
    static const int FINISHED_FLAG = 0x3u;

    friend class DTLSSocket;  // Allow DTLSSocket::connect() to do name resolution on the _stack
    SocketStats _socket_stats;

#endif //!defined(DOXYGEN_ONLY)
};

#endif // INTERNETSOCKET_H

/** @}*/
