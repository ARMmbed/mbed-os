/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#ifndef LOWPAN_ADAPTATION_INTERFACE_H_
#define LOWPAN_ADAPTATION_INTERFACE_H_
#include "Core/include/ns_address_internal.h"

struct protocol_interface_info_entry;
struct mcps_data_conf_s;
struct mcps_data_ind_s;
struct buffer;
struct mpx_api_s;
struct mac_neighbor_table_entry;

int8_t lowpan_adaptation_interface_init(int8_t interface_id, uint16_t mac_mtu_size);

void lowpan_adaptation_interface_etx_update_enable(int8_t interface_id);

int8_t lowpan_adaptation_interface_free(int8_t interface_id);

int8_t lowpan_adaptation_interface_reset(int8_t interface_id);

int8_t lowpan_adaptation_interface_mpx_register(int8_t interface_id, struct mpx_api_s *mpx_api, uint16_t mpx_user_id);

/**
 * \brief call this before normatl TX. This function prepare buffer link spesific metadata and verify packet destination
 */
struct buffer *lowpan_adaptation_data_process_tx_preprocess(struct protocol_interface_info_entry *cur, struct buffer *buf);

int8_t lowpan_adaptation_interface_tx(struct protocol_interface_info_entry *cur, struct buffer *buf);

int8_t lowpan_adaptation_interface_tx_confirm(struct protocol_interface_info_entry *cur, const struct mcps_data_conf_s *confirm);

void lowpan_adaptation_interface_data_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data_ind);

struct buffer *lowpan_adaptation_reassembly(struct protocol_interface_info_entry *cur, struct buffer *buf);

bool lowpan_adaptation_tx_active(int8_t interface_id);

void lowpan_adaptation_neigh_remove_free_tx_tables(struct protocol_interface_info_entry *cur_interface, struct mac_neighbor_table_entry *entry_ptr);

int8_t lowpan_adaptation_free_messages_from_queues_by_address(struct protocol_interface_info_entry *cur, uint8_t *address_ptr, addrtype_t adr_type);

int8_t lowpan_adaptation_indirect_queue_params_set(struct protocol_interface_info_entry *cur, uint16_t indirect_big_packet_threshold, uint16_t max_indirect_big_packets_total, uint16_t max_indirect_small_packets_per_child);

#endif /* LOWPAN_ADAPTATION_INTERFACE_H_ */
