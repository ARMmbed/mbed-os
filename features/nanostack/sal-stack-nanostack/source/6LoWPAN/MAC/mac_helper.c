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
#include "mac_api.h"

#define TRACE_GROUP "MACh"

static const uint8_t mac_helper_default_key_source[8] = {0xff,0,0,0,0,0,0,0};

static uint8_t mac_helper_header_security_aux_header_length(uint8_t keyIdmode);
static uint8_t mac_helper_security_mic_length_get(uint8_t security_level);
static void mac_helper_keytable_pairwise_descriptor_set(struct mac_api_s *api, const uint8_t *key, const uint8_t *mac64, uint8_t attribute_id);

static int8_t mac_helper_pib_8bit_set(protocol_interface_info_entry_t *interface, mlme_attr_t attribute, uint8_t value)
{
    switch (attribute) {
        case macAutoRequestKeyIdMode:
            interface->mac_parameters->mac_key_id_mode = value;
            break;
        case macAutoRequestKeyIndex:
            interface->mac_parameters->mac_default_key_index = value;
            break;

        case macAutoRequestSecurityLevel:
        default:
            interface->mac_parameters->mac_security_level = value;
            break;
    }

    if (interface->mac_api && interface->mac_api->mlme_req ) {
        mlme_set_t set_req;
        set_req.attr = attribute;
        set_req.attr_index = 0;
        set_req.value_pointer = &value;
        set_req.value_size = 1;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    }

    return 0;
}

void mac_create_scan_request(mac_scan_type_t type, channel_list_s *chanlist, uint8_t scan_duration, mlme_scan_t *request)
{
    if( !chanlist || !request ){
        return;
    }

    memset(request, 0, sizeof(mlme_scan_t));

    request->ScanType = type;
    request->ScanChannels = *chanlist;

    request->ScanDuration = scan_duration;
}

nwk_pan_descriptor_t *mac_helper_select_best_lqi(nwk_pan_descriptor_t *list)
{
    nwk_pan_descriptor_t *best = list;
    //Analyze Best Result
    while (list) {
        tr_debug("LinkQuality: %i, LogicalCh: %i", list->pan_descriptor->LinkQuality, list->pan_descriptor->LogicalChannel);
        if (best->pan_descriptor->LinkQuality < list->pan_descriptor->LinkQuality) {
            best = list;
        }
        list = list->next;
    }
    return best;
}

void mac_helper_drop_selected_from_the_scanresult(nwk_scan_params_t *scanParams, nwk_pan_descriptor_t *selected)
{
    if( !scanParams || !selected ){
        return;
    }
    nwk_pan_descriptor_t *cur;
    nwk_pan_descriptor_t *prev = 0;
    cur = scanParams->nwk_response_info;

    while (cur) {
        if (cur == selected) {
            tr_debug("Clean Selected out from the list");
            if (prev) {
                prev->next = cur->next;
            } else {
                scanParams->nwk_response_info = cur->next;

            }
            scanParams->nwk_scan_res_size--;
            cur = 0;

        }
        if (cur) {
            prev = cur;
            cur = cur->next;
        }
    }
}

void mac_helper_free_scan_confirm(nwk_scan_params_t *params)
{
    if( !params ){
        return;
    }
    if (params->nwk_scan_res_size) {
        nwk_pan_descriptor_t *cur = params->nwk_response_info;
        nwk_pan_descriptor_t *tmp;
        tr_debug("Scanned Results");
        while (cur) {
            tmp = cur;
            cur = cur->next;
            tr_debug("Free NWK Structure");
            mac_helper_free_pan_descriptions(tmp);
        }

        params->nwk_response_info = 0;
        params->nwk_scan_res_size = 0;
    }
    params->nwk_cur_active = mac_helper_free_pan_descriptions(params->nwk_cur_active);
}

nwk_pan_descriptor_t * mac_helper_free_pan_descriptions(nwk_pan_descriptor_t *nwk_cur_active)
{
    if (nwk_cur_active) {
        ns_dyn_mem_free(nwk_cur_active->pan_descriptor);
        ns_dyn_mem_free(nwk_cur_active->beacon_payload);
        ns_dyn_mem_free(nwk_cur_active);
    }
    return NULL;
}

