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
#include "test_mac_helper.h"
#include "mac_helper.h"
#include <string.h>
#include "mlme.h"
#include "mac_api.h"
#include "mac_common_defines.h"
#include "net_nwk_scan.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol.h"

#include "nsdynmemLIB_stub.h"
#include "protocol_core_stub.h"

static void mlme_req(const mac_api_t* api, mlme_primitive id, const void *data)
{

}

static int8_t mac_set( const mac_api_t* api, const uint8_t *mac64)
{

}

static int8_t mac_get( const mac_api_t* api, mac_extended_address_type type, uint8_t *mac64_buf)
{

}

bool test_mac_create_scan_request()
{
    mac_create_scan_request(0, NULL, 0, NULL);
    mlme_scan_t scan;
    channel_list_s list;

    mac_create_scan_request(0, &list, 0, &scan);

    return true;
}

bool test_mac_helper_select_best_lqi()
{
    if( mac_helper_select_best_lqi(NULL) ){
        return false;
    }
    nwk_pan_descriptor_t pan;
    mlme_pan_descriptor_t desc;
    pan.pan_descriptor = &desc;
    nwk_pan_descriptor_t pan2;
    mlme_pan_descriptor_t desc2;
    pan2.pan_descriptor = &desc2;
    desc.LinkQuality = 10;
    desc2.LinkQuality = 100;
    pan.next = &pan2;
    pan2.next = NULL;

    if( !mac_helper_select_best_lqi(&pan) ){
        return false;
    }
    return true;
}

bool test_mac_helper_drop_selected_from_the_scanresult()
{
    mac_helper_drop_selected_from_the_scanresult(NULL, NULL);
    nwk_scan_params_t scan;
    nwk_pan_descriptor_t pan;
    nwk_pan_descriptor_t pan2;

    scan.nwk_response_info = NULL;
    mac_helper_drop_selected_from_the_scanresult(&scan, &pan);

    pan.next = NULL;
    scan.nwk_response_info = &pan;
    mac_helper_drop_selected_from_the_scanresult(&scan, &pan);

    pan.next = &pan2;
    pan2.next = NULL;
    scan.nwk_response_info = &pan;
    scan.nwk_scan_res_size = 2;
    mac_helper_drop_selected_from_the_scanresult(&scan, &pan2);
    if( 1 != scan.nwk_scan_res_size ){
        return false;
    }

    return true;
}

bool test_mac_helper_free_scan_confirm()
{
    mac_helper_free_scan_confirm(NULL);
    nwk_scan_params_t scan;
    nsdynmemlib_stub.returnCounter = 1;
    nwk_pan_descriptor_t *pan = ns_dyn_mem_alloc(sizeof(nwk_pan_descriptor_t));
    memset(pan, 0, sizeof(nwk_pan_descriptor_t));
    pan->next = NULL;
    scan.nwk_response_info = pan;
    scan.nwk_scan_res_size = 1;
    scan.nwk_cur_active = NULL;
    mac_helper_free_scan_confirm(&scan);

    return true;
}

bool test_mac_helper_free_pan_descriptions()
{
    if( mac_helper_free_pan_descriptions(NULL) ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    nwk_pan_descriptor_t *pan = ns_dyn_mem_alloc(sizeof(nwk_pan_descriptor_t));
    memset(pan, 0, sizeof(nwk_pan_descriptor_t));
    pan->next = NULL;
    if( mac_helper_free_pan_descriptions(pan) ){
        return false;
    }
    return true;
}

bool test_mac_helper_nwk_id_filter_set()
{
    nsdynmemlib_stub.returnCounter = 0;
    if( -1 != mac_helper_nwk_id_filter_set(NULL, NULL) ){
        return false;
    }
    nwk_filter_params_s filt;
    filt.beacon_nwk_id_filter = NULL;
    if( 0 != mac_helper_nwk_id_filter_set(NULL, &filt) ){
        return false;
    }

    uint8_t data[16];
    if( -1 != mac_helper_nwk_id_filter_set(&data, &filt) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( 0 != mac_helper_nwk_id_filter_set(&data, &filt) ){
        return false;
    }
    ns_dyn_mem_free(filt.beacon_nwk_id_filter);
    return true;
}

bool test_mac_helper_panid_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    mac_helper_panid_set(&entry, 0);
    return true;
}

bool test_mac_helper_mac16_address_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    mac_helper_mac16_address_set(&entry, 0);

    mac_helper_mac16_address_set(&entry, 0xffff);
    return true;
}

