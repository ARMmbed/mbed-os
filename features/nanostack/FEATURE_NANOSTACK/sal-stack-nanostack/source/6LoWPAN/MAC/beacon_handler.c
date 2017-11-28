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
#include <string.h>
#include "beacon_handler.h"
#include "nsdynmemLIB.h"
#include "mlme.h"
#include "mac_helper.h"
#include "Service_Libs/load_balance/load_balance_api.h"
#include "Service_Libs/pan_blacklist/pan_blacklist_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ns_trace.h"
#include "common_functions.h"


#define TRACE_GROUP "BHlr"

static mlme_pan_descriptor_t *duplicate_pan_descriptor(const mlme_pan_descriptor_t *desc);
static nwk_pan_descriptor_t * get_local_description(uint16_t payload_length, uint8_t *payload_ptr);

static uint8_t *beacon_optional_tlv_field_get(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t type);
static bool beacon_join_priority_tlv_val_get(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t *join_priority);
static bool beacon_join_priority_tlv_val_set(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t join_priority);
static bool beacon_join_priority_tlv_add(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t join_priority);

void beacon_received(int8_t if_id, const mlme_beacon_ind_t* data)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(if_id);
    if (!interface || !data) {
        return;
    }

    if (pan_blacklist_filter(&interface->pan_blaclist_cache, data->PANDescriptor.CoordPANId)) {
        tr_debug("Drop black listed beacon");
        return;
    }

    uint8_t coord_pan_address[10];
    common_write_16_bit(data->PANDescriptor.CoordPANId, coord_pan_address);
    memcpy(coord_pan_address + 2,data->PANDescriptor.CoordAddress, 8 );
    if (data->PANDescriptor.CoordAddrMode == MAC_ADDR_MODE_16_BIT) {
        memset(coord_pan_address +4, 0, 6);
    }

    if (pan_cordinator_blacklist_filter(&interface->pan_cordinator_black_list, coord_pan_address)) {
        tr_debug("Drop black listed beacon by coordinator");
        return;
    }

    nwk_scan_params_t *scan_params = &interface->mac_parameters->nwk_scan_params;

    if (!scan_params || !scan_params->active_scan_active) {

        if (interface->lb_api && data->PANDescriptor.CoordPANId != interface->mac_parameters->pan_id) {

            uint8_t priority;
            if (interface->mac_parameters->beacon_ind && beacon_join_priority_tlv_val_get(data->beacon_data_length, data->beacon_data, PLAIN_BEACON_PAYLOAD_SIZE, &priority)) {
                if (interface->mac_parameters->beacon_ind(data->beacon_data, data->beacon_data_length, interface)) {
                    interface->lb_api->lb_beacon_notify(interface->lb_api, data, priority);
                }
            }
        }
        return;
    }

    nwk_filter_params_s *filter = &(interface->mac_parameters->nwk_filter_params);

    if (filter->net_pan_id_filter != 0xffff) {
        if (filter->net_pan_id_filter != data->PANDescriptor.CoordPANId) {
            tr_debug("PAN-id Filter drop");
            return;
        }
    }

    tr_debug("Beacon indication: %s", trace_array(data->beacon_data, data->beacon_data_length));
    if (filter->nwk_active_scan_level) {
        bool dropBeacon = false;

        if (interface->mac_parameters->beacon_ind && data->beacon_data_length) {
            if (interface->mac_parameters->beacon_ind(data->beacon_data, data->beacon_data_length, interface) == 0) {
                tr_debug("Beacon payload filter drop");
                dropBeacon = true;
            }
        } else {
            tr_debug("Beacon dropped: No payload");
            dropBeacon = true;
        }
        if (dropBeacon) {

            return;
        }
    }

    //Here possible dynamic function API Call
    uint8_t *b_data = ns_dyn_mem_temporary_alloc(data->beacon_data_length);
    if( !b_data ){
        return;
    }
    uint16_t b_len = data->beacon_data_length;
    memcpy(b_data, data->beacon_data, b_len);
    uint8_t lqi = data->PANDescriptor.LinkQuality;

    if (interface->mac_parameters->beacon_compare_rx_cb_ptr) {
        uint8_t join_priority;
        if (beacon_join_priority_tlv_val_get(b_len, b_data, PLAIN_BEACON_PAYLOAD_SIZE, &join_priority)) {
            lqi = interface->mac_parameters->beacon_compare_rx_cb_ptr(
                    interface->id, join_priority, data->PANDescriptor.LinkQuality);
        }
    }

    if (scan_params->nwk_response_info) {
        nwk_pan_descriptor_t *cur = scan_params->nwk_response_info;
        nwk_pan_descriptor_t *prev = 0;
        mlme_pan_descriptor_t *cur_desc;

        while (cur) {
            cur_desc = cur->pan_descriptor;
            if (cur_desc->LogicalChannel == data->PANDescriptor.LogicalChannel &&
                cur_desc->CoordPANId == data->PANDescriptor.CoordPANId) {

                //Compare address to primary we need to check that we are not storage same parent twice FHSS
                if (memcmp(cur_desc->CoordAddress,data->PANDescriptor.CoordAddress , 8) == 0) {
                    //Update allways better LQI
                    if (cur_desc->LinkQuality < lqi) {
                        cur_desc->LinkQuality = lqi;
                    }

                } else {

                    //Compare LQI
                    if (cur_desc->LinkQuality < lqi) {
                        if (cur->beacon_length != b_len) {

                            if (b_len) {
                                uint8_t *temp_payload = ns_dyn_mem_alloc(b_len);
                                if (!temp_payload) {
                                    ns_dyn_mem_free(b_data);
                                    return;
                                }
                                ns_dyn_mem_free(cur->beacon_payload);
                                cur->beacon_payload = temp_payload;
                                memcpy(cur->beacon_payload, b_data,b_len);
                            } else {
                                ns_dyn_mem_free(cur->beacon_payload);
                                cur->beacon_payload = NULL;
                            }
                            cur->beacon_length = b_len;
                        }

                        cur->alternative_parent.CoordAddrMode = cur_desc->CoordAddrMode;
                        memcpy(cur->alternative_parent.CoordAddress, cur_desc->CoordAddress, 8);
                        cur->alternative_parent.LinkQuality = cur_desc->LinkQuality;

                        cur_desc->CoordAddrMode = data->PANDescriptor.CoordAddrMode;
                        memcpy(cur_desc->CoordAddress, data->PANDescriptor.CoordAddress, 8);
                        cur_desc->LinkQuality = lqi;

                    } else if (cur->alternative_parent.CoordAddrMode == MAC_ADDR_MODE_NONE || cur->alternative_parent.LinkQuality < lqi) {
                        cur->alternative_parent.CoordAddrMode = data->PANDescriptor.CoordAddrMode;
                        memcpy(cur->alternative_parent.CoordAddress, data->PANDescriptor.CoordAddress, 8);
                        cur->alternative_parent.LinkQuality = lqi;
                    }

                }
                ns_dyn_mem_free(b_data);
                return;
            }

            if (cur) {
                prev = cur;
                cur = cur->next;
            }
        }

        nwk_pan_descriptor_t *new_entry = get_local_description(b_len, b_data);
        ns_dyn_mem_free(b_data);
        b_data = NULL;
        if (!new_entry) {
            return;
        }

        new_entry->pan_descriptor = duplicate_pan_descriptor(&data->PANDescriptor);
        if( !new_entry->pan_descriptor ){
            ns_dyn_mem_free(new_entry->beacon_payload);
            ns_dyn_mem_free(new_entry);
            return;
        }
        new_entry->pan_descriptor->LinkQuality = lqi;
        tr_debug("Beacon:Link new Last");
        prev->next = new_entry;
        scan_params->nwk_scan_res_size++;

    } else {
        nwk_pan_descriptor_t *new_entry = get_local_description(b_len, b_data);
        ns_dyn_mem_free(b_data);
        b_data = NULL;
        if (!new_entry) {
            return;
        }

        new_entry->pan_descriptor = duplicate_pan_descriptor(&data->PANDescriptor);
        if( !new_entry->pan_descriptor ){
            ns_dyn_mem_free(new_entry->beacon_payload);
            ns_dyn_mem_free(new_entry);
            return;
        }
        new_entry->pan_descriptor->LinkQuality = lqi;
        scan_params->nwk_response_info = new_entry;
        scan_params->nwk_scan_res_size++;
        return;
    }
}

