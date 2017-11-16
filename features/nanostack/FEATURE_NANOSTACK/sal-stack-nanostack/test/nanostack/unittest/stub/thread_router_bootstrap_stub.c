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

/*
 * \file thread_router_bootstrap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "shalib.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_host_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_synch.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_address_registration_client.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_beacon.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_lowpower_private_api.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "thread_management_if.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "thread_config.h"
#include "multicast_api.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "thread_dhcpv6_client.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "thread_border_router_api.h"
#include "Core/include/address.h"

int thread_router_bootstrap_mle_advertise(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_multicast_forwarder_enable(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
}

void thread_router_bootstrap_child_information_clear(protocol_interface_info_entry_t *cur)
{
}
void thread_router_bootstrap_router_id_request(protocol_interface_info_entry_t *cur, uint8_t status)
{
}
uint16_t thread_router_bootstrap_child_count_get(protocol_interface_info_entry_t *cur)
{
}
void thread_router_bootstrap_child_id_handler(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_mle_receive_cb(int8_t interface_id, mle_message_t *mle_msg, mle_security_header_t *security_headers)
{
}

int thread_router_bootstrap_link_synch_start(protocol_interface_info_entry_t *cur)
{
}

bool thread_router_bootstrap_router_downgrade(protocol_interface_info_entry_t *cur)
{
    return true;
}

bool thread_router_bootstrap_reed_upgrade(protocol_interface_info_entry_t *cur)
{
    return true;
}

void thread_router_bootstrap_child_id_reject(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_active_router_attach(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_routing_activate(protocol_interface_info_entry_t *cur)
{
}
int thread_router_bootstrap_route_tlv_push(protocol_interface_info_entry_t *cur, uint8_t *route_tlv, uint8_t route_len, uint8_t linkMargin, mle_neigh_table_entry_t *entry)
{
}

void thread_router_bootstrap_reed_advertisements_start(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_router_id_release(protocol_interface_info_entry_t *cur)
{
}

void thread_router_bootstrap_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
}

void thread_router_bootstrap_advertiment_analyze(protocol_interface_info_entry_t *cur, uint8_t *src_address, mle_neigh_table_entry_t *entry_temp, uint16_t shortAddress)
{
}
void thread_router_bootstrap_anycast_address_register(protocol_interface_info_entry_t *cur)
{
}

bool thread_router_bootstrap_routing_allowed(struct protocol_interface_info_entry *cur)
{
    return true;
}
void thread_router_bootstrap_network_data_distribute(protocol_interface_info_entry_t *cur)
{
}
void thread_router_bootstrap_address_change_notify_send(protocol_interface_info_entry_t *cur)
{
}

