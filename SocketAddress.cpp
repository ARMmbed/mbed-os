/* Socket
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

#include "SocketAddress.h"
#include "NetworkInterface.h"
#include <string.h>
#include "mbed.h"

static bool address_is_ipv4(const char *addr)
{
    int i = 0;

    // Check each digit for [0-9.]
    for (; addr[i]; i++) {
        if (!(addr[i] >= '0' && addr[i] <= '9') && addr[i] != '.') {
            return false;
        }
    }

    // Ending with '.' garuntees host
    if (i > 0 && addr[i-1] == '.') {
        return false;
    }

    return true;
}

static bool address_is_ipv6(const char *addr)
{
    // Check each digit for [0-9a-fA-F:]
    for (int i = 0; addr[i]; i++) {
        if (!(addr[i] >= '0' && addr[i] <= '9') &&
            !(addr[i] >= 'a' && addr[i] <= 'f') &&
            !(addr[i] >= 'A' && addr[i] <= 'F') &&
            addr[i] != ':') {
            return false;
        }
    }

    return true;
}

static void address_to_ipv4(uint8_t *bytes, const char *addr)
{
    sscanf(addr, "%hhd.%hhd.%hhd.%hhd", &bytes[0], &bytes[1], &bytes[2], &bytes[3]);
}

static void address_to_ipv6(uint8_t *bytes, const char *addr)
{
    // TODO support short form (::1, 2001::ffee:100a)
    // Use a more intellegent algorithm
    uint16_t shorts[NSAPI_IPv6_BYTES/2];
    sscanf(addr, "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
            &shorts[0], &shorts[1], &shorts[2], &shorts[3],
            &shorts[4], &shorts[5], &shorts[6], &shorts[7]);

    for (int i = 0; i < NSAPI_IPv6_BYTES/2; i++) {
        bytes[2*i+0] = (uint8_t)(shorts[i] >> 8);
        bytes[2*i+1] = (uint8_t)(shorts[i] >> 0);
    }
}

static void ipv4_to_address(char *addr, const uint8_t *bytes)
{
    sprintf(addr, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

static void ipv6_to_address(char *addr, const uint8_t *bytes)
{
    for (int i = 0; i < NSAPI_IPv6_BYTES; i+=2) {
        sprintf(&addr[5*i], "%02x%02x", bytes[i], bytes[i+1]);
        addr[5*i+4] = ':';
    }
    addr[NSAPI_IPv6_BYTES-1] = '\0';
}

SocketAddress::SocketAddress(NetworkInterface *iface, const char *host, uint16_t port)
{
    // Check for valid IP addresses
    if (host && address_is_ipv4(host)) {
        _ip_version = NSAPI_IPv4;
        address_to_ipv4(_ip_bytes, host);
    } else if (host && address_is_ipv6(host)) {
        _ip_version = NSAPI_IPv6;
        address_to_ipv4(_ip_bytes, host);
    } else {
        // DNS lookup
        int err = iface->gethostbyname(this, host);
        if (!err) {
            set_port(port);
        } else {
            _ip_version = NSAPI_IPv4;
            memset(_ip_bytes, 0, NSAPI_IPv4_BYTES);
            set_port(0);
        }
    }
}

SocketAddress::SocketAddress(const char *addr, uint16_t port)
{
    set_ip_address(addr);
    set_port(port);
}

SocketAddress::SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port)
{
    set_ip_bytes(bytes, version);
    set_port(port);
}

SocketAddress::SocketAddress(const SocketAddress &addr)
{
    set_ip_bytes(addr.get_ip_bytes(), addr.get_ip_version());
    set_port(addr.get_port());
}

void SocketAddress::set_ip_address(const char *addr)
{
    _ip_address[0] = '\0';

    if (addr && address_is_ipv4(addr)) {
        _ip_version = NSAPI_IPv4;
        address_to_ipv4(_ip_bytes, addr);
    } else if (addr && address_is_ipv6(addr)) {
        _ip_version = NSAPI_IPv6;
        address_to_ipv4(_ip_bytes, addr);
    } else {
        _ip_version = NSAPI_IPv4;
        memset(_ip_bytes, 0, NSAPI_IPv4_BYTES);
    }
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
    _ip_address[0] = '\0';

    if (_ip_version == NSAPI_IPv4) {
        _ip_version = NSAPI_IPv4;
        memcpy(_ip_bytes, bytes, NSAPI_IPv4_BYTES);
    } else if (_ip_version == NSAPI_IPv6) {
        _ip_version = NSAPI_IPv6;
        memcpy(_ip_bytes, bytes, NSAPI_IPv6_BYTES);
    } else {
        _ip_version = NSAPI_IPv4;
        memset(_ip_bytes, 0, NSAPI_IPv4_BYTES);
    }
}

void SocketAddress::set_port(uint16_t port)
{
    _port = port;
}

const char *SocketAddress::get_ip_address() const
{
    char *ip_address = (char *)_ip_address;

    if (!ip_address[0]) {
        if (_ip_version == NSAPI_IPv4) {
            ipv4_to_address(ip_address, _ip_bytes);
        } else if (_ip_version == NSAPI_IPv4) {
            ipv6_to_address(ip_address, _ip_bytes);
        }
    }

    return ip_address;
}

const void *SocketAddress::get_ip_bytes() const
{
    return _ip_bytes;
}

nsapi_version_t SocketAddress::get_ip_version() const
{
    return _ip_version;
}

uint16_t SocketAddress::get_port() const
{
    return _port;
}

SocketAddress::operator bool() const
{
    int count = 0;
    if (_ip_version == NSAPI_IPv4) {
        count = NSAPI_IPv4_BYTES;
    } else if (_ip_version == NSAPI_IPv6) {
        count = NSAPI_IPv6_BYTES;
    }

    for (int i = 0; i < count; i++) {
        if (_ip_bytes[i]) {
            return true;
        }
    }

    return false;
}
