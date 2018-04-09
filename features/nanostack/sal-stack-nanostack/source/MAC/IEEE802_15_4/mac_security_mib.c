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
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "sw_mac.h"
#include "mac_common_defines.h"
#include "common_functions.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"

#define TRACE_GROUP "mMIB"
/**
 * Allocate device description table based on size
 */
static mlme_device_descriptor_t * mac_sec_mib_device_description_table_allocate(uint8_t table_size) {
    mlme_device_descriptor_t *table_ptr = ns_dyn_mem_alloc(sizeof(mlme_device_descriptor_t) * table_size);
    if (table_ptr) {
        memset(table_ptr, 0xff, (sizeof(mlme_device_descriptor_t) * table_size));
    }
    return table_ptr;
}

static mlme_key_descriptor_t * mac_sec_mib_key_description_table_allocate(uint8_t table_size) {
    mlme_key_descriptor_t *table_ptr = ns_dyn_mem_alloc(sizeof(mlme_key_descriptor_t) * table_size);
    if (table_ptr) {
        memset(table_ptr, 0, (sizeof(mlme_key_descriptor_t) * table_size));
    }
    return table_ptr;
}

static mlme_key_device_descriptor_t * mac_sec_mib_key_device_description_table_allocate(uint16_t list_size) {
    mlme_key_device_descriptor_t *table_ptr = ns_dyn_mem_alloc(sizeof(mlme_key_device_descriptor_t) * list_size);
    if (table_ptr) {
        memset(table_ptr, 0, (sizeof(mlme_key_device_descriptor_t) * list_size));
    }
    return table_ptr;
}

static mlme_key_usage_descriptor_t * mac_sec_mib_key_usage_table_allocate(uint16_t list_size) {

    mlme_key_usage_descriptor_t *table_ptr = ns_dyn_mem_alloc(sizeof(mlme_key_usage_descriptor_t) * list_size);
    if (table_ptr) {
        memset(table_ptr, 0, (sizeof(mlme_key_usage_descriptor_t) * list_size));
    }
    return table_ptr;
}


static mlme_key_id_lookup_descriptor_t * mac_sec_mib_key_lookup_table_allocate(uint16_t list_size) {

    mlme_key_id_lookup_descriptor_t *table_ptr = ns_dyn_mem_alloc(sizeof(mlme_key_id_lookup_descriptor_t) * list_size);
    if (table_ptr) {
        memset(table_ptr, 0, (sizeof(mlme_key_id_lookup_descriptor_t) * list_size));
    }
    return table_ptr;
}

static mlme_device_descriptor_t *mac_sec_mib_device_description_get_by_mac16(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint16_t mac16) {

    mlme_device_descriptor_t *device_table = rf_mac_setup->device_description_table;
    if (!device_table) {
        return NULL;
    }

    for (int i=0; i < rf_mac_setup->device_description_table_size; i++) {
        if (device_table->ShortAddress == mac16) {
            return device_table;
        }
        device_table++;
    }

    return NULL;
}

static mlme_device_descriptor_t * mac_sec_mib_device_description_get_by_mac64(protocol_interface_rf_mac_setup_s *rf_mac_setup, const uint8_t *mac64) {

    mlme_device_descriptor_t *device_table = rf_mac_setup->device_description_table;
    if (!device_table) {
        return NULL;
    }

    for (int i=0; i < rf_mac_setup->device_description_table_size; i++) {
        if (memcmp(device_table->ExtAddress, mac64, 8) == 0) {
            return device_table;
        }
        device_table++;
    }
    return NULL;
}

