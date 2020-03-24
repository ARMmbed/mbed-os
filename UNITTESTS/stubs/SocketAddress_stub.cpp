/*
 * Copyright (c) , Arm Limited and affiliates.
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


SocketAddress::SocketAddress(const nsapi_addr_t &addr, uint16_t port)
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

bool SocketAddress::set_ip_address(const char *addr)
{
    return false;
}

void SocketAddress::set_ip_bytes(const void *bytes, nsapi_version_t version)
{
}

void SocketAddress::set_addr(const nsapi_addr_t &addr)
{
}

const char *SocketAddress::get_ip_address() const
{
    return NULL;
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
