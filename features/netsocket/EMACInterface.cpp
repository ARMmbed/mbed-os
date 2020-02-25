/* LWIP implementation of NetworkInterfaceAPI
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

#include "EMACInterface.h"

using namespace mbed;

/* Interface implementation */
EMACInterface::EMACInterface(EMAC &emac, OnboardNetworkStack &stack) :
    _emac(emac),
    _stack(stack),
    _interface(NULL),
    _dhcp(true),
    _blocking(true),
    _ip_address(),
    _netmask(),
    _gateway()
{
}

nsapi_error_t EMACInterface::set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway)
{
    _dhcp = false;

    // Don't check return values, so user can clear the addresses by passing empty strings.
    _ip_address = ip_address;
    _netmask = netmask;
    _gateway = gateway;

    return NSAPI_ERROR_OK;
}

nsapi_error_t EMACInterface::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;
    return NSAPI_ERROR_OK;
}

nsapi_error_t EMACInterface::connect()
{
    if (!_interface) {
        nsapi_error_t err = _stack.add_ethernet_interface(_emac, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
    }

    return _interface->bringup(_dhcp,
                               _ip_address.get_ip_address() ? _ip_address.get_ip_address() : 0,
                               _netmask.get_ip_address() ? _netmask.get_ip_address() : 0,
                               _gateway.get_ip_address() ? _gateway.get_ip_address() : 0,
                               DEFAULT_STACK,
                               _blocking);
}

nsapi_error_t EMACInterface::disconnect()
{
    if (_interface) {
        return _interface->bringdown();
    }
    return NSAPI_ERROR_NO_CONNECTION;
}

const char *EMACInterface::get_mac_address()
{
    if (_interface && _interface->get_mac_address(_mac_address, sizeof(_mac_address))) {
        return _mac_address;
    }
    return nullptr;
}

nsapi_error_t EMACInterface::get_ip_address(SocketAddress *address)
{
    if (_interface && _interface->get_ip_address(address) == NSAPI_ERROR_OK) {
        _ip_address = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t EMACInterface::get_ipv6_link_local_address(SocketAddress *address)
{
    if (_interface) {
        return _interface->get_ipv6_link_local_address(address);
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t EMACInterface::get_netmask(SocketAddress *address)
{
    if (_interface && _interface->get_netmask(address) == NSAPI_ERROR_OK) {
        _netmask = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t EMACInterface::get_gateway(SocketAddress *address)
{
    if (_interface && _interface->get_gateway(address) == NSAPI_ERROR_OK) {
        _gateway = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

char *EMACInterface::get_interface_name(char *interface_name)
{
    if (_interface) {
        return _interface->get_interface_name(interface_name);
    }

    return NULL;
}

void EMACInterface::set_as_default()
{
    if (_interface) {
        _stack.set_default_interface(_interface);
    }
}

NetworkStack *EMACInterface::get_stack()
{
    return &_stack;
}

void EMACInterface::attach(
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
    if (_interface) {
        _interface->attach(status_cb);
    }
}

nsapi_connection_status_t EMACInterface::get_connection_status() const
{
    if (_interface) {
        return _interface->get_connection_status();
    } else {
        return NSAPI_STATUS_DISCONNECTED;
    }
}

nsapi_error_t EMACInterface::set_blocking(bool blocking)
{
    _blocking = blocking;
    return NSAPI_ERROR_OK;
}
