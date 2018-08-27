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

#include <cstdlib>
#include "GEMALTO_CINTERION_CellularStack.h"
#include "GEMALTO_CINTERION_Module.h"
#include "CellularLog.h"

// defines as per ELS61-E2_ATC_V01.000 and BGS2-W_ATC_V00.100
#define SOCKET_MAX 10
#define UDP_PACKET_SIZE 1460
#define FAILURE_TIMEOUT (30*1000) // failure timeout in milliseconds on modem side

/*
 * Use connection profile 0 and Internet service profiles starting from 0 for sockets.
 */
#define CONNECTION_PROFILE_ID 0

using namespace mbed;

GEMALTO_CINTERION_CellularStack::GEMALTO_CINTERION_CellularStack(ATHandler &atHandler, const char *apn,
                                                                 int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type), _apn(apn)
{
}

GEMALTO_CINTERION_CellularStack::~GEMALTO_CINTERION_CellularStack()
{
}

GEMALTO_CINTERION_CellularStack::CellularSocket *GEMALTO_CINTERION_CellularStack::find_socket(int sock_id)
{
    CellularSocket *sock = NULL;
    for (int i = 0; i < SOCKET_MAX; i++) {
        if (_socket[i] && _socket[i]->id == sock_id) {
            sock = _socket[i];
            break;
        }
    }
    if (!sock) {
        tr_error("Socket not found %d", sock_id);
    }
    return sock;
}

void GEMALTO_CINTERION_CellularStack::urc_sis()
{
    int sock_id = _at.read_int();
    int urc_code = _at.read_int();
    CellularSocket *sock = find_socket(sock_id);
    if (sock) {
        if (urc_code == 5) { // data available
            if (sock->_cb) {
                sock->started = true;
                sock->tx_ready = true;
                sock->_cb(sock->_data);
            }
        } else if (urc_code == 2) { // socket closed
            sock->created = false;
        }
    }
}

void GEMALTO_CINTERION_CellularStack::urc_sisw()
{
    int sock_id = _at.read_int();
    int urc_code = _at.read_int();
    CellularSocket *sock = find_socket(sock_id);
    if (sock) {
        if (urc_code == 1) { // ready
            if (sock->_cb) {
                sock->tx_ready = true;
                if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelBGS2) {
                    sock->started = true;
                }
                sock->_cb(sock->_data);
            }
        } else if (urc_code == 2) { // socket closed
            sock->created = false;
        }
    }
}

void GEMALTO_CINTERION_CellularStack::urc_sisr()
{
    int sock_id = _at.read_int();
    int urc_code = _at.read_int();
    CellularSocket *sock = find_socket(sock_id);
    if (sock) {
        if (urc_code == 1) { // data available
            if (sock->_cb) {
                sock->rx_avail = true;
                sock->_cb(sock->_data);
            }
        } else if (urc_code == 2) { // socket closed
            sock->created = false;
        }
    }
}

nsapi_error_t GEMALTO_CINTERION_CellularStack::socket_stack_init()
{
    _at.lock();
    if (create_connection_profile()) {
        _at.set_urc_handler("^SIS:", mbed::Callback<void()>(this, &GEMALTO_CINTERION_CellularStack::urc_sis));
        _at.set_urc_handler("^SISW:", mbed::Callback<void()>(this, &GEMALTO_CINTERION_CellularStack::urc_sisw));
        _at.set_urc_handler("^SISR:", mbed::Callback<void()>(this, &GEMALTO_CINTERION_CellularStack::urc_sisr));
    }
    return _at.unlock_return_error();
}

int GEMALTO_CINTERION_CellularStack::get_max_socket_count()
{
    return SOCKET_MAX;
}

bool GEMALTO_CINTERION_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP);
}

nsapi_error_t GEMALTO_CINTERION_CellularStack::socket_close_impl(int sock_id)
{
    CellularSocket *socket = find_socket(sock_id);
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    _at.set_at_timeout(FAILURE_TIMEOUT);
    _at.cmd_start("AT^SISC=");
    _at.write_int(sock_id);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    _at.restore_at_timeout();

    socket->started = false;
    socket->created = false;
    socket->tx_ready = false;
    socket->rx_avail = false;

    tr_debug("Closed socket %d (err %d)", sock_id, _at.get_last_error());
    return _at.get_last_error();
}

