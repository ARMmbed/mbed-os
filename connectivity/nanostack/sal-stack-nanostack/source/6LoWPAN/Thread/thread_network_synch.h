/*
 * Copyright (c) 2015-2018, Pelion and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * \file thread_network_synch.h
 *
 */

#ifndef THREAD_NETWORK_SYNCH_H_
#define THREAD_NETWORK_SYNCH_H_

struct protocol_interface_info_entry;
struct mac_neighbor_table_entry;

//Call This when clean synched networkdata
int thread_network_synch_data_free(int8_t interface_id);
//Call This when want synch last network setup
int thread_network_synch_start(int8_t interface_id);

uint16_t thread_network_synch_get_strored_mac16(int8_t interface_id);

bool thread_dynamic_storage_pending_configuration_exists(int8_t interface_id);
void thread_dynamic_storage_pending_configuration_read(int8_t interface_id, void *data, uint16_t size);
void thread_dynamic_storage_pending_configuration_store(int8_t interface_id, void *data, uint16_t size);

void thread_dynamic_storage_child_info_store(struct protocol_interface_info_entry *cur_interface, struct mac_neighbor_table_entry *child);
void thread_dynamic_storage_child_info_clear(int8_t interface_id, struct mac_neighbor_table_entry *child);
void thread_dynamic_storage_build_mle_table(int8_t interface_id);
void thread_dynamic_storage_frame_counter_store(int8_t interface_id, uint32_t mle_frame_counter, uint32_t mac_frame_counter);
uint32_t thread_dynamic_storage_mle_frame_counter_get(int8_t interfaceId);
uint32_t thread_dynamic_storage_mac_frame_counter_get(int8_t interfaceId);
uint32_t thread_dynamic_storage_network_seq_counter_get(int8_t interfaceId);
void thread_dynamic_storage_network_seq_counter_store(int8_t interface_id, uint32_t network_seq_counter);
int thread_pending_data_delete(int8_t interfaceId);
void thread_dynamic_storage_device_configuration_read(int8_t interface_id, uint8_t *mac_ptr, uint8_t *mleid_ptr);
void thread_dynamic_storage_device_configuration_store(int8_t interface_id, uint8_t *mac_ptr, uint8_t *mleid_ptr);

#endif /* THREAD_NETWORK_SYNCH_H_ */
