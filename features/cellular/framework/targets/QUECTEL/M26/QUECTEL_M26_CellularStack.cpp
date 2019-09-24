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

#include "rtos/Kernel.h"
#include "QUECTEL/M26/QUECTEL_M26_CellularStack.h"
#include "CellularLog.h"

#define SOCKET_SEND_READY_TIMEOUT (30*1000)
#define SOCKET_READ_TIMEOUT 1000

using namespace mbed;

QUECTEL_M26_CellularStack::QUECTEL_M26_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    _at.set_urc_handler("+QIRDI:", Callback<void()>(this, &QUECTEL_M26_CellularStack::urc_qiurc));

    _at.set_urc_handler("0, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_0));
    _at.set_urc_handler("1, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_1));
    _at.set_urc_handler("2, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_2));
    _at.set_urc_handler("3, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_3));
    _at.set_urc_handler("4, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_4));
    _at.set_urc_handler("5, CLOSED", Callback<void()>(this, &QUECTEL_M26_CellularStack::socket_closed_5));
}

QUECTEL_M26_CellularStack::~QUECTEL_M26_CellularStack()
{
    _at.set_urc_handler("5, CLOSED", NULL);
    _at.set_urc_handler("4, CLOSED", NULL);
    _at.set_urc_handler("3, CLOSED", NULL);
    _at.set_urc_handler("2, CLOSED", NULL);
    _at.set_urc_handler("1, CLOSED", NULL);
    _at.set_urc_handler("0, CLOSED", NULL);

    _at.set_urc_handler("+QIRDI:", NULL);
}

nsapi_error_t QUECTEL_M26_CellularStack::socket_listen(nsapi_socket_t handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_M26_CellularStack::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QUECTEL_M26_CellularStack::socket_bind(nsapi_socket_t handle, const SocketAddress &addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

void QUECTEL_M26_CellularStack::socket_closed(int sock_id)
{
    CellularSocket *sock = find_socket(sock_id);
    if (sock) {
        tr_info("Socket closed %d", sock_id);
        sock->closed = true;
    }
}

void QUECTEL_M26_CellularStack::socket_closed_0()
{
    socket_closed(0);
}

void QUECTEL_M26_CellularStack::socket_closed_1()
{
    socket_closed(1);
}

void QUECTEL_M26_CellularStack::socket_closed_2()
{
    socket_closed(2);
}

void QUECTEL_M26_CellularStack::socket_closed_3()
{
    socket_closed(3);
}

void QUECTEL_M26_CellularStack::socket_closed_4()
{
    socket_closed(4);
}

void QUECTEL_M26_CellularStack::socket_closed_5()
{
    socket_closed(5);
}

void QUECTEL_M26_CellularStack::urc_qiurc()
{
    int sock_id = 0;

    _at.lock();
    (void) _at.skip_param(); /*<id> AT+QIFGCNT*/
    (void) _at.skip_param(); /*<sc> 1 Client, 2 Server*/
    sock_id = _at.read_int();
    (void) _at.skip_param(); /*<num>*/
    (void) _at.skip_param(); /*<len>*/
    (void) _at.skip_param(); /*<tlen>*/
    _at.unlock();

    for (int i = 0; i < get_max_socket_count(); i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock->id == sock_id) {
            if (sock->_cb) {
                sock->_cb(sock->_data);
            }
            break;
        }
    }
}

nsapi_error_t QUECTEL_M26_CellularStack::socket_stack_init()
{
    int tcpip_mode = 1;
    int mux_mode = 0;
    int cache_mode = 0;
    nsapi_error_t ret_val;

    tr_debug("QUECTEL_M26_CellularStack:%s:%u: START ", __FUNCTION__, __LINE__);
    _at.lock();

    /*AT+QIFGCNT=0*/
    _at.at_cmd_discard("+QIFGCNT", "=", "%d", 0);

    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }
#if 0
    if (_pwd && _uname) {
        _at.at_cmd_discard("+QICSGP", "=", "%d%s%s%s", 1, _apn, _uname, _pwd);
    } else {
        _at.at_cmd_discard("+QICSGP", "=", "%d%s", 1, _apn);
    }

#endif
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QIMODE=0 Set transparent mode*/
    _at.cmd_start_stop("+QIMODE", "?");

    _at.resp_start("+QIMODE:");
    if (_at.info_resp()) {
        tcpip_mode = _at.read_int();
    }
    _at.resp_stop();
    if (tcpip_mode) {
        _at.at_cmd_discard("+QIMODE", "=", "%d", 0);
    }
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QIMUX=1*/
    _at.cmd_start_stop("+QIMUX", "?");

    _at.resp_start("+QIMUX:");
    if (_at.info_resp()) {
        mux_mode = _at.read_int();
    }
    _at.resp_stop();
    if (!mux_mode) {
        _at.at_cmd_discard("+QIMUX", "=", "%d", 1);
    }
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QINDI=2*/
    _at.cmd_start_stop("+QINDI", "?");

    _at.resp_start();
    if (_at.info_resp()) {
        cache_mode = _at.read_int();
    }
    _at.resp_stop();
    if (cache_mode != 2) {
        _at.at_cmd_discard("+QINDI", "=", "%d", 2);
    }

    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    _at.unlock();

    tr_debug("QUECTEL_M26_CellularStack:%s:%u: SUCCESS ", __FUNCTION__, __LINE__);
    return NSAPI_ERROR_OK;
}

int QUECTEL_M26_CellularStack::get_max_socket_count()
{
    return M26_SOCKET_MAX;
}

bool QUECTEL_M26_CellularStack::is_protocol_supported(nsapi_protocol_t protocol)
{
    return (protocol == NSAPI_UDP || protocol == NSAPI_TCP);
}

nsapi_error_t QUECTEL_M26_CellularStack::socket_close_impl(int sock_id)
{
    tr_debug("QUECTEL_M26_CellularStack:%s:%u:", __FUNCTION__, __LINE__);

    return _at.at_cmd_discard("+QICLOSE", "=", "%d", sock_id);
}

void QUECTEL_M26_CellularStack::handle_open_socket_response(int &modem_connect_id, int &err)
{
    char status[15];
    tr_debug("QUECTEL_M26_CellularStack:%s:%u: START", __FUNCTION__, __LINE__);

    _at.resp_start("ALREADY CONNECT");
    if (_at.info_resp()) {
        /* ALREADY CONNECT: The request socket already connected */
        err = 0;
        return;
    }
    _at.resp_stop();
    if (_at.get_last_error() != NSAPI_ERROR_OK) {
        /* ERROR: The command format error */
        err = 1;
        return;
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u: OK", __FUNCTION__, __LINE__);

    _at.set_at_timeout(M26_CREATE_SOCKET_TIMEOUT);
    _at.resp_start();
    _at.set_stop_tag("\r\n");
    modem_connect_id = _at.read_int();
    _at.read_string(status, sizeof(status), true);
    _at.resp_stop();
    _at.restore_at_timeout();

    if ((!strcmp(status, "CONNECT FAIL")) || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        err = 1;
        return;
    }

    err = 0;
    tr_debug("QUECTEL_M26_CellularStack:%s:%u: END [%s, %d]", __FUNCTION__, __LINE__, status, err);
}


nsapi_error_t QUECTEL_M26_CellularStack::socket_connect(nsapi_socket_t handle, const SocketAddress &address)
{
    CellularSocket *socket = (CellularSocket *)handle;

    int modem_connect_id = -1;
    int err = -1;

    int request_connect_id = find_socket_index(socket);
    // assert here as its a programming error if the socket container doesn't contain
    // specified handle
    MBED_ASSERT(request_connect_id != -1);

    _at.lock();
    if (socket->proto == NSAPI_TCP) {
        _at.cmd_start_stop("+QIOPEN", "=", "%d%s%s%d", request_connect_id, "TCP",
                           address.get_ip_address(), address.get_port());;

        handle_open_socket_response(modem_connect_id, err);

        if ((_at.get_last_error() == NSAPI_ERROR_OK) && err) {
            _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);

            _at.cmd_start_stop("+QIOPEN", "=", "%d%s%s%d", request_connect_id, "TCP",
                               address.get_ip_address(), address.get_port());

            handle_open_socket_response(modem_connect_id, err);
        }
    }

    // If opened successfully BUT not requested one, close it
    if (!err && (modem_connect_id != request_connect_id)) {
        _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);
    }

    nsapi_error_t ret_val = _at.get_last_error();
    _at.unlock();

    if ((ret_val == NSAPI_ERROR_OK) && (modem_connect_id == request_connect_id)) {
        socket->remoteAddress = address;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t QUECTEL_M26_CellularStack::create_socket_impl(CellularSocket *socket)
{
    // This modem is a special case. It takes in the socket ID rather than spitting
    // it out. So we will first try to use the index of the socket construct as the id
    // but if another opened socket is already opened with that id we will pick the next
    // id which is not in use
    bool duplicate = false;
    int potential_sid = -1;
    int index = find_socket_index(socket);

    for (int i = 0; i < get_max_socket_count(); i++) {
        CellularSocket *sock = _socket[i];
        if (sock && sock != socket && sock->id == index) {
            duplicate = true;
        } else if (duplicate && !sock) {
            potential_sid = i;
            break;
        }
    }

    if (duplicate) {
        index = potential_sid;
    }

    int request_connect_id = index;
    int modem_connect_id = request_connect_id;
    int err = -1;
    nsapi_error_t ret_val;
    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d,%d]", __FUNCTION__, __LINE__, socket->proto, socket->connected);

    if (socket->connected) {
        _at.cmd_start_stop("+QIOPEN", "=", "%d%s%s%d", request_connect_id, (socket->proto == NSAPI_TCP) ? "TCP" : "UDP",
                           socket->remoteAddress.get_ip_address(), socket->remoteAddress.get_port());

        handle_open_socket_response(modem_connect_id, err);

        /* Close and retry if socket create fail */
        if ((_at.get_last_error() != NSAPI_ERROR_OK) || err) {
            _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);

            _at.cmd_start_stop("+QIOPEN", "=", "%d%s%s%d", request_connect_id, (socket->proto == NSAPI_TCP) ? "TCP" : "UDP",
                               socket->remoteAddress.get_ip_address(), socket->remoteAddress.get_port());

            handle_open_socket_response(modem_connect_id, err);
        }

        /* If opened successfully BUT not requested one, close it */
        if (!err && (modem_connect_id != request_connect_id)) {
            _at.at_cmd_discard("+QICLOSE", "=", "%d", modem_connect_id);
        }

        ret_val = _at.get_last_error();
        if ((ret_val == NSAPI_ERROR_OK) && (modem_connect_id == request_connect_id)) {
            socket->id = request_connect_id;
        }
        return ret_val;
    } else {
        ret_val = NSAPI_ERROR_OK;
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u: END [%d]", __FUNCTION__, __LINE__, ret_val);
    return ret_val;
}


nsapi_size_or_error_t QUECTEL_M26_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                    const void *data, nsapi_size_t size)
{
    int sent_len = size;
    int sent_acked = 0;
    int sent_nacked = 0;
    int sent_len_before = 0;
    int sent_len_after = 0;
    nsapi_error_t error;

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d-%d]", __FUNCTION__, __LINE__, sent_len, size);

    if (sent_len == 0 || size > M26_SENT_BYTE_MAX) {
        tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_PARAMETER]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_PARAMETER;
    }

    if (socket->id == -1) {
        socket->remoteAddress = address;
        socket->connected = true;
        nsapi_error_t ret_val = create_socket_impl(socket);
        if ((ret_val != NSAPI_ERROR_OK) || (socket->id == -1)) {
            tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_NO_SOCKET]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    if (socket->proto == NSAPI_TCP) {
        bool ready_to_send = false;
        uint64_t start_time = rtos::Kernel::get_ms_count();
        while (!ready_to_send && start_time < rtos::Kernel::get_ms_count() + SOCKET_SEND_READY_TIMEOUT) {
            _at.cmd_start_stop("+QISACK", "=", "%d", socket->id);
            _at.resp_start("+QISACK:");
            sent_len_before = _at.read_int();
            sent_acked = _at.read_int();
            (void)sent_acked;
            sent_nacked = _at.read_int();
            _at.resp_stop();

            if (_at.get_last_error() != NSAPI_ERROR_OK) {
                tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_DEVICE_ERROR]", __FUNCTION__, __LINE__);
                return NSAPI_ERROR_DEVICE_ERROR;
            }

            if (sent_nacked == 0) {
                ready_to_send = true;
            } else {
                tr_debug("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_WOULD_BLOCK]", __FUNCTION__, __LINE__);
            }
        }
    }

    _at.cmd_start_stop("+QISEND", "=", "%d%d", socket->id, sent_len);

    _at.resp_start(">");
    _at.write_bytes((uint8_t *)data, sent_len);
    _at.resp_start();
    _at.set_stop_tag("\r\n");
    // Possible responses are SEND OK, SEND FAIL or ERROR.
    char response[16];
    response[0] = '\0';
    _at.read_string(response, sizeof(response));
    _at.resp_stop();
    if (strcmp(response, "SEND OK") != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (_at.get_last_error() != NSAPI_ERROR_OK) {
        tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_DEVICE_ERROR]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (socket->proto == NSAPI_TCP) {
        _at.cmd_start_stop("+QISACK", "=", "%d", socket->id);

        _at.resp_start("+QISACK:");
        sent_len_after = _at.read_int();
        sent_acked = _at.read_int();
        (void)sent_acked; // avoid compile warning, used only for debugging
        sent_nacked = _at.read_int();
        _at.resp_stop();

        error = _at.get_last_error();
        if (error == NSAPI_ERROR_OK) {
            sent_len = sent_len_after - sent_len_before;
            tr_debug("QUECTEL_M26_CellularStack:%s:%u:[TCP: BA %d-%d, ACK %d-%d,LEN %d-%d]", __FUNCTION__, __LINE__, sent_len_before, sent_len_after, sent_acked, sent_nacked, sent_len, size);
            return sent_len;
        }

        tr_error("QUECTEL_M26_CellularStack:%s:%u:[TCP: %d]", __FUNCTION__, __LINE__, error);
        return error;
    }

    error = _at.get_last_error();
    if (error == NSAPI_ERROR_OK) {
        tr_debug("QUECTEL_M26_CellularStack:%s:%u:[UDP: %d]", __FUNCTION__, __LINE__, sent_len);
        return sent_len;
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[ERROR: %d]", __FUNCTION__, __LINE__, error);
    return error;
}

nsapi_size_or_error_t QUECTEL_M26_CellularStack::socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
                                                                      void *buffer, nsapi_size_t size)
{
    int port;
    char type[8];
    char ip_address[NSAPI_IP_SIZE + 1];

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d]", __FUNCTION__, __LINE__, size);

    uint64_t start_time = rtos::Kernel::get_ms_count();
    nsapi_size_t len = 0;
    for (; len < size;) {
        unsigned int read_len = (size - len > M26_RECV_BYTE_MAX) ? M26_RECV_BYTE_MAX : size - len;

        _at.cmd_start_stop("+QIRD", "=", "%d%d%d%d", 0, 1, socket->id, read_len);

        nsapi_size_t recv_len = 0;
        _at.resp_start("+QIRD:");
        if (_at.info_resp()) {
            _at.set_delimiter(':');
            _at.read_string(ip_address, sizeof(ip_address));
            _at.set_default_delimiter();
            port = _at.read_int();
            _at.read_string(type, sizeof(type));
            recv_len = _at.read_int();
            _at.read_bytes((uint8_t *)buffer + len, recv_len);
            len += recv_len;
        }
        _at.resp_stop();

        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            tr_warn("QUECTEL_M26_CellularStack:%s:%u:[ERROR NSAPI_ERROR_OK]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        if (rtos::Kernel::get_ms_count() > start_time + SOCKET_READ_TIMEOUT) {
            tr_warn("QUECTEL_M26_CellularStack:%s:%u:[ERROR NSAPI_ERROR_TIMEOUT]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_TIMEOUT;
        }

        if (recv_len == 0 || recv_len < read_len) {
            break;
        }
    }

    if (len == 0) {
        tr_debug("QUECTEL_M26_CellularStack:%s:%u:[ERROR NSAPI_ERROR_WOULD_BLOCK]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (address) {
        address->set_ip_address(ip_address);
        address->set_port(port);
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d]", __FUNCTION__, __LINE__, len);
    return len;
}
