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

#include "UBLOX_N2XX_CellularStack.h"
#include "CellularUtil.h"
#include <stdlib.h>

using namespace mbed;
using namespace mbed_cellular_util;

UBLOX_N2XX_CellularStack::UBLOX_N2XX_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type): AT_CellularStack(atHandler, cid, stack_type)
{
    // URC handlers for sockets
    _at.set_urc_handler("+NSONMI:", callback(this, &UBLOX_N2XX_CellularStack::NSONMI_URC));
}

UBLOX_N2XX_CellularStack::~UBLOX_N2XX_CellularStack()
{
    _at.set_urc_handler("+NSONMI:", NULL);
}

nsapi_error_t UBLOX_N2XX_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t UBLOX_N2XX_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

// Callback for Socket Read URC.
void UBLOX_N2XX_CellularStack::NSONMI_URC()
{
    int a, b;
    CellularSocket *socket;

    a = _at.read_int();
    b = _at.read_int();

    socket = find_socket(a);
    if (socket != NULL) {
        socket->rx_avail = true;
        socket->pending_bytes = b;
        // No debug prints here as they can affect timing
        // and cause data loss in UARTSerial
        if (socket->_cb != NULL) {
            socket->_cb(socket->_data);
        }
    }
}

int UBLOX_N2XX_CellularStack::get_max_socket_count()
{
    return N2XX_MAX_SOCKET;
}

bool UBLOX_N2XX_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP);
}

nsapi_error_t UBLOX_N2XX_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int sock_id = -1;
    int localport = socket->localAddress.get_port();

    if (localport == 5683 || localport < 0 || localport > 65535) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _at.lock();
    _at.cmd_start_stop("+NSOCR", "=", "%s%d%d%d", "DGRAM", 17, localport, 1);

    _at.resp_start();
    sock_id = _at.read_int();
    _at.resp_stop();

    if ((_at.get_last_error() != NSAPI_ERROR_OK) || (sock_id == -1)) {
        _at.unlock();
        return NSAPI_ERROR_NO_SOCKET;
    }
    _at.unlock();

    // Check for duplicate socket id delivered by modem
    for (int i = 0; i < N2XX_MAX_SOCKET; i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock != socket && sock->id == sock_id) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    socket->started = true;
    socket->id = sock_id;

    return NSAPI_ERROR_OK;
}

nsapi_size_or_error_t UBLOX_N2XX_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                   const void *data, nsapi_size_t size)
{
    MBED_ASSERT(socket->id != -1);

    if (size > N2XX_MAX_PACKET_SIZE) {
        return NSAPI_ERROR_PARAMETER;
    }

    int sent_len = 0;
    char *dataStr = new char [(size * 2) + 1]();
    if (!dataStr) {
        return NSAPI_ERROR_NO_MEMORY;
    }
    char_str_to_hex_str((const char *)data, size, dataStr);

    _at.cmd_start_stop("+NSOST", "=", "%d%s%d%d%s", socket->id, address.get_ip_address(),
                       address.get_port(), size, dataStr);

    _at.resp_start();
    _at.skip_param(); // skip socket id
    sent_len = _at.read_int();
    _at.resp_stop();

    delete[] dataStr;
    if ((_at.get_last_error() == NSAPI_ERROR_OK)) {
        return sent_len;
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t UBLOX_N2XX_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                     void *buffer, nsapi_size_t size)
{
    MBED_ASSERT(socket->id != -1);

    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    nsapi_size_t read_blk, usorf_sz, count = 0, length = size;
    bool success = true;
    char ipAddress[NSAPI_IP_SIZE];
    int port = 0;
    Timer timer;

    if (socket->pending_bytes == 0) {
        _at.process_oob();
        if (socket->pending_bytes == 0) {
            return NSAPI_ERROR_WOULD_BLOCK;
        }
    }

    timer.start();
    while (success && (length > 0)) {
        read_blk = N2XX_MAX_PACKET_SIZE;
        if (read_blk > length) {
            read_blk = length;
        }
        if (socket->pending_bytes > 0) {
            _at.cmd_start_stop("+NSORF", "=", "%d%d", socket->id, read_blk);

            _at.resp_start();
            _at.skip_param(); // receiving socket id
            _at.read_string(ipAddress, sizeof(ipAddress));
            port = _at.read_int();
            usorf_sz = _at.read_int();
            if (usorf_sz > length) {
                usorf_sz = length;
            }
            _at.read_hex_string((char *)buffer + count, usorf_sz);
            _at.resp_stop();

            // Must use what +NSORF returns here as it may be less or more than we asked for
            if (usorf_sz >= socket->pending_bytes) {
                socket->pending_bytes = 0;
            } else {
                socket->pending_bytes -= usorf_sz;
            }

            if (usorf_sz > 0) {
                count += (usorf_sz);
                length -= (usorf_sz);
            } else {
                // read() should not fail
                success = false;
            }
        }  else if (timer.read_ms() < SOCKET_TIMEOUT) {
            // Wait for URCs
            _at.process_oob();
        } else {
            if (count == 0) {
                // Timeout with nothing received
                nsapi_error_size = NSAPI_ERROR_WOULD_BLOCK;
                success = false;
            }
            length = 0; // This simply to cause an exit
        }
    }
    timer.stop();

    socket->rx_avail = false;
    socket->pending_bytes = 0;
    if (!count || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (success && socket->proto == NSAPI_UDP && address) {
        address->set_ip_address(ipAddress);
        address->get_ip_address();
        address->set_port(port);
    }

    return nsapi_error_size = count;
}

nsapi_error_t UBLOX_N2XX_CellularStack::socket_close_impl(int sock_id)
{
    return _at.at_cmd_discard("+NSOCL", "=", "%d", sock_id);
}
