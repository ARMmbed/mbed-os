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
#include "test_mac_mlme.h"
#include <string.h>
#include "mac_api.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "MAC/rf_driver_storage.h"
#include "mlme.h"
#include "nsdynmemLIB.h"
#include "fhss_api.h"

#include "sw_mac_stub.h"
#include "nsdynmemLIB_stub.h"
#include "mac_mcps_sap_stub.h"
#include "common_functions_stub.h"
#include "mac_security_mib_stub.h"
#include "ns_timer_stub.h"
#include "mac_header_helper_functions_stub.h"
#include "fhss_config_stub.h"
#include "fhss_api.h"

bool visited = false;

static int8_t test_arm_net_virtual_tx_fn(const virtual_data_req_t *data_req,int8_t driver_id)
{
    return 0;
}

static int8_t test_phy_rf_virtual_rx(const uint8_t *data_ptr, uint16_t data_len,int8_t driver_id)
{

}

static phy_link_tx_status_e virtual_tx_done_status;
static bool virtual_cb_ok = false;

static int8_t test_phy_rf_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    if (status == virtual_tx_done_status) {
        virtual_cb_ok = true;
    } else {
        virtual_cb_ok = false;
    }

}

static uint16_t expected_virtual_rx_data_length = 0;
static bool virtual_rx_data_length_ok = false;

static int8_t test_phy_rf_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    //Validate proper data length here
    if (expected_virtual_rx_data_length == data_len) {
        virtual_rx_data_length_ok = true;
    } else {
        virtual_rx_data_length_ok = false;
    }
}

static int8_t test_phy_rf_virtual_tx(const virtual_data_req_t *data, int8_t driver_id){

    return 0;
}

static  arm_device_driver_list_s * rf_client_driver_allocate()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    phy_device_driver_s *dev_driver = malloc(sizeof(phy_device_driver_s));


    memset(driver, 0, sizeof(arm_device_driver_list_s));
    memset(dev_driver, 0, sizeof(phy_device_driver_s));

    driver->phy_driver = dev_driver;


    dev_driver->arm_net_virtual_rx_cb = &test_phy_rf_virtual_rx;
    dev_driver->phy_rx_cb = &test_phy_rf_rx;
    dev_driver->phy_tx_done_cb = &test_phy_rf_tx_done;
    dev_driver->arm_net_virtual_tx_cb = &test_phy_rf_virtual_tx;


    return driver;
}

void test_mac_rf_mac_class_free(protocol_interface_rf_mac_setup_s *rf_ptr) {
    if (rf_ptr->tun_extension_rf_driver) {
        free(rf_ptr->tun_extension_rf_driver->phy_driver);
        free(rf_ptr->tun_extension_rf_driver);
    }
}


int8_t test_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    return 1;
}

int8_t test_phy_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    return 1;
}

static void mlme_conf( const mac_api_t* api, mlme_primitive id, const void *data )
{
    visited = true;
}

static void mlme_ind_callback( const mac_api_t* api, mlme_primitive id, const void *data )
{

}

int8_t *test_state_control(phy_interface_state_e a, uint8_t b)
{
    visited = true;
    return 0;
}

int8_t *addr_write(phy_address_type_e a, uint8_t *b)
{
    return 0;
}

int8_t *ext(phy_extension_type_e a, uint8_t *b)
{
    if( b ){
        *b = 1;
    }
    return 0;
}

bool test_mac_mlme_link_beacon_join_priority_tx_callback_set()
{
    //TODO: test when refactor complete

    return true;
}

bool test_mac_mlme_link_beacon_compare_rx_callback_set()
{
    //TODO: test when refactor complete
    return true;
}

void fhss_sync_state_set(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id)
{
    (void)api;
    (void)fhss_state;
    (void)pan_id;
}