int8_t mac_helper_nwk_id_filter_set(const uint8_t *nw_id, nwk_filter_params_s *filter)
{
    if( !filter ){
        return -1;
    }
    int8_t ret_val = 0;
    if (nw_id) {
        if (filter->beacon_nwk_id_filter == 0) {
            filter->beacon_nwk_id_filter = ns_dyn_mem_alloc(16);
        }
        if (filter->beacon_nwk_id_filter) {
            memcpy(filter->beacon_nwk_id_filter, nw_id, 16);
        } else {
            ret_val = -1;
        }
    } else {
        ns_dyn_mem_free(filter->beacon_nwk_id_filter);
        filter->beacon_nwk_id_filter = 0;
    }
    return ret_val;
}

void mac_helper_panid_set(protocol_interface_info_entry_t *interface, uint16_t panId)
{
    interface->mac_parameters->pan_id = panId;
    mlme_set_t set_req;
    set_req.attr = macPANId;
    set_req.attr_index = 0;
    set_req.value_pointer = &panId;
    set_req.value_size = 2;
    interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
}

void mac_helper_mac16_address_set(protocol_interface_info_entry_t *interface, uint16_t mac16)
{
    interface->mac_parameters->mac_short_address = mac16;
    if (mac16 < 0xfffe) {
        interface->mac_parameters->shortAdressValid = true;
    } else {
        interface->mac_parameters->shortAdressValid = false;
    }
    mlme_set_t set_req;
    set_req.attr = macShortAddress;
    set_req.attr_index = 0;
    set_req.value_pointer = &mac16;
    set_req.value_size = 2;
    interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
}

uint16_t mac_helper_mac16_address_get(const protocol_interface_info_entry_t *interface)
{
    uint16_t shortAddress = 0xfffe;
    if (interface ) {
        shortAddress = interface->mac_parameters->mac_short_address;
    }
    return shortAddress;
}

uint16_t mac_helper_panid_get(const protocol_interface_info_entry_t *interface)
{
    uint16_t panId = 0xffff;
    if (interface) {
        panId = interface->mac_parameters->pan_id;
    }
    return panId;
}

void mac_helper_default_key_index_set(protocol_interface_info_entry_t *interface, uint8_t keyIndex)
{
    interface->mac_parameters->mac_default_key_index = keyIndex;
}

uint8_t mac_helper_default_key_index_get(protocol_interface_info_entry_t *interface)
{
    return interface->mac_parameters->mac_default_key_index;
}

void mac_helper_set_default_key_source(protocol_interface_info_entry_t *interface)
{
    mlme_set_t set_req;
    set_req.attr_index = 0;
    set_req.value_pointer = (void*)mac_helper_default_key_source;
    set_req.value_size = 8;
    //Set first default key source
    set_req.attr = macDefaultKeySource;
    interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    //Set first default key source
    set_req.attr = macAutoRequestKeySource;
    interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);

}

void mac_helper_default_security_level_set(protocol_interface_info_entry_t *interface, uint8_t securityLevel)
{
    bool security_enabled;
    if (securityLevel) {
        security_enabled = true;
    } else {
        security_enabled = false;
    }
    mac_helper_pib_8bit_set(interface,macAutoRequestSecurityLevel,  securityLevel);
    mac_helper_pib_boolean_set(interface, macSecurityEnabled, security_enabled);

}

uint8_t mac_helper_default_security_level_get(protocol_interface_info_entry_t *interface)
{
    return interface->mac_parameters->mac_security_level;
}

void mac_helper_default_security_key_id_mode_set(protocol_interface_info_entry_t *interface, uint8_t keyIdMode)
{
    mac_helper_pib_8bit_set(interface,macAutoRequestKeyIdMode,  keyIdMode);
}

uint8_t mac_helper_default_security_key_id_mode_get(protocol_interface_info_entry_t *interface)
{
    return interface->mac_parameters->mac_key_id_mode;
}

static void mac_helper_key_lookup_set(mlme_key_id_lookup_descriptor_t *lookup, uint8_t id)
{
    memcpy(lookup->LookupData, mac_helper_default_key_source, 8);
    lookup->LookupData[8] = id;
    lookup->LookupDataSize = 1;
}


