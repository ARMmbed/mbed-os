/*
 * Copyright (c) 2016-2018, Pelion and affiliates.
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

/**
 * \brief Nanostack Thread leader service. Service is responsible of Leader specific
 * tasks like:
 * -Router ID assignment
 * -Partitioning
 * -Leader TLV writing
 * -Leader COAP message handling
 */

#ifndef _THREAD_LEADER_SERVICE_H_
#define _THREAD_LEADER_SERVICE_H_
#ifdef HAVE_THREAD_LEADER_SERVICE

#include "MLE/mle_tlv.h"

/** Initialize Thread Leader Service
 *
 * \param interface interface id of this thread instance.
 * \param coap_service_id assigned COAP service id to use
 *
 * \return 0 for success, negative value for error
 */
int thread_leader_service_init(int8_t interface_id, uint8_t coap_service_id);

/** Delete Thread Leader Service
 *
 * \param interface interface id of this thread instance.
 */
void thread_leader_service_delete(int8_t interface_id);

/** Start and initialise CoAP thread leader.
 *  When becoming a leader this method enables all the leader management functionalities using Thread management CoAP.
 *
 * \param interface interface id of this thread instance.
 * \param assign_cb callback for router id assignment request.
 * \param release_cb callback for router id release request.
 *
 * \return 0 for success, negative value in case of error
 *
 */
int thread_leader_service_start(int8_t interface_id);

/** Stop Thread leader service and return to normal device mode.
 *  This closes the leader functionality services from CoAP, but leaves the normal node behavior.
 *
 * \param interface interface id of this thread instance.
 *
 */
void thread_leader_service_stop(int8_t interface_id);

/** Free Thread leader data
 *
 * \param thread_info
 */
void thread_leader_service_leader_data_free(struct thread_info_s *thread_info);

/** Generate new Thread partition
 *
 * \param interface_id current interface
 * \param newPartition true if new partition is needed
 */
void thread_leader_service_thread_partitition_generate(int8_t interface_id, bool newPartition);

/** Start Leader after sync
 *
 * \param interface_id current interface
 * \param routing routing info used to rebuild
 */
int thread_leader_service_thread_partitition_restart(int8_t interface_id, mle_tlv_info_t *routing);

/** Update ID
 *
 * \param cur current interface
 */
void thread_leader_service_update_id_set(protocol_interface_info_entry_t *cur);

/** Generate new Thread partition
 *
 * \param cur current interface
 * \param tickUpdate ticks
 */
void thread_leader_service_router_id_valid_lifetime_update(protocol_interface_info_entry_t *cur, uint32_t tickUpdate);

bool thread_leader_service_route_mask_bit_clear(thread_leader_info_t *info, uint8_t router_id);

void thread_leader_service_generate_network_data(protocol_interface_info_entry_t *cur);

void thread_leader_service_network_data_changed(protocol_interface_info_entry_t *cur, bool force_stable_update, bool force_unstable_update);

void thread_leader_service_timer(protocol_interface_info_entry_t *cur, uint32_t ticks);

int thread_leader_mleid_rloc_map_to_nvm_write(protocol_interface_info_entry_t *cur);

/** Get first child ID from network data based on parent ID
 *
 * \param thread_info
 * \param router_id router short address
 *
 * \return First child ID of the router children. 0 if no child found.
 */
uint16_t thread_leader_service_child_id_from_networkdata_get(struct thread_info_s *thread_info, uint16_t router_short_addr);

void thread_leader_service_router_state_changed(thread_info_t *thread_info, uint8_t router_id, bool available, int8_t interface_id);

#else

#define thread_leader_service_init(interface_id, coap_service_id) (0)

#define thread_leader_service_delete(interface_id)

#define thread_leader_service_start(interface_id) (-1)

#define thread_leader_service_stop(interface_id)

#define thread_leader_service_leader_data_free(thread_info)

#define thread_leader_service_thread_partitition_generate(interface_id,newPartition)

#define thread_leader_service_thread_partitition_restart(interface_id, routing) (-1)

#define thread_leader_service_update_id_set(cur);

#define thread_leader_service_router_id_valid_lifetime_update(cur,tickUpdate)

#define thread_leader_service_route_mask_bit_clear(info,router_id) false

#define thread_leader_service_generate_network_data(cur)

#define thread_leader_service_network_data_changed(cur, force_stable_update, force_unstable_update)

#define thread_leader_service_timer(cur,ticks)

#define thread_leader_service_child_id_from_networkdata_get(thread_info,router_short_addr)

#define thread_leader_service_router_state_changed(thread_info, router_id, available, interface_id)

#define thread_leader_mleid_rloc_map_to_nvm_write(cur) (0)

#endif /*HAVE_THREAD_LEADER_SERVICE*/

#endif /* _THREAD_LEADER_SERVICE_H_ */
