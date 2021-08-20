/*
 * Copyright (c) 2016-2020, Pelion and affiliates.
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

#ifndef MAC_SECURITY_MIB_H_
#define MAC_SECURITY_MIB_H_

#include "mlme.h"

struct protocol_interface_rf_mac_setup;
struct mac_description_storage_size_s;

typedef struct mlme_key_descriptor_s {
    mlme_key_id_lookup_descriptor_t *KeyIdLookupList;
    mlme_key_device_descriptor_t *KeyDeviceList;
    mlme_key_usage_descriptor_t *KeyUsageList;
    uint32_t *KeyDeviceFrameCounterList;
    uint32_t KeyFrameCounter;
    uint8_t Key[16];
    uint8_t KeyDeviceListSize;
    uint8_t KeyDeviceListEntries;
    uint8_t KeyIdLookupListEntries;
    uint8_t KeyUsageListEntries;
    uint8_t KeyUsageListSize;
    bool unique_key_descriptor: 1;
    bool KeyFrameCounterPerKey: 1;
} mlme_key_descriptor_t;

int8_t mac_sec_mib_init(struct protocol_interface_rf_mac_setup *rf_mac_setup, struct mac_description_storage_size_s *storage_sizes);

void mac_sec_mib_deinit(struct protocol_interface_rf_mac_setup *rf_mac_setup);

int8_t mac_sec_mib_frame_counter_per_key_set(struct protocol_interface_rf_mac_setup *rf_mac_setup, bool enabled);

int8_t mac_sec_mib_device_description_set(uint8_t atribute_index, mlme_device_descriptor_t *device_descriptor, struct protocol_interface_rf_mac_setup *rf_mac_setup);

int8_t mac_sec_mib_key_description_set(uint8_t atribute_index, mlme_key_descriptor_entry_t *key_descriptor, struct protocol_interface_rf_mac_setup *rf_mac_setup);

mlme_device_descriptor_t *mac_sec_mib_device_description_get_attribute_index(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint8_t attribute_index);

mlme_device_descriptor_t *mac_sec_mib_device_description_get(struct protocol_interface_rf_mac_setup *rf_mac_setup, const uint8_t *address, uint8_t type, uint16_t pan_id);

void mac_sec_mib_device_description_pan_update(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint16_t pan_id);

uint8_t mac_mib_device_descption_attribute_get_by_descriptor(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_device_descriptor_t *descriptor);

mlme_key_descriptor_t *mac_sec_key_description_get(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_security_t *key_source, uint8_t address_mode, uint8_t *address_ptr, uint16_t pan_id);

mlme_key_descriptor_t *mac_sec_key_description_get_by_attribute(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint8_t atribute_index);

mlme_key_device_descriptor_t *mac_sec_mib_key_device_description_list_update(mlme_key_descriptor_t *key_descpription_table);

mlme_key_device_descriptor_t *mac_sec_mib_key_device_description_discover_from_list(mlme_key_descriptor_t *key_descpription_table, uint8_t device_descriptor_handle);

void mac_sec_mib_device_description_blacklist(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint8_t device_handle);

void mac_sec_mib_key_device_frame_counter_set(mlme_key_descriptor_t *key_descpription_table, mlme_device_descriptor_t *device_info, uint32_t frame_counter, uint8_t attribute_index);

uint32_t mac_mib_key_device_frame_counter_get(mlme_key_descriptor_t *key_descpription_table, mlme_device_descriptor_t *device_info, uint8_t attribute_index);

uint32_t mac_sec_mib_key_outgoing_frame_counter_get(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_key_descriptor_t *key_descpription);

void mac_sec_mib_key_outgoing_frame_counter_set(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_key_descriptor_t *key_descpription, uint32_t value);

void mac_sec_mib_key_outgoing_frame_counter_increment(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_key_descriptor_t *key_descpription);

void mac_sec_mib_key_outgoing_frame_counter_decrement(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_key_descriptor_t *key_descpription);

#endif /* MAC_SECURITY_MIB_H_ */