static void mac_helper_keytable_descriptor_set(struct mac_api_s *api, const uint8_t *key, uint8_t id, uint8_t attribute_id) {
    mlme_set_t set_req;
    mlme_key_id_lookup_descriptor_t lookup_description;
    mlme_key_descriptor_entry_t key_description;
    if (key) {
        mac_helper_key_lookup_set(&lookup_description, id);
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
        memcpy(key_description.Key, key, 16);
        key_description.KeyIdLookupList = &lookup_description;
        key_description.KeyIdLookupListEntries = 1;
    } else {
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
    }
    set_req.attr = macKeyTable;
    set_req.attr_index = attribute_id;
    set_req.value_pointer = &key_description;
    set_req.value_size = sizeof(mlme_key_descriptor_entry_t);

    api->mlme_req(api, MLME_SET, &set_req);
}

static void mac_helper_keytable_pairwise_descriptor_set(struct mac_api_s *api, const uint8_t *key, const uint8_t *mac64, uint8_t attribute_id) {
    mlme_set_t set_req;
    mlme_key_id_lookup_descriptor_t lookup_description;
    mlme_key_descriptor_entry_t key_description;
    if (key) {
        memcpy(lookup_description.LookupData, mac64, 8);
        lookup_description.LookupData[8] = 0;
        lookup_description.LookupDataSize = 1;
        tr_debug("Key add %u index %s", attribute_id,trace_array(lookup_description.LookupData, 9));
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
        memcpy(key_description.Key, key, 16);
        key_description.KeyIdLookupList = &lookup_description;
        key_description.KeyIdLookupListEntries = 1;
    } else {
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));
    }
    set_req.attr = macKeyTable;
    set_req.attr_index = attribute_id;
    set_req.value_pointer = &key_description;
    set_req.value_size = sizeof(mlme_key_descriptor_entry_t);

    api->mlme_req(api,MLME_SET , &set_req);
}


int8_t mac_helper_security_default_key_set(protocol_interface_info_entry_t *interface, const uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    if (id == 0 || keyid_mode > 3) {
        return -1;
    }

    mac_helper_pib_8bit_set(interface,macAutoRequestKeyIndex, id);
    mac_helper_keytable_descriptor_set(interface->mac_api, key, id, interface->mac_parameters->mac_default_key_attribute_id);
    return 0;
}


int8_t mac_helper_security_pairwisekey_set(protocol_interface_info_entry_t *interface, const uint8_t *key, const uint8_t *mac_64, uint8_t key_attribute)
{
    if (key && !mac_64) {
        return -1;
    }

    mac_helper_keytable_pairwise_descriptor_set(interface->mac_api, key, mac_64, key_attribute);
    return 0;
}

int8_t mac_helper_security_next_key_set(protocol_interface_info_entry_t *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    if (id == 0 || keyid_mode > 3) {
        return -1;
    }

    interface->mac_parameters->mac_next_key_index = id;
    mac_helper_keytable_descriptor_set(interface->mac_api, key, id, interface->mac_parameters->mac_next_key_attribute_id);
    return 0;

}

int8_t mac_helper_security_prev_key_set(protocol_interface_info_entry_t *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode)
{
    if (id == 0 || keyid_mode > 3) {
        return -1;
    }

    interface->mac_parameters->mac_prev_key_index = id;
    mac_helper_keytable_descriptor_set(interface->mac_api, key, id, interface->mac_parameters->mac_prev_key_attribute_id);
    return 0;

}


void mac_helper_security_key_swap_next_to_default(protocol_interface_info_entry_t *interface)
{
    //Free old prev key
    /*
     * Update key setup next way
     *
     * Current Key -> Prev key
     * Next Key -> Current key
     * Prev Key ->Overwrite by for next Purpose
     */


    //Free current prev
    mac_helper_keytable_descriptor_set(interface->mac_api, NULL, 0, interface->mac_parameters->mac_prev_key_attribute_id);

    uint8_t prev_attribute = interface->mac_parameters->mac_prev_key_attribute_id; //save current pre for next purpose

    interface->mac_parameters->mac_prev_key_index = interface->mac_parameters->mac_default_key_index;
    interface->mac_parameters->mac_prev_key_attribute_id = interface->mac_parameters->mac_default_key_attribute_id;

    interface->mac_parameters->mac_default_key_index = interface->mac_parameters->mac_next_key_index;
    interface->mac_parameters->mac_default_key_attribute_id = interface->mac_parameters->mac_next_key_attribute_id;
    interface->mac_parameters->mac_next_key_index = 0;
    interface->mac_parameters->mac_next_key_attribute_id = prev_attribute;

    mac_helper_pib_8bit_set(interface,macAutoRequestKeyIndex,  interface->mac_parameters->mac_default_key_index);

}

