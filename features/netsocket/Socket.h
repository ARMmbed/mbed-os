
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
#include "Callback.h"

/** Abstract socket class
 */
class Socket {
public:
    /** Destroy a socket.
     *
     *  Closes socket if the socket is still open
     */
    virtual ~Socket() {}

    /** Close the socket.
     *
     *  Closes any open connection and deallocates any memory associated
     *  with the socket. Called from destructor if socket is not closed.
     *
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t close() = 0;

    /** Connects socket to a remote address.
     *
     *  Attempt to make connection on connection-mode protocol or set or reset
     *  the peer address on connectionless protocol.
     *
     *  Also connectionless protocols use the connected address to filter
     *  incoming packets for recv() and recvfrom() calls.
     *
     *  To reset the peer address, zero initialised(default constructor) SocketAddress
     *  object have to be in the address parameter.
     *
     *  @param address  The SocketAddress of the remote peer
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t connect(const SocketAddress &address) = 0;

    /** Send data on a socket
     *
     *  The socket must be connected to a remote host before send() call.
     *  Returns the number of bytes sent from the buffer.
     *  In case of connectionless socket, send data to pre-specified remote.
     *
     *  By default, send blocks until all data is sent. If socket is set to
     *  non-blocking or times out, a partial amount can be written.
     *  NSAPI_ERROR_WOULD_BLOCK is returned if no data was written.
     *
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure.
     */
    virtual nsapi_size_or_error_t send(const void *data, nsapi_size_t size) = 0;

    /** Receive data from a socket.
     *
     *  Receive data from connected socket or in case of connectionless socket
     *  this is equivalent of calling recvfrom(NULL, data, size).
     *
     *  If socket is connected, only packets coming from connected peer address
     *  are accepted.
     *
     *  @note recv() is allowed write to data buffer even if error occurs.
     *
     *  By default, recv blocks until some data is received. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK can be returned to
     *  indicate no data.
     *
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure. If no data is available to be received
     *                  and the peer has performed an orderly shutdown,
     *                  recv() returns 0.
     */
    virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size) = 0;

    /** Send a message on a socket.
     *
     * The sendto() function shall send a message through a connection-mode or connectionless-mode socket.
     * If the socket is connectionless-mode, the message shall be sent to the address specified.
     * If the socket is connection-mode, address shall be ignored.
     *
     * By default, sendto blocks until data is sent. If socket is set to
     * non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     * immediately.
     *
     *  @param address  Remote address
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t sendto(const SocketAddress &address,
                                         const void *data, nsapi_size_t size) = 0;

    /** Receive a data from a socket
     *
     *  Receives a data and stores the source address in address if address
     *  is not NULL. Returns the number of bytes written into the buffer.
     *
     *  If socket is connected, only packets coming from connected peer address
     *  are accepted.
     *
     *  @note recvfrom() is allowed write to address and data buffers even if error occurs.
     *
     *  By default, recvfrom blocks until a datagram is received. If socket is set to
     *  non-blocking or times out with no data, NSAPI_ERROR_WOULD_BLOCK
     *  is returned.
     *
     *  @param address  Destination for the source address or NULL
     *  @param data     Destination buffer for datagram received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t recvfrom(SocketAddress *address,
                                           void *data, nsapi_size_t size) = 0;

    /** Bind a specific address to a socket.
     *
     *  Binding a socket specifies the address and port on which to receive
     *  data. If the IP address is zeroed, only the port is bound.
     *
     *  @param address  Local address to bind
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t bind(const SocketAddress &address) = 0;

    /** Set blocking or non-blocking mode of the socket.
     *
     *  Initially all sockets are in blocking mode. In non-blocking mode
     *  blocking operations such as send/recv/accept return
     *  NSAPI_ERROR_WOULD_BLOCK if they can not continue.
     *
     *  set_blocking(false) is equivalent to set_timeout(0)
     *  set_blocking(true) is equivalent to set_timeout(-1)
     *
     *  @param blocking true for blocking mode, false for non-blocking mode.
     */
    virtual void set_blocking(bool blocking) = 0;

    /** Set timeout on blocking socket operations.
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
    virtual void set_timeout(int timeout) = 0;

    /** Register a callback on state change of the socket.
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
     *  @param func     Function to call on state change.
     */
    virtual void sigio(mbed::Callback<void()> func) = 0;

    /** Set socket options.
     *
     *  setsockopt() allows an application to pass stack-specific options
     *  to the underlying stack using stack-specific level and option names,
     *  or to request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t.
     *  @param optname  Level-specific option name.
     *  @param optval   Option value.
     *  @param optlen   Length of the option value.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen) = 0;

    /** Get socket options.
     *
     *  getsockopt() allows an application to retrieve stack-specific options
     *  from the underlying stack using stack-specific level and option names,
     *  or to request generic options using levels from nsapi_socket_level_t.
     *
     *  For unsupported options, NSAPI_ERROR_UNSUPPORTED is returned
     *  and the socket is unmodified.
     *
     *  @param level    Stack-specific protocol level or nsapi_socket_level_t.
     *  @param optname  Level-specific option name.
     *  @param optval   Destination for option value.
     *  @param optlen   Length of the option value.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen) = 0;

    /** Accepts a connection on a socket.
     *
     *  The server socket must be bound and set to listen for connections.
     *  On a new connection, returns connected network socket which user is expected to call close()
     *  and that deallocates the resources. Referencing a returned pointer after a close()
     *  call is not allowed and leads to undefined behaviour.
     *
     *  By default, accept blocks until incomming connection occurs. If socket is set to
     *  non-blocking or times out, error is set to NSAPI_ERROR_WOULD_BLOCK.
     *
     *  @param error      pointer to storage of the error value or NULL
     *  @return           pointer to a socket
     */
    virtual Socket *accept(nsapi_error_t *error = NULL) = 0;

    /** Listen for incoming connections.
     *
     *  Marks the socket as a passive socket that can be used to accept
     *  incoming connections.
     *
     *  @param backlog  Number of pending connections that can be queued
     *                  simultaneously, defaults to 1
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure
     */
    virtual nsapi_error_t listen(int backlog = 1) = 0;
};


#endif

/** @}*/
