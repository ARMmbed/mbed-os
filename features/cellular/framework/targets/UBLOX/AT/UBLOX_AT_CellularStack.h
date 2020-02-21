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
#ifndef UBLOX_AT_CELLULARSTACK_H_
#define UBLOX_AT_CELLULARSTACK_H_

#include "AT_CellularStack.h"
#include "CellularUtil.h"
#include "rtos/ThisThread.h"
#include "drivers/Timer.h"


namespace mbed {

class UBLOX_AT_CellularStack : public AT_CellularStack {
public:
    UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device);
    virtual ~UBLOX_AT_CellularStack();

#ifndef UBX_MDM_SARA_R41XM
    virtual nsapi_error_t get_ip_address(SocketAddress *address);
#endif

    virtual nsapi_error_t gethostbyname(const char *host,
                                        SocketAddress *address, nsapi_version_t version = NSAPI_UNSPEC, const char *interface_name = NULL);

protected:
    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address = 0);

    /** The profile to use (on board the modem).
     */
#define PROFILE 0

    /** Socket "unused" value.
     */
    static const int SOCKET_UNUSED = -1;

    /** Socket timeout value in milliseconds.
     * Note: the sockets layer above will retry the
     * call to the functions here when they return NSAPI_ERROR_WOULD_BLOCK
     * and the user has set a larger timeout or full blocking.
     */
    static const int SOCKET_TIMEOUT = 1000;

    /** The maximum number of bytes in a packet that can be write/read from
     * the AT interface in one go.
     */
    static const int UBLOX_MAX_PACKET_SIZE = 1024;

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    virtual nsapi_error_t socket_connect(nsapi_socket_t handle, const SocketAddress &address);

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                     const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                       void *buffer, nsapi_size_t size);

    virtual nsapi_error_t socket_close_impl(int sock_id);

private:
    // URC handlers
    void UUSORD_URC();
    void UUSORF_URC();
    void UUSOCL_URC();
    void UUPSDD_URC();

    /** Find a socket from the list.
     *
     * @param id       Socket ID.
     * @return         Socket if True, otherwise NULL.
     */
    CellularSocket *find_socket(int id = SOCKET_UNUSED);

    /** Clear out the storage for a socket.
     *
     * @param id       Cellular Socket.
     * @return         None
     */
    void clear_socket(CellularSocket *socket);
};
} // namespace mbed
#endif /* UBLOX_AT_CELLULARSTACK_H_ */