//Remove entry from the list
static void mac_sec_mib_key_device_description_remove_from_list(mlme_key_descriptor_t *key_descpription_table, uint8_t device_descriptor_handle)
{
    bool removed_entry = false;
    mlme_key_device_descriptor_t *cur, *prev;
    prev = NULL;
    cur = key_descpription_table->KeyDeviceList;
    for (uint8_t i=0; i< key_descpription_table->KeyDeviceListEntries; i++) {
        if (removed_entry) {
            //copy current to last one
            *prev = *cur;
        } else if (cur->DeviceDescriptorHandle == device_descriptor_handle) {
            removed_entry = true;
            tr_debug("Remove user %u from key", device_descriptor_handle);
        }
        prev = cur;
        cur++;
    }

    if (removed_entry) {
        key_descpription_table->KeyDeviceListEntries--;
    }
}

static void mac_sec_mib_device_description_remove(protocol_interface_rf_mac_setup_s *rf_mac_setup , uint8_t device_handle)
{
    for(uint8_t i=0; i< rf_mac_setup->key_description_table_size; i++) {
        mac_sec_mib_key_device_description_remove_from_list(&rf_mac_setup->key_description_table[i], device_handle);
    }
}


static int8_t  mac_sec_mib_device_description_table_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    ns_dyn_mem_free(rf_mac_setup->device_description_table);
    rf_mac_setup->device_description_table = NULL;
    rf_mac_setup->device_description_table_size = 0;
    return 0;
}

static void mac_sec_mib_security_material_free(protocol_interface_rf_mac_setup_s *rf_mac_setup) {
    ns_dyn_mem_free(rf_mac_setup->key_description_table);
    ns_dyn_mem_free(rf_mac_setup->key_device_desc_buffer);
    ns_dyn_mem_free(rf_mac_setup->key_usage_list_buffer);
    ns_dyn_mem_free(rf_mac_setup->key_lookup_buffer);
    rf_mac_setup->key_usage_list_buffer = NULL;
    rf_mac_setup->key_description_table = NULL;
    rf_mac_setup->key_lookup_buffer = NULL;
    rf_mac_setup->key_device_desc_buffer = NULL;
    rf_mac_setup->key_description_table_size = 0;
    rf_mac_setup->key_lookup_list_size = 0;
    rf_mac_setup->key_usage_list_size = 0;
}

static int8_t  mac_sec_mib_key_description_table_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup) {
    mac_sec_mib_security_material_free(rf_mac_setup);
    return 0;
}

static int8_t  mac_sec_mib_device_description_table_init(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t table_size) {

    rf_mac_setup->device_description_table = mac_sec_mib_device_description_table_allocate(table_size);
    if (!rf_mac_setup->device_description_table) {
        return -1;
    }

    rf_mac_setup->device_description_table_size = table_size;
    return 0;
}

static int8_t mac_sec_mib_key_description_table_init(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t table_size, uint8_t device_decription_size, uint8_t key_lookup_size, uint8_t key_usage_size) {

    rf_mac_setup->key_description_table = mac_sec_mib_key_description_table_allocate(table_size);
    rf_mac_setup->key_device_desc_buffer = mac_sec_mib_key_device_description_table_allocate(device_decription_size * table_size);
    rf_mac_setup->key_usage_list_buffer = mac_sec_mib_key_usage_table_allocate(key_usage_size * table_size);
    rf_mac_setup->key_lookup_buffer = mac_sec_mib_key_lookup_table_allocate(key_lookup_size * table_size);

    if (!rf_mac_setup->key_lookup_buffer || !rf_mac_setup->key_description_table || !rf_mac_setup->key_device_desc_buffer || !rf_mac_setup->key_usage_list_buffer) {
        mac_sec_mib_security_material_free(rf_mac_setup);
        return -1;
    }

    //SET description buffer sizes
    rf_mac_setup->key_description_table_size = table_size;
    rf_mac_setup->key_lookup_list_size = key_lookup_size;
    rf_mac_setup->key_usage_list_size = key_usage_size;

    mlme_key_descriptor_t *key_descriptor_list = rf_mac_setup->key_description_table;
    mlme_key_device_descriptor_t *key_device_decription = rf_mac_setup->key_device_desc_buffer;
    mlme_key_usage_descriptor_t *key_usage_ptr = rf_mac_setup->key_usage_list_buffer;
    mlme_key_id_lookup_descriptor_t *key_lookup = rf_mac_setup->key_lookup_buffer;


    for (uint8_t i = 0; i< rf_mac_setup->key_description_table_size; i++) {

        //Update Pointer values after first init
        if (i) {
            key_device_decription += device_decription_size;
            key_usage_ptr += key_usage_size;
            key_lookup += key_lookup_size;
            key_descriptor_list++;
        }
        key_descriptor_list->KeyDeviceListSize = device_decription_size;
        key_descriptor_list->KeyDeviceList = key_device_decription;
        key_descriptor_list->KeyUsageList = key_usage_ptr;
        key_descriptor_list->KeyIdLookupList = key_lookup;
    }

    return 0;

}

