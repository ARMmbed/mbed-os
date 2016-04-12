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

SocketAddress::SocketAddress(NetworkInterface *iface, const char *host, uint16_t port)
{
    int err = iface->gethostbyname(host, _ip_address);
    set_port(port);

    if (err) {
        _ip_address[0] = '\0';
        _port = 0;
    }
}

SocketAddress::SocketAddress(const char *addr, uint16_t port)
{
    set_ip_address(addr);
    set_port(port);
}

SocketAddress::SocketAddress(const SocketAddress &addr)
{
    set_ip_address(addr.get_ip_address());
    set_port(addr.get_port());
}

void SocketAddress::set_ip_address(const char *addr)
{
    strncpy(_ip_address, addr, sizeof _ip_address);
    _ip_address[sizeof _ip_address - 1] = '\0';
}

void SocketAddress::set_port(uint16_t port)
{
    _port = port;
}

const char *SocketAddress::get_ip_address() const
{
    if (!_ip_address[0]) {
        return 0;
    }
    return _ip_address;
}

uint16_t SocketAddress::get_port() const
{
    return _port;
}