bool test_mac_mlme_scan_confirmation_handle()
{
    mac_mlme_scan_confirmation_handle(NULL);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    nsdynmemlib_stub.returnCounter = 1;
    mlme_scan_conf_t *conf = ns_dyn_mem_alloc(sizeof(mlme_scan_conf_t));
    memset(conf, 0, sizeof(mlme_scan_conf_t));
    setup.mac_mlme_scan_resp = conf;
    conf->ScanType = MAC_ACTIVE_SCAN;
    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    api.mlme_conf_cb = &mlme_conf;
    mac_mlme_scan_confirmation_handle(&setup);

    // Test with TUN extension
    nsdynmemlib_stub.returnCounter = 1;
    conf = ns_dyn_mem_alloc(sizeof(mlme_scan_conf_t));
    conf->ScanType = MAC_ACTIVE_SCAN;
    conf->ED_values = NULL;
    conf->ResultListSize = 0;
    setup.mac_mlme_scan_resp = conf;
    setup.tun_extension_rf_driver = rf_client_driver_allocate();
    mac_mlme_scan_confirmation_handle(&setup);
    test_mac_rf_mac_class_free(&setup);

    arm_device_driver_list_s driver;
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.arm_net_virtual_tx_cb = test_arm_net_virtual_tx_fn;
    driver.phy_driver = &phy_driver;
    setup.tun_extension_rf_driver = &driver;

    nsdynmemlib_stub.returnCounter = 1;
    conf = ns_dyn_mem_alloc(sizeof(mlme_scan_conf_t));
    memset(conf, 0, sizeof(mlme_scan_conf_t));
    nsdynmemlib_stub.returnCounter = 1;
    conf->ED_values = ns_dyn_mem_alloc(5);
    nsdynmemlib_stub.returnCounter = 1;
    conf->PAN_values[0] = ns_dyn_mem_alloc(sizeof(mlme_pan_descriptor_t));
    conf->ResultListSize = 1;
    setup.mac_mlme_scan_resp = conf;
    conf->ScanType = MAC_ED_SCAN_TYPE;
    mac_mlme_scan_confirmation_handle(&setup);

    return true;
}

bool test_mac_mlme_scan_request()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    nsdynmemlib_stub.returnCounter = 1;
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );

    nsdynmemlib_stub.returnCounter = 1;
    mlme_scan_conf_t *conf = ns_dyn_mem_alloc(sizeof(mlme_scan_conf_t));
    memset(conf, 0, sizeof(mlme_scan_conf_t));
    setup.mac_mlme_scan_resp = conf;
    mac_pre_build_frame_t fr;
    setup.active_pd_data_request = &fr;
    mlme_scan_t scan;
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    api.mlme_conf_cb = &mlme_conf;
    scan.ScanChannels.channel_page = 0;
    memset(&scan.ScanChannels.channel_mask, 0, 8);
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;
    conf = NULL;

    scan.ScanChannels.channel_mask[0] = 0x7000000; //0x7fff800
    scan.ScanType = MAC_ED_SCAN_TYPE;
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    scan.ScanChannels.channel_mask[0] = 0x7000000; //0x7fff800
    scan.ScanType = MAC_ED_SCAN_TYPE;
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_ETHERNET_TYPE;
    scan.ScanChannels.channel_mask[0] = 0;
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x7000000;
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x0f;
    nsdynmemlib_stub.returnCounter = 1;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x0f;
    nsdynmemlib_stub.returnCounter = 2;
    drv.state_control = NULL;
    setup.macRfRadioOn = true;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x0f;
    nsdynmemlib_stub.returnCounter = 2;
    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    scan.ScanType = MAC_ACTIVE_SCAN;
    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x0f;
    nsdynmemlib_stub.returnCounter = 2;
    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    scan.ScanChannels.channel_page = CHANNEL_PAGE_10;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    scan.ScanType = MAC_ORPHAN_SCAN;
    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x0f;
    nsdynmemlib_stub.returnCounter = 2;
    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    scan.ScanType = MAC_ACTIVE_SCAN;
    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    scan.ScanChannels.channel_mask[0] = 0x7000000;
    scan.ScanChannels.channel_page = CHANNEL_PAGE_0;
    nsdynmemlib_stub.returnCounter = 2;
    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    mac_mlme_scan_request(&scan, &setup);
    ns_dyn_mem_free( setup.mac_mlme_scan_resp->ED_values );
    ns_dyn_mem_free( setup.mac_mlme_scan_resp );
    setup.mac_mlme_scan_resp = NULL;

    return true;
}

