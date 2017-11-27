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
#include "test_sw_mac.h"
#include <string.h>
#include "mac_api.h"
#include "fhss_api.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "sw_mac.h"
#include "mlme.h"
#include "rf_driver_storage_stub.h"
#include "nsdynmemLIB_stub.h"
#include "mac_mlme_stub.h"
#include "mac_security_mib_stub.h"
#include "mac_mcps_sap_stub.h"
#include "fhss_mac_interface_stub.h"

#define C_CHECK(condition) \
  do { if (!(condition)) { printf("ERROR: %s at file %s on line %d\n", #condition, __FILE__, __LINE__); return false; } } while(0)

void conf_cb( const mac_api_t* api, mlme_primitive id, const void *data )
{

}

static void internal_mib_observer_test(const mlme_set_t * set_req)
{

}

static void driver_storage_init(arm_device_driver_list_s *list)
{
    list->mlme_observer_cb = internal_mib_observer_test;
    list->id = 1;
    //Init stubs
    rf_driver_storage_stub.list_value = list;
    mac_security_mib_stub.int8_value = 0;
    mac_mcps_sap_stub.int8_value = 0;
}

bool test_ns_sw_mac_create()
{
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    mac_description_storage_size_t storage_sizes;
    rf_driver_storage_stub.list_value = NULL;
    mac_security_mib_stub.int8_value = 0;
    mac_mcps_sap_stub.int8_value = 0;
    mac_mlme_stub.setup_ptr = NULL;

    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;

    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }
    arm_device_driver_list_s list;
    driver_storage_init(&list);

    list.phy_driver = &drv;
    storage_sizes.key_description_table_size = 0;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }
    storage_sizes.device_decription_table_size = 0;
    storage_sizes.key_description_table_size = 3;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }
    storage_sizes.device_decription_table_size = 32;
    nsdynmemlib_stub.returnCounter = 0;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }

    //Test no MAC
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }
    //Set MAC and mac class allocate fail
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }

    //Test data base allocate fail
    //Enable data allocate for mac api
    nsdynmemlib_stub.returnCounter = 3;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( api ){
        return false;
    }

    //Test succesfully init
    protocol_interface_rf_mac_setup_s setup;
    mac_mlme_stub.setup_ptr = &setup;
    nsdynmemlib_stub.returnCounter = 3;
    api = ns_sw_mac_create(0, &storage_sizes);
    if( !api ){
        return false;
    }

    ns_dyn_mem_free(api);

    return true;
}

bool test_get_sw_mac_api()
{
    mac_mlme_stub.setup_ptr = NULL;
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;

    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;

    mac_api_t *orig = ns_sw_mac_create(0, &storage_sizes);


    mac_api_t *api = get_sw_mac_api(&setup);
    if( api ){
        return false;
    }


    orig->mac_initialize(orig, NULL, NULL, NULL, NULL, NULL, 0);

    api = get_sw_mac_api(&setup);
    if( !api ){
        return false;
    }

    ns_dyn_mem_free(orig);

    return true;
}

bool test_get_sw_mac_ptr_by_mac_api()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    protocol_interface_rf_mac_setup_s *ret = get_sw_mac_ptr_by_mac_api(NULL);
    if( ret ){
        return false;
    }

    ret = get_sw_mac_ptr_by_mac_api(api);
    if( !ret ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_get_sw_mac_ptr_by_fhss_api()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    fhss_api_t fhss_api;
    mac_mlme_stub.setup_ptr->fhss_api = &fhss_api;
    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    // Test NULL API
    protocol_interface_rf_mac_setup_s *ret = get_sw_mac_ptr_by_fhss_api(NULL);
    if (ret) {
        return false;
    }
    // Test proper API
    ret = get_sw_mac_ptr_by_fhss_api(&fhss_api);
    if (!ret) {
        return false;
    }
    // Verify pointer
    if (ret != mac_mlme_stub.setup_ptr) {
        return false;
    }
    ns_dyn_mem_free(api);
    return true;
}

