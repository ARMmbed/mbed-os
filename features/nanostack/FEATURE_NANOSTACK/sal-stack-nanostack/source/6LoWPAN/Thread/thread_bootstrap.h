/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 * \file thread_bootstrap.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_BOOTSTRAP_H_
#define THREAD_BOOTSTRAP_H_

#include "eventOS_event.h"
#include "net_polling_api.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#define THREAD_NETWORK_ACTIVE_SCAN_COUNTER 3
/** Thread Attach Retry limiters */
#define THREAD_CHILD_ID_REQUEST_MAX_RETRY_CNT 3
#define THREAD_PARENT_REQUEST_MAX_RETRY_CNT 2
#define THREAD_REQUEST_MAX_RETRY_CNT 3

#define THREAD_CHILD_ID_TIMEOUT (2 * 1000) /* 2 second */

struct thread_info_s;
struct protocol_interface_info_entry;
struct thread_leader_data_s;
struct link_configuration;
struct mle_neigh_table_entry_t;
struct mle_tlv_info_s;

typedef enum {
    CON_ERROR_POLL,
    CON_ERROR_LINK_TX_FAIL,
    CON_ERROR_NO_THREAD_NETWORK_AVAILABLE,
    CON_ERROR_NEIGHBOR_UNREACHABLE,
    CON_ERROR_NETWORK_KICK,
    CON_ERROR_NETWORK_REATTACH,
    CON_ERROR_PARTITION_MERGE,
    CON_ERROR_NETWORK_ATTACH_FAIL,
    CON_PARENT_CONNECT_DOWN
} nwk_connect_error_types;


typedef enum {
    THREAD_INIT_EVENT = 0, /**< Initialize Thread Tasklet*/
    THREAD_CHILD_UPDATE,
    THREAD_BOOTSTRAP_RESET,
    THREAD_ATTACH_READY,
    THREAD_ATTACH_UPGRADE_REED,
    THREAD_ATTACH_DOWNGRADE_ROUTER,
    THREAD_ATTACH_ACTIVE_ROUTER,
    THREAD_ATTACH_ROUTER_ID_GET_FAIL,
    THREAD_ATTACH_ROUTER_ID_RELEASED,
    THREAD_CHILD_ID_REQUEST,
    THREAD_ANNOUNCE_ACTIVE

} thread_bootsrap_event_type_e;

typedef enum {
    THREAD_NORMAL_ATTACH = 0,
    THREAD_REATTACH,
    THREAD_REATTACH_REED,
    THREAD_PARTITION_MERGE,
    THREAD_ANY_ATTACH
} thread_bootsrap_state_type_e;

#ifdef HAVE_THREAD

/** Reset bootstrap and start using new settings
 */
void thread_bootstrap_reset_restart(int8_t interface);

/* removes child id when becoming router*/

uint8_t thread_mode_get_by_interface_ptr(struct protocol_interface_info_entry *cur);
void thread_bootstrap_device_synch_finish(protocol_interface_info_entry_t *cur);
int8_t thread_mle_class_init(int8_t interface_id);
void thread_general_mle_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers);
int thread_bootstrap_reset_child_info(protocol_interface_info_entry_t *cur, struct mle_neigh_table_entry_t *child);
void thread_bootstrap_ready(struct protocol_interface_info_entry *cur);
int thread_bootstrap_reset(struct protocol_interface_info_entry *cur);
void thread_clean_all_routers_from_neighbor_list(int8_t interface_id);
bool thread_bootstrap_request_network_data(struct protocol_interface_info_entry *cur, struct thread_leader_data_s *leaderData, uint16_t short_address);
bool thread_check_is_this_my_parent(struct protocol_interface_info_entry *cur, struct mle_neigh_table_entry_t *entry_temp);
void thread_clean_old_16_bit_address_based_addresses(struct protocol_interface_info_entry *cur);
int8_t thread_bootsrap_event_trig(thread_bootsrap_event_type_e event_type, int8_t Id, arm_library_event_priority_e priority);
void thread_interface_init(struct protocol_interface_info_entry *cur);

/*
 * Thread bootstrap layer configurations
 */
int thread_configuration_mac_activate(protocol_interface_info_entry_t *cur, uint16_t channel, uint16_t panid, uint8_t *extended_random_mac);
int thread_configuration_6lowpan_activate(protocol_interface_info_entry_t *cur );
int thread_configuration_mle_activate(protocol_interface_info_entry_t *cur);
int thread_configuration_mle_disable(protocol_interface_info_entry_t *cur);
int thread_configuration_thread_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration );
int thread_link_configuration_activate(struct protocol_interface_info_entry *cur, struct link_configuration *linkConfiguration);
int thread_parent_discover_start(int8_t interface_id, uint8_t *mac64 );

