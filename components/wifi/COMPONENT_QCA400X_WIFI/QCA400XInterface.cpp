/* QCA4002/4004 implementation of NetworkInterfaceAPI
 * Copyright (c) 2019 ARM Limited
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

#include "QCA400XInterface.h"
#include "events/EventQueue.h"
#include "events/mbed_shared_queues.h"
#include "features/netsocket/nsapi_types.h"
#include "mbed_trace.h"
#include "platform/Callback.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_debug.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_error.h"

#define TRACE_GROUP  "QCAI" // QCA400X Interface

#define QCA400X_WIFI_IF_NAME "qca0"

using namespace mbed;
using namespace rtos;

QCA400XInterface::QCA400XInterface():
    _connect_retval(NSAPI_ERROR_OK),
    _ap_sec(NSAPI_SECURITY_NONE),
    _channel_num(1),
    _if_blocking(true),
    _if_connected(_cmutex),
    _conn_stat(NSAPI_STATUS_DISCONNECTED),
    _conn_stat_cb(NULL),
    _global_event_queue(mbed_event_queue()), // Needs to be set before attaching event() to SIGIO
    _connect_event_id(0)
{
    memset(_ssid, 0, sizeof(_ssid));
    memset(ap_pass, 0, sizeof(ap_pass));

    _qca400x.sigio(this, &QCA400XInterface::event);
    _qca400x.attach(this, &QCA400XInterface::refresh_conn_state_cb);

    for (int i = 0; i < QCA400X_SOCKET_COUNT; i++) {
        _sock_i[i].open = false;
        _sock_i[i].sport = 0;
    }
}

QCA400XInterface::~QCA400XInterface()
{
    _cmutex.lock();
    if (_connect_event_id) {
        _global_event_queue->cancel(_connect_event_id);
    }
    _cmutex.unlock();
}

int QCA400XInterface::connect(const char *ssid, const char *pass,
                              nsapi_security_t security, uint8_t channel)
{
    nsapi_error_t ret_code;

    ret_code = set_credentials(ssid, pass, security);
    if (ret_code) {
        return ret_code;
    }

    _channel_num = channel;

    return connect();
}

void QCA400XInterface::_connect_async()
{
    _cmutex.lock();
    if (!_connect_event_id) {
        _cmutex.unlock();
        return;
    }

    _connect_retval = _qca400x.connect(_ssid, ap_pass, _ap_sec, _channel_num);

    _if_connected.notify_all();

    _connect_event_id = 0;

    _cmutex.unlock();
}

int QCA400XInterface::connect()
{
    nsapi_error_t status = _conn_status_to_error();
    if (status != NSAPI_ERROR_NO_CONNECTION) {
        return status;
    }

    if (strlen(_ssid) == 0) {
        return NSAPI_ERROR_NO_SSID;
    }

    if (_ap_sec != NSAPI_SECURITY_NONE) {
        if (strlen(ap_pass) < QCA400X_PASSPHRASE_MIN_LENGTH) {
            return NSAPI_ERROR_PARAMETER;
        }
    }

    status = _qca400x.initialize();
    if (status != NSAPI_ERROR_OK) {
        return status;
    }

    _cmutex.lock();

    _connect_retval = NSAPI_ERROR_NO_CONNECTION;
    MBED_ASSERT(!_connect_event_id);

    _connect_event_id = _global_event_queue->call(callback(this, &QCA400XInterface::_connect_async));

    if (!_connect_event_id) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ENOMEM), \
                   "connect(): unable to add event to queue. Increase \"events.shared-eventsize\"\n");
    }

    while (_if_blocking && (_conn_status_to_error() != NSAPI_ERROR_IS_CONNECTED)
            && (_connect_retval == NSAPI_ERROR_NO_CONNECTION)) {
        _if_connected.wait();
    }

    _cmutex.unlock();

    if (!_if_blocking) {
        return NSAPI_ERROR_OK;
    } else {
        return _connect_retval;
    }
}

int QCA400XInterface::set_credentials(const char *ssid, const char *pass,
                                      nsapi_security_t security)
{
    nsapi_error_t status = _conn_status_to_error();
    if (status != NSAPI_ERROR_NO_CONNECTION) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    _ap_sec = security;

    if (!ssid) {
        return NSAPI_ERROR_PARAMETER;
    }

    int ssid_length = strlen(ssid);

    if (ssid_length > 0 && ssid_length <= QCA400X_SSID_MAX_LENGTH) {
        memset(_ssid, 0, sizeof(_ssid));
        strncpy(_ssid, ssid, sizeof(_ssid));
    } else {
        return NSAPI_ERROR_PARAMETER;
    }

    if (_ap_sec != NSAPI_SECURITY_NONE) {

        if (!pass) {
            return NSAPI_ERROR_PARAMETER;
        }

        int pass_length = strlen(pass);
        if (pass_length >= QCA400X_PASSPHRASE_MIN_LENGTH
                && pass_length <= QCA400X_PASSPHRASE_MAX_LENGTH) {
            memset(ap_pass, 0, sizeof(ap_pass));
            strncpy(ap_pass, pass, sizeof(ap_pass));
        } else {
            return NSAPI_ERROR_PARAMETER;
        }
    } else {
        memset(ap_pass, 0, sizeof(ap_pass));
    }

    return NSAPI_ERROR_OK;
}

int QCA400XInterface::set_channel(uint8_t channel)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int QCA400XInterface::disconnect()
{
    _cmutex.lock();
    if (_connect_event_id) {
        _global_event_queue->cancel(_connect_event_id);
        _connect_event_id = 0; // cancel asynchronous connection attempt if one is ongoing
    }
    _cmutex.unlock();

    nsapi_error_t status = _conn_status_to_error();
    if (status == NSAPI_ERROR_NO_CONNECTION) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    int ret = _qca400x.disconnect() ? NSAPI_ERROR_OK : NSAPI_ERROR_DEVICE_ERROR;

    return ret;
}

const char *QCA400XInterface::get_ip_address()
{
    const char *ip_buff = _qca400x.getIPAddress();
    if (!ip_buff || strcmp(ip_buff, "0.0.0.0") == 0) {
        return NULL;
    }

    return ip_buff;
}

nsapi_error_t QCA400XInterface::get_ip_address(SocketAddress *address)
{
    const char *ip_buff = _qca400x.getIPAddress();
    if (!ip_buff || strcmp(ip_buff, "0.0.0.0") == 0) {
        return NULL;
    }
    if (ip_buff) {
        address->set_ip_address(ip_buff);
        return NSAPI_ERROR_OK;
    }
    return NSAPI_ERROR_NO_ADDRESS;
}

const char *QCA400XInterface::get_mac_address()
{
    return _qca400x.getMACAddress();
}

const char *QCA400XInterface::get_gateway()
{
    return _conn_stat != NSAPI_STATUS_DISCONNECTED ? _qca400x.getGateway() : NULL;
}

nsapi_error_t QCA400XInterface::get_gateway(SocketAddress *address)
{
    if (address == nullptr) {
        return NSAPI_ERROR_PARAMETER;
    }
    if (_conn_stat == NSAPI_STATUS_DISCONNECTED) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    if (!address->set_ip_address(_qca400x.getGateway())) {
        return NSAPI_ERROR_NO_ADDRESS;
    }

    return NSAPI_ERROR_OK;
}

const char *QCA400XInterface::get_netmask()
{
    return _conn_stat != NSAPI_STATUS_DISCONNECTED ? _qca400x.getNetmask() : NULL;
}

nsapi_error_t QCA400XInterface::get_netmask(SocketAddress *address)
{
    if (address == nullptr) {
        return NSAPI_ERROR_PARAMETER;
    }
    if (_conn_stat == NSAPI_STATUS_DISCONNECTED) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    if (!address->set_ip_address(_qca400x.getNetmask())) {
        return NSAPI_ERROR_NO_ADDRESS;
    }

    return NSAPI_ERROR_OK;
}

char *QCA400XInterface::get_interface_name(char *interface_name)
{
    memcpy(interface_name, QCA400X_WIFI_IF_NAME, sizeof(QCA400X_WIFI_IF_NAME));
    return interface_name;
}

int8_t QCA400XInterface::get_rssi()
{
    if (_conn_stat == NSAPI_STATUS_DISCONNECTED) {
        return 0;
    }

    return _qca400x.rssi();
}

int QCA400XInterface::scan(WiFiAccessPoint *res, unsigned count)
{
    nsapi_error_t status;

    status = _qca400x.initialize();
    if (status != NSAPI_ERROR_OK) {
        return status;
    }

    return _qca400x.scan(res, count);
}

nsapi_error_t QCA400XInterface::socket_open(void **handle, nsapi_protocol_t proto)
{
    // Look for an unused socket
    int id = -1;
    nsapi_error_t ret;
    nsapi_error_t status;

    for (int i = 0; i < QCA400X_SOCKET_COUNT; i++) {
        if (!_sock_i[i].open) {
            id = i;
            _sock_i[i].open = true;
            break;
        }
    }

    if (id == -1) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    status = _qca400x.initialize();
    if (status != NSAPI_ERROR_OK) {
        return status;
    }

    ret = _qca400x.open_socket(id, proto);
    if (ret != NSAPI_ERROR_OK) {
        _sock_i[id].open = false;
        return NSAPI_ERROR_NO_SOCKET;
    }

    struct qca400x_socket *socket = new struct qca400x_socket;
    if (!socket) {
        _sock_i[id].open = false;
        return NSAPI_ERROR_NO_SOCKET;
    }

    socket->id = id;
    socket->proto = proto;
    socket->connected = false;
    *handle = socket;

    return 0;
}

nsapi_error_t QCA400XInterface::socket_close(void *handle)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;
    int err = 0;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (!_qca400x.close_socket(socket->id)) {
        err = NSAPI_ERROR_DEVICE_ERROR;
    }

    _cbs[socket->id].callback = NULL;
    _cbs[socket->id].data = NULL;
    core_util_atomic_store_u8(&_cbs[socket->id].deferred, false);

    socket->connected = false;
    _sock_i[socket->id].open = false;
    _sock_i[socket->id].sport = 0;

    delete socket;

    return err;
}

int QCA400XInterface::socket_bind(void *handle, const SocketAddress &address)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;
    nsapi_error_t ret;
    nsapi_addr_t ip_addr;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (socket->connected) {
        return NSAPI_ERROR_PARAMETER;
    }

    if (socket->proto == NSAPI_UDP) {
        if (address.get_addr().version != NSAPI_UNSPEC) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        for (int id = 0; id < QCA400X_SOCKET_COUNT; id++) {
            if (_sock_i[id].sport == address.get_port() && id != socket->id) { // Port already reserved by another socket
                return NSAPI_ERROR_PARAMETER;
            }
        }

        ip_addr = address.get_addr();
        ret = _qca400x.bind_socket(socket->id, ip_addr.bytes, address.get_port());
        if (ret == NSAPI_ERROR_OK) {
            _sock_i[socket->id].sport = address.get_port();
            return 0;
        }
    }

    return NSAPI_ERROR_UNSUPPORTED;
}

int QCA400XInterface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int QCA400XInterface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;
    nsapi_error_t ret;
    nsapi_addr_t ip_addr;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    ip_addr = addr.get_addr();

    ret = _qca400x.connect_socket(socket->id, ip_addr.bytes, addr.get_port());

    socket->connected = (ret == NSAPI_ERROR_OK) ? true : false;

    return ret;

}

int QCA400XInterface::socket_accept(void *handle, void **socket, SocketAddress *address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int QCA400XInterface::socket_send(void *handle, const void *data, unsigned size)
{
    nsapi_error_t status;
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (!socket->connected) {
        return NSAPI_ERROR_CONNECTION_LOST;
    }

    if (data == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    status = _qca400x.send(socket->id, data, size);

    return status != NSAPI_ERROR_OK ? status : size;

}

int QCA400XInterface::socket_recv(void *handle, void *data, unsigned size)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (!_sock_i[socket->id].open) {
        return NSAPI_ERROR_CONNECTION_LOST;
    }

    int32_t recv;

    recv = _qca400x.recv(socket->id, data, size);
    if (recv <= 0 && recv != NSAPI_ERROR_WOULD_BLOCK) {
        socket->connected = false;
    }

    return recv;
}

int QCA400XInterface::socket_sendto(void *handle, const SocketAddress &addr, const void *data, unsigned size)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;
    nsapi_error_t status;
    nsapi_addr_t ip_addr;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if ((strcmp(addr.get_ip_address(), "0.0.0.0") == 0) || !addr.get_port())  {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    if (data == NULL) {
        return NSAPI_ERROR_PARAMETER;
    }

    ip_addr = addr.get_addr();

    status = _qca400x.sendto(socket->id, data, size, ip_addr.bytes, addr.get_port());

    socket->connected = (status == NSAPI_ERROR_OK) ? true : false;

    return status != NSAPI_ERROR_OK ? status : size;
}

int QCA400XInterface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    int recv = _qca400x.recvfrom(socket, data, size);
    if (recv < 0 && recv != NSAPI_ERROR_WOULD_BLOCK) {
        socket->connected = false;
    } else if (recv >= 0 && addr) {
        *addr = socket->addr;
    }

    return recv;
}

void QCA400XInterface::socket_attach(void *handle, void (*attach_callback)(void *), void *data)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;
    _cbs[socket->id].callback = attach_callback;
    _cbs[socket->id].data = data;
}

nsapi_error_t QCA400XInterface::setsockopt(nsapi_socket_t handle, int level, int optname,
                                           const void *optval, unsigned optlen)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t QCA400XInterface::getsockopt(nsapi_socket_t handle, int level, int optname,
                                           void *optval, unsigned *optlen)
{
    struct qca400x_socket *socket = (struct qca400x_socket *)handle;

    if (!optval || !optlen) {
        return NSAPI_ERROR_PARAMETER;
    } else if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (level == NSAPI_SOCKET && socket->proto == NSAPI_TCP) {
        if ((optname == NSAPI_KEEPALIVE) || (optname == NSAPI_LINGER)) {
            if (*optlen > sizeof(int)) {
                *optlen = sizeof(int);
            }
            return _qca400x.getsockopt(socket->id, optname, optval);
        }
    }

    return NSAPI_ERROR_UNSUPPORTED;
}

void QCA400XInterface::event(int sock)
{
    if (_cbs[sock].callback) {
        _cbs[sock].callback(_cbs[sock].data);
    }
}

void QCA400XInterface::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _conn_stat_cb = status_cb;
}

nsapi_connection_status_t QCA400XInterface::get_connection_status() const
{
    return _conn_stat;
}

#if MBED_CONF_QCA400X_PROVIDE_DEFAULT

WiFiInterface *WiFiInterface::get_default_instance()
{
    static QCA400XInterface wifi;
    return &wifi;
}

#endif

void QCA400XInterface::refresh_conn_state_cb()
{
    nsapi_connection_status_t prev_stat = _conn_stat;
    _conn_stat = _qca400x.connection_status();

    switch (_conn_stat) {
        // Doesn't require changes
        case NSAPI_STATUS_CONNECTING:
        case NSAPI_STATUS_GLOBAL_UP:
            break;
        // Start from scratch if connection drops/is dropped
        case NSAPI_STATUS_DISCONNECTED:
            break;
        // Handled on AT layer
        case NSAPI_STATUS_LOCAL_UP:
        case NSAPI_STATUS_ERROR_UNSUPPORTED:
        default:
            _conn_stat = NSAPI_STATUS_DISCONNECTED;
            for (int i = 0; i < QCA400X_SOCKET_COUNT; i++) {
                _sock_i[i].open = false;
                _sock_i[i].sport = 0;
            }
    }

    if (prev_stat == _conn_stat) {
        return;
    }

    tr_debug("refresh_conn_state_cb(): changed to %d", _conn_stat);

    // Inform upper layers
    if (_conn_stat_cb) {
        _conn_stat_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _conn_stat);
    }
}

nsapi_error_t QCA400XInterface::_conn_status_to_error()
{
    nsapi_error_t ret;

    switch (_conn_stat) {
        case NSAPI_STATUS_DISCONNECTED:
            ret = NSAPI_ERROR_NO_CONNECTION;
            break;
        case NSAPI_STATUS_CONNECTING:
            ret = NSAPI_ERROR_ALREADY;
            break;
        case NSAPI_STATUS_GLOBAL_UP:
            ret = NSAPI_ERROR_IS_CONNECTED;
            break;
        default:
            ret = NSAPI_ERROR_DEVICE_ERROR;
    }

    return ret;
}

nsapi_error_t QCA400XInterface::set_blocking(bool blocking)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

