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
EthernetInterface::EthernetInterface(const emac_interface_ops_t *emac_ops, void *hw)
    : _emac_ops(emac_ops), _hw(hw), _dhcp(true), _mac_address(), _ip_address(), _netmask(), _gateway()
{
}
nsapi_error_t EthernetInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    _dhcp = false;

    strncpy(_ip_address, ip_address ? ip_address : "", sizeof(_ip_address));
    _ip_address[sizeof(_ip_address) - 1] = '\0';
    strncpy(_netmask, netmask ? netmask : "", sizeof(_netmask));
    _netmask[sizeof(_netmask) - 1] = '\0';
    strncpy(_gateway, gateway ? gateway : "", sizeof(_gateway));
    _gateway[sizeof(_gateway) - 1] = '\0';

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
    if (_emac_ops == NULL) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    // This checks for double init, so okay
    mbed_ipstack_init();
    // XXX But this will add a second time
    err = mbed_ipstack_add_ethernet_interface(_emac_ops, _hw, true, &_interface);
    if (err != NSAPI_ERROR_OK) {
        return err;
    }

    return mbed_ipstack_bringup(_interface, _dhcp,
            _ip_address[0] ? _ip_address : 0,
            _netmask[0] ? _netmask : 0,
            _gateway[0] ? _gateway : 0,
            DEFAULT_STACK);
}

nsapi_error_t EthernetInterface::disconnect()
{
    return mbed_ipstack_bringdown(_interface);
}

const char *EthernetInterface::get_mac_address()
{
    if (mbed_ipstack_get_mac_address(_interface, _mac_address, sizeof(_mac_address))) {
        return _mac_address;
    }
    return NULL;
}

const char *EthernetInterface::get_ip_address()
{
    if (mbed_ipstack_get_ip_address(_interface, _ip_address, sizeof(_ip_address))) {
        return _ip_address;
    }

    return NULL;
}

const char *EthernetInterface::get_netmask()
{
    if (mbed_ipstack_get_netmask(_interface, _netmask, sizeof(_netmask))) {
        return _netmask;
    }

    return 0;
}

const char *EthernetInterface::get_gateway()
{
    if (mbed_ipstack_get_gateway(_interface, _gateway, sizeof(_gateway))) {
        return _gateway;
    }

    return 0;
}

NetworkStack *EthernetInterface::get_stack()
{
    return mbed_ipstack_get_stack();
}