bool test_mac_mlme_energy_scan_request()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    nsdynmemlib_stub.returnCounter = 1;
    mlme_scan_conf_t *conf = ns_dyn_mem_alloc(sizeof(mlme_scan_conf_t));
    memset(conf, 0, sizeof(mlme_scan_conf_t));
    mac_pre_build_frame_t fr;
    setup.active_pd_data_request = &fr;
    mlme_scan_t scan;
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    scan.ScanChannels.channel_page = 0;
    memset(&scan.ScanChannels.channel_mask, 0, 8);
    nsdynmemlib_stub.returnCounter = 1;
    scan.ScanType = MAC_ED_SCAN_TYPE;
    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    api.mlme_conf_cb = &mlme_conf;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_SUBGHZ_TYPE;

    // Page 0, channel 0, 868 Mhz BPSK, 20000 symbol rate
    scan.ScanChannels.channel_mask[0] = 0x00000001;
    scan.ScanChannels.channel_page = 0;
    scan.ScanDuration = 10;

    nsdynmemlib_stub.returnCounter = 2;
    mac_mlme_scan_request(&scan, &setup);

    // aBaseSuperframeDuration * (2^10 + 1) / 4,8ms (timer length)
    if (setup.mlme_ED_counter != 10250) {
        ns_dyn_mem_free( conf );
        return false;
    }

    ns_dyn_mem_free(setup.mac_mlme_scan_resp->ED_values);
    ns_dyn_mem_free(setup.mac_mlme_scan_resp);
    setup.mac_mlme_scan_resp = NULL;

    // Page 0, channel 1, 915 Mhz BPSK, 40000 symbol rate
    scan.ScanChannels.channel_mask[0] = 0x00000002;

    nsdynmemlib_stub.returnCounter = 2;
    mac_mlme_scan_request(&scan, &setup);

    // aBaseSuperframeDuration * (2^10 + 1) / 4,8ms (timer length)
    if (setup.mlme_ED_counter != 5125) {
        ns_dyn_mem_free( conf );
        return false;
    }

    ns_dyn_mem_free(setup.mac_mlme_scan_resp->ED_values);
    ns_dyn_mem_free(setup.mac_mlme_scan_resp);
    setup.mac_mlme_scan_resp = NULL;

    setup.dev_driver->phy_driver->link_type = PHY_LINK_15_4_2_4GHZ_TYPE;

    // Page 0, channel 11, 2450 Mhz O-QPSK, 62500 symbol rate
    scan.ScanChannels.channel_mask[0] = 0x00000800;

    nsdynmemlib_stub.returnCounter = 2;
    mac_mlme_scan_request(&scan, &setup);

    // aBaseSuperframeDuration * (2^10 + 1) / 4,8ms (timer length)
    if (setup.mlme_ED_counter != 3280) {
        ns_dyn_mem_free( conf );
        return false;
    }

    ns_dyn_mem_free(setup.mac_mlme_scan_resp->ED_values);
    ns_dyn_mem_free(setup.mac_mlme_scan_resp);
    setup.mac_mlme_scan_resp = NULL;

    ns_dyn_mem_free( conf );
    return true;
}

bool test_mac_mlme_start_req()
{
    if( -1 != mac_mlme_start_req(NULL, NULL) ){
        return false;
    }


    mac_pre_build_frame_t buf;
    memset(&buf, 0, sizeof(mac_pre_build_frame_t));

    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_start_t start;
    memset(&start, 0, sizeof(mlme_start_t));
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    drv.state_control = &test_state_control;

    start.PANCoordinator = true;
    start.BatteryLifeExtension = true;
    setup.active_pd_data_request = &buf;
    sw_mac_stub.mac_api_ptr = NULL;
    if( 0 != mac_mlme_start_req(&start, &setup) ){
        return false;
    }

    setup.active_pd_data_request = NULL;
    drv.address_write = &addr_write;
    start.PANCoordinator = false;
    start.BatteryLifeExtension = false;
    if( 0 != mac_mlme_start_req(&start, &setup) ){
        return false;
    }

    // FHSS TESTS START
    setup.fhss_api = ns_fhss_create(0, 0);
    if (0 != mac_mlme_start_req(&start, &setup)) {
        return false;
    }

    return true;
}

