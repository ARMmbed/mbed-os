/*
 * Copyright (c) 2019 ARM Limited
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

#include "PPPInterface.h"

using namespace mbed;

/* Interface implementation */
PPPInterface::PPPInterface(PPP &ppp, OnboardNetworkStack &stack) :
    _ppp(ppp),
    _stack(stack),
    _interface(NULL),
    _blocking(true),
    _ip_address(),
    _netmask(),
    _gateway(),
    _stream(NULL),
    _ip_stack(DEFAULT_STACK),
    _uname(NULL),
    _password(NULL)
{
}

PPPInterface::~PPPInterface()
{
    _stack.remove_ppp_interface(&_interface);
}

nsapi_error_t PPPInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    strncpy(_ip_address, ip_address ? ip_address : "", sizeof(_ip_address));
    _ip_address[sizeof(_ip_address) - 1] = '\0';
    strncpy(_netmask, netmask ? netmask : "", sizeof(_netmask));
    _netmask[sizeof(_netmask) - 1] = '\0';
    strncpy(_gateway, gateway ? gateway : "", sizeof(_gateway));
    _gateway[sizeof(_gateway) - 1] = '\0';

    return NSAPI_ERROR_OK;
}

nsapi_error_t PPPInterface::connect()
{
    _ppp.set_stream(_stream);
    _ppp.set_ip_stack(_ip_stack);
    _ppp.set_credentials(_uname, _password);

    if (!_interface) {
        nsapi_error_t err = _stack.add_ppp_interface(_ppp, true, &_interface);
        if (err != NSAPI_ERROR_OK) {
            _interface = NULL;
            return err;
        }
        _interface->attach(_connection_status_cb);
    }

    return _interface->bringup(false,
                               _ip_address[0] ? _ip_address : 0,
                               _netmask[0] ? _netmask : 0,
                               _gateway[0] ? _gateway : 0,
                               _ip_stack,
                               _blocking);
}

nsapi_error_t PPPInterface::disconnect()
{
    if (_interface) {
        return _interface->bringdown();
    }
    return NSAPI_ERROR_OK;
}

const char *PPPInterface::get_ip_address()
{
    if (_interface && _interface->get_ip_address(_ip_address, sizeof(_ip_address))) {
        return _ip_address;
    }

    return NULL;
}

const char *PPPInterface::get_netmask()
{
    if (_interface && _interface->get_netmask(_netmask, sizeof(_netmask))) {
        return _netmask;
    }

    return 0;
}

const char *PPPInterface::get_gateway()
{
    if (_interface && _interface->get_gateway(_gateway, sizeof(_gateway))) {
        return _gateway;
    }

    return 0;
}

char *PPPInterface::get_interface_name(char *interface_name)
{
    if (_interface) {
        return _interface->get_interface_name(interface_name);
    }

    return NULL;
}

void PPPInterface::set_as_default()
{
    if (_interface) {
        _stack.set_default_interface(_interface);
    }
}

void PPPInterface::set_stream(mbed::FileHandle *stream)
{
    _stream = stream;
}

void PPPInterface::set_ip_stack(nsapi_ip_stack_t ip_stack)
{

    _ip_stack = ip_stack;
}

void PPPInterface::set_credentials(const char *uname, const char *password)
{
    _uname = uname;
    _password = password;
}

NetworkStack *PPPInterface::get_stack()
{
    return &_stack;
}

void PPPInterface::attach(
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
    if (_interface) {
        _interface->attach(status_cb);
    }
}

nsapi_connection_status_t PPPInterface::get_connection_status() const
{
    if (_interface) {
        return _interface->get_connection_status();
    } else {
        return NSAPI_STATUS_DISCONNECTED;
    }
}

nsapi_error_t PPPInterface::set_blocking(bool blocking)
{
    _blocking = blocking;
    return NSAPI_ERROR_OK;
}

