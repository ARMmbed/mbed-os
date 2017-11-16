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
#include "test_rf_driver_storage.h"
#include <string.h>
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "protocol_tun_driver_stub.h"
#include "nsdynmemLIB_stub.h"

typedef struct {
    phy_device_driver_s *drv;
    ns_list_link_t link;
} test_s;

static NS_LIST_DEFINE(test_list, test_s, link);

int8_t test_control(phy_interface_state_e a, uint8_t b){
    return 1;
}

bool test_arm_net_phy_register()
{
    int8_t ret = arm_net_phy_register(NULL);
    if( ret != -1 ){
        return false;
    }

    phy_device_driver_s driver;

    driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    driver.phy_channel_pages = NULL;
    ret = arm_net_phy_register(&driver);
    if( ret != -1 ){
        return false;
    }


    phy_device_channel_page_s phy_channel_pages;
    phy_rf_channel_configuration_s rf_channel_configuration;
    memset(&rf_channel_configuration, 0, sizeof(phy_rf_channel_configuration_s));

    driver.phy_channel_pages = &phy_channel_pages;
    phy_channel_pages.rf_channel_configuration = NULL;
    ret = arm_net_phy_register(&driver);
    if( ret != -1 ){
        return false;
    }

    phy_channel_pages.rf_channel_configuration = &rf_channel_configuration;


    protocol_tun_driver_stub.int8_value = -1;
    ret = arm_net_phy_register(&driver);
    if( ret != -2 ){
        return false;
    }

    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    ret = arm_net_phy_register(&driver);
    if( ret != 0 ){
        return false;
    }

    //Test re-register
    ret = arm_net_phy_register(&driver);
    if( ret != -1 ){
        return false;
    }

    arm_net_phy_unregister(0);

    //test Id's run out scenario (quite useless test)
    for (int16_t id = 0; id <= INT8_MAX+1; id++) {
        nsdynmemlib_stub.returnCounter = 1;
        test_s *test = ns_dyn_mem_alloc(sizeof(test_s));
        memset(test, 0, sizeof(test_s));
        nsdynmemlib_stub.returnCounter = 1;
        phy_device_driver_s *drv = ns_dyn_mem_alloc(sizeof(phy_device_driver_s));
        memset(drv, 0, sizeof(phy_device_driver_s));
        drv->state_control = NULL;
        drv->phy_MTU = id;
        test->drv = drv;
        ns_list_add_to_end(&test_list, test);

        nsdynmemlib_stub.returnCounter = 1;
        ret = arm_net_phy_register(drv);
        if( id <= INT8_MAX && id != ret ){
            return false;
        }
    }
    if( ret != -1 ){
        return false;
    }

    for (int16_t id = 0; id <= INT8_MAX; id++) {
        arm_net_phy_unregister(id);
    }
    arm_net_phy_unregister(-1);

    ns_list_foreach_safe(test_s, d, &test_list){
        ns_list_remove(&test_list, d);
        ns_dyn_mem_free(d->drv);
        ns_dyn_mem_free(d);
        d = NULL;
    }


    return true;
}

bool test_arm_net_phy_init()
{
    int8_t ret = arm_net_phy_init(NULL, NULL, NULL);
    if( -1 != ret ){
        return false;
    }
    phy_device_driver_s drv;
    ret = arm_net_phy_init(&drv, NULL, NULL);
    if( 0 != ret ){
        return false;
    }
    return true;
}

bool test_arm_net_phy_driver_pointer()
{
    arm_device_driver_list_s *list = arm_net_phy_driver_pointer(0);
    if( list != NULL ){
        return false;
    }
    return true;
}