nsapi_error_t GEMALTO_CINTERION_CellularStack::socket_open_defer(CellularSocket *socket, const SocketAddress *address)
{
    // host address (IPv4) and local+remote port is needed only for BGS2 which does not support UDP server socket
    char sock_addr[sizeof("sockudp://") - 1 + NSAPI_IPv4_SIZE + sizeof(":12345;port=12345") - 1 + 1];
    if (GEMALTO_CINTERION_Module::get_model() != GEMALTO_CINTERION_Module::ModelBGS2) {
        std::sprintf(sock_addr, "sockudp://%s:%u", address ? address->get_ip_address() : "", socket->localAddress.get_port());
    } else {
        std::sprintf(sock_addr, "sockudp://%s:%u;port=%u", address->get_ip_address(), address->get_port(), socket->localAddress.get_port());
    }

    _at.cmd_start("AT^SISS=");
    _at.write_int(socket->id);
    _at.write_string("address", false);
    _at.write_string(sock_addr);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    _at.cmd_start("AT^SISO=");
    _at.write_int(socket->id);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
    if (_at.get_last_error()) {
        tr_error("Socket %d open failed!", socket->id);
        _at.clear_error();
        socket_close_impl(socket->id); // socket may already be open on modem if app and modem are not in sync, as a recovery, try to close the socket so open succeeds the next time
        return NSAPI_ERROR_NO_SOCKET;
    }

    socket->created = true;
    tr_debug("Socket %d created (err %d)", socket->id, _at.get_last_error());

    return _at.get_last_error();
}

