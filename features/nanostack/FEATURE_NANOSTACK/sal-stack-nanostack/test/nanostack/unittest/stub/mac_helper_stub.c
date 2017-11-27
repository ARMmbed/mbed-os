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
#include "NWK_INTERFACE/Include/protocol.h"
#include "mlme.h"
#include "mac_helper.h"
#include "mac_common_defines.h"
#include "nsdynmemLIB.h"
#include "net_nwk_scan.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "MLE/mle_tlv.h"
#include "MLE/mle.h"
#include "mac_api.h"
#include "mac_common_defines.h"
#include "mac_helper_stub.h"

#define TRACE_GROUP "MACh"

mac_helper_stub_def mac_helper_stub;

void mac_create_scan_request(mac_scan_type_t type, channel_list_s *chanlist, uint8_t scan_duration, mlme_scan_t *request)
{
}

nwk_pan_descriptor_t *mac_helper_select_best_lqi(nwk_pan_descriptor_t *list)
{
    return mac_helper_stub.desc_ptr;
}

void mac_helper_drop_selected_from_the_scanresult(nwk_scan_params_t *scanParams, nwk_pan_descriptor_t *selected)
{
}

void mac_helper_free_scan_confirm(nwk_scan_params_t *params)
{
}

nwk_pan_descriptor_t * mac_helper_free_pan_descriptions(nwk_pan_descriptor_t *nwk_cur_active)
{
    return mac_helper_stub.desc_ptr;
}

int8_t mac_helper_nwk_id_filter_set(const uint8_t *nw_id, nwk_filter_params_s *filter)
{
    return mac_helper_stub.int8_value;
}

void mac_helper_panid_set(protocol_interface_info_entry_t *interface, uint16_t panId)
{

}

void mac_helper_mac16_address_set(protocol_interface_info_entry_t *interface, uint16_t mac16)
{
}

uint16_t mac_helper_mac16_address_get(const protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.uint16_value;
}

uint16_t mac_helper_panid_get(const protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.uint16_value;
}

void mac_helper_default_key_index_set(protocol_interface_info_entry_t *interface, uint8_t keyIndex)
{
}

uint8_t mac_helper_default_key_index_get(protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.uint8_value;
}

void mac_helper_set_default_key_source(protocol_interface_info_entry_t *interface)
{

}

void mac_helper_default_security_level_set(protocol_interface_info_entry_t *interface, uint8_t securityLevel)
{

}

uint8_t mac_helper_default_security_level_get(protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.uint8_value;
}

void mac_helper_default_security_key_id_mode_set(protocol_interface_info_entry_t *interface, uint8_t keyIdMode)
{
}

uint8_t mac_helper_default_security_key_id_mode_get(protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.uint8_value;
}

int8_t mac_helper_security_default_key_set(protocol_interface_info_entry_t *interface, const uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    return mac_helper_stub.int8_value;
}

int8_t mac_helper_security_next_key_set(protocol_interface_info_entry_t *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    return mac_helper_stub.int8_value;
}

int8_t mac_helper_security_prev_key_set(protocol_interface_info_entry_t *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    return mac_helper_stub.int8_value;
}

void mac_helper_security_key_swap_next_to_default(protocol_interface_info_entry_t *interface)
{
}

void mac_helper_security_key_clean(protocol_interface_info_entry_t *interface)
{
}

void mac_helper_coordinator_address_set(protocol_interface_info_entry_t *interface, addrtype_t adr_type, uint8_t *adr_ptr)
{
}

addrtype_t mac_helper_coordinator_address_get(protocol_interface_info_entry_t *interface, uint8_t *adr_ptr)
{
    return mac_helper_stub.uint8_value;
}

uint8_t *mac_helper_beacon_payload_reallocate(protocol_interface_info_entry_t *interface, uint8_t len)
{
    if (mac_helper_stub.enable_storing) {
        if (interface->mac_parameters->mac_beacon_payload_size != len) {
            uint8_t *beacon_ptr = malloc(len);
            uint8_t beacon_len;
            if (len >= interface->mac_parameters->mac_beacon_payload_size) {
                beacon_len = interface->mac_parameters->mac_beacon_payload_size;
            } else {
                beacon_len = len;
            }
            memcpy(beacon_ptr, interface->mac_parameters->mac_beacon_payload, beacon_len);
            free(interface->mac_parameters->mac_beacon_payload);
            interface->mac_parameters->mac_beacon_payload = beacon_ptr;
        }
        interface->mac_parameters->mac_beacon_payload_size = len;
        return interface->mac_parameters->mac_beacon_payload;
    } else {
        return mac_helper_stub.uint8_ptr;
    }
}

int8_t mac_helper_beacon_payload_register(protocol_interface_info_entry_t *interface)
{
    return mac_helper_stub.int8_value;

}

uint8_t *mac_helper_beacon_payload_pointer_get(protocol_interface_info_entry_t *interface)
{
    if (mac_helper_stub.enable_storing) {
        return interface->mac_parameters->mac_beacon_payload;
    } else {
        return mac_helper_stub.uint8_ptr;
    }
}

uint8_t mac_helper_beacon_payload_length_get(protocol_interface_info_entry_t *interface)
{
    if (mac_helper_stub.enable_storing) {
        return interface->mac_parameters->mac_beacon_payload_size;
    } else {
        return mac_helper_stub.uint8_value;
    }
}

int8_t mac_helper_pib_boolean_set(protocol_interface_info_entry_t *interface, mlme_attr_t attribute, bool value)
{
    return mac_helper_stub.int8_value;
}

int8_t mac_helper_mac_channel_set(protocol_interface_info_entry_t *interface, uint8_t new_channel)
{
    return mac_helper_stub.int8_value;
}

bool mac_helper_write_our_addr(protocol_interface_info_entry_t *interface, sockaddr_t *ptr)
{
    return mac_helper_stub.bool_value;
}

int8_t mac_helper_mac64_set(protocol_interface_info_entry_t *interface, const uint8_t *mac64)
{
    return mac_helper_stub.int8_value;
}

uint_fast16_t mac_helper_max_payload_size(protocol_interface_info_entry_t *cur, uint_fast8_t frame_overhead)
{
    return mac_helper_stub.uint16_value;
}

uint_fast8_t mac_helper_frame_overhead(protocol_interface_info_entry_t *cur, const buffer_t *buf)
{
    return mac_helper_stub.uint8_value;
}

int8_t mac_helper_link_frame_counter_read(int8_t interface_id, uint32_t *seq_ptr)
{
    *seq_ptr = mac_helper_stub.uint32_value;
    return mac_helper_stub.int8_value;
}

int8_t mac_helper_link_frame_counter_set(int8_t interface_id, uint32_t seq_ptr)
{
    mac_helper_stub.uint32_value = seq_ptr;
    return mac_helper_stub.int8_value;
}

int8_t mac_helper_security_pairwisekey_set(protocol_interface_info_entry_t *interface, const uint8_t *key, const uint8_t *mac_64, uint8_t key_attribute)
{
    return mac_helper_stub.int8_value;
}

void mac_helper_devicetable_remove(struct mac_api_s *mac_api, uint8_t attribute_index)
{

}

void mac_helper_devicetable_set(mle_neigh_table_entry_t *entry_temp, protocol_interface_info_entry_t *cur, uint32_t frame_counter, uint8_t keyID)
{

}
int8_t mac_helper_mac_mlme_max_retry_set(int8_t interface_id, uint8_t mac_retry_set)
{

}
