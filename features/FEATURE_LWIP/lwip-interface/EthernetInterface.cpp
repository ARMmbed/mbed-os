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
EthernetInterface::EthernetInterface() : 
    _dhcp(true), 
    _ip_address(), 
    _netmask(), 
    _gateway(),
    _connection_status_cb(NULL),
    _connect_status(NSAPI_STATUS_DISCONNECTED)
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
    return mbed_lwip_bringup_2(_dhcp, false,
            _ip_address[0] ? _ip_address : 0,
            _netmask[0] ? _netmask : 0,
            _gateway[0] ? _gateway : 0,
            DEFAULT_STACK);
}

nsapi_error_t EthernetInterface::disconnect()
{
    return mbed_lwip_bringdown_2(false);
}

const char *EthernetInterface::get_mac_address()
{
    return mbed_lwip_get_mac_address();
}

const char *EthernetInterface::get_ip_address()
{
    if (mbed_lwip_get_ip_address(_ip_address, sizeof _ip_address)) {
        return _ip_address;
    }

    return NULL;
}

const char *EthernetInterface::get_netmask()
{
    if (mbed_lwip_get_netmask(_netmask, sizeof _netmask)) {
        return _netmask;
    }

    return 0;
}

const char *EthernetInterface::get_gateway()
{
    if (mbed_lwip_get_gateway(_gateway, sizeof _gateway)) {
        return _gateway;
    }

    return 0;
}

NetworkStack *EthernetInterface::get_stack()
{
    return nsapi_create_stack(&lwip_stack);
}

void EthernetInterface::attach(
    Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
    mbed_lwip_attach(netif_status_cb, this);
}

nsapi_connection_status_t EthernetInterface::get_connection_status() const
{
    return _connect_status;
}

void EthernetInterface::netif_status_cb(void *ethernet_if_ptr, 
    nsapi_event_t reason, intptr_t parameter) 
{
    EthernetInterface *eth_ptr = static_cast<EthernetInterface*>(ethernet_if_ptr);
    eth_ptr->_connect_status = (nsapi_connection_status_t)parameter;
    if (eth_ptr->_connection_status_cb)
    {
        eth_ptr->_connection_status_cb(reason, parameter);
    }   
}

nsapi_error_t EthernetInterface::set_blocking(bool blocking)
{
    mbed_lwip_set_blocking(blocking);
    return NSAPI_ERROR_OK;
}
