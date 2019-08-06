/*
 * Copyright (c) 2013-2019, Arm Limited and affiliates.
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
#ifdef HAVE_6LOWPAN_ND
#include "ns_types.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "randLIB.h"
#ifdef HAVE_RPL
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#endif
#include "MLE/mle.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "common_functions.h"
#include "BorderRouter/border_router.h"
#include "Service_Libs/pan_blacklist/pan_blacklist_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/ws/ws_common.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"

#define TRACE_GROUP "loND"

void icmp_nd_router_object_release(nd_router_t *router_object);
uint8_t icmp_nd_router_prefix_ttl_update(nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface, uint16_t seconds);
static uint8_t nd_router_bootstrap_timer(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t ticks);
#ifdef HAVE_RPL
static void nd_ra_build(nd_router_t *cur, const uint8_t *address, protocol_interface_info_entry_t *cur_interface);
static void nd_ns_forward_timer_reset(uint8_t *root_adr);
static void nd_router_forward_timer(nd_router_t *cur, uint16_t ticks_update);
static nd_router_t *nd_router_object_scan_by_prefix(const uint8_t *prefix, nwk_interface_id nwk_id);
#else
#define nd_ra_build(cur, address, cur_interface) ((void)0)
#define nd_ns_forward_timer_reset(root_adr) ((void)0)
#define nd_router_forward_timer(root_adr, ticks_update) ((void)0)
#define nd_router_object_scan_by_prefix(prefix, nwk_id) NULL
#endif

static void lowpan_nd_address_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason);
uint8_t nd_rs_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface);
bool icmp_nd_compare_to_def_next_hop(nd_router_next_hop *hop, sockaddr_t *adr);
void icmp_nd_router_context_ttl_update(nd_router_t *nd_router_object, uint16_t seconds);

//ND Router List
static NS_LARGE NS_LIST_DEFINE(nd_router_list, nd_router_t, link);

/*
 * Default values are documented in net_6lowpan_parameter_api.h - keep in sync.
 */
uint8_t nd_base_tick = 1;

nd_parameters_s nd_params = {
    .rs_retry_max = 3,
    .rs_retry_interval_min = 15,
    .ns_retry_interval_min = 100,
    .ns_retry_linear_backoff = 100,
    .timer_random_max = 31,
    .ns_retry_max = 5,
    .multihop_dad = true,
    .iids_map_to_mac = false,
    .send_nud_probes = true,
    .ra_interval_min = 160,
    .ra_transmits = 3,
    .ra_cur_hop_limit = ADV_CUR_HOP_LIMIT,
    .ra_link_mtu = 0,
    .ra_reachable_time = 0,
    .ra_retrans_timer = 0,
    .ns_forward_timeout = 300,
};

#ifdef HAVE_6LOWPAN_BORDER_ROUTER
int8_t nd_set_br(nd_router_t *br)
{
    if (ns_list_is_empty(&nd_router_list)) {
        ns_list_add_to_start(&nd_router_list, br);
        return 0;
    }
    return -1;
}
#endif

bool nd_object_active(void)
{
    if (!ns_list_is_empty(&nd_router_list)) {
        return true;
    }
    return false;
}

void icmp_nd_routers_init(void)
{
    ns_list_foreach_safe(nd_router_t, cur, &nd_router_list) {
        ns_list_remove(&nd_router_list, cur);
        icmp_nd_router_object_release(cur);
    }
}


void icmp_nd_set_nd_def_router_address(uint8_t *ptr, nd_router_t *cur)
{
    memcpy(ptr, ADDR_LINK_LOCAL_PREFIX, 8);
    ptr += 8;
    if (cur->default_hop.addrtype == ADDR_802_15_4_SHORT) {
        memcpy(ptr, ADDR_SHORT_ADR_SUFFIC, 6);
        ptr += 6;
        *ptr++ = cur->default_hop.address[0];
        *ptr = cur->default_hop.address[1];
    } else {
        memcpy(ptr, cur->default_hop.address, 8);
    }
}

void nd_ns_trig(nd_router_t *router_object, protocol_interface_info_entry_t *cur)
{
    //
    ns_list_foreach(prefix_entry_t, prefix, &router_object->prefix_list) {
        if (prefix->options & PIO_A) {
            ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
                if (e->source == ADDR_SOURCE_SLAAC && (memcmp(e->address, prefix->prefix, 8) == 0)) {
                    if (cur->if_6lowpan_dad_process.active) {
                        e->state_timer = 5;
                    } else {
                        e->state_timer = 25;
                        cur->if_6lowpan_dad_process.active = true;
                        memcpy(cur->if_6lowpan_dad_process.address, e->address, 16);
                        cur->if_6lowpan_dad_process.count = nd_params.ns_retry_max;
                    }
                }
            }
        }
    }
}

void nd_router_base_init(nd_router_t *new_entry)
{
    ns_list_link_init(new_entry, link);
    new_entry->nd_timer = 0;
    new_entry->nd_bootstrap_tick = 0;
    new_entry->nd_re_validate = 0;
    new_entry->mle_advert_timer = 0;
    new_entry->mle_purge_timer = 0;
    new_entry->default_hop.addrtype = ADDR_NONE;
    ns_list_init(&new_entry->prefix_list);
    ns_list_init(&new_entry->context_list);
    new_entry->secondaty_hop = 0;
    new_entry->ns_forward_timer = 0;
    new_entry->flags = 0;
    new_entry->ra_timing.initial_rtr_adv_count = 0;
    new_entry->ra_timing.rtr_adv_last_send_time = protocol_core_monotonic_time - 0x10000;
    new_entry->abro_version_num = 0;
    new_entry->trig_address_reg = false;
}

static void nd_router_remove(nd_router_t *router, protocol_interface_info_entry_t *interface)
{
    tr_debug("route remove");
    icmpv6_stop_router_advertisements(interface, router->border_router);
    ns_list_remove(&nd_router_list, router);
    icmp_nd_router_object_release(router);

    if (ns_list_is_empty(&nd_router_list)) {
        arm_6lowpan_bootstrap_init(interface);
    }
}

nd_router_t *icmp_nd_router_object_get(const uint8_t *border_router, nwk_interface_id nwk_id)
{
    nd_router_t *new_entry = 0;
    uint_fast8_t count = 0;

    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (cur->nd_state == ND_BR_READY) {
            return NULL;
        }
        if (cur->nwk_id == nwk_id) {
            if (memcmp(cur->border_router, border_router, 16) == 0) {
                return cur;
            }
        }
        ++count;
    }

    if (count >= ND_OBJECT_MAX) {
        return NULL;
    }

    new_entry = ns_dyn_mem_alloc(sizeof(nd_router_t));
    if (!new_entry) {
        tr_error("No heap for New Border Router");
        return NULL;
    }

    new_entry->nd_state = ND_READY;
    new_entry->nwk_id = nwk_id;
    nd_router_base_init(new_entry);
    memcpy(new_entry->border_router, border_router, 16);
    new_entry->trig_address_reg = false;
    ns_list_add_to_end(&nd_router_list, new_entry);

    return new_entry;
}

void icmp_nd_router_object_reset(nd_router_t *router_object)
{
    icmpv6_prefix_list_free(&router_object->prefix_list);

    lowpan_context_list_free(&router_object->context_list);

    if (router_object->secondaty_hop) {
        ns_dyn_mem_free(router_object->secondaty_hop);
        router_object->secondaty_hop = 0;
    }
}
#ifdef HAVE_6LOWPAN_BORDER_ROUTER
int8_t icmp_nd_router_prefix_proxy_update(uint8_t *dptr, nd_router_setup_t *nd_router_object)
{
    prefix_entry_t *new_entry = 0;
    uint8_t pre_setups;
    uint32_t lifeTime, prefTime;
    uint8_t prefix_len = *dptr++;
    pre_setups = *dptr++;
    lifeTime = common_read_32_bit(dptr);
    dptr += 4;
    prefTime = common_read_32_bit(dptr);
    dptr += 4;
    pre_setups |= PIO_R;

    new_entry = icmpv6_prefix_add(&nd_router_object->prefix_list, dptr, prefix_len, lifeTime, prefTime, pre_setups);
    if (new_entry) {
        new_entry->options = pre_setups;
        return 0;
    }
    return -2;
}
#endif

