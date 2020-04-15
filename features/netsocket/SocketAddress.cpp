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
#include <stdio.h>
#include "ip4string.h"
#include "ip6string.h"



SocketAddress::SocketAddress(const nsapi_addr_t &addr, uint16_t port) : _addr(addr), _port(port)
{
}

SocketAddress::SocketAddress(const char *addr, uint16_t port) : _port(port)
{
    set_ip_address(addr);
}

SocketAddress::SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port) : _port(port)
{
    set_ip_bytes(bytes, version);
}

SocketAddress::SocketAddress(const SocketAddress &addr) : _addr(addr._addr), _port(addr._port)
{
}

bool SocketAddress::set_ip_address(const char *addr)
{
    _ip_address = nullptr;

    if (addr && stoip4(addr, strlen(addr), _addr.bytes)) {
        _addr.version = NSAPI_IPv4;
        return true;
    } else if (addr && stoip6(addr, strlen(addr), _addr.bytes)) {
        _addr.version = NSAPI_IPv6;
        return true;
    } else {
        _addr = nsapi_addr_t();
        return false;
    }
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
    nsapi_addr_t addr{};

    addr.version = version;
    if (version == NSAPI_IPv6) {
        memcpy(addr.bytes, bytes, NSAPI_IPv6_BYTES);
    } else if (version == NSAPI_IPv4) {
        memcpy(addr.bytes, bytes, NSAPI_IPv4_BYTES);
    }
    set_addr(addr);
}

void SocketAddress::set_addr(const nsapi_addr_t &addr)
{
    _ip_address = nullptr;
    _addr = addr;
}

const char *SocketAddress::get_ip_address() const
{
    if (_addr.version == NSAPI_UNSPEC) {
        return nullptr;
    }

    if (!_ip_address) {
        _ip_address.reset(new char[NSAPI_IP_SIZE]);
        if (_addr.version == NSAPI_IPv4) {
            ip4tos(_addr.bytes, _ip_address.get());
        } else if (_addr.version == NSAPI_IPv6) {
            ip6tos(_addr.bytes, _ip_address.get());
        }
    }

    return _ip_address.get();
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

SocketAddress &SocketAddress::operator=(const SocketAddress &addr)
{
    set_addr(addr._addr);
    set_port(addr._port);
    return *this;
}

bool operator==(const SocketAddress &a, const SocketAddress &b)
{
    if (!a && !b) {
        return true;
    } else if (a._addr.version != b._addr.version) {
        return false;
    } else if (a._port != b._port) {
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