bool test_mac_helper_mac16_address_get()
{
    uint16_t check = 0xfffe;
    if( check != mac_helper_mac16_address_get(NULL)){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.mac_short_address = 3;

    if( 3 != mac_helper_mac16_address_get(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_helper_panid_get()
{
    uint16_t check = 0xffff;
    if( check != mac_helper_panid_get(NULL)){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.pan_id = 3;

    if( 3 != mac_helper_panid_get(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_helper_set_default_key_source()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    mac_helper_set_default_key_source(&entry);

    return true;
}

bool test_mac_helper_default_security_level_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    mac_helper_default_security_level_set(&entry, 0);

    mac_helper_default_security_level_set(&entry, 1);

    return true;
}

bool test_mac_helper_default_security_level_get()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    params.mac_security_level = 5;
    entry.mac_parameters = &params;
    if( 5 != mac_helper_default_security_level_get(&entry) ){
        return false;
    }

    return true;
}

bool test_mac_helper_default_security_key_id_mode_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    mac_helper_default_security_key_id_mode_set(&entry, 3);

    return true;
}

bool test_mac_helper_default_security_key_id_mode_get()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    params.mac_key_id_mode = 2;

    if( 2 != mac_helper_default_security_key_id_mode_get(&entry) ){
        return false;
    }

    return true;
}

bool test_mac_helper_default_key_index_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    mac_helper_default_key_index_set(&entry, 3);
    return true;
}

bool test_mac_helper_default_key_index_get()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.mac_default_key_index = 4;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    if( 4 != mac_helper_default_key_index_get(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_helper_security_default_key_set()
{
    uint8_t key[16];

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;

    entry.mac_api = &api;

    if( -1 != mac_helper_security_default_key_set(&entry, &key, 0, 1)){
            return false;
        }

    if( 0 != mac_helper_security_default_key_set(&entry, &key, 1, 1)){
        return false;
    }

    return true;
}

bool test_mac_helper_security_pairwisekey_set()
{
    uint8_t key;
    uint8_t mac;
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;

    //test mac_helper_keytable_pairwise_descriptor_set here -->
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    if( -1 != mac_helper_security_pairwisekey_set(&entry, &key, NULL, 0) ){
            return false;
        }

    if( 0 != mac_helper_security_pairwisekey_set(&entry, &key, &mac, 0) ){
        return false;
    }

    if( 0 != mac_helper_security_pairwisekey_set(&entry, NULL, &mac, 0) ){
        return false;
    }

    //<-- test mac_helper_keytable_pairwise_descriptor_set here
}

bool test_mac_helper_security_next_key_set()
{

    uint8_t key[16];
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    if( -1 != mac_helper_security_next_key_set(&entry, &key, 0, 1)){
        return false;
    }


    if( 0 != mac_helper_security_next_key_set(&entry, &key, 1, 1)){
        return false;
    }
    return true;
}

bool test_mac_helper_security_prev_key_set()
{

    uint8_t key[16];

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    if( -1 != mac_helper_security_prev_key_set(&entry, &key, 0, 1)){
        return false;
    }

    if( 0 != mac_helper_security_prev_key_set(&entry, &key, 1, 1)){
        return false;
    }
    return true;
}

bool test_mac_helper_security_key_swap_next_to_default()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    mac_helper_security_key_swap_next_to_default(&entry);

    return true;
}

bool test_mac_helper_security_key_clean()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    mac_helper_security_key_clean(&entry);
    return true;
}

bool test_mac_helper_coordinator_address_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    uint8_t buf[8];
    mac_helper_coordinator_address_set(&entry, ADDR_802_15_4_SHORT, &buf);

    mac_helper_coordinator_address_set(&entry, ADDR_802_15_4_LONG, &buf);
    return true;
}

bool test_mac_helper_coordinator_address_get()
{
    if( 0 != mac_helper_coordinator_address_get(NULL, NULL) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    uint8_t buf[8];
    params.mac_cordinator_info.cord_adr_mode = MAC_ADDR_MODE_16_BIT;
    if( ADDR_802_15_4_SHORT != mac_helper_coordinator_address_get(&entry, &buf) ){
        return false;
    }

    params.mac_cordinator_info.cord_adr_mode = MAC_ADDR_MODE_64_BIT;
    if( ADDR_802_15_4_LONG != mac_helper_coordinator_address_get(&entry, &buf) ){
        return false;
    }
    return true;
}

bool test_mac_helper_beacon_payload_reallocate()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    params.mac_beacon_payload_size = 0;
    params.mac_beacon_payload = NULL;
    if( mac_helper_beacon_payload_reallocate(&entry, 0) ){
        return false;
    }
    params.mac_beacon_payload_size = 5;
    nsdynmemlib_stub.returnCounter = 1;
    params.mac_beacon_payload = ns_dyn_mem_alloc(5);
    if( mac_helper_beacon_payload_reallocate(&entry, 0) ){
        return false;
    }

    params.mac_beacon_payload_size = 5;
    nsdynmemlib_stub.returnCounter = 1;
    params.mac_beacon_payload = ns_dyn_mem_alloc(5);
    if( mac_helper_beacon_payload_reallocate(&entry, 15) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if( !mac_helper_beacon_payload_reallocate(&entry, 15) ){
        return false;
    }
    ns_dyn_mem_free(params.mac_beacon_payload);

    return true;
}

bool test_mac_helper_beacon_payload_register()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    if( 0 != mac_helper_beacon_payload_register(&entry) ){
        return false;
    }

    return true;
}

bool test_mac_helper_beacon_payload_pointer_get()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    uint8_t buf[4];
    params.mac_beacon_payload = &buf;
    if( !mac_helper_beacon_payload_pointer_get(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_helper_beacon_payload_length_get()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.mac_beacon_payload_size = 5;
    if( 5 != mac_helper_beacon_payload_length_get(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_helper_pib_boolean_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    if( 0 != mac_helper_pib_boolean_set(&entry, macSecurityEnabled, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macRxOnWhenIdle, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macPromiscuousMode, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macGTSPermit, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macAssociationPermit, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macAssociatedPANCoord, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macTimestampSupported, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macBattLifeExt, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macAutoRequest, false) ){
        return false;
    }

    if( 0 != mac_helper_pib_boolean_set(&entry, macThreadForceLongAddressForBeacon, false) ){
        return false;
    }

    if( -1 != mac_helper_pib_boolean_set(&entry, 0xfd, false) ){
        return false;
    }

    return true;
}

bool test_mac_helper_mac_channel_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    if( 0 != mac_helper_mac_channel_set(&entry, 8) ){
        return false;
    }

    return true;
}

bool test_mac_helper_write_our_addr()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    sockaddr_t addr;
    addr.addr_type = ADDR_NONE;
    params.shortAdressValid = false;

    if( !mac_helper_write_our_addr(&entry, &addr) ){
        return false;
    }

    params.shortAdressValid = true;
    addr.addr_type = ADDR_NONE;
    if( !mac_helper_write_our_addr(&entry, &addr) ){
        return false;
    }

    return true;
}

bool test_mac_helper_mac64_set()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mac64_set = &mac_set;
    entry.mac_api = &api;

    uint8_t buf[8];
    if( 0 != mac_helper_mac64_set(&entry, &buf) ){
        return false;
    }

    return true;
}

bool test_mac_helper_max_payload_size()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    mac_api_t api;
    api.mac64_set = &mac_set;
    entry.mac_api = &api;
    api.phyMTU = MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE;
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.MacUnsusecured_2003_cab = true;

    if( MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE != mac_helper_max_payload_size(&entry, 1) ){
        return false;
    }

    params.MacUnsusecured_2003_cab = false;
    if( MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE - 13 != mac_helper_max_payload_size(&entry, 13) ){
        return false;
    }
    return true;
}

