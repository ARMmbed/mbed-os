/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#ifndef NO_MLE
#include "MLE/mle.h"
//#include "MLE/mle_challenge_data.h"
#endif
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread_common.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#ifndef NO_TCP
#ifndef NO_TLS
#include "ccmLIB.h"
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#endif
#include "Common_Protocols/tcp.h"
#endif
#include "randLIB.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "common_functions.h"

#include "protocol_6lowpan_stub.h"

#define TRACE_GROUP_LOWPAN "6lo"
#ifdef NO_TCP
buffer_t *tcp_up(buffer_t *buf);
buffer_t *tcp_down(buffer_t *buf);

buffer_t *tcp_up(buffer_t *buf)
{
    return buffer_free(buf);
}
buffer_t *tcp_down(buffer_t *buf)
{
    return buffer_free(buf);
}
#endif

#ifdef NO_TLS
#define sec_timer_handle()
#endif

#ifdef HAVE_RPL
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"

rpl_domain_t *protocol_6lowpan_rpl_domain;
/* Having to sidestep old rpl_dodag_t type for the moment */
struct rpl_dodag *protocol_6lowpan_rpl_root_dodag;
#endif

protocol_6lowpan_stub_def protocol_6lowpan_stub;

void protocol_init(void)
{
}

void protocol_6lowpan_stack(buffer_t *b)
{
}

void protocol_6lowpan_host_init(protocol_interface_info_entry_t *cur, bool sleepy_host)
{

}

void protocol_6lowpan_router_init(protocol_interface_info_entry_t *cur)
{

}

void protocol_6lowpan_configure_core(protocol_interface_info_entry_t *cur)
{

}

void protocol_6lowpan_register_handlers(protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_release_short_link_address_from_neighcache(protocol_interface_info_entry_t *cur, uint16_t shortAddress)
{
}

void protocol_6lowpan_release_long_link_address_from_neighcache(protocol_interface_info_entry_t *cur, uint8_t *mac64)
{
}

void protocol_6lowpan_neighbor_priority_update(protocol_interface_info_entry_t *cur, uint8_t *removed_priority, uint8_t *updated_priority)
{
}

int8_t protocol_6lowpan_neighbor_remove(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, addrtype_t type)
{
    return 0;
}

void protocol_6lowpan_allocate_mac16(protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_interface_common_init(protocol_interface_info_entry_t *cur)
{
}

int8_t protocol_6lowpan_interface_compare_cordinator_netid(protocol_interface_info_entry_t *cur, uint8_t *adr_ptr)
{
    return 0;
}

int8_t protocol_6lowpan_interface_get_link_local_cordinator_address(protocol_interface_info_entry_t *cur, uint8_t *adr_ptr)
{
    return 0;
}

int16_t protocol_6lowpan_rpl_global_priority_get(void)
{
    return protocol_6lowpan_stub.uint8_value;
}

uint8_t protocol_6lowpan_beacon_join_priority_tx(int8_t interface_id)
{
    return protocol_6lowpan_stub.uint8_value;
}

uint8_t protocol_6lowpan_beacon_compare_rx(int8_t interface_id, uint8_t join_priority, uint8_t link_quality)
{
    return protocol_6lowpan_stub.uint8_value;
}

int8_t protocol_6lowpan_neighbor_address_state_synch(protocol_interface_info_entry_t *cur, const uint8_t eui64[8], const uint8_t iid[8])
{
    return 0;
}

uint16_t protocol_6lowpan_neighbor_priority_set(int8_t interface_id, addrtype_t addr_type, const uint8_t *addr_ptr)
{
    return 0;
}

uint16_t protocol_6lowpan_neighbor_second_priority_set(int8_t interface_id, addrtype_t addr_type, const uint8_t *addr_ptr)
{
    return 0;
}

void protocol_6lowpan_neighbor_priority_clear_all(int8_t interface_id, neighbor_priority priority)
{

}

int8_t protocol_6lowpan_interface_get_mac_coordinator_address(protocol_interface_info_entry_t *cur, sockaddr_t *adr_ptr)
{
    return 0;
}
