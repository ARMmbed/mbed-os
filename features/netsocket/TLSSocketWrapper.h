/** @file TLSSocketWrapper.h TLSSocketWrapper */
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

#ifndef _MBED_HTTPS_TLS_SOCKET_WRAPPER_H_
#define _MBED_HTTPS_TLS_SOCKET_WRAPPER_H_

#include "netsocket/Socket.h"
#include "rtos/EventFlags.h"
#include "platform/Callback.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

/**
 * TLSSocket is a wrapper around Socket for interacting with TLS servers.
 *
 * TLSSocketWrapper can use any Socket as a transport. After
 * completing the TLS handshake, it can be used as any Socket would be used.
 *
 */
class TLSSocketWrapper : public Socket {
public:
    /** Transport modes */
    enum control_transport {
        TRANSPORT_KEEP,                 /**< Doesn't call connect() or close() on transport socket */
        TRANSPORT_CONNECT_AND_CLOSE,    /**< Does call connect() and close() on transport socket */
        TRANSPORT_CONNECT,              /**< Does call only connect() on transport socket */
        TRANSPORT_CLOSE,                /**< Does call close() on transport socket */
    };

    /** Create a TLSSocketWrapper.
     *
     * @param transport    Underlying transport socket to wrap.
     * @param hostname     Hostname of the remote host, used for certificate checking.
     * @param control      Transport control mode. See @ref control_transport.
     */
    TLSSocketWrapper(Socket *transport, const char *hostname = NULL, control_transport control = TRANSPORT_CONNECT_AND_CLOSE);

    /** Destroy a socket wrapper.
     *
     *  Closes socket wrapper if the socket wrapper is still open.
     */
    virtual ~TLSSocketWrapper();

    /** Set hostname.
     *
     * TLSSocket requires hostname used to verify the certificate.
     * If hostname is not given in constructor, this function must be used before
     * starting the TLS handshake.
     *
     * @param hostname     Hostname of the remote host, used for certificate checking.
     */
    void set_hostname(const char *hostname);

    /** Sets the certification of Root CA.
     *
     * @note Must be called before calling connect()
     *
     * @param root_ca Root CA Certificate in any Mbed TLS-supported format.
     * @param len     Length of certificate (including terminating 0 for PEM).
     * @return        0 on success, negative error code on failure.
     */
    nsapi_error_t set_root_ca_cert(const void *root_ca, size_t len);

    /** Sets the certification of Root CA.
     *
     * @note Must be called before calling connect()
     *
     * @param root_ca_pem Root CA Certificate in PEM format.
     */
    nsapi_error_t set_root_ca_cert(const char *root_ca_pem);

    /** Sets client certificate, and client private key.
     *
     * @param client_cert Client certification in PEM or DER format.
     * @param client_cert_len Certificate size including the terminating null byte for PEM data.
     * @param client_private_key_pem Client private key in PEM or DER format.
     * @param client_private_key_len Key size including the terminating null byte for PEM data
     * @return   0 on success, negative error code on failure.
     */
    nsapi_error_t set_client_cert_key(const void *client_cert, size_t client_cert_len,
                                      const void *client_private_key_pem, size_t client_private_key_len);

    /** Sets client certificate, and client private key.
     *
     * @param client_cert_pem Client certification in PEM format.
     * @param client_private_key_pem Client private key in PEM format.
     * @return   0 on success, negative error code on failure.
     */
    nsapi_error_t set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem);

    /** Send data over a TLS socket.
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes sent from the buffer.
     *
     *  @param data     Buffer of data to send to the host.
     *  @param size     Size of the buffer in bytes.
     *  @return         Number of sent bytes on success, negative error code on failure.
     */
    virtual nsapi_error_t send(const void *data, nsapi_size_t size);

    /** Receive data over a TLS socket.
     *
     *  The socket must be connected to a remote host. Returns the number of
     *  bytes received into the buffer.
     *
     *  @param data     Destination buffer for data received from the host.
     *  @param size     Size of the buffer in bytes.
     *  @return         Number of received bytes on success, negative error
     *                  code on failure. If no data is available to be received
     *                  and the peer has performed an orderly shutdown,
     *                  recv() returns 0.
     */
    virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size);

    /* = Functions inherited from Socket = */
    virtual nsapi_error_t close();
    /*
     *  @note: In case connect() returns an error, the state of the socket is
     *  unspecified. A new socket should be created before reconnecting.
     */
    virtual nsapi_error_t connect(const SocketAddress &address = SocketAddress());
    virtual nsapi_size_or_error_t sendto(const SocketAddress &address, const void *data, nsapi_size_t size);
    virtual nsapi_size_or_error_t recvfrom(SocketAddress *address,
                                           void *data, nsapi_size_t size);
    virtual nsapi_error_t bind(const SocketAddress &address);
    virtual void set_blocking(bool blocking);
    virtual void set_timeout(int timeout);
    virtual void sigio(mbed::Callback<void()> func);
    virtual nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);
    virtual nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen);
    virtual Socket *accept(nsapi_error_t *error = NULL);
    virtual nsapi_error_t listen(int backlog = 1);
    virtual nsapi_error_t getpeername(SocketAddress *address);

