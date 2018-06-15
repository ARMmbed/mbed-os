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

#include "netsocket/NetworkInterface.h"
#include "netsocket/NetworkStack.h"
#include <string.h>


// Default network-interface state
const char *NetworkInterface::get_mac_address()
{
    return 0;
}

const char *NetworkInterface::get_ip_address()
{
    return 0;
}

const char *NetworkInterface::get_netmask()
{
    return 0;
}

const char *NetworkInterface::get_gateway()
{
    return 0;
}

nsapi_error_t NetworkInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::set_dhcp(bool dhcp)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

// DNS operations go through the underlying stack by default
nsapi_error_t NetworkInterface::gethostbyname(const char *name, SocketAddress *address, nsapi_version_t version)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::add_dns_server(const SocketAddress &address)
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

nsapi_value_or_error_t NetworkInterface::gethostbyname_async(char const*, mbed::Callback<void (int, SocketAddress*)>, nsapi_version)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t NetworkInterface::gethostbyname_async_cancel(int id)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

