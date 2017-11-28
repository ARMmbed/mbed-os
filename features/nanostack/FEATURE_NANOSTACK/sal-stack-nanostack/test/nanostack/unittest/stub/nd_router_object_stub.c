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
#include "ns_types.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "randLIB.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "common_functions.h"

#define TRACE_GROUP_LOWPAN_ND "loND"

void icmp_nd_router_object_release(nd_router_t *router_object);
uint8_t icmp_nd_router_prefix_ttl_update(nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface);

uint8_t nd_rs_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface);
bool icmp_nd_compare_to_def_next_hop(nd_router_next_hop *hop, sockaddr_t *adr);
void icmp_nd_router_context_ttl_update(nd_router_t *nd_router_object);
extern void nd_border_router_setup_refresh(nwk_interface_id id, bool fresh_abro);

nd_parameters_s nd_params;

uint8_t nd_base_tick = 1;

int8_t nd_set_br(nd_router_t *br)
{
    return -1;
}

bool nd_object_active(void)
{
    return false;
}

void icmp_nd_routers_init(void)
{
}


void icmp_nd_set_nd_def_router_address(uint8_t *ptr, nd_router_t *cur)
{

}

void nd_ns_trig(nd_router_t *router_object, protocol_interface_info_entry_t *cur)
{

}

void nd_router_base_init(nd_router_t *new_entry)
{

}

nd_router_t *icmp_nd_router_object_get(const uint8_t *border_router, nwk_interface_id nwk_id)
{
    return NULL;
}

void icmp_nd_router_object_reset(nd_router_t *router_object)
{

}

int8_t icmp_nd_router_prefix_proxy_update(uint8_t *dptr, nd_router_setup_t *nd_router_object)
{
    return -1;
}

uint8_t icmp_nd_router_prefix_valid(nd_router_t *nd_router_object)
{
    return 0;
}

uint8_t icmp_nd_router_prefix_ttl_update(nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface)
{
    return 0;
}

int8_t icmp_nd_router_prefix_update(uint8_t *dptr, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface)
{
    return 0;
}

void icmp_nd_router_context_ttl_update(nd_router_t *nd_router_object)
{

}

int8_t icmp_nd_router_context_update(const uint8_t *dptr, nd_router_t *router_object)
{
    return -1;
}

void icmp_nd_router_object_release(nd_router_t *router_object)
{
}


void icmp_nd_border_router_release(nd_router_t *router_object)
{
}



void gp_address_list_free(gp_ipv6_address_list_t *list)
{
}

void icmp_nd_prefixs_parse(buffer_t *buf, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface)
{

}


void icmp_nd_set_next_hop(nd_router_next_hop *hop, sockaddr_t *adr)
{

}

bool icmp_nd_compare_to_def_next_hop(nd_router_next_hop *hop, sockaddr_t *adr)
{
    return false;
}

uint8_t nd_rs_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface)
{
    return 0;
}

void nd_ns_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint8_t *address_ptr)
{
}

buffer_t *nd_dar_parse(buffer_t *buf, protocol_interface_info_entry_t *cur_interface)
{
    return NULL;
}

bool nd_ns_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *aro_opt, const uint8_t *slla_opt, const uint8_t *src_addr, aro_t *aro_out)
{
    return false;
}

buffer_t *nd_dac_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

static void nd_ns_forward_timer_reset(uint8_t *root_adr)
{

}

bool nd_ra_process_abro(protocol_interface_info_entry_t *cur, buffer_t *buf, const uint8_t *dptr, uint8_t ra_flags, uint16_t router_lifetime)
{
    return false;
}

void nd_ra_process_lowpan_context_option(protocol_interface_info_entry_t *cur, const uint8_t *opt)
{
}

void nd_ra_build_by_abro(const uint8_t *abro, const uint8_t *dest, protocol_interface_info_entry_t *cur_interface)
{
}

void nd_trigger_ras_from_rs(const uint8_t *unicast_adr, protocol_interface_info_entry_t *cur_interface)
{
}

void nd_na_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *dptr, const uint8_t *dst_addr)
{
}

void gp_address_add_to_end(gp_ipv6_address_list_t *list, const uint8_t address[__static 16])
{
}

void nd_object_timer(struct protocol_interface_info_entry *cur_interface, uint16_t ticks_update)
{
}

ipv6_ra_timing_t *nd_ra_timing(const uint8_t abro[16])
{
    return NULL;
}

uint32_t nd_object_time_to_next_nd_reg(void)
{
    return 0;
}


void nd_object_timer_balance(uint16_t sleep_time, protocol_interface_info_entry_t *cur_interface)
{
}

uint8_t nd_prefix_dst_check(uint8_t *ptr)
{
    return 0;
}

int8_t nd_parent_loose_indcate(uint8_t *neighbor_address, protocol_interface_info_entry_t *cur_interface)
{
    return -1;
}

nd_router_t *nd_get_object_by_nwk_id(nwk_interface_id nwk_id)
{
    return NULL;
}

nd_router_t *nd_get_pana_address(void)
{
    return NULL;
}

void nd_remove_registration(protocol_interface_info_entry_t *cur_interface, addrtype_t ll_type, const uint8_t *ll_address)
{

}

void nd_6lowpan_set_radv_params(protocol_interface_info_entry_t *cur_interface)
{

}
