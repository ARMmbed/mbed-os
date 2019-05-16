/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#ifndef UBLOX_N2XX_CELLULARSTACK_H_
#define UBLOX_N2XX_CELLULARSTACK_H_

#include "AT_CellularStack.h"
#include "CellularUtil.h"
#include "mbed_wait_api.h"
#include "drivers/Timer.h"

namespace mbed {

class UBLOX_N2XX_CellularStack : public AT_CellularStack {

public:

    UBLOX_N2XX_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type);
    virtual ~UBLOX_N2XX_CellularStack();

protected:

    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address = 0);

protected:

    /** Socket timeout value in milliseconds.
     * Note: the sockets layer above will retry the
     * call to the functions here when they return NSAPI_ERROR_WOULD_BLOCK
     * and the user has set a larger timeout or full blocking.
     */
    static const int SOCKET_TIMEOUT = 1000;

    /** Maximum allowed sockets.
     */
    static const int N2XX_MAX_SOCKET = 7;

    /** The maximum number of bytes in a packet that can be write/read from
     * the AT interface in one go.
     */
    static const int N2XX_MAX_PACKET_SIZE = 512;

    virtual int get_max_socket_count();

    virtual bool is_protocol_supported(nsapi_protocol_t protocol);

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                     const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                       void *buffer, nsapi_size_t size);

    virtual nsapi_error_t socket_close_impl(int sock_id);

private:

    // URC handlers
    void NSONMI_URC();
};

} // namespace mbed

#endif // UBLOX_N2XX_CELLULARSTACK_H_
