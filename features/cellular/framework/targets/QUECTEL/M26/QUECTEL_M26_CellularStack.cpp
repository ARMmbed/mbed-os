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

#include "QUECTEL/M26/QUECTEL_M26_CellularStack.h"
#include "CellularLog.h"

using namespace mbed;

QUECTEL_M26_CellularStack::QUECTEL_M26_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type) : AT_CellularStack(atHandler, cid, stack_type)
{
    _at.set_urc_handler("+QIRDI:", mbed::Callback<void()>(this, &QUECTEL_M26_CellularStack::urc_qiurc));
}

QUECTEL_M26_CellularStack::~QUECTEL_M26_CellularStack()
{
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
    _at.cmd_start("AT+QIFGCNT=");
    _at.write_int(0); /* Set the Context 0 as the foreground context.*/
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }
#if 0
    /*AT+QICSGP=1*/
    _at.cmd_start("AT+QICSGP=");
    _at.write_int(1); /* mode: 0-CSD, 1-GPRS */
    _at.write_string(_apn);
    if (_pwd && _uname) {
        _at.write_string(_uname);
        _at.write_string(_pwd);
    }
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();
#endif
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QIMODE=0 Set transparent mode*/
    _at.cmd_start("AT+QIMODE?");
    _at.cmd_stop();
    _at.resp_start("+QIMODE:");
    if (_at.info_resp()) {
        tcpip_mode = _at.read_int();
    }
    _at.resp_stop();
    if (tcpip_mode) {
        _at.cmd_start("AT+QIMOD=");
        _at.write_int(0); /* 0-Disable, 1-Enable */
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QIMUX=1*/
    _at.cmd_start("AT+QIMUX?");
    _at.cmd_stop();
    _at.resp_start("+QIMUX:");
    if (_at.info_resp()) {
        mux_mode = _at.read_int();
    }
    _at.resp_stop();
    if (!mux_mode) {
        _at.cmd_start("AT+QIMUX=");
        _at.write_int(1); /* 0-Disable, 1-Enable */
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }
    ret_val = _at.get_last_error();
    if (ret_val != NSAPI_ERROR_OK) {
        _at.unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    /*AT+QINDI=2*/
    _at.cmd_start("AT+QINDI?");
    _at.cmd_stop();
    _at.resp_start();
    if (_at.info_resp()) {
        cache_mode = _at.read_int();
    }
    _at.resp_stop();
    if (cache_mode != 2) {
        _at.cmd_start("AT+QINDI=");
        _at.write_int(2); /* 0-Disable, 1-Single Cache, 2-Multi Cache */
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
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

    _at.cmd_start("AT+QICLOSE=");
    _at.write_int(sock_id);
    _at.cmd_stop();
    _at.resp_start();
    _at.resp_stop();

    return _at.get_last_error();
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
    int request_connect_id = socket->id;
    int err = -1;

    _at.lock();
    if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+QIOPEN=");
        _at.write_int(request_connect_id);
        _at.write_string("TCP");
        _at.write_string(address.get_ip_address());
        _at.write_int(address.get_port());
        _at.cmd_stop();

        handle_open_socket_response(modem_connect_id, err);

        if ((_at.get_last_error() == NSAPI_ERROR_OK) && err) {
            _at.cmd_start("AT+QICLOSE=");
            _at.write_int(modem_connect_id);
            _at.cmd_stop();
            _at.resp_start();
            _at.resp_stop();

            _at.cmd_start("AT+QIOPEN=");
            _at.write_int(request_connect_id);
            _at.write_string("TCP");
            _at.write_string(address.get_ip_address());
            _at.write_int(address.get_port());
            _at.cmd_stop();

            handle_open_socket_response(modem_connect_id, err);
        }
    }

    // If opened successfully BUT not requested one, close it
    if (!err && (modem_connect_id != request_connect_id)) {
        _at.cmd_start("AT+QICLOSE=");
        _at.write_int(modem_connect_id);
        _at.cmd_stop();
        _at.resp_start();
        _at.resp_stop();
    }

    nsapi_error_t ret_val = _at.get_last_error();
    _at.unlock();

    if ((ret_val == NSAPI_ERROR_OK) && (modem_connect_id == request_connect_id)) {
        socket->created = true;
        socket->remoteAddress = address;
        socket->connected = true;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t QUECTEL_M26_CellularStack::create_socket_impl(CellularSocket *socket)
{
    int request_connect_id = socket->id;
    int modem_connect_id = request_connect_id;
    int err = -1;
    nsapi_error_t ret_val;
    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d,%d]", __FUNCTION__, __LINE__, socket->proto, socket->connected);

    if (socket->connected) {
        _at.cmd_start("AT+QIOPEN=");
        _at.write_int(request_connect_id);
        _at.write_string((socket->proto == NSAPI_TCP) ? "TCP" : "UDP");
        _at.write_string(socket->remoteAddress.get_ip_address());
        _at.write_int(socket->remoteAddress.get_port());
        _at.cmd_stop();

        handle_open_socket_response(modem_connect_id, err);

        /* Close and retry if socket create fail */
        if ((_at.get_last_error() != NSAPI_ERROR_OK) || err) {
            _at.cmd_start("AT+QICLOSE=");
            _at.write_int(modem_connect_id);
            _at.cmd_stop();
            _at.resp_start();
            _at.resp_stop();

            _at.cmd_start("AT+QIOPEN=");
            _at.write_int(request_connect_id);
            _at.write_string((socket->proto == NSAPI_TCP) ? "TCP" : "UDP");
            _at.write_string(socket->remoteAddress.get_ip_address());
            _at.write_int(socket->remoteAddress.get_port());
            _at.cmd_stop();

            handle_open_socket_response(modem_connect_id, err);
        }

        /* If opened successfully BUT not requested one, close it */
        if (!err && (modem_connect_id != request_connect_id)) {
            _at.cmd_start("AT+QICLOSE=");
            _at.write_int(modem_connect_id);
            _at.cmd_stop();
            _at.resp_start();
            _at.resp_stop();
        }

        ret_val = _at.get_last_error();
        socket->created = ((ret_val == NSAPI_ERROR_OK) && (modem_connect_id == request_connect_id));
        return ret_val;
    } else {
        tr_warn("QUECTEL_M26_CellularStack:%s:%u: Do not support TCP Listner/UDP Service Mode [%d,%d]", __FUNCTION__, __LINE__, socket->created, ret_val);
        ret_val = NSAPI_ERROR_OK;
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u: END [%d,%d]", __FUNCTION__, __LINE__, socket->created, ret_val);
    return ret_val;
}


nsapi_size_or_error_t QUECTEL_M26_CellularStack::socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
                                                                    const void *data, nsapi_size_t size)
{
    int sent_len = (size > M26_SENT_BYTE_MAX) ? M26_SENT_BYTE_MAX : size;
    int sent_nacked = 0;
    int sent_len_before = 0;
    int sent_len_after = 0;
    int sent_acked;
    nsapi_error_t error;

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d-%d]", __FUNCTION__, __LINE__, sent_len, size);

    if (sent_len == 0) {
        tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_PARAMETER]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_PARAMETER;
    }

    if (!socket->created) {
        socket->remoteAddress = address;
        socket->connected = true;
        nsapi_error_t ret_val = create_socket_impl(socket);
        if ((ret_val != NSAPI_ERROR_OK) || (!socket->created)) {
            tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_NO_SOCKET]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_NO_SOCKET;
        }
    }

    if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+QISACK=");
        _at.write_int(socket->id);
        _at.cmd_stop();
        _at.resp_start("+QISACK:");
        sent_len_before = _at.read_int();
        sent_acked = _at.read_int();
        sent_nacked = _at.read_int();
        _at.resp_stop();

        if (_at.get_last_error() != NSAPI_ERROR_OK) {
            tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_DEVICE_ERROR]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        if (sent_nacked != 0) {
            tr_debug("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_WOULD_BLOCK]", __FUNCTION__, __LINE__);
            return NSAPI_ERROR_WOULD_BLOCK;
        }
    }

    _at.cmd_start("AT+QISEND=");
    _at.write_int(socket->id);
    _at.write_int(sent_len);
    _at.cmd_stop();

    _at.resp_start(">");
    _at.write_bytes((uint8_t *)data, sent_len);
    _at.resp_start();
    _at.resp_stop();

    if (_at.get_last_error() != NSAPI_ERROR_OK) {
        tr_error("QUECTEL_M26_CellularStack:%s:%u:[NSAPI_ERROR_DEVICE_ERROR]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (socket->proto == NSAPI_TCP) {
        _at.cmd_start("AT+QISACK=");
        _at.write_int(socket->id);
        _at.cmd_stop();

        _at.resp_start("+QISACK:");
        sent_len_after = _at.read_int();
        sent_acked = _at.read_int();
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
    nsapi_size_or_error_t recv_len = (size > M26_RECV_BYTE_MAX) ? M26_RECV_BYTE_MAX : size;
    int recv_len_after = 0;
    int port;
    char type[8];
    char ip_address[NSAPI_IP_SIZE + 1];

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d]", __FUNCTION__, __LINE__, size);
    _at.cmd_start("AT+QIRD=");
    _at.write_int(0); /* at+qifgcnt 0-1 */
    _at.write_int(1); /* 1-Client, 2-Server */
    _at.write_int(socket->id);
    _at.write_int(recv_len);
    _at.cmd_stop();

    _at.resp_start("+QIRD:");
    if (_at.info_resp()) {
        _at.set_delimiter(':');
        _at.read_string(ip_address, sizeof(ip_address));
        _at.set_default_delimiter();
        port = _at.read_int();
        _at.read_string(type, sizeof(type));
        recv_len_after = _at.read_int();
        if (recv_len_after > 0) {
            _at.read_bytes((uint8_t *)buffer, recv_len_after);
        }
    }
    _at.resp_stop();

    if (!recv_len_after || (_at.get_last_error() != NSAPI_ERROR_OK)) {
        tr_debug("QUECTEL_M26_CellularStack:%s:%u:[ERROR NSAPI_ERROR_WOULD_BLOCK]", __FUNCTION__, __LINE__);
        return NSAPI_ERROR_WOULD_BLOCK;
    }

    if (address) {
        address->set_ip_address(ip_address);
        address->set_port(port);
    }

    tr_debug("QUECTEL_M26_CellularStack:%s:%u:[%d]", __FUNCTION__, __LINE__, recv_len_after);
    return recv_len_after;
}
