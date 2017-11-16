/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_ethernet_mac_api.h"
#include "ethernet_mac_api.h"
#include <string.h>
#include "rf_driver_storage_stub.h"
#include "nsdynmemLIB_stub.h"
#include "platform/arm_hal_phy.h"
#include "eventOS_event.h"
#include "event_stub.h"
#include "common_functions_stub.h"

bool cb_visited = false;

static void confirm_test_cb( const eth_mac_api_t* api, const eth_data_conf_t *data )
{
    cb_visited = true;
}

void indication_test_cb( const eth_mac_api_t* api, const eth_data_ind_t *data )
{
    cb_visited = true;
}

static int8_t mac_state_control(phy_interface_state_e a, uint8_t b)
{

}

static int8_t mac_tx(uint8_t *a, uint16_t b, uint8_t c)
{

}

static int8_t mac_address_write(phy_address_type_e a,uint8_t *b)
{

}

static int8_t mac_extension(phy_extension_type_e a,uint8_t *b)
{

}

static int8_t addr_write_cb(phy_address_type_e a, uint8_t *b)
{

}

static uint8_t mymac[8];

static phy_device_driver_s device_driver = {
.PHY_MAC = mymac,
.link_type = PHY_LINK_ETHERNET_TYPE,
.driver_description = "eth_test_iface",
.phy_MTU = 1280,
.phy_tail_length = 0,
.phy_header_length = 0,
.state_control = mac_state_control,
.tx = mac_tx,
.address_write = mac_address_write,
.extension = mac_extension,
};

