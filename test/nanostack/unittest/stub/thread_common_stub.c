/*
 * Copyright (c) 2014-2015, 2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "randLIB.h"
#include "common_functions.h"

#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_interface.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_dhcpv6_client.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_common_stub.h"

thread_stub_def thread_stub;
uint32_t thread_router_selection_jitter;

uint16_t thread_log2_aprx(uint32_t n)
{
    return 0;
}

int thread_proxy_state_update(int8_t caller_interface_id , int8_t handler_interface_id, bool status)
{
    return thread_stub.int_value;
}

void thread_anycast_address_policy_update(const thread_info_t *thread_info, bool addPolicy)
{
}
int8_t thread_bootstrap_up(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int8_value;
}

int8_t thread_bootstrap_down(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int8_value;
}

int thread_leader_data_base_allocate(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int_value;
}

bool thread_addr_is_mesh_local(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return thread_stub.bool_value;
}

bool thread_addr_is_mesh_local_16(const uint8_t *addr, const protocol_interface_info_entry_t *cur)
{
    return thread_stub.bool_value;
}

uint8_t *thread_addr_write_mesh_local_16(uint8_t ip_addr_out[16], uint16_t addr16, const thread_info_t *info)
{
    return NULL;
}
uint8_t *thread_route_option_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}

uint16_t thread_active_operational_dataset_size(protocol_interface_info_entry_t *cur)
{
    return 0;
}
uint8_t *thread_active_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}
uint8_t *thread_network_data_tlv_write(protocol_interface_info_entry_t *cur, uint8_t *ptr, bool fulllist)
{
    return ptr;
}

uint8_t *thread_active_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}
uint8_t *thread_address_registration_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    return ptr;
}

bool thread_active_operational_dataset_process(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t len, uint64_t dataset_timestamp)
{
    return false;
}
uint8_t *thread_pending_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}
uint16_t thread_pending_operational_dataset_size(protocol_interface_info_entry_t *cur)
{
    return 0;
}
uint8_t *thread_pending_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr)
{
    return 0;
}
bool thread_pending_operational_dataset_process(protocol_interface_info_entry_t *cur, uint64_t mle_pending_timestamp, uint8_t *ptr, uint16_t len)
{
    return 0;
}

bool thread_leader_data_parse(uint8_t *ptr, uint16_t dataLength, thread_leader_data_t *leaderDataBuf)
{
    return thread_stub.bool_value;
}

bool thread_connectivity_tlv_parse(uint8_t *ptr, uint16_t dataLength, thread_connectivity_t *connectivityTlv)
{
    return thread_stub.bool_value;
}

int thread_leader_ul16_address_get(thread_info_t *thread_info, uint8_t *address_ptr)
{
    return thread_stub.int_value;
}

bool thread_leader_commissioner_create(thread_info_t *thread_info)
{
    return thread_stub.bool_value;
}

void thread_leader_commissioner_free(thread_info_t *thread_info)
{
}

void thread_data_base_init(thread_info_t *thread_info, int8_t interfaceId)
{
}


int thread_info_allocate_and_init(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int_value;
}

thread_link_quality_e thread_link_margin_to_quality(thread_link_margin_t margin)
{
    return QUALITY_BAD;
}

uint_fast8_t thread_sum_rx_path_cost_and_link_cost(uint8_t inMargim, uint8_t outMargin, uint8_t pathCost)
{
    return 0;
}

void thread_info_deallocate(protocol_interface_info_entry_t *cur)
{
}

int thread_leader_route_mask_update(thread_leader_info_t *info, uint8_t router_id, bool setActive)
{
    return thread_stub.int_value;
}

/* Called by thread_routing.c as a result of updates to routing table - allows
 * leader to monitor a router being available (having a finite route cost).
 */
void thread_leader_router_state_changed(thread_leader_info_t *info, uint8_t router_id, bool available, int8_t interface_id)
{
}

void thread_leader_allocate_router_id_by_allocated_id(thread_leader_info_t *info, uint8_t router_id, const uint8_t *eui64)
{
}

int thread_generate_leasequery(int8_t interfaceID, uint8_t *addressRequested)
{
    return thread_stub.bool_value;
}


void thread_child_id_request_info_init(thread_pending_child_id_req_t *child_info)
{
}

thread_pending_child_id_req_t *thread_child_id_request_allocate(void)
{
    return thread_stub.req_ptr;
}

