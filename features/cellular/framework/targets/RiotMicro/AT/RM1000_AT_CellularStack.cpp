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

#include "RM1000_AT_CellularStack.h"

#include "mbed_poll.h"

#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOT"
#endif // TRACE_GROUP

using namespace mbed;
using namespace mbed_cellular_util;

RM1000_AT_CellularStack::RM1000_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    tr_debug("RM1000_AT_CellularStack::RM1000_AT_CellularStack");

    // URC handlers for sockets
    _at.set_urc_handler("+RUSORCV:", callback(this, &RM1000_AT_CellularStack::RUSORCV_URC));
    _at.set_urc_handler("+RUSOCL:", callback(this, &RM1000_AT_CellularStack::RUSOCL_URC));
}

RM1000_AT_CellularStack::~RM1000_AT_CellularStack()
{
    tr_debug("RM1000_AT_CellularStack::~RM1000_AT_CellularStack");

    _at.set_urc_handler("+RUSORCV:", NULL);
    _at.set_urc_handler("+RUSOCL:", NULL);
}

nsapi_error_t RM1000_AT_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    tr_debug("RM1000_AT_CellularStack::socket_listen");
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t RM1000_AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    tr_debug("RM1000_AT_CellularStack::socket_accept");
    return NSAPI_ERROR_UNSUPPORTED;
}

// Callback for Socket Receive URC.
void RM1000_AT_CellularStack::RUSORCV_URC()
{
    int a, b;
    CellularSocket *socket;

    a = _at.read_int();
    b = _at.read_int();
    socket = find_socket(a);
    if (socket != NULL) {
        socket->pending_bytes = b;
        // No debug prints here as they can affect timing
        // and cause data loss in UARTSerial
        if (socket->_cb != NULL) {
            socket->_cb(socket->_data);
        }
    }
}

// Callback for Socket Close URC.
void RM1000_AT_CellularStack::RUSOCL_URC()
{
    int a;
    CellularSocket *socket;

    a = _at.read_int();
    socket = find_socket(a);
    clear_socket(socket);
}

int RM1000_AT_CellularStack::get_max_socket_count()
{
    tr_debug("RM1000_AT_CellularStack::get_max_socket_count");
    return RM1000_MAX_SOCKET;
}

bool RM1000_AT_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP || protocol == NSAPI_TCP);
}

nsapi_error_t RM1000_AT_CellularStack::create_socket_impl(CellularSocket *socket)
{
    tr_debug("RM1000_AT_CellularStack::create_socket_impl");

    int sock_id = SOCKET_UNUSED;

    nsapi_error_t err = NSAPI_ERROR_OK;
    if (socket->proto == NSAPI_UDP) {
        err = _at.at_cmd_int("+RSOCR", "=0", sock_id);
    } else if (socket->proto == NSAPI_TCP) {
        err = _at.at_cmd_int("+RSOCR", "=1", sock_id);
    } // Unsupported protocol is checked in "is_protocol_supported" function

    if ((err != NSAPI_ERROR_OK) || (sock_id == -1)) {
        tr_error("RM1000_AT_CellularStack::create_socket_impl error sock_id=%d err=%d", sock_id, err);
        return NSAPI_ERROR_NO_SOCKET;
    }

    // Check for duplicate socket id delivered by modem
    for (int i = 0; i < RM1000_MAX_SOCKET; i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock != socket && sock->id == sock_id) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    socket->id = sock_id;

    return err;
}

