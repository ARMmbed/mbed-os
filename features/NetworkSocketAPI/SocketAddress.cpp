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
#include "NetworkStack.h"
#include <string.h>
#include "mbed.h"


static bool ipv4_is_valid(const char *addr)
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

static bool ipv6_is_valid(const char *addr)
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

static void ipv4_from_address(uint8_t *bytes, const char *addr)
{
    int count = 0;
    int i = 0;

    for (; count < NSAPI_IPv4_BYTES; count++) {
        int scanned = sscanf(&addr[i], "%hhu", &bytes[count]);
        if (scanned < 1) {
            return;
        }

        for (; addr[i] != '.'; i++) {
            if (!addr[i]) {
                return;
            }
        }

        i++;
    }
}

static int ipv6_scan_chunk(uint16_t *shorts, const char *chunk) {
    int count = 0;
    int i = 0;

    for (; count < NSAPI_IPv6_BYTES/2; count++) {
        int scanned = sscanf(&chunk[i], "%hx", &shorts[count]);
        if (scanned < 1) {
            return count;
        }

        for (; chunk[i] != ':'; i++) {
            if (!chunk[i]) {
                return count+1;
            }
        }

        i++;
    }

    return count;
}

static void ipv6_from_address(uint8_t *bytes, const char *addr)
{
    // Start with zeroed address
    uint16_t shorts[NSAPI_IPv6_BYTES/2];
    memset(shorts, 0, sizeof shorts);

    int suffix = 0;

    // Find double colons and scan suffix
    for (int i = 0; addr[i]; i++) {
        if (addr[i] == ':' && addr[i+1] == ':') {
            suffix = ipv6_scan_chunk(shorts, &addr[i+2]);
            break;
        }
    }

    // Move suffix to end
    memmove(&shorts[NSAPI_IPv6_BYTES/2-suffix], &shorts[0],
            suffix*sizeof(uint16_t));

    // Scan prefix
    ipv6_scan_chunk(shorts, &addr[0]);

    // Flip bytes
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
    for (int i = 0; i < NSAPI_IPv6_BYTES/2; i++) {
        sprintf(&addr[5*i], "%02x%02x", bytes[2*i], bytes[2*i+1]);
        addr[5*i+4] = ':';
    }
    addr[NSAPI_IPv6_SIZE-1] = '\0';
}


SocketAddress::SocketAddress(NetworkStack *iface, const char *host, uint16_t port)
{
    memset(&_ip_address, 0, sizeof _ip_address);

    // Check for valid IP addresses
    if (host && ipv4_is_valid(host)) {
        _ip_version = NSAPI_IPv4;
        ipv4_from_address(_ip_bytes, host);
        set_port(port);
    } else if (host && ipv6_is_valid(host)) {
        _ip_version = NSAPI_IPv6;
        ipv6_from_address(_ip_bytes, host);
        set_port(port);
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
    memset(&_ip_address, 0, sizeof _ip_address);
    set_ip_address(addr);
    set_port(port);
}

SocketAddress::SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port)
{
    memset(&_ip_address, 0, sizeof _ip_address);
    set_ip_bytes(bytes, version);
    set_port(port);
}

SocketAddress::SocketAddress(const SocketAddress &addr)
{
    memset(&_ip_address, 0, sizeof _ip_address);
    set_ip_bytes(addr.get_ip_bytes(), addr.get_ip_version());
    set_port(addr.get_port());
}

void SocketAddress::set_ip_address(const char *addr)
{
    _ip_address[0] = '\0';

    if (addr && ipv4_is_valid(addr)) {
        _ip_version = NSAPI_IPv4;
        ipv4_from_address(_ip_bytes, addr);
    } else if (addr && ipv6_is_valid(addr)) {
        _ip_version = NSAPI_IPv6;
        ipv6_from_address(_ip_bytes, addr);
    } else {
        _ip_version = NSAPI_IPv4;
        memset(_ip_bytes, 0, NSAPI_IPv4_BYTES);
    }
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
    _ip_address[0] = '\0';

    if (version == NSAPI_IPv4) {
        _ip_version = NSAPI_IPv4;
        memcpy(_ip_bytes, bytes, NSAPI_IPv4_BYTES);
    } else if (version == NSAPI_IPv6) {
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
        } else if (_ip_version == NSAPI_IPv6) {
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
