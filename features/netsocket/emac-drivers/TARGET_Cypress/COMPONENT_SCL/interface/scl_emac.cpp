/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "scl_emac.h"
#include "lwip/etharp.h"
#include "lwip/ethip6.h"
#include "mbed_shared_queues.h"
#include "scl_buffer_api.h"
#include "cy_result.h"
#include "cy_pdl.h"
#include "scl_ipc.h"

/** @file
 *  Provides EMAC interface functions to be used with the SCL_EMAC object
 */

SCL_EMAC::SCL_EMAC(scl_interface_role_t role)
    : interface_type(role)
{
}

SCL_EMAC::SCL_EMAC()
    : interface_type(SCL_STA_ROLE)
{
}

SCL_EMAC &SCL_EMAC::get_instance()
{
    return get_instance(SCL_STA_ROLE);
}

SCL_EMAC &SCL_EMAC::get_instance(scl_interface_role_t role)
{
    static SCL_EMAC emac_sta(SCL_STA_ROLE);
    static SCL_EMAC emac_ap(SCL_AP_ROLE);
    return role == SCL_AP_ROLE ? emac_ap : emac_sta;
}

uint32_t SCL_EMAC::get_mtu_size() const
{
    return SCL_PAYLOAD_MTU;
}

uint32_t SCL_EMAC::get_align_preference() const
{
    return 0;
}

void SCL_EMAC::add_multicast_group(const uint8_t *addr)
{
    memcpy(multicast_addr.octet, addr, sizeof(multicast_addr.octet));
    scl_wifi_register_multicast_address(&multicast_addr);
}

void SCL_EMAC::remove_multicast_group(const uint8_t *address)
{
    /* To Do */
}
void SCL_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void SCL_EMAC::power_down()
{
    /* No-op at this stage */
}

bool SCL_EMAC::power_up()
{
    if (!powered_up) {
        if (scl_wifi_on() != true) {
            SCL_LOG(("returning False in scl_wifi_on()\n"));
            return false;
        }
        powered_up = true;
        if (link_state && emac_link_state_cb) {
            emac_link_state_cb(link_state);
        }
    }
    return true;
}

bool SCL_EMAC::get_hwaddr(uint8_t *addr) const
{
    scl_mac_t mac;
    scl_result_t res = scl_wifi_get_mac_address(&mac);
    if (res == SCL_SUCCESS) {
        memcpy(addr, mac.octet, sizeof(mac.octet));
        return true;
    } else {
        SCL_LOG(("return False in SCL_EMAC::gethwaddr\n"));
        return false;
    }

}

void SCL_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

uint8_t SCL_EMAC::get_hwaddr_size() const
{
    scl_mac_t mac;
    return sizeof(mac.octet);
}

void SCL_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void SCL_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void SCL_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

bool SCL_EMAC::link_out(emac_mem_buf_t *buf)
{
    scl_result_t retval;
    scl_tx_buf_t scl_tx_data;
    scl_tx_data.size = memory_manager->get_total_len(buf);
    scl_tx_data.buffer = buf;
    if (buf == NULL) {
        return false;
    }
    retval = scl_network_send_ethernet_data(scl_tx_data);
    if (retval != SCL_SUCCESS) {
        return false;
    }
    memory_manager->free(buf);
    return true;
}

void SCL_EMAC::get_ifname(char *name, uint8_t size) const
{
    if (name != NULL) {
        memcpy(name, "scl", size);
    }
}

void SCL_EMAC::set_activity_cb(mbed::Callback<void(bool)> cb)
{
    activity_cb = cb;
}

extern "C"
{
    void scl_network_process_ethernet_data(scl_buffer_t buffer)
    {
        emac_mem_buf_t *mem_buf = NULL;

        SCL_EMAC &emac = SCL_EMAC::get_instance(SCL_STA_ROLE);

        if (!emac.powered_up && !emac.emac_link_input_cb) {
            scl_buffer_release(buffer, SCL_NETWORK_RX);
            return;
        }
        mem_buf = buffer;
        if (emac.activity_cb) {
            emac.activity_cb(false);
        }
        emac.emac_link_input_cb(mem_buf);
    }

    void scl_emac_wifi_link_state_changed(bool state_up)
    {
        SCL_EMAC &emac = SCL_EMAC::get_instance(SCL_STA_ROLE);

        emac.link_state = state_up;
        if (emac.emac_link_state_cb) {
            emac.emac_link_state_cb(state_up);
        }
    }
} // extern "C"


