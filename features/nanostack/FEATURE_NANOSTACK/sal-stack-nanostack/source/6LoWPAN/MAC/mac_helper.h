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

#ifndef MAC_HELPER_H
#define MAC_HELPER_H

#include "mlme.h"
#include "Core/include/address.h"

struct channel_list_s;
struct nwk_scan_params;
struct nwk_filter_params;
struct nwk_pan_descriptor_t;
struct protocol_interface_info_entry;
struct ns_sockaddr;
struct buffer;
struct mac_api_s;
struct mle_neigh_table_entry_t;


void mac_create_scan_request(mac_scan_type_t type, struct channel_list_s *chanlist, uint8_t scan_duration, struct mlme_scan_s *request);

struct nwk_pan_descriptor_t *mac_helper_select_best_lqi(struct nwk_pan_descriptor_t *list);

void mac_helper_drop_selected_from_the_scanresult(struct nwk_scan_params *scanParams, struct nwk_pan_descriptor_t *selected);

void mac_helper_free_scan_confirm(struct nwk_scan_params *params);

struct nwk_pan_descriptor_t * mac_helper_free_pan_descriptions(struct nwk_pan_descriptor_t *nwk_cur_active);

int8_t mac_helper_nwk_id_filter_set(const uint8_t *nw_id, struct nwk_filter_params *filter);

void mac_helper_panid_set(struct protocol_interface_info_entry *interface, uint16_t panId);

void mac_helper_mac16_address_set(struct protocol_interface_info_entry *interface, uint16_t mac16);

uint16_t mac_helper_mac16_address_get(const struct protocol_interface_info_entry *interface);

uint16_t mac_helper_panid_get(const struct protocol_interface_info_entry *interface);

void mac_helper_set_default_key_source(struct protocol_interface_info_entry *interface);

void mac_helper_default_security_level_set(struct protocol_interface_info_entry *interface, uint8_t securityLevel);

uint8_t mac_helper_default_security_level_get(struct protocol_interface_info_entry *interface);

void mac_helper_default_security_key_id_mode_set(struct protocol_interface_info_entry *interface, uint8_t keyIdMode);

uint8_t mac_helper_default_security_key_id_mode_get(struct protocol_interface_info_entry *interface);

void mac_helper_default_key_index_set(struct protocol_interface_info_entry *interface, uint8_t keyIndex);

uint8_t mac_helper_default_key_index_get(struct protocol_interface_info_entry *interface);

int8_t mac_helper_security_default_key_set(struct protocol_interface_info_entry *interface, const uint8_t *key, uint8_t id, uint8_t keyid_mode);

int8_t mac_helper_security_next_key_set(struct protocol_interface_info_entry *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode);

int8_t mac_helper_security_prev_key_set(struct protocol_interface_info_entry *interface, uint8_t *key, uint8_t id, uint8_t keyid_mode);

void mac_helper_security_key_swap_next_to_default(struct protocol_interface_info_entry *interface);

int8_t mac_helper_security_pairwisekey_set(struct protocol_interface_info_entry *interface, const uint8_t *key, const uint8_t *mac_64, uint8_t key_attribute);

void mac_helper_security_key_clean(struct protocol_interface_info_entry *interface);

void mac_helper_coordinator_address_set(struct protocol_interface_info_entry *interface, addrtype_t adr_type, uint8_t *adr_ptr);

addrtype_t mac_helper_coordinator_address_get(struct protocol_interface_info_entry *interface, uint8_t *adr_ptr);

/* Beacon Payload API */
uint8_t *mac_helper_beacon_payload_reallocate(struct protocol_interface_info_entry *interface, uint8_t len);

int8_t mac_helper_beacon_payload_register(struct protocol_interface_info_entry *interface);

uint8_t *mac_helper_beacon_payload_pointer_get(struct protocol_interface_info_entry *interface);

uint8_t mac_helper_beacon_payload_length_get(struct protocol_interface_info_entry *interface);
/* Beacon Payload API end*/

int8_t mac_helper_pib_boolean_set(struct protocol_interface_info_entry *interface, mlme_attr_t attribute, bool value);

int8_t mac_helper_mac_channel_set(struct protocol_interface_info_entry *interface, uint8_t new_channel);

bool mac_helper_write_our_addr(struct protocol_interface_info_entry *interface, struct ns_sockaddr *ptr);

int8_t mac_helper_mac64_set(struct protocol_interface_info_entry *interface, const uint8_t *mac64);

uint_fast16_t mac_helper_max_payload_size(struct protocol_interface_info_entry *cur, uint_fast8_t frame_overhead);

uint_fast8_t mac_helper_frame_overhead(struct protocol_interface_info_entry *cur, const struct buffer *buf);

int8_t mac_helper_link_frame_counter_read(int8_t interface_id, uint32_t *seq_ptr);

int8_t mac_helper_link_frame_counter_set(int8_t interface_id, uint32_t seq_ptr);

void mac_helper_devicetable_remove(struct mac_api_s *mac_api, uint8_t attribute_index);

void mac_helper_devicetable_set(struct mle_neigh_table_entry_t *entry_temp, struct protocol_interface_info_entry *cur, uint32_t frame_counter, uint8_t keyID);

int8_t mac_helper_mac_mlme_max_retry_set(int8_t interface_id, uint8_t mac_retry_set);

#endif // MAC_HELPER_H
