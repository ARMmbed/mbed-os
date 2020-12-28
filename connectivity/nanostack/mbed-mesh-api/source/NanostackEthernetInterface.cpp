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

using namespace std::chrono_literals;

nsapi_error_t Nanostack::EthernetInterface::initialize()
{
    nanostack_lock();

    if (register_phy() < 0) {
        nanostack_unlock();
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    nanostack_unlock();

    return NSAPI_ERROR_OK;
}

nsapi_error_t NanostackEthernetInterface::initialize(NanostackEthernetPhy *phy)
{
    if (_interface) {
        return NSAPI_ERROR_PARAMETER;
    }

    _interface = new (std::nothrow) Nanostack::EthernetInterface(*phy);
    if (!_interface) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    return get_interface()->initialize();
}

nsapi_error_t Nanostack::EthernetInterface::bringup(bool dhcp, const char *ip,
                                                    const char *netmask, const char *gw,
                                                    nsapi_ip_stack_t stack, bool blocking)
{
    if (stack == IPV4_STACK) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    nanostack_lock();
    _blocking = blocking;
    if (interface_id < 0) {
        enet_tasklet_init();
        __mesh_handler_set_callback(this);
        interface_id = enet_tasklet_network_init(_device_id);
    }
    int8_t status = -1;
    if (interface_id >= 0) {
        status = enet_tasklet_connect(&__mesh_handler_c_callback, interface_id);
    }
    nanostack_unlock();

    if (status == -1) {
        return NSAPI_ERROR_DEVICE_ERROR;
    } else if (status == -2) {
        return NSAPI_ERROR_NO_MEMORY;
    } else if (status == -3) {
        return NSAPI_ERROR_ALREADY;
    } else if (status != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (blocking) {
        bool acquired = connect_semaphore.try_acquire_for(30s);

        if (!acquired) {
            return NSAPI_ERROR_DHCP_FAILURE; // sort of...
        }
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t NanostackEthernetInterface::do_initialize()
{
    if (!_interface) {
        return NSAPI_ERROR_PARAMETER;
    }
    return NSAPI_ERROR_OK;
}

nsapi_error_t Nanostack::EthernetInterface::bringdown()
{
    nanostack_lock();
    int8_t status = enet_tasklet_disconnect(true);
    nanostack_unlock();

    if (status == -1) {
        return NSAPI_ERROR_DEVICE_ERROR;
    } else if (status == -2) {
        return NSAPI_ERROR_NO_MEMORY;
    } else if (status == -3) {
        return NSAPI_ERROR_ALREADY;
    } else if (status != 0) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (_blocking) {
        bool acquired = disconnect_semaphore.try_acquire_for(30s);

        if (!acquired) {
            return NSAPI_ERROR_TIMEOUT;
        }
    }
    return NSAPI_ERROR_OK;
}

char *Nanostack::EthernetInterface::get_interface_name(char *buf)
{
    if (interface_id < 0) {
        return NULL;
    }
    sprintf(buf, "ETH%d", interface_id);
    return buf;
};

void Nanostack::EthernetInterface::get_mac_address(uint8_t *buf)
{
    if (!buf) {
        return;
    }

    get_phy().get_mac_address(buf);
}

char *Nanostack::EthernetInterface::get_mac_address(char *buf, nsapi_size_t buflen)
{
    uint8_t mac_buf[NSAPI_MAC_BYTES] = {0};

    if (!buf || buflen < NSAPI_MAC_SIZE) {
        return NULL;
    }

    get_phy().get_mac_address(mac_buf);

    snprintf(buf, buflen, "%02x:%02x:%02x:%02x:%02x:%02x", mac_buf[0], mac_buf[1], mac_buf[2], mac_buf[3], mac_buf[4], mac_buf[5]);
    return buf;
}