void mac_helper_security_key_clean(protocol_interface_info_entry_t *interface) {
    if (interface->mac_api ) {
        mlme_set_t set_req;
        mlme_key_descriptor_entry_t key_description;
        memset(&key_description, 0, sizeof(mlme_key_descriptor_entry_t));

        set_req.attr = macKeyTable;

        set_req.value_pointer = &key_description;
        set_req.value_size = sizeof(mlme_key_descriptor_entry_t);
        set_req.attr_index = interface->mac_parameters->mac_prev_key_attribute_id;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
        set_req.attr_index = interface->mac_parameters->mac_default_key_attribute_id;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
        set_req.attr_index = interface->mac_parameters->mac_next_key_attribute_id;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    }
    interface->mac_parameters->mac_prev_key_index = 0;
    interface->mac_parameters->mac_default_key_index = 0;
    interface->mac_parameters->mac_next_key_index = 0;
}

void mac_helper_coordinator_address_set(protocol_interface_info_entry_t *interface, addrtype_t adr_type, uint8_t *adr_ptr)
{
    mlme_set_t set_req;
    set_req.attr_index = 0;

    if (adr_type == ADDR_802_15_4_SHORT) {
        memcpy(interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address, adr_ptr, 2);
        interface->mac_parameters->mac_cordinator_info.cord_adr_mode = MAC_ADDR_MODE_16_BIT;
        uint16_t short_addr = common_read_16_bit(interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address);
        set_req.attr = macCoordShortAddress;
        set_req.value_pointer = &short_addr;
        set_req.value_size = 2;
    } else if (adr_type == ADDR_802_15_4_LONG) {
        memcpy(interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address, adr_ptr, 8);
        interface->mac_parameters->mac_cordinator_info.cord_adr_mode = MAC_ADDR_MODE_64_BIT;
        set_req.attr = macCoordExtendedAddress;
        set_req.value_pointer = &interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address;
        set_req.value_size = 8;
    }

    if (interface->mac_api) {
        interface->mac_api->mlme_req(interface->mac_api, MLME_SET , &set_req);
    }
}

addrtype_t mac_helper_coordinator_address_get(protocol_interface_info_entry_t *interface, uint8_t *adr_ptr)
{
    addrtype_t ret = ADDR_NONE;
    if (!interface) {
        return ret;
    }

    if (interface->mac_parameters->mac_cordinator_info.cord_adr_mode == MAC_ADDR_MODE_16_BIT) {
        memcpy(adr_ptr,interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address, 2);
        ret = ADDR_802_15_4_SHORT;
    } else if (interface->mac_parameters->mac_cordinator_info.cord_adr_mode == MAC_ADDR_MODE_64_BIT) {
        memcpy(adr_ptr,interface->mac_parameters->mac_cordinator_info.mac_mlme_coord_address, 8);
        ret = ADDR_802_15_4_LONG;
    }
    return ret;
}

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))

static void mac_helper_beacon_payload_length_set_to_mac(protocol_interface_info_entry_t *interface, uint8_t len)
{
    if (interface->mac_api) {
        mlme_set_t set_req;
        set_req.attr = macBeaconPayloadLength;
        set_req.attr_index = 0;
        set_req.value_pointer = &len;
        set_req.value_size = 1;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    }
}

static void mac_helper_beacon_payload_set_to_mac(protocol_interface_info_entry_t *interface, const uint8_t *payload, uint8_t length)
{
    if (interface->mac_api) {
        mlme_set_t set_req;
        set_req.attr = macBeaconPayload;
        set_req.attr_index = 0;
        set_req.value_pointer = payload;
        set_req.value_size = length;
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    }
}