uint8_t icmp_nd_router_prefix_valid(nd_router_t *nd_router_object)
{
    ns_list_foreach(prefix_entry_t, cur, &nd_router_object->prefix_list) {
        if ((cur->options & PIO_A) && cur->lifetime) {
            return 1;
        }
    }
    return 0;
}

/* Returns 1 if the router object has been removed */
uint8_t icmp_nd_router_prefix_ttl_update(nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface, uint16_t seconds)
{
    ns_list_foreach(prefix_entry_t, cur, &nd_router_object->prefix_list) {
        if (cur->preftime != 0xffffffff && cur->preftime) {
            if (cur->preftime <=  seconds) {
                tr_warn("PREFTIME zero");
                cur->preftime = 0;
            } else {
                cur->preftime -= seconds;
            }

        }

        if (cur->lifetime != 0xffffffff && cur->lifetime) {
            if (cur->lifetime > seconds) {
                cur->lifetime -= seconds;
            } else {
                tr_debug("Prefix Expiry");
                if (nd_router_object->nd_state != ND_BR_READY) {
                    if (cur->options & PIO_A) {
                        tr_debug("Delete GP Address by Prefix, start RS");
                        nd_router_remove(nd_router_object, cur_interface);
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

static int icmp_nd_slaac_prefix_address_gen(protocol_interface_info_entry_t *cur_interface, uint8_t *prefix, uint8_t prefix_len, uint32_t lifetime, uint32_t preftime, bool borRouterDevice, slaac_src_e slaac_src)
{
    if_address_entry_t *address_entry = NULL;
    address_entry = icmpv6_slaac_address_add(cur_interface, prefix, prefix_len, lifetime, preftime, true, slaac_src);
    if (address_entry) {
        //Set Callback
        address_entry->cb = lowpan_nd_address_cb;
        if (borRouterDevice) {
            address_entry->state_timer = 0;
        } else {
            address_entry->state_timer = 45 + randLIB_get_random_in_range(1, nd_params.timer_random_max);
            //Allocate Addres registration state
            if (cur_interface->if_6lowpan_dad_process.active == false) {
                cur_interface->if_6lowpan_dad_process.count = nd_params.ns_retry_max;
                cur_interface->if_6lowpan_dad_process.active = true;
                memcpy(cur_interface->if_6lowpan_dad_process.address, address_entry->address, 16);
            }

            if ((cur_interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) && cur_interface->nwk_bootstrap_state == ER_SCAN) {
                cur_interface->nwk_bootstrap_state = ER_ADDRESS_REQ;
                cur_interface->bootsrap_state_machine_cnt = 0;
            }
        }
        return 0;
    }
    return -1;
}


static void lowpan_nd_address_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{
    nd_router_t *cur = NULL;
    bool g16_address;
    tr_debug("Interface ID: %i, ipv6: %s", interface->id, trace_ipv6(addr->address));

    if (memcmp(&addr->address[8], ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
        g16_address = true;
    } else {
        g16_address = false;
    }

    switch (reason) {
        case ADDR_CALLBACK_DAD_COMPLETE:
            if (addr_ipv6_equal(interface->if_6lowpan_dad_process.address, addr->address)) {
                tr_info("Address REG OK: %s", trace_ipv6(interface->if_6lowpan_dad_process.address));
                interface->if_6lowpan_dad_process.active = false;
                interface->global_address_available = true;
                //Check If GP16 Address Add LL16 Address
                if (interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ACTIVE) {
                    if (g16_address) {
                        //Register also GP64 if NET_6LOWPAN_MULTI_GP_ADDRESS mode is enabled
                        if (interface->lowpan_address_mode == NET_6LOWPAN_MULTI_GP_ADDRESS) {
                            if (icmp_nd_slaac_prefix_address_gen(interface, addr->address, addr->prefix_len,
                                                                 addr->valid_lifetime, addr->preferred_lifetime, false, SLAAC_IID_DEFAULT) == 0) {
                                return;
                            } else {
                                tr_warning("Secondary Address allocate fail");
                            }
                        }

                    }

                    protocol_6lowpan_bootstrap_nd_ready(interface);
                } else {
                    //Disable protocol poll
                    mac_data_poll_protocol_poll_mode_decrement(interface);
                }

            }
            break;

        case ADDR_CALLBACK_TIMER:
            tr_debug("State Timer CB");
            if (interface->if_6lowpan_dad_process.active) {
                if (memcmp(addr->address, interface->if_6lowpan_dad_process.address, 16) == 0) {
                    cur = nd_get_object_by_nwk_id(interface->nwk_id);
                } else {
                    addr->state_timer = 5;
                }
            } else {
                cur = nd_get_object_by_nwk_id(interface->nwk_id);
                if (cur) {
                    interface->if_6lowpan_dad_process.count = nd_params.ns_retry_max;
                    interface->if_6lowpan_dad_process.active = true;
                    memcpy(interface->if_6lowpan_dad_process.address, addr->address, 16);
                } else {
                    tr_debug("No ND Object for Address");
                }
            }

            if (cur) {
                if (interface->if_6lowpan_dad_process.count) {
                    nd_ns_build(cur, interface, addr->address);
                    addr->state_timer = nd_params.ns_retry_interval_min;
                    addr->state_timer += nd_params.ns_retry_linear_backoff * (nd_params.ns_retry_max - interface->if_6lowpan_dad_process.count);
                    addr->state_timer += (randLIB_get_16bit() & nd_params.timer_random_max);
                    tr_debug("NS Configured");
                    interface->if_6lowpan_dad_process.count--;
                    //Enable Protocol Poll mode
                    if (interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
                        mac_data_poll_init_protocol_poll(interface);
                    }
                } else {

                    //ND FAIL
                    tr_error("NS Fail");
                    protocol_6lowpan_neighbor_remove(interface, cur->default_hop.address, cur->default_hop.addrtype);

                    if (cur->secondaty_hop) {
                        tr_warn("Try Secondary Route");
                        memcpy(&cur->default_hop, cur->secondaty_hop, sizeof(nd_router_next_hop));
                        ns_dyn_mem_free(cur->secondaty_hop);
                        cur->secondaty_hop = 0;
                        interface->if_6lowpan_dad_process.count = nd_params.ns_retry_max;
                        addr->state_timer = 1;
                    } else {
                        interface->if_6lowpan_dad_process.active = false;
                        protocol_6lowpan_nd_borderrouter_connection_down(interface);
                    }
                }
            }

            break;

        case ADDR_CALLBACK_PARENT_FULL:
            interface->if_6lowpan_dad_process.count = 0;
            tr_error("ND cache full--> Black list by given lifetime");
            cur = nd_get_object_by_nwk_id(interface->nwk_id);
            if (cur) {
                pan_blacklist_pan_set(&interface->pan_blaclist_cache, mac_helper_panid_get(interface), cur->life_time);
            }
            break;

        case ADDR_CALLBACK_DAD_FAILED:
            if (g16_address) {
                tr_warn("ARO Failure:Duplicate address");
                uint16_t shortAddress = common_read_16_bit(&addr->address[14]);
                tr_warn("Del old ll16");
                protocol_6lowpan_del_ll16(interface, shortAddress);
                //Check if Application not freeze new address allocartion
                if (interface->reallocate_short_address_if_duplicate) {

                    protocol_6lowpan_allocate_mac16(interface);
                    interface->if_6lowpan_dad_process.active = false;
                    if (icmp_nd_slaac_prefix_address_gen(interface, addr->address, addr->prefix_len,
                                                         addr->valid_lifetime, addr->preferred_lifetime, false, SLAAC_IID_6LOWPAN_SHORT) == 0) {
                        addr->state_timer = 1;
                        return;
                    }
                }
            }
            bootsrap_next_state_kick(ER_BOOTSTRAP_DAD_FAIL, interface);

            break;

        default:
            break;
    }
}

int8_t icmp_nd_router_prefix_update(uint8_t *dptr, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface)
{
    slaac_src_e slaac_src;
    bool borRouterDevice;
    prefix_entry_t *new_entry = 0;
    uint8_t prefix_len = *dptr++;
    uint8_t pre_setups = *dptr++;
    uint32_t lifetime = common_read_32_bit(dptr);
    uint32_t preftime = common_read_32_bit(dptr + 4);

    if (cur_interface->lowpan_address_mode == NET_6LOWPAN_GP16_ADDRESS
            || cur_interface->lowpan_address_mode == NET_6LOWPAN_MULTI_GP_ADDRESS) {
        slaac_src = SLAAC_IID_6LOWPAN_SHORT;
    } else {
        slaac_src = SLAAC_IID_DEFAULT;
    }
    if (cur_interface->border_router_setup) {
        borRouterDevice = true;
    } else {
        borRouterDevice = false;
    }

    //Read Lifetimes + skip resertved 4 bytes
    dptr += 12;
    new_entry = icmpv6_prefix_add(&nd_router_object->prefix_list, dptr, prefix_len, lifetime, preftime, pre_setups);
    if (new_entry) {
        if (new_entry->options == 0xff) {
            new_entry->options = pre_setups;
            if (new_entry->options & PIO_A) {
                if (icmpv6_slaac_prefix_update(cur_interface, new_entry->prefix, new_entry->prefix_len, new_entry->lifetime, new_entry->preftime) != 0) {
                    icmp_nd_slaac_prefix_address_gen(cur_interface, new_entry->prefix, new_entry->prefix_len, new_entry->lifetime, new_entry->preftime, borRouterDevice, slaac_src);
                }
            }
        } else {
            icmpv6_slaac_prefix_update(cur_interface, dptr, prefix_len, lifetime, preftime);
        }

        if (nd_router_object->trig_address_reg) {
            nd_router_object->trig_address_reg = false;
            icmpv6_slaac_prefix_register_trig(cur_interface, dptr, prefix_len);
        }

        return 0;
    }

    return -2;
}

/* Update lifetime and expire contexts in ABRO storage */
void icmp_nd_router_context_ttl_update(nd_router_t *nd_router_object, uint16_t seconds)
{
    ns_list_foreach_safe(lowpan_context_t, cur, &nd_router_object->context_list) {
        /* We're using seconds in call, but lifetime is in 100ms ticks */
        if (cur->lifetime <= (uint32_t)seconds * 10) {
            /* When lifetime in the ABRO storage runs out, just drop it,
             * so we stop advertising it. This is different from the
             * interface context handling.
             */
            ns_list_remove(&nd_router_object->context_list, cur);
            ns_dyn_mem_free(cur);
        } else {
            cur->lifetime -= (uint32_t)seconds * 10;
        }
    }
}

void icmp_nd_router_object_release(nd_router_t *router_object)
{
    if (router_object) {
        icmp_nd_router_object_reset(router_object);
        ns_dyn_mem_free(router_object);
    }
}


void icmp_nd_border_router_release(nd_router_t *router_object)
{
    if (router_object) {
        if (!ns_list_is_empty(&nd_router_list)) {
            ns_list_remove(&nd_router_list, router_object);
        }
        icmp_nd_router_object_reset(router_object);
    }
}



void gp_address_list_free(gp_ipv6_address_list_t *list)
{
    ns_list_foreach_safe(gp_ipv6_address_entry_t, cur, list) {
        ns_list_remove(list, cur);
        ns_dyn_mem_free(cur);
    }
}

/*
 * Parse 6LoWPAN Context Options (RFC 6775 4.2) for ABRO storage.
 *
 *     0                   1                   2                   3
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
static void icmp_nd_context_parse(buffer_t *buf, nd_router_t *nd_router_object)
{
    const uint8_t *dptr = buffer_data_pointer(buf);
    uint16_t data_len = buffer_data_length(buf);
    uint16_t read = 0;

    while (read < data_len) {
        uint8_t type = dptr[0];
        uint16_t len = dptr[1] * 8;

        read += len;

        if (len == 0 || read > data_len) {
            return;
        } else if (type == ICMPV6_OPT_6LOWPAN_CONTEXT) {
            uint8_t ctx_len = dptr[2];

            if ((len == 16 && ctx_len <= 64) || (len == 24 && ctx_len <= 128)) {
                uint8_t c_id = dptr[3] & 0x1f; // ignore reserved fields
                uint16_t lifetime_mins = common_read_16_bit(dptr + 6);

                lowpan_context_update(&nd_router_object->context_list, c_id, lifetime_mins, dptr + 8, ctx_len, true);
            } else {
                tr_warn("Context len fail");
            }
        }
        dptr += len;
    }
}

void icmp_nd_prefixs_parse(buffer_t *buf, nd_router_t *nd_router_object, protocol_interface_info_entry_t *cur_interface)
{
    uint8_t *dptr;
    uint16_t data_len = buffer_data_length(buf);
    uint16_t readed = 0;
    uint8_t type, len;
    dptr = buffer_data_pointer(buf);
    while (readed  < data_len) {
        type = *dptr++;
        len  = *dptr++;
        if (len == 0) {
            return;
        }
        len <<= 3;
        readed += len;

        if (readed > data_len) {
            return;
        } else if (type == ICMPV6_OPT_PREFIX_INFO && len == 32) {
            icmp_nd_router_prefix_update(dptr, nd_router_object, cur_interface);
            dptr += 30;
        } else {
            dptr += (len - 2);
        }
    }
}


void icmp_nd_set_next_hop(nd_router_next_hop *hop, sockaddr_t *adr)
{
    uint8_t *ptr = 0;
    ptr = hop->address;
    if (adr->addr_type == ADDR_IPV6) {
        if (memcmp(&adr->address[8], ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
            hop->addrtype = ADDR_802_15_4_SHORT;
            memcpy(ptr, &adr->address[14], 2);
        } else {
            hop->addrtype = ADDR_802_15_4_LONG;
            memcpy(ptr, &adr->address[8], 8);
        }
    } else {
        hop->addrtype = adr->addr_type;
        memcpy(ptr, &adr->address[2], 8);
    }
}

bool icmp_nd_compare_to_def_next_hop(nd_router_next_hop *hop, sockaddr_t *adr)
{
    if (adr->addr_type == ADDR_IPV6) {
        if (memcmp(&adr->address[8], ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
            if (hop->addrtype == ADDR_802_15_4_SHORT) {
                if (memcmp(hop->address, &adr->address[14], 2) == 0) {
                    return false;
                }
            }
        } else {
            if (hop->addrtype == ADDR_802_15_4_LONG) {
                if (memcmp(hop->address, &adr->address[8], 8) == 0) {
                    return false;
                }
            }

        }
    } else {
        hop->addrtype = adr->addr_type;
        memcpy(hop->address, &adr->address[2], 8);
    }
    return true;
}

uint8_t nd_rs_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface)
{
    buffer_t *buf;

    if (cur) {
        uint8_t dest_addr[16];
        icmp_nd_set_nd_def_router_address(dest_addr, cur);
        buf = icmpv6_build_rs(cur_interface, dest_addr);
    } else {
        buf = icmpv6_build_rs(cur_interface, NULL);
    }

    if (buf) {
        arm_net_protocol_packet_handler(buf, cur_interface);
        return 1;
    }
    return 0;
}

#ifdef HAVE_RPL
static bool rpl_parents_only(const ipv6_route_info_t *route, bool valid)
{
    return valid && rpl_data_is_rpl_parent_route(route->source);
}
#endif

/* Neighbor Solicitation (RFC4861) with Address Registration Option (RFC6775)
 * and Source Link-Layer Address Option (RFC4861)
 */
void nd_ns_build(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint8_t *address_ptr)
{
    uint8_t router[16];
    aro_t aro;
    buffer_t *buf;

#ifdef HAVE_RPL
    /* If we're a host, we will just send to our ND parent. But as a router,
     * we don't really maintain our ND parent - send NA instead to the RPL
     * parent we would use to talk to the border router.
     */
    if ((cur_interface->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) && cur_interface->rpl_domain) {
        ipv6_route_t *route = ipv6_route_choose_next_hop(cur->border_router, cur_interface->id, rpl_parents_only);
        if (!route) {
            /* Important to return 1 so this counts as a "success" - caller then backs off due to lack of response and time out */
            return;
        }
        memcpy(router, route->info.next_hop_addr, 16);
    } else
#endif
    {
        icmp_nd_set_nd_def_router_address(router, cur);
    }

    aro.status = ARO_SUCCESS;
    aro.present = true;
    aro.lifetime = (cur->life_time / 60) + 1;
    memcpy(aro.eui64, cur_interface->mac, 8);

    buf = icmpv6_build_ns(cur_interface, router, address_ptr, true, false, &aro);
    protocol_push(buf);
}

/* RFC 6775 Duplicate Address Request/Confirmation packets
 *
 *     0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |     Code      |          Checksum             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |    Status     |   Reserved    |     Registration Lifetime     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * +                            EUI-64                             +
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * +                                                               +
 * |                                                               |
 * +                       Registered Address                      +
 * |                                                               |
 * +                                                               +
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * (and 8.2.1 implies this is can be followed by options, although
 * none are defined).
 */
#ifdef HAVE_6LOWPAN_ROUTER
static bool nd_dar_dac_valid(buffer_t *buf)
{
    const uint8_t *dptr = buffer_data_pointer(buf);

    if (buf->options.code != 0) {
        return false;
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 28)) {
        return false;
    }

    if (addr_is_ipv6_multicast(dptr + 12)) {
        return false;
    }

    if (addr_is_ipv6_unspecified(buf->src_sa.address) ||
            addr_is_ipv6_multicast(buf->src_sa.address)) {
        return false;
    }

    return true;
}
#endif

buffer_t *nd_dar_parse(buffer_t *buf, protocol_interface_info_entry_t *cur_interface)
{
#if defined WHITEBOARD && defined HAVE_6LOWPAN_BORDER_ROUTER
    uint8_t *dptr = buffer_data_pointer(buf);
    buffer_t *retbuf;
    uint8_t status;
    uint16_t lifetime;
    const uint8_t *eui64;

    if (!nd_dar_dac_valid(buf)) {
        goto drop;
    }

    status = *dptr;
    dptr += 2;
    lifetime = common_read_16_bit(dptr);
    dptr += 2;

    if (status != ARO_SUCCESS) {
        goto drop;
    }

    whiteboard_entry_t *wb;

    /* EUI-64 */
    eui64 = dptr;
    dptr += 8;
    tr_debug("DAR adr: %s, from %s", trace_ipv6(dptr), trace_ipv6(buf->src_sa.address));

    //SET White board
    wb = whiteboard_table_update(dptr, eui64, &status);
    if (wb && status == ARO_SUCCESS) {
        memcpy(wb->address, dptr, 16);
        memcpy(wb->eui64, eui64, 8);
        wb->interface_index = cur_interface->id;
        wb->ttl = UINT24_C(60) * lifetime;
    }

    retbuf = icmpv6_build_dad(cur_interface, NULL, ICMPV6_TYPE_INFO_DAC, buf->src_sa.address, eui64, dptr, status, lifetime);
    if (retbuf) {
        buffer_free(buf);
        return retbuf;
    }

drop:
#else
    (void)cur_interface;
#endif

    return buffer_free(buf);
}

#ifdef HAVE_6LOWPAN_ROUTER
static void nd_update_registration(protocol_interface_info_entry_t *cur_interface, ipv6_neighbour_t *neigh, const aro_t *aro)
{
    /* We are about to send an ARO response - update our Neighbour Cache accordingly */
    if (aro->status == ARO_SUCCESS && aro->lifetime != 0) {
        neigh->type = IP_NEIGHBOUR_REGISTERED;
        neigh->lifetime = aro->lifetime * UINT32_C(60);
        ipv6_neighbour_set_state(&cur_interface->ipv6_neighbour_cache, neigh, IP_NEIGHBOUR_STALE);
        /* Register with 2 seconds off the lifetime - don't want the NCE to expire before the route */
        ipv6_route_add_metric(neigh->ip_address, 128, cur_interface->id, neigh->ip_address, ROUTE_ARO, NULL, 0, neigh->lifetime - 2, 32);

        /* We need to know peer is a host before publishing - this needs MLE. Not yet established
         * what to do without MLE - might need special external/non-external prioritisation at root.
         * This "publish for RFD" rule comes from ZigBee IP.
         */
        mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur_interface), ipv6_neighbour_eui64(&cur_interface->ipv6_neighbour_cache, neigh), ADDR_802_15_4_LONG);

        if (entry) {
            if (ws_info(cur_interface)) {
                ws_common_etx_validate(cur_interface, entry);
            }

            if (!entry->ffd_device) {
                rpl_control_publish_host_address(protocol_6lowpan_rpl_domain, neigh->ip_address, neigh->lifetime);
            }
        }
        protocol_6lowpan_neighbor_address_state_synch(cur_interface, aro->eui64, neigh->ip_address + 8);

    } else {
        /* Um, no - can't transmit response if we remove NCE now! */
        //ipv6_neighbour_entry_remove(&cur_interface->ipv6_neighbour_cache, neigh);
        neigh->type = IP_NEIGHBOUR_TENTATIVE;
        neigh->lifetime = 2;
        ipv6_neighbour_set_state(&cur_interface->ipv6_neighbour_cache, neigh, IP_NEIGHBOUR_STALE);
        ipv6_route_add_metric(neigh->ip_address, 128, cur_interface->id, neigh->ip_address, ROUTE_ARO, NULL, 0, 4, 32);
        rpl_control_unpublish_address(protocol_6lowpan_rpl_domain, neigh->ip_address);
    }
}

void nd_remove_registration(protocol_interface_info_entry_t *cur_interface, addrtype_t ll_type, const uint8_t *ll_address)
{
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cur_interface->ipv6_neighbour_cache.list) {
        if ((cur->type == IP_NEIGHBOUR_REGISTERED
                || cur->type == IP_NEIGHBOUR_TENTATIVE)
                && ipv6_neighbour_ll_addr_match(cur, ll_type, ll_address)) {

            ipv6_route_delete(cur->ip_address, 128, cur_interface->id, cur->ip_address,
                              ROUTE_ARO);
            ipv6_neighbour_entry_remove(&cur_interface->ipv6_neighbour_cache,
                                        cur);
        }
    }
}

/* Process ICMP Neighbor Solicitation (RFC 4861 + RFC 6775) ARO. */
bool nd_ns_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *aro_opt, const uint8_t *slla_opt, const uint8_t *src_addr, aro_t *aro_out)
{
    /* Ignore any ARO if source is link-local */
    if (addr_is_ipv6_link_local(src_addr)) {
        return true; /* Transmit NA, without ARO */
    }

    /* If we can't parse the SLLAO, then act as if no SLLAO: ignore ARO */
    sockaddr_t ll_addr;
    if (!cur_interface->if_llao_parse(cur_interface, slla_opt, &ll_addr)) {
        return true; /* Transmit NA, without ARO */
    }

    /*
     *  0                   1                   2                   3
     *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |   Type = 33   |   Length = 2  |    Status     |   Reserved    |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |           Reserved            |     Registration Lifetime     |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |                                                               |
     * +                            EUI-64                             +
     * |                                                               |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */
    /* icmpv6_ns_handler has already checked incoming status == 0 */
    aro_out->lifetime = common_read_16_bit(aro_opt + 6);
    memcpy(aro_out->eui64, aro_opt + 8, 8);

    /* Check if we are already using this address ourself */
    if (addr_interface_address_compare(cur_interface, src_addr) == 0) {
        aro_out->present = true;
        aro_out->status = ARO_DUPLICATE;
        return true;
    }

    /* TODO - check hard upper limit on registrations? */
    if (ws_info(cur_interface) &&
            !ws_common_allow_child_registration(cur_interface, aro_out->eui64)) {
        aro_out->present = true;
        aro_out->status = ARO_FULL;
        return true;
    }

    /* We need to have entry in the Neighbour Cache */
    ipv6_neighbour_t *neigh = ipv6_neighbour_lookup_or_create(&cur_interface->ipv6_neighbour_cache, src_addr);
    if (!neigh) {
        aro_out->present = true;
        aro_out->status = ARO_FULL;
        return true;
    }

    uint8_t *nce_eui64 = ipv6_neighbour_eui64(&cur_interface->ipv6_neighbour_cache, neigh);
    if (neigh->state != IP_NEIGHBOUR_NEW) {
        switch (neigh->type) {
            case IP_NEIGHBOUR_TENTATIVE:
                /* Is zero EUI-64 still possible? */
                if (memcmp(nce_eui64, aro_out->eui64, 8) && memcmp(nce_eui64, ADDR_EUI64_ZERO, 8)) {
                    /* Have a Tentative NCE with different EUI-64 - ignore NS; two
                     * people trying to register at once. One should retry.
                     */
                    return false;
                }
                break;
            case IP_NEIGHBOUR_REGISTERED:
                if (memcmp(nce_eui64, aro_out->eui64, 8)) {
                    /* Already registered with different EUI-64 - duplicate */
                    aro_out->present = true;
                    aro_out->status = ARO_DUPLICATE;
                    return true;
                }
                break;
            case IP_NEIGHBOUR_GARBAGE_COLLECTIBLE:
                break;
        }
    }

    if (neigh->type != IP_NEIGHBOUR_REGISTERED) {
        neigh->type = IP_NEIGHBOUR_TENTATIVE;
        neigh->lifetime = TENTATIVE_NCE_LIFETIME;
        memcpy(nce_eui64, aro_out->eui64, 8);
    }

    /* Set the LL address, ensure it's marked STALE */
    ipv6_neighbour_entry_update_unsolicited(&cur_interface->ipv6_neighbour_cache, neigh, ll_addr.addr_type, ll_addr.address);
    ipv6_neighbour_set_state(&cur_interface->ipv6_neighbour_cache, neigh, IP_NEIGHBOUR_STALE);
    if (ws_info(cur_interface)) {
        aro_out->status = ARO_SUCCESS;
        aro_out->present = true;
        // Todo: this might not be needed...
        nd_update_registration(cur_interface, neigh, aro_out);
        return true;
    }
    if (cur_interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER || nd_params.multihop_dad == false) {
        if (cur_interface->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
            whiteboard_entry_t *wb;
            wb = whiteboard_table_update(src_addr, aro_out->eui64, &aro_out->status);
            if (wb) {
                if (aro_out->status == ARO_SUCCESS) {
                    memcpy(wb->address, src_addr, 16);
                    memcpy(wb->eui64, aro_out->eui64, 8);
                    wb->interface_index = cur_interface->id;
                    wb->ttl = 50000;//life_time;
                }
            } else {
                tr_warn("white Board Registry fail");
                aro_out->status = ARO_FULL;
                goto RESPONSE;
            }
        }

RESPONSE:
        aro_out->present = true;
        nd_update_registration(cur_interface, neigh, aro_out);
        return true; /* Transmit NA */
    } else { /* Non-border router and multihop DAD: relay as DAR to Border Router */
        nd_router_t *nd_router_obj = 0;

        nd_router_obj = nd_router_object_scan_by_prefix(src_addr, cur_interface->nwk_id);
        if (!nd_router_obj) {
            /* Don't know where to send this. Do we say "yay" or "nay"? */
            /* For now, ignore ARO, as with old code; don't set aro_out.present */
            return true;
        }

        buffer_t *buf = icmpv6_build_dad(cur_interface, NULL, ICMPV6_TYPE_INFO_DAR, nd_router_obj->border_router, aro_out->eui64, src_addr, 0, aro_out->lifetime);
        if (!buf) {
            return false;    /* Failed to build DAR - drop NS */
        }

        tr_debug("RX:NS --> TX DAR to Root");
        protocol_push(buf);
        if (nd_router_obj->ns_forward_timer == 0) {
            nd_router_obj->ns_forward_timer = nd_params.ns_forward_timeout;
        }

        return false; /* Tell ns_handler to not transmit now */
    }
}

buffer_t *nd_dac_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    uint8_t *dptr, target_address[16], *reg_address;
    aro_t aro;

    dptr = buffer_data_pointer(buf);

    if (!nd_dar_dac_valid(buf)) {
        return buffer_free(buf);
    }

    nd_ns_forward_timer_reset(buf->src_sa.address);

    aro.status  = *dptr;
    dptr += 2;
    aro.lifetime = common_read_16_bit(dptr);
    dptr += 2;
    /* EUI-64 */
    memcpy(aro.eui64, dptr, 8);
    dptr += 8;
    reg_address = dptr;
    dptr += 16;

    ipv6_neighbour_t *neigh = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, reg_address);
    if (!neigh || neigh->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE || memcmp(ipv6_neighbour_eui64(&cur->ipv6_neighbour_cache, neigh), aro.eui64, 8)) {
        return buffer_free(buf);
    }

    nd_update_registration(cur, neigh, &aro);

    /* RFC 6775 has a bit of a hole here - what's the Target Address? */
    /* It's not in the DAC. We didn't record locally when we sent the DAR */
    /* I guess it's logical that we use a link-local address. We break */
    /* RFC 4861 by responding "solicited", but not to the NS Target... */
    /* However, my reading of RFC 4861 says that the receiver should do */
    /* the right thing. Only problem is that what if they really did want */
    /* to do a NUD probe for our GP addr, but included the ARO by mistake? */
    if (addr_interface_get_ll_address(cur, target_address, 0)) {
        return buffer_free(buf);
    }

    /* NA builder will send it to the address in the buffer's source */
    memcpy(buf->src_sa.address, reg_address, 16);

    buffer_t *na_buf = icmpv6_build_na(cur, true, true, false, target_address, &aro, buf->src_sa.address);

    buffer_free(buf);

    return na_buf;
}
#endif // HAVE_6LOWPAN_ROUTER

