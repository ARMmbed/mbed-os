/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
 * \file thread_router_bootstrap.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_ROUTER_BOOTSTRAP_H_
#define THREAD_ROUTER_BOOTSTRAP_H_
#include "Service_Libs/mle_service/mle_service_api.h"

#ifdef HAVE_THREAD_ROUTER

struct protocol_interface_info_entry;
struct thread_info_s;
struct mle_security_header;
struct buffer;

void thread_router_bootstrap_reed_advertisements_start(protocol_interface_info_entry_t *cur);
int thread_router_bootstrap_mle_advertise(struct protocol_interface_info_entry *cur);

void thread_router_bootstrap_child_information_clear(protocol_interface_info_entry_t *cur);
int thread_router_bootstrap_reset_child_info(protocol_interface_info_entry_t *cur, mle_neigh_table_entry_t *child);
uint16_t thread_router_bootstrap_child_count_get(protocol_interface_info_entry_t *cur);
void thread_router_bootstrap_child_id_handler(struct protocol_interface_info_entry *cur);
void thread_router_bootstrap_child_id_reject(struct protocol_interface_info_entry *cur);
void thread_router_bootstrap_router_id_request(struct protocol_interface_info_entry *cur, uint8_t status);
void thread_router_bootstrap_router_id_release(struct protocol_interface_info_entry *cur);


int thread_router_bootstrap_link_synch_start(struct protocol_interface_info_entry *cur);
bool thread_router_bootstrap_router_downgrade(struct protocol_interface_info_entry *cur);
bool thread_router_bootstrap_reed_upgrade(struct protocol_interface_info_entry *cur);
void thread_router_bootstrap_active_router_attach(struct protocol_interface_info_entry *cur);
int thread_router_bootstrap_route_tlv_push(protocol_interface_info_entry_t *cur, uint8_t *route_tlv, uint8_t route_len, uint8_t linkMargin, mle_neigh_table_entry_t *entry);
void thread_router_bootstrap_mle_receive_cb(int8_t interface_id, mle_message_t *mle_msg, struct mle_security_header *security_headers);
void thread_router_bootstrap_timer(protocol_interface_info_entry_t *cur, uint32_t ticks);
uint32_t thread_router_bootstrap_random_upgrade_jitter();
void thread_router_bootstrap_advertiment_analyze(protocol_interface_info_entry_t *cur, uint8_t *src_address, mle_neigh_table_entry_t *entry_temp, uint16_t shortAddress);

void thread_router_bootstrap_multicast_forwarder_enable(protocol_interface_info_entry_t *cur, buffer_t *buf);
void thread_router_bootstrap_anycast_address_register(protocol_interface_info_entry_t *cur);
void thread_router_bootstrap_network_data_distribute(protocol_interface_info_entry_t *cur);
bool thread_router_bootstrap_routing_allowed(struct protocol_interface_info_entry *cur);
void thread_router_bootstrap_address_change_notify_send(protocol_interface_info_entry_t *cur);

#else

#define thread_router_bootstrap_reed_advertisements_start(cur)
#define thread_router_bootstrap_mle_advertise(cur) (-1)

#define thread_router_bootstrap_child_information_clear(cur)
#define thread_router_bootstrap_child_count_get(cur) 0
#define thread_router_bootstrap_child_id_handler(cur)
#define thread_router_bootstrap_child_id_reject(cur)
#define thread_router_bootstrap_router_id_request(cur, status)
#define thread_router_bootstrap_router_id_release(cur)

#define thread_router_bootstrap_link_synch_start(cur) (-1)
#define thread_router_bootstrap_router_downgrade(cur) false
#define thread_router_bootstrap_reed_upgrade(cur) false
#define thread_router_bootstrap_active_router_attach(cur)
#define thread_router_bootstrap_routing_activate(cur)
#define thread_router_bootstrap_route_tlv_push(cur, route_tlv, route_len, linkMargin, entry)
#define thread_router_bootstrap_mle_receive_cb(interface_id,mle_msg, security_headers)
#define thread_router_bootstrap_timer(cur, ticks)
#define thread_router_bootstrap_random_upgrade_jitter() 0;
#define thread_router_bootstrap_advertiment_analyze(cur, src_address, entry_temp, shortAddress)
#define thread_router_bootstrap_reset_child_info(cur, child) 0

#define thread_router_bootstrap_multicast_forwarder_enable(cur, buf) NULL
#define thread_router_bootstrap_anycast_address_register(cur)
#define thread_router_bootstrap_network_data_distribute(cur)
#define thread_router_bootstrap_routing_allowed(cur) false
#define thread_router_bootstrap_address_change_notify_send(cur)

#endif/*HAVE_THREAD_ROUTER*/

#endif /* THREAD_ROUTER_BOOTSTRAP_H_ */