// XXX: a ns_dyn_mem_realloc() would be nice to have
uint8_t *mac_helper_beacon_payload_reallocate(protocol_interface_info_entry_t *interface, uint8_t len)
{

    if (len == interface->mac_parameters->mac_beacon_payload_size) {
        // no change to size, return the existing buff
        //Set allways length to zero for safe beacon payload manipulate
        mac_helper_beacon_payload_length_set_to_mac(interface,0);
        return interface->mac_parameters->mac_beacon_payload;
    }

    if (len == 0) {
        //SET MAC beacon payload to length to zero
        mac_helper_beacon_payload_length_set_to_mac(interface,0);
        ns_dyn_mem_free(interface->mac_parameters->mac_beacon_payload);
        interface->mac_parameters->mac_beacon_payload = NULL;
        interface->mac_parameters->mac_beacon_payload_size = 0;
        return NULL;
    }

    tr_debug("mac_helper_beacon_payload_reallocate, old len: %d, new: %d", interface->mac_parameters->mac_beacon_payload_size, len);

    uint8_t* temp_buff = ns_dyn_mem_alloc(len);

    if (temp_buff == NULL) {
        // no need to proceed, could not allocate more space
        return NULL;
    }

    //SET MAC beacon payload to length to zero
    mac_helper_beacon_payload_length_set_to_mac(interface,0);

    // copy data into new buffer before freeing old one
    if (interface->mac_parameters->mac_beacon_payload_size > 0) {
        const uint8_t min_len = MIN(len, interface->mac_parameters->mac_beacon_payload_size);

        memcpy(temp_buff, interface->mac_parameters->mac_beacon_payload, min_len);
        ns_dyn_mem_free(interface->mac_parameters->mac_beacon_payload);
    }

    //Set New Length and pointer to MAC

    interface->mac_parameters->mac_beacon_payload = temp_buff;
    interface->mac_parameters->mac_beacon_payload_size = len;

    return interface->mac_parameters->mac_beacon_payload;
}

int8_t mac_helper_beacon_payload_register(protocol_interface_info_entry_t *interface)
{
    mac_helper_beacon_payload_set_to_mac(interface, interface->mac_parameters->mac_beacon_payload, interface->mac_parameters->mac_beacon_payload_size);
    mac_helper_beacon_payload_length_set_to_mac(interface, interface->mac_parameters->mac_beacon_payload_size);

    return 0;
}

uint8_t *mac_helper_beacon_payload_pointer_get(protocol_interface_info_entry_t *interface)
{
    return interface->mac_parameters->mac_beacon_payload;
}

uint8_t mac_helper_beacon_payload_length_get(protocol_interface_info_entry_t *interface)
{
    return interface->mac_parameters->mac_beacon_payload_size;
}

int8_t mac_helper_pib_boolean_set(protocol_interface_info_entry_t *interface, mlme_attr_t attribute, bool value)
{

    switch (attribute) {
        case macSecurityEnabled:
            interface->mac_parameters->SecurityEnabled = value;
            break;

        case macRxOnWhenIdle:
            interface->mac_parameters->RxOnWhenIdle = value;
            break;

        case macPromiscuousMode:
            interface->mac_parameters->PromiscuousMode = value;
            break;

        case macGTSPermit:
            interface->mac_parameters->GTSPermit = value;
            break;

        case macAssociationPermit:
            interface->mac_parameters->AssociationPermit = value;
            break;

        case macAssociatedPANCoord:
            interface->mac_parameters->AssociatedPANCoord = value;
            break;

        case macTimestampSupported:
            interface->mac_parameters->TimestampSupported = value;
            break;

        case macBattLifeExt:
            interface->mac_parameters->BattLifeExt = value;
            break;

        case macAutoRequest:
            interface->mac_parameters->AutoRequest = value;
            break;
        case macThreadForceLongAddressForBeacon:
            break;
        default:
            return -1;
    }
    if (interface->mac_api && interface->mac_api->mlme_req) {
        mlme_set_t set_req;
        set_req.attr = attribute;
        set_req.attr_index = 0;
        set_req.value_pointer = &value;
        set_req.value_size = sizeof(bool);
        interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
    }

    return 0;
}

int8_t mac_helper_mac_channel_set(protocol_interface_info_entry_t *interface, uint8_t new_channel)
{

    if (interface->mac_parameters->mac_channel != new_channel) {

        interface->mac_parameters->mac_channel = new_channel;
        if (interface->mac_api && interface->mac_api->mlme_req ) {
            mlme_set_t set_req;
            set_req.attr = phyCurrentChannel;
            set_req.attr_index = 0;
            set_req.value_pointer = &new_channel;
            set_req.value_size = 1;
            interface->mac_api->mlme_req(interface->mac_api,MLME_SET , &set_req);
        }
    }
    return 0;
}