bool test_mac_mlme_reset()
{
    if( -1 != mac_mlme_reset(NULL, NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_reset_t reset;
    memset(&reset, 0, sizeof(mlme_reset_t));
    setup.mac_mlme_event = ARM_NWK_MAC_MLME_ED_ANALYZE;

    fhss_api_t api;
    setup.fhss_api = &api;
    setup.fhss_api->synch_state_set = &fhss_sync_state_set;

    reset.SetDefaultPIB = true;
    if (0 != mac_mlme_reset(&setup, &reset)) {
        return false;
    }

    return true;
}

bool test_mac_mlme_set_req()
{
    uint8_t beacon_payload[MAC_IEEE_802_15_4_MAX_BEACON_PAYLOAD_LENGTH];
    memset(beacon_payload, 0, MAC_IEEE_802_15_4_MAX_BEACON_PAYLOAD_LENGTH);
    uint8_t test_beacon_payload[MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE];
    if( -1 != mac_mlme_set_req(NULL, NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_set_t set;
    memset(&set, 0, sizeof(mlme_set_t));
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    drv.extension = ext;
    setup.mac_beacon_payload = beacon_payload;
    setup.max_beacon_payload_length = MAC_IEEE_802_15_4_MAX_BEACON_PAYLOAD_LENGTH;

    set.attr = macBeaconPayload;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = test_beacon_payload;

    set.value_size = 57;
    if( 0 == mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.value_size = 1;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = macCoordExtendedAddress;
    set.value_size = 8;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    uint8_t val[1];
    set.value_pointer = &val;

    set.attr = 0xfb;
    set.value_size = 0;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = 0xfc;
    set.value_size = 13;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    //Test buffer type values here -->
    uint8_t val_buf[8];
    set.value_pointer = &val_buf;
    set.value_size = 1;
    set.attr = macDeviceTable;

    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    mlme_device_descriptor_t desc;
    set.value_pointer = &desc;
    set.value_size = sizeof(mlme_device_descriptor_t);
    set.attr = macDeviceTable;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = &val_buf;
    set.value_size = 1;
    set.attr = macKeyTable;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    mlme_key_descriptor_entry_t entry;
    set.value_pointer = &entry;
    set.value_size = sizeof(mlme_key_descriptor_entry_t);
    set.attr = macKeyTable;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = &val_buf;
    set.value_size = 1;
    set.attr = macDefaultKeySource;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = &val_buf;
    set.value_size = 8;
    set.attr = macDefaultKeySource;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = &val_buf;
    set.value_size = 1;
    set.attr = macAutoRequestKeySource;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.value_pointer = &val_buf;
    set.value_size = 8;
    set.attr = macAutoRequestKeySource;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    // <-- Tested buffer type values here

    //default case: -->
    //Test all boolean set (mac_mlme_boolean_set) values here -->
    bool bval = true;
    set.value_pointer = &bval;
    set.value_size = 1;

    set.attr = macSecurityEnabled;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macRxOnWhenIdle;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macPromiscuousMode;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macGTSPermit;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAssociationPermit;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAssociatedPANCoord;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macTimestampSupported;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macBattLifeExt;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAutoRequest;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = macThreadForceLongAddressForBeacon;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    drv.extension = NULL;
    set.attr = macLoadBalancingAcceptAnyBeacon;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    drv.extension = ext;
    set.attr = macLoadBalancingAcceptAnyBeacon;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    //<-- Tested all boolean set values here (except default case)
    //Test all uint8_t* set (mac_mlme_8bit_set) values here -->
    set.value_size = 1;
    set.value_pointer = &val;
    set.attr = phyCurrentChannel;
    setup.mac_channel = 2;
    val[0] = 2;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    val[0] = 3;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    mac_pre_build_frame_t buf2;
    setup.active_pd_data_request = &buf2;
    val[0] = 4;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    setup.active_pd_data_request = NULL;

    set.attr = macMaxFrameRetries;
    setup.mac_mlme_retry_max = 3;
    val[0] = 5;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.mac_mlme_retry_max != 5) {
        return false;
    }

    val[0] = 8;

    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.mac_mlme_retry_max != 5) {
        return false;
    }

    setup.mac_channel = 2;
    mac_pre_build_frame_t frame;
    setup.active_pd_data_request = &frame;
    val[0] = 1;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = macBeaconPayloadLength;
    val[0] = 60;
    if( 0 == mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    val[0] = 1;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAutoRequestKeyIndex;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAutoRequestKeyIdMode;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.attr = macAutoRequestSecurityLevel;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }


    set.attr = 0xf0;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    //<-- Tested all uint8_t* set values here (also default)
    //Test all uint16_t* set (mac_mlme_16bit_set) values here -->
    set.value_size = 2;
    uint16_t val16;
    set.value_pointer = &val16;
    val16 = 0x6712;
    set.attr = macCoordShortAddress;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = macPANId;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }



    drv.link_type = PHY_LINK_TUN;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    drv.link_type = 0;

    drv.address_write = &addr_write;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.pan_id != val16) {
        return false;
    }

    set.attr = macShortAddress;
    val16 = 0x400;

    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.mac_short_address != val16) {
        return false;
    }

    val16 = 0xffff;

    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.mac_short_address != val16) {
        return false;
    }

    common_functions_stub.uint16_value = 0xffff;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    common_functions_stub.uint16_value = 0;

    set.attr = macSyncSymbolOffset;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = macTransactionPersistenceTime;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    set.attr = 0xfc;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    // <-- Tested all uint16_t* set (mac_mlme_16bit_set) values here
    //Test all uint32_t* set (mac_mlme_32bit_set) values here -->
    set.value_size = 4;
    uint32_t val32 = 0xac457733;
    set.value_pointer = &val32;
    set.attr = macFrameCounter;
    if( 0 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    if (setup.security_frame_counter != val32) {
        return false;
    }


    set.attr = 0xfc;
    if( -1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    // <-- Tested all uint32_t* set (mac_mlme_32bit_set) values here
    set.attr = 0xfd;
    set.value_size = 4;
    uint16_t tx_period = 100;
    set.value_pointer = &tx_period;
    if(-1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }
    set.value_size = 2;
    if(-1 != mac_mlme_set_req(&setup, &set) ){
        return false;
    }

    return true;
}

bool test_mac_mlme_get_req()
{
    if( -1 != mac_mlme_get_req(NULL, NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_get_conf_t get;
    memset(&get, 0, sizeof(mlme_get_conf_t));
    get.attr = macDeviceTable;
    get.value_pointer = NULL;
    mac_security_mib_stub.device_ptr = NULL;
    if( 0 != mac_mlme_get_req(&setup, &get) ){
        return false;
    }

    mlme_device_descriptor_t desc;
    mac_security_mib_stub.device_ptr = &desc;
    if( 0 != mac_mlme_get_req(&setup, &get) ){
        return false;
    }
    mac_security_mib_stub.device_ptr = NULL;

    get.attr = macFrameCounter;
    if( 0 != mac_mlme_get_req(&setup, &get) ){
        return false;
    }

    get.attr = macDefaultKeySource;
    if( 0 != mac_mlme_get_req(&setup, &get) ){
        return false;
    }

    setup.mac_mlme_retry_max = 5;
    get.attr = macMaxFrameRetries;
    if( 0 != mac_mlme_get_req(&setup, &get) ){
        return false;
    }
    uint8_t *ptr = get.value_pointer;
    if (get.value_size != 1 || *ptr != setup.mac_mlme_retry_max) {
        return false;
    }


    return true;
}

bool test_mac_extended_mac_set()
{
    mac_extended_mac_set(NULL, NULL);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    uint8_t mac[8];
    memset(&mac, 0, 8);
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    drv.address_write = &addr_write;
    mac_extended_mac_set(&setup, &mac);

    return true;
}

bool test_mac_mlme_poll_req()
{
    mac_mlme_poll_req(NULL, NULL);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_poll_t poll;
    memset(&poll, 0, sizeof(mlme_poll_t));
    setup.macDataPollReq = true;
    mac_mlme_poll_req(&setup, &poll);

    mac_mcps_sap_stub.frame_ptr = NULL;
    setup.macDataPollReq = false;
    mac_mlme_poll_req(&setup, &poll);

    mac_pre_build_frame_t frame;
    mac_mcps_sap_stub.frame_ptr = &frame;
    mac_header_helper_functions_stub.security_header.securityLevel = 3; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = 1;
    mac_header_helper_functions_stub.security_header.KeyIndex = 1;
    setup.mac_short_address = 0xffff;
    mac_mlme_poll_req(&setup, &poll);

    setup.macDataPollReq = false;
    setup.mac_short_address = 0x0015;
    mac_mlme_poll_req(&setup, &poll);

    return true;
}

bool test_mac_mlme_poll_process_confirm()
{
    mac_mlme_poll_process_confirm(NULL, 0);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_pre_build_frame_t frame;
    memset(&frame, 0, sizeof(mac_pre_build_frame_t));
    setup.active_pd_data_request = &frame;
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;

    sw_mac_stub.mac_api_ptr = NULL;
    mac_mlme_poll_process_confirm(&setup, 0);
    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    api.mlme_conf_cb = &mlme_conf;

    visited = false;
    mac_mlme_poll_process_confirm(&setup, 0);
    if( !visited ){
        return false;
    }

    return true;
}

bool test_mac_mlme_mac_radio_disabled()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    mac_mlme_mac_radio_disabled(NULL);

    mac_mlme_mac_radio_disabled(&setup);

    drv.link_type = PHY_LINK_TUN;
    mac_mlme_mac_radio_disabled(&setup);

    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;
    visited = false;
    mac_mlme_mac_radio_disabled(&setup);
    if( !visited ){
        return false;
    }

    return true;
}

bool test_mac_mlme_mac_radio_enable()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    mac_mlme_mac_radio_enable(NULL);

    mac_mlme_mac_radio_enable(&setup);

    drv.link_type = PHY_LINK_TUN;
    mac_mlme_mac_radio_enable(&setup);

    drv.state_control = &test_state_control;
    setup.macRfRadioOn = true;

    mac_mlme_mac_radio_enable(&setup);

    setup.macRfRadioOn = false;
    setup.macProminousMode = true;
    visited = false;
    mac_mlme_mac_radio_enable(&setup);
    if( !visited ){
        return false;
    }

    setup.macRfRadioOn = false;
    setup.macProminousMode = false;
    visited = false;
    mac_mlme_mac_radio_enable(&setup);
    if( !visited ){
        return false;
    }
    return true;
}

bool test_mac_mlme_rf_channel_change()
{
    if( -1 != mac_mlme_rf_channel_change(NULL, 1) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    drv.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    drv.extension = NULL;

    if( -1 != mac_mlme_rf_channel_change(&setup, 1) ){
        return false;
    }
    drv.link_type = PHY_LINK_TUN;
    if( 0 != mac_mlme_rf_channel_change(&setup, 1) ){
        return false;
    }

    drv.extension = &ext;
    setup.mac_channel = 1;
    if( 0 != mac_mlme_rf_channel_change(&setup, 1) ){
        return false;
    }

    setup.mac_channel = 3;
    if( 0 != mac_mlme_rf_channel_change(&setup, 1) ){
        return false;
    }

    return true;
}

bool test_mac_mlme_active_scan_response_timer_start()
{
    mac_mlme_active_scan_response_timer_start(NULL);

    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
    mac_mlme_active_scan_response_timer_start(&setup);
    return true;
}

bool test_mac_mlme_beacon_notify()
{
    if( -1 != mac_mlme_beacon_notify(NULL, NULL)){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.tun_extension_rf_driver = NULL;
    mlme_beacon_ind_t data;
    mac_api_t api;
    api.mlme_ind_cb = &mlme_ind_callback;
    sw_mac_stub.mac_api_ptr = &api;
    mlme_scan_conf_t conf;
    setup.mac_mlme_scan_resp = &conf;

    //Test add_or_update_beacon here completely -->
    mlme_pan_descriptor_t desc1;
    conf.PAN_values[0] = &desc1;
    conf.PAN_values[1] = NULL;
    conf.ResultListSize = 2;
    desc1.LogicalChannel = 11;
    data.PANDescriptor.LogicalChannel = 11;
    desc1.CoordPANId = 1;
    data.PANDescriptor.CoordPANId = 1;
    desc1.LinkQuality = 1;
    data.PANDescriptor.LinkQuality = 2;
    // Beacon data length doesn't contain FHSS synch info on single channel
    data.beacon_data_length = FHSS_SYNCH_INFO_START;

    //for loop covered here
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }

    conf.PAN_values[0] = NULL;
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }

    conf.ResultListSize = 2;
    nsdynmemlib_stub.returnCounter = 1;
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }

    // Test FHSS beacon on single channel
    data.beacon_data_length = FHSS_SYNCH_INFO_START + FHSS_SYNCH_INFO_LENGTH;
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }

    ns_dyn_mem_free(conf.PAN_values[2]);

    // FHSS TESTS START
    setup.fhss_api = ns_fhss_create(0, 0);
    nsdynmemlib_stub.returnCounter = 1;
    data.beacon_data_length = FHSS_SYNCH_INFO_START + FHSS_SYNCH_INFO_LENGTH;
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }
    // Beacon notify should cut synch info from data length
    if (data.beacon_data_length != FHSS_SYNCH_INFO_START) {
        return false;
    }

    // Test too short FHSS beacon
    data.beacon_data_length = FHSS_SYNCH_INFO_START;
    if(0 != mac_mlme_beacon_notify(&setup, &data)){
        return false;
    }

    ns_dyn_mem_free(conf.PAN_values[2]);

    //<-- Test add_or_update_beacon here completely

    return true;
}