void thread_child_id_request_entry_clean(protocol_interface_info_entry_t *cur)
{
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get(protocol_interface_info_entry_t *cur, uint8_t *euid64)
{
    return thread_stub.req_ptr;
}

thread_pending_child_id_req_t *thread_child_id_request_entry_get_from_the_list(protocol_interface_info_entry_t *cur)
{
    return thread_stub.req_ptr;
}

void thread_child_id_request_entry_remove(protocol_interface_info_entry_t *cur, thread_pending_child_id_req_t *entry)
{
}

int thread_init(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int_value;
}

int thread_attach_ready(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int_value;
}

bool thread_attach_active_router(protocol_interface_info_entry_t *cur)
{
    return thread_stub.bool_value;
}

bool thread_scan_mask_validation(protocol_interface_info_entry_t *cur, uint8_t mask)
{
    return thread_stub.bool_value;
}

int thread_route_ready_to_leader(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int_value;
}

thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info)
{
    return thread_stub.info_ptr;
}

void thread_dhcp_client_gua_error_cb(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status)
{
}


void thread_network_data_service_learn(protocol_interface_info_entry_t *cur)
{
}

void thread_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
}

void thread_timer(protocol_interface_info_entry_t *cur, uint8_t ticks)
{
}

void thread_network_data_changed(protocol_interface_info_entry_t *cur, bool force_stable_update, bool force_unstable_update)
{
}

void thread_network_data_update(protocol_interface_info_entry_t *cur, bool force_stable_update, bool force_unstable_update)
{
}

bool thread_insist_that_mesh_isnt_a_link(const protocol_interface_info_entry_t *cur)
{
    return thread_stub.bool_value;
}

/* If it's a 16-bit src address, only pass it if it's on the list */
bool thread_test_addr_filter(const buffer_t *buf, const protocol_interface_info_entry_t *cur)
{
    return thread_stub.bool_value;
}

int8_t thread_beacon_create_payload(protocol_interface_info_entry_t *cur)
{
    return thread_stub.int8_value;
}

uint8_t thread_beacon_indication(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur)
{
    return thread_stub.uint8_value;
}

void thread_network_data_request_send(protocol_interface_info_entry_t *cur, uint8_t *requestDstAddress, bool delaydTrig)
{

}

void thread_calculate_key_guard_timer(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, bool is_init)
{
    return;
}

void thread_network_data_request_disable_trig(protocol_interface_info_entry_t *cur)
{

}

uint8_t *thread_leader_data_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur)
{
    return thread_stub.uint8_ptr;
}

int8_t thread_network_data_sender_verify(uint8_t source_address[16], thread_commissioner_t *registered_commissioner, link_configuration_s *link_configuration, uint8_t *data_ptr, uint16_t data_len)
{
    return thread_stub.int8_value;
}

void thread_network_data_request_trig(protocol_interface_info_entry_t *cur, uint8_t *requestDstAddress, bool delaydTrig)
{

}

int thread_network_data_propagation(protocol_interface_info_entry_t *cur, uint8_t *childUnicastAddress, bool fullList)
{
    return thread_stub.int_value;
}

bool thread_on_mesh_route_possible_add(thread_attach_device_mode_e threadMode)
{
    return thread_stub.bool_value;
}

uint8_t thread_leader_data_tlv_size(protocol_interface_info_entry_t *cur)
{
    return 0;
}

int thread_link_reject_send(protocol_interface_info_entry_t *interface, const uint8_t *ll64)
{
    return 0;
}

uint8_t thread_get_router_count_from_route_tlv(mle_tlv_info_t *routeTlv)
{
    return 0;
}

uint8_t *thread_management_key_request_with_sequence(int8_t interface_id, uint8_t keyId, uint32_t keySequnce)
{
    return NULL;
}

void thread_mcast_address_change(struct protocol_interface_info_entry *interface, struct addr_multicast_t *addr, bool addr_added)
{
}

void thread_child_mcast_entries_remove(protocol_interface_info_entry_t *cur, const uint8_t *mac64)
{
}

void thread_registered_mcast_addr_entry_clean(protocol_interface_info_entry_t *cur)
{
}
void thread_address_registration_primary_bbr_process(struct protocol_interface_info_entry *cur)
{
}
void thread_address_registration_primary_bbr_update_done(struct protocol_interface_info_entry *cur)
{
}
int thread_address_registration_primary_bbr_update_needed(struct protocol_interface_info_entry *cur, uint16_t rloc, uint8_t seq)
{
    return 0;
}
int thread_address_registration_primary_bbr_get(struct protocol_interface_info_entry *cur, uint8_t *addr_ptr, uint8_t *seq_ptr, uint32_t *mlr_timer_ptr, uint32_t *delay_timer_ptr)
{
    return 0;
}