bool thread_device_synch_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries);
bool thread_link_request_timeout(int8_t interface_id, uint16_t msgId, bool usedAllRetries);
bool thread_instance_id_matches(struct protocol_interface_info_entry *cur, struct thread_leader_data_s *leaderData);
int thread_leader_data_validation(struct protocol_interface_info_entry *cur, struct thread_leader_data_s *leaderData, struct mle_tlv_info_s *routeTlv);
uint8_t thread_calculate_link_margin(int8_t dbm, uint8_t compLinkMarginFromParent);
uint8_t thread_compute_link_margin(int8_t rssi);
uint8_t thread_parent_margin_calc(uint8_t marginFromParent, uint8_t marginToParent);
void thread_bootstrap_parent_network_data_request(struct protocol_interface_info_entry *cur, bool delay_request);
void thread_generate_ml64_address(struct protocol_interface_info_entry *cur);
void thread_delete_ml64_address(struct protocol_interface_info_entry *cur);
void thread_delete_ml16_addresses(protocol_interface_info_entry_t *cur);
void thread_generate_ml16_address(struct protocol_interface_info_entry *cur);
int thread_bootstrap_tasklet_init(struct protocol_interface_info_entry *cur);
void thread_bootstrap_stop(struct protocol_interface_info_entry *cur);
void thread_interface_up(struct protocol_interface_info_entry *cur);
void thread_bootstrap_state_machine(struct protocol_interface_info_entry *cur);
void thread_bootsrap_device_synch_fail(struct protocol_interface_info_entry *cur);
void thread_bootstrap_attached_downgrade_router(struct protocol_interface_info_entry *cur);
void thread_bootstrap_attched_upgrade_reed(struct protocol_interface_info_entry *cur);
void thread_bootstrap_attached_ready(struct protocol_interface_info_entry *cur);
void thread_bootstrap_router_synch_ready(protocol_interface_info_entry_t *cur);
void thread_bootstrap_attached_active_router(struct protocol_interface_info_entry *cur);
void thread_bootstrap_router_id_get_fail(struct protocol_interface_info_entry *cur);
void thread_bootstrap_router_id_release_ready(struct protocol_interface_info_entry *cur);
int8_t thread_bootstrap_child_id_request(struct protocol_interface_info_entry *cur);
void thread_bootstrap_child_update_trig(struct protocol_interface_info_entry *cur);
void thread_bootstrap_connection_error(int8_t interface_id, nwk_connect_error_types errorType, uint8_t *LinkId);
int8_t nwk_thread_host_control(struct protocol_interface_info_entry *cur, enum net_host_mode mode, uint32_t delay);
void thread_bootstrap_routing_activate(protocol_interface_info_entry_t *cur);
void thread_end_device_mode_set(protocol_interface_info_entry_t *cur, bool sleepy);
int8_t thread_network_data_save_after_sync(protocol_interface_info_entry_t *cur, uint8_t address[16], thread_leader_data_t *leader_data, uint8_t *commission_tlv_ptr, uint16_t commission_tlv_len);
int8_t thread_active_configuration_dataset_query_done(protocol_interface_info_entry_t *cur);
int8_t thread_active_configuration_dataset_query_clean(protocol_interface_info_entry_t *cur);
void thread_bootstrap_mac_activate(protocol_interface_info_entry_t *cur, uint16_t channel, uint16_t panid, bool coordinator);
int thread_bootstrap_network_data_process(protocol_interface_info_entry_t *cur, uint8_t *network_data_ptr, uint16_t network_data_length);
int thread_bootstrap_network_data_activate(protocol_interface_info_entry_t *cur);
int thread_bootstrap_network_data_save(struct protocol_interface_info_entry *cur, thread_leader_data_t *leader_data, uint8_t* network_data_ptr, uint16_t network_data_len);
void thread_bootstrap_network_prefixes_process(struct protocol_interface_info_entry *cur);
void thread_bootstrap_network_data_update(protocol_interface_info_entry_t *cur);
bool thread_bootstrap_link_create_check(protocol_interface_info_entry_t *interface, uint16_t short_address);
bool thread_bootstrap_link_create_allowed(protocol_interface_info_entry_t *interface, uint16_t short_address, const uint8_t *src_addr);
bool thread_bootstrap_should_register_address(protocol_interface_info_entry_t *cur);
bool thread_tlv_request(int8_t interface_id, uint8_t *address, bool delayed_message, uint8_t *req_tlv, uint8_t req_len);
void thread_bootstrap_clear_neighbor_entries(protocol_interface_info_entry_t *cur);
void thread_bootstrap_dynamic_configuration_save(protocol_interface_info_entry_t *cur);
void thread_bootstrap_update_ml16_address(protocol_interface_info_entry_t *cur, uint16_t mac16);
void thread_bootstrap_pending_configuration_save(protocol_interface_info_entry_t *cur);
void thread_bootstrap_all_nodes_address_generate(uint8_t multicast_address[16],uint8_t prefix[8], uint8_t scope);
/**
 * Check advertisement or parent response is from a singleton partition and decide on accepting or dropping the packet
 *
 * \param cur protocol_interface_info_entry_t
 * \param heard_partition_routers number of routers in heard advertisement or parent response
 * \param heard_partition_leader_data leader data of the partition heard
 * \param routeTlv route TLV of heard advertisement or NULL if there the comparison is between parent responses
 *
 * return  2  merge has to be triggered based on the singleton partition rules or parent response should be accepted
 * return -2  no merge needs to be triggered or parent response can be dropped
 */

int thread_bootstrap_partition_process(protocol_interface_info_entry_t *cur,uint8_t heard_partition_routers,thread_leader_data_t *heard_partition_leader_data, mle_tlv_info_t *routeTlv);

/*
 * Thread announcement control functions
 */
int thread_bootstrap_announce_send(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp, uint16_t active_channel);
void thread_bootstrap_announcement_start(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint8_t count, uint16_t period);
void thread_bootstrap_temporary_attach(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp);


#else
#define thread_interface_up(cur) ((void) 0)
#define thread_bootstrap_state_machine(cur) ((void)0)
#define thread_bootstrap_child_update_trig(cur) ((void)0)
#define thread_bootstrap_connection_error(interface_id, errorType, LinkId) ((void)0)
#define thread_bootstrap_network_data_update(cur,force_stable_update, force_unstable_update) ((void)0)
#define thread_bootstrap_network_data_changed(cur,force_stable_update,force_unstable_update) ((void)0)
#define thread_bootstrap_dynamic_configuration_save(cur) ((void)0)

#endif // HAVE_THREAD

#endif /* THREAD_BOOTSTRAP_H_ */
