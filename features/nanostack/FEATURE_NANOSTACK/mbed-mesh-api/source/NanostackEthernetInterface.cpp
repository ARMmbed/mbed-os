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
#include "NanostackEthernetInterface.h"
#include "mesh_system.h"
#include "callback_handler.h"
#include "enet_tasklet.h"

nsapi_error_t NanostackEthernetInterface::initialize(NanostackEthernetPhy *phy)
{
    nsapi_error_t err = MeshInterfaceNanostack::initialize(phy);
    if (err != NSAPI_ERROR_OK)
        return err;

    nanostack_lock();

    if (register_phy() < 0) {
        nanostack_unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    enet_tasklet_init();
    __mesh_handler_set_callback(this);
    _network_interface_id = enet_tasklet_network_init(_device_id);

    nanostack_unlock();

    if (_network_interface_id < 0)
        return MESH_ERROR_UNKNOWN;
    else
        return MESH_ERROR_NONE;
}

int NanostackEthernetInterface::connect()
{
    nanostack_lock();
    int8_t status = enet_tasklet_connect(&__mesh_handler_c_callback, _network_interface_id);
    nanostack_unlock();

    if (status == -1) {
        return MESH_ERROR_PARAM;
    } else if (status == -2) {
        return MESH_ERROR_MEMORY;
    } else if (status == -3) {
        return MESH_ERROR_STATE;
    } else if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    int32_t count = connect_semaphore.wait(30000);

    if (count <= 0) {
        return NSAPI_ERROR_DHCP_FAILURE; // sort of...
    }
    return 0;
}

int NanostackEthernetInterface::disconnect()
{
    enet_tasklet_disconnect();
    return 0;
}

bool NanostackEthernetInterface::getOwnIpAddress(char *address, int8_t len)
{
    return enet_tasklet_get_ip_address(address, len)?false:true;
}

bool NanostackEthernetInterface::getRouterIpAddress(char *address, int8_t len)
{
    return false;
}