//bool test_dev_get_phy_datarate()
//{
//    protocol_interface_rf_mac_setup_s cur;
//    arm_device_driver_list_s list;
//    cur.dev_driver = &list;
//    phy_device_driver_s drv;
//    cur.dev_driver->phy_driver = &drv;
//    phy_device_channel_page_s page[] = {{CHANNEL_PAGE_0, NULL}};
//    cur.dev_driver->phy_driver->phy_channel_pages = &page;
//    uint8_t ret = dev_get_phy_datarate(&cur);
//    if( 0 != ret ) {
//        return false;
//    }

//    const phy_rf_channel_configuration_s test_conf = {1, 2, 3, 4, M_UNDEFINED};
//    phy_device_channel_page_s page2[] = {{CHANNEL_PAGE_2, &test_conf}, {CHANNEL_PAGE_6, &test_conf}};
//    cur.dev_driver->phy_driver->phy_channel_pages = &page2;
//    cur.mac_channel_list.channel_page = CHANNEL_PAGE_6;

//    ret = dev_get_phy_datarate(&cur);
//    if( 3 != ret ) {
//        return false;
//    }

//    return true;
//}

bool test_arm_net_phy_mac64_set()
{
    if( -1 != arm_net_phy_mac64_set(NULL, 0) ){
        return false;
    }
    phy_device_driver_s driver;
    uint8_t mac[8];
    driver.PHY_MAC = &mac;
    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    int8_t ret = arm_net_phy_register(&driver);
    if( ret != 0 ){
        return false;
    }
    if( 0 != arm_net_phy_mac64_set(&mac, 0) ){
        return false;
    }

    arm_net_phy_unregister(0);

    return true;
}

bool test_arm_net_phy_mac64_get()
{
    if( arm_net_phy_mac64_get(0) ){
        return false;
    }
    phy_device_driver_s driver;
    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    int8_t ret = arm_net_phy_register(&driver);
    if( ret != 0 ){
        return false;
    }
    if( !arm_net_phy_mac64_get(0) ){
        return false;
    }

    arm_net_phy_unregister(0);

    return true;
}

bool test_arm_net_phy_rf_type()
{
    if( -1 != arm_net_phy_rf_type(0) ){
        return false;
    }
    phy_device_driver_s driver;
    driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    phy_device_channel_page_s phy_channel_pages;
    phy_rf_channel_configuration_s rf_channel_configuration;
    memset(&rf_channel_configuration, 0, sizeof(phy_rf_channel_configuration_s));
    phy_channel_pages.rf_channel_configuration = &rf_channel_configuration;
    driver.phy_channel_pages = &phy_channel_pages;

    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    int8_t ret = arm_net_phy_register(&driver);
    if( ret != 0 ){
        return false;
    }
    if( PHY_LINK_15_4_2_4GHZ_TYPE != arm_net_phy_rf_type(0) ){
        return false;
    }

    arm_net_phy_unregister(0);

    return true;
}
#define MLE_RAND_LOW   0x7333 // 1 - 0.1; minimum for "1+RAND"
#define MLE_RAND_HIGH  0x8CCD // 1 + 0.1; maximum for "1+RAND"
bool test_arm_net_phy_mtu_size()
{
    if( 0 != arm_net_phy_mtu_size(0) ){
        return false;
    }
    phy_device_driver_s driver;
    driver.phy_MTU = 7;
    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    int8_t ret = arm_net_phy_register(&driver);
    if( ret != 0 ){
        return false;
    }
    if( 7 != arm_net_phy_mtu_size(0) ){
        return false;
    }

    arm_net_phy_unregister(0);

    return true;
}

static void teset_mib_observer(const mlme_set_t * set_req)
{

}

bool test_arm_net_observer_cb_set()
{
    phy_device_driver_s driver;
    protocol_tun_driver_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    driver.state_control = &test_control;
    protocol_tun_driver_stub.int8_value = 0;
    arm_net_phy_register(&driver);


    arm_net_observer_cb_set(1, teset_mib_observer);

    arm_net_observer_cb_set(0, teset_mib_observer);

    arm_net_phy_unregister(0);

    return true;
}
