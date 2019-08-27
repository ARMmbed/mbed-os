/* ESP8266 implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
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

#if DEVICE_SERIAL && DEVICE_INTERRUPTIN && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_PRESENT)

#include <string.h>
#include <stdint.h>

#include "ESP8266.h"
#include "ESP8266Interface.h"
#include "events/EventQueue.h"
#include "events/mbed_shared_queues.h"
#include "features/netsocket/nsapi_types.h"
#include "mbed_trace.h"
#include "platform/Callback.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_debug.h"
#include "rtos/ThisThread.h"

#ifndef MBED_CONF_ESP8266_DEBUG
#define MBED_CONF_ESP8266_DEBUG false
#endif

#ifndef MBED_CONF_ESP8266_RTS
#define MBED_CONF_ESP8266_RTS NC
#endif

#ifndef MBED_CONF_ESP8266_CTS
#define MBED_CONF_ESP8266_CTS NC
#endif

#ifndef MBED_CONF_ESP8266_RST
#define MBED_CONF_ESP8266_RST NC
#endif

#ifndef MBED_CONF_ESP8266_PWR
#define MBED_CONF_ESP8266_PWR NC
#endif

#define TRACE_GROUP  "ESPI" // ESP8266 Interface

using namespace mbed;
using namespace rtos;

#if defined MBED_CONF_ESP8266_TX && defined MBED_CONF_ESP8266_RX
ESP8266Interface::ESP8266Interface()
    : _esp(MBED_CONF_ESP8266_TX, MBED_CONF_ESP8266_RX, MBED_CONF_ESP8266_DEBUG, MBED_CONF_ESP8266_RTS, MBED_CONF_ESP8266_CTS),
      _rst_pin(MBED_CONF_ESP8266_RST), // Notice that Pin7 CH_EN cannot be left floating if used as reset
      _pwr_pin(MBED_CONF_ESP8266_PWR),
      _ap_sec(NSAPI_SECURITY_UNKNOWN),
      _if_blocking(true),
      _if_connected(_cmutex),
      _initialized(false),
      _connect_retval(NSAPI_ERROR_OK),
      _conn_stat(NSAPI_STATUS_DISCONNECTED),
      _conn_stat_cb(NULL),
      _global_event_queue(mbed_event_queue()), // Needs to be set before attaching event() to SIGIO
      _oob_event_id(0),
      _connect_event_id(0)
{
    memset(_cbs, 0, sizeof(_cbs));
    memset(ap_ssid, 0, sizeof(ap_ssid));
    memset(ap_pass, 0, sizeof(ap_pass));

    _ch_info.track_ap = true;
    strncpy(_ch_info.country_code, MBED_CONF_ESP8266_COUNTRY_CODE, sizeof(_ch_info.country_code));
    _ch_info.channel_start = MBED_CONF_ESP8266_CHANNEL_START;
    _ch_info.channels = MBED_CONF_ESP8266_CHANNELS;

    _esp.sigio(this, &ESP8266Interface::event);
    _esp.set_timeout();
    _esp.attach(this, &ESP8266Interface::refresh_conn_state_cb);

    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        _sock_i[i].open = false;
        _sock_i[i].sport = 0;
    }
}
#endif

// ESP8266Interface implementation
ESP8266Interface::ESP8266Interface(PinName tx, PinName rx, bool debug, PinName rts, PinName cts, PinName rst, PinName pwr)
    : _esp(tx, rx, debug, rts, cts),
      _rst_pin(rst),
      _pwr_pin(pwr),
      _ap_sec(NSAPI_SECURITY_UNKNOWN),
      _if_blocking(true),
      _if_connected(_cmutex),
      _initialized(false),
      _connect_retval(NSAPI_ERROR_OK),
      _conn_stat(NSAPI_STATUS_DISCONNECTED),
      _conn_stat_cb(NULL),
      _global_event_queue(mbed_event_queue()), // Needs to be set before attaching event() to SIGIO
      _oob_event_id(0),
      _connect_event_id(0)
{
    memset(_cbs, 0, sizeof(_cbs));
    memset(ap_ssid, 0, sizeof(ap_ssid));
    memset(ap_pass, 0, sizeof(ap_pass));

    _ch_info.track_ap = true;
    strncpy(_ch_info.country_code, MBED_CONF_ESP8266_COUNTRY_CODE, sizeof(_ch_info.country_code));
    _ch_info.channel_start = MBED_CONF_ESP8266_CHANNEL_START;
    _ch_info.channels = MBED_CONF_ESP8266_CHANNELS;

    _esp.sigio(this, &ESP8266Interface::event);
    _esp.set_timeout();
    _esp.attach(this, &ESP8266Interface::refresh_conn_state_cb);

    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        _sock_i[i].open = false;
        _sock_i[i].sport = 0;
    }
}

ESP8266Interface::~ESP8266Interface()
{
    if (_oob_event_id) {
        _global_event_queue->cancel(_oob_event_id);
    }

    _cmutex.lock();
    if (_connect_event_id) {
        _global_event_queue->cancel(_connect_event_id);
    }
    _cmutex.unlock();

    // Power down the modem
    _rst_pin.rst_assert();
    // Power off the modem
    _pwr_pin.power_off();
}

ESP8266Interface::ResetPin::ResetPin(PinName rst_pin) : _rst_pin(mbed::DigitalOut(rst_pin, 1))
{
}

void ESP8266Interface::ResetPin::rst_assert()
{
    if (_rst_pin.is_connected()) {
        _rst_pin = 0;
        tr_debug("HW reset asserted");
    }
}

void ESP8266Interface::ResetPin::rst_deassert()
{
    if (_rst_pin.is_connected()) {
        // Notice that Pin7 CH_EN cannot be left floating if used as reset
        _rst_pin = 1;
        tr_debug("HW reset deasserted");
    }
}

bool ESP8266Interface::ResetPin::is_connected()
{
    return _rst_pin.is_connected();
}

ESP8266Interface::PowerPin::PowerPin(PinName pwr_pin) : _pwr_pin(mbed::DigitalOut(pwr_pin, !MBED_CONF_ESP8266_POWER_ON_POLARITY))
{
}

void ESP8266Interface::PowerPin::power_on()
{
    if (_pwr_pin.is_connected()) {
        _pwr_pin = MBED_CONF_ESP8266_POWER_ON_POLARITY;
        tr_debug("HW power-on");
        ThisThread::sleep_for(MBED_CONF_ESP8266_POWER_ON_TIME_MS);
    }
}

void ESP8266Interface::PowerPin::power_off()
{
    if (_pwr_pin.is_connected()) {
        _pwr_pin = !MBED_CONF_ESP8266_POWER_ON_POLARITY;
        tr_debug("HW power-off");
        ThisThread::sleep_for(MBED_CONF_ESP8266_POWER_OFF_TIME_MS);
    }
}

bool ESP8266Interface::PowerPin::is_connected()
{
    return _pwr_pin.is_connected();
}

int ESP8266Interface::connect(const char *ssid, const char *pass, nsapi_security_t security,
                              uint8_t channel)
{
    if (channel != 0) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    int err = set_credentials(ssid, pass, security);
    if (err) {
        return err;
    }

    return connect();
}

void ESP8266Interface::_connect_async()
{
    _cmutex.lock();
    if (!_connect_event_id) {
        tr_debug("_connect_async(): cancelled");
        _cmutex.unlock();
        return;
    }
    _connect_retval = _esp.connect(ap_ssid, ap_pass);
    int timeleft_ms = ESP8266_INTERFACE_CONNECT_TIMEOUT_MS - _conn_timer.read_ms();
    if (_connect_retval == NSAPI_ERROR_OK || _connect_retval == NSAPI_ERROR_AUTH_FAILURE
            || _connect_retval == NSAPI_ERROR_NO_SSID
            || ((_if_blocking == true) && (timeleft_ms <= 0))) {
        _connect_event_id = 0;
        _conn_timer.stop();
        if (timeleft_ms <= 0) {
            _connect_retval = NSAPI_ERROR_CONNECTION_TIMEOUT;
        }
        _if_connected.notify_all();
    } else {
        // Postpone to give other stuff time to run
        _connect_event_id = _global_event_queue->call_in(ESP8266_INTERFACE_CONNECT_INTERVAL_MS,
                                                         callback(this, &ESP8266Interface::_connect_async));
        if (!_connect_event_id) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ENOMEM), \
                       "ESP8266Interface::_connect_async(): unable to add event to queue. Increase \"events.shared-eventsize\"\n");
        }
    }
    _cmutex.unlock();
}

int ESP8266Interface::connect()
{
    nsapi_error_t status = _conn_status_to_error();
    if (status != NSAPI_ERROR_NO_CONNECTION) {
        return status;
    }

    if (strlen(ap_ssid) == 0) {
        return NSAPI_ERROR_NO_SSID;
    }

    if (_ap_sec != NSAPI_SECURITY_NONE) {
        if (strlen(ap_pass) < ESP8266_PASSPHRASE_MIN_LENGTH) {
            return NSAPI_ERROR_PARAMETER;
        }
    }

    status = _init();
    if (status != NSAPI_ERROR_OK) {
        return status;
    }

    if (get_ip_address()) {
        return NSAPI_ERROR_IS_CONNECTED;
    }

    if (!_esp.dhcp(true, 1)) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    _cmutex.lock();

    _connect_retval = NSAPI_ERROR_NO_CONNECTION;
    MBED_ASSERT(!_connect_event_id);
    _conn_timer.stop();
    _conn_timer.reset();
    _conn_timer.start();
    _connect_event_id = _global_event_queue->call(callback(this, &ESP8266Interface::_connect_async));

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

int ESP8266Interface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    nsapi_error_t status = _conn_status_to_error();
    if (status != NSAPI_ERROR_NO_CONNECTION) {
        return status;
    }

    _ap_sec = security;

    if (!ssid) {
        return NSAPI_ERROR_PARAMETER;
    }

    int ssid_length = strlen(ssid);

    if (ssid_length > 0
            && ssid_length <= ESP8266_SSID_MAX_LENGTH) {
        memset(ap_ssid, 0, sizeof(ap_ssid));
        strncpy(ap_ssid, ssid, sizeof(ap_ssid));
    } else {
        return NSAPI_ERROR_PARAMETER;
    }

    if (_ap_sec != NSAPI_SECURITY_NONE) {

        if (!pass) {
            return NSAPI_ERROR_PARAMETER;
        }

        int pass_length = strlen(pass);
        if (pass_length >= ESP8266_PASSPHRASE_MIN_LENGTH
                && pass_length <= ESP8266_PASSPHRASE_MAX_LENGTH) {
            memset(ap_pass, 0, sizeof(ap_pass));
            strncpy(ap_pass, pass, ESP8266_PASSPHRASE_MAX_LENGTH);
        } else {
            return NSAPI_ERROR_PARAMETER;
        }
    } else {
        memset(ap_pass, 0, sizeof(ap_pass));
    }

    return NSAPI_ERROR_OK;
}

int ESP8266Interface::set_channel(uint8_t channel)
{
    return NSAPI_ERROR_UNSUPPORTED;
}


int ESP8266Interface::disconnect()
{
    _cmutex.lock();
    if (_connect_event_id) {
        _global_event_queue->cancel(_connect_event_id);
        _connect_event_id = 0; // cancel asynchronous connection attempt if one is ongoing
    }
    _cmutex.unlock();
    _initialized = false;

    nsapi_error_t status = _conn_status_to_error();
    if (status == NSAPI_ERROR_NO_CONNECTION) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    int ret = _esp.disconnect() ? NSAPI_ERROR_OK : NSAPI_ERROR_DEVICE_ERROR;

    if (ret == NSAPI_ERROR_OK) {
        // Try to lure the nw status update from ESP8266, might come later
        _esp.bg_process_oob(ESP8266_RECV_TIMEOUT, true);
        // In case the status update arrives later inform upper layers manually
        if (_conn_stat != NSAPI_STATUS_DISCONNECTED) {
            _conn_stat = NSAPI_STATUS_DISCONNECTED;
            if (_conn_stat_cb) {
                _conn_stat_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _conn_stat);
            }
        }
    }

    // Power down the modem
    _rst_pin.rst_assert();
    // Power off the modem
    _pwr_pin.power_off();

    return ret;
}

const char *ESP8266Interface::get_ip_address()
{
    const char *ip_buff = _esp.ip_addr();
    if (!ip_buff || strcmp(ip_buff, "0.0.0.0") == 0) {
        return NULL;
    }

    return ip_buff;
}

const char *ESP8266Interface::get_mac_address()
{
    return _esp.mac_addr();
}

const char *ESP8266Interface::get_gateway()
{
    return _conn_stat != NSAPI_STATUS_DISCONNECTED ? _esp.gateway() : NULL;
}

const char *ESP8266Interface::get_netmask()
{
    return _conn_stat != NSAPI_STATUS_DISCONNECTED ? _esp.netmask() : NULL;
}

int8_t ESP8266Interface::get_rssi()
{
    return _esp.rssi();
}

int ESP8266Interface::scan(WiFiAccessPoint *res, unsigned count)
{
    return scan(res, count, SCANMODE_ACTIVE, 0, 0);
}

int ESP8266Interface::scan(WiFiAccessPoint *res, unsigned count, scan_mode mode, unsigned t_max, unsigned t_min)
{
    if (t_max > ESP8266_SCAN_TIME_MAX) {
        return NSAPI_ERROR_PARAMETER;
    }
    if (mode == SCANMODE_ACTIVE && t_min > t_max) {
        return NSAPI_ERROR_PARAMETER;
    }

    nsapi_error_t status = _init();
    if (status != NSAPI_ERROR_OK) {
        return status;
    }

    return _esp.scan(res, count, (mode == SCANMODE_ACTIVE ? ESP8266::SCANMODE_ACTIVE : ESP8266::SCANMODE_PASSIVE),
                     t_min, t_max);
}

bool ESP8266Interface::_get_firmware_ok()
{
    ESP8266::fw_at_version at_v = _esp.at_version();
    if (at_v.major < ESP8266_AT_VERSION_MAJOR) {
        debug("ESP8266: ERROR: AT Firmware v%d incompatible with this driver.", at_v.major);
        debug("Update at least to v%d - https://developer.mbed.org/teams/ESP8266/wiki/Firmware-Update\n", ESP8266_AT_VERSION_MAJOR);
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_UNSUPPORTED), "Too old AT firmware");
    }
    ESP8266::fw_sdk_version sdk_v = _esp.sdk_version();
    if (sdk_v.major < ESP8266_SDK_VERSION_MAJOR) {
        debug("ESP8266: ERROR: Firmware v%d incompatible with this driver.", sdk_v.major);
        debug("Update at least to v%d - https://developer.mbed.org/teams/ESP8266/wiki/Firmware-Update\n", ESP8266_SDK_VERSION_MAJOR);
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_UNSUPPORTED), "Too old SDK firmware");
    }

    return true;
}

nsapi_error_t ESP8266Interface::_init(void)
{
    if (!_initialized) {
        _pwr_pin.power_off();
        _pwr_pin.power_on();
        if (_reset() != NSAPI_ERROR_OK) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.echo_off()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.start_uart_hw_flow_ctrl()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_get_firmware_ok()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.set_default_wifi_mode(ESP8266::WIFIMODE_STATION)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.set_country_code_policy(true, _ch_info.country_code, _ch_info.channel_start, _ch_info.channels)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.cond_enable_tcp_passive_mode()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.startup(ESP8266::WIFIMODE_STATION)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }

        _initialized = true;
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t ESP8266Interface::_reset()
{
    if (_rst_pin.is_connected()) {
        _rst_pin.rst_assert();
        // If you happen to use Pin7 CH_EN as reset pin, not needed otherwise
        // https://www.espressif.com/sites/default/files/documentation/esp8266_hardware_design_guidelines_en.pdf
        ThisThread::sleep_for(2); // Documentation says 200 us; need 2 ticks to get minimum 1 ms.
        _esp.flush();
        _rst_pin.rst_deassert();
    } else {
        _esp.flush();
        if (!_esp.at_available()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        if (!_esp.reset()) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
    }

    return _esp.at_available() ? NSAPI_ERROR_OK : NSAPI_ERROR_DEVICE_ERROR;
}

struct esp8266_socket {
    int id;
    nsapi_protocol_t proto;
    bool connected;
    SocketAddress addr;
    int keepalive; // TCP
};

int ESP8266Interface::socket_open(void **handle, nsapi_protocol_t proto)
{
    // Look for an unused socket
    int id = -1;

    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        if (!_sock_i[i].open) {
            id = i;
            _sock_i[i].open = true;
            break;
        }
    }

    if (id == -1) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    struct esp8266_socket *socket = new struct esp8266_socket;
    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    socket->id = id;
    socket->proto = proto;
    socket->connected = false;
    socket->keepalive = 0;
    *handle = socket;
    return 0;
}

int ESP8266Interface::socket_close(void *handle)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    int err = 0;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (socket->connected && !_esp.close(socket->id)) {
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

int ESP8266Interface::socket_bind(void *handle, const SocketAddress &address)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (socket->proto == NSAPI_UDP) {
        if (address.get_addr().version != NSAPI_UNSPEC) {
            return NSAPI_ERROR_UNSUPPORTED;
        }

        for (int id = 0; id < ESP8266_SOCKET_COUNT; id++) {
            if (_sock_i[id].sport == address.get_port() && id != socket->id) { // Port already reserved by another socket
                return NSAPI_ERROR_PARAMETER;
            } else if (id == socket->id && socket->connected) {
                return NSAPI_ERROR_PARAMETER;
            }
        }
        _sock_i[socket->id].sport = address.get_port();
        return 0;
    }

    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    nsapi_error_t ret;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (socket->proto == NSAPI_UDP) {
        ret = _esp.open_udp(socket->id, addr.get_ip_address(), addr.get_port(), _sock_i[socket->id].sport);
    } else {
        ret = _esp.open_tcp(socket->id, addr.get_ip_address(), addr.get_port(), socket->keepalive);
    }

    socket->connected = (ret == NSAPI_ERROR_OK) ? true : false;

    return ret;
}

int ESP8266Interface::socket_accept(void *server, void **socket, SocketAddress *addr)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_send(void *handle, const void *data, unsigned size)
{
    nsapi_error_t status;
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    uint8_t expect_false = false;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (!_sock_i[socket->id].open) {
        return NSAPI_ERROR_CONNECTION_LOST;
    }

    if (!size) {
        // Firmware limitation
        return socket->proto == NSAPI_TCP ? 0 : NSAPI_ERROR_UNSUPPORTED;
    }

    status = _esp.send(socket->id, data, size);

    if (status == NSAPI_ERROR_WOULD_BLOCK
            && socket->proto == NSAPI_TCP
            && core_util_atomic_cas_u8(&_cbs[socket->id].deferred, &expect_false, true)) {
        tr_debug("Postponing SIGIO from the device");
        if (!_global_event_queue->call_in(50, callback(this, &ESP8266Interface::event_deferred))) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ENOMEM), \
                       "socket_send(): unable to add event to queue. Increase \"events.shared-eventsize\"\n");
        }

    } else if (status == NSAPI_ERROR_WOULD_BLOCK && socket->proto == NSAPI_UDP) {
        status = NSAPI_ERROR_DEVICE_ERROR;
    }

    return status != NSAPI_ERROR_OK ? status : size;
}

int ESP8266Interface::socket_recv(void *handle, void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (!_sock_i[socket->id].open) {
        return NSAPI_ERROR_CONNECTION_LOST;
    }

    int32_t recv;
    if (socket->proto == NSAPI_TCP) {
        recv = _esp.recv_tcp(socket->id, data, size);
        if (recv <= 0 && recv != NSAPI_ERROR_WOULD_BLOCK) {
            socket->connected = false;
        }
    } else {
        recv = _esp.recv_udp(socket->id, data, size);
    }

    return recv;
}

int ESP8266Interface::socket_sendto(void *handle, const SocketAddress &addr, const void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if ((strcmp(addr.get_ip_address(), "0.0.0.0") == 0) || !addr.get_port())  {
        return NSAPI_ERROR_DNS_FAILURE;
    }

    if (socket->connected && socket->addr != addr) {
        if (!_esp.close(socket->id)) {
            return NSAPI_ERROR_DEVICE_ERROR;
        }
        socket->connected = false;
    }

    if (!socket->connected) {
        int err = socket_connect(socket, addr);
        if (err < 0) {
            return err;
        }
        socket->addr = addr;
    }

    return socket_send(socket, data, size);
}

int ESP8266Interface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    int ret = socket_recv(socket, data, size);
    if (ret >= 0 && addr) {
        *addr = socket->addr;
    }

    return ret;
}

void ESP8266Interface::socket_attach(void *handle, void (*callback)(void *), void *data)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _cbs[socket->id].callback = callback;
    _cbs[socket->id].data = data;
}

nsapi_error_t ESP8266Interface::setsockopt(nsapi_socket_t handle, int level,
                                           int optname, const void *optval, unsigned optlen)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!optlen) {
        return NSAPI_ERROR_PARAMETER;
    } else if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (level == NSAPI_SOCKET && socket->proto == NSAPI_TCP) {
        switch (optname) {
            case NSAPI_KEEPALIVE: {
                if (socket->connected) { // ESP8266 limitation, keepalive needs to be given before connecting
                    return NSAPI_ERROR_UNSUPPORTED;
                }

                if (optlen == sizeof(int)) {
                    int secs = *(int *)optval;
                    if (secs  >= 0 && secs <= 7200) {
                        socket->keepalive = secs;
                        return NSAPI_ERROR_OK;
                    }
                }
                return NSAPI_ERROR_PARAMETER;
            }
        }
    }

    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t ESP8266Interface::getsockopt(nsapi_socket_t handle, int level, int optname, void *optval, unsigned *optlen)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;

    if (!optval || !optlen) {
        return NSAPI_ERROR_PARAMETER;
    } else if (!socket) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (level == NSAPI_SOCKET && socket->proto == NSAPI_TCP) {
        switch (optname) {
            case NSAPI_KEEPALIVE: {
                if (*optlen > sizeof(int)) {
                    *optlen = sizeof(int);
                }
                memcpy(optval, &(socket->keepalive), *optlen);
                return NSAPI_ERROR_OK;
            }
        }
    }

    return NSAPI_ERROR_UNSUPPORTED;
}


void ESP8266Interface::event()
{
    if (!_oob_event_id) {
        // Throttles event creation by using arbitrary small delay
        _oob_event_id = _global_event_queue->call_in(50, callback(this, &ESP8266Interface::proc_oob_evnt));
        if (!_oob_event_id) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ENOMEM), \
                       "ESP8266Interface::event(): unable to add event to queue. Increase \"events.shared-eventsize\"\n");
        }
    }

    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        if (_cbs[i].callback) {
            _cbs[i].callback(_cbs[i].data);
        }
    }
}

void ESP8266Interface::event_deferred()
{
    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        uint8_t expect_true = true;
        if (core_util_atomic_cas_u8(&_cbs[i].deferred, &expect_true, false) && _cbs[i].callback) {
            _cbs[i].callback(_cbs[i].data);
        }
    }
}

void ESP8266Interface::attach(Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _conn_stat_cb = status_cb;
}

nsapi_connection_status_t ESP8266Interface::get_connection_status() const
{
    return _conn_stat;
}

#if MBED_CONF_ESP8266_PROVIDE_DEFAULT

WiFiInterface *WiFiInterface::get_default_instance()
{
    static ESP8266Interface esp;
    return &esp;
}

#endif

void ESP8266Interface::refresh_conn_state_cb()
{
    nsapi_connection_status_t prev_stat = _conn_stat;
    _conn_stat = _esp.connection_status();

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
            _initialized = false;
            _conn_stat = NSAPI_STATUS_DISCONNECTED;
            for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
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

void ESP8266Interface::proc_oob_evnt()
{
    _oob_event_id = 0; // Allows creation of a new event
    _esp.bg_process_oob(ESP8266_RECV_TIMEOUT, true);
}

nsapi_error_t ESP8266Interface::_conn_status_to_error()
{
    nsapi_error_t ret;

    _esp.bg_process_oob(ESP8266_RECV_TIMEOUT, true);

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

nsapi_error_t ESP8266Interface::set_blocking(bool blocking)
{
    _if_blocking = blocking;

    return NSAPI_ERROR_OK;
}

nsapi_error_t ESP8266Interface::set_country_code(bool track_ap, const char *country_code, int len, int channel_start, int channels)
{
    for (int i = 0; i < len; i++) {
        // Validation done by firmware
        if (!country_code[i]) {
            tr_warning("invalid country code");
            return NSAPI_ERROR_PARAMETER;
        }
    }

    _ch_info.track_ap = track_ap;

    // Firmware takes only first three characters
    strncpy(_ch_info.country_code, country_code, sizeof(_ch_info.country_code));
    _ch_info.country_code[sizeof(_ch_info.country_code) - 1] = '\0';

    _ch_info.channel_start = channel_start;
    _ch_info.channels = channels;

    return NSAPI_ERROR_OK;
}

#endif
