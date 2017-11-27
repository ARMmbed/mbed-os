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
#include "test_fhss_callbacks.h"
#include "fhss_api.h"
#include <string.h>
#include <inttypes.h>
#include "sw_mac_stub.h"
#include "rf_driver_storage_stub.h"
#include "mac_mlme_stub.h"

static int8_t tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    return 0;
}

int8_t virt_cfg(int8_t driver_id, const uint8_t *data, uint16_t length)
{
    (void)driver_id;
    (void)data;
    (void)length;
    return 0;
}

void mlme_ind(const mac_api_t* api, mlme_primitive id, const void *data)
{

}

bool test_mac_read_tx_queue_sizes()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    setup.unicast_queue_size = 5;
    setup.broadcast_queue_size = 3;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_read_tx_queue_sizes(&fhss_api, false)) {
        return false;
    }
    sw_mac_stub.rf_mac_ptr = &setup;
    // Test when unicast TX queue size is set to 5
    if (mac_read_tx_queue_sizes(&fhss_api, false) != 5) {
        return false;
    }
    // Test when broadcast TX queue size is set to 3
    if (mac_read_tx_queue_sizes(&fhss_api, true) != 3) {
        return false;
    }
    return true;
}

bool test_mac_read_64bit_mac_address()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    uint8_t mac_array[8];
    memset(mac_array, 0, 8);
    memset(setup.mac64, 1, 8);
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_read_64bit_mac_address(&fhss_api, &mac_array) == 0) {
        return false;
    }
    // MAC assigned
    sw_mac_stub.rf_mac_ptr = &setup;
    if (mac_read_64bit_mac_address(&fhss_api, &mac_array) != 0) {
        return false;
    }
    // Test that mac64 is copied to mac_array
    if (memcmp(mac_array, setup.mac64, 8)) {
        return false;
    }
    return true;
}

bool test_mac_read_phy_datarate()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    arm_device_driver_list_s driver;
    phy_device_driver_s dev_driver;
    setup.fhss_api = &fhss_api;
    driver.phy_driver = &dev_driver;
    setup.dev_driver = &driver;
    rf_driver_storage_stub.uint32_value = 250000;

    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_read_phy_datarate(&fhss_api) != 0) {
        return false;
    }
    // MAC assigned
    sw_mac_stub.rf_mac_ptr = &setup;
    // Test that function returns correct datarate
    if (mac_read_phy_datarate(&fhss_api) != 250000) {
        return false;
    }
    return true;
}

bool test_mac_set_channel()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    phy_device_driver_s phy_driver;
    arm_device_driver_list_s dev_driver;
    phy_driver.phy_tx_done_cb = tx_done;
    dev_driver.phy_driver = &phy_driver;
    setup.fhss_api = &fhss_api;
    setup.dev_driver = &dev_driver;
    mac_mlme_stub.int8_value = 0;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_set_channel(&fhss_api, 0) == 0) {
        return false;
    }
    // MAC assigned
    sw_mac_stub.rf_mac_ptr = &setup;
    setup.macRfRadioTxActive = true;
    if (mac_set_channel(&fhss_api, 0) != 0) {
        return false;
    }
    return true;
}

bool test_mac_fhss_frame_tx()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    mac_mlme_stub.uint8_value = 0;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_fhss_frame_tx(&fhss_api, FHSS_SYNCH_FRAME) == 0) {
        return false;
    }
    // MAC assigned
    sw_mac_stub.rf_mac_ptr = &setup;
    // Should fail because mac_mlme_beacon_req_tx returns 0
    if (mac_fhss_frame_tx(&fhss_api, FHSS_SYNCH_REQUEST_FRAME) == 0) {
        return false;
    }
    // Send synch frame
    if (mac_fhss_frame_tx(&fhss_api, FHSS_SYNCH_FRAME) != 0) {
        return false;
    }
    // Send synch frame when MAC fails to start transmission
    mac_mlme_stub.int8_value = -1;
    if (mac_fhss_frame_tx(&fhss_api, FHSS_SYNCH_FRAME) == 0) {
        return false;
    }
    // Send non supported frame
    if (mac_fhss_frame_tx(&fhss_api, FHSS_DATA_FRAME) == 0) {
        return false;
    }
    return true;
}

bool test_mac_synch_lost()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    mac_mlme_stub.uint8_value = 0;
    sw_mac_stub.rf_mac_ptr = NULL;

    if (-1 != mac_synch_lost(&fhss_api)) {
        return false;
    }
    sw_mac_stub.rf_mac_ptr = &setup;

    sw_mac_stub.mac_api_ptr = NULL;
    if (-1 != mac_synch_lost(&fhss_api)) {
        return false;
    }

    mac_api_t api;
    api.mlme_ind_cb = &mlme_ind;
    sw_mac_stub.mac_api_ptr = &api;

    if (0 != mac_synch_lost(&fhss_api)) {
        return false;
    }

    return true;
}

bool test_mac_poll_tx_queue()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_poll_tx_queue(&fhss_api) == 0) {
        return false;
    }
    // MAC assigned
    sw_mac_stub.rf_mac_ptr = &setup;
    if (mac_poll_tx_queue(&fhss_api) != 0) {
        return false;
    }
    return true;
}

bool test_mac_broadcast_notification()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.fhss_api = &fhss_api;
    mac_mlme_stub.uint8_value = 0;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    if (mac_broadcast_notification(&fhss_api, 2) != -1) {
        return false;
    }

    sw_mac_stub.rf_mac_ptr = &setup;
    struct arm_device_driver_list driv;
    setup.tun_extension_rf_driver = &driv;
    phy_device_driver_s dev_driver;
    driv.phy_driver = &dev_driver;
    dev_driver.virtual_config_tx_cb = &virt_cfg;
    if (mac_broadcast_notification(&fhss_api, 2) != 0) {
        return false;
    }

    return true;
}

bool test_mac_get_coordinator_mac_address()
{
    fhss_api_t fhss_api;
    protocol_interface_rf_mac_setup_s setup;
    setup.fhss_api = &fhss_api;
    mac_mlme_stub.uint8_value = 0;
    // Test when MAC cannot be found
    sw_mac_stub.rf_mac_ptr = NULL;
    uint8_t address[8];
    if (mac_get_coordinator_mac_address(&fhss_api, address) != -1) {
        return false;
    }

    sw_mac_stub.rf_mac_ptr = &setup;

    if (mac_get_coordinator_mac_address(&fhss_api, address) != 0) {
        return false;
    }
    return true;
}