#if defined(MBEDTLS_X509_CRT_PARSE_C) || defined(DOXYGEN_ONLY)
    /** Get own certificate directly from Mbed TLS.
     *
     * @return Internal Mbed TLS X509 structure.
     */
    mbedtls_x509_crt *get_own_cert();

    /** Set own certificate directly to Mbed TLS.
     *
     * @param crt Mbed TLS X509 certificate chain.
     * @return error code from mbedtls_ssl_conf_own_cert().
     */
    int set_own_cert(mbedtls_x509_crt *crt);

    /** Get CA chain structure.
     *
     * @return Mbed TLS X509 certificate chain.
     */
    mbedtls_x509_crt *get_ca_chain();

    /** Set CA chain directly to Mbed TLS.
     *
     * @param crt Mbed TLS X509 certificate chain.
     */
    void set_ca_chain(mbedtls_x509_crt *crt);
#endif

    /** Get internal Mbed TLS configuration structure.
     *
     * @return Mbed TLS SSL config.
     */
    mbedtls_ssl_config *get_ssl_config();

    /** Override Mbed TLS configuration.
     *
     * @param conf Mbed TLS SSL configuration structure.
     */
    void set_ssl_config(mbedtls_ssl_config *conf);

    /** Get internal Mbed TLS context structure.
     *
     * @return SSL context.
     */
    mbedtls_ssl_context *get_ssl_context();

protected:
#ifndef DOXYGEN_ONLY
    /** Initiates TLS Handshake.
     *
     *  Initiates a TLS handshake to a remote peer.
     *  Underlying transport socket should already be connected.
     *
     *  Root CA certification must be set by set_ssl_ca_pem() before
     *  calling this function.
     *
     *  For non-blocking purposes, this functions needs to know whether this
     *  was a first call to Socket::connect() API so that NSAPI_ERROR_INPROGRESS
     *  does not happen twice.
     *
     *  @param        first_call is this a first call to Socket::connect() API.
     *  @return       0 on success, negative error code on failure
     */
    nsapi_error_t start_handshake(bool first_call);

    bool is_handshake_started() const;

    void event();
#endif



private:
    /** Continue already initialized handshake */
    nsapi_error_t continue_handshake();
    /**
     * Helper for pretty-printing Mbed TLS error codes
     */
    static void print_mbedtls_error(const char *name, int err);

#if MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0
    /**
     * Debug callback for Mbed TLS
     * Just prints on the USB serial port
     */
    static void my_debug(void *ctx, int level, const char *file, int line,
                         const char *str);

    /**
     * Certificate verification callback for Mbed TLS
     * Here we only use it to display information on each cert in the chain
     */
    static int my_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags);

#endif /* MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0 */

    /**
     * Receive callback for Mbed TLS
     */
    static int ssl_recv(void *ctx, unsigned char *buf, size_t len);

    /**
     * Send callback for Mbed TLS
     */
    static int ssl_send(void *ctx, const unsigned char *buf, size_t len);

    mbedtls_ssl_context _ssl;
#ifdef MBEDTLS_X509_CRT_PARSE_C
    mbedtls_pk_context _pkctx;
#endif
    mbedtls_ctr_drbg_context _ctr_drbg;
    mbedtls_entropy_context _entropy;

    rtos::EventFlags _event_flag;
    mbed::Callback<void()> _sigio;
    Socket *_transport;
    int _timeout;

#ifdef MBEDTLS_X509_CRT_PARSE_C
    mbedtls_x509_crt *_cacert;
    mbedtls_x509_crt *_clicert;
#endif
    mbedtls_ssl_config *_ssl_conf;

    bool _connect_transport: 1;
    bool _close_transport: 1;
    bool _tls_initialized: 1;
    bool _handshake_completed: 1;
    bool _cacert_allocated: 1;
    bool _clicert_allocated: 1;
    bool _ssl_conf_allocated: 1;

};

#endif /* MBEDTLS_SSL_CLI_C */
#endif // _MBED_HTTPS_TLS_SOCKET_WRAPPER_H_
/** @} */
