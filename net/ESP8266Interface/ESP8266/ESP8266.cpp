/* ESP8266 Example
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

#include "ESP8266.h"

ESP8266::ESP8266(PinName tx, PinName rx, bool debug)
    : _serial(tx, rx, 1024), _parser(_serial)
{
    _serial.baud(115200);
    _parser.debugOn(debug);
}

bool ESP8266::startup(int mode)
{
    //only 3 valid modes
    if(mode < 1 || mode > 3) {
        return false;
    }

    return reset()
        && _parser.send("AT+CWMODE=%d", mode) 
        && _parser.recv("OK")
        && _parser.send("AT+CIPMUX=1") 
        && _parser.recv("OK");
}

bool ESP8266::reset(void)
{
    for (int i = 0; i < 2; i++) {
        if (_parser.send("AT+RST")
            && _parser.recv("OK\r\nready")) {
            return true;
        }
    }

    return false;
}

bool ESP8266::dhcp(bool enabled, int mode)
{
    //only 3 valid modes
    if(mode < 0 || mode > 2) {
        return false;
    }

    return _parser.send("AT+CWDHCP=%d,%d", enabled?1:0, mode)
        && _parser.recv("OK");
}

bool ESP8266::connect(const char *ap, const char *passPhrase)
{
    return _parser.send("AT+CWJAP=\"%s\",\"%s\"", ap, passPhrase)
        && _parser.recv("OK");
}

bool ESP8266::disconnect(void)
{
    return _parser.send("AT+CWQAP") && _parser.recv("OK");
}

const char *ESP8266::getIPAddress(void)
{
    if (!(_parser.send("AT+CIFSR")
        && _parser.recv("+CIFSR:STAIP,\"%[^\"]\"", _ip_buffer)
        && _parser.recv("OK"))) {
        return 0;
    }

    return _ip_buffer;
}

const char *ESP8266::getMACAddress(void)
{
    if (!(_parser.send("AT+CIFSR")
        && _parser.recv("+CIFSR:STAMAC,\"%[^\"]\"", _mac_buffer)
        && _parser.recv("OK"))) {
        return 0;
    }

    return _mac_buffer;
}

bool ESP8266::isConnected(void)
{
    return getIPAddress() != 0;
}

bool ESP8266::open(const char *type, int id, const char* addr, int port)
{
    //IDs only 0-4
    if(id > 4) {
        return false;
    }

    return _parser.send("AT+CIPSTART=%d,\"%s\",\"%s\",%d", id, type, addr, port)
        && _parser.recv("OK");
}

bool ESP8266::send(int id, const void *data, uint32_t amount)
{
    //May take a second try if device is busy
    for (unsigned i = 0; i < 2; i++) {
        if (_parser.send("AT+CIPSEND=%d,%d", id, amount)
            && _parser.recv(">")
            && _parser.write((char*)data, (int)amount) >= 0) {
            return true;
        }
    }

    return false;
}

int32_t ESP8266::recv(int id, void *data, uint32_t amount)
{
    uint32_t recv_amount;
    int recv_id;

    if (!(_parser.recv("+IPD,%d,%d:", &recv_id, &recv_amount)
        && recv_id == id
        && recv_amount <= amount
        && _parser.read((char*)data, recv_amount)
        && _parser.recv("OK"))) {
        return -1;
    }

    return recv_amount;
}

bool ESP8266::close(int id)
{
    //May take a second try if device is busy
    for (unsigned i = 0; i < 2; i++) {
        if (_parser.send("AT+CIPCLOSE=%d", id)
            && _parser.recv("OK")) {
            return true;
        }
    }

    return false;
}

void ESP8266::setTimeout(uint32_t timeout_ms)
{
    _parser.setTimeout(timeout_ms);
}

bool ESP8266::readable()
{
    return _serial.readable();
}

bool ESP8266::writeable()
{
    return _serial.writeable();
}

void ESP8266::attach(Callback<void()> func)
{
    _serial.attach(func);
}