static bool beacon_join_priority_tlv_val_get(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t *join_priority)
{
    const uint8_t *tlv_ptr = beacon_optional_tlv_field_get(len, ptr, offset, BEACON_OPTION_JOIN_PRIORITY_TYPE);

    // If TLV is found and its contains value field
    if (tlv_ptr && ((*tlv_ptr & 0x0F) >= BEACON_OPTION_JOIN_PRIORITY_VAL_LEN)) {
        *join_priority = *(++tlv_ptr);
        return true;
    }

    return false;
}

static bool beacon_join_priority_tlv_val_set(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t join_priority)
{
    uint8_t *tlv_ptr = beacon_optional_tlv_field_get(len, ptr, offset, BEACON_OPTION_JOIN_PRIORITY_TYPE);

    // If TLV is found and its contains value field
    if (tlv_ptr && ((*tlv_ptr & 0x0F) >= BEACON_OPTION_JOIN_PRIORITY_VAL_LEN)) {
        *(++tlv_ptr) = join_priority;
        return true;
    }

    return false;
}

static bool beacon_join_priority_tlv_add(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t join_priority)
{
    // Invalid length
    if (len < offset + BEACON_OPTION_JOIN_PRIORITY_LEN) {
       return false;
    }

    ptr += offset;
    offset += BEACON_OPTION_JOIN_PRIORITY_LEN;

    // If there is already data adds TLV right after the plain beacon data
    if (len > offset) {
        memmove(ptr + BEACON_OPTION_JOIN_PRIORITY_LEN, ptr, len - offset);
    }

    *ptr++ = BEACON_OPTION_JOIN_PRIORITY_TYPE_LEN;
    *ptr = join_priority;

    return true;
}