// To open socket:
// 1. Select URC mode or polling mode with AT^SCFG
// 2. create a GPRS connection profile with AT^SICS (must have PDP)
// 3. create service profile with AT^SISS and map connectionID to serviceID
// 4. open internet session with AT^SISO (ELS61 tries to attach to a packet domain)
nsapi_error_t GEMALTO_CINTERION_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int connection_profile_id = CONNECTION_PROFILE_ID;

    // setup internet session profile
    int internet_service_id = socket->id;
    bool foundSrvType = false;
    bool foundConIdType = false;
    _at.cmd_start("AT^SISS?");
    _at.cmd_stop();
    _at.resp_start("^SISS:");
    /*
     * Profile is a list of tag-value map:
     * ^SISS: <srvProfileId>, <srvParmTag>, <srvParmValue>
     * [^SISS: ...]
     */
    while (_at.info_resp()) {
        int id = _at.read_int();
        if (id == internet_service_id) {
            char paramTag[16];
            int paramTagLen = _at.read_string(paramTag, sizeof(paramTag));
            if (paramTagLen > 0) {
                char paramValue[100 + 1]; // APN may be up to 100 chars
                int paramValueLen = _at.read_string(paramValue, sizeof(paramValue));
                if (paramValueLen >= 0) {
                    if (strcmp(paramTag, "srvType") == 0) {
                        if (strcmp(paramValue, "Socket") == 0) {
                            tr_debug("srvType %s", paramValue);
                            foundSrvType = true;
                        }
                    }
                    if (strcmp(paramTag, "address") == 0) {
                        if (strncmp(paramValue, "sock", sizeof("sock")) == 0) {
                            tr_debug("address %s", paramValue);
                            foundSrvType = true;
                        }
                    }
                    if (strcmp(paramTag, "conId") == 0) {
                        char buf[10];
                        std::sprintf(buf, "%d", connection_profile_id);
                        tr_debug("conId %s", paramValue);
                        if (strcmp(paramValue, buf) == 0) {
                            foundConIdType = true;
                        }
                    }
                }
            }
        }
    }
    _at.resp_stop();

    if (!foundSrvType) {
        _at.cmd_start("AT^SISS=");
        _at.write_int(internet_service_id);
        _at.write_string("srvType");
        _at.write_string("Socket");
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    if (!foundConIdType) {
        _at.cmd_start("AT^SISS=");
        _at.write_int(internet_service_id);
        _at.write_string("conId");
        _at.write_int(connection_profile_id);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    tr_debug("Internet service %d created (err %d)", internet_service_id, _at.get_last_error());

    if (GEMALTO_CINTERION_Module::get_model() != GEMALTO_CINTERION_Module::ModelBGS2) {
        return socket_open_defer(socket);
    }
    return _at.get_last_error();
}

nsapi_size_or_error_t GEMALTO_CINTERION_CellularStack::socket_sendto_impl(CellularSocket *socket,
                                                                          const SocketAddress &address, const void *data, nsapi_size_t size)
{
    tr_debug("Socket %d, sendto %s, len %d", socket->id, address.get_ip_address(), size);

    int ip_version = address.get_ip_version();
    if ((ip_version == NSAPI_IPv4 && _stack_type != IPV4_STACK) ||
            (ip_version == NSAPI_IPv6 && _stack_type != IPV6_STACK)) {
        tr_warn("No IP route for %s", address.get_ip_address());
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelBGS2) {
        tr_error("Send addr %s, prev addr %s", address.get_ip_address(), socket->remoteAddress.get_ip_address());
        if (address != socket->remoteAddress) {
            if (socket->started) {
                socket_close_impl(socket->id);
                _at.clear_error();
            }

            if (socket_open_defer(socket, &address) != NSAPI_ERROR_OK) {
                tr_error("Failed to open socket %d", socket->id);
                return NSAPI_ERROR_NO_SOCKET;
            }
            socket->remoteAddress = address;
            _at.resp_start("^SISW:");
            int sock_id = _at.read_int();
            int urc_code = _at.read_int();
            tr_debug("TX ready: socket=%d, urc=%d (err=%d)", sock_id, urc_code, _at.get_last_error());
            (void)sock_id;
            (void)urc_code;
            socket->created = true;
            socket->started = true;
            socket->tx_ready = true;
        }
    }
    if (!socket->started || !socket->tx_ready) {
        tr_debug("Socket %d would block (started %d, tx %d)", socket->id, socket->started, socket->tx_ready);
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (size > UDP_PACKET_SIZE) {
        tr_warn("Sending UDP packet size %d (max %d)", size, UDP_PACKET_SIZE);
        size = UDP_PACKET_SIZE;
    }

    _at.set_at_timeout(FAILURE_TIMEOUT);
    _at.cmd_start("AT^SISW=");
    _at.write_int(socket->id);
    _at.write_int(size);
    if (GEMALTO_CINTERION_Module::get_model() != GEMALTO_CINTERION_Module::ModelBGS2) {
        _at.write_int(0);
        char socket_address[NSAPI_IPv6_SIZE + sizeof("[]:12345") - 1 + 1];
        if (address.get_ip_version() == NSAPI_IPv4) {
            std::sprintf(socket_address, "%s:%u", address.get_ip_address(), address.get_port());
        } else {
            std::sprintf(socket_address, "[%s]:%u", address.get_ip_address(), address.get_port());
        }
        _at.write_string(socket_address);
    }
    _at.cmd_stop();

sisw_retry:
    _at.resp_start("^SISW:");
    if (!_at.info_resp()) {
        tr_error("Socket %d send failure", socket->id);
        _at.restore_at_timeout();
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    _at.restore_at_timeout();
    int socket_id = _at.read_int();
    if (socket_id != socket->id) {
        tr_error("Socket id %d != %d", socket_id, socket->id);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    int accept_len = _at.read_int();
    if (accept_len == -1) {
        tr_error("Socket %d send failed", socket->id);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    int unack_len = _at.read_int();
    if (unack_len != 0) {
        tr_warn("Socket %d unack_len %d", socket->id, unack_len);
        if (GEMALTO_CINTERION_Module::get_model() != GEMALTO_CINTERION_Module::ModelBGS2) {
            // assume that an URC was received when unackData is not received
            _at.resp_stop();
            goto sisw_retry;
        }
    }

    _at.write_bytes((uint8_t *)data, accept_len);
    _at.resp_stop();

    tr_debug("Socket %d sendto %s, %d bytes (err %d)", socket->id, address.get_ip_address(), accept_len, _at.get_last_error());

    if (_at.get_last_error() == NSAPI_ERROR_OK) {
        socket->tx_ready = false;
    }

    return (_at.get_last_error() == NSAPI_ERROR_OK) ? accept_len : NSAPI_ERROR_DEVICE_ERROR;
}

nsapi_size_or_error_t GEMALTO_CINTERION_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                            void *buffer, nsapi_size_t size)
{
    tr_debug("Socket %d recvfrom %d bytes", socket->id, size);

    if (size > UDP_PACKET_SIZE) {
        tr_debug("Socket recvfrom size %d > %d", size, UDP_PACKET_SIZE);
        size = UDP_PACKET_SIZE;
    }

    if (!socket->rx_avail) {
        _at.process_oob(); // check for ^SISR URC
        if (!socket->rx_avail) {
            tr_debug("Socket %d would block", socket->id);
            return NSAPI_ERROR_WOULD_BLOCK;
        }
    }

    _at.cmd_start("AT^SISR=");
    _at.write_int(socket->id);
    _at.write_int(size);
    _at.cmd_stop();

    _at.resp_start("^SISR:");
    if (!_at.info_resp()) {
        tr_error("Socket %d not responding", socket->id);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    int socket_id = _at.read_int();
    if (socket_id != socket->id) {
        tr_error("Socket recvfrom id %d != %d", socket_id, socket->id);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    nsapi_size_or_error_t len = _at.read_int();
    if (len == 0) {
        tr_warn("Socket %d no data", socket->id);
        _at.resp_stop();
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    if (len == -1) {
        tr_error("Socket %d recvfrom failed!", socket->id);
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    socket->rx_avail = false;
    if (len >= (nsapi_size_or_error_t)size) {
        int remain_len = _at.read_int();
        if (remain_len > 0) {
            socket->rx_avail = true;
        }
    }
    if (GEMALTO_CINTERION_Module::get_model() != GEMALTO_CINTERION_Module::ModelBGS2) {
        char ip_address[NSAPI_IPv6_SIZE + sizeof("[]:12345") - 1 + 1];
        int ip_len = _at.read_string(ip_address, sizeof(ip_address));
        if (ip_len <= 0) {
            tr_error("Socket %d recvfrom addr!", socket->id);
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (address) {
            char *ip_start = ip_address;
            char *ip_stop;
            char *port_start;
            if (_stack_type == IPV6_STACK) {
                ip_start++; // skip '['
                ip_stop = strchr(ip_address, ']');
                if (ip_stop) {
                    port_start = strchr(ip_stop, ':');
                }
            } else {
                ip_stop = strchr(ip_address, ':');
                port_start = ip_stop;
            }
            if (ip_stop && port_start) {
                char tmp_ch = *ip_stop;
                *ip_stop = '\0'; // split IP and port
                address->set_ip_address(ip_start);
                port_start++; // skip ':'
                int port = std::strtol(port_start, NULL, 10);
                address->set_port(port);
                tr_debug("IP address %s:%d", address->get_ip_address(), address->get_port());
                *ip_stop = tmp_ch; // restore original IP string
            }
        }
    } else {
        if (address) {
            *address = socket->remoteAddress;
        }
    }

    nsapi_size_or_error_t recv_len = _at.read_bytes((uint8_t *)buffer, len);

    _at.resp_stop();

    tr_debug("Socket %d, recvfrom %s, %d bytes (err %d)", socket->id, address, len, _at.get_last_error());

    return (_at.get_last_error() == NSAPI_ERROR_OK) ? recv_len : NSAPI_ERROR_DEVICE_ERROR;
}

// setup internet connection profile for sockets
bool GEMALTO_CINTERION_CellularStack::create_connection_profile()
{
    if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelEMS31) {
        // EMS31 connection has only DNS settings and there is no need to modify those here for now
        return true;
    }

    char conParamType[12];
    std::sprintf(conParamType, "GPRS%d", (_stack_type == IPV4_STACK) ? 0 : 6);
    _at.cmd_start("AT^SICS?");
    _at.cmd_stop();
    bool foundConnection = false;
    bool foundAPN = false;
    int connection_profile_id = CONNECTION_PROFILE_ID;
    _at.resp_start("^SICS:");
    while (_at.info_resp()) {
        int id = _at.read_int();
        tr_debug("SICS %d", id);
        if (id == connection_profile_id) {
            char paramTag[16];
            int paramTagLen = _at.read_string(paramTag, sizeof(paramTag));
            if (paramTagLen > 0) {
                tr_debug("paramTag %s", paramTag);
                char paramValue[100 + 1]; // APN may be up to 100 chars
                int paramValueLen = _at.read_string(paramValue, sizeof(paramValue));
                if (paramValueLen >= 0) {
                    tr_debug("paramValue %s", paramValue);
                    if (strcmp(paramTag, "conType") == 0) {
                        tr_debug("conType %s", paramValue);
                        if (strcmp(paramValue, conParamType) == 0) {
                            foundConnection = true;
                        }
                    }
                    if (strcmp(paramTag, "apn") == 0) {
                        tr_debug("apn %s", paramValue);
                        if (strcmp(paramValue, _apn ? _apn : "") == 0) {
                            foundAPN = true;
                        }
                    }
                }
            }
        }
    }
    _at.resp_stop();

    if (!foundConnection) {
        tr_debug("Socket conType %s", conParamType);
        _at.cmd_start("AT^SICS=");
        _at.write_int(connection_profile_id);
        _at.write_string("conType");
        _at.write_string(conParamType);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    if (!foundAPN && _apn) {
        tr_debug("Socket APN %s", _apn ? _apn : "");
        _at.cmd_start("AT^SICS=");
        _at.write_int(connection_profile_id);
        _at.write_string("apn");
        _at.write_string(_apn);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    // use URC mode
    _at.cmd_start("AT^SCFG=\"Tcp/withURCs\",\"on\"");
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    tr_debug("Connection profile %d created, stack_type %d (err %d)", connection_profile_id, _stack_type, _at.get_last_error());
    return _at.get_last_error() == NSAPI_ERROR_OK;
}
