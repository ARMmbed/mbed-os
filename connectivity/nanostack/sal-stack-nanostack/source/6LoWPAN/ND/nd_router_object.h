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
#include "6LoWPAN/ND/nd_defines.h"
#include "net_6lowpan_parameter_api.h"

#ifndef ND_ROUTER_OBJECT_H_
#define ND_ROUTER_OBJECT_H_


#define ND_OBJECT_MAX 1

#define ND_MAX_PROXY_CONTEXT_COUNT 5
#define ND_MAX_PROXY_PREFIX_COUNT 5

/* RFC4861 AdvCurHopLimit: value placed in Router Advertisement Cur Hop Limit */
#ifndef ADV_CUR_HOP_LIMIT
#define ADV_CUR_HOP_LIMIT 64
#endif
#ifdef HAVE_6LOWPAN_ND
extern uint8_t nd_base_tick;
extern nd_parameters_s nd_params;
struct aro;

#ifdef HAVE_6LOWPAN_BORDER_ROUTER
int8_t icmp_nd_router_prefix_proxy_update(uint8_t *dptr, nd_router_setup_t *nd_router_object);
int8_t nd_set_br(nd_router_t *br);
#else
#define icmp_nd_router_prefix_proxy_update(dptr, nd_router_object) -1
#define nd_set_br(br) -1
#endif
extern void icmp_nd_prefixs_parse(buffer_t *buf, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface);
extern int8_t icmp_nd_router_prefix_update(uint8_t *dptr, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface);
extern void gp_address_add_to_end(gp_ipv6_address_list_t *list, const uint8_t address[static 16]);
extern void gp_address_list_free(gp_ipv6_address_list_t *list);
extern uint8_t nd_set_adr_by_dest_prefix(uint8_t *ptr, uint8_t *prefix);
extern bool nd_object_active(void);
extern void icmp_nd_set_nd_def_router_address(uint8_t *ptr, nd_router_t *cur);
extern nd_router_t *icmp_nd_router_object_get(const uint8_t *border_router, nwk_interface_id nwk_id);
extern void icmp_nd_set_next_hop(nd_router_next_hop *hop, sockaddr_t *adr);



/** 6LoWPAN spesific ICMP message Handler */
extern buffer_t *nd_dar_parse(buffer_t *buf, protocol_interface_info_entry_t *cur_interface);
extern buffer_t *nd_dac_handler(buffer_t *buf, protocol_interface_info_entry_t *cur);
extern void nd_ns_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint8_t *address_ptr);
extern int8_t nd_parent_loose_indcate(uint8_t *neighbor_address, protocol_interface_info_entry_t *cur_interface);

void nd_router_base_init(nd_router_t *new_entry);


void icmp_nd_routers_init(void);

#ifdef HAVE_RPL
ipv6_ra_timing_t *nd_ra_timing(const uint8_t abro[16]);
void nd_ra_build_by_abro(const uint8_t *abro, const uint8_t *dest, protocol_interface_info_entry_t *cur_interface);
void nd_trigger_ras_from_rs(const uint8_t *unicast_adr, protocol_interface_info_entry_t *cur_interface);
/** 6LoWPAN spesific ICMP message Handler */
bool nd_ns_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *aro_opt, const uint8_t *slaa_opt, const uint8_t *target, struct aro *aro_out);
void nd_remove_registration(protocol_interface_info_entry_t *cur_interface, addrtype_t ll_type, const uint8_t *ll_address);
#else
#define nd_ra_timing(abro) NULL
#define nd_ra_build_by_abro(abro, dest, cur_interface) ((void)0)
#define nd_trigger_ras_from_rs(unicast_adr, cur_interface) ((void)0)
#define nd_ns_aro_handler(cur_interface, aro_opt, slaa_opt, target, aro_out) false
#define nd_remove_registration(cur_interface, l_type, ll_address) ((void)0)
#endif

nd_router_t *nd_get_pana_address(void);
/* This processes the 6CO for the interface itself - separate from the ABRO
 * multihop relay storage.
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |     Length    |Context Length | Res |C|  CID  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |            Reserved           |         Valid Lifetime        |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * .                                                               .
 * .                       Context Prefix                          .
 * .                                                               .
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
void nd_ra_process_lowpan_context_option(struct protocol_interface_info_entry *cur, const uint8_t *opt);

/** ND Routing Part */
uint8_t nd_prefix_dst_check(uint8_t *ptr);
nd_router_t *nd_get_object_by_nwk_id(nwk_interface_id nwk_id);
/* Original ABRO-based all-in-one parser. This needs some rework to separate ABRO-related and unrelated bits */
/* Returns "false" if ABRO suggested it was a stale message, so not worth handling in the normal code */
extern bool nd_ra_process_abro(struct protocol_interface_info_entry *cur, buffer_t *buf, const uint8_t *dptr, uint8_t ra_flags, uint16_t router_lifetime);
extern void nd_object_timer(struct protocol_interface_info_entry *cur_interface, uint16_t ticks_update);
extern uint32_t nd_object_time_to_next_nd_reg(void);

extern void icmp_nd_router_object_reset(nd_router_t *router_object);
extern void icmp_nd_border_router_release(nd_router_t *router_object);
void nd_6lowpan_set_radv_params(protocol_interface_info_entry_t *cur_interface);
#else
#define icmp_nd_routers_init() ((void)0)
#define nd_ns_aro_handler(cur_interface, aro_opt, slaa_opt, target, aro_out) false
#define nd_ra_build_by_abro(abro, dest, cur_interface) ((void)0)
#define nd_trigger_ras_from_rs(unicast_adr, cur_interface) ((void)0)
#define nd_ra_timing(abro) NULL
#define nd_prefix_dst_check(x) 0
#define nd_get_object_by_nwk_id(x) NULL
#define nd_ra_process_abro(x,y,z,a,b) false
#define nd_ra_process_lowpan_context_option(x,y) ((void)0)
#define nd_get_pana_address() NULL
#define nd_object_timer(x, ticks_update) ((void)0)
#define nd_object_time_to_next_nd_reg() 0
#define nd_object_active() false
#define gp_address_list_free(list) ((void)0)
#define gp_address_add_to_end(list, address) ((void)0)
#define nd_6lowpan_set_radv_params(cur) ((void)0)
#define nd_remove_registration(cur_interface, l_type, ll_address) ((void)0)
#endif
#endif /* ND_ROUTER_OBJECT_H_ */