static bool mac_helper_write_16bit(uint16_t temp16, uint8_t *addrPtr)
{
    common_write_16_bit(temp16, addrPtr);
    return temp16 != 0xffff;
}

/* Write functions return "false" if they write an "odd" address, true if they
 * write a "normal" address. They still write odd addresses, as certain special
 * packets may want them, but this allows normal data paths to check and block
 * odd cases.
 * "Odd" is currently defined as PAN ID == 0xffff, or short address > 0xfffd.
 */
bool mac_helper_write_our_addr(protocol_interface_info_entry_t *interface, sockaddr_t *ptr)
{
    bool normal = true;

    //Set First PANID
    normal &= mac_helper_write_16bit(interface->mac_parameters->pan_id, ptr->address);

    if (ptr->addr_type != ADDR_802_15_4_LONG && ptr->addr_type != ADDR_802_15_4_SHORT) {
        if (interface->mac_parameters->shortAdressValid) {
            ptr->addr_type = ADDR_802_15_4_SHORT;
        } else {
            ptr->addr_type = ADDR_802_15_4_LONG;
        }
    }

    if (ptr->addr_type == ADDR_802_15_4_SHORT) {
        normal &= mac_helper_write_16bit(interface->mac_parameters->mac_short_address, &ptr->address[2]);
    } else {
        memcpy(&ptr->address[2], interface->mac, 8);
    }

    return normal;
}

int8_t mac_helper_mac64_set(protocol_interface_info_entry_t *interface, const uint8_t *mac64)
{
    memcpy(interface->mac, mac64, 8);
    if (interface->mac_api ) {
        interface->mac_api->mac64_set(interface->mac_api, mac64);
    }
    return 0;
}


/*
 * Given a buffer, with address and security flags set, compute the maximum
 * MAC payload that could be put in that buffer.
 */
uint_fast16_t mac_helper_max_payload_size(protocol_interface_info_entry_t *cur, uint_fast8_t frame_overhead)
{
    uint16_t max;

    max = cur->mac_api->phyMTU - frame_overhead;

    /* But if we want IEEE 802.15.4-2003 compatibility (and it looks like a
     * standard PHY), limit ourselves to the 2003 maximum */
    if (cur->mac_parameters->MacUnsusecured_2003_cab && max > MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE &&
            cur->mac_api->phyMTU == MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE) {
        max = MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE;
    }
    return max;
}

/*
 * Given a buffer, with address and security flags set, compute the MAC overhead
 * size once MAC header and footer are added.
 * May not be accurate if MAC_MAX_PHY_PACKET_SIZE isn't set, implying a
 * non-standard MAC.
 */
uint_fast8_t mac_helper_frame_overhead(protocol_interface_info_entry_t *cur, const buffer_t *buf)
{
    uint_fast8_t length = 15;

    /*8bytes src address, 2 frame control, 1 sequence, 2 pan-id, 2 FCS*/
    if (buf->src_sa.addr_type == ADDR_NONE) {
        if (cur->mac_parameters->shortAdressValid) {
            length -= 6; //Cut 6 bytes from src address
        }
    } else if (buf->src_sa.addr_type == ADDR_802_15_4_SHORT) {
        length -= 6; //Cut 6 bytes from src address
    }

    if (memcmp(buf->dst_sa.address, buf->src_sa.address, 2) == 0) {
        length -= 2; // Cut Pan-id
    }

    if (buf->dst_sa.addr_type == ADDR_802_15_4_LONG) {
        length += 10;
    } else if (buf->dst_sa.addr_type == ADDR_802_15_4_SHORT || buf->dst_sa.addr_type == ADDR_BROADCAST) {
        length += 4;
    }

    if (cur->mac_parameters->mac_security_level && (!buf->options.ll_security_bypass_tx)) {
        length += mac_helper_header_security_aux_header_length(cur->mac_parameters->mac_key_id_mode);
        length += mac_helper_security_mic_length_get(cur->mac_parameters->mac_security_level);
    }

    return length;
}

