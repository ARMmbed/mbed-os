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

#include "TLSSocket.h"

#define TRACE_GROUP "TLSS"
#include "mbed-trace/mbed_trace.h"

#if !defined(MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET) || !(MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET)

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C)

TLSSocket::~TLSSocket()
{
    /* Transport is a member of TLSSocket which is derived from TLSSocketWrapper.
     * Make sure that TLSSocketWrapper::close() is called before the transport is
     * destroyed.
     */
    close();
}
#endif // MBEDTLS_SSL_CLI_C

#else // MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET

TLSSocket::TLSSocket()
{
}

TLSSocket::~TLSSocket()
{
}


nsapi_error_t TLSSocket::set_hostname(const char *hostname)
{
    return setsockopt(NSAPI_TLSSOCKET_LEVEL, NSAPI_TLSSOCKET_SET_HOSTNAME, hostname, strlen(hostname));
}

nsapi_error_t TLSSocket::set_root_ca_cert(const void *root_ca, size_t len)
{
    return setsockopt(NSAPI_TLSSOCKET_LEVEL, NSAPI_TLSSOCKET_SET_CACERT, root_ca, len);
}

nsapi_error_t TLSSocket::set_root_ca_cert(const char *root_ca_pem)
{
    return set_root_ca_cert(root_ca_pem, strlen(root_ca_pem));
}

nsapi_error_t TLSSocket::set_client_cert_key(const void *client_cert, size_t client_cert_len,
                                             const void *client_private_key_pem, size_t client_private_key_len)
{
    nsapi_error_t ret = setsockopt(NSAPI_TLSSOCKET_LEVEL, NSAPI_TLSSOCKET_SET_CLCERT, client_cert, client_cert_len);
    if (ret == NSAPI_ERROR_OK) {
        ret = setsockopt(NSAPI_TLSSOCKET_LEVEL, NSAPI_TLSSOCKET_SET_CLKEY, client_private_key_pem, client_private_key_len);
    }
    return ret;
}

nsapi_error_t TLSSocket::set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem)
{
    return set_client_cert_key(client_cert_pem, strlen(client_cert_pem), client_private_key_pem, strlen(client_private_key_pem));
}

nsapi_error_t TLSSocket::connect(const SocketAddress &address)
{
    nsapi_error_t ret = enable_tlssocket();
    if (ret == NSAPI_ERROR_OK) {
        ret = TCPSocket::connect(address);
    }
    return ret;
}

nsapi_error_t TLSSocket::enable_tlssocket()
{
    bool enabled = true;
    return setsockopt(NSAPI_TLSSOCKET_LEVEL, NSAPI_TLSSOCKET_ENABLE, &enabled, sizeof(enabled));
}

#endif // MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET
