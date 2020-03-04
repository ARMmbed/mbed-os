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

#include "UBLOX_AT_CellularStack.h"
#include "rtos/ThisThread.h"

using namespace mbed;
using namespace mbed_cellular_util;

UBLOX_AT_CellularStack::UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type, AT_CellularDevice &device) :
    AT_CellularStack(atHandler, cid, stack_type, device)
{
    // URC handlers for sockets
    _at.set_urc_handler("+UUSORD:", callback(this, &UBLOX_AT_CellularStack::UUSORD_URC));
    _at.set_urc_handler("+UUSORF:", callback(this, &UBLOX_AT_CellularStack::UUSORF_URC));
    _at.set_urc_handler("+UUSOCL:", callback(this, &UBLOX_AT_CellularStack::UUSOCL_URC));
    _at.set_urc_handler("+UUPSDD:", callback(this, &UBLOX_AT_CellularStack::UUPSDD_URC));
}

UBLOX_AT_CellularStack::~UBLOX_AT_CellularStack()
{
}

nsapi_error_t UBLOX_AT_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t UBLOX_AT_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

// Callback for Socket Read URC.
void UBLOX_AT_CellularStack::UUSORD_URC()
{
    int a, b;
    CellularSocket *socket;

    a = _at.read_int();
    b = _at.read_int();

    socket = find_socket(a);
    if (socket != NULL) {
        socket->pending_bytes = b;
        // No debug prints here as they can affect timing
        // and cause data loss in BufferedSerial
        if (socket->_cb != NULL) {
            socket->_cb(socket->_data);
        }
    }
}

// Callback for Socket Read From URC.
void UBLOX_AT_CellularStack::UUSORF_URC()
{
    int a, b;
    CellularSocket *socket;

    a = _at.read_int();
    b = _at.read_int();

    socket = find_socket(a);
    if (socket != NULL) {
        socket->pending_bytes = b;
        // No debug prints here as they can affect timing
        // and cause data loss in BufferedSerial
        if (socket->_cb != NULL) {
            socket->_cb(socket->_data);
        }
    }
}

// Callback for Socket Close URC.
void UBLOX_AT_CellularStack::UUSOCL_URC()
{
    int a;
    CellularSocket *socket;

    a = _at.read_int();
    socket = find_socket(a);
    clear_socket(socket);
}

// Callback for UUPSDD.
void UBLOX_AT_CellularStack::UUPSDD_URC()
{
    int a;
    CellularSocket *socket;

    a = _at.read_int();
    socket = find_socket(a);
    clear_socket(socket);
}

nsapi_error_t UBLOX_AT_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int sock_id = SOCKET_UNUSED;

    nsapi_error_t err = NSAPI_ERROR_OK;
    if (socket->proto == NSAPI_UDP) {
        err = _at.at_cmd_int("+USOCR", "=17", sock_id);
    } else if (socket->proto == NSAPI_TCP) {
        err = _at.at_cmd_int("+USOCR", "=6", sock_id);
    } // Unsupported protocol is checked in socket_open()

    if ((err != NSAPI_ERROR_OK) || (sock_id == -1)) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    // Check for duplicate socket id delivered by modem
    for (int i = 0; i < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT); i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock != socket && sock->id == sock_id) {
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    socket->started = true;
    socket->id = sock_id;

    return err;
}

