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

#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

#if !defined(MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET) || !(MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET)

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

#include "TLSSocketWrapper.h"

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
     *  @return         NSAPI_ERROR_OK on success. See @ref TCPSocket::open
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

private:
    TCPSocket tcp_socket;
};
#endif // MBEDTLS_SSL_CLI_C

#else // MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET

class TLSSocket : public TCPSocket {
public:
    TLSSocket();
    virtual ~TLSSocket();

    /** Set hostname.
     *
     * TLSSocket requires hostname used to verify the certificate.
     * If hostname is not given in constructor, this function must be used before
     * starting the TLS handshake.
     *
     * @param hostname     Hostname of the remote host, used for certificate checking.
     */
    nsapi_error_t set_hostname(const char *hostname);

    /** Sets the certification of Root CA.
     *
     * @note Must be called after open() before calling connect()
     *
     * @param root_ca Root CA Certificate in any Mbed TLS-supported format.
     * @param len     Length of certificate (including terminating 0 for PEM).
     * @return        NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t set_root_ca_cert(const void *root_ca, size_t len);

    /** Sets the certification of Root CA.
     *
     * @note Must be called after open() before calling connect()
     *
     * @param root_ca_pem Root CA Certificate in PEM format.
     */
    virtual nsapi_error_t set_root_ca_cert(const char *root_ca_pem);


    /** Sets client certificate, and client private key.
     *
     * @param client_cert Client certification in PEM or DER format.
     * @param client_cert_len Certificate size including the terminating null byte for PEM data.
     * @param client_private_key_pem Client private key in PEM or DER format.
     * @param client_private_key_len Key size including the terminating null byte for PEM data
     * @return   NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t set_client_cert_key(const void *client_cert, size_t client_cert_len,
                                              const void *client_private_key_pem, size_t client_private_key_len);

    /** Sets client certificate, and client private key.
     *
     * @param client_cert_pem Client certification in PEM format.
     * @param client_private_key_pem Client private key in PEM format.
     * @return   NSAPI_ERROR_OK on success, negative error code on failure.
     */
    virtual nsapi_error_t set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem);

    // From TCPSocket
    virtual nsapi_error_t connect(const char *host, uint16_t port);
    virtual nsapi_error_t connect(const SocketAddress &address);

protected:
    virtual nsapi_error_t enable_tlssocket();
};

#endif // MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET

#endif // _MBED_HTTPS_TLS_TCP_SOCKET_H_

/** @} */