bool test_mac_mlme_event_cb()
{
    mac_mlme_event_cb(NULL);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.mac_mlme_event = ARM_NWK_MAC_MLME_IDLE;
    mac_mlme_event_cb(&setup);

    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    setup.dev_driver = &list;
    drv.state_control = &test_state_control;

    //Test static mlme_scan_operation here -->
    setup.mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
    setup.scan_type = MAC_ED_SCAN_TYPE;
    setup.mac_channel_list.channel_page = CHANNEL_PAGE_1;
    setup.mac_channel_list.channel_mask[0] = 0x400;
    mlme_scan_conf_t conf;
    memset(&conf, 0, sizeof(mlme_scan_conf_t));
    uint8_t buf[20];
    memset(&buf, 0, 20);
    conf.ED_values = &buf;
    setup.mac_mlme_scan_resp = &conf;
    setup.mac_mlme_scan_resp->ResultListSize = MLME_MAC_RES_SIZE_MAX-1;
    mac_mlme_event_cb(&setup);

    setup.mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
    setup.mac_mlme_scan_resp->ResultListSize = 0;
    setup.scan_type = MAC_ACTIVE_SCAN;
    mac_mlme_event_cb(&setup);

    //Only way to test mac_mlme_energy_scan_start with radio ON
    setup.mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
    setup.scan_type = MAC_ED_SCAN_TYPE;
    setup.macRfRadioOn = true;
    setup.mac_mlme_scan_resp->ResultListSize = MLME_MAC_RES_SIZE_MAX-2;
    setup.mac_channel_list.channel_mask[0] = 0x04;
    setup.scan_duration = 1;
    mac_mlme_event_cb(&setup);

    //<-- Test static mlme_scan_operation here

    setup.mac_mlme_event = ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL;
    //No need to test inner function, handled elsewhere
    mac_mlme_event_cb(&setup);

    setup.mac_mlme_event = ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL_AFTER_DATA;
    mac_mlme_event_cb(&setup);

    return true;
}

