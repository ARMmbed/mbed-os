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
#include "lwip_stack.h"


/* Interface implementation */
EthernetInterface::EthernetInterface()
    : _dhcp(true), _ip_address(), _netmask(), _gateway()
{
}

int EthernetInterface::set_network(const char *ip_address, const char *netmask, const char *gateway)
{
    _dhcp = false;
    strncpy(_ip_address, ip_address ? ip_address : "", sizeof(_ip_address));
    strncpy(_netmask, netmask ? netmask : "", sizeof(_netmask));
    strncpy(_gateway, gateway ? gateway : "", sizeof(_gateway));
    return 0;
}

int EthernetInterface::set_dhcp(bool dhcp)
{
    _dhcp = dhcp;
    return 0;
}

int EthernetInterface::connect()
{
    return lwip_bringup(_dhcp,
            _ip_address[0] ? _ip_address : 0,
            _netmask[0] ? _netmask : 0,
            _gateway[0] ? _gateway : 0);
}

int EthernetInterface::disconnect()
{
    return lwip_bringdown();
}

const char *EthernetInterface::get_mac_address()
{
    return lwip_get_mac_address();
}

const char *EthernetInterface::get_ip_address()
{
    return lwip_get_ip_address();
}

const char *EthernetInterface::get_netmask()
{
    return lwip_get_netmask();
}

const char *EthernetInterface::get_gateway()
{
    return lwip_get_gateway();
}

NetworkStack *EthernetInterface::get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}