bool test_mac_helper_frame_overhead()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    params.mac_security_level = 1;
    params.shortAdressValid = true;
    params.mac_key_id_mode = MAC_KEY_ID_MODE_SRC8_IDX;
    uint8_t ret;

    buffer_t buf;
    memset(&buf, 0, sizeof(buffer_t));
    buf.src_sa.addr_type = ADDR_NONE;
    buf.dst_sa.addr_type = ADDR_802_15_4_LONG; //17
    buf.options.ll_security_bypass_tx = false;

    ret = mac_helper_frame_overhead(&entry, &buf); // +14 + 4
    if( 35 != ret ){
        return false;
    }

    buf.src_sa.addr_type = ADDR_802_15_4_SHORT;
    buf.dst_sa.addr_type = ADDR_BROADCAST; //11
    params.mac_key_id_mode = 0;
    params.mac_security_level = 2;
    ret = mac_helper_frame_overhead(&entry, &buf); // +5 +8
    if( 24 != ret ){
        return false;
    }

    params.mac_security_level = 3;
    ret = mac_helper_frame_overhead(&entry, &buf); // +5 +16
    if( 32 != ret ){
        return false;
    }

    params.mac_security_level = 4;
    ret = mac_helper_frame_overhead(&entry, &buf); // +5 +0
    if( 16 != ret ){
        return false;
    }

    return true;
}