int8_t mac_sec_mib_device_description_set(uint8_t atribute_index, mlme_device_descriptor_t *device_descriptor, protocol_interface_rf_mac_setup_s *rf_mac_setup) {

    //validate index to list size
    if (!rf_mac_setup || !device_descriptor || atribute_index >= rf_mac_setup->device_description_table_size) {
        tr_debug("Too many Devices");
        return -1;
    }

    mlme_device_descriptor_t *device_ptr = rf_mac_setup->device_description_table + atribute_index;

    //Copy description
    if (memcmp(device_ptr->ExtAddress, device_descriptor->ExtAddress, 8)) {
        //Remove last handles key user's
        mac_sec_mib_device_description_remove(rf_mac_setup, atribute_index);
        tr_debug("Over write %u, mac16 %x mac64: %s, %"PRIu32, atribute_index, device_ptr->ShortAddress, trace_array(device_ptr->ExtAddress, 8), device_ptr->FrameCounter);
    }

    tr_debug("Set %u, mac16 %x mac64: %s, %"PRIu32, atribute_index, device_descriptor->ShortAddress, trace_array(device_descriptor->ExtAddress, 8), device_descriptor->FrameCounter);

    *device_ptr = *device_descriptor;
    return 0;
}

int8_t mac_sec_mib_key_description_set(uint8_t atribute_index, mlme_key_descriptor_entry_t *key_descriptor, protocol_interface_rf_mac_setup_s *rf_mac_setup) {

    //validate index to list size
    if (!rf_mac_setup || !key_descriptor || atribute_index >= rf_mac_setup->key_description_table_size) {
        return -1;
    }

    if (key_descriptor->KeyDeviceListEntries > rf_mac_setup->key_description_table_size || (key_descriptor->KeyDeviceListEntries && !key_descriptor->KeyDeviceList)) {
        return -1;
    }

    if (key_descriptor->KeyIdLookupListEntries > rf_mac_setup->key_lookup_list_size || (key_descriptor->KeyIdLookupListEntries && !key_descriptor->KeyIdLookupList)) {
        return -1;
    }

    if (key_descriptor->KeyUsageListEntries > rf_mac_setup->key_usage_list_size || (key_descriptor->KeyUsageListEntries && !key_descriptor->KeyUsageList)) {
        return -1;
    }

    mlme_key_descriptor_t *key_ptr = rf_mac_setup->key_description_table + atribute_index;

    //Copy description
    tr_debug("Set key %"PRIu8, atribute_index);

    memcpy(key_ptr->Key, key_descriptor->Key, 16);
    key_ptr->KeyDeviceListEntries = key_descriptor->KeyDeviceListEntries;
    key_ptr->unique_key_descriptor = false;

    if(key_ptr->KeyDeviceListEntries) {
        memcpy(key_ptr->KeyDeviceList,key_descriptor->KeyDeviceList , sizeof(mlme_key_device_descriptor_t) * key_ptr->KeyDeviceListEntries);
        //Check unique key
        if (key_ptr->KeyDeviceListEntries == 1) {
            key_ptr->unique_key_descriptor = key_descriptor->KeyDeviceList->UniqueDevice;
        }
    }

    key_ptr->KeyIdLookupListEntries = key_descriptor->KeyIdLookupListEntries;

    if(key_ptr->KeyIdLookupListEntries) {
        memcpy(key_ptr->KeyIdLookupList,key_descriptor->KeyIdLookupList , sizeof(mlme_key_id_lookup_descriptor_t) * key_ptr->KeyIdLookupListEntries);
    }

    key_ptr->KeyUsageListEntries = key_descriptor->KeyUsageListEntries;

    if(key_ptr->KeyUsageListEntries) {
        memcpy(key_ptr->KeyUsageList,key_descriptor->KeyUsageList , sizeof(mlme_key_usage_descriptor_t) * key_ptr->KeyUsageListEntries);
    }

    return 0;
}

