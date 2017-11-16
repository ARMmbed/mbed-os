/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

/*
 * \file thread_bootstrap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "common_functions.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "dhcp_service_api.h"
#include "thread_dhcpv6_client.h"


#define TRACE_GROUP "thbs"

void threa_ula_address_generate_and_push_to_stack(protocol_interface_info_entry_t *cur);
int thread_bootstrap_reset(protocol_interface_info_entry_t *cur);
static void  thread_mle_challenge_trig(int8_t interface_id, uint8_t *LinkId);

void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur);

int8_t nwk_thread_host_control(protocol_interface_info_entry_t *cur, net_host_mode_t mode, uint32_t delay)
{
    return 0;
}

int thread_link_configuration_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    return 0;
}

void thread_interface_init(protocol_interface_info_entry_t *cur)
{
}

void thread_tasklet(arm_event_s *event)
{
}

int thread_bootstrap_tasklet_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_clean_old_16_bit_address_based_addresses(protocol_interface_info_entry_t *cur)
{
}


void thread_bootstrap_ready(protocol_interface_info_entry_t *cur)
{
}

int thread_leader_configure_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_bootstrap_generate_leader_and_link(int8_t interface_id)
{
}

void thread_bootstrap_connection_error(int8_t interface_id, nwk_connect_error_types errorType, uint8_t *LinkId)
{
}

void thread_bootsrap_router_id_cb(int8_t interface_id, uint8_t *ml16_address, uint8_t router_id,  const uint8_t *router_mask_ptr)
{
}

void thread_interface_up(protocol_interface_info_entry_t *cur)
{
}

int thread_bootstrap_reset(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_network_attach_start(protocol_interface_info_entry_t *cur)
{
}

void threa_ula_address_generate_and_push_to_stack(protocol_interface_info_entry_t *cur)
{
}

void thread_generate_domain_address(protocol_interface_info_entry_t *cur)
{
}
void thread_bootstrap_attached_ready(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attached_downgrade_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attched_upgrade_reed(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_attched_active_router(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_router_id_release_ready(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_router_id_get_fail(protocol_interface_info_entry_t *cur)
{
}

int8_t thread_bootstrap_child_id_request(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_bootstrap_attached_finish(protocol_interface_info_entry_t *cur)
{
}

void thread_parent_scan(protocol_interface_info_entry_t *cur)
{
}

int thread_set_link_for_commission(protocol_interface_info_entry_t *interface, nwk_pan_descriptor_t *best)
{
    return 0;
}

void thread_mac_active_scan_response(uint8_t *data)
{
}

void thread_bootstrap_state_machine(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_stop(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_address_register(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_child_update_trig(protocol_interface_info_entry_t *cur)
{
}

void thread_bootstrap_global_leader_data(thread_info_t *thread_info)
{

}

bool thread_tlv_request(int8_t interface_id, uint8_t *address, bool delayed_message, uint8_t *req_tlv, uint8_t req_len)
{
    return false;
}

void thread_bootstrap_reset_restart(int8_t interface)
{
}

int thread_bootstrap_announce_send(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp, uint16_t selected_channel)
{
    return 0;
}
void thread_bootstrap_announcement_start(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint8_t count, uint16_t period)
{
    return;
}
void thread_bootstrap_temporary_attach(protocol_interface_info_entry_t *cur, uint8_t channel_page, uint16_t channel, uint16_t panid, uint64_t timestamp)
{
    return;
}
int8_t thread_active_configuration_dataset_query_clean(protocol_interface_info_entry_t *cur)
{

    return 0;
}
int8_t thread_active_configuration_dataset_query_done(protocol_interface_info_entry_t *cur)
{
    return 0;
}

void thread_bootstrap_network_data_changed(protocol_interface_info_entry_t *cur, bool force_stable_update, bool force_unstable_update)
{
    return;
}
void thread_bootstrap_network_data_update(protocol_interface_info_entry_t *cur)
{
    return;
}
void thread_bootstrap_dynamic_configuration_save(protocol_interface_info_entry_t *cur)
{
    return;
}
void thread_bootstrap_update_ml16_address(protocol_interface_info_entry_t *cur, uint16_t mac16)
{
    return;
}
void thread_bootstrap_clear_neighbor_entries(protocol_interface_info_entry_t *cur)
{
    return;
}
bool thread_bootstrap_link_create_check(protocol_interface_info_entry_t *interface, uint16_t short_address)
{
    return false;
}

int8_t thread_mle_class_init(int8_t interface_id)
{
    return 0;
}

void thread_clean_all_routers_from_neighbor_list(int8_t interface_id)
{

}

void thread_set_link_local_address(protocol_interface_info_entry_t *cur)
{

}

void thread_bootstrap_attached_active_router(protocol_interface_info_entry_t *cur)
{

}

void thread_bootstrap_pending_configuration_save(protocol_interface_info_entry_t *cur)
{

}

bool thread_check_is_this_my_parent(protocol_interface_info_entry_t *cur, mle_neigh_table_entry_t *entry_temp)
{
    return false;
}

int thread_bootstrap_reset_child_info(protocol_interface_info_entry_t *cur, mle_neigh_table_entry_t *child)
{
    return 0;
}

bool thread_device_register_address(protocol_interface_info_entry_t *cur)
{
    return false;
}

void thread_bootstrap_all_nodes_address_generate(uint8_t multicast_address[16],uint8_t prefix[8], uint8_t scope)
{
}

int thread_configuration_thread_activate(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    return 0;
}

bool thread_bootstrap_should_register_address(protocol_interface_info_entry_t *cur)
{
    return false;
}

int thread_bootstrap_network_data_process(protocol_interface_info_entry_t *cur, uint8_t *network_data_ptr, uint16_t network_data_length)
{
    return 0;
}