/* Original ABRO-based all-in-one parser. This needs some rework to separate ABRO-related and unrelated bits */
/* Returns "false" if ABRO suggested it was a stale message, so not worth handling in the normal code */
bool nd_ra_process_abro(protocol_interface_info_entry_t *cur, buffer_t *buf, const uint8_t *dptr, uint8_t ra_flags, uint16_t router_lifetime)
{
    nd_router_t *router;
    uint32_t abro_ver_num;
    uint16_t temp16;
    bool uptodate = false;

    /* XXX this is really a 32-bit number these days */
    temp16 = common_read_16_bit(dptr);
    abro_ver_num = temp16;
    dptr += 2;
    temp16 = common_read_16_bit(dptr);
    //SET MSB bytes to 32-bit
    abro_ver_num |= ((uint32_t)temp16 << 16);

    dptr += 4;
    //If Border Router boot is state

    if (cur->border_router_setup) {
        if (memcmp(dptr, cur->border_router_setup->border_router_gp_adr, 16) == 0) {
            if (cur->border_router_setup->initActive) {
                //save New Context
                if (common_serial_number_greater_32(abro_ver_num, cur->border_router_setup->nd_border_router_configure->abro_version_num)) {
                    cur->border_router_setup->initActive = false;
                    cur->border_router_setup->nd_border_router_configure->abro_version_num = (abro_ver_num + 0x00010000);
                    cur->border_router_setup->nd_nwk->abro_version_num = (abro_ver_num + 0x00010000);
                } else if (abro_ver_num == cur->border_router_setup->nd_border_router_configure->abro_version_num) {

                    cur->border_router_setup->initActive = false;
                    cur->border_router_setup->nd_border_router_configure->abro_version_num = (abro_ver_num + 0x00010000);
                    cur->border_router_setup->nd_nwk->abro_version_num = (abro_ver_num + 0x00010000);
                }
            }
            return true;
        }
        return false;
    }

    router = icmp_nd_router_object_get(dptr, buf->interface->nwk_id);
    if (!router) {
        return true;
    }

    if (router->default_hop.addrtype == ADDR_NONE) {
        tr_debug("Create ND version");

        router->flags = ra_flags;

        uptodate = true;
        icmp_nd_set_next_hop(&router->default_hop, &buf->src_sa);
        router->default_hop.LQI = buf->options.lqi;

        icmp_nd_prefixs_parse(buf, router, cur);
        icmp_nd_context_parse(buf, router);

        if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
            //SET OnlY primary
            uint8_t address[8];
            if (router->default_hop.addrtype == ADDR_802_15_4_SHORT) {
                address[6] = router->default_hop.address[0];
                address[7] = router->default_hop.address[1];
                memcpy(address, ADDR_SHORT_ADR_SUFFIC, 6);
            } else {
                memcpy(address, router->default_hop.address, 8);
            }
            protocol_6lowpan_neighbor_priority_update(cur, NULL, address);
        }
        router->nd_state = ND_READY;
        router->nd_re_validate = (router_lifetime / 5) * 4;
        router->nd_timer = 10;
    } else {
        bool new_router = icmp_nd_compare_to_def_next_hop(&router->default_hop, &buf->src_sa);

        if (abro_ver_num == router->abro_version_num) {
            /* Up-to-date for host processing, but don't necessarily process for multi-hop relaying */
            uptodate = true;
        }

        /* XXX this is really a 32-bit number these days */
        if (common_serial_number_greater_32(abro_ver_num, router->abro_version_num)) {
            uint32_t diff_update;
            diff_update = (abro_ver_num - router->abro_version_num);
            uptodate = true;

            //uprouter_info=1;
            if (diff_update >= 0x00010000) {
                tr_debug("Border Router Boot Trig NS");
                router->trig_address_reg = true;
            } else {
                tr_debug("Next num");
            }

            icmpv6_prefix_list_free(&router->prefix_list);

            lowpan_context_list_free(&router->context_list);

            icmp_nd_prefixs_parse(buf, router, cur);
            router->trig_address_reg = false;
            icmp_nd_context_parse(buf, router);
            icmpv6_restart_router_advertisements(cur, router->border_router);
        } else if (router->default_hop.LQI < buf->options.lqi) {
            /* XXX another zero-hysteresis parent swap */
            if (new_router) {
                uint8_t *sec_ptr = NULL;
                if (router->secondaty_hop == 0) {
                    router->secondaty_hop = ns_dyn_mem_alloc(sizeof(nd_router_next_hop));
                } else {
                    nd_router_next_hop *hop = router->secondaty_hop;
                    sec_ptr = hop->address;
                    if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
                        if (hop->addrtype == ADDR_802_15_4_SHORT) {
                            hop->address[6] = hop->address[0];
                            hop->address[7] = hop->address[1];
                            memcpy(hop->address, ADDR_SHORT_ADR_SUFFIC, 6);
                        }
                    }
                }
                if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
                    //Remove old secondary priority and set new primary
                    protocol_6lowpan_neighbor_priority_update(cur, sec_ptr, &buf->src_sa.address[8]);
                }
                //
                if (router->secondaty_hop) {
                    memcpy(router->secondaty_hop, &router->default_hop, sizeof(nd_router_next_hop));
                }
                icmp_nd_set_next_hop(&router->default_hop, &buf->src_sa);
            }
            router->default_hop.LQI = buf->options.lqi;
        } else if (new_router) {
            if (!router->secondaty_hop) {
                router->secondaty_hop = ns_dyn_mem_alloc(sizeof(nd_router_next_hop));
                if (router->secondaty_hop) {
                    router->secondaty_hop->LQI = 0;
                }
            }
            if (router->secondaty_hop) {
                if (router->secondaty_hop->LQI < buf->options.lqi) {
                    icmp_nd_set_next_hop(router->secondaty_hop, &buf->src_sa);
                    router->secondaty_hop->LQI = buf->options.lqi;
                    if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
                        //SET only new primary
                        protocol_6lowpan_neighbor_priority_update(cur, 0, &buf->src_sa.address[8]);
                    }
                }
            }
        }
        /* If waiting for an RS response */
        /* XXX this accepts it even if stale - no abro version check. Problem? */
        /* XXX this section below smells like stuff that should be outside this function */
        if (router->nd_state == ND_RS_UNCAST || router->nd_state == ND_RS_MULTICAST) {
            /* Set uptodate to true so we accept data into our interface tables.
             * Consistent with previous behaviour, but is this what we want?
             */
            uptodate = true;
            router->flags = ra_flags;
            icmp_nd_set_next_hop(&router->default_hop, &buf->src_sa);
            router->default_hop.LQI = buf->options.lqi;

            icmp_nd_prefixs_parse(buf, router, cur);
            icmp_nd_context_parse(buf, router);

            if (router->nd_state == ND_RS_MULTICAST) {
                if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
                    router->nd_timer = 1;
                } else {
                    router->nd_timer = nd_params.rs_retry_interval_min;
                }
                tr_debug("RS MC Done");
            } else {
                router->nd_timer = 1;
                tr_debug("RS Unicast Done");
                mac_data_poll_protocol_poll_mode_decrement(cur);
            }
            router->ns_retry = nd_params.ns_retry_max;
            router->nd_state = ND_READY;
            router->nd_re_validate = (router_lifetime / 5) * 4;
        }
    }

    if (uptodate) {
        router->abro_version_num = abro_ver_num;
        router->life_time = router_lifetime;
    }

    return uptodate;
}

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
void nd_ra_process_lowpan_context_option(protocol_interface_info_entry_t *cur, const uint8_t *opt)
{
    uint8_t opt_len = opt[1];
    uint8_t ctx_len = opt[2];
    uint8_t cid_flags = opt[3];
    uint16_t lifetime = common_read_16_bit(opt + 6);

    if (ctx_len > 128 || ctx_len > (opt_len - 1) * 64) {
        return;
    }

    lowpan_context_update(&cur->lowpan_contexts, cid_flags, lifetime, opt + 8, ctx_len, true);
}
#ifdef HAVE_6LOWPAN_ROUTER
static void nd_ra_build(nd_router_t *cur, const uint8_t *address, protocol_interface_info_entry_t *cur_interface)
{
    if (!(cur_interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY) || !icmp_nd_router_prefix_valid(cur)) {
        return;
    }

    /* Base buffer size: RA Hdr = 12, ABRO = 24, MTU = up to 8, SLLAO = up to 16 */
    uint16_t length = 12 + 24 + 8 + 16;
    length += 32 * ns_list_count(&cur->prefix_list);

    ns_list_foreach(lowpan_context_t, context_ptr, &cur->context_list) {
        length += (context_ptr->length <= 64) ? 16 : 24;
    }

    buffer_t *db = buffer_get(length);
    if (!db) {
        return;
    } else if (addr_interface_get_ll_address(cur_interface, db->src_sa.address, 0) != 0) {
        buffer_free(db);
        return;
    }
    db->src_sa.addr_type = ADDR_IPV6;

    uint8_t *dptr = buffer_data_pointer(db);
    *dptr++ = cur_interface->adv_cur_hop_limit;
    *dptr++ = cur->flags;
    dptr = common_write_16_bit(cur->life_time, dptr);
    dptr = common_write_32_bit(cur_interface->adv_reachable_time, dptr);
    dptr = common_write_32_bit(cur_interface->adv_retrans_timer, dptr);

    //SET ABRO
    *dptr++ = ICMPV6_OPT_AUTHORITATIVE_BORDER_RTR;
    *dptr++ = 3;        //Len * 8 byte

    dptr = common_write_16_bit((uint16_t)cur->abro_version_num, dptr);
    dptr = common_write_16_bit((uint16_t)(cur->abro_version_num >> 16), dptr);
    dptr = common_write_16_bit(0, dptr);
    memcpy(dptr, cur->border_router, 16);
    dptr += 16;
    //SET Prefixs
    dptr = icmpv6_write_prefix_option(&cur->prefix_list, dptr, 0, cur_interface);

    ns_list_foreach(lowpan_context_t, context_ptr, &cur->context_list) {
        *dptr++ = ICMPV6_OPT_6LOWPAN_CONTEXT;
        *dptr++ = (context_ptr->length <= 64) ? 2 : 3;
        *dptr++ = context_ptr->length;
        *dptr++ = context_ptr->cid | (context_ptr->compression ? LOWPAN_CONTEXT_C : 0);
        dptr = common_write_16_bit(0, dptr);
        dptr = common_write_16_bit((context_ptr->lifetime + 599) / 600, dptr);
        length = (context_ptr->length <= 64) ? 8 : 16;
        memcpy(dptr, context_ptr->prefix, length);
        dptr += length;
    }

    //MTU
    if (cur_interface->adv_link_mtu != 0) {
        dptr = icmpv6_write_mtu_option(cur_interface->adv_link_mtu, dptr);
    }

    /* RFC 6775 mandates SLLAO in RA */
    dptr = icmpv6_write_icmp_lla(cur_interface, dptr, ICMPV6_OPT_SRC_LL_ADDR, true, db->src_sa.address);

    if (address) {
        memcpy(db->dst_sa.address, address, 16);
    } else {
        memcpy(db->dst_sa.address, ADDR_LINK_LOCAL_ALL_NODES, 16);
    }
    buffer_data_end_set(db, dptr);


    db->dst_sa.addr_type = ADDR_IPV6;
    db->interface = cur_interface;
    db->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
    db->options.type = ICMPV6_TYPE_INFO_RA;
    db->options.code = 0;
    db->options.hop_limit = 255;
    arm_net_protocol_packet_handler(db, cur_interface);

}