mlme_device_descriptor_t *mac_sec_mib_device_description_get_attribute_index(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t attribute_index)
{
    if (!rf_mac_setup || attribute_index >= rf_mac_setup->device_description_table_size) {
        return NULL;
    }
    return rf_mac_setup->device_description_table + attribute_index;
}

mlme_device_descriptor_t *mac_sec_mib_device_description_get(protocol_interface_rf_mac_setup_s *rf_mac_setup, const uint8_t *address, uint8_t type)
{
    if( rf_mac_setup ){
        if (type == MAC_ADDR_MODE_16_BIT) {
            uint16_t short_id = common_read_16_bit(address);
            return  mac_sec_mib_device_description_get_by_mac16(rf_mac_setup, short_id);
        } else if (type == MAC_ADDR_MODE_64_BIT) {
            return mac_sec_mib_device_description_get_by_mac64(rf_mac_setup, address);
        }
    }

    return NULL;
}

uint8_t mac_mib_device_descption_attribute_get_by_descriptor(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_device_descriptor_t *descriptor)
{
    if( !rf_mac_setup || !descriptor ){
        return 0xff;
    }
    mlme_device_descriptor_t *device_table = rf_mac_setup->device_description_table;
    for (uint8_t i=0; i < rf_mac_setup->device_description_table_size; i++) {
        if (device_table == descriptor) {
            return i;
        }
        device_table++;
    }
    return 0xff;
}


static bool mac_sec_key_description_lookup_validate(mlme_key_descriptor_t * key_description, uint8_t *lookupdata) {
    uint8_t lookup_length;
    mlme_key_id_lookup_descriptor_t *cur_lookup_ptr = key_description->KeyIdLookupList;

    for (uint8_t i=0; i<key_description->KeyIdLookupListEntries; i++) {
        if (cur_lookup_ptr->LookupDataSize ) {
            lookup_length = 9;
        } else {
            lookup_length = 5;
        }

        if (memcmp(cur_lookup_ptr->LookupData, lookupdata, lookup_length) == 0) {
            return true;
        }
        cur_lookup_ptr++;
    }
    return false;
}

