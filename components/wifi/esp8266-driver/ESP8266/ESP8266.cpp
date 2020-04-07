/* ESP8266 Example
 * Copyright (c) 2015 ARM Limited
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

#if DEVICE_SERIAL && DEVICE_INTERRUPTIN && defined(MBED_CONF_EVENTS_PRESENT) && defined(MBED_CONF_NSAPI_PRESENT) && defined(MBED_CONF_RTOS_API_PRESENT)
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include <string.h>
#include <stdlib.h>

#include "ESP8266.h"
#include "features/netsocket/nsapi_types.h"
#include "mbed_trace.h"
#include "PinNames.h"
#include "platform/Callback.h"
#include "platform/mbed_error.h"
#include "rtos/Kernel.h"

#define TRACE_GROUP  "ESPA" // ESP8266 AT layer

#define ESP8266_ALL_SOCKET_IDS      -1

#define ESP8266_DEFAULT_SERIAL_BAUDRATE 115200


using namespace mbed;

ESP8266::ESP8266(PinName tx, PinName rx, bool debug, PinName rts, PinName cts)
    : _sdk_v(-1, -1, -1),
      _at_v(-1, -1, -1),
      _tcp_passive(false),
      _callback(),
      _serial(tx, rx, MBED_CONF_ESP8266_SERIAL_BAUDRATE),
      _serial_rts(rts),
      _serial_cts(cts),
      _parser(&_serial),
      _packets(0),
      _packets_end(&_packets),
      _sock_active_id(-1),
      _heap_usage(0),
      _connect_error(0),
      _disconnect(false),
      _fail(false),
      _sock_already(false),
      _closed(false),
      _error(false),
      _busy(false),
      _reset_done(false),
      _sock_sending_id(-1),
      _conn_status(NSAPI_STATUS_DISCONNECTED)
{
    _serial.set_baud(ESP8266_DEFAULT_SERIAL_BAUDRATE);
    _parser.debug_on(debug);
    _parser.set_delimiter("\r\n");
    _parser.oob("+IPD", callback(this, &ESP8266::_oob_packet_hdlr));
    //Note: espressif at command document says that this should be +CWJAP_CUR:<error code>
    //but seems that at least current version is not sending it
    //https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
    //Also seems that ERROR is not sent, but FAIL instead
    _parser.oob("0,CLOSED", callback(this, &ESP8266::_oob_socket0_closed));
    _parser.oob("1,CLOSED", callback(this, &ESP8266::_oob_socket1_closed));
    _parser.oob("2,CLOSED", callback(this, &ESP8266::_oob_socket2_closed));
    _parser.oob("3,CLOSED", callback(this, &ESP8266::_oob_socket3_closed));
    _parser.oob("4,CLOSED", callback(this, &ESP8266::_oob_socket4_closed));
    _parser.oob("+CWJAP:", callback(this, &ESP8266::_oob_connect_err));
    _parser.oob("WIFI ", callback(this, &ESP8266::_oob_connection_status));
    _parser.oob("UNLINK", callback(this, &ESP8266::_oob_socket_close_err));
    _parser.oob("ALREADY CONNECTED", callback(this, &ESP8266::_oob_conn_already));
    _parser.oob("ERROR", callback(this, &ESP8266::_oob_err));
    _parser.oob("ready", callback(this, &ESP8266::_oob_ready));
    _parser.oob("+CWLAP:", callback(this, &ESP8266::_oob_scan_results));
    // Don't expect to find anything about the watchdog reset in official documentation
    //https://techtutorialsx.com/2017/01/21/esp8266-watchdog-functions/
    _parser.oob("wdt reset", callback(this, &ESP8266::_oob_watchdog_reset));
    // Don't see a reason to make distiction between software(Software WDT reset) and hardware(wdt reset) watchdog treatment
    //https://github.com/esp8266/Arduino/blob/4897e0006b5b0123a2fa31f67b14a3fff65ce561/doc/faq/a02-my-esp-crashes.md#watchdog
    _parser.oob("Soft WDT reset", callback(this, &ESP8266::_oob_watchdog_reset));
    _parser.oob("busy ", callback(this, &ESP8266::_oob_busy));
    // NOTE: documentation v3.0 says '+CIPRECVDATA:<data_len>,' but it's not how the FW responds...
    _parser.oob("+CIPRECVDATA,", callback(this, &ESP8266::_oob_tcp_data_hdlr));
    // Register 'SEND OK'/'SEND FAIL' oobs here. Don't get involved in oob management with send status
    // because ESP8266 modem possibly doesn't reply these packets on error case.
    _parser.oob("SEND OK", callback(this, &ESP8266::_oob_send_ok_received));
    _parser.oob("SEND FAIL", callback(this, &ESP8266::_oob_send_fail_received));

    for (int i = 0; i < SOCKET_COUNT; i++) {
        _sock_i[i].open = false;
        _sock_i[i].proto = NSAPI_UDP;
        _sock_i[i].tcp_data = NULL;
        _sock_i[i].tcp_data_avbl = 0;
        _sock_i[i].tcp_data_rcvd = 0;
        _sock_i[i].send_fail = false;
    }

    _scan_r.res = NULL;
    _scan_r.limit = 0;
    _scan_r.cnt = 0;
}

bool ESP8266::at_available()
{
    bool ready = false;

    _smutex.lock();
    // Might take a while to respond after HW reset
    for (int i = 0; i < 5; i++) {
        ready = _parser.send("AT")
                && _parser.recv("OK\n");
        if (ready) {
            break;
        }
        tr_debug("at_available(): Waiting AT response.");
    }
    // Switch baud-rate from default one to assigned one
    if (MBED_CONF_ESP8266_SERIAL_BAUDRATE !=  ESP8266_DEFAULT_SERIAL_BAUDRATE) {
        ready &= _parser.send("AT+UART_CUR=%u,8,1,0,0", MBED_CONF_ESP8266_SERIAL_BAUDRATE)
                 && _parser.recv("OK\n");
        _serial.set_baud(MBED_CONF_ESP8266_SERIAL_BAUDRATE);
        ready &= _parser.send("AT")
                 && _parser.recv("OK\n");
    }
    _smutex.unlock();

    return ready;
}

bool ESP8266::echo_off()
{
    _smutex.lock();
    bool ready = _parser.send("ATE0")
                 && _parser.recv("OK\n");
    _smutex.unlock();

    return ready;
}

struct ESP8266::fw_sdk_version ESP8266::sdk_version()
{
    int major;
    int minor;
    int patch;

    _smutex.lock();
    bool done = _parser.send("AT+GMR")
                && _parser.recv("SDK version:%d.%d.%d", &major, &minor, &patch)
                && _parser.recv("OK\n");
    _smutex.unlock();

    if (done) {
        _sdk_v.major = major;
        _sdk_v.minor = minor;
        _sdk_v.patch = patch;
    }
    return _sdk_v;
}

struct ESP8266::fw_at_version ESP8266::at_version()
{
    int major;
    int minor;
    int patch;
    int nused;

    _smutex.lock();
    bool done = _parser.send("AT+GMR")
                && _parser.recv("AT version:%d.%d.%d.%d", &major, &minor, &patch, &nused)
                && _parser.recv("OK\n");
    _smutex.unlock();

    if (done) {
        _at_v.major = major;
        _at_v.minor = minor;
        _at_v.patch = patch;
    }
    return _at_v;
}

bool ESP8266::stop_uart_hw_flow_ctrl(void)
{
    bool done = true;
#if DEVICE_SERIAL_FC

    if (_serial_rts != NC || _serial_cts != NC) {
        // Stop board's flow control
        _serial.set_flow_control(SerialBase::Disabled, _serial_rts, _serial_cts);

        // Stop ESP8266's flow control
        done = _parser.send("AT+UART_CUR=%u,8,1,0,0", MBED_CONF_ESP8266_SERIAL_BAUDRATE)
               && _parser.recv("OK\n");
    }

#endif
    return done;
}

bool ESP8266::start_uart_hw_flow_ctrl(void)
{
    bool done = true;

#if DEVICE_SERIAL_FC
    _smutex.lock();
    if (_serial_rts != NC && _serial_cts != NC) {
        // Start ESP8266's flow control
        done = _parser.send("AT+UART_CUR=%u,8,1,0,3", MBED_CONF_ESP8266_SERIAL_BAUDRATE)
               && _parser.recv("OK\n");

        if (done) {
            // Start board's flow control
            _serial.set_flow_control(SerialBase::RTSCTS, _serial_rts, _serial_cts);
        }

    } else if (_serial_rts != NC) {
        _serial.set_flow_control(SerialBase::RTS, _serial_rts, NC);

        // Enable ESP8266's CTS pin
        done = _parser.send("AT+UART_CUR=%u,8,1,0,2", MBED_CONF_ESP8266_SERIAL_BAUDRATE)
               && _parser.recv("OK\n");

    } else if (_serial_cts != NC) {
        // Enable ESP8266's RTS pin
        done = _parser.send("AT+UART_CUR=%u,8,1,0,1", MBED_CONF_ESP8266_SERIAL_BAUDRATE)
               && _parser.recv("OK\n");

        if (done) {
            _serial.set_flow_control(SerialBase::CTS, NC, _serial_cts);
        }
    }
    _smutex.unlock();

    if (!done) {
        tr_debug("start_uart_hw_flow_ctrl(): Enable UART HW flow control: FAIL.");
    }
#else
    if (_serial_rts != NC || _serial_cts != NC) {
        done = false;
    }
#endif
    return done;
}

bool ESP8266::startup(int mode)
{
    if (!(mode == WIFIMODE_STATION || mode == WIFIMODE_SOFTAP
            || mode == WIFIMODE_STATION_SOFTAP)) {
        return false;
    }

    _smutex.lock();
    set_timeout(ESP8266_CONNECT_TIMEOUT);
    bool done = _parser.send("AT+CWMODE_CUR=%d", mode)
                && _parser.recv("OK\n")
                && _parser.send("AT+CIPMUX=1")
                && _parser.recv("OK\n");
    set_timeout(); //Restore default
    _smutex.unlock();

    return done;
}

bool ESP8266::reset(void)
{
    static const int ESP8266_BOOTTIME = 10000; // [ms]
    bool done = false;

    _smutex.lock();

    unsigned long int start_time = rtos::Kernel::get_ms_count();
    _reset_done = false;
    set_timeout(ESP8266_RECV_TIMEOUT);
    for (int i = 0; i < 2; i++) {
        if (!_parser.send("AT+RST") || !_parser.recv("OK\n")) {
            tr_debug("reset(): AT+RST failed or no response.");
            continue;
        }

        while (!_reset_done) {
            _process_oob(ESP8266_RECV_TIMEOUT, true); // UART mutex claimed -> need to check for OOBs ourselves
            if (_reset_done || (rtos::Kernel::get_ms_count() - start_time >= ESP8266_BOOTTIME)) {
                break;
            }
            rtos::ThisThread::sleep_for(100);
        }

        done = _reset_done;
        if (done) {
            break;
        }
    }

    tr_debug("reset(): Done: %s.", done ? "OK" : "FAIL");

    _clear_socket_packets(ESP8266_ALL_SOCKET_IDS);
    _sock_sending_id = -1;
    set_timeout();
    _smutex.unlock();

    return done;
}

bool ESP8266::dhcp(bool enabled, int mode)
{
    //only 3 valid modes
    if (mode < 0 || mode > 2) {
        return false;
    }

    _smutex.lock();
    bool done = _parser.send("AT+CWDHCP_CUR=%d,%d", mode, enabled ? 1 : 0)
                && _parser.recv("OK\n");
    _smutex.unlock();

    return done;
}

bool ESP8266::cond_enable_tcp_passive_mode()
{
    bool done = true;

    if (FW_AT_LEAST_VERSION(_at_v.major, _at_v.minor, _at_v.patch, 0, ESP8266_AT_VERSION_TCP_PASSIVE_MODE)) {
        _smutex.lock();
        done = _parser.send("AT+CIPRECVMODE=1")
               && _parser.recv("OK\n");
        _smutex.unlock();

        _tcp_passive = done ? true : false;
    }

    return done;
}


nsapi_error_t ESP8266::connect(const char *ap, const char *passPhrase)
{
    nsapi_error_t ret = NSAPI_ERROR_OK;

    _smutex.lock();
    set_timeout(ESP8266_CONNECT_TIMEOUT);

    bool res = _parser.send("AT+CWJAP_CUR=\"%s\",\"%s\"", ap, passPhrase);
    if (!res || !_parser.recv("OK\n")) {
        if (_fail) {
            if (_connect_error == 1) {
                ret = NSAPI_ERROR_CONNECTION_TIMEOUT;
            } else if (_connect_error == 2) {
                ret = NSAPI_ERROR_AUTH_FAILURE;
            } else if (_connect_error == 3) {
                ret = NSAPI_ERROR_NO_SSID;
            } else {
                ret = NSAPI_ERROR_NO_CONNECTION;
            }
            _fail = false;
            _connect_error = 0;
        }
    }

    set_timeout();
    _smutex.unlock();

    return ret;
}

bool ESP8266::disconnect(void)
{
    _smutex.lock();
    _disconnect = true;
    bool done = _parser.send("AT+CWQAP") && _parser.recv("OK\n");
    _smutex.unlock();

    return done;
}

bool ESP8266::ip_info_print(int enable)
{
    _smutex.lock();
    _disconnect = true;
    bool done = _parser.send("AT+CIPDINFO=%d", enable) && _parser.recv("OK\n");
    _smutex.unlock();

    return done;
}


const char *ESP8266::ip_addr(void)
{
    _smutex.lock();
    set_timeout(ESP8266_CONNECT_TIMEOUT);
    if (!(_parser.send("AT+CIFSR")
            && _parser.recv("+CIFSR:STAIP,\"%15[^\"]\"", _ip_buffer)
            && _parser.recv("OK\n"))) {
        _smutex.unlock();
        return 0;
    }
    set_timeout();
    _smutex.unlock();

    return _ip_buffer;
}

const char *ESP8266::mac_addr(void)
{
    _smutex.lock();
    if (!(_parser.send("AT+CIFSR")
            && _parser.recv("+CIFSR:STAMAC,\"%17[^\"]\"", _mac_buffer)
            && _parser.recv("OK\n"))) {
        _smutex.unlock();
        return 0;
    }
    _smutex.unlock();

    return _mac_buffer;
}

const char *ESP8266::gateway()
{
    _smutex.lock();
    if (!(_parser.send("AT+CIPSTA_CUR?")
            && _parser.recv("+CIPSTA_CUR:gateway:\"%15[^\"]\"", _gateway_buffer)
            && _parser.recv("OK\n"))) {
        _smutex.unlock();
        return 0;
    }
    _smutex.unlock();

    return _gateway_buffer;
}

const char *ESP8266::netmask()
{
    _smutex.lock();
    if (!(_parser.send("AT+CIPSTA_CUR?")
            && _parser.recv("+CIPSTA_CUR:netmask:\"%15[^\"]\"", _netmask_buffer)
            && _parser.recv("OK\n"))) {
        _smutex.unlock();
        return 0;
    }
    _smutex.unlock();

    return _netmask_buffer;
}

int8_t ESP8266::rssi()
{
    int8_t rssi = 0;
    char bssid[18];

    _smutex.lock();
    set_timeout(ESP8266_CONNECT_TIMEOUT);
    if (!(_parser.send("AT+CWJAP_CUR?")
            && _parser.recv("+CWJAP_CUR:\"%*[^\"]\",\"%17[^\"]\"", bssid)
            && _parser.recv("OK\n"))) {
        _smutex.unlock();
        return 0;
    }

    set_timeout();
    _smutex.unlock();

    WiFiAccessPoint ap[1];
    _scan_r.res = ap;
    _scan_r.limit = 1;
    _scan_r.cnt = 0;

    _smutex.lock();
    set_timeout(ESP8266_CONNECT_TIMEOUT);
    if (!(_parser.send("AT+CWLAP=\"\",\"%s\",", bssid)
            && _parser.recv("OK\n"))) {
        rssi = 0;
    } else if (_scan_r.cnt == 1) {
        //All OK so read and return rssi
        rssi = ap[0].get_rssi();
    }

    _scan_r.cnt = 0;
    _scan_r.res = NULL;
    set_timeout();
    _smutex.unlock();

    return rssi;
}

int ESP8266::scan(WiFiAccessPoint *res, unsigned limit, scan_mode mode, unsigned t_max, unsigned t_min)
{
    _smutex.lock();

    // Default timeout plus time spend scanning each channel
    set_timeout(ESP8266_MISC_TIMEOUT + 13 * (t_max ? t_max : ESP8266_SCAN_TIME_MAX_DEFAULT));

    _scan_r.res = res;
    _scan_r.limit = limit;
    _scan_r.cnt = 0;

    bool ret_parse_send = true;

    if (FW_AT_LEAST_VERSION(_at_v.major, _at_v.minor, _at_v.patch, 0, ESP8266_AT_VERSION_WIFI_SCAN_CHANGE)) {
        ret_parse_send = _parser.send("AT+CWLAP=,,,%u,%u,%u", (mode == SCANMODE_ACTIVE ? 0 : 1), t_min, t_max);
    } else {
        ret_parse_send = _parser.send("AT+CWLAP");
    }

    if (!(ret_parse_send && _parser.recv("OK\n"))) {
        tr_warning("scan(): AP info parsing aborted.");
        // Lets be happy about partial success and not return NSAPI_ERROR_DEVICE_ERROR
        if (!_scan_r.cnt) {
            _scan_r.cnt = NSAPI_ERROR_DEVICE_ERROR;
        }
    }


    int cnt = _scan_r.cnt;
    _scan_r.res = NULL;

    set_timeout();
    _smutex.unlock();

    return cnt;
}

nsapi_error_t ESP8266::open_udp(int id, const char *addr, int port, int local_port, int udp_mode)
{
    static const char *type = "UDP";
    bool done = false;

    ip_info_print(1);

    _smutex.lock();

    // process OOB so that _sock_i reflects the correct state of the socket
    _process_oob(ESP8266_SEND_TIMEOUT, true);

    // Previous close() can fail with busy in sending. Usually, user will ignore the close()
    // error code and cause 'spurious close', in which case user has closed the socket but ESP8266 modem
    // hasn't yet. Because we don't know how long ESP8266 modem will trap in busy, enlarge retry count
    // or timeout in close() isn't a nice way. Here, we actively re-call close() in open() to let the modem
    // close the socket. User can re-try open() on failure. Without this active close(), open() can fail forever
    // with previous 'spurious close', unless peer closes the socket and so ESP8266 modem closes it accordingly.
    if (id >= SOCKET_COUNT) {
        _smutex.unlock();
        return NSAPI_ERROR_PARAMETER;
    } else if (_sock_i[id].open) {
        close(id);
    }

    for (int i = 0; i < 2; i++) {
        if (local_port) {
            done = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d,%d", id, type, addr, port, local_port, udp_mode);
        } else {
            done = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d", id, type, addr, port);
        }

        if (done) {
            if (!_parser.recv("OK\n")) {
                if (_sock_already) {
                    _sock_already = false; // To be raised again by OOB msg
                    done = close(id);
                    if (!done) {
                        break;
                    }
                }
                if (_error) {
                    _error = false;
                    done = false;
                }
                continue;
            }
            _sock_i[id].open = true;
            _sock_i[id].proto = NSAPI_UDP;
            break;
        }
    }
    _clear_socket_packets(id);

    _smutex.unlock();

    tr_debug("open_udp(): UDP socket %d opened: %s.", id, (_sock_i[id].open ? "true" : "false"));

    return done ? NSAPI_ERROR_OK : NSAPI_ERROR_DEVICE_ERROR;
}

nsapi_error_t ESP8266::open_tcp(int id, const char *addr, int port, int keepalive)
{
    static const char *type = "TCP";
    bool done = false;

    ip_info_print(1);

    if (!addr) {
        return NSAPI_ERROR_PARAMETER;
    }
    _smutex.lock();

    // process OOB so that _sock_i reflects the correct state of the socket
    _process_oob(ESP8266_SEND_TIMEOUT, true);

    // See the reason above with close()
    if (id >= SOCKET_COUNT) {
        _smutex.unlock();
        return NSAPI_ERROR_PARAMETER;
    } else if (_sock_i[id].open) {
        close(id);
    }

    for (int i = 0; i < 2; i++) {
        if (keepalive) {
            done = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d", id, type, addr, port, keepalive);
        } else {
            done = _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d", id, type, addr, port);
        }

        if (done) {
            if (!_parser.recv("OK\n")) {
                if (_sock_already) {
                    _sock_already = false; // To be raised again by OOB msg
                    done = close(id);
                    if (!done) {
                        break;
                    }
                }
                if (_error) {
                    _error = false;
                    done = false;
                }
                continue;
            }
            _sock_i[id].open = true;
            _sock_i[id].proto = NSAPI_TCP;
            break;
        }
    }
    _clear_socket_packets(id);

    _smutex.unlock();

    tr_debug("open_tcp: TCP socket %d opened: %s . ", id, (_sock_i[id].open ? "true" : "false"));

    return done ? NSAPI_ERROR_OK : NSAPI_ERROR_DEVICE_ERROR;
}

bool ESP8266::dns_lookup(const char *name, char *ip)
{
    _smutex.lock();
    set_timeout(ESP8266_DNS_TIMEOUT);
    bool done = _parser.send("AT+CIPDOMAIN=\"%s\"", name)
                && _parser.recv("+CIPDOMAIN:%15[^\n]\n", ip)
                && _parser.recv("OK\n");
    set_timeout();
    _smutex.unlock();

    return done;
}

nsapi_size_or_error_t ESP8266::send(int id, const void *data, uint32_t amount)
{
    if (_sock_i[id].proto == NSAPI_TCP) {
        if (_sock_sending_id >= 0 && _sock_sending_id < SOCKET_COUNT) {
            if (!_sock_i[id].send_fail) {
                tr_debug("send(): Previous packet (socket %d) was not yet ACK-ed with SEND OK.", _sock_sending_id);
                return NSAPI_ERROR_WOULD_BLOCK;
            } else {
                tr_debug("send(): Previous packet (socket %d) failed.", id);
                return NSAPI_ERROR_DEVICE_ERROR;
            }
        }
    }

    nsapi_error_t ret = NSAPI_ERROR_DEVICE_ERROR;
    int bytes_confirmed = 0;
    constexpr unsigned int send_ack_retries = 3;

    // +CIPSEND supports up to 2048 bytes at a time
    // Data stream can be truncated
    if (amount > 2048 && _sock_i[id].proto == NSAPI_TCP) {
        amount = 2048;
        // Datagram must stay intact
    } else if (amount > 2048 && _sock_i[id].proto == NSAPI_UDP) {
        tr_debug("send(): UDP datagram maximum size is 2048 .");
        return NSAPI_ERROR_PARAMETER;
    }

    _smutex.lock();
    // Mark this socket is sending. We allow only one actively sending socket because:
    // 1. ESP8266 AT packets 'SEND OK'/'SEND FAIL' are not associated with socket ID. No way to tell them.
    // 2. In original implementation, ESP8266::send() is synchronous, which implies only one actively sending socket.
    _sock_sending_id = id;
    set_timeout(ESP8266_SEND_TIMEOUT);
    _busy = false;
    _error = false;
    if (!_parser.send("AT+CIPSEND=%d,%" PRIu32, id, amount)) {
        tr_debug("send(): AT+CIPSEND failed.");
        goto END;
    }

    if (!_parser.recv(">")) {
        // This means ESP8266 hasn't even started to receive data
        tr_debug("send(): Didn't get \">\"");
        if (_sock_i[id].proto == NSAPI_TCP) {
            ret = NSAPI_ERROR_WOULD_BLOCK; // Not necessarily critical error.
        } else if (_sock_i[id].proto == NSAPI_UDP) {
            ret = NSAPI_ERROR_NO_MEMORY;
        }
        goto END;
    }

    if (_parser.write((char *)data, (int)amount) < 0) {
        tr_debug("send(): Failed to write serial data");
        // Serial is not working, serious error, reset needed.
        ret = NSAPI_ERROR_DEVICE_ERROR;
        goto END;
    }

    // The "Recv X bytes" is not documented.
    if (!_parser.recv("Recv %d bytes", &bytes_confirmed)) {
        tr_debug("send(): Bytes not confirmed.");
        if (_sock_i[id].proto == NSAPI_TCP) {
            ret = NSAPI_ERROR_WOULD_BLOCK;
        } else if (_sock_i[id].proto == NSAPI_UDP) {
            ret = NSAPI_ERROR_NO_MEMORY;
        }
    } else if (bytes_confirmed != amount && _sock_i[id].proto == NSAPI_UDP) {
        tr_debug("send(): Error: confirmed %d bytes, but expected %d.", bytes_confirmed, amount);
        ret = NSAPI_ERROR_DEVICE_ERROR;
    } else {
        // TCP can accept partial writes (if they ever happen)
        ret = bytes_confirmed;
    }

END:
    _process_oob(ESP8266_RECV_TIMEOUT, true); // Drain USART receive register to avoid data overrun

    // error hierarchy, from low to high
    // NOTE: We cannot return NSAPI_ERROR_WOULD_BLOCK when "Recv X bytes" has reached, otherwise duplicate data send.
    if (_busy && ret < 0) {
        ret = NSAPI_ERROR_WOULD_BLOCK;
        tr_debug("send(): Modem busy.");
    }

    if (_error) {
        // FIXME: Not sure clear or not of _error. See it as device error and it can recover only via reset?
        _sock_sending_id = -1;
        ret = NSAPI_ERROR_CONNECTION_LOST;
        tr_debug("send(): Connection disrupted.");
    }

    if (_sock_i[id].send_fail) {
        _sock_sending_id = -1;
        if (_sock_i[id].proto == NSAPI_TCP) {
            ret = NSAPI_ERROR_DEVICE_ERROR;
        } else {
            ret = NSAPI_ERROR_NO_MEMORY;
        }
        tr_debug("send(): SEND FAIL received.");
    }

    if (!_sock_i[id].open && ret < 0) {
        _sock_sending_id = -1;
        ret = NSAPI_ERROR_CONNECTION_LOST;
        tr_debug("send(): Socket %d closed abruptly.", id);
    }

    set_timeout();
    _smutex.unlock();

    return ret;
}

void ESP8266::_oob_packet_hdlr()
{
    int id;
    int port;
    int amount;
    int pdu_len;

    // Get socket id
    if (!_parser.scanf(",%d,", &id)) {
        return;
    }

    if (_tcp_passive && _sock_i[id].open == true && _sock_i[id].proto == NSAPI_TCP) {
        //For TCP +IPD return only id and amount and it is independent on AT+CIPDINFO settings
        //Unfortunately no information about that in ESP manual but it has sense.
        if (_parser.recv("%d\n", &amount)) {
            _sock_i[id].tcp_data_avbl = amount;

            // notify data is available
            if (_callback) {
                _callback();
            }
        }
        return;
    } else {
        if (!(_parser.scanf("%d,", &amount)
                && _parser.scanf("%15[^,],", _ip_buffer)
                && _parser.scanf("%d:", &port))) {
            return;
        }
    }

    pdu_len = sizeof(struct packet) + amount;

    if ((_heap_usage + pdu_len) > MBED_CONF_ESP8266_SOCKET_BUFSIZE) {
        tr_debug("\"esp8266.socket-bufsize\"-limit exceeded, packet dropped");
        return;
    }

    struct packet *packet = (struct packet *)malloc(pdu_len);
    if (!packet) {
        tr_debug("_oob_packet_hdlr(): Out of memory, unable to allocate memory for packet.");
        return;
    }
    _heap_usage += pdu_len;

    packet->id = id;
    if (_sock_i[id].proto == NSAPI_UDP) {
        packet->remote_port = port;
        memcpy(packet->remote_ip, _ip_buffer, 16);
    }
    packet->len = amount;
    packet->alloc_len = amount;
    packet->next = 0;

    if (_parser.read((char *)(packet + 1), amount) < amount) {
        free(packet);
        _heap_usage -= pdu_len;
        return;
    }

    // append to packet list
    *_packets_end = packet;
    _packets_end = &packet->next;
}

void ESP8266::_process_oob(uint32_t timeout, bool all)
{
    set_timeout(timeout);
    // Poll for inbound packets
    while (_parser.process_oob() && all) {
    }
    set_timeout();
}

void ESP8266::bg_process_oob(uint32_t timeout, bool all)
{
    _smutex.lock();
    _process_oob(timeout, all);
    _smutex.unlock();
}

int32_t ESP8266::_recv_tcp_passive(int id, void *data, uint32_t amount, uint32_t timeout)
{
    int32_t ret = NSAPI_ERROR_WOULD_BLOCK;

    _smutex.lock();

    _process_oob(timeout, true);

    if (_sock_i[id].tcp_data_avbl != 0) {
        _sock_i[id].tcp_data = (char *)data;
        _sock_i[id].tcp_data_rcvd = NSAPI_ERROR_WOULD_BLOCK;
        _sock_active_id = id;

        // +CIPRECVDATA supports up to 2048 bytes at a time
        amount = amount > 2048 ? 2048 : amount;

        // NOTE: documentation v3.0 says '+CIPRECVDATA:<data_len>,' but it's not how the FW responds...
        bool done = _parser.send("AT+CIPRECVDATA=%d,%" PRIu32, id, amount)
                    && _parser.recv("OK\n");

        _sock_i[id].tcp_data = NULL;
        _sock_active_id = -1;

        if (!done) {
            goto BUSY;
        }

        // update internal variable tcp_data_avbl to reflect the remaining data
        if (_sock_i[id].tcp_data_rcvd > 0) {
            if (_sock_i[id].tcp_data_rcvd > (int32_t)amount) {
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_EBADMSG), \
                           "ESP8266::_recv_tcp_passive() too much data from modem\n");
            }
            if (_sock_i[id].tcp_data_avbl > _sock_i[id].tcp_data_rcvd) {
                _sock_i[id].tcp_data_avbl -= _sock_i[id].tcp_data_rcvd;
            } else {
                _sock_i[id].tcp_data_avbl = 0;
            }
        }

        ret = _sock_i[id].tcp_data_rcvd;
    }

    if (!_sock_i[id].open && ret == NSAPI_ERROR_WOULD_BLOCK) {
        ret = 0;
    }

    _smutex.unlock();
    return ret;

BUSY:
    _process_oob(ESP8266_RECV_TIMEOUT, true);
    if (_busy) {
        tr_debug("_recv_tcp_passive(): Modem busy.");
        ret = NSAPI_ERROR_WOULD_BLOCK;
    } else {
        tr_error("_recv_tcp_passive(): Unknown state.");
        ret = NSAPI_ERROR_DEVICE_ERROR;
    }
    _smutex.unlock();
    return ret;
}

int32_t ESP8266::recv_tcp(int id, void *data, uint32_t amount, uint32_t timeout)
{
    if (_tcp_passive) {
        return _recv_tcp_passive(id, data, amount, timeout);
    }

    _smutex.lock();

    // No flow control, drain the USART receive register ASAP to avoid data overrun
    if (_serial_rts == NC) {
        _process_oob(timeout, true);
    }

    // check if any packets are ready for us
    for (struct packet **p = &_packets; *p; p = &(*p)->next) {
        if ((*p)->id == id) {
            struct packet *q = *p;

            if (q->len <= amount) { // Return and remove full packet
                memcpy(data, q + 1, q->len);

                if (_packets_end == &(*p)->next) {
                    _packets_end = p;
                }
                *p = (*p)->next;

                _smutex.unlock();

                uint32_t pdu_len = sizeof(struct packet) + q->alloc_len;
                uint32_t len = q->len;
                free(q);
                _heap_usage -= pdu_len;
                return len;
            } else { // return only partial packet
                memcpy(data, q + 1, amount);

                q->len -= amount;
                memmove(q + 1, (uint8_t *)(q + 1) + amount, q->len);

                _smutex.unlock();
                return amount;
            }
        }
    }
    if (!_sock_i[id].open) {
        _smutex.unlock();
        return 0;
    }

    // Flow control, read from USART receive register only when no more data is buffered, and as little as possible
    if (_serial_rts != NC) {
        _process_oob(timeout, false);
    }
    _smutex.unlock();

    return NSAPI_ERROR_WOULD_BLOCK;
}

int32_t ESP8266::recv_udp(struct esp8266_socket *socket, void *data, uint32_t amount, uint32_t timeout)
{
    _smutex.lock();
    set_timeout(timeout);

    // Process OOB data since this is
    // how UDP packets are received
    _process_oob(timeout, true);

    set_timeout();

    // check if any packets are ready for us
    for (struct packet **p = &_packets; *p; p = &(*p)->next) {
        if ((*p)->id == socket->id) {
            struct packet *q = *p;

            socket->addr.set_ip_address((*p)->remote_ip);
            socket->addr.set_port((*p)->remote_port);

            // Return and remove packet (truncated if necessary)
            uint32_t len = q->len < amount ? q->len : amount;
            memcpy(data, q + 1, len);

            if (_packets_end == &(*p)->next) {
                _packets_end = p;
            }
            *p = (*p)->next;
            _smutex.unlock();

            uint32_t pdu_len = sizeof(struct packet) + q->alloc_len;
            free(q);
            _heap_usage -= pdu_len;
            return len;
        }
    }

    // Flow control, read from USART receive register only when no more data is buffered, and as little as possible
    if (_serial_rts != NC) {
        _process_oob(timeout, false);
    }

    _smutex.unlock();

    return NSAPI_ERROR_WOULD_BLOCK;
}

void ESP8266::_clear_socket_packets(int id)
{
    struct packet **p = &_packets;

    while (*p) {
        if ((*p)->id == id || id == ESP8266_ALL_SOCKET_IDS) {
            struct packet *q = *p;
            int pdu_len = sizeof(struct packet) + q->alloc_len;

            if (_packets_end == &(*p)->next) {
                _packets_end = p; // Set last packet next field/_packets
            }
            *p = (*p)->next;
            free(q);
            _heap_usage -= pdu_len;
        } else {
            // Point to last packet next field
            p = &(*p)->next;
        }
    }
    if (id == ESP8266_ALL_SOCKET_IDS) {
        for (int id = 0; id < 5; id++) {
            _sock_i[id].tcp_data_avbl = 0;
        }
    } else {
        _sock_i[id].tcp_data_avbl = 0;
    }
}

void ESP8266::_clear_socket_sending(int id)
{
    if (id == _sock_sending_id) {
        _sock_sending_id = -1;
    }
    _sock_i[id].send_fail = false;
}

bool ESP8266::close(int id)
{
    //May take a second try if device is busy
    for (unsigned i = 0; i < 2; i++) {
        _smutex.lock();
        if (_parser.send("AT+CIPCLOSE=%d", id)) {
            if (!_parser.recv("OK\n")) {
                if (_closed) { // UNLINK ERROR
                    _closed = false;
                    _sock_i[id].open = false;
                    _clear_socket_packets(id);
                    // Closed, so this socket escapes from SEND FAIL status.
                    _clear_socket_sending(id);
                    _smutex.unlock();
                    // ESP8266 has a habit that it might close a socket on its own.
                    tr_debug("close(%d): socket close OK with UNLINK ERROR", id);
                    return true;
                }
            } else {
                // _sock_i[id].open set to false with an OOB
                _clear_socket_packets(id);
                // Closed, so this socket escapes from SEND FAIL status
                _clear_socket_sending(id);
                _smutex.unlock();
                tr_debug("close(%d): socket close OK with AT+CIPCLOSE OK", id);
                return true;
            }
        }
        _smutex.unlock();
    }

    tr_debug("close(%d): socket close FAIL'ed (spurious close)", id);
    return false;
}

void ESP8266::set_timeout(uint32_t timeout_ms)
{
    _parser.set_timeout(timeout_ms);
}

bool ESP8266::readable()
{
    return _serial.FileHandle::readable();
}

bool ESP8266::writeable()
{
    return _serial.FileHandle::writable();
}

void ESP8266::sigio(Callback<void()> func)
{
    _serial.sigio(func);
    _callback = func;
}

void ESP8266::attach(Callback<void()> status_cb)
{
    _conn_stat_cb = status_cb;
}

bool ESP8266::set_sntp_config(bool enable, int timezone, const char *server0,
                              const char *server1, const char *server2)
{
    bool done = false;
    _smutex.lock();
    if ((server0 == nullptr || server0[0] == '\0')) {
        done = _parser.send("AT+CIPSNTPCFG=%d,%d",
                            enable ? 1 : 0, timezone);
    } else if ((server0 != nullptr || server0[0] != '\0')
               && (server1 == nullptr && server1[0] == '\0')) {
        done = _parser.send("AT+CIPSNTPCFG=%d,%d,%s",
                            enable ? 1 : 0, timezone, server0);
    } else if ((server0 != nullptr || server0[0] != '\0')
               && (server1 != nullptr && server1[0] != '\0')
               && (server2 == nullptr && server2[0] == '\0')) {
        done = _parser.send("AT+CIPSNTPCFG=%d,%d,%s,%s",
                            enable ? 1 : 0, timezone, server0, server1);
    } else {
        done = _parser.send("AT+CIPSNTPCFG=%d,%d,%s,%s,%s",
                            enable ? 1 : 0, timezone, server0, server1, server2);
    }
    done &= _parser.recv("OK\n");
    _smutex.unlock();
    return done;
}

bool ESP8266::get_sntp_config(bool *enable, int *timezone, char *server0,
                              char *server1, char *server2)
{
    _smutex.lock();
    unsigned int tmp;
    bool done = _parser.send("AT+CIPSNTPCFG?")
                && _parser.scanf("+CIPSNTPCFG:%d,%d,\"%32[^\"]\",\"%32[^\"]\",\"%32[^\"]\"",
                                 &tmp, timezone, server0, server1, server2)
                && _parser.recv("OK\n");
    _smutex.unlock();
    *enable = tmp ? true : false;
    return done;
}

bool ESP8266::get_sntp_time(std::tm *t)
{
    _smutex.lock();
    char buf[25]; // Thu Aug 04 14:48:05 2016 (always 24 chars + \0)
    memset(buf, 0, 25);

    bool done = _parser.send("AT+CIPSNTPTIME?")
                && _parser.scanf("+CIPSNTPTIME:%24c", &buf)
                && _parser.recv("OK\n");
    _smutex.unlock();

    if (!done) {
        return false;
    }

    char wday[4] = "\0", mon[4] = "\0";
    int mday = 0, hour = 0, min = 0, sec = 0, year = 0;
    int ret = sscanf(buf, "%s %s %d %d:%d:%d %d",
                     wday, mon, &mday, &hour, &min, &sec, &year);
    if (ret != 7) {
        tr_debug("get_sntp_time(): sscanf returned %d", ret);
        return false;
    }

    t->tm_sec = sec;
    t->tm_min = min;
    t->tm_hour = hour;
    t->tm_mday = mday;

    t->tm_wday = 0;
    if (strcmp(wday, "Mon") == 0) {
        t->tm_wday = 0;
    } else if (strcmp(wday, "Tue") == 0) {
        t->tm_wday = 1;
    } else if (strcmp(wday, "Wed") == 0) {
        t->tm_wday = 2;
    } else if (strcmp(wday, "Thu") == 0) {
        t->tm_wday = 3;
    } else if (strcmp(wday, "Fri") == 0) {
        t->tm_wday = 4;
    } else if (strcmp(wday, "Sat") == 0) {
        t->tm_wday = 5;
    } else if (strcmp(wday, "Sun") == 0) {
        t->tm_wday = 6;
    } else {
        tr_debug("get_sntp_time(): Invalid weekday: %s", wday);
        return false;
    }

    t->tm_mon = 0;
    if (strcmp(mon, "Jan") == 0) {
        t->tm_mon = 0;
    } else if (strcmp(mon, "Feb") == 0) {
        t->tm_mon = 1;
    } else if (strcmp(mon, "Mar") == 0) {
        t->tm_mon = 2;
    } else if (strcmp(mon, "Apr") == 0) {
        t->tm_mon = 3;
    } else if (strcmp(mon, "May") == 0) {
        t->tm_mon = 4;
    } else if (strcmp(mon, "Jun") == 0) {
        t->tm_mon = 5;
    } else if (strcmp(mon, "Jul") == 0) {
        t->tm_mon = 6;
    } else if (strcmp(mon, "Aug") == 0) {
        t->tm_mon = 7;
    } else if (strcmp(mon, "Sep") == 0) {
        t->tm_mon = 8;
    } else if (strcmp(mon, "Oct") == 0) {
        t->tm_mon = 9;
    } else if (strcmp(mon, "Nov") == 0) {
        t->tm_mon = 10;
    } else if (strcmp(mon, "Dec") == 0) {
        t->tm_mon = 11;
    } else {
        tr_debug("get_sntp_time(): Invalid month: %s", mon);
        return false;
    }

    t->tm_year = (year - 1900);

    return true;
}

bool ESP8266::_recv_ap(nsapi_wifi_ap_t *ap)
{
    int sec = NSAPI_SECURITY_UNKNOWN;
    int dummy;
    int ret;

    if (FW_AT_LEAST_VERSION(_at_v.major, _at_v.minor, _at_v.patch, 0, ESP8266_AT_VERSION_WIFI_SCAN_CHANGE)) {
        ret = _parser.scanf("(%d,\"%32[^\"]\",%hhd,\"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\",%hhu,%d,%d,%d,%d,%d,%d)\n",
                            &sec,
                            ap->ssid,
                            &ap->rssi,
                            &ap->bssid[0], &ap->bssid[1], &ap->bssid[2], &ap->bssid[3], &ap->bssid[4], &ap->bssid[5],
                            &ap->channel,
                            &dummy,
                            &dummy,
                            &dummy,
                            &dummy,
                            &dummy,
                            &dummy);
    } else {
        ret = _parser.scanf("(%d,\"%32[^\"]\",%hhd,\"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx\",%hhu,%d,%d)\n",
                            &sec,
                            ap->ssid,
                            &ap->rssi,
                            &ap->bssid[0], &ap->bssid[1], &ap->bssid[2], &ap->bssid[3], &ap->bssid[4], &ap->bssid[5],
                            &ap->channel,
                            &dummy,
                            &dummy);
    }

    if (ret < 0) {
        _parser.abort();
        tr_warning("_recv_ap(): AP info missing.");
    }

    ap->security = sec < 5 ? (nsapi_security_t)sec : NSAPI_SECURITY_UNKNOWN;

    return ret < 0 ? false : true;
}

void ESP8266::_oob_watchdog_reset()
{
    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ETIME), \
               "_oob_watchdog_reset() modem watchdog reset triggered\n");
}

void ESP8266::_oob_ready()
{
    _reset_done = true;

    for (int i = 0; i < SOCKET_COUNT; i++) {
        _sock_i[i].open = false;
    }

    // Makes possible to reinitialize
    _conn_status = NSAPI_STATUS_ERROR_UNSUPPORTED;
    _conn_stat_cb();

    tr_debug("_oob_reset(): Reset detected.");
}

void ESP8266::_oob_busy()
{
    char status;
    if (_parser.scanf("%c...\n", &status)) {
        if (status == 's') {
            tr_debug("_oob_busy(): Busy s...");
        } else if (status == 'p') {
            tr_debug("_oob_busy(): Busy p...");
        } else {
            tr_error("_oob_busy(): unrecognized busy state '%c...'", status);
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_EBADMSG), \
                       "ESP8266::_oob_busy() unrecognized busy state\n");
        }
    } else {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_ENOMSG), \
                   "ESP8266::_oob_busy() AT timeout\n");
    }
    _busy = true;
}

void ESP8266::_oob_tcp_data_hdlr()
{
    int32_t len;

    MBED_ASSERT(_sock_active_id >= 0 && _sock_active_id < 5);

    if (!_parser.scanf("%" SCNd32 ":", &len)) {
        return;
    }

    if (_parser.read(_sock_i[_sock_active_id].tcp_data, len) == -1) {
        return;
    }

    _sock_i[_sock_active_id].tcp_data_rcvd = len;
}

void ESP8266::_oob_scan_results()
{
    nsapi_wifi_ap_t ap;

    if (_recv_ap(&ap)) {
        if (_scan_r.res && _scan_r.cnt < _scan_r.limit) {
            _scan_r.res[_scan_r.cnt] = WiFiAccessPoint(ap);
        }

        _scan_r.cnt++;
    }
}

void ESP8266::_oob_connect_err()
{
    _fail = false;
    _connect_error = 0;

    if (_parser.scanf("%d", &_connect_error) && _parser.scanf("FAIL")) {
        _fail = true;
        _parser.abort();
    }
}


void ESP8266::_oob_conn_already()
{
    _sock_already = true;
    _parser.abort();
}

void ESP8266::_oob_err()
{
    _error = true;
    _parser.abort();
}

void ESP8266::_oob_socket_close_err()
{
    if (_error) {
        _error = false;
    }
    _closed = true; // Not possible to pinpoint to a certain socket
}

void ESP8266::_oob_socket0_closed()
{
    static const int id = 0;
    _sock_i[id].open = false;
    // Closed, so this socket escapes from SEND FAIL status
    _clear_socket_sending(id);
    tr_debug("_oob_socket0_closed(): Socket %d closed.", id);
}

void ESP8266::_oob_socket1_closed()
{
    static const int id = 1;
    _sock_i[id].open = false;
    // Closed, so this socket escapes from SEND FAIL status
    _clear_socket_sending(id);
    tr_debug("_oob_socket1_closed(): Socket %d closed.", id);
}

void ESP8266::_oob_socket2_closed()
{
    static const int id = 2;
    _sock_i[id].open = false;
    _clear_socket_sending(id);
    tr_debug("_oob_socket2_closed(): Socket %d closed.", id);
}

void ESP8266::_oob_socket3_closed()
{
    static const int id = 3;
    _sock_i[id].open = false;
    _clear_socket_sending(id);
    tr_debug("_oob_socket3_closed(): %d closed.", id);
}

void ESP8266::_oob_socket4_closed()
{
    static const int id = 4;
    _sock_i[id].open = false;
    // Closed, so this socket escapes from SEND FAIL status
    _clear_socket_sending(id);
    tr_debug("_oob_socket0_closed(): Socket %d closed.", id);
}

void ESP8266::_oob_connection_status()
{
    char status[13];
    if (_parser.recv("%12[^\"]\n", status)) {
        if (strcmp(status, "GOT IP\n") == 0) {
            _conn_status = NSAPI_STATUS_GLOBAL_UP;
        } else if (strcmp(status, "DISCONNECT\n") == 0) {
            if (_disconnect) {
                _conn_status = NSAPI_STATUS_DISCONNECTED;
                _disconnect = false;
            } else {
                _conn_status = NSAPI_STATUS_CONNECTING;
            }
        } else if (strcmp(status, "CONNECTED\n") == 0) {
            _conn_status = NSAPI_STATUS_CONNECTING;
        } else {
            tr_error("_oob_connection_status(): Invalid AT cmd \'%s\' .", status);
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_EBADMSG), \
                       "ESP8266::_oob_connection_status: invalid AT cmd\n");
        }
    } else {
        tr_error("_oob_connection_status(): Network status timeout, disconnecting.");
        if (!disconnect()) {
            tr_warning("_oob_connection_status(): Driver initiated disconnect failed.");
        } else {
            tr_debug("_oob_connection_status(): Disconnected.");
        }
        _conn_status = NSAPI_STATUS_ERROR_UNSUPPORTED;
    }

    MBED_ASSERT(_conn_stat_cb);
    _conn_stat_cb();
}

void ESP8266::_oob_send_ok_received()
{
    tr_debug("_oob_send_ok_received called for socket %d", _sock_sending_id);
    _sock_sending_id = -1;
}

void ESP8266::_oob_send_fail_received()
{
    tr_debug("_oob_send_fail_received called for socket %d", _sock_sending_id);
    if (_sock_sending_id >= 0 && _sock_sending_id < SOCKET_COUNT) {
        _sock_i[_sock_sending_id].send_fail = true;
    }
    _sock_sending_id = -1;
}

int8_t ESP8266::default_wifi_mode()
{
    int8_t mode;

    _smutex.lock();
    if (_parser.send("AT+CWMODE_DEF?")
            && _parser.recv("+CWMODE_DEF:%hhd", &mode)
            && _parser.recv("OK\n")) {
        _smutex.unlock();
        return mode;
    }
    _smutex.unlock();

    return 0;
}

void ESP8266::flush()
{
    _smutex.lock();
    _parser.flush();
    _smutex.unlock();
}

bool ESP8266::set_default_wifi_mode(const int8_t mode)
{
    _smutex.lock();
    bool done = _parser.send("AT+CWMODE_DEF=%hhd", mode)
                && _parser.recv("OK\n");
    _smutex.unlock();

    return done;
}

nsapi_connection_status_t ESP8266::connection_status() const
{
    return _conn_status;
}

bool ESP8266::set_country_code_policy(bool track_ap, const char *country_code, int channel_start, int channels)
{
    if (!(FW_AT_LEAST_VERSION(_at_v.major, _at_v.minor, _at_v.patch, 0, ESP8266_AT_VERSION_WIFI_SCAN_CHANGE))) {
        return true;
    }

    int t_ap = track_ap ? 0 : 1;

    _smutex.lock();
    bool done = _parser.send("AT+CWCOUNTRY_DEF=%d,\"%s\",%d,%d", t_ap, country_code, channel_start, channels)
                && _parser.recv("OK\n");

    if (!done) {
        tr_error("\"AT+CWCOUNTRY_DEF=%d,\"%s\",%d,%d\" - FAIL", t_ap, country_code, channel_start, channels);
    }

    done &= _parser.send("AT+CWCOUNTRY_CUR=%d,\"%s\",%d,%d", t_ap, country_code, channel_start, channels)
            && _parser.recv("OK\n");

    if (!done) {
        tr_error("\"AT+CWCOUNTRY_CUR=%d,\"%s\",%d,%d\" - FAIL", t_ap, country_code, channel_start, channels);
    }

    _smutex.unlock();

    return done;
}

int ESP8266::uart_enable_input(bool enabled)
{
    return _serial.enable_input(enabled);
}

#endif
