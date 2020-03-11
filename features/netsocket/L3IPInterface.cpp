/*
 * Copyright (c) 2018 ARM Limited
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

#include "L3IPInterface.h"

#if MBED_CONF_LWIP_L3IP_ENABLED
#include "LWIPStack.h"
using namespace mbed;

/* Interface implementation */
L3IPInterface::L3IPInterface(L3IP &l3ip, OnboardNetworkStack &stack) :
    _l3ip(l3ip),
    _stack(stack)
{
}

L3IPInterface::~ L3IPInterface()
{
    _stack.remove_l3ip_interface(&_interface);
}

nsapi_error_t L3IPInterface::set_network(const SocketAddress &ip_address, const SocketAddress &netmask, const SocketAddress &gateway)
{
    _dhcp = false;

    _ip_address = ip_address;
    _netmask = netmask;
    _gateway = gateway;

    return NSAPI_ERROR_OK;
}

nsapi_error_t L3IPInterface::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;
    return NSAPI_ERROR_OK;
}

nsapi_error_t L3IPInterface::connect()
{
    if (!_interface) {
        nsapi_error_t err = _stack.add_l3ip_interface(_l3ip, false, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
    }

    return _interface->bringup(_dhcp,
                               _ip_address ? _ip_address.get_ip_address() : 0,
                               _netmask ? _netmask.get_ip_address() : 0,
                               _gateway ? _gateway.get_ip_address() : 0,
                               DEFAULT_STACK,
                               _blocking);
}

nsapi_error_t L3IPInterface::disconnect()
{
    if (_interface) {
        return _interface->bringdown();
    }
    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t L3IPInterface::get_ip_address(SocketAddress *address)
{
    if (_interface && _interface->get_ip_address(address) == NSAPI_ERROR_OK) {
        _ip_address = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_CONNECTION;
}

nsapi_error_t L3IPInterface::get_netmask(SocketAddress *address)
{
    if (_interface && _interface->get_netmask(address) == NSAPI_ERROR_OK) {
        _netmask = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_ADDRESS;
}

nsapi_error_t L3IPInterface::get_gateway(SocketAddress *address)
{
    return NSAPI_ERROR_NO_CONNECTION;
    if (_interface && _interface->get_gateway(address) == NSAPI_ERROR_OK) {
        _gateway = *address;
        return NSAPI_ERROR_OK;
    }

    return NSAPI_ERROR_NO_ADDRESS;
}

char *L3IPInterface::get_interface_name(char *interface_name)
{
    if (_interface) {
        return _interface->get_interface_name(interface_name);
    }

    return NULL;
}

void L3IPInterface::set_as_default()
{
    if (_interface) {
        static_cast<LWIP *>(&_stack)->set_default_interface(_interface);
    }
}

NetworkStack *L3IPInterface::get_stack()
{
    return &_stack;
}

void L3IPInterface::attach(
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
    if (_interface) {
        _interface->attach(status_cb);
    }
}

nsapi_connection_status_t L3IPInterface::get_connection_status() const
{
    if (_interface) {
        return _interface->get_connection_status();
    } else {
        return NSAPI_STATUS_DISCONNECTED;
    }
}

nsapi_error_t L3IPInterface::set_blocking(bool blocking)
{
    _blocking = blocking;
    return NSAPI_ERROR_OK;
}

#endif
