/** @file DTLSSocketWrapper.h DTLSSocketWrapper */
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

#ifndef DTLSSOCKETWRAPPER_H
#define DTLSSOCKETWRAPPER_H

#include "TLSSocketWrapper.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C) || defined(DOXYGEN_ONLY)

/**
 * \brief DTLSSocketWrapper implement DTLS stream over the existing Socket transport.
 */
class DTLSSocketWrapper : public TLSSocketWrapper {
public:
    /** Create a DTLSSocketWrapper.
     *
     * @param transport    Underlying transport socket to wrap.
     * @param hostname     Hostname of the remote host, used for certificate checking.
     * @param control      Transport control mode. See @ref control_transport.
     */
    DTLSSocketWrapper(Socket *transport, const char *hostname = NULL, control_transport control = TRANSPORT_CONNECT_AND_CLOSE);
private:
    static void timing_set_delay(void *ctx, uint32_t int_ms, uint32_t fin_ms);
    static int timing_get_delay(void *ctx);
    void timer_event();
    uint64_t _int_ms_tick = 0;
    int _timer_event_id = 0;
    bool _timer_expired = false;
};

#endif
#endif
/** @} */