void nd_ra_build_by_abro(const uint8_t *abro, const uint8_t *dest, protocol_interface_info_entry_t *cur_interface)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (addr_ipv6_equal(cur->border_router, abro)) {
            nd_ra_build(cur, dest, cur_interface);
        }
    }
}


void nd_trigger_ras_from_rs(const uint8_t *unicast_adr, protocol_interface_info_entry_t *cur_interface)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (cur->nwk_id == cur_interface->nwk_id) {
            if (icmp_nd_router_prefix_valid(cur)) {
                //Allocate
                icmpv6_trigger_ra_from_rs(cur_interface, unicast_adr, cur->border_router);
            }
        } else {
            tr_error("BIND_CONFIRM FAIL!!");
        }
    }
}

void nd_ns_forward_timer_reset(uint8_t *root_adr)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (memcmp(root_adr, cur->border_router, 16) == 0) {
            if (cur->ns_forward_timer) {
                cur->ns_forward_timer = 0;
                tr_warn("RX VALID DAC");
            }
            break;
        }
    }
}

static void nd_router_forward_timer(nd_router_t *cur, uint16_t ticks_update)
{
    protocol_interface_info_entry_t *cur_interface;
    if (!(cur->ns_forward_timer)) {
        return;
    }

    if (cur->ns_forward_timer > ticks_update) {
        cur->ns_forward_timer -= ticks_update;
        return;
    }

    cur->ns_forward_timer = 0;
    cur_interface = protocol_stack_interface_info_get(cur->nwk_id);
    if (cur_interface) {
        if (cur->nd_re_validate > 10) {
            tr_debug("TRIG NS/ND");
            cur->nd_timer = 1;
            cur->nd_re_validate = 1;
        }
        if (cur_interface->if_6lowpan_dad_process.active == false) {
            nd_ns_trig(cur, cur_interface);
        }
    }
}

