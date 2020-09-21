/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#ifndef TELIT_ME310_CELLULARSTACK_H_
#define TELIT_ME310_CELLULARSTACK_H_

#include "AT_CellularStack.h"
#include "mbed_trace.h"
#include "drivers/Timer.h"


namespace mbed {

#define ME310_SOCKET_MAX 6
#define ME310_CONTEXT_MAX 6
#define ME310_CREATE_SOCKET_TIMEOUT 150000 //150 seconds
#define ME310_CLOSE_SOCKET_TIMEOUT 20000 // TCP socket max timeout is >10sec
#define ME310_MAX_RECV_SIZE 1000
#define ME310_MAX_SEND_SIZE 1023
#define ME310_SOCKET_BIND_FAIL 556
#define ME310_IPEASY_ACTIVATED_CONTEXT 1
#define ME310_IPEASY_DEACTIVATED_CONTEXT 0
#define CTRL_Z  "\x1a"
#define ESC     "\x1b"

class TELIT_ME310_CellularStack : public AT_CellularStack {
public:
    TELIT_ME310_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device);
    virtual ~TELIT_ME310_CellularStack();

protected: // NetworkStack

    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address = 0);

    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address);

#if defined(MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET) && (MBED_CONF_NSAPI_OFFLOAD_TLSSOCKET)
    virtual nsapi_error_t setsockopt(nsapi_socket_t handle, int level,
                                     int optname, const void *optval, unsigned optlen);
#endif

protected: // AT_CellularStack

    virtual int get_max_socket_count();

    virtual bool is_protocol_supported(nsapi_protocol_t protocol);

    virtual nsapi_error_t socket_close_impl(int sock_id);

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                     const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                       void *buffer, nsapi_size_t size);

private:
    // URC handler for socket data being received
    void urc_sring();

    bool is_ipeasy_context_activated(int context_id);
    nsapi_error_t activate_ipeasy_context(int context_id);
    nsapi_error_t deactivate_ipeasy_context(int context_id);

    uint8_t _tls_sec_level;
};
} // namespace mbed
#endif /* TELIT_ME310_CELLULARSTACK_H_ */