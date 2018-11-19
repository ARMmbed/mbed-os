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

#include "DTLSSocket.h"

#define TRACE_GROUP "TLSS"
#include "mbed-trace/mbed_trace.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C)

nsapi_error_t DTLSSocket::connect(const char *host, uint16_t port)
{
    SocketAddress addr;
    nsapi_error_t ret;

    ret = _udp_socket.getpeername(&addr);
    if (ret != NSAPI_ERROR_NO_CONNECTION) {
        return ret;
    }

    if (!addr || ret == NSAPI_ERROR_NO_CONNECTION) {
        nsapi_error_t err = _udp_socket._stack->gethostbyname(host, &addr);
        if (err) {
            return NSAPI_ERROR_DNS_FAILURE;
        }

        addr.set_port(port);

        set_hostname(host);
        _udp_socket.connect(addr); // UDPSocket::connect() cannot fail
    }

    return connect(addr);
}

DTLSSocket::~DTLSSocket()
{
    // Make sure that DTLSSocketWrapper::close() is called before the transport is
    // destroyed.
    close();
}

#endif // MBEDTLS_SSL_CLI_C
