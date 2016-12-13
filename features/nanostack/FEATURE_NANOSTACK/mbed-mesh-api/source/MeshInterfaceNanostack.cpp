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
#include "NanostackInterface.h"
#include "mesh_system.h"

MeshInterfaceNanostack::MeshInterfaceNanostack()
    : phy(NULL), _network_interface_id(-1), _device_id(-1), eui64(),
      ip_addr_str(), mac_addr_str(), connect_semaphore(0)
{
    // Nothing to do
}

MeshInterfaceNanostack::MeshInterfaceNanostack(NanostackPhy *phy)
    : phy(phy), _network_interface_id(-1), _device_id(-1), connect_semaphore(0)
{
    // Nothing to do
}

nsapi_error_t MeshInterfaceNanostack::initialize(NanostackPhy *phy)
{
    mesh_system_init();
    if (this->phy != NULL) {
        error("Phy already set");
    }
    this->phy = phy;
    return 0;
}

void MeshInterfaceNanostack::mesh_network_handler(mesh_connection_status_t status)
{
    nanostack_lock();

    if (status == MESH_CONNECTED) {
        connect_semaphore.release();
    }

    nanostack_unlock();
}

nsapi_error_t MeshInterfaceNanostack::register_phy()
{
    nanostack_lock();

    _device_id = phy->phy_register();
    if (_device_id < 0) {
        nanostack_unlock();
        return -1;
    }
    // Read mac address after registering the device.
    phy->get_mac_address(eui64);
    sprintf(mac_addr_str, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", eui64[0], eui64[1], eui64[2], eui64[3], eui64[4], eui64[5], eui64[6], eui64[7]);

    nanostack_unlock();

    return 0;
}

NetworkStack * MeshInterfaceNanostack::get_stack()
{
    return NanostackInterface::get_stack();
}

const char *MeshInterfaceNanostack::get_ip_address()
{
    nanostack_lock();

    const char *ret = NULL;
    if (getOwnIpAddress(ip_addr_str, sizeof ip_addr_str)) {
        ret = ip_addr_str;
    }

    nanostack_unlock();

    return ret;
}

const char *MeshInterfaceNanostack::get_mac_address()
{
    return mac_addr_str;
}
