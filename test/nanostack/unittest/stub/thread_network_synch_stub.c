/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "thread_management_if.h"
#include "mle.h"
#include <ns_types.h>
#include "NWK_INTERFACE/Include/protocol.h"

int thread_network_synch_start(int8_t interface_id)
{
    return 0;
}

int thread_network_synch_data_free(int8_t interface_id)
{
    return 0;
}

uint16_t thread_network_synch_get_strored_mac16(int8_t interface_id)
{
    return 0;
}

uint32_t thread_dynamic_storage_mle_frame_counter_get(int8_t interfaceId)
{
    return 0;
}
void thread_dynamic_storage_frame_counter_store(int8_t interface_id, uint32_t mle_frame_counter, uint32_t mac_frame_counter)
{
}
uint32_t thread_dynamic_storage_mac_frame_counter_get(int8_t interfaceId)
{
    return 0;
}

void thread_dynamic_storage_pending_configuration_store(int8_t interface_id, link_configuration_s *pendingConfiguration, uint64_t pending_timestamp)
{
    return;
}
link_configuration_s *thread_network_synch_get_stored_pending_set(int8_t interface_id, uint64_t *pending_timestamp)
{
    return NULL;
}

void thread_dynamic_storage_child_info_store(int8_t interface_id, mle_neigh_table_entry_t *child)
{
}

void thread_dynamic_storage_child_info_clear(int8_t interface_id, mle_neigh_table_entry_t *child)
{
}

void thread_dynamic_storage_build_mle_table(int8_t interface_id)
{
}

void thread_dynamic_storage_network_seq_counter_store(int8_t interface_id, uint32_t network_seq_counter)
{
    return;
}

uint32_t thread_dynamic_storage_network_seq_counter_get(int8_t interfaceId)
{
    return 0;
}
void thread_nvm_store_link_info_file_read()
{
}
bool thread_nvm_store_link_info_get(uint8_t *parent_mac64, uint16_t *my_short_address)
{
}

void thread_nvm_store_link_info_clear()
{
}

void thread_nvm_store_link_info_file_write(protocol_interface_info_entry_t *cur)
{
}