nsapi_error_t RM1000_AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
    tr_debug("RM1000_AT_CellularStack::socket_connect");

    CellularSocket *socket = (CellularSocket *)handle;

    if (socket) {
        if (socket->id == SOCKET_UNUSED) {
            nsapi_error_t err = create_socket_impl(socket);
            if (err != NSAPI_ERROR_OK) {
                return err;
            }
        }
    } else {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _at.lock();
    _at.cmd_start("AT+RSOCO=");
    _at.write_int(socket->id);
    _at.write_string(addr.get_ip_address(), false);
    _at.write_int(addr.get_port());
    _at.cmd_stop();
    _at.resp_start("+RSOCO:");
    int socket_id = _at.read_int();
    _at.resp_stop();
    nsapi_error_t error = _at.get_last_error();
    _at.unlock();

    if ((error == NSAPI_ERROR_OK) && (socket_id == socket->id)) {
        socket->remoteAddress = addr;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_size_or_error_t RM1000_AT_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                  const void *data, nsapi_size_t size)
{
    tr_debug("RM1000_AT_CellularStack::socket_sendto_impl");

    int sent_len = 0;
    pollfh fhs;
    fhs.fh = _at.get_file_handle();
    fhs.events = POLLIN;

    bool success = true;
    const char *buf = (const char *) data;
    nsapi_size_t blk = RM1000_MAX_PACKET_SIZE;
    nsapi_size_t count = size;

    while ((count > 0) && success) {
        if (count < blk) {
            blk = count;
        }
        _at.cmd_start("AT+RSOSND=");
        _at.write_int(socket->id);
        _at.write_int(blk);
        if (socket->proto == NSAPI_UDP) {
            _at.write_string(address.get_ip_address(), false);
            _at.write_int(address.get_port());
        }
        _at.cmd_stop();
        (void)poll(&fhs, 1, 50);
        _at.write_bytes((uint8_t *)buf, blk);

        _at.resp_start("+RSOSND:");
        _at.skip_param(); // skip socket id
        sent_len = _at.read_int();
        _at.resp_stop();

        if ((sent_len >= (int) blk) &&
                (_at.get_last_error() == NSAPI_ERROR_OK)) {
        } else {
            success = false;
        }

        buf += blk;
        count -= blk;
    }

    if (success && _at.get_last_error() == NSAPI_ERROR_OK) {
        return size - count;
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t RM1000_AT_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                    void *buffer, nsapi_size_t size)
{
    tr_debug("RM1000_AT_CellularStack::socket_recvfrom_impl");

    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    nsapi_size_t read_blk;
    nsapi_size_t count = 0;
    nsapi_size_t rsorcv_sz;
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
    while (success && (size > 0)) {
        read_blk = RM1000_MAX_PACKET_SIZE;
        if (read_blk > size) {
            read_blk = size;
        }
        if (socket->pending_bytes > 0) {
            _at.cmd_start_stop("+RSORCV", "=", "%d%d", socket->id, read_blk);

            _at.resp_start("+RSORCV:");
            _at.skip_param(); // receiving socket id
            rsorcv_sz = _at.read_int();
            if (socket->proto == NSAPI_UDP) {
                _at.read_string(ipAddress, sizeof(ipAddress));
                port = _at.read_int();
            }
            if (rsorcv_sz > size) {
                rsorcv_sz = size;
            }
            _at.read_bytes((uint8_t *)buffer + count, rsorcv_sz);
            _at.resp_stop();

            // Must use what +RSORCV returns here as it may be less or more than we asked for
            if (rsorcv_sz > socket->pending_bytes) {
                socket->pending_bytes = 0;
            } else {
                socket->pending_bytes -= rsorcv_sz;
            }

            if (rsorcv_sz > 0) {
                count += rsorcv_sz;
                size -= rsorcv_sz;
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
                success = false;
            }
            break;
        }
    }
    timer.stop();

    if (!count || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        return NSAPI_ERROR_WOULD_BLOCK;
    } else {
        nsapi_error_size = count;
    }

    if (success && socket->proto == NSAPI_UDP && address) {
        address->set_ip_address(ipAddress);
        address->get_ip_address();
        address->set_port(port);
    }

    return nsapi_error_size;
}

nsapi_error_t RM1000_AT_CellularStack::socket_close_impl(int sock_id)
{
    tr_debug("RM1000_AT_CellularStack::socket_close_impl");

    return _at.at_cmd_discard("+RSOCL", "=", "%d", sock_id);
}

// Clear out the storage for a socket
void RM1000_AT_CellularStack::clear_socket(CellularSocket *socket)
{
    if (socket != NULL) {
        socket->id       = SOCKET_UNUSED;
        socket->pending_bytes = 0;
        socket->_cb      = NULL;
        socket->_data    = NULL;
    }
}

nsapi_error_t RM1000_AT_CellularStack::gethostbyname(const char *host, SocketAddress *address, nsapi_version_t version, const char *interface_name)
{
    tr_debug("RM1000_AT_CellularStack::gethostbyname");

    char ipAddress[NSAPI_IP_SIZE];
    nsapi_error_t err = NSAPI_ERROR_NO_CONNECTION;

    _at.lock();
    if (address->set_ip_address(host)) {
        err = NSAPI_ERROR_OK;
    } else {
        // This interrogation can sometimes take longer than the usual 8 seconds
        _at.cmd_start("AT+RDNS=");
        _at.write_string(host, false);
        _at.cmd_stop();

        _at.set_at_timeout(70000);
        _at.resp_start("+RDNS:");
        if (_at.info_resp()) {
            _at.read_string(ipAddress, sizeof(ipAddress));
            if (address->set_ip_address(ipAddress)) {
                err = NSAPI_ERROR_OK;
            }
        }
        _at.resp_stop();
        _at.restore_at_timeout();
    }
    _at.unlock();

    return err;
}
