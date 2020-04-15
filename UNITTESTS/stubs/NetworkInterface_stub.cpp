/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "netsocket/NetworkInterface.h"
#include "netsocket/NetworkStack.h"
#include <string.h>


// Default network-interface state
const char *NetworkInterface::get_mac_address()
{
    return 0;
}

nsapi_error_t NetworkInterface::get_ip_address(SocketAddress *)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::get_ipv6_link_local_address(SocketAddress *address)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::get_netmask(SocketAddress *)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::get_gateway(SocketAddress *)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::set_dhcp(bool dhcp)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

// DNS operations go through the underlying stack by default
nsapi_error_t NetworkInterface::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_value_or_error_t NetworkInterface::getaddrinfo(const char *hostname, SocketAddress *hints, SocketAddress **res, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::add_dns_server(const SocketAddress &address, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::get_dns_server(int index, SocketAddress *address, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

void NetworkInterface::attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{

}

nsapi_connection_status_t NetworkInterface::get_connection_status() const
{
    return NSAPI_STATUS_LOCAL_UP;
}

nsapi_error_t NetworkInterface::set_blocking(bool blocking)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_value_or_error_t NetworkInterface::gethostbyname_async(char const *, mbed::Callback<void (int, SocketAddress *)>, nsapi_version, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_value_or_error_t NetworkInterface::getaddrinfo_async(const char *hostname, SocketAddress *hints, hostbyname_cb_t callback, const char *interface_name)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::gethostbyname_async_cancel(int id)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

void NetworkInterface::set_as_default()
{
}

char *NetworkInterface::get_interface_name(char *interface_name)
{
    return NULL;
}

NetworkInterface::~NetworkInterface()
{
}
