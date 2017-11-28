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
#include "test_beacon_handler.h"
#include "beacon_handler.h"
#include <string.h>

#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"

#include "protocol_core_stub.h"
#include "nsdynmemLIB_stub.h"
#include "mac_helper_stub.h"
#include "protocol_6lowpan_stub.h"
#include "load_balance_api.h"

uint8_t *beacon_optional_tlv_field_get(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t type);

static uint8_t *beacon_ind_cb(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    return 0;
}

static uint8_t compare_cb(int8_t interface_id, uint8_t join_priority, uint8_t link_quality)
{
    return 1;
}

void lb_beac_notify(const load_balance_api_t* api, const struct mlme_beacon_ind_s *beacon_ind, uint8_t priority)
{
    (void)api;
    (void)beacon_ind;
    (void)priority;
}

uint8_t beac_ind(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    return 1;
}

bool test_beacon_received()
{
    //These test cases just confirms that there are no crashes or mem leaks.
    beacon_received(0, NULL);

    protocol_core_stub.entry_ptr = NULL;
    mlme_beacon_ind_t ind;
    beacon_received(0, &ind);

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    struct load_balance_api api;
    memset(&api, 0, sizeof(struct load_balance_api));
    api.lb_beacon_notify = &lb_beac_notify;
    entry.lb_api = &api;
    arm_15_4_mac_parameters_t params;
    memset(&params, 0, sizeof(arm_15_4_mac_parameters_t));
    params.beacon_ind = &beac_ind;
    entry.mac_parameters = &params;
    protocol_core_stub.entry_ptr = &entry;

    uint8_t beacon_data_in_3[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0x05};
    ind.beacon_data = malloc(20);
    memcpy(ind.beacon_data, beacon_data_in_3, sizeof(beacon_data_in_3));
    ind.beacon_data_length = 20;

    ind.PANDescriptor.CoordPANId = 6;

    beacon_received(0, &ind);

    free(ind.beacon_data);
    ind.beacon_data = NULL;

    entry.lb_api = NULL;

    params.nwk_scan_params.active_scan_active = true;
    params.nwk_filter_params.net_pan_id_filter = 1;
    ind.beacon_data_length = 40;
    ind.PANDescriptor.CoordPANId = 2;
    beacon_received(0, &ind);

    ind.PANDescriptor.CoordPANId = 1;
    params.nwk_filter_params.nwk_active_scan_level = 1;
    entry.mac_parameters->beacon_ind = &beacon_ind_cb;
    ind.beacon_data_length = 1;

    beacon_received(0, &ind);

    ind.beacon_data_length = 0;

    beacon_received(0, &ind);

    params.nwk_filter_params.nwk_active_scan_level = 0;

    beacon_received(0, &ind); //ns_dyn_mem_alloc fails

    //Test beacon_join_priority_tlv_val_get here -->
    params.beacon_compare_rx_cb_ptr = &compare_cb;
    ind.beacon_data_length = PLAIN_BEACON_PAYLOAD_SIZE+6;
    uint8_t dat[PLAIN_BEACON_PAYLOAD_SIZE+6];
    dat[PLAIN_BEACON_PAYLOAD_SIZE] = 0x00;

    ind.beacon_data = &dat;
    nsdynmemlib_stub.returnCounter = 1;

    //Test scan_params->nwk_response_info ==NULL here -->
    //Test beacon_optional_tlv_field_get here -->
    beacon_received(0, &ind); //just end delimiter

    dat[PLAIN_BEACON_PAYLOAD_SIZE] = 0x01; //len
    dat[PLAIN_BEACON_PAYLOAD_SIZE+1] = 0x08; //val
    dat[PLAIN_BEACON_PAYLOAD_SIZE+2] = (BEACON_OPTION_JOIN_PRIORITY_TYPE << 4 & 0xf0) +2;

    nsdynmemlib_stub.returnCounter = 2;
    beacon_received(0, &ind); //BEACON_OPTION_JOIN_PRIORITY_TYPE

    dat[PLAIN_BEACON_PAYLOAD_SIZE+2] = (BEACON_OPTION_JOIN_PRIORITY_TYPE << 4 & 0xf0) + 12;

    //Test duplicate_pan_descriptor here -->
    nsdynmemlib_stub.returnCounter = 3;
    beacon_received(0, &ind); //BEACON_OPTION_JOIN_PRIORITY_TYPE

    nsdynmemlib_stub.returnCounter = 4;
    beacon_received(0, &ind); //BEACON_OPTION_JOIN_PRIORITY_TYPE

    ns_dyn_mem_free(params.nwk_scan_params.nwk_response_info->pan_descriptor);
    ns_dyn_mem_free(params.nwk_scan_params.nwk_response_info->beacon_payload);
    ns_dyn_mem_free(params.nwk_scan_params.nwk_response_info);
    //<-- Test duplicate_pan_descriptor here
    //<-- Test beacon_optional_tlv_field_get here
    //<-- Test beacon_join_priority_tlv_val_get here
    //<--Test scan_params->nwk_response_info ==NULL here

    //Test scan_params->nwk_response_info != NULL here -->
    dat[PLAIN_BEACON_PAYLOAD_SIZE] = 0x00;

    nwk_pan_descriptor_t info;
    memset(&info, 0, sizeof(nwk_pan_descriptor_t));
    mlme_pan_descriptor_t pan;
    info.pan_descriptor = &pan;
    pan.LogicalChannel = 1;
    ind.PANDescriptor.LogicalChannel = 2;

    params.nwk_scan_params.nwk_response_info = &info;
    nsdynmemlib_stub.returnCounter = 1;
    beacon_received(0, &ind);

    nsdynmemlib_stub.returnCounter = 3;
    beacon_received(0, &ind);

    nsdynmemlib_stub.returnCounter = 4;
    beacon_received(0, &ind);
    ns_dyn_mem_free(info.next->pan_descriptor);
    ns_dyn_mem_free(info.next->beacon_payload);
    ns_dyn_mem_free(info.next);

    mlme_pan_descriptor_t pan2;
    nwk_pan_descriptor_t info2;
    info2.pan_descriptor = &pan2;
    pan2.LogicalChannel = 2;
    pan2.CoordPANId = 2;
    ind.PANDescriptor.CoordPANId = 2;
    info2.pan_descriptor->LinkQuality = 110;
    info2.alternative_parent.CoordAddrMode = MAC_ADDR_MODE_NONE;
    info.next = &info2;

    mlme_pan_descriptor_t pan3;
    nwk_pan_descriptor_t info3;
    info3.pan_descriptor = &pan3;
    pan3.LogicalChannel = 3;
    info2.next = &info3;
    info3.next = NULL;

    info2.beacon_length = 0;
    info2.beacon_payload = NULL;

    ind.beacon_data_length = 0;
    params.nwk_filter_params.net_pan_id_filter = 2;

    nsdynmemlib_stub.returnCounter = 1;
    ind.PANDescriptor.LinkQuality = 0;
    beacon_received(0, &ind);

    info2.pan_descriptor->LinkQuality = 0;
    ind.PANDescriptor.LinkQuality = 10;
    nsdynmemlib_stub.returnCounter = 1;
    info2.beacon_payload = ns_dyn_mem_alloc(2);
    info2.beacon_length = 2;
    nsdynmemlib_stub.returnCounter = 1;
    beacon_received(0, &ind);

    info2.pan_descriptor->LinkQuality = 0;
    ind.beacon_data_length = PLAIN_BEACON_PAYLOAD_SIZE+6;
    nsdynmemlib_stub.returnCounter = 1;
    beacon_received(0, &ind);

    info2.pan_descriptor->LinkQuality = 0;
    nsdynmemlib_stub.returnCounter = 2;
    beacon_received(0, &ind);
    ns_dyn_mem_free( info2.beacon_payload );

    //<--Test scan_params->nwk_response_info != NULL here

    return true;
}

