/** @file DTLSSocket.h DTLSSocket */
/*
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
/** @addtogroup netsocket
* @{
*/

#ifndef DTLSSOCKET_H
#define DTLSSOCKET_H

#include "DTLSSocketWrapper.h"
#include "SocketAddress.h"
#include "UDPSocket.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

/**
 * \brief DTLSSocket implement DTLS stream over UDP Socket.
 *
 * This is a helper class that uses DTLSSocketWrapper with
 * internal UDPSocket.
 */

class DTLSSocket : public DTLSSocketWrapper {
public:
    /** Create an uninitialized DTLS socket.
     *
     *  Must call open to initialize the socket on a network stack.
     */
    DTLSSocket() : DTLSSocketWrapper(&_udp_socket) {}

    /** Destroy the DTLSSocket and closes the transport.
     */
    ~DTLSSocket() override;

    /** Create a socket on a network interface.
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *  If hostname is also given, user is not required to call set_hostname() later.
     *
     *  @param stack    Network stack as target for socket.
     *  @param hostname Hostname used for certificate verification.
     */
    template <typename S>
    DTLSSocket(S *stack, const char *hostname = NULL) : DTLSSocketWrapper(&_udp_socket, hostname)
    {
        nsapi_error_t ret = _udp_socket.open(stack);
        MBED_ASSERT(ret == NSAPI_ERROR_OK);
    }

    /** Opens a socket.
     *
     *  Creates a network socket on the network stack of the given
     *  network interface. Not needed if stack is passed to the
     *  socket's constructor.
     *
     *  @param stack    Network stack as target for socket.
     *  @return         NSAPI_ERROR_OK on success, negative error code on failure.
     *                  See @ref UDPSocket::open.
     */
    nsapi_error_t open(NetworkStack *stack)
    {
        return _udp_socket.open(stack);
    }

    template <typename S>
    nsapi_error_t open(S *stack)
    {
        return open(nsapi_create_stack(stack));
    }

    using DTLSSocketWrapper::connect;

private:
    UDPSocket _udp_socket;
};

#endif
#endif
/** @} */
