/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MeshInterfaceNanostack.h"
#include "Nanostack.h"
#include "NanostackLockGuard.h"
#include "mesh_system.h"
#include "nanostack/net_interface.h"
#include "thread_management_if.h"
#include "ip6string.h"

char *Nanostack::Interface::get_ip_address(char *buf, nsapi_size_t buflen)
{
    NanostackLockGuard lock;
    uint8_t binary_ipv6[16];

    if (buflen >= 40 && arm_net_address_get(interface_id, ADDR_IPV6_GP, binary_ipv6) == 0) {
        ip6tos(binary_ipv6, buf);
        return buf;
    } else {
        return NULL;
    }
}

char *Nanostack::Interface::get_mac_address(char *buf, nsapi_size_t buflen)
{
    NanostackLockGuard lock;
    link_layer_address_s addr;
    if (buflen >= 24 && arm_nwk_mac_address_read(interface_id, &addr) == 0) {
        snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", addr.mac_long[0], addr.mac_long[1], addr.mac_long[2], addr.mac_long[3], addr.mac_long[4], addr.mac_long[5], addr.mac_long[6], addr.mac_long[7]);
        return buf;
    } else {
        return NULL;
    }
}

char *Nanostack::Interface::get_netmask(char *, nsapi_size_t)
{
    return NULL;
}

char *Nanostack::Interface::get_gateway(char *, nsapi_size_t)
{
    return NULL;
}

nsapi_connection_status_t Nanostack::Interface::get_connection_status() const
{
    return _connect_status;
}

void Nanostack::Interface::attach(
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
}

Nanostack::Interface::Interface(NanostackPhy &phy) : interface_phy(phy), interface_id(-1), _device_id(-1),
      _connect_status(NSAPI_STATUS_DISCONNECTED), _blocking(true)
{
    mesh_system_init();
}


InterfaceNanostack::InterfaceNanostack()
    : _interface(NULL),
      ip_addr_str(), mac_addr_str(), _blocking(true)
{
    // Nothing to do
}

nsapi_error_t MeshInterfaceNanostack::initialize(NanostackRfPhy *phy)
{
    if (_phy) {
        error("Phy already set");
        return NSAPI_ERROR_IS_CONNECTED;
    }
    _phy = phy;
    return NSAPI_ERROR_OK;
}


void Nanostack::Interface::network_handler(mesh_connection_status_t status)
{
    if ((status == MESH_CONNECTED || status == MESH_CONNECTED_LOCAL ||
         status == MESH_CONNECTED_GLOBAL) && _blocking) {
        connect_semaphore.release();
    }


    if (status == MESH_CONNECTED) {
        uint8_t temp_ipv6_global[16];
        uint8_t temp_ipv6_local[16];
        if (arm_net_address_get(interface_id, ADDR_IPV6_LL, temp_ipv6_local) == 0) {
            _connect_status = NSAPI_STATUS_LOCAL_UP;
        }
        if (arm_net_address_get(interface_id, ADDR_IPV6_GP, temp_ipv6_global) == 0
            && (memcmp(temp_ipv6_global, temp_ipv6_local, 16) != 0)) {
            _connect_status = NSAPI_STATUS_GLOBAL_UP;
        }
    } else if (status == MESH_CONNECTED_LOCAL ) {
        _connect_status = NSAPI_STATUS_LOCAL_UP;
    } else if (status == MESH_CONNECTED_GLOBAL) {
        _connect_status = NSAPI_STATUS_GLOBAL_UP;
    } else if (status == MESH_BOOTSTRAP_STARTED) {
        _connect_status = NSAPI_STATUS_CONNECTING;
    } else {
        _connect_status = NSAPI_STATUS_DISCONNECTED;
    }

    if (_connection_status_cb) {
        _connection_status_cb(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, _connect_status);
    }
}

nsapi_error_t Nanostack::Interface::register_phy()
{
    NanostackLockGuard lock;

    _device_id = interface_phy.phy_register();
    if (_device_id < 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return NSAPI_ERROR_OK;
}

Nanostack *InterfaceNanostack::get_stack()
{
    return &Nanostack::get_instance();
}

const char *InterfaceNanostack::get_ip_address()
{
    if (_interface->get_ip_address(ip_addr_str, sizeof(ip_addr_str))) {
        return ip_addr_str;
    }
    return NULL;
}

const char *InterfaceNanostack::get_mac_address()
{
    if (_interface->get_mac_address(mac_addr_str, sizeof(mac_addr_str))) {
        return mac_addr_str;
    }
    return NULL;
}

nsapi_connection_status_t InterfaceNanostack::get_connection_status() const
{
    if (_interface) {
        return _interface->get_connection_status();
    } else {
        return NSAPI_STATUS_DISCONNECTED;
    }
}

void InterfaceNanostack::attach(
    mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb)
{
    _connection_status_cb = status_cb;
    if (_interface) {
        _interface->attach(status_cb);
    }
}

nsapi_error_t InterfaceNanostack::set_blocking(bool blocking)
{
    _blocking = blocking;
    return NSAPI_ERROR_OK;
}

#if !DEVICE_802_15_4_PHY
MBED_WEAK MeshInterface *MeshInterface::get_target_default_instance()
{
    return NULL;
}
#endif
