/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ns_types.h"
#include "ns_trace.h"
#include "platform/arm_hal_phy.h"
#include "net_interface.h"
#include "MAC/rf_driver_storage.h"
#include "virtual_rf_defines.h"


#define TRACE_GROUP "vrfc"

static phy_device_driver_s device_driver;
static int8_t rf_driver_id = (-1);
static const phy_rf_channel_configuration_s phy_2_4ghz = {.channel_0_center_frequency = 2405000000, .channel_spacing = 5000000, .datarate = 250000, .number_of_channels = 16, .modulation = M_OQPSK};
static const phy_device_channel_page_s phy_channel_pages = { CHANNEL_PAGE_0, &phy_2_4ghz};

static int8_t phy_rf_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    if (rf_driver_id == -1 || rf_driver_id != driver_id || !device_driver.arm_net_virtual_tx_cb) {
        return -1;
    }

    virtual_data_req_t data_req;
    uint8_t buf_temp[3];
    uint8_t *ptr = buf_temp;
    memset(&data_req, 0, sizeof(virtual_data_req_t));

    *ptr++ = NAP_DATA_PHY_RAW_INDICATION;
    *ptr++ = link_quality;
    *ptr = dbm;

    data_req.parameters = buf_temp;
    data_req.parameter_length = 3;
    data_req.msdu = data_ptr;
    data_req.msduLength = data_len;

    return device_driver.arm_net_virtual_tx_cb(&data_req, driver_id);
}

static uint8_t mac_mlme_status_2_phy_status(uint8_t status)
{
    switch (status) {
        case MLME_TX_NO_ACK:
            return PHY_LINK_TX_FAIL;
        case MLME_BUSY_CHAN:
            return PHY_LINK_CCA_FAIL;
        case MLME_SUCCESS:
            return PHY_LINK_TX_DONE;
        default:
            return PHY_LINK_TX_SUCCESS;
    }
}

static int8_t phy_rf_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    if (rf_driver_id == -1 || rf_driver_id != driver_id || !device_driver.arm_net_virtual_tx_cb) {
        return -1;
    }
    (void) tx_handle;
    virtual_data_req_t data_req;
    memset(&data_req, 0, sizeof(virtual_data_req_t));
    uint8_t buf_temp[4];
    uint8_t *ptr = buf_temp;
    *ptr++ = NAP_DATA_PHY_RAW_RESPONSE;
    *ptr++ = mac_mlme_status_2_phy_status(status);
    *ptr++ = cca_retry;
    *ptr = tx_retry;
    data_req.msdu = buf_temp;
    data_req.msduLength = 4;

    return device_driver.arm_net_virtual_tx_cb(&data_req, driver_id);
}

static int8_t phy_rf_virtual_config_send(int8_t driver_id, const uint8_t *data, uint16_t length)
{
    if (rf_driver_id != driver_id || !device_driver.arm_net_virtual_tx_cb) {
        return -1;
    }
    virtual_data_req_t data_req;
    memset(&data_req, 0, sizeof(virtual_data_req_t));
    uint8_t buf_temp[length + 1];
    uint8_t *ptr = buf_temp;
    *ptr++ = NAP_CONFIG_INTERNAL;
    memcpy(ptr, data, length);
    data_req.msdu = buf_temp;
    data_req.msduLength = length + 1;
    return device_driver.arm_net_virtual_tx_cb(&data_req, driver_id);
}

static int8_t phy_rf_virtual_rx(const uint8_t *data_ptr, uint16_t data_len, int8_t driver_id)
{
    if (rf_driver_id != driver_id || !data_ptr) {
        return -1;
    }

    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if (!driver || !driver->phy_sap_identifier || !driver->phy_sap_upper_cb) {
        return -1;
    }

    uint8_t data_type = *data_ptr++;
    arm_phy_sap_msg_t phy_msg;

    switch (data_type) {
        case NAP_DATA_PHY_RAW_REQUEST:
            if (data_len <= 5) {
                return -1;
            }
            phy_msg.id = MACTUN_PD_SAP_NAP_IND;
            phy_msg.message.generic_data_ind.data_len = data_len - 1;
            phy_msg.message.generic_data_ind.data_ptr = data_ptr;
            phy_msg.message.generic_data_ind.dbm = 0;
            phy_msg.message.generic_data_ind.link_quality = 0;
            break;

        case NAP_MLME_REQUEST:
            if (data_len < 3) {
                return -1;
            }
            phy_msg.id = MACTUN_MLME_NAP_EXTENSION;
            phy_msg.message.mlme_request.primitive = (mlme_primitive) * data_ptr++;
            phy_msg.message.mlme_request.mlme_ptr = data_ptr;
            phy_msg.message.mlme_request.ptr_length = (data_len - 2);

            break;

        default:
            return -1;
    }


    return driver->phy_sap_upper_cb(driver->phy_sap_identifier, &phy_msg);
}

int8_t virtual_rf_client_register(void)
{
    if (rf_driver_id < 0) {
        memset(&device_driver, 0, sizeof(phy_device_driver_s));
        device_driver.arm_net_virtual_rx_cb = &phy_rf_virtual_rx;
        device_driver.phy_rx_cb = &phy_rf_rx;
        device_driver.phy_tx_done_cb = &phy_rf_tx_done;
        device_driver.virtual_config_tx_cb = phy_rf_virtual_config_send;
        device_driver.driver_description = "VSND_Client";
        device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
        device_driver.phy_channel_pages = &phy_channel_pages;
        rf_driver_id = arm_net_phy_register(&device_driver);

    }
    return rf_driver_id;
}


