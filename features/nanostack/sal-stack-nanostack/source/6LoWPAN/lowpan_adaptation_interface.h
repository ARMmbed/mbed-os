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

#ifndef LOWPAN_ADAPTATION_INTERFACE_H_
#define LOWPAN_ADAPTATION_INTERFACE_H_
#include "Core/include/address.h"

struct protocol_interface_info_entry;
struct mcps_data_conf_s;
struct buffer;
int8_t lowpan_adaptation_interface_init(int8_t interface_id, uint16_t mac_mtu_size);

int8_t lowpan_adaptation_interface_free(int8_t interface_id);

int8_t lowpan_adaptation_interface_reset(int8_t interface_id);

/**
 * \brief call this before normatl TX. This function prepare buffer link spesific metadata and verify packet destination
 */
struct buffer * lowpan_adaptation_data_process_tx_preprocess(struct protocol_interface_info_entry *cur, struct buffer *buf);

int8_t lowpan_adaptation_interface_tx(struct protocol_interface_info_entry *cur, struct buffer *buf);

int8_t lowpan_adaptation_interface_tx_confirm(struct protocol_interface_info_entry *cur, const struct mcps_data_conf_s *confirm);

struct buffer *lowpan_adaptation_reassembly(struct protocol_interface_info_entry *cur, struct buffer *buf);

bool lowpan_adaptation_tx_active(int8_t interface_id);

int8_t lowpan_adaptation_indirect_free_messages_from_queues_by_address(struct protocol_interface_info_entry *cur, uint8_t *address_ptr, addrtype_t adr_type);

int8_t lowpan_adaptation_indirect_queue_params_set(struct protocol_interface_info_entry *cur, uint16_t indirect_big_packet_threshold, uint16_t max_indirect_big_packets_total, uint16_t max_indirect_small_packets_per_child);

#endif /* LOWPAN_ADAPTATION_INTERFACE_H_ */