static uint8_t mac_helper_security_mic_length_get(uint8_t security_level) {
    uint8_t mic_length;
    switch (security_level) {
        case SEC_MIC32:
        case SEC_ENC_MIC32:
            mic_length = 4;
            break;
        case SEC_MIC64:
        case SEC_ENC_MIC64:
            mic_length = 8;
            break;
        case SEC_MIC128:
        case SEC_ENC_MIC128:
            mic_length = 16;
            break;
        case SEC_NONE:
        case SEC_ENC:
        default:
            mic_length = 0;
            break;
    }

    return mic_length;
}

static uint8_t mac_helper_header_security_aux_header_length(uint8_t keyIdmode) {

    uint8_t header_length = 5; //Header + 32-bit counter
    switch (keyIdmode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            header_length += 4; //64-bit key source first part
            /* fall through  */
        case MAC_KEY_ID_MODE_SRC4_IDX:
            header_length += 4; //32-bit key source inline
            /* fall through  */
        case MAC_KEY_ID_MODE_IDX:
            header_length += 1;
            break;
        default:
            break;
    }
    return header_length;
}

int8_t mac_helper_link_frame_counter_read(int8_t interface_id, uint32_t *seq_ptr)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_api || !seq_ptr) {
        return -1;
    }
    mlme_get_t get_req;
    get_req.attr = macFrameCounter;
    get_req.attr_index = 0;
    cur->mac_api->mlme_req(cur->mac_api, MLME_GET, &get_req);
    *seq_ptr = cur->mac_parameters->security_frame_counter;

    return 0;
}


int8_t mac_helper_link_frame_counter_set(int8_t interface_id, uint32_t seq_ptr)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_api) {
        return -1;
    }
    mlme_set_t set_req;
    set_req.attr = macFrameCounter;
    set_req.attr_index = 0;
    set_req.value_pointer = &seq_ptr;
    set_req.value_size = 4;
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);

    return 0;
}

void mac_helper_devicetable_remove(mac_api_t *mac_api, uint8_t attribute_index)
{
    if (!mac_api) {
        return;
    }

    mlme_device_descriptor_t device_desc;
    mlme_set_t set_req;
    memset(&device_desc, 0xff, sizeof(mlme_device_descriptor_t));

    set_req.attr = macDeviceTable;
    set_req.attr_index = attribute_index;
    set_req.value_pointer = (void*)&device_desc;
    set_req.value_size = sizeof(mlme_device_descriptor_t);
    tr_debug("unRegister Device");
    mac_api->mlme_req(mac_api,MLME_SET , &set_req);
}

void mac_helper_devicetable_set(mle_neigh_table_entry_t *entry_temp, protocol_interface_info_entry_t *cur, uint32_t frame_counter, uint8_t keyID, bool force_set)
{
    if (!cur->mac_api) {
        return;
    }

    if (!force_set && cur->mac_parameters->SecurityEnabled && cur->mac_parameters->mac_default_key_index != keyID) {
        tr_debug("Do not set counter by index %u != %u", cur->mac_parameters->mac_default_key_index, keyID);
        return;
    }

    mlme_device_descriptor_t device_desc;
    mlme_set_t set_req;
    device_desc.FrameCounter = frame_counter;
    device_desc.Exempt = false;
    device_desc.ShortAddress = entry_temp->short_adr;
    memcpy(device_desc.ExtAddress, entry_temp->mac64, 8);
    device_desc.PANId = mac_helper_panid_get(cur);


    set_req.attr = macDeviceTable;
    set_req.attr_index = entry_temp->attribute_index;
    set_req.value_pointer = (void*)&device_desc;
    set_req.value_size = sizeof(mlme_device_descriptor_t);
    tr_debug("Register Device");
    cur->mac_api->mlme_req(cur->mac_api,MLME_SET , &set_req);
}


int8_t mac_helper_mac_mlme_max_retry_set(int8_t interface_id, uint8_t mac_retry_set)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->mac_api) {
        return -1;
    }
    mlme_set_t set_req;
    set_req.attr = macMaxFrameRetries;
    set_req.attr_index = 0;
    set_req.value_pointer = &mac_retry_set;
    set_req.value_size = 1;
    cur->mac_api->mlme_req(cur->mac_api, MLME_SET, &set_req);

    return 0;
}