ipv6_ra_timing_t *nd_ra_timing(const uint8_t abro[16])
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (addr_ipv6_equal(abro, cur->border_router)) {
            return &cur->ra_timing;
        }
    }

    return NULL;
}

static nd_router_t *nd_router_object_scan_by_prefix(const uint8_t *ptr, nwk_interface_id nwk_id)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (cur->nwk_id == nwk_id) {
            if (icmpv6_prefix_compare(&cur->prefix_list, ptr, 64)) {
                return cur;
            }
        }
    }

    return NULL;
}

#endif

void gp_address_add_to_end(gp_ipv6_address_list_t *list, const uint8_t address[static 16])
{
    gp_ipv6_address_entry_t *new_entry;

    ns_list_foreach(gp_ipv6_address_entry_t, cur, list) {
        if (memcmp(cur->address, address, 16) == 0) {
            return;
        }
    }

    new_entry = ns_dyn_mem_alloc(sizeof(gp_ipv6_address_entry_t));
    if (!new_entry) {
        tr_warn("No heap for New Address");
        return;
    }

    memcpy(new_entry->address, address, 16);
    ns_list_add_to_end(list, new_entry);
}

/* Returns 1 if the router object has been removed */
static uint8_t nd_router_ready_timer(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t ticks_update)
{
    if (!cur->nd_timer) {
        return 0;
    }

    if (cur->nd_timer > ticks_update) {
        cur->nd_timer -= ticks_update;
        return 0;
    }

    //Take out last remaing time from ticks
    ticks_update -= cur->nd_timer;
    uint16_t updated_seconds = 1;
    cur->nd_timer = 10;
    if (ticks_update) {
        updated_seconds += (ticks_update / 10);
        //Set Next second based on over based time
        cur->nd_timer -= (ticks_update % 10);
    }

    if (icmp_nd_router_prefix_ttl_update(cur, cur_interface, updated_seconds)) {
        return 1;
    }

    //Update seconds
    icmp_nd_router_context_ttl_update(cur, updated_seconds);

    if (!cur->nd_re_validate) {
        return 0;
    }

    if (cur->nd_re_validate > updated_seconds) {
        cur->nd_re_validate -= updated_seconds;
        //tr_debug("NDR:Tick Update %u", cur->nd_re_validate);
        return 0;
    }

    if (cur->nd_state == ND_READY) {
        tr_debug("RE ND Process: RS Unicast!");
        cur->ns_retry = nd_params.rs_retry_max;
        cur->nd_state = ND_RS_UNCAST;
        set_power_state(ICMP_ACTIVE);
        cur->nd_timer = 1;
        cur->nd_bootstrap_tick = (nd_base_tick - 1);
        if (cur_interface->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {
            mac_data_poll_init_protocol_poll(cur_interface);
        }
        nd_router_bootstrap_timer(cur, cur_interface, 1);
    } else { /* ND_BR_READY */
        nd_border_router_setup_refresh(cur->nwk_id, true);
        tr_debug("ND BR refresh ABRO");
        cur->nd_re_validate = (cur->life_time / 4) * 3;
        icmpv6_restart_router_advertisements(cur_interface, cur->border_router);
    }
    return 0;
}

/* Returns 1 if the router object has been removed, or we want no further processing on this tick */
static uint8_t nd_router_bootstrap_timer(nd_router_t *cur, protocol_interface_info_entry_t *cur_interface, uint16_t ticks)
{
    uint16_t scaled_ticks;
    /*
     * nd_timer is scaled by nd_base_tick during the discovery states,
     * to allow API to slow down the ND process. Note we count up and test
     * inequality, just in case someone decides to change nd_base_tick on
     * the fly.
     */
    if (cur->nd_bootstrap_tick + ticks < nd_base_tick) {
        cur->nd_bootstrap_tick += ticks;
        return 0;
    }

    //Take off scaled ticks
    ticks -= (nd_base_tick - cur->nd_bootstrap_tick);

    scaled_ticks = 1 + (ticks / nd_base_tick);

    cur->nd_bootstrap_tick = 0 + (ticks % nd_base_tick);

    if (!cur->nd_timer) {
        tr_debug("NDB:Tick Update fail %u", scaled_ticks);
        return 0;
    }


    if (cur->nd_timer > scaled_ticks) {
        cur->nd_timer -= scaled_ticks;
        return 0;
    }

    switch (cur->nd_state) {

        case ND_RS_UNCAST:
        case ND_RS_MULTICAST:
            if (cur->ns_retry) {
                if (nd_rs_build(cur->nd_state == ND_RS_UNCAST ? cur : NULL, cur_interface)) {
                    cur->nd_timer = nd_params.rs_retry_interval_min;
                    cur->nd_timer += randLIB_get_16bit() & nd_params.timer_random_max;
                    cur->ns_retry--;
                    tr_debug(cur->nd_state == ND_RS_UNCAST ? "RS" : "RS+");
                } else {
                    cur->nd_timer = 2;
                }
            } else {
                //ND FAIL
                if (cur->nd_state == ND_RS_UNCAST) {
                    cur->ns_retry = nd_params.rs_retry_max;
                    cur->nd_state = ND_RS_MULTICAST;
                    cur->nd_timer = 1;
                } else {
                    //RS UNICAST Fail
                    /*if (rpl_object_poisons() == 0) ??? */ {
                        protocol_6lowpan_bootstrap_re_start(cur_interface);
                    }
                    return 1;
                }
            }
            break;

        case ND_READY:
        case ND_BR_READY:
            /* Not called for these states - put in to suppress GCC warning */
            break;
    }

    return 0;
}


void nd_object_timer(protocol_interface_info_entry_t *cur_interface, uint16_t ticks_update)
{
    ns_list_foreach_safe(nd_router_t, cur, &nd_router_list) {
        /* This may nd_router_remove(cur), so need to use safe loop */
        if (cur_interface->nwk_id == cur->nwk_id) {

            protocol_6lowpan_link_advertise_handle(cur, cur_interface, ticks_update);

            if (cur->nd_state != ND_BR_READY) {
                nd_router_forward_timer(cur, ticks_update);
            }

            if (nd_is_ready_state(cur->nd_state)) {
                nd_router_ready_timer(cur, cur_interface, ticks_update);
            } else {
                nd_router_bootstrap_timer(cur, cur_interface, ticks_update);
            }
            return;
        }
    }
}

uint32_t nd_object_time_to_next_nd_reg(void)
{
    uint32_t ret_val = 0;
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (cur->nd_state == ND_READY) {
            ret_val = cur->nd_re_validate;
            ret_val++;
            ret_val *= 1000;
            /* XXX surely this should find the shortest, not the first? */
            break;
        }
    }
    return ret_val;
}