mlme_key_descriptor_t * mac_sec_key_description_get(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_security_t *key_source, uint8_t address_mode, uint8_t *address_ptr, uint16_t pan_id)
{
    if( !rf_mac_setup || !key_source ){
        return NULL;
    }

    mlme_key_descriptor_t *key_description;
    uint8_t lookup_data[9];
    memset(lookup_data, 0, 9);

    switch (key_source->KeyIdMode) {
        case MAC_KEY_ID_MODE_IMPLICIT:
            if (address_mode == MAC_ADDR_MODE_64_BIT) {
                memcpy(lookup_data, address_ptr, 8);
            } else if (address_mode == MAC_ADDR_MODE_16_BIT) {
                common_write_16_bit(pan_id,lookup_data);
                memcpy(&lookup_data[2], address_ptr, 2);
            } else {
                return NULL; //Not supported this yet
            }
            break;
        case MAC_KEY_ID_MODE_IDX:
            //SET Default keysource
            memcpy(lookup_data, rf_mac_setup->mac_default_key_source, 8);
            lookup_data[8] = key_source->KeyIndex;
            break;

        case MAC_KEY_ID_MODE_SRC4_IDX:
            memcpy(lookup_data, key_source->Keysource ,4);
            lookup_data[4] = key_source->KeyIndex;
            break;

        case MAC_KEY_ID_MODE_SRC8_IDX:
            memcpy(lookup_data, key_source->Keysource ,8);
            lookup_data[8] = key_source->KeyIndex;
            break;
    }
    key_description = rf_mac_setup->key_description_table;
    for (uint8_t i = 0; i< rf_mac_setup->key_description_table_size; i++) {

        if (mac_sec_key_description_lookup_validate(key_description, lookup_data)) {
            return key_description;
        }
        key_description++;
    }

    tr_debug("LookuPdata search fail %s", trace_array(lookup_data, 9));

    return NULL;
}

int8_t mac_sec_mib_init(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_description_storage_size_t *storage_sizes) {

    if( !rf_mac_setup || !storage_sizes){
        return -1;
    }

    mac_sec_mib_deinit(rf_mac_setup);
    if (mac_sec_mib_device_description_table_init(rf_mac_setup, storage_sizes->device_decription_table_size)!= 0) {
        return -1;
    }

    if (mac_sec_mib_key_description_table_init(rf_mac_setup, storage_sizes->key_description_table_size, storage_sizes->device_decription_table_size, storage_sizes->key_lookup_size, storage_sizes->key_usage_size)) {
        mac_sec_mib_deinit(rf_mac_setup);
        return -1;
    }

    return 0;
}

void mac_sec_mib_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if( !rf_mac_setup ){
        return;
    }
    mac_sec_mib_device_description_table_deinit(rf_mac_setup);
    mac_sec_mib_key_description_table_deinit(rf_mac_setup);

}

//allocate new entry and update entries size
mlme_key_device_descriptor_t * mac_sec_mib_key_device_description_list_update(mlme_key_descriptor_t *key_descpription_table)
{
    if (!key_descpription_table || key_descpription_table->KeyDeviceListEntries == key_descpription_table->KeyDeviceListSize) {
        return NULL;
    }
    mlme_key_device_descriptor_t * new_entry = key_descpription_table->KeyDeviceList;
    new_entry += key_descpription_table->KeyDeviceListEntries++;
    new_entry->Blacklisted = false;
    new_entry->UniqueDevice = false;
    return new_entry;
}

//Discover device entry from the list
mlme_key_device_descriptor_t * mac_sec_mib_key_device_description_discover_from_list(mlme_key_descriptor_t *key_description_table, uint8_t device_descriptor_handle)
{
    if( key_description_table ){
        mlme_key_device_descriptor_t * entry = key_description_table->KeyDeviceList;
        for (uint8_t i=0; i< key_description_table->KeyDeviceListEntries; i++) {
            if (entry->DeviceDescriptorHandle == device_descriptor_handle) {
                return entry;
            }
            entry++;
        }
    }

    return NULL;
}

//Black list device from key descriptior list

void mac_sec_mib_device_description_blacklist(protocol_interface_rf_mac_setup_s *rf_mac_setup , uint8_t device_handle) {

    if( !rf_mac_setup ){
        return;
    }
    mlme_key_device_descriptor_t *descriptor;
    for(uint8_t i=0; i< rf_mac_setup->key_description_table_size; i++) {
        descriptor = mac_sec_mib_key_device_description_discover_from_list(&rf_mac_setup->key_description_table[i], device_handle);
        if (descriptor) {
            tr_debug("Black listed device %u lookup%s", device_handle, trace_array(rf_mac_setup->key_description_table[i].Key, 16));
            descriptor->Blacklisted = true;
        }

    }
}


