/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef GEMALTO_CINTERION_CELLULAR_STACK_H_
#define GEMALTO_CINTERION_CELLULAR_STACK_H_

#include "AT_CellularStack.h"

namespace mbed {

class GEMALTO_CINTERION_CellularStack : public AT_CellularStack {
public:
    GEMALTO_CINTERION_CellularStack(ATHandler &atHandler, const char *apn, const char *username,
                                    const char *password, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device);
    virtual ~GEMALTO_CINTERION_CellularStack();

    /** Initialize
     *  Must be called immediately after constructor to initialize IP stack on the modem.
     *  @return NSAPI_ERROR_OK on success
     */
    nsapi_error_t socket_stack_init();

protected:

    virtual nsapi_error_t socket_close_impl(int sock_id);

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                     const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                       void *buffer, nsapi_size_t size);

    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address);

private:
    // socket URC handlers as per Cinterion AT manuals
    void urc_sis();
    void urc_sisw();
    void sisw_urc_handler(int sock_id, int urc_code);
    void urc_sisr();
    void sisr_urc_handler(int sock_id, int urc_code);

    // sockets need a connection profile, one profile is enough to support single stack sockets
    nsapi_error_t create_connection_profile(int connection_profile_id);
    void close_connection_profile(int connection_profile_id);

    // socket open need to be deferred until sendto due to BGS2 does not support UDP server endpoint
    nsapi_error_t socket_open_defer(CellularSocket *socket, const SocketAddress *address = NULL);

    // connection profile configuration needs Access Point Name, User Name and Password
    const char *_apn;
    const char *_user;
    const char *_password;
};

} // namespace mbed

#endif /* GEMALTO_CINTERION_CELLULAR_STACK_H_ */