bool test_mac_mlme_timer_cb()
{
    protocol_interface_rf_mac_setup_s *setup;
    uint8_t buf[8];
    memset(&buf, 0, 8);
    arm_device_driver_list_s list;
    phy_device_driver_s drv;

    phy_device_channel_page_s phy_channel_pages;
        phy_rf_channel_configuration_s rf_channel_configuration;
        rf_channel_configuration.datarate = 250000;
        rf_channel_configuration.number_of_channels = 16;
        rf_channel_configuration.modulation = M_OQPSK;
        rf_channel_configuration.channel_0_center_frequency = 2405000000;
        rf_channel_configuration.channel_spacing = 5000000;

        phy_channel_pages.channel_page = CHANNEL_PAGE_0;
        phy_channel_pages.rf_channel_configuration = &rf_channel_configuration;
    list.phy_driver = &drv;
    drv.extension = &ext;
    mac_security_mib_stub.int8_value = 0;
    drv.phy_MTU = MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE;
    drv.phy_channel_pages = &phy_channel_pages;
    mac_mcps_sap_stub.int8_value = 1;
    ns_timer_stub.cb_counter = 1;

    ns_timer_stub.int8_value = 1;
    nsdynmemlib_stub.returnCounter = 3;
    drv.address_write = &addr_write;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);

    if( ns_timer_stub.cb ){
        sw_mac_stub.rf_mac_ptr = NULL;
        ns_timer_stub.cb(1, 1);

        sw_mac_stub.rf_mac_ptr = setup;
        setup->mlme_tick_count = 0;
        setup->mac_mlme_event = ARM_NWK_MAC_MLME_ED_ANALYZE;
        setup->mlme_ED_counter = 2;
        setup->max_ED = 0;
        ns_timer_stub.cb(1, 1);

        setup->mlme_ED_counter = 0;
        ns_timer_stub.cb(1, 1);

        setup->mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
        ns_timer_stub.cb(1, 1);

        setup->mlme_tick_count = 1;
        ns_timer_stub.cb(1, 1);
    }

    mac_mlme_data_base_deallocate(setup);
    return true;
}

