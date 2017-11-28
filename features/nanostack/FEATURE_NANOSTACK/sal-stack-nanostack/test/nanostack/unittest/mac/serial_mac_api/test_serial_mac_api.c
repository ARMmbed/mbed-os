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
#include "test_serial_mac_api.h"
#include <string.h>
#include "serial_mac_api.h"
#include "eventOS_event.h"

#include "rf_driver_storage_stub.h"
#include "nsdynmemLIB_stub.h"
#include "event_stub.h"

bool visited = false;
int8_t tx_value = 0;

int8_t tx_cb(uint8_t *a, uint16_t b, uint8_t c, data_protocol_e d)
{
    return tx_value;
}

void data_ind_cb(const serial_mac_api_t *api, const uint8_t *data_ptr, uint16_t data_length)
{
    visited = true;
}

int8_t virt_rx_cb(const uint8_t *data_ptr, uint16_t data_len,int8_t driver_id)
{
    visited = true;
    return 0;
}

bool test_serial_mac_create()
{
    rf_driver_storage_stub.list_value = NULL;
    if( serial_mac_create(0) ){
        return false;
    }

    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    if( serial_mac_create(0) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *ret = serial_mac_create(0);
    if( !ret ){
        return false;
    }

    if( event_stub.func_ptr ){
        arm_event_s ev;
        ev.event_type = 100;
        event_stub.func_ptr(&ev);
    }

    ns_dyn_mem_free(ret);

    return true;
}

bool test_data_request()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    drv.tx = &tx_cb;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if( -1 != api->data_request_cb(NULL, NULL, 0) ){
        return false;
    }

    if( -2 != api->data_request_cb(api, NULL, 0) ){
        return false;
    }

    //Test generic_data_request here -->
    //serial_mac_buffer_get cannot be tested here, see virtual data request test
    uint8_t buf[20];
    memset(&buf, 0, 20);
    if( -1 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    tx_value = 1;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    if( event_stub.func_ptr ){ //cleanup of request
        arm_event_s ev;
        ev.event_type = 102; //SERIAL_DATA_CNF_EVENT
        event_stub.func_ptr(&ev);
    }

    tx_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    if( event_stub.func_ptr ){ //cleanup of request
        arm_event_s ev;
        ev.event_type = 102; //SERIAL_DATA_CNF_EVENT
        event_stub.func_ptr(&ev);

        event_stub.func_ptr(&ev);

        event_stub.func_ptr(&ev);

        event_stub.func_ptr(&ev);
    }
    //<-- Test generic_data_request here

    ns_dyn_mem_free(api);

    return true;
}

//NOTE: This must match with internal struct!!!!
typedef struct data_ind_s {
    uint16_t msduLength;
    uint8_t *msdu;
}test_data_ind_t;


bool test_data_indication()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    drv.arm_net_virtual_rx_cb = &virt_rx_cb;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if (api->mac_initialize(api, &data_ind_cb) != 0) {
        return false;
    }
    if (api->virtual_initilize(api, 0) != 0) {
        return false;
    }

    if( event_stub.func_ptr ){ //cleanup of request
        arm_event_s ev;
        ev.event_type = 101; //SERIAL_DATA_CNF_EVENT
        nsdynmemlib_stub.returnCounter = 1;
        test_data_ind_t *ind = ns_dyn_mem_temporary_alloc(sizeof(test_data_ind_t));
        nsdynmemlib_stub.returnCounter = 1;
        uint8_t *buf = ns_dyn_mem_temporary_alloc(20);
        ind->msdu = buf;
        buf[0] = 0;
        ind->msduLength = 20;
        ev.data_ptr = ind;
        visited = false;
        event_stub.func_ptr(&ev);
        if( !visited ){
            return false;
        }

        nsdynmemlib_stub.returnCounter = 1;
        ind = ns_dyn_mem_temporary_alloc(sizeof(test_data_ind_t));
        nsdynmemlib_stub.returnCounter = 1;
        buf = ns_dyn_mem_temporary_alloc(20);
        ind->msdu = buf;
        buf[0] = 1;
        ind->msduLength = 20;
        ev.data_ptr = ind;
        visited = false;
        event_stub.func_ptr(&ev);
        if( !visited ){
            return false;
        }
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_serial_mac_api_initialize()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if( -1 != api->mac_initialize(NULL, NULL) ){
        return false;
    }

    if( 0 != api->mac_initialize(api, NULL) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_serial_mac_virtual_initialize()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if( -1 != api->virtual_initilize(NULL, 0) ){
        return false;
    }

    drv.arm_net_virtual_rx_cb = NULL;
    if( -1 != api->virtual_initilize(api, 0) ){
        return false;
    }

    drv.arm_net_virtual_rx_cb = &virt_rx_cb;
    if( 0 != api->virtual_initilize(api, 0) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_serial_mac_net_phy_rx()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if( !rf_driver_storage_stub.rx_cb ){
        return false;
    }
    rf_driver_storage_stub.list_value = NULL;
    if( -1 != rf_driver_storage_stub.rx_cb(NULL, 0, 0, 0, 0) ){
        return false;
    }
    uint8_t data[20];
    rf_driver_storage_stub.list_value = &list;
    list.phy_sap_identifier = NULL;
    if( -1 != rf_driver_storage_stub.rx_cb(&data, 20, 0, 0, 0) ){
        return false;
    }
    uint8_t nothing[1];
    list.phy_sap_identifier = &nothing;
    if( -1 != rf_driver_storage_stub.rx_cb(&data, 20, 0, 0, 0) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( -1 != rf_driver_storage_stub.rx_cb(&data, 20, 0, 0, 0) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    event_stub.int8_value = 2;
    if( 2 != rf_driver_storage_stub.rx_cb(&data, 20, 0, 0, 0) ){
        return false;
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_serial_mac_net_phy_tx_done()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    if( !rf_driver_storage_stub.done_cb ){
        return false;
    }

    rf_driver_storage_stub.list_value = NULL;
    if( -1 != rf_driver_storage_stub.done_cb(0,0,0,0,0) ){
        return false;
    }

    drv.tx = &tx_cb;
    tx_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    uint8_t buf[20];
    if( 0 != api->data_request_cb(api, &buf, 20) ){
        return false;
    }

    rf_driver_storage_stub.list_value = &list;
    event_stub.int8_value = 2;
    if( 2 != rf_driver_storage_stub.done_cb(0,0,0,0,0) ){
        return false;
    }

    if( event_stub.func_ptr ){ //cleanup of request
        arm_event_s ev;
        ev.event_type = 102; //SERIAL_DATA_CNF_EVENT
        event_stub.func_ptr(&ev);
    }

    ns_dyn_mem_free(api);
    return true;
}

bool test_serial_mac_virtual_data_request()
{
    arm_device_driver_list_s list;
    phy_device_driver_s drv;
    list.phy_driver = &drv;
    rf_driver_storage_stub.list_value = &list;
    nsdynmemlib_stub.returnCounter = 1;
    serial_mac_api_t *api = serial_mac_create(0);

    drv.arm_net_virtual_rx_cb = &virt_rx_cb;
    if( 0 != api->virtual_initilize(api, 0) ){
        return false;
    }

    if(!drv.arm_net_virtual_tx_cb){
        return false;
    }

    virtual_data_req_t data;
    memset(&data, 0, sizeof(virtual_data_req_t));
    rf_driver_storage_stub.list_value = NULL;
    drv.arm_net_virtual_tx_cb(&data, 0);

    //Test serial_mac_buffer_get here -->

    rf_driver_storage_stub.list_value = &list;
    if( -1 != drv.arm_net_virtual_tx_cb(&data, 0) ){
        return false;
    }

    uint8_t buf[8];
    data.msdu = &buf;
    data.msduLength = 8;
    data.parameter_length = 20;

    if( -1 != drv.arm_net_virtual_tx_cb(&data, 0) ){
        return false;
    }

    uint8_t buf2[20];
    data.parameters = &buf2;

    drv.tx = &tx_cb;
    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != drv.arm_net_virtual_tx_cb(&data, 0) ){
        return false;
    }

    if( event_stub.func_ptr ){ //cleanup of request
        arm_event_s ev;
        ev.event_type = 102; //SERIAL_DATA_CNF_EVENT
        event_stub.func_ptr(&ev);
    }

    //<-- Test serial_mac_buffer_get here

    ns_dyn_mem_free(api);
    return true;
}