static nwk_pan_descriptor_t * get_local_description(uint16_t payload_length, uint8_t *payload_ptr) {
    nwk_pan_descriptor_t *description = ns_dyn_mem_temporary_alloc(sizeof(nwk_pan_descriptor_t));
    if (description) {
        memset(description, 0, sizeof(nwk_pan_descriptor_t));
        if (payload_length) {
            description->beacon_payload = ns_dyn_mem_temporary_alloc(payload_length);
            if (!description->beacon_payload) {
                ns_dyn_mem_free(description);
                return NULL;
            }
            memcpy(description->beacon_payload, payload_ptr, payload_length);
            description->beacon_length = payload_length;
        }
        description->alternative_parent.CoordAddrMode = MAC_ADDR_MODE_NONE;
    }
    return description;
}

static uint8_t *beacon_optional_tlv_field_get(uint8_t len, uint8_t *ptr, uint8_t offset, uint8_t type)
{
    uint8_t tlvs_len = 0;

    while (len > offset + tlvs_len) {
        uint8_t *tlv_ptr;
        tlv_ptr = ptr + offset + tlvs_len;
        if (*tlv_ptr == BEACON_OPTION_END_DELIMITER) {
            break;
        // If TLV is found
        } else if (*tlv_ptr >> 4 == type) {
            // Validates TLV length
            if (len >= offset + tlvs_len + 1 + (*tlv_ptr & 0x0F)) {
                return tlv_ptr;
            } else {
                break;
            }
        } else {
            // Length/type octet and value octets
            tlvs_len += 1 + (*tlv_ptr & 0x0F);
        }
    }

    return NULL;
}

