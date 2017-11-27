/*
 * Copyright (c) 2012-2017, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#ifndef NO_TLS
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#endif

#include "6LoWPAN/ND/nd_router_object.h" // for gp_address_ functions - better place?
#include "ipv6_stack/ipv6_routing_table.h"
#include "ipv6_stack/protocol_ipv6.h"
#ifndef NO_TCP
#include "Common_Protocols/tcp.h"
#endif
#include "Service_Libs/whiteboard/whiteboard.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "randLIB.h"

typedef struct ipv6_interface_prefix_on_link_t {
    uint8_t                             prefix[16];                 /*!< destination address */
    uint8_t                             prefix_len;
    uint32_t                            prefix_valid_ttl;
    ns_list_link_t                      link;
} ipv6_interface_prefix_on_link_t;

typedef struct ipv6_interface_route_on_link_t {
    uint8_t                             prefix[16];                 /*!< destination address */
    uint8_t                             prefix_len;
    uint8_t                             routePrefer;
    uint32_t                            prefix_valid_ttl;
    ns_list_link_t                      link;
} ipv6_interface_route_on_link_t;

#define WB_UPDATE_PERIOD_SECONDS            23

#define ROUTER_SOL_MAX_COUNTER              4

#define TRACE_GROUP_PROTOCOL_IPv6  "ip6s"

void ipv6_interface_phy_sap_register(protocol_interface_info_entry_t *cur)
{

}

int ipv6_generate_static_gp_setup(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int ipv6_generate_ula_prefix(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int ipv6_prefix_router_flag_activate(uint8_t *ipv6_address)
{
    return -1;
}

buffer_t *ethernet_header_build_push(buffer_t *buf, uint16_t ethernet_type, int8_t *status)
{
    return NULL;
}

buffer_t *ethernet_down(buffer_t *buf)
{
    return NULL;
}

void ipv6_stack_prefix_on_link_update(protocol_interface_info_entry_t *cur, uint8_t *address)
{
}

void ipv6_stack_route_advert_update(uint8_t *address, uint8_t prefixLength, uint8_t routePrefer)
{
}

void ipv6_prefix_on_link_update(uint8_t *address)
{

}

void ipv6_prefix_on_link_remove(uint8_t *address)
{

}

void ipv6_stack_route_advert_remove(uint8_t *address, uint8_t prefixLength)
{
}

void ipv6_prefix_online_list_free(void)
{
}

void ipv6_rote_advert_list_free(void)
{
}

void ipv6_core_slow_timer_event_handle(struct protocol_interface_info_entry *cur)
{
}

void ipv6_core_timer_event_handle(protocol_interface_info_entry_t *cur, const uint8_t event)
{
}

int ipv6_prefix_register(uint8_t *prefix_64, uint32_t lifetime, uint32_t prefer_lifetime)
{
    return -1;
}


int8_t ipv6_interface_up(protocol_interface_info_entry_t *cur)
{
    return -1;
}

int8_t ipv6_interface_down(protocol_interface_info_entry_t *cur)
{
    return -1;
}


#ifdef RADV_TX
void ipv6_nd_ra_advert(protocol_interface_info_entry_t *cur, const uint8_t *dest)
{
}
#endif



void ipv6_trigger_resolve_query(protocol_interface_info_entry_t *cur_interface, buffer_t *buf, ipv6_neighbour_t *n)
{
}

void ipv6_interface_slaac_handler(protocol_interface_info_entry_t *cur, uint8_t *slaacPrefix, uint8_t prefixLen, uint32_t validLifeTime, uint32_t preferredLifeTime)
{
}

void ipv6_interface_sitelocal_clone(buffer_t *buf)
{

}

int8_t ipv6_interface_configure_ipv6_bootstrap_set(int8_t interface_id, net_ipv6_mode_e bootstrap_mode, const uint8_t *ipv6_prefix_pointer)
{
    return 0;
}

int8_t ipv6_interface_accept_ra(int8_t interface_id, net_ipv6_accept_ra_e accept_ra)
{
    return 0;
}
