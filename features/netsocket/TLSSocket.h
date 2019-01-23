/** @file TLSSocket.h TLSSocket */
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

#ifndef _MBED_HTTPS_TLS_TCP_SOCKET_H_
#define _MBED_HTTPS_TLS_TCP_SOCKET_H_

#include "netsocket/TCPSocket.h"
#include "TLSSocketWrapper.h"

#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

/**
 * \brief TLSSocket is a wrapper around TCPSocket for interacting with TLS servers.
 *
 * TLSSocket uses the TLSSocketWrapper with internal TCP socket.
 * This is a helper for creating commonly used TLS connections over TCP.
 *
 */
class TLSSocket : public TLSSocketWrapper {
public:
    /** Create an uninitialized socket.
     *
     *  Must call open to initialize the socket on a network stack.
     */
    TLSSocket() : TLSSocketWrapper(&tcp_socket) {}

    /** Destroy the TLSSocket and closes the transport.
     */
    virtual ~TLSSocket();

    /** Opens a socket.
     *
     *  Creates a network socket on the network stack of the given
     *  network interface.
     *
     *  @note TLSSocket cannot be reopened after closing. It should be destructed to
     *        clear internal TLS memory structures.
     *
     *  @param stack    Network stack as target for socket.
     *  @return         0 on success, negative error code on failure.
     */
    virtual nsapi_error_t open(NetworkStack *stack)
    {
        return tcp_socket.open(stack);
    }

    template <typename S>
    nsapi_error_t open(S *stack)
    {
        return open(nsapi_create_stack(stack));
    }

    using TLSSocketWrapper::connect;

    /** Connects TCP socket to a remote host.
     *
     *  Initiates a connection to a remote server specified by either
     *  a domain name or an IP address and port.
     *
     *  @note: In case connect() returns NSAPI_ERROR_AUTH_FAILURE,
     *  the socket must be freed either by calling close() or destroying it.
     *
     *  @param host     Hostname of the remote host.
     *  @param port     Port of the remote host.
     *  @return         0 on success, negative error code on failure.
     */
    nsapi_error_t connect(const char *host, uint16_t port);

private:
    TCPSocket tcp_socket;
};

#endif // MBEDTLS_SSL_CLI_C
#endif // _MBED_HTTPS_TLS_TCP_SOCKET_H_

/** @} */