bool test_get_sw_mac_ptr_by_timer()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    protocol_interface_rf_mac_setup_s *ret = get_sw_mac_ptr_by_timer(6, ARM_NWK_MLME_TIMER);
    C_CHECK( !ret );

    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    ret = get_sw_mac_ptr_by_timer(6, ARM_NWK_MLME_TIMER);
    C_CHECK( !ret );

    setup.mlme_timer_id = 6;
    ret = get_sw_mac_ptr_by_timer(6, ARM_NWK_MLME_TIMER);
    C_CHECK( ret );

    ret = get_sw_mac_ptr_by_timer(7, ARM_NWK_MAC_TIMER);
    C_CHECK( !ret );

    setup.mac_timer_id = 7;
    ret = get_sw_mac_ptr_by_timer(7, ARM_NWK_MAC_TIMER);
    C_CHECK( ret );

    ret = get_sw_mac_ptr_by_timer(8, ARM_NWK_CCA_TIMER);
    C_CHECK( !ret );

    setup.cca_timer_id = 8;
    ret = get_sw_mac_ptr_by_timer(8, ARM_NWK_CCA_TIMER);
    C_CHECK( ret );

    ret = get_sw_mac_ptr_by_timer(9, ARM_NWK_IFS_TIMER);
    C_CHECK( !ret );

    setup.ifs_timer_id = 9;
    ret = get_sw_mac_ptr_by_timer(9, ARM_NWK_IFS_TIMER);
    C_CHECK( ret );

    ns_dyn_mem_free(api);

    return true;
}

bool test_get_sw_mac_ptr_by_driver_id()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    protocol_interface_rf_mac_setup_s *ret = get_sw_mac_ptr_by_driver_id(0);
    if( ret ){
        return false;
    }

    ret = get_sw_mac_ptr_by_driver_id(1);
    if( !ret ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_initialize()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);

    if( -1 != api->mac_initialize(NULL, NULL, NULL, NULL, NULL, NULL, 0) ){
        return false;
    }

    if( 0 != api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}


bool test_ns_sw_mac_virtual_client_register()
{
    arm_device_driver_list_s list;
    mac_api_t tester_api;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;

    if (-1 != ns_sw_mac_virtual_client_register(NULL, 1) ) {
        return false;
    }
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);

    //This should not never fail
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    //SETDriver to NULL
    rf_driver_storage_stub.list_value = NULL;
    if (-1 != ns_sw_mac_virtual_client_register(api, 1) ) {
        return false;
    }
    //Succesfully test
    rf_driver_storage_stub.list_value = &list;
    if (0 !=ns_sw_mac_virtual_client_register(api, 1) ) {
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_ns_sw_mac_virtual_client_unregister()
{
    arm_device_driver_list_s list;
    mac_api_t tester_api;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;

    if (-1 != ns_sw_mac_virtual_client_unregister(NULL) ) {
        return false;
    }
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);

    //This should not never fail
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);
    //Register
    ns_sw_mac_virtual_client_register(api, 1);

    if (0 != ns_sw_mac_virtual_client_unregister(api) ) {
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_mlme_req()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, &conf_cb, NULL, 0);

    //Just run the unsupported cases
    api->mlme_req(NULL, MLME_ASSOCIATE, NULL);
    api->mlme_req(api, MLME_ASSOCIATE, NULL);
    api->mlme_req(api, MLME_DISASSOCIATE, NULL);
    api->mlme_req(api, MLME_GTS, NULL);
    api->mlme_req(api, MLME_RX_ENABLE, NULL);
    api->mlme_req(api, MLME_SYNC, NULL);
    api->mlme_req(api, 254, NULL);

    mac_mlme_stub.int8_value = 0;
    mlme_get_t get_data;
    api->mlme_req(api, MLME_GET, &get_data);

    mlme_reset_t reset_data;
    api->mlme_req(api, MLME_RESET, &reset_data);

    mlme_scan_t scan_data;
    api->mlme_req(api, MLME_SCAN, &scan_data);

    mlme_set_t set_data;
    api->mlme_req(api, MLME_SET, &set_data);
    set_data.attr = macLoadBalancingBeaconTx;
    api->mlme_req(api, MLME_SET, &set_data);

    mlme_start_t start_data;
    api->mlme_req(api, MLME_START, &start_data);

    mlme_poll_t poll_data;
    api->mlme_req(api, MLME_POLL, &poll_data);

    ns_dyn_mem_free(api);
    return true;
}

bool test_mcps_data_req()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    mcps_data_req_t data;
    api->mcps_data_req(api, &data);

    ns_dyn_mem_free(api);
    return true;
}