bool test_mac_helper_link_frame_counter_read()
{
    if( -1 != mac_helper_link_frame_counter_read(0, NULL) ){
        return false;
    }

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    protocol_core_stub.entry_ptr = &entry;
    uint32_t buf;
    if( 0 != mac_helper_link_frame_counter_read(0, &buf) ){
        return false;
    }
    return true;
}

bool test_mac_helper_link_frame_counter_set()
{
    protocol_core_stub.entry_ptr = NULL;
    int8_t ret = mac_helper_link_frame_counter_set(0, 0);
    if( ret != -1 ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    protocol_core_stub.entry_ptr = &entry;

    ret = mac_helper_link_frame_counter_set(0, 0);
    if( ret != 0 ){
        return false;
    }
    return true;
}

bool test_mac_helper_mac_mlme_max_retry_set()
{
    protocol_core_stub.entry_ptr = NULL;
    int8_t ret = mac_helper_mac_mlme_max_retry_set(0, 4);
    if( ret != -1 ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;
    protocol_core_stub.entry_ptr = &entry;

    ret = mac_helper_mac_mlme_max_retry_set(0, 4);
    if( ret != 0 ){
        return false;
    }
    return true;
}

bool test_mac_helper_devicetable_remove()
{
    mac_api_t api;
    api.mlme_req = &mlme_req;

    mac_helper_devicetable_remove(NULL, 0);

    mac_helper_devicetable_remove(&api, 0);
    return true;
}

bool test_mac_helper_devicetable_set()
{
    protocol_interface_info_entry_t entry;
    mle_neigh_table_entry_t entry_temp;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    api.mlme_req = &mlme_req;
    entry.mac_api = &api;

    entry.mac_parameters->SecurityEnabled = false;

    mac_helper_devicetable_set(&entry_temp, &entry, 10, 1);

    entry.mac_parameters->SecurityEnabled = true;
    entry.mac_parameters->mac_default_key_index = 2;

    mac_helper_devicetable_set(&entry_temp, &entry, 10, 1);


    return true;
}