bool test_ethernet_mac_create()
{
    rf_driver_storage_stub.list_value = NULL;
    eth_mac_api_t *api = ethernet_mac_create(0);
    if( api ){
        return NULL;
    }

    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;

    nsdynmemlib_stub.returnCounter = 0;
    nsdynmemlib_stub.expectedPointer = NULL;

    api = ethernet_mac_create(0);
    if( api ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 0;
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    phy_driver.link_type = PHY_LINK_SLIP;
    api = ethernet_mac_create(0);
    if( api ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_TUN;
    nsdynmemlib_stub.returnCounter = 0;
    api = ethernet_mac_create(0);
    if( api ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    nsdynmemlib_stub.returnCounter = 0;
    api = ethernet_mac_create(0);
    if( api ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_ETHERNET_TYPE;
    nsdynmemlib_stub.returnCounter = 1;
    api = ethernet_mac_create(0);
    if( api ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    api = ethernet_mac_create(0);
    if( !api ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);

    return true;
}

bool test_ethernet_mac_destroy()
{
    if( -1 != ethernet_mac_destroy(NULL)) {
        return false;
    }
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;
    nsdynmemlib_stub.returnCounter = 2;
    eth_mac_api_t *api = ethernet_mac_create(0);

    if( 0!= ethernet_mac_destroy(api) ){
        return false;
    }
    free(driver);
    return true;
}

bool test_data_request()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;
    nsdynmemlib_stub.returnCounter = 2;
    eth_mac_api_t *api = ethernet_mac_create(0);

    api->data_req(NULL, NULL);

    api->data_req(api, NULL);

    driver->phy_driver = &device_driver;
    rf_driver_storage_stub.list_value = driver;

    api->data_req(api, NULL);

    driver->phy_driver = &device_driver;
    rf_driver_storage_stub.list_value = driver;

    uint8_t databuf[100];
    eth_data_req_t data;
    memset(&data, 0, sizeof(eth_data_req_t));
    data.dstAddress = databuf;
    data.srcAddress = databuf;
    data.msdu = data.dstAddress = databuf;

    device_driver.link_type = PHY_LINK_ETHERNET_TYPE;
    data.msduLength = 1600;
    api->data_req(api, &data);

    data.msduLength = 100;
    api->data_req(api, &data);

    device_driver.link_type = PHY_LINK_TUN;
    data.msduLength = 1600;
    api->data_req(api, &data);

    data.msduLength = 100;
    api->data_req(api, &data);

    device_driver.link_type = PHY_LINK_SLIP;
    data.msduLength = 100;
    api->data_req(api, &data);

    device_driver.link_type = PHY_LINK_ETHERNET_TYPE;

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

#define ETH_INIT_EVENT 0
#define ETH_DATA_IND_EVENT 1
#define ETH_DATA_CNF_EVENT 2

bool test_data_confirm()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    nsdynmemlib_stub.returnCounter = 2;
    rf_driver_storage_stub.list_value = driver;
    eth_mac_api_t *api = ethernet_mac_create(0);
    api->mac_initialize(api, confirm_test_cb, NULL , 0);
    cb_visited = false;

    arm_event_s event;
    //NOTE: Specially test init here (does nothing)
    event.event_type = ETH_INIT_EVENT;
    event_stub.func_ptr(&event);

    event.event_type = ETH_DATA_CNF_EVENT;
    nsdynmemlib_stub.returnCounter = 1;
    eth_data_conf_t *data = ns_dyn_mem_temporary_alloc(sizeof(eth_data_conf_t));
    event.data_ptr = data;
    event_stub.func_ptr(&event);

    if( !cb_visited ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_data_indication()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    nsdynmemlib_stub.returnCounter = 2;
    rf_driver_storage_stub.list_value = driver;
    eth_mac_api_t *api = ethernet_mac_create(0);
    api->mac_initialize(api, NULL, indication_test_cb, 0);
    cb_visited = false;

    arm_event_s event;
    event.event_type = ETH_DATA_IND_EVENT;
    nsdynmemlib_stub.returnCounter = 1;
    eth_data_ind_t *data = ns_dyn_mem_temporary_alloc(sizeof(eth_data_ind_t));
    data->msdu = NULL;
    event.data_ptr = data;
    event_stub.func_ptr(&event);

    if( !cb_visited ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_eth_mac_mac48_address_set()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;
    phy_driver.address_write = &addr_write_cb;
    nsdynmemlib_stub.returnCounter = 2;
    eth_mac_api_t *api = ethernet_mac_create(0);

    uint8_t buf[8];
    int8_t ret = api->mac48_set(NULL, &buf);
    if( -1 != ret ){
        return false;
    }

    ret = api->mac48_set(api, &buf);
    if( 0 != ret ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_eth_mac_mac48_address_get()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;
    nsdynmemlib_stub.returnCounter = 2;
    eth_mac_api_t *api = ethernet_mac_create(0);

    uint8_t buf[8];
    int8_t ret = api->mac48_get(NULL, &buf);
    if( -1 != ret ){
        return false;
    }

    ret = api->mac48_get(api, &buf);
    if( 0 != ret ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_eth_mac_api_initialize()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    rf_driver_storage_stub.list_value = driver;
    nsdynmemlib_stub.returnCounter = 2;
    eth_mac_api_t *api = ethernet_mac_create(0);

    int8_t ret = api->mac_initialize(NULL, NULL, NULL, 0);
    if( -1 != ret ){
        return false;
    }

    ret = api->mac_initialize(api, NULL, NULL, 0);
    if( 0 != ret ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_rx_cb()
{
    event_stub.int8_value = 0;
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    nsdynmemlib_stub.returnCounter = 2;
    rf_driver_storage_stub.list_value = driver;
    eth_mac_api_t *api = ethernet_mac_create(0);
    api->mac_initialize(api, confirm_test_cb, indication_test_cb, 0);
    rf_driver_storage_stub.list_value = NULL;

    uint8_t data[20];
    int8_t ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        return false;
    }

    rf_driver_storage_stub.list_value = driver;
    nsdynmemlib_stub.returnCounter = 0;
    ret = rf_driver_storage_stub.rx_cb(&data, 0, 0, 0, 0);
    if( -1 != ret ){
        return false;
    }

    ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_ETHERNET_TYPE;
    nsdynmemlib_stub.returnCounter = 1;
    ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_ETHERNET_TYPE;
    nsdynmemlib_stub.returnCounter = 2;
    ret = rf_driver_storage_stub.rx_cb(&data, 20, 0, 0, 0);
    if( 0 != ret ){
        return false;
    }

    phy_driver.link_type = PHY_LINK_TUN;
    event_stub.int8_value = -1;
    event_stub.ignore_callback = true;
    nsdynmemlib_stub.returnCounter = 2;
    ret = rf_driver_storage_stub.rx_cb(&data, 3, 0, 0, 0);
    if( -1 != ret ){
        event_stub.ignore_callback = false;
        return false;
    }

    common_functions_stub.uint16_value = 3;
    phy_driver.tunnel_type = 7;
    phy_driver.link_type = PHY_LINK_TUN;
    nsdynmemlib_stub.returnCounter = 1;
    ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        event_stub.ignore_callback = false;
        return false;
    }

    common_functions_stub.uint16_value = 7;
    phy_driver.link_type = PHY_LINK_SLIP;
    nsdynmemlib_stub.returnCounter = 1;
    ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        event_stub.ignore_callback = false;
        return false;
    }

    phy_driver.link_type = PHY_LINK_TUN;
    nsdynmemlib_stub.returnCounter = 2;
    ret = rf_driver_storage_stub.rx_cb(&data, 10, 0, 0, 0);
    if( -1 != ret ){
        event_stub.ignore_callback = false;
        return false;
    }
    event_stub.ignore_callback = false;
    ethernet_mac_destroy(api);
    free(driver);
    return true;
}

bool test_tx_done_cb()
{
    event_stub.int8_value = 0;
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    memset(driver, 0, sizeof(arm_device_driver_list_s));
    uint8_t mac64[8];
    memset(mac64, 0, 8);
    phy_device_driver_s phy_driver;
    memset(&phy_driver, 0, sizeof(phy_device_driver_s));
    phy_driver.PHY_MAC = &mac64;
    driver->phy_driver = &phy_driver;
    nsdynmemlib_stub.returnCounter = 2;
    rf_driver_storage_stub.list_value = driver;
    eth_mac_api_t *api = ethernet_mac_create(0);
    api->mac_initialize(api, confirm_test_cb, indication_test_cb, 0);
    rf_driver_storage_stub.list_value = NULL;

    int8_t ret = rf_driver_storage_stub.done_cb(0, 0, PHY_LINK_TX_DONE, 0, 0);
    if( -1 != ret ){
        return false;
    }

    driver->phy_driver = &device_driver;
    rf_driver_storage_stub.list_value = driver;
    ret = rf_driver_storage_stub.done_cb(0, 0, PHY_LINK_TX_DONE, 0, 0);
    if( 0 != ret ){
        return false;
    }
    uint8_t databuf[100];
    eth_data_req_t data;
    memset(&data, 0, sizeof(eth_data_req_t));
    data.dstAddress = databuf;
    data.srcAddress = databuf;
    data.msdu = data.dstAddress = databuf;

    data.msduLength = 100;

    api->data_req(api, &data);
    event_stub.int8_value = 12;
    ret = rf_driver_storage_stub.done_cb(0, 0, PHY_LINK_TX_DONE, 0, 0);
    if( -1 != ret ){
        return false;
    }

    api->data_req(api, &data);
    nsdynmemlib_stub.returnCounter = 1;
    ret = rf_driver_storage_stub.done_cb(0, 0, PHY_LINK_TX_DONE, 0, 0);
    if( 12 != ret ){
        return false;
    }

    ethernet_mac_destroy(api);
    free(driver);
    return true;
}