bool test_mac64_get()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
        memset(mac64, 0, 8);
        drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);

    memset(&setup.mac64, 0, 8);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    if( -1 != api->mac64_get(NULL, MAC_EXTENDED_READ_ONLY, NULL)){
        return false;
    }

    uint8_t buf[8];
    memset(&buf, 0, 8);
    drv.PHY_MAC = &buf;
    if( 0 != api->mac64_get(api, MAC_EXTENDED_READ_ONLY, &buf)){
        return false;
    }

    if( 0 != api->mac64_get(api, MAC_EXTENDED_DYNAMIC, &buf)){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_mac64_set()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    if( -1 != api->mac64_set(NULL, NULL)){
        return false;
    }

    uint8_t buf[8];
    memset(&buf, 0, 8);
    if( 0 != api->mac64_set(api, &buf)){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_sw_mac_net_phy_rx()
{
//    rf_driver_storage_stub.rx_cb = rx_cb;
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;
    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;
    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    rf_driver_storage_stub.list_value = NULL;

    if( -1 != rf_driver_storage_stub.rx_cb(NULL,0,0,0,0) ){
        return false;
    }

    rf_driver_storage_stub.list_value = &list;
    if( 0 != rf_driver_storage_stub.rx_cb(NULL,0,0,0,1) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_sw_mac_net_phy_tx_done()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;

    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;


    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;

    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    rf_driver_storage_stub.list_value = NULL;

    if( -1 != rf_driver_storage_stub.done_cb(0,0,PHY_LINK_TX_SUCCESS,0,0) ){
        return false;
    }

    rf_driver_storage_stub.list_value = &list;
    if( 0 != rf_driver_storage_stub.done_cb(1,0,PHY_LINK_TX_SUCCESS,0,0) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}



bool test_sw_mac_storage_decription_sizes_get()
{
    arm_device_driver_list_s list;
    driver_storage_init(&list);
    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    list.phy_driver = &drv;

    nsdynmemlib_stub.returnCounter = 3;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    setup.device_description_table_size = 32;
    setup.key_description_table_size = 3;
    setup.key_lookup_list_size = 1;
    setup.key_usage_list_size = 3;


    mac_mlme_stub.setup_ptr = &setup;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;

    mac_description_storage_size_t test_storage_sizes;
    memset(&test_storage_sizes, 0, sizeof(mac_description_storage_size_t));
    mac_api_t *api = ns_sw_mac_create(0, &storage_sizes);
    setup.dev_driver = &list;

    api->mac_initialize(api, NULL, NULL, NULL, NULL, NULL, 0);

    if (api->mac_storage_sizes_get(NULL, &test_storage_sizes) != -1) {
        return false;
    }

    if (api->mac_storage_sizes_get(api, NULL) != -1) {
        return false;
    }

    if (api->mac_storage_sizes_get(api, &test_storage_sizes) != 0) {
        return false;
    }

    if (test_storage_sizes.device_decription_table_size != 32 || test_storage_sizes.key_description_table_size != 3 ||
        test_storage_sizes.key_lookup_size != 1 || test_storage_sizes.key_usage_size != 3) {
        return false;
    }

    ns_dyn_mem_free(api);
    return true;

}

bool test_ns_sw_mac_fhss_register()
{
    fhss_api_t fhss_api;
    memset(&fhss_api, 0, sizeof(fhss_api_t));
    mac_api_t dummy_mac_api;
    memset(&dummy_mac_api, 0, sizeof(mac_api_t));
    mac_description_storage_size_t storage_sizes;
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    fhss_api.init_callbacks = &fhss_init_callbacks_cb;

    phy_device_driver_s drv;
    memset(&drv, 0, sizeof(phy_device_driver_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    drv.PHY_MAC = mac64;
    arm_device_driver_list_s list;
    memset(&list, 0, sizeof(arm_device_driver_list_s));
    driver_storage_init(&list);
    list.phy_driver = &drv;

    memset(&dummy_mac_api, 0, sizeof(mac_api_t));

    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_mlme_stub.setup_ptr = &setup;
    nsdynmemlib_stub.returnCounter = 3;
    mac_api_t *mac_api = ns_sw_mac_create(0, &storage_sizes);
    if( !mac_api ){
        return false;
    }

    // Test without MAC API
    if (ns_sw_mac_fhss_register(NULL, &fhss_api) != -1) {
        return false;
    }
    // Test without FHSS API
    if (ns_sw_mac_fhss_register(mac_api, NULL) != -1) {
        return false;
    }

    // Test with dummy MAC API
    if (ns_sw_mac_fhss_register(&dummy_mac_api, &fhss_api) != -2) {
        return false;
    }

    // Test with proper API's
    if (ns_sw_mac_fhss_register(mac_api, &fhss_api) != 0) {
        return false;
    }
    ns_dyn_mem_free(mac_api);
    return true;
}