nsapi_error_t UBLOX_AT_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &addr)
{
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

    nsapi_error_t err = _at.at_cmd_discard("+USOCO", "=", "%d%s%d", socket->id, addr.get_ip_address(), addr.get_port());

    if (err == NSAPI_ERROR_OK) {
        socket->remoteAddress = addr;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                 const void *data, nsapi_size_t size)
{
    MBED_ASSERT(socket->id != -1);

    int sent_len = 0;
    uint8_t ch = 0;

    if (socket->proto == NSAPI_UDP) {
        if (size > UBLOX_MAX_PACKET_SIZE) {
            return NSAPI_ERROR_PARAMETER;
        }
        _at.cmd_start_stop("+USOST", "=", "%d%s%d%d", socket->id, address.get_ip_address(), address.get_port(), size);
        _at.resp_start("@", true);
        rtos::ThisThread::sleep_for(50); //wait for 50ms before sending data

        _at.write_bytes((uint8_t *)data, size);

        _at.resp_start("+USOST:");
        _at.skip_param(); // skip socket id
        sent_len = _at.read_int();
        _at.resp_stop();

        if ((_at.get_last_error() == NSAPI_ERROR_OK)) {
            return sent_len;
        }
    } else if (socket->proto == NSAPI_TCP) {
        bool success = true;
        const char *buf = (const char *) data;
        nsapi_size_t blk = UBLOX_MAX_PACKET_SIZE;
        nsapi_size_t count = size;

        while ((count > 0) && success) {
            if (count < blk) {
                blk = count;
            }
            _at.cmd_start_stop("+USOWR", "=", "%d%d", socket->id, blk);
            _at.resp_start("@", true);
            rtos::ThisThread::sleep_for(50); //wait for 50ms before sending data

            _at.write_bytes((uint8_t *)buf, blk);

            _at.resp_start("+USOWR:");
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
    }

    return _at.get_last_error();
}

nsapi_size_or_error_t UBLOX_AT_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                   void *buffer, nsapi_size_t size)
{
    MBED_ASSERT(socket->id != -1);

    nsapi_size_or_error_t nsapi_error_size = NSAPI_ERROR_DEVICE_ERROR;
    bool success = true;
    nsapi_size_t read_blk;
    nsapi_size_t count = 0;
    nsapi_size_t usorf_sz;
    char ipAddress[NSAPI_IP_SIZE];
    uint8_t ch = 0;
    int port = 0;
    Timer timer;

    if (socket->pending_bytes == 0) {
        _at.process_oob();
        if (socket->pending_bytes == 0) {
            return NSAPI_ERROR_WOULD_BLOCK;
        }
    }

    timer.start();
    if (socket->proto == NSAPI_UDP) {
        bool packet_received = false;
        while (success && (size > 0 && !packet_received)) {
            read_blk = UBLOX_MAX_PACKET_SIZE;
            if (read_blk > size) {
                read_blk = size;
            }
            if (socket->pending_bytes > 0) {
                _at.cmd_start_stop("+USORF", "=", "%d%d", socket->id, read_blk);

                _at.resp_start("+USORF:");
                _at.skip_param(); // receiving socket id
                _at.read_string(ipAddress, sizeof(ipAddress));
                port = _at.read_int();
                usorf_sz = _at.read_int();
                if (usorf_sz > size) {
                    usorf_sz = size;
                } else {
                    packet_received = true;
                }
                _at.read_bytes(&ch, 1);
                _at.read_bytes((uint8_t *)buffer + count, usorf_sz);
                _at.resp_stop();

                // Must use what +USORF returns here as it may be less or more than we asked for
                if (usorf_sz > socket->pending_bytes) {
                    socket->pending_bytes = 0;
                } else {
                    socket->pending_bytes -= usorf_sz;
                }

                if (usorf_sz > 0) {
                    count += usorf_sz;
                    size -= usorf_sz;
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
    } else if (socket->proto == NSAPI_TCP) {
        while (success && (size > 0)) {
            read_blk = UBLOX_MAX_PACKET_SIZE;
            if (read_blk > size) {
                read_blk = size;
            }
            if (socket->pending_bytes > 0) {
                _at.cmd_start_stop("+USORD", "=", "%d%d", socket->id, read_blk);

                _at.resp_start("+USORD:");
                _at.skip_param(); // receiving socket id
                usorf_sz = _at.read_int();
                if (usorf_sz > size) {
                    usorf_sz = size;
                }
                _at.read_bytes(&ch, 1);
                _at.read_bytes((uint8_t *)buffer + count, usorf_sz);
                _at.resp_stop();

                // Must use what +USORD returns here as it may be less or more than we asked for
                if (usorf_sz > socket->pending_bytes) {
                    socket->pending_bytes = 0;
                } else {
                    socket->pending_bytes -= usorf_sz;
                }

                if (usorf_sz > 0) {
                    count += usorf_sz;
                    size -= usorf_sz;
                } else {
                    success = false;
                }
            } else if (timer.read_ms() < SOCKET_TIMEOUT) {
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
    }
    timer.stop();

    socket->pending_bytes = 0;
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

nsapi_error_t UBLOX_AT_CellularStack::socket_close_impl(int sock_id)
{
    return _at.at_cmd_discard("+USOCL", "=", "%d", sock_id);
}

// Find or create a socket from the list.
UBLOX_AT_CellularStack::CellularSocket *UBLOX_AT_CellularStack::find_socket(int id)
{
    CellularSocket *socket = NULL;

    for (unsigned int x = 0; (socket == NULL) && (x < _device.get_property(AT_CellularDevice::PROPERTY_SOCKET_COUNT)); x++) {
        if (_socket) {
            if (_socket[x]->id == id) {
                socket = (_socket[x]);
            }
        }
    }

    return socket;
}


// Clear out the storage for a socket
void UBLOX_AT_CellularStack::clear_socket(CellularSocket *socket)
{
    if (socket != NULL) {
        socket->id = SOCKET_UNUSED;
        socket->started = false;
        socket->pending_bytes = 0;
        socket->closed = true;
        if (socket->_cb) {
            socket->_cb(socket->_data);
        }
    }
}

#ifndef UBX_MDM_SARA_R41XM
nsapi_error_t UBLOX_AT_CellularStack::get_ip_address(SocketAddress *address)
{
    if (!address) {
        return NSAPI_ERROR_PARAMETER;
    }
    _at.lock();

    bool ipv4 = false, ipv6 = false;

    _at.cmd_start_stop("+UPSND", "=", "%d%d", PROFILE, 0);
    _at.resp_start("+UPSND:");

    if (_at.info_resp()) {
        _at.skip_param(2);

        if (_at.read_string(_ip, PDP_IPV6_SIZE) != -1) {
            convert_ipv6(_ip);
            address->set_ip_address(_ip);

            ipv4 = (address->get_ip_version() == NSAPI_IPv4);
            ipv6 = (address->get_ip_version() == NSAPI_IPv6);

            // Try to look for second address ONLY if modem has support for dual stack(can handle both IPv4 and IPv6 simultaneously).
            // Otherwise assumption is that second address is not reliable, even if network provides one.
            if ((_device.get_property(AT_CellularDevice::PROPERTY_IPV4V6_PDP_TYPE) && (_at.read_string(_ip, PDP_IPV6_SIZE) != -1))) {
                convert_ipv6(_ip);
                address->set_ip_address(_ip);
                ipv6 = (address->get_ip_version() == NSAPI_IPv6);
            }
        }
    }
    _at.resp_stop();
    _at.unlock();

    if (ipv4 && ipv6) {
        _stack_type = IPV4V6_STACK;
    } else if (ipv4) {
        _stack_type = IPV4_STACK;
    } else if (ipv6) {
        _stack_type = IPV6_STACK;
    }

    return (ipv4 || ipv6) ? NSAPI_ERROR_OK : NSAPI_ERROR_NO_ADDRESS;
}
#endif

nsapi_error_t UBLOX_AT_CellularStack::gethostbyname(const char *host, SocketAddress *address, nsapi_version_t version, const char *interface_name)
{
    char ipAddress[NSAPI_IP_SIZE];
    nsapi_error_t err = NSAPI_ERROR_DNS_FAILURE;

    _at.lock();
    if (address->set_ip_address(host)) {
        err = NSAPI_ERROR_OK;
    } else {
#ifdef UBX_MDM_SARA_R41XM
        _at.set_at_timeout(70000);
#else
        _at.set_at_timeout(120000);
#endif
        // This interrogation can sometimes take longer than the usual 8 seconds
        _at.cmd_start_stop("+UDNSRN", "=0,", "%s", host);

        _at.resp_start("+UDNSRN:");
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