bool test_mac_mlme_set_active_state()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_set_active_state(&setup, true);
    return true;
}

bool test_mac_mlme_data_base_allocate()
{
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;

    protocol_interface_rf_mac_setup_s *setup = mac_mlme_data_base_allocate(NULL, NULL, &storage_sizes);
    if( setup ){
        return false;
    }
    uint8_t buf[8];
    memset(buf, 3, 8);
    arm_device_driver_list_s list;
    list.phy_driver = NULL;

    setup = mac_mlme_data_base_allocate(NULL, &list, &storage_sizes);
    if( setup ){
        return false;
    }

    setup = mac_mlme_data_base_allocate(&buf, &list, NULL);
    if( setup ){
        return false;
    }


    //Allocate Fail
    nsdynmemlib_stub.returnCounter = 0;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }
    phy_device_driver_s drv;
    phy_device_channel_page_s phy_channel_pages;
    phy_rf_channel_configuration_s rf_channel_configuration;
    rf_channel_configuration.datarate = 250000;
    rf_channel_configuration.number_of_channels = 16;
    rf_channel_configuration.modulation = M_OQPSK;
    rf_channel_configuration.channel_0_center_frequency = 2405000000;
    rf_channel_configuration.channel_spacing = 5000000;

    phy_channel_pages.channel_page = CHANNEL_PAGE_0;
    phy_channel_pages.rf_channel_configuration = &rf_channel_configuration;

    list.phy_driver = &drv;
    drv.phy_MTU = 0;
    drv.phy_channel_pages = &phy_channel_pages;
    nsdynmemlib_stub.returnCounter = 0;
    mac_security_mib_stub.int8_value = 1;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    setup = mac_mlme_data_base_allocate(&buf, &list,&storage_sizes);
    if( setup ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    mac_security_mib_stub.int8_value = 0;
    setup = mac_mlme_data_base_allocate(&buf, &list,&storage_sizes);
    if( setup ){
        return false;
    }

    drv.phy_MTU = MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE;
    mac_security_mib_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_mcps_sap_stub.int8_value = -1;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }


    mac_mcps_sap_stub.int8_value = -1;
    nsdynmemlib_stub.returnCounter = 3;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }

    mac_mcps_sap_stub.int8_value = 1;
    ns_timer_stub.int8_value = -1;
    nsdynmemlib_stub.returnCounter = 3;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( setup ){
        return false;
    }

    ns_timer_stub.int8_value = 1;
    nsdynmemlib_stub.returnCounter = 3;
    drv.address_write = &addr_write;
    setup = mac_mlme_data_base_allocate(&buf, &list, &storage_sizes);
    if( !setup ){
        return false;
    }

    mac_mlme_data_base_deallocate(setup);

    return true;
}

