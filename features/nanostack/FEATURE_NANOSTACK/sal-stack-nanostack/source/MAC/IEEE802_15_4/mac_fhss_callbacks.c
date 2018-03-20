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

#include <string.h>
#include "nsconfig.h"
#include "ns_types.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "MAC/IEEE802_15_4/mac_fhss_callbacks.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/rf_driver_storage.h"
#include "common_functions.h"


uint16_t mac_read_tx_queue_sizes(const fhss_api_t *fhss_api, bool broadcast_queue)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return 0;
    }
    if (broadcast_queue == true) {
        return mac_setup->broadcast_queue_size;
    }
    return mac_setup->unicast_queue_size;
}

int mac_read_64bit_mac_address(const fhss_api_t *fhss_api, uint8_t *mac_address)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    memcpy(mac_address, mac_setup->mac64, 8);
    return 0;
}

uint32_t mac_read_phy_datarate(const fhss_api_t *fhss_api)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return 0;
    }
    return dev_get_phy_datarate(mac_setup->dev_driver->phy_driver, mac_setup->mac_channel_list.channel_page);
}

int mac_set_channel(const fhss_api_t *fhss_api, uint8_t channel_number)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    // If TX is active, send internal CCA fail event. MAC state machine would crash without TX done event.
    if (mac_setup->macRfRadioTxActive == true) {
        mac_setup->dev_driver->phy_driver->phy_tx_done_cb(mac_setup->dev_driver->id, 1, PHY_LINK_TX_FAIL, 0, 0);
    }
    return mac_mlme_rf_channel_change(mac_setup, channel_number);
}

int mac_fhss_frame_tx(const fhss_api_t *fhss_api, int frame_type)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    if (FHSS_SYNCH_REQUEST_FRAME == frame_type) {
        if (mac_mlme_beacon_req_tx(mac_setup) != 1) {
            return -1;
        }
    } else if (FHSS_SYNCH_FRAME == frame_type) {
        if (mac_mlme_beacon_tx(mac_setup) != 0) {
            return -1;
        }
    } else {
        return -1;
    }
    return 0;
}

int mac_synch_lost(const fhss_api_t *fhss_api)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    mac_api_t *mac_api = get_sw_mac_api(mac_setup);
    if (!mac_api) {
        return -1;
    }
    mlme_sync_loss_t sync_loss;
    sync_loss.LossReason = BEACON_LOST;
    mac_api->mlme_ind_cb(mac_api, MLME_SYNC_LOSS, &sync_loss);
    return 0;
}

int mac_poll_tx_queue(const fhss_api_t *fhss_api)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    mcps_sap_trig_tx(mac_setup);
    return 0;
}

int mac_broadcast_notification(const fhss_api_t *fhss_api, uint32_t broadcast_time)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    if (mac_setup->tun_extension_rf_driver && mac_setup->tun_extension_rf_driver->phy_driver->virtual_config_tx_cb) {
        uint8_t data_buffer[5];
        data_buffer[0] = MAC_BROADCAST_EVENT;
        common_write_32_bit(broadcast_time, &data_buffer[1]);
        mac_setup->tun_extension_rf_driver->phy_driver->virtual_config_tx_cb(mac_setup->tun_extension_rf_driver->id, data_buffer, 5);
    }
    return 0;
}

int mac_get_coordinator_mac_address(const fhss_api_t *fhss_api, uint8_t *mac_address)
{
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_fhss_api(fhss_api);
    if (!mac_setup) {
        return -1;
    }
    memcpy(mac_address, mac_setup->coord_long_address, 8);
    return 0;
}
