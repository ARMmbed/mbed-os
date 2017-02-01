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

#include "EthernetInterface.h"
#include "mbed_ipstack.h"

/* Interface implementation */
EthernetInterface::EthernetInterface(emac_interface_t *emac)
    : _dhcp(true), _ip_address(), _netmask(), _gateway(), _stack()
{
    mbed_ipstack_set_stack(emac, &_stack);
}

nsapi_error_t EthernetInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    _dhcp = false;
    strncpy(_ip_address, ip_address ? ip_address : "", sizeof(_ip_address));
    strncpy(_netmask, netmask ? netmask : "", sizeof(_netmask));
    strncpy(_gateway, gateway ? gateway : "", sizeof(_gateway));
    return NSAPI_ERROR_OK;
}

nsapi_error_t EthernetInterface::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;
    return NSAPI_ERROR_OK;
}

nsapi_error_t EthernetInterface::connect()
{
    nsapi_error_t err;
    if (_stack.emac == NULL)
        return NSAPI_ERROR_UNSUPPORTED;

    mbed_ipstack_init();
    err = mbed_ipstack_add_netif(_stack.emac, true);
    if (err != NSAPI_ERROR_OK)
        return err;

    return mbed_ipstack_bringup(_stack.emac, _dhcp,
            _ip_address[0] ? _ip_address : 0,
            _netmask[0] ? _netmask : 0,
            _gateway[0] ? _gateway : 0);
}

nsapi_error_t EthernetInterface::disconnect()
{
    return mbed_ipstack_bringdown(_stack.emac);
}

const char *EthernetInterface::get_mac_address()
{
    return mbed_ipstack_get_mac_address(_stack.emac);
}

const char *EthernetInterface::get_ip_address()
{
    if (mbed_ipstack_get_ip_address(_stack.emac, _ip_address, sizeof(_ip_address))) {
        return _ip_address;
    }

    return NULL;
}

const char *EthernetInterface::get_netmask()
{
    if (mbed_ipstack_get_netmask(_stack.emac, _netmask, sizeof(_netmask))) {
        return _netmask;
    }

    return 0;
}

const char *EthernetInterface::get_gateway()
{
    if (mbed_ipstack_get_gateway(_stack.emac, _gateway, sizeof(_gateway))) {
        return _gateway;
    }

    return 0;
}

NetworkStack *EthernetInterface::get_stack()
{
    return nsapi_create_stack(&_stack);
}