bool test_mac_mlme_data_base_deallocate()
{
    nsdynmemlib_stub.returnCounter = 1;
    protocol_interface_rf_mac_setup_s *setup = ns_dyn_mem_alloc(sizeof(protocol_interface_rf_mac_setup_s));
    memset(setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    nsdynmemlib_stub.returnCounter = 1;
    setup->dev_driver_tx_buffer.buf = ns_dyn_mem_alloc(12);
    arm_device_driver_list_s list;
    setup->dev_driver = &list;

    mac_mlme_data_base_deallocate(setup);

    return true;
}

bool test_mac_mlme_set_new_sqn()
{
    if( 0 != mac_mlme_set_new_sqn(NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    if( 1 != mac_mlme_set_new_sqn(&setup) ){
        return false;
    }

    return true;
}

bool test_mac_mlme_set_new_beacon_sqn()
{
    if( 0 != mac_mlme_set_new_beacon_sqn(NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    if( 1 != mac_mlme_set_new_beacon_sqn(&setup) ){
        return false;
    }

    return true;
}

bool test_mac_mlme_get_panid()
{
    uint16_t ret = mac_mlme_get_panid(NULL);
    if( 0xffff != ret ){
        return false;
    }

    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.pan_id = 0x1234;
    ret = mac_mlme_get_panid(&setup);
    if( 0x1234 != ret ){
        return false;
    }

    return true;
}

bool test_mac_frame_src_address_set_from_interface()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    uint8_t buf[100];
    memset(&buf, 0, 100);

    mac_frame_src_address_set_from_interface(MAC_ADDR_MODE_16_BIT, NULL, &buf);

    mac_frame_src_address_set_from_interface(MAC_ADDR_MODE_16_BIT, &setup, &buf);

    mac_frame_src_address_set_from_interface(MAC_ADDR_MODE_64_BIT, &setup, &buf);
    return true;
}

bool test_mac_mlme_beacon_tx()
{
    if( -1 != mac_mlme_beacon_tx(NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    //setup.beacon_join_priority_tx_cb_ptr = &beacon_join_pri_tx_cb;
    mac_mcps_sap_stub.frame_ptr = NULL;
    if( -1 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }
    setup.macCapCordinator = true;
    if( -1 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }




    mac_pre_build_frame_t fr;

    setup.active_pd_data_request = &fr;

    fr.next = NULL;
    fr.fcf_dsn.frametype = FC_BEACON_FRAME;
    if( -2 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }
    setup.active_pd_data_request = NULL;
    setup.pd_data_request_queue_to_go = &fr;

    if( -2 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }
    fr.fcf_dsn.frametype = FC_DATA_FRAME;

    mac_mcps_sap_stub.frame_ptr = &fr;
    setup.beaconSrcAddressModeLong = true;
    setup.macCapCordinator = true;
    setup.macCapBatteryPowered = true;
    setup.macCapAssocationPermit = true;
    uint8_t buf[100];
    memset(&buf, 0, 100);
    fr.mac_payload = &buf;
    common_functions_stub.uint8_ptr = &buf;
    setup.mac_beacon_payload_size = 1;
    setup.mac_beacon_payload = &buf;
    if( 0 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }
    setup.pd_data_request_queue_to_go = NULL;
    setup.active_pd_data_request = NULL;
    setup.beaconSrcAddressModeLong = false;
    setup.shortAdressValid = true;
    if( 0 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }
    setup.active_pd_data_request = NULL;
    setup.shortAdressValid = false;
    if( 0 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }

    // FHSS TESTS START
    setup.fhss_api = ns_fhss_create(0, 0);
    setup.active_pd_data_request = NULL;
    setup.shortAdressValid = false;
    if( 0 != mac_mlme_beacon_tx(&setup) ){
        return false;
    }

    return true;
}

bool test_mac_mlme_beacon_req_tx()
{
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    if( 0 != mac_mlme_beacon_req_tx(&setup) ){
        return false;
    }
    mac_pre_build_frame_t fr;
    mac_mcps_sap_stub.frame_ptr = &fr;

    if( 1 != mac_mlme_beacon_req_tx(&setup) ){
        return false;
    }

    return true;
}


bool test_mac_mlme_virtual_confirmation_handle()
{
    if (-1 != mac_mlme_virtual_confirmation_handle(-1, NULL, 0)) {
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    sw_mac_stub.rf_mac_ptr = &setup;

    //Stub A driver
    arm_device_driver_list_s driver;
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.arm_net_virtual_tx_cb = test_arm_net_virtual_tx_fn;
    driver.phy_driver = &phy_driver;
    setup.tun_extension_rf_driver = &driver;
    uint8_t test_data[10];
    test_data[0] = MLME_SCAN;

    // Fail memory allocation
    nsdynmemlib_stub.returnCounter = 0;
    if (-1 != mac_mlme_virtual_confirmation_handle(0, test_data, 0)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    if (0 != mac_mlme_virtual_confirmation_handle(0, test_data, 0)) {
        return false;
    }

    return true;
}