bool test_mac_beacon_link_beacon_compare_rx_callback_set()
{
    if( -1 != mac_beacon_link_beacon_compare_rx_callback_set(0, NULL) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    memset(&params, 0, sizeof(arm_15_4_mac_parameters_t));
    entry.mac_parameters = &params;
    protocol_core_stub.entry_ptr = &entry;

    //It is okay to set NULL for callback
    if( 0 != mac_beacon_link_beacon_compare_rx_callback_set(0, NULL) ){
        return false;
    }

    protocol_core_stub.entry_ptr = NULL;

}

bool test_beacon_join_priority_update()
{
    //These test cases just confirms that there are no crashes or mem leaks.
    beacon_join_priority_update(0x00);

    mlme_beacon_ind_t ind;
    beacon_join_priority_update(0x00);

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    memset(&params, 0, sizeof(arm_15_4_mac_parameters_t));
    entry.mac_parameters = &params;
    protocol_core_stub.entry_ptr = &entry;

    beacon_join_priority_update(0x00);

    // Uses real storing of beacon payload data
    mac_helper_stub.enable_storing = true;

    // Sets join priority
    protocol_6lowpan_stub.uint8_value = 0xe1;
    entry.mac_parameters->beacon_join_priority_tx_cb_ptr = protocol_6lowpan_beacon_join_priority_tx;


    // Test: TLV does not exists.

    // Sets input data
    uint8_t beacon_data_in_1[18] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    entry.mac_parameters->mac_beacon_payload = malloc(18);
    memcpy(entry.mac_parameters->mac_beacon_payload, beacon_data_in_1, sizeof(beacon_data_in_1));
    entry.mac_parameters->mac_beacon_payload_size = 18;

    beacon_join_priority_update(0x00);

    // Check output data
    if (entry.mac_parameters->mac_beacon_payload_size != 20) {
        return false;
    }
    uint8_t beacon_data_out_1[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0xe1};
    if (memcmp(entry.mac_parameters->mac_beacon_payload, beacon_data_out_1, 20) != 0) {
        return false;
    }
    free(entry.mac_parameters->mac_beacon_payload);


    // Test: TLV does not exists. Delimiter 0x00 in the end of the beacon.

    uint8_t beacon_data_in_2[19] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0x00};
    entry.mac_parameters->mac_beacon_payload = malloc(19);
    memcpy(entry.mac_parameters->mac_beacon_payload, beacon_data_in_2, sizeof(beacon_data_in_2));
    entry.mac_parameters->mac_beacon_payload_size = 19;

    beacon_join_priority_update(0x00);

    // Check output data
    if (entry.mac_parameters->mac_beacon_payload_size != 21) {
        return false;
    }
    uint8_t beacon_data_out_2[21] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0xe1,0x00};
    if (memcmp(entry.mac_parameters->mac_beacon_payload, beacon_data_out_2, 21) != 0) {
        return false;
    }
    free(entry.mac_parameters->mac_beacon_payload);


    // Test: join priority TLV in the end of the beacon.

    uint8_t beacon_data_in_3[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0x05};
    entry.mac_parameters->mac_beacon_payload = malloc(20);
    memcpy(entry.mac_parameters->mac_beacon_payload, beacon_data_in_3, sizeof(beacon_data_in_3));
    entry.mac_parameters->mac_beacon_payload_size = 20;

    beacon_join_priority_update(0x00);

    // Check output data
    if (entry.mac_parameters->mac_beacon_payload_size != 20) {
        return false;
    }
    uint8_t beacon_data_out_3[21] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0xe1};
    if (memcmp(entry.mac_parameters->mac_beacon_payload, beacon_data_out_3, 20) != 0) {
        return false;
    }
    free(entry.mac_parameters->mac_beacon_payload);


    // Test: two TLVs and after that delimiter and data in the end of the beacon. Second TLV is join priority.

    uint8_t beacon_data_in_4[27] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xa2,0x03,0x04,0xc1,0x05,0x00,1,2,3};
    entry.mac_parameters->mac_beacon_payload = malloc(27);
    memcpy(entry.mac_parameters->mac_beacon_payload, beacon_data_in_4, sizeof(beacon_data_in_4));
    entry.mac_parameters->mac_beacon_payload_size = 27;

    beacon_join_priority_update(0x00);

    // Check output data
    if (entry.mac_parameters->mac_beacon_payload_size != 27) {
        return false;
    }
    uint8_t beacon_data_out_4[27] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xa2,0x03,0x04,0xc1,0xe1,0x00,1,2,3};
    if (memcmp(entry.mac_parameters->mac_beacon_payload, beacon_data_out_4, 27) != 0) {
        return false;
    }
    free(entry.mac_parameters->mac_beacon_payload);


    // Test: TLV in the end of the beacon. TLV length is too long.

    uint8_t beacon_data_in_5[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc2,0x05};
    entry.mac_parameters->mac_beacon_payload = malloc(20);
    memcpy(entry.mac_parameters->mac_beacon_payload, beacon_data_in_5, sizeof(beacon_data_in_5));
    entry.mac_parameters->mac_beacon_payload_size = 20;

    beacon_join_priority_update(0x00);

    // Check output data
    if (entry.mac_parameters->mac_beacon_payload_size != 22) {
        return false;
    }
    uint8_t beacon_data_out_5[22] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,0xc1,0xe1,0xc2,0x05};
    if (memcmp(entry.mac_parameters->mac_beacon_payload, beacon_data_out_5, 22) != 0) {
        return false;
    }
    free(entry.mac_parameters->mac_beacon_payload);



    // Reset stub values
    protocol_core_stub.entry_ptr = NULL;
    protocol_6lowpan_stub.uint8_value = 0;
    mac_helper_stub.enable_storing = false;
    mac_helper_stub.uint8_ptr = 0;
    mac_helper_stub.uint8_value = 0;


}


static uint8_t tester_beacon_join_priority_tx_cb(int8_t interface_id)
{
    return 0;
}

bool test_mac_beacon_link_beacon_join_priority_tx_callback_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    memset(&params, 0, sizeof(arm_15_4_mac_parameters_t));


    if (mac_beacon_link_beacon_join_priority_tx_callback_set(0,&tester_beacon_join_priority_tx_cb) != -1) {
        return false;
    }
    protocol_core_stub.entry_ptr = &entry;

    if (mac_beacon_link_beacon_join_priority_tx_callback_set(0,&tester_beacon_join_priority_tx_cb) != -1) {
        return false;
    }

    entry.mac_parameters = &params;
    if (mac_beacon_link_beacon_join_priority_tx_callback_set(0,&tester_beacon_join_priority_tx_cb) != 0) {
        return false;
    }

    if (!entry.mac_parameters->beacon_join_priority_tx_cb_ptr) {
        return false;
    }

    // Reset stub values
        protocol_core_stub.entry_ptr = NULL;
        protocol_6lowpan_stub.uint8_value = 0;
        mac_helper_stub.enable_storing = false;
        mac_helper_stub.uint8_ptr = 0;
        mac_helper_stub.uint8_value = 0;

    return true;
}