static mlme_pan_descriptor_t *duplicate_pan_descriptor(const mlme_pan_descriptor_t *desc)
{
    mlme_pan_descriptor_t *ret = ns_dyn_mem_temporary_alloc(sizeof(mlme_pan_descriptor_t));
    if(!ret){
        return NULL;
    }
    memset(ret, 0, sizeof(mlme_pan_descriptor_t));

    ret->CoordAddrMode = desc->CoordAddrMode;
    ret->CoordPANId = desc->CoordPANId;
    memcpy(ret->CoordAddress, desc->CoordAddress, 8);
    ret->LogicalChannel = desc->LogicalChannel;
    ret->ChannelPage = desc->ChannelPage;
    memcpy(ret->SuperframeSpec, desc->SuperframeSpec, 2);
    ret->GTSPermit = desc->GTSPermit;
    ret->LinkQuality = desc->LinkQuality;
    ret->Timestamp = desc->Timestamp;
    ret->SecurityFailure = desc->SecurityFailure;

    ret->Key.SecurityLevel = desc->Key.SecurityLevel;
    ret->Key.KeyIdMode = desc->Key.KeyIdMode;
    ret->Key.KeyIndex = desc->Key.KeyIndex;
    memcpy(ret->Key.Keysource, desc->Key.Keysource, 8);
    return ret;
}

int8_t mac_beacon_link_beacon_compare_rx_callback_set(int8_t interface_id, beacon_compare_rx_cb *beacon_compare_rx_cb_ptr)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_parameters) {
        return -1;
    }

    cur->mac_parameters->beacon_compare_rx_cb_ptr = beacon_compare_rx_cb_ptr;

    return 0;
}

static void mac_beacon_joining_priority_update(protocol_interface_info_entry_t *cur, uint8_t join_priority)
{
    uint8_t beacon_payload_len = mac_helper_beacon_payload_length_get(cur);
    uint8_t *beacon_payload_ptr = mac_helper_beacon_payload_pointer_get(cur);

    // Checks if join priority TLV exists
    if (beacon_payload_len >= PLAIN_BEACON_PAYLOAD_SIZE + BEACON_OPTION_JOIN_PRIORITY_LEN) {
        // If TLV list exists
        if (beacon_payload_ptr[PLAIN_BEACON_PAYLOAD_SIZE] != BEACON_OPTION_END_DELIMITER) {
            uint8_t current_join_priority;
            uint8_t tlv_exists = beacon_join_priority_tlv_val_get(beacon_payload_len, beacon_payload_ptr, PLAIN_BEACON_PAYLOAD_SIZE, &current_join_priority);
            if (tlv_exists) {
                if (current_join_priority != join_priority) {
                    beacon_join_priority_tlv_val_set(beacon_payload_len, beacon_payload_ptr, PLAIN_BEACON_PAYLOAD_SIZE, join_priority);
                    // Updates beacon to MAC
                    (void) mac_helper_beacon_payload_register(cur);
                }
                return;
            }
        }
    }

    // Adds join priority TLV as first field after plain beacon data
    if (beacon_payload_len >= PLAIN_BEACON_PAYLOAD_SIZE) {
        beacon_payload_len = beacon_payload_len + BEACON_OPTION_JOIN_PRIORITY_LEN;
        beacon_payload_ptr = mac_helper_beacon_payload_reallocate(cur, beacon_payload_len);
        beacon_join_priority_tlv_add(beacon_payload_len, beacon_payload_ptr,
                                     PLAIN_BEACON_PAYLOAD_SIZE, join_priority);
        // Updates beacon to MAC
        (void) mac_helper_beacon_payload_register(cur);
    }
}

int8_t mac_beacon_link_beacon_join_priority_tx_callback_set(int8_t interface_id, beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_parameters) {
        return -1;
    }

    cur->mac_parameters->beacon_join_priority_tx_cb_ptr = beacon_join_priority_tx_cb_ptr;

    return 0;
}

void beacon_join_priority_update(int8_t interface_id)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);

    if (!interface || !interface->mac_parameters ||
        !interface->mac_parameters->beacon_join_priority_tx_cb_ptr) {
        return;
    }

    uint8_t join_priority = interface->mac_parameters->beacon_join_priority_tx_cb_ptr(interface_id);
    mac_beacon_joining_priority_update(interface, join_priority);
}
