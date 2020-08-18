/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "MAC/rf_driver_storage.h"

static arm_device_driver_list_s *dev_driver_dynamically_allocate(void);
static int8_t dev_driver_get_free_id(void);

static NS_LIST_DEFINE(arm_device_driver_list, arm_device_driver_list_s, link);

arm_device_driver_list_s *arm_net_phy_driver_pointer(int8_t id)
{
    ns_list_foreach(arm_device_driver_list_s, cur, &arm_device_driver_list) {
        if (cur->id == id) {
            return cur;
        }
    }

    return NULL;
}

static arm_device_driver_list_s *dev_driver_dynamically_allocate(void)
{
    arm_device_driver_list_s *event = 0;
    event = ns_dyn_mem_alloc(sizeof(arm_device_driver_list_s));
    if (event) {
        event->phy_sap_identifier = NULL;
        event->phy_sap_upper_cb = NULL;
        event->mlme_observer_cb = NULL;
        ns_list_link_init(event, link);
    }
    return event;
}

static int8_t dev_driver_get_free_id(void)
{
    uint_fast8_t id; // Must be unsigned for loop test to work...

    for (id = 0; id <= INT8_MAX; id++) {
        if (!arm_net_phy_driver_pointer(id)) {
            return id;
        }
    }

    return -1;
}

int8_t arm_net_phy_register(phy_device_driver_s *phy_driver)
{
    arm_device_driver_list_s *new;
    if (!phy_driver) {
        return -1;
    }

    ns_list_foreach(arm_device_driver_list_s, cur, &arm_device_driver_list) {
        if (cur->phy_driver == phy_driver) {
            return -1;
        }
    }
    /* Validate That Channel page is defined for RF driver */
    if (phy_driver->link_type == PHY_LINK_15_4_2_4GHZ_TYPE || phy_driver->link_type == PHY_LINK_15_4_SUBGHZ_TYPE) {
        if (!phy_driver->phy_channel_pages || !phy_driver->phy_channel_pages->rf_channel_configuration) {
            return -1;
        }
    }


    //Allocate new
    new = dev_driver_dynamically_allocate();
    if (!new) {
        return -2;
    }

    new->id = dev_driver_get_free_id();
    new->phy_driver = phy_driver;
    if (new->phy_driver->state_control) {
        new->phy_driver->state_control(PHY_INTERFACE_RESET, 0);
    }
    ns_list_add_to_end(&arm_device_driver_list, new);

    return new->id;
}

int8_t arm_net_phy_mac64_set(uint8_t *MAC, int8_t id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(id);
    if (!driver || !driver->phy_driver->PHY_MAC || !MAC) {
        return -1;
    }

    memcpy(driver->phy_driver->PHY_MAC, MAC, 8);
    return 0;
}

uint8_t *arm_net_phy_mac64_get(int8_t id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(id);
    if (!driver) {
        return NULL;
    }
    return driver->phy_driver->PHY_MAC;
}

int arm_net_phy_rf_type(int8_t id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(id);
    if (!driver) {
        return -1;
    }
    return driver->phy_driver->link_type;
}

uint16_t arm_net_phy_mtu_size(int8_t id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(id);
    if (!driver) {
        return 0;
    }
    return driver->phy_driver->phy_MTU;
}

void arm_net_phy_unregister(int8_t driver_id)
{
    ns_list_foreach_safe(arm_device_driver_list_s, cur, &arm_device_driver_list) {
        if (cur->id == driver_id) {
            ns_list_remove(&arm_device_driver_list, cur);
            ns_dyn_mem_free(cur);
            cur = NULL;
        }
    }
}

int8_t arm_net_phy_init(phy_device_driver_s *phy_driver, arm_net_phy_rx_fn *rx_cb, arm_net_phy_tx_done_fn *done_cb)
{
    if (!phy_driver) {
        return -1;
    }
    phy_driver->phy_rx_cb = rx_cb;
    phy_driver->phy_tx_done_cb = done_cb;
    return 0;
}

void arm_net_observer_cb_set(int8_t id, internal_mib_observer *observer_cb)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(id);
    if (!driver) {
        return;
    }
    driver->mlme_observer_cb = observer_cb;
}

void arm_net_virtual_config_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_config_rx_fn *virtual_config_rx)
{
    if (!phy_driver) {
        return;
    }
    phy_driver->virtual_config_rx_cb = virtual_config_rx;
}

void arm_net_virtual_confirmation_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_confirmation_rx_fn *virtual_confirmation_rx)
{
    if (!phy_driver) {
        return;
    }
    phy_driver->virtual_confirmation_rx_cb = virtual_confirmation_rx;
}

uint32_t dev_get_phy_datarate(phy_device_driver_s *phy_driver, channel_page_e channel_page)
{
    uint32_t retval = 0;
    const phy_device_channel_page_s *phy_ch_pages = phy_driver->phy_channel_pages;
    while (1) {
        if (phy_ch_pages->rf_channel_configuration == NULL) {
            break;
        }

        if (phy_ch_pages->channel_page == channel_page) {
            retval = phy_ch_pages->rf_channel_configuration->datarate;
            break;
        }
        phy_ch_pages++;
    }
    return retval;
}
