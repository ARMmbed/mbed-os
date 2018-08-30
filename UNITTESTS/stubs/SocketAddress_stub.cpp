/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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

#include "SocketAddress.h"
#include "NetworkInterface.h"
#include "NetworkStack.h"
#include <string.h>
#include "mbed.h"


static bool ipv4_is_valid(const char *addr)
{
    return false;
}

static bool ipv6_is_valid(const char *addr)
{
    return false;
}

static void ipv4_from_address(uint8_t *bytes, const char *addr)
{

}

static int ipv6_scan_chunk(uint16_t *shorts, const char *chunk)
{
    return 0;
}

static void ipv6_from_address(uint8_t *bytes, const char *addr)
{

}

static void ipv4_to_address(char *addr, const uint8_t *bytes)
{

}

static void ipv6_to_address(char *addr, const uint8_t *bytes)
{
}


SocketAddress::SocketAddress(nsapi_addr_t addr, uint16_t port)
{
}

SocketAddress::SocketAddress(const char *addr, uint16_t port)
{
}

SocketAddress::SocketAddress(const void *bytes, nsapi_version_t version, uint16_t port)
{
}

SocketAddress::SocketAddress(const SocketAddress &addr)
{
}

SocketAddress::~SocketAddress()
{
}

bool SocketAddress::set_ip_address(const char *addr)
{
    return false;
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
}

void SocketAddress::set_addr(nsapi_addr_t addr)
{
}

void SocketAddress::set_port(uint16_t port)
{
}

const char *SocketAddress::get_ip_address() const
{
    return NULL;
}

const void *SocketAddress::get_ip_bytes() const
{
    return NULL;
}

nsapi_version_t SocketAddress::get_ip_version() const
{
    nsapi_version_t ver = NSAPI_IPv6;
    return ver;
}

nsapi_addr_t SocketAddress::get_addr() const
{
    nsapi_addr_t addr;
    addr.version = NSAPI_IPv6;
    return _addr;
}

uint16_t SocketAddress::get_port() const
{
    return 0;
}

SocketAddress::operator bool() const
{
    return false;
}

SocketAddress &SocketAddress::operator=(const SocketAddress &addr)
{
    set_addr(addr.get_addr());
    set_port(addr.get_port());
    return *this;
}

bool operator==(const SocketAddress &a, const SocketAddress &b)
{
    return false;
}

bool operator!=(const SocketAddress &a, const SocketAddress &b)
{
    return false;
}