uint8_t nd_prefix_dst_check(uint8_t *ptr)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (icmpv6_prefix_compare(&cur->prefix_list, ptr, 64)) {
            return 1;
        }
    }
    return 0;
}


int8_t nd_parent_loose_indcate(uint8_t *neighbor_address, protocol_interface_info_entry_t *cur_interface)
{
    int8_t ret_val = -1;
    addrtype_t adr_type = ADDR_802_15_4_LONG;
    uint8_t *adr_ptr = neighbor_address;
    nd_router_next_hop *hop;
    uint8_t compare_len = 8;

    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (!(cur_interface->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
            continue;
        }

        hop = &cur->default_hop;
        if (memcmp(neighbor_address, ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
            adr_ptr += 6;
            adr_type = ADDR_802_15_4_SHORT;
            compare_len = 2;
        }
        if (hop->addrtype == adr_type) {
            if (memcmp(hop->address, adr_ptr, compare_len) == 0) {
                tr_debug("ND Primary Parent Lost");

                if (cur->secondaty_hop == 0) {
                    ret_val = -1;
                } else {
                    ret_val = 0;
                    tr_debug("Swap Secondary to primary");
                    memcpy(hop, cur->secondaty_hop, sizeof(nd_router_next_hop));
                    tr_debug("Trig NS/NA with new parent");
                    if (cur->nd_state == ND_READY) {
                        cur->nd_re_validate = 1;
                        if (cur_interface->if_6lowpan_dad_process.active == false) {
                            nd_ns_trig(cur, cur_interface);
                        }
                    }
                    ns_dyn_mem_free(cur->secondaty_hop);
                    cur->secondaty_hop = 0;
                }
                return ret_val;
            }
        } else if (cur->secondaty_hop) {
            hop = cur->secondaty_hop;
            if (hop->addrtype == adr_type) {
                if (memcmp(hop->address, adr_ptr, compare_len) == 0) {
                    tr_debug("ND Secondary Parent Lost");
                    ns_dyn_mem_free(cur->secondaty_hop);
                    cur->secondaty_hop = 0;
                    return 0;
                }
            }
        }
    }
    return -1;
}

nd_router_t *nd_get_object_by_nwk_id(nwk_interface_id nwk_id)
{
    ns_list_foreach(nd_router_t, cur, &nd_router_list) {
        if (cur->nwk_id == nwk_id) {
            return cur;
        }
    }

    return 0;
}

nd_router_t *nd_get_pana_address(void)
{
    return ns_list_get_first(&nd_router_list);
}

void nd_6lowpan_set_radv_params(protocol_interface_info_entry_t *cur_interface)
{
#ifndef NO_RADV_TX
    cur_interface->max_ra_delay_time = 20;
    cur_interface->min_delay_between_ras = 100;
    cur_interface->rtr_adv_unicast_to_rs = true;
    cur_interface->adv_cur_hop_limit = nd_params.ra_cur_hop_limit;
    cur_interface->adv_link_mtu = nd_params.ra_link_mtu;
    cur_interface->adv_reachable_time = nd_params.ra_reachable_time;
    cur_interface->adv_retrans_timer = nd_params.ra_retrans_timer;
    cur_interface->max_initial_rtr_adv_interval = nd_params.ra_interval_min;
    cur_interface->max_initial_rtr_advertisements = nd_params.ra_transmits;
#endif
}
#endif // HAVE_6LOWPAN_ND

