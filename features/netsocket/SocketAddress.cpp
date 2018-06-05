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
#include "NetworkStack.h"
#include <string.h>
#include "ip6string.h"
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
    // Must also have at least 2 colons
    int colons = 0;
    for (int i = 0; addr[i]; i++) {
        if (!(addr[i] >= '0' && addr[i] <= '9') &&
            !(addr[i] >= 'a' && addr[i] <= 'f') &&
            !(addr[i] >= 'A' && addr[i] <= 'F') &&
            addr[i] != ':') {
            return false;
        }
        if (addr[i] == ':') {
            colons++;
        }
    }

    return colons >= 2;
}

static void ipv4_from_address(uint8_t *bytes, const char *addr)
{
    int count = 0;
    int i = 0;

    for (; count < NSAPI_IPv4_BYTES; count++) {
        unsigned d;
        // Not using %hh, since it might be missing in newlib-based toolchains.
        // See also: https://git.io/vxiw5
        int scanned = sscanf(&addr[i], "%u", &d);
        if (scanned < 1) {
            return;
        }

        bytes[count] = static_cast<uint8_t>(d);

        for (; addr[i] != '.'; i++) {
            if (!addr[i]) {
                return;
            }
        }

        i++;
    }
}

static void ipv6_from_address(uint8_t *bytes, const char *addr)
{
    stoip6(addr, strlen(addr), bytes);
}

static void ipv4_to_address(char *addr, const uint8_t *bytes)
{
    sprintf(addr, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
}

static void ipv6_to_address(char *addr, const uint8_t *bytes)
{
    ip6tos(bytes, addr);
}


SocketAddress::SocketAddress(nsapi_addr_t addr, uint16_t port)
{
    _ip_address[0] = '\0';
    set_addr(addr);
    set_port(port);
}

SocketAddress::SocketAddress(const char *addr, uint16_t port)
{
    _ip_address[0] = '\0';
    set_ip_address(addr);
    set_port(port);
}

SocketAddress::SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port)
{
    _ip_address[0] = '\0';
    set_ip_bytes(bytes, version);
    set_port(port);
}

SocketAddress::SocketAddress(const SocketAddress &addr)
{
    _ip_address[0] = '\0';
    set_addr(addr.get_addr());
    set_port(addr.get_port());
}

bool SocketAddress::set_ip_address(const char *addr)
{
    _ip_address[0] = '\0';

    if (addr && ipv4_is_valid(addr)) {
        _addr.version = NSAPI_IPv4;
        ipv4_from_address(_addr.bytes, addr);
        return true;
    } else if (addr && ipv6_is_valid(addr)) {
        _addr.version = NSAPI_IPv6;
        ipv6_from_address(_addr.bytes, addr);
        return true;
    } else {
        _addr = nsapi_addr_t();
        return false;
    }
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
    nsapi_addr_t addr;

    addr = nsapi_addr_t();
    addr.version = version;
    if (version == NSAPI_IPv6) {
        memcpy(addr.bytes, bytes, NSAPI_IPv6_BYTES);
    } else if (version == NSAPI_IPv4) {
        memcpy(addr.bytes, bytes, NSAPI_IPv4_BYTES);
    }
    set_addr(addr);
}

void SocketAddress::set_addr(nsapi_addr_t addr)
{
    _ip_address[0] = '\0';
    _addr = addr;
}

void SocketAddress::set_port(uint16_t port)
{
    _port = port;
}

const char *SocketAddress::get_ip_address() const
{
    if (_addr.version == NSAPI_UNSPEC) {
        return NULL;
    }

    if (!_ip_address[0]) {
        if (_addr.version == NSAPI_IPv4) {
            ipv4_to_address(_ip_address, _addr.bytes);
        } else if (_addr.version == NSAPI_IPv6) {
            ipv6_to_address(_ip_address, _addr.bytes);
        }
    }

    return _ip_address;
}

const void *SocketAddress::get_ip_bytes() const
{
    return _addr.bytes;
}

nsapi_version_t SocketAddress::get_ip_version() const
{
    return _addr.version;
}

nsapi_addr_t SocketAddress::get_addr() const
{
    return _addr;
}

uint16_t SocketAddress::get_port() const
{
    return _port;
}

SocketAddress::operator bool() const
{
    if (_addr.version == NSAPI_IPv4) {
        for (int i = 0; i < NSAPI_IPv4_BYTES; i++) {
            if (_addr.bytes[i]) {
                return true;
            }
        }

        return false;
    } else if (_addr.version == NSAPI_IPv6) {
        for (int i = 0; i < NSAPI_IPv6_BYTES; i++) {
            if (_addr.bytes[i]) {
                return true;
            }
        }

        return false;
    } else {
        return false;
    }
}

bool operator==(const SocketAddress &a, const SocketAddress &b)
{
    if (!a && !b) {
        return true;
    } else if (a._addr.version != b._addr.version) {
        return false;
    } else if (a._addr.version == NSAPI_IPv4) {
        return memcmp(a._addr.bytes, b._addr.bytes, NSAPI_IPv4_BYTES) == 0;
    } else if (a._addr.version == NSAPI_IPv6) {
        return memcmp(a._addr.bytes, b._addr.bytes, NSAPI_IPv6_BYTES) == 0;
    }

    MBED_UNREACHABLE;
}

bool operator!=(const SocketAddress &a, const SocketAddress &b)
{
    return !(a == b);
}

void SocketAddress::_SocketAddress(NetworkStack *iface, const char *host, uint16_t port)
{
    _ip_address[0] = '\0';

    // gethostbyname must check for literals, so can call it directly
    int err = iface->gethostbyname(host, this);
    _port = port;
    if (err) {
        _addr = nsapi_addr_t();
        _port = 0;
    }
}
