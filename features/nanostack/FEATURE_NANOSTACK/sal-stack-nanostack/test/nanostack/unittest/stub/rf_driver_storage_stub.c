/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "rf_driver_storage_stub.h"

//TODO: refactor away (needed by local_socket)
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"

rf_driver_storage_def rf_driver_storage_stub;

arm_device_driver_list_s *arm_net_phy_driver_pointer(int8_t id)
{
    return rf_driver_storage_stub.list_value;
}

int8_t arm_net_phy_register(phy_device_driver_s *phy_driver)
{
    rf_driver_storage_stub.registered_driver = phy_driver;
    return rf_driver_storage_stub.int8_value;
}

int8_t arm_net_phy_init(phy_device_driver_s *phy_driver, arm_net_phy_rx_fn *rx_cb, arm_net_phy_tx_done_fn *done_cb)
{
    rf_driver_storage_stub.rx_cb = rx_cb;
    rf_driver_storage_stub.done_cb = done_cb;
    return rf_driver_storage_stub.int8_value;
}

uint32_t dev_get_phy_datarate(phy_device_driver_s *phy_driver, channel_page_e channel_page)
{
    return rf_driver_storage_stub.uint32_value;
}

int8_t arm_net_phy_mac64_set(uint8_t *MAC, int8_t id)
{
    return rf_driver_storage_stub.int8_value;
}

uint8_t *arm_net_phy_mac64_get(int8_t id)
{
    return rf_driver_storage_stub.uint8_ptr;
}

int arm_net_phy_rf_type(int8_t id)
{
    return rf_driver_storage_stub.int_value;
}

uint16_t arm_net_phy_mtu_size(int8_t id)
{
    return rf_driver_storage_stub.uint16_value;
}

void arm_net_observer_cb_set(int8_t id, internal_mib_observer *observer_cb)
{
    rf_driver_storage_stub.mlme_observer = observer_cb;
}

void arm_net_virtual_config_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_config_rx_fn *virtual_config_rx)
{

}

void arm_net_virtual_confirmation_rx_cb_set(phy_device_driver_s *phy_driver, arm_net_virtual_confirmation_rx_fn *virtual_confirmation_rx)
{

}
