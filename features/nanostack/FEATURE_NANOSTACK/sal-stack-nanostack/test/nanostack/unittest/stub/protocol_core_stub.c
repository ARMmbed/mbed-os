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

#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "platform/arm_hal_interrupt.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "common_functions.h"
#include "randLIB.h"
#include "platform/arm_hal_phy.h"
#include "platform/arm_hal_interrupt.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#ifndef NO_TLS
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "net_pana_parameters_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#else
#define sec_timer_handle()
#endif
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "libDHCPv6/libDHCPv6.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#ifdef IPV6_STACK
#include "ipv6_stack/protocol_ipv6.h"
#endif
#include "Service_Libs/whiteboard/whiteboard.h"

#include "sw_mac.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "mac_api.h"
#include "ethernet_mac_api.h"

#include "protocol_core_stub.h"

uint32_t protocol_core_monotonic_time;

protocol_interface_info_entry_t *protocol_core_multicast_upstream;

protocol_interface_list_t NS_LIST_NAME_INIT(protocol_interface_info_list);

protocol_core_stub_def protocol_core_stub;

int protocol_core_buffers_in_event_queue;

int8_t protocol_read_tasklet_id(void)
{
    return protocol_core_stub.int8_value;
}

uint8_t check_power_state(uint8_t mode)
{
    return protocol_core_stub.uint8_value;
}


void set_power_state(uint8_t mode)
{
}

void clear_power_state(uint8_t mode)
{
}

void arm_net_protocol_packet_handler(buffer_t *buf, protocol_interface_info_entry_t *cur_interface)
{

}

void protocol_root_tasklet(arm_event_s *event)
{
}

void protocol_core_security_tick_update(uint16_t tick_update)
{
}

void nwk_bootsrap_timer(protocol_interface_info_entry_t *cur)
{
}

void core_timer_event_handle(uint16_t ticksUpdate)
{
}

void protocol_core_cb(uint16_t ticksUpdate)
{
}


void protocol_core_init(void)
{
}

void protocol_core_interface_info_reset(protocol_interface_info_entry_t *entry)
{
}

void bootsrap_next_state_kick(icmp_state_t new_state, protocol_interface_info_entry_t *cur)
{
}

uint32_t protocol_stack_interface_set_reachable_time(protocol_interface_info_entry_t *cur, uint32_t base_reachable_time)
{
    return protocol_core_stub.uint32_value;
}

protocol_interface_info_entry_t *nwk_interface_get_ipv6_ptr(void)
{
    return protocol_core_stub.entry_ptr;
}

void nwk_interface_print_neigh_cache(route_print_fn_t *print_fn)
{
}

void nwk_interface_flush_neigh_cache(void)
{
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id)
{
    return protocol_core_stub.entry_ptr;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_id(int8_t nwk_id)
{
    return protocol_core_stub.entry_ptr;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_bootstrap_id(int8_t id)
{
    return protocol_core_stub.entry_ptr;
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get_by_rpl_domain(const struct rpl_domain *domain, int8_t last_id)
{
    return protocol_core_stub.entry_ptr;
}

protocol_interface_info_entry_t *protocol_stack_interface_sleep_possibility(void)
{
    return protocol_core_stub.entry_ptr;
}

uint8_t nwk_bootsrap_ready(protocol_interface_info_entry_t *cur)
{
    return protocol_core_stub.uint8_value;
}

int8_t nwk_interface_MAC_MTU_allocate(protocol_interface_info_entry_t *cur)
{
    return protocol_core_stub.int8_value;
}

protocol_interface_info_entry_t *protocol_stack_interface_generate_ethernet(eth_mac_api_t *api)
{
    return protocol_core_stub.entry_ptr;
}

protocol_interface_info_entry_t *protocol_stack_interface_generate_lowpan(mac_api_t *api)
{
    return protocol_core_stub.entry_ptr;
}

bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t *addr)
{
    return protocol_core_stub.bool_value;
}

int nwk_lowpan_routing_check(buffer_t *buf)
{
    return protocol_core_stub.int_value;
}

void protocol_push(buffer_t *buf)
{
}

void nwk_bootsrap_state_update(arm_nwk_interface_status_type_e posted_event, protocol_interface_info_entry_t *cur)
{
}

void net_bootsrap_cb_run(uint8_t event)
{
}

void protocol_core_dhcpv6_allocated_address_remove(protocol_interface_info_entry_t *cur, uint8_t *guaPrefix)
{
}

int8_t protcol_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    return protocol_core_stub.int8_value;
}

