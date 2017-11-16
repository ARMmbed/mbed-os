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
#include "test_mac_indirect_data.h"
#include <string.h>
#include "inttypes.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/rf_driver_storage.h"

#include "sw_mac_stub.h"
#include "common_functions_stub.h"
#include "mac_header_helper_functions_stub.h"

bool visited = false;

static int8_t mac_state_control(phy_interface_state_e a, uint8_t b)
{
    visited = true;
}

static int8_t mac_tx(uint8_t *a, uint16_t b, uint8_t c)
{
    visited = true;
}

static int8_t mac_address_write(phy_address_type_e a,uint8_t *b)
{
    visited = true;
}

static int8_t mac_extension(phy_extension_type_e a,uint8_t *b)
{
    visited = true;
}

static uint8_t mymac[8];

static phy_device_driver_s device_driver = {
.PHY_MAC = mymac,
.link_type = PHY_LINK_ETHERNET_TYPE,
.driver_description = "indirect_test_iface",
.phy_MTU = 1280,
.phy_tail_length = 0,
.phy_header_length = 0,
.state_control = mac_state_control,
.tx = mac_tx,
.address_write = mac_address_write,
.extension = mac_extension,
};

static void test_data_confirm_cb( const mac_api_t* api, const mcps_data_conf_t *data )
{

}

void test_mlme_request(const mac_api_t* api, mlme_primitive id, const void *data)
{

}

bool test_mac_indirect_data_ttl_handle()
{
    mac_indirect_data_ttl_handle(NULL, 0); //Crashes if NULL not handled

    arm_device_driver_list_s list;
    protocol_interface_rf_mac_setup_s cur;
    cur.dev_driver = &list;
    cur.dev_driver->phy_driver = &device_driver;
    cur.indirect_pd_data_request_queue = NULL;
    visited = false;
    mac_indirect_data_ttl_handle(&cur, 0);
    if( !visited ){
        return false;
    }

    mac_pre_build_frame_t frame;
    mac_pre_build_frame_t frame2;
    mac_pre_build_frame_t frame3;
    frame.next = &frame2;
    frame2.next = &frame3;
    frame3.next = NULL;
    sw_mac_stub.mac_api_ptr = NULL;
    cur.indirect_pd_data_request_queue = &frame;
    mac_indirect_data_ttl_handle(&cur, 0);

    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    frame.buffer_ttl = 2;
    api.data_conf_cb = &test_data_confirm_cb;
    cur.indirect_pd_data_request_queue = &frame;
    mac_indirect_data_ttl_handle(&cur, 0);

    frame.buffer_ttl = 1;
    frame2.buffer_ttl = 80;
    frame3.buffer_ttl = 2;
    cur.indirect_pd_data_request_queue = &frame;
    mac_indirect_data_ttl_handle(&cur, 80);

    return true;
}

bool test_mac_indirect_data_req_handle()
{
    uint8_t ret = mac_indirect_data_req_handle( NULL, NULL );
    if( 1 != ret ){
        return false;
    }

    mac_pre_parsed_frame_t frame;
    memset(&frame, 0, sizeof(mac_pre_parsed_frame_t));
    protocol_interface_rf_mac_setup_s mac;
    frame.fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 1 != ret ){
        return false;
    }

    mac_api_t api;
    sw_mac_stub.mac_api_ptr = &api;
    api.mlme_ind_cb = &test_mlme_request;
    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    frame.ack_pendinfg_status = false;
    mac_pre_build_frame_t pre_frame;
    memset(&pre_frame, 0, sizeof(mac_pre_build_frame_t));

    mac.indirect_pd_data_request_queue = &pre_frame;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 1 != ret ){
        return false;
    }
    frame.ack_pendinfg_status = true;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 0 != ret ){
        return false;
    }

    mac_pre_build_frame_t pre_frame2;
    memset(&pre_frame2, 0, sizeof(mac_pre_build_frame_t));
    pre_frame.next = &pre_frame2;
    pre_frame2.fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    frame.ack_pendinfg_status = true;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 1 != ret ){
        return false;
    }

    mlme_device_descriptor_t desc;
    frame.neigh_info = &desc;

    desc.ShortAddress = 8;
    common_functions_stub.uint16_value = 8;
    pre_frame.fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 1 != ret ){
        return false;
    }

    memset(&desc.ExtAddress, 4, 8);
    memset(&pre_frame.DstAddr, 4, 8);
    pre_frame.fcf_dsn.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    mac.indirect_pd_data_request_queue = &pre_frame;
    frame.fcf_dsn.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    ret = mac_indirect_data_req_handle( &frame, &mac );
    if( 1 != ret ){
        return false;
    }

    return true;
}

bool test_mac_indirect_queue_write()
{
    mac_indirect_queue_write(NULL, NULL);

    mac_pre_build_frame_t pre_frame;
    memset(&pre_frame, 0, sizeof(mac_pre_build_frame_t));
    protocol_interface_rf_mac_setup_s cur;
    memset(&cur, 0, sizeof(protocol_interface_rf_mac_setup_s));
    arm_device_driver_list_s list;
    cur.dev_driver = &list;
    cur.dev_driver->phy_driver = &device_driver;

    visited = false;
    mac_indirect_queue_write(&cur, &pre_frame);
    if( !visited ){
        return false;
    }

    mac_pre_build_frame_t pre_frame2;
    memset(&pre_frame2, 0, sizeof(mac_pre_build_frame_t));
    mac_indirect_queue_write(&cur, &pre_frame2);

    mac_pre_build_frame_t pre_frame3;
    memset(&pre_frame3, 0, sizeof(mac_pre_build_frame_t));
    mac_indirect_queue_write(&cur, &pre_frame3);

    return true;
}
