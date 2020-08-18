/*
 * Copyright (c) 2008, 2010-2019, Arm Limited and affiliates.
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
/**
 * \file ns_address_internal.c
 * \brief Utility functions concerning addresses
 *
 * This file contains all the utility functions that can be used to
 * check, manipulate etc. addresses.
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "ip6string.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_sha256.h"
#include "socket_api.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/mld.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "common_functions.h"

#include "RPL/rpl_control.h"

#define TRACE_GROUP "addr"

typedef struct addr_notification {
    if_address_notification_fn *fn;
    ns_list_link_t link;
} addr_notification_t;

static NS_LIST_DEFINE(addr_notifications, addr_notification_t, link);

typedef struct addr_policy_table_entry_t {
    uint8_t prefix[16];
    uint8_t prefix_len;
    uint8_t precedence;
    uint8_t label;
    ns_list_link_t link;
} addr_policy_table_entry_t;

static NS_LIST_DEFINE(addr_policy_table, addr_policy_table_entry_t, link);

uint32_t addr_preferences_default = SOCKET_IPV6_PREFER_SRC_TMP | SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;

static void addr_policy_table_reset(void);
static void addr_max_entries_check(protocol_interface_info_entry_t *cur, if_address_source_t source);

const uint8_t ADDR_LINK_LOCAL_PREFIX[8]         = { 0xfe, 0x80 };
const uint8_t ADDR_SHORT_ADR_SUFFIC[6]          = { 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00};

const uint8_t ADDR_MULTICAST_SOLICITED[13]      = { 0xff, 0x02, [11] = 0x01, 0xff};
const uint8_t ADDR_IF_LOCAL_ALL_NODES[16]       = { 0xff, 0x01, [15] = 0x01 };
const uint8_t ADDR_IF_LOCAL_ALL_ROUTERS[16]     = { 0xff, 0x01, [15] = 0x02 };
const uint8_t ADDR_LINK_LOCAL_ALL_NODES[16]     = { 0xff, 0x02, [15] = 0x01 };
const uint8_t ADDR_LINK_LOCAL_ALL_ROUTERS[16]   = { 0xff, 0x02, [15] = 0x02 };
const uint8_t ADDR_LINK_LOCAL_ALL_MLDV2_ROUTERS[16] = { 0xff, 0x02, [15] = 0x16 };
const uint8_t ADDR_LINK_LOCAL_MDNS[16]          = { 0xff, 0x02, [15] = 0xfb };
const uint8_t ADDR_REALM_LOCAL_ALL_NODES[16]    = { 0xff, 0x03, [15] = 0x01 };
const uint8_t ADDR_REALM_LOCAL_ALL_ROUTERS[16]  = { 0xff, 0x03, [15] = 0x02 };
const uint8_t ADDR_SITE_LOCAL_ALL_ROUTERS[16]   = { 0xff, 0x05, [15] = 0x02 };
const uint8_t ADDR_ALL_MPL_FORWARDERS[16]       = { 0xff, 0x03, [15] = 0xfc };
const uint8_t ADDR_ALL_DHCP_RELAY_AGENTS_AND_SERVERS[16] = { 0xff, 0x02, [13] = 0x01, 0x00, 0x02 };

const uint8_t ADDR_IPV4_MAPPED_PREFIX[12]       = { [10] = 0xff, 0xff };
const uint8_t ADDR_LOOPBACK[16]                 = { [15] = 1 };
const uint8_t ADDR_UNSPECIFIED[16]              = { 0 };        /* Note a few bits of code check for pointer equality with ADDR_UNSPECIFIED */
#define ADDR_IPV4_COMPATIBLE                    ADDR_LOOPBACK /* First 96 bits match...*/

#define ADDR_MULTICAST_LINK_PREFIX              ADDR_LINK_LOCAL_ALL_NODES /* ff02::xx */
#define ADDR_MULTICAST_REALM_PREFIX             ADDR_ALL_MPL_FORWARDERS /* ff03::xx */

static const uint8_t *addr_iid_secret_key;
static const uint8_t *addr_initial_iid;
static uint8_t addr_iid_secret_key_len;

static bool addr_am_implicit_group_member(const uint8_t group[static 16])
{
    static const uint8_t *const implicit_groups[] = {
        ADDR_LINK_LOCAL_ALL_NODES,
        ADDR_IF_LOCAL_ALL_NODES,
    };

    for (uint_fast8_t i = 0; i < sizeof implicit_groups / sizeof implicit_groups[0]; i++) {
        if (addr_ipv6_equal(implicit_groups[i], group)) {
            return true;
        }
    }
    return false;
}

uint8_t addr_len_from_type(addrtype_t addr_type)
{
    switch (addr_type) {
        case ADDR_NONE:
            return 0;
        case ADDR_802_15_4_SHORT:
            return 2 + 2; /* Some users don't have the PAN ID */
        case ADDR_802_15_4_LONG:
            return 2 + 8;
        case ADDR_EUI_48:
            return 6;
        case ADDR_IPV6:
            return 16;
        case ADDR_BROADCAST:
            return 0; /* Don't really handle this */
    }
    return 0;
}

/**
 * Check if an address is a broadcast address
 *
 * \param addr pointer to an address_t containing the address to be checked
 * \param addr_type the type of the address_t
 * \return 0 if the address is a broadcast address
 */
uint8_t addr_check_broadcast(const address_t addr, addrtype_t addr_type)
{
    switch (addr_type) {
        case ADDR_802_15_4_SHORT:
            break;
        case ADDR_BROADCAST:
            return 0;
        default:
            return 1;
    }

    uint8_t size = 2;
    uint8_t *ptr = (uint8_t *) addr;
    ptr += 2;

    while (size) {
        if ((*ptr++) != 0xFF) {
            break;
        } else {
            size--;
        }
    }
    return (size);

}

bool addr_is_ipv6_link_local(const uint8_t addr[static 16])
{
    return addr[0] == 0xfe && (addr[1] & 0xc0) == 0x80;
}

/* Site-local addresses deprecated, but still processed by RFC 6724 address selection */
static bool addr_is_ipv6_site_local(const uint8_t addr[static 16])
{
    return addr[0] == 0xfe && (addr[1] & 0xc0) == 0xc0;
}

static bool addr_is_ipv4_mapped(const uint8_t addr[static 16])
{
    return memcmp(addr, ADDR_IPV4_MAPPED_PREFIX, 12) == 0;
}

/* Scope(A), as defined in RFC 6724 plus RFC 4007 */
uint_fast8_t addr_ipv6_scope(const uint8_t addr[static 16], const protocol_interface_info_entry_t *interface)
{
#ifndef HAVE_THREAD
    (void)interface;
#endif
    if (addr_is_ipv6_multicast(addr)) {
        return addr_ipv6_multicast_scope(addr);
    }
    if (addr_is_ipv6_link_local(addr) || addr_is_ipv6_loopback(addr)) {
        return IPV6_SCOPE_LINK_LOCAL;
    }
    if (addr_is_ipv6_site_local(addr)) {
        return IPV6_SCOPE_SITE_LOCAL;
    }
    /* Thread-specific hack - we want to treat one specific "mesh local" ULA as
     * "realm local" scope. (This is necessary, for example, to use a
     * mesh-local ULA address as source when sending realm-local multicast).
     */
    if (thread_addr_is_mesh_local(addr, interface)) {
        return IPV6_SCOPE_REALM_LOCAL;
    }
    if (addr_is_ipv4_mapped(addr)) {
        if ((addr[12] == 169 && addr[13] == 254) || addr[12] == 127) {
            return IPV6_SCOPE_LINK_LOCAL;
        }
        return IPV6_SCOPE_GLOBAL;
    }
    return IPV6_SCOPE_GLOBAL;
}

void address_module_init(void)
{
    addr_policy_table_reset();
    //mac_reset_short_address();
}

#ifdef MULTICAST_FORWARDING
static if_group_fwd_entry_t *addr_multicast_fwd_list_lookup(if_group_fwd_list_t *list, const uint8_t group[16])
{
    ns_list_foreach(if_group_fwd_entry_t, e, list) {
        if (addr_ipv6_equal(e->group, group)) {
            return e;
        }
    }
    return NULL;
}

bool addr_multicast_fwd_check(protocol_interface_info_entry_t *interface, const uint8_t group[16])
{
    return addr_multicast_fwd_list_lookup(&interface->ip_groups_fwd, group) != NULL;
}

static void addr_multicast_fwd_adjust_upstream(protocol_interface_info_entry_t *downstream, protocol_interface_info_entry_t *upstream, const uint8_t group[16], bool add)
{
    if (!upstream || downstream == upstream || !downstream->ip_multicast_forwarding) {
        return;
    }
    uint8_t group_scope = addr_ipv6_multicast_scope(group);
    if (downstream->zone_index[group_scope] == upstream->zone_index[group_scope]) {
        tr_debug("Multicast proxy %s %s", add ? "add" : "remove", trace_ipv6(group));
        if (add) {
            addr_add_group(upstream, group);
        } else {
            addr_remove_group(upstream, group);
        }
    }
}

void addr_multicast_fwd_adjust_upstream_full(protocol_interface_info_entry_t *upstream, bool add)
{
    ns_list_foreach(protocol_interface_info_entry_t, interface, &protocol_interface_info_list) {
        ns_list_foreach(if_group_fwd_entry_t, group, &interface->ip_groups_fwd) {
            addr_multicast_fwd_adjust_upstream(interface, upstream, group->group, add);
        }
    }
}

void addr_multicast_fwd_set_forwarding(struct protocol_interface_info_entry *interface, bool enable)
{
    /* Do nothing if enable state isn't changing */
    if (interface->ip_multicast_forwarding == enable) {
        return;
    }

    /* If we disable forwarding on upstream, clear it out to avoid maintenance confusion */
    if (!enable && interface == protocol_core_multicast_upstream) {
        multicast_fwd_set_proxy_upstream(-1);
    }

    /* Adjust routine checks that forwarding is on before doing any processing. So make sure it's on
     * now, then we'll set requested state afterwards. */
    interface->ip_multicast_forwarding = true;

    /* Add or remove all groups on changing downstream interface to upstream */
    if (protocol_core_multicast_upstream) {
        ns_list_foreach(if_group_fwd_entry_t, group, &interface->ip_groups_fwd) {
            addr_multicast_fwd_adjust_upstream(interface, protocol_core_multicast_upstream, group->group, enable);
        }
    }

    interface->ip_multicast_forwarding = enable;
}

bool addr_multicast_fwd_add(protocol_interface_info_entry_t *interface, const uint8_t group[16], uint32_t lifetime)
{
    if_group_fwd_entry_t *entry = addr_multicast_fwd_list_lookup(&interface->ip_groups_fwd, group);
    if (entry) {
        if (entry->lifetime < lifetime) {
            entry->lifetime = lifetime;
        }
        return true;
    }
    entry = ns_dyn_mem_alloc(sizeof * entry);
    if (!entry) {
        return false;
    }
    memcpy(entry->group, group, 16);
    ns_list_add_to_end(&interface->ip_groups_fwd, entry);
    addr_multicast_fwd_adjust_upstream(interface, protocol_core_multicast_upstream, group, true);
    entry->lifetime = lifetime;
    tr_debug("MC fwd added to IF %d: %s", interface->id, trace_ipv6(group));
    return true;
}

static void addr_multicast_fwd_delete_entry(protocol_interface_info_entry_t *interface, if_group_fwd_entry_t *entry)
{
    addr_multicast_fwd_adjust_upstream(interface, protocol_core_multicast_upstream, entry->group, false);
    ns_list_remove(&interface->ip_groups_fwd, entry);
    ns_dyn_mem_free(entry);
}

bool addr_multicast_fwd_remove(protocol_interface_info_entry_t *interface, const uint8_t group[16])
{
    if_group_fwd_entry_t *entry = addr_multicast_fwd_list_lookup(&interface->ip_groups_fwd, group);
    if (!entry) {
        return false;
    }

    tr_debug("MC fwd removed from IF %d: %s", interface->id, trace_ipv6(group));
    addr_multicast_fwd_delete_entry(interface, entry);

    return true;
}
#endif // MULTICAST_FORWARDING

int_fast8_t addr_policy_table_add_entry(const uint8_t *prefix, uint8_t len, uint8_t precedence, uint8_t label)
{
    addr_policy_table_entry_t *entry = ns_dyn_mem_alloc(sizeof(addr_policy_table_entry_t));
    if (!entry) {
        return -1;
    }
    bitcopy(entry->prefix, prefix, len);
    entry->prefix_len = len;
    entry->precedence = precedence;
    entry->label = label;

    /* Table is sorted longest-prefix-first, for longest-match searching */
    bool inserted = false;
    ns_list_foreach(addr_policy_table_entry_t, before, &addr_policy_table) {
        if (before->prefix_len > len) {
            continue;
        }
        if (before->prefix_len == len && bitsequal(before->prefix, prefix, len)) {
            ns_dyn_mem_free(entry);
            return -2;
        }
        ns_list_add_before(&addr_policy_table, before, entry);
        inserted = true;
        break;
    }

    if (!inserted) {
        ns_list_add_to_end(&addr_policy_table, entry);
    }

    return 0;
}

int_fast8_t addr_policy_table_delete_entry(const uint8_t *prefix, uint8_t len)
{
    ns_list_foreach(addr_policy_table_entry_t, entry, &addr_policy_table) {
        if (entry->prefix_len == len && bitsequal(entry->prefix, prefix, len)) {
            ns_list_remove(&addr_policy_table, entry);
            ns_dyn_mem_free(entry);
            return 0;
        }
    }

    return -1;
}
/// @TODO do we need this test print anymore ?
void addr_policy_table_print(void)
{
    ns_list_foreach(addr_policy_table_entry_t, entry, &addr_policy_table) {
        char addr[40];
        ip6tos(entry->prefix, addr);
        tr_debug("%3d %3d %s/%u\n", entry->precedence, entry->label, addr, entry->prefix_len);
    }
}

static void addr_policy_table_reset(void)
{
    ns_list_foreach_safe(addr_policy_table_entry_t, entry, &addr_policy_table) {
        ns_list_remove(&addr_policy_table, entry);
        ns_dyn_mem_free(entry);
    }

    /* Default policy table from RFC 6724 */
    addr_policy_table_add_entry(ADDR_LOOPBACK,                         128, 50, 0);
    addr_policy_table_add_entry(NULL,                                    0, 40, 1);
    addr_policy_table_add_entry(ADDR_IPV4_MAPPED_PREFIX,                96, 35,  4);
    addr_policy_table_add_entry((const uint8_t[]) {
        0x20, 0x02
    },       16, 30,  2);
    addr_policy_table_add_entry((const uint8_t[]) {
        0x20, 0x01, 0, 0
    }, 32,  5,  5);
    addr_policy_table_add_entry((const uint8_t[]) {
        0xfc
    },              7,  3, 13);
    addr_policy_table_add_entry(ADDR_IPV4_COMPATIBLE,                   96,  1,  3);
    addr_policy_table_add_entry((const uint8_t[]) {
        0xfe, 0xc0
    },       10,  1, 11);
    addr_policy_table_add_entry((const uint8_t[]) {
        0x3f, 0xfe
    },       16,  1, 12);
    //addr_policy_table_print();
}

static const addr_policy_table_entry_t *addr_get_policy(const uint8_t addr[static 16])
{
    ns_list_foreach(const addr_policy_table_entry_t, entry, &addr_policy_table) {
        if (bitsequal(entry->prefix, addr, entry->prefix_len)) {
            return entry;
        }
    }

    /* Shouldn't happen - should always have a default entry */
    return NULL;
}

/* RFC 6724 CommonPrefixLen(S, D) */
static uint_fast8_t addr_common_prefix_len(const uint8_t src[static 16], uint_fast8_t src_prefix_len, const uint8_t dst[static 16])
{
    uint_fast8_t common = 0;

    while (src_prefix_len >= 8 && *src == *dst) {
        common += 8;
        src_prefix_len -= 8;
        ++src;
        ++dst;
    }

    if (src_prefix_len) {
        uint8_t trail = common_count_leading_zeros_8(*src ^ *dst);
        if (trail > src_prefix_len) {
            trail = src_prefix_len;
        }
        common += trail;
    }

    return common;
}

if_address_entry_t *addr_get_entry(const protocol_interface_info_entry_t *interface, const uint8_t addr[static 16])
{
    ns_list_foreach(if_address_entry_t, entry, &interface->ip_addresses) {
        if (addr_ipv6_equal(entry->address, addr)) {
            return entry;
        }
    }

    return NULL;
}

bool addr_is_assigned_to_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[static 16])
{
    if_address_entry_t *entry = addr_get_entry(interface, addr);
    return entry && !entry->tentative;
}

bool addr_is_tentative_for_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[static 16])
{
    if_address_entry_t *entry = addr_get_entry(interface, addr);
    return entry && entry->tentative;
}

if_group_entry_t *addr_add_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    if_group_entry_t *entry = addr_get_group_entry(interface, group);
    if (entry) {
        if (entry->ref_count != 0xFFFF) {
            entry->ref_count++;
        }
        return entry;
    }

    if (!addr_is_ipv6_multicast(group)) {
        return NULL;
    }

    if (addr_am_implicit_group_member(group)) {
        return NULL;
    }

    entry = ns_dyn_mem_alloc(sizeof(if_group_entry_t));
    if (!entry) {
        return NULL;
    }
    memcpy(entry->group, group, 16);
    entry->ref_count = 1;
    ns_list_add_to_end(&interface->ip_groups, entry);

    mld_start_listening(interface, entry);

    return entry;
}

/* This does reference count */
void addr_remove_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    if_group_entry_t *entry = addr_get_group_entry(interface, group);
    if (entry) {
        if (entry->ref_count != 0xFFFF) {
            if (--entry->ref_count == 0) {
                addr_delete_group_entry(interface, entry);
            }
        }
    }
}

/* This does NOT reference count - it actually deletes the entry */
void addr_delete_group_entry(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
    mld_stop_listening(interface, entry);
    ns_list_remove(&interface->ip_groups, entry);
    ns_dyn_mem_free(entry);
}

void addr_delete_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    if_group_entry_t *entry = addr_get_group_entry(interface, group);
    if (entry) {
        addr_delete_group_entry(interface, entry);
    }
}

if_group_entry_t *addr_get_group_entry(const protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    ns_list_foreach(if_group_entry_t, entry, &interface->ip_groups) {
        if (addr_ipv6_equal(entry->group, group)) {
            return entry;
        }
    }

    return NULL;
}

static void addr_generate_solicited_node_group(uint8_t group[static 16], const uint8_t addr[static 16])
{
    memcpy(group, ADDR_MULTICAST_SOLICITED, 13);
    memcpy(group + 13, addr + 13, 3);
}

static if_group_entry_t *addr_add_solicited_node_group(protocol_interface_info_entry_t *interface, const uint8_t address[static 16])
{
    uint8_t group[16];
    addr_generate_solicited_node_group(group, address);
    return addr_add_group(interface, group);
}

static void addr_remove_solicited_node_group(protocol_interface_info_entry_t *interface, const uint8_t address[static 16])
{
    uint8_t group[16];
    addr_generate_solicited_node_group(group, address);
    addr_remove_group(interface, group);
}

void addr_add_router_groups(protocol_interface_info_entry_t *interface)
{
    /* The standard IPv6 ones, but not "Realm-Local-All-Routers"
     * which is ZigBee IP / Thread-specific (and not IANA registered)
     */
    addr_add_group(interface, ADDR_IF_LOCAL_ALL_ROUTERS);
    addr_add_group(interface, ADDR_LINK_LOCAL_ALL_ROUTERS);

    /* We only want to join the site address on one interface per site zone,
     * or we'd get multiple copies of packets. Exit if we're already a member.
     */
    ns_list_foreach(protocol_interface_info_entry_t, i, &protocol_interface_info_list) {
        if (i->zone_index[IPV6_SCOPE_SITE_LOCAL] == interface->zone_index[IPV6_SCOPE_SITE_LOCAL] &&
                addr_get_group_entry(i, ADDR_SITE_LOCAL_ALL_ROUTERS)) {
            return;
        }
    }
    addr_add_group(interface, ADDR_SITE_LOCAL_ALL_ROUTERS);
}

bool addr_am_group_member_on_interface(const protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    return addr_am_implicit_group_member(group) || addr_get_group_entry(interface, group);
}

/* RFC 6724 Default source address selection */
const uint8_t *addr_select_source(protocol_interface_info_entry_t *interface, const uint8_t dest[static 16], uint32_t addr_preferences)
{
    /* Let's call existing preferred address "SA" and new candidate "SB", to
     * make it look like a bit like RFC 6724
     */
    if_address_entry_t *SA = NULL;
    uint_fast8_t scope_D = addr_ipv6_scope(dest, interface);
    const addr_policy_table_entry_t *policy_D = addr_get_policy(dest);

    if (addr_preferences == 0) {
        addr_preferences = addr_preferences_default;
    }

    /*
     * As we go around the loop, if we prefer the new "SB", we set SA to SB
     * and continue. If we decide we prefer the existing SA, we just continue.
     *
     * Careful with these macros - they must only be used with if/else, and
     * inside { }, as per the coding style rules.
     */
#define PREFER_SA continue
#define PREFER_SB SA = SB; continue

    ns_list_foreach(if_address_entry_t, SB, &interface->ip_addresses) {
        /* Ignore tentative addresses */
        if (SB->tentative) {
            continue;
        }

        /* First (non-tentative) address seen becomes SA */
        if (!SA) {
            PREFER_SB;
        }

        /* Rule 1: Prefer same address */
        if (memcmp(SB->address, dest, 16) == 0) {
            SA = SB;
            /* It's an exact match, no point checking any other addresses */
            break;
        }

        /* Rule 2: Prefer appropriate scope */
        uint_fast8_t scope_SA = addr_ipv6_scope(SA->address, interface);
        uint_fast8_t scope_SB = addr_ipv6_scope(SB->address, interface);
        if (scope_SA < scope_SB) {
            if (scope_SA < scope_D) {
                PREFER_SB;
            } else {
                PREFER_SA;
            }
        } else if (scope_SB < scope_SA) {
            if (scope_SB < scope_D) {
                PREFER_SA;
            } else {
                PREFER_SB;
            }
        }

        /* Rule 3: Avoid deprecated addresses */
        if (SA->preferred_lifetime != 0 && SB->preferred_lifetime == 0) {
            PREFER_SA;
        } else if (SB->preferred_lifetime != 0 && SA->preferred_lifetime == 0) {
            PREFER_SB;
        }

        /* (Rule 4: Prefer home addresses - Mobile IPv6 not implemented) */
        /* (Rule 5: Prefer outgoing interface - candidate set already limited) */
        /* (Rule 5.5: Prefer addresses in a prefix advertised by the next-hop - we don't track this information) */

        /* Rule 6: Prefer matching label */
        const addr_policy_table_entry_t *policy_SA = addr_get_policy(SA->address);
        const addr_policy_table_entry_t *policy_SB = addr_get_policy(SB->address);
        if (policy_SA->label == policy_D->label && policy_SB->label != policy_D->label) {
            PREFER_SA;
        } else if (policy_SB->label == policy_D->label && policy_SA->label != policy_D->label) {
            PREFER_SB;
        }

        /* Rule 7: Prefer temporary addresses (or the opposite) */
        if (SA->temporary != SB->temporary) {
            bool prefer_public = (addr_preferences & SOCKET_IPV6_PREFER_SRC_PUBLIC);

            if (SA->temporary != prefer_public) {
                PREFER_SA;
            } else {
                PREFER_SB;
            }
        }

        /* Rule 8: Use longest matching prefix */
        uint_fast8_t common_SA = addr_common_prefix_len(SA->address, SA->prefix_len, dest);
        uint_fast8_t common_SB = addr_common_prefix_len(SB->address, SB->prefix_len, dest);
        if (common_SA > common_SB) {
            PREFER_SA;
        } else if (common_SB > common_SA) {
            PREFER_SB;
        }

        /* A tie-breaker: Prefer 6LoWPAN short address (or the opposite) */
        bool short_SA = SA->prefix_len == 64 && memcmp(SA->address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0;
        bool short_SB = SB->prefix_len == 64 && memcmp(SB->address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0;
        if (short_SA != short_SB) {
            bool prefer_short = (addr_preferences & SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT);

            if (short_SA == prefer_short) {
                PREFER_SA;
            } else {
                PREFER_SB;
            }
        }

        /* Rule 9 select most precated one  */
        if (policy_SA->precedence > policy_SB->precedence) {
            PREFER_SA;
        } else if (policy_SB->precedence > policy_SA->precedence) {
            PREFER_SB;
        }

        /* Tie */
        PREFER_SA;
    }

    return SA ? SA->address : NULL;
}

/* A variant of RFC 6724 Default source address selection, to select an address
 * on an interface with a specific prefix. The prefix must match, and some
 * source address rules don't apply, but some are handled similarly. See
 * comments in addr_select_source.
 */
const uint8_t *addr_select_with_prefix(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, uint32_t addr_preferences)
{
    if_address_entry_t *SA = NULL;

    if (addr_preferences == 0) {
        addr_preferences = addr_preferences_default;
    }

    ns_list_foreach(if_address_entry_t, SB, &cur->ip_addresses) {
        /* Ignore tentative addresses */
        if (SB->tentative) {
            continue;
        }

        /* Prefix must match */
        if (!bitsequal(SB->address, prefix, prefix_len)) {
            continue;
        }

        /* First (non-tentative, matching prefix) address seen becomes SA */
        if (!SA) {
            PREFER_SB;
        }

        /* (Rule 1: Prefer same address - doesn't apply here) */
        /* Rule 2: Was prefer appropriate scope - for this purpose we instead prefer wider scope */
        uint_fast8_t scope_SA = addr_ipv6_scope(SA->address, cur);
        uint_fast8_t scope_SB = addr_ipv6_scope(SB->address, cur);
        if (scope_SA < scope_SB) {
            PREFER_SB;
        } else if (scope_SB < scope_SA) {
            PREFER_SA;
        }

        /* Rule 3: Avoid deprecated addresses */
        if (SA->preferred_lifetime != 0 && SB->preferred_lifetime == 0) {
            PREFER_SA;
        } else if (SB->preferred_lifetime != 0 && SA->preferred_lifetime == 0) {
            PREFER_SB;
        }

        /* (Rule 4: Prefer home addresses - Mobile IPv6 not implemented) */
        /* (Rule 5: Prefer outgoing interface - candidate set already limited) */
        /* (Rule 5.5: Prefer addresses in a prefix advertised by the next-hop - we don't track this information) */

        /* Rule 6: Prefer matching label - doesn't apply here. But instead,
         * let's use precedence, like rule 6 of destination selection.
         */
        const addr_policy_table_entry_t *policy_SA = addr_get_policy(SA->address);
        const addr_policy_table_entry_t *policy_SB = addr_get_policy(SB->address);
        if (policy_SA->precedence > policy_SB->precedence) {
            PREFER_SA;
        } else if (policy_SB->precedence > policy_SA->precedence) {
            PREFER_SB;
        }

        /* Rule 7: Prefer temporary addresses (or the opposite) */
        if (SA->temporary != SB->temporary) {
            bool prefer_public = (addr_preferences & SOCKET_IPV6_PREFER_SRC_PUBLIC);

            if (SA->temporary != prefer_public) {
                PREFER_SA;
            } else {
                PREFER_SB;
            }
        }

        /* (Rule 8: Use longest matching prefix - doesn't apply) */

        /* A tie-breaker: Prefer 6LoWPAN short address (or the opposite) */
        bool short_SA = SA->prefix_len == 64 && memcmp(SA->address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0;
        bool short_SB = SB->prefix_len == 64 && memcmp(SB->address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0;
        if (short_SA != short_SB) {
            bool prefer_short = (addr_preferences & SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT);

            if (short_SA == prefer_short) {
                PREFER_SA;
            } else {
                PREFER_SB;
            }
        }

        /* Tie */
        PREFER_SA;
    }

    return SA ? SA->address : NULL;
}

#undef PREFER_SA
#undef PREFER_SB

void addr_delete_entry(protocol_interface_info_entry_t *cur, if_address_entry_t *addr)
{
    if (addr->group_added) {
        addr_remove_solicited_node_group(cur, addr->address);
    }
    ns_list_remove(&cur->ip_addresses, addr);
    addr_cb(cur, addr, ADDR_CALLBACK_DELETED);
    ns_dyn_mem_free(addr);
}

/* ticks is in 1/10s */
void addr_fast_timer(protocol_interface_info_entry_t *cur, uint_fast16_t ticks)
{
    /* Fast timers only run while the interface is active. */
    if (!(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return;
    }

    ns_list_foreach_safe(if_address_entry_t, addr, &cur->ip_addresses) {
        if (addr->state_timer == 0) {
            continue;
        }

        if (addr->state_timer > ticks) {
            addr->state_timer -= ticks;
            continue;
        }

        addr->state_timer = 0;
        /* Advance DAD state machine if tentative, else give it to the protocol state machine */
        if (addr->tentative) {
            if (addr->count == 0) {
#if 0
                // Initial join delay finished
                // We don't have full MLD support - send 1 report for now
                // This will become a real "join" later
                buffer_t *mld_buf;
                uint8_t sol_addr[16];
                memcpy(sol_addr, ADDR_MULTICAST_SOLICITED, 13);
                memcpy(sol_addr + 13, addr->address + 13, 3);
                mld_buf = mld_build(cur, NULL, ICMPV6_TYPE_INFO_MCAST_LIST_REPORT, 0, sol_addr);
                protocol_push(mld_buf);
#endif
                if (addr_add_solicited_node_group(cur, addr->address)) {
                    addr->group_added = true;
                }
            }
#ifdef HAVE_IPV6_ND
            if (addr->count >= cur->dup_addr_detect_transmits) {
                /* Finished - if we've not been nerfed already, we can transition
                 * to non-tentative.
                 */
                addr->tentative = false;
                addr->count = 0;
                tr_info("DAD passed on IF %d: %s", cur->id, trace_ipv6(addr->address));
                addr_cb(cur, addr, ADDR_CALLBACK_DAD_COMPLETE);
            } else {
                buffer_t *ns_buf = icmpv6_build_ns(cur, addr->address, NULL, false, true, NULL);
                protocol_push(ns_buf);
                addr->state_timer = (cur->ipv6_neighbour_cache.retrans_timer + 50) / 100; // ms -> ticks
                addr->count++;
            }
#endif
        } else {
            addr_cb(cur, addr, ADDR_CALLBACK_TIMER);
        }

        /* If a callback has shut down the interface, break now - this isn't
         * just a nicety; it avoids an iteration failure if shutdown disrupted
         * the address list (as is likely).
         */
        if (!(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
            break;
        }
    }
}

void addr_slow_timer(protocol_interface_info_entry_t *cur, uint_fast16_t seconds)
{
    /* Slow (lifetime) timers run whether the interface is active or not */
    ns_list_foreach_safe(if_address_entry_t, addr, &cur->ip_addresses) {
        if (addr->preferred_lifetime != 0xffffffff && addr->preferred_lifetime != 0) {
            if (addr->preferred_lifetime > seconds) {
                addr->preferred_lifetime -= seconds;
            } else {
                tr_info("Address deprecated: %s", trace_ipv6(addr->address));
                addr->preferred_lifetime = 0;
                addr_cb(cur, addr, ADDR_CALLBACK_DEPRECATED);
            }
        }

        if (addr->valid_lifetime != 0xffffffff) {
            if (addr->valid_lifetime > seconds) {
                addr->valid_lifetime -= seconds;
            } else {
                tr_info("Address invalidated: %s", trace_ipv6(addr->address));
                addr->valid_lifetime = 0;
                addr_cb(cur, addr, ADDR_CALLBACK_INVALIDATED);
                /* Give them the chance to revalidate */
                if (addr->valid_lifetime == 0) {
                    addr_delete_entry(cur, addr);
                }
            }
        }
    }

#ifdef MULTICAST_FORWARDING
    ns_list_foreach_safe(if_group_fwd_entry_t, group, &cur->ip_groups_fwd) {
        if (group->lifetime != 0xffffffff) {
            if (group->lifetime > seconds) {
                group->lifetime -= seconds;
            } else {
                tr_debug("MC fwd expired: %s", trace_ipv6(group->group));
                addr_multicast_fwd_delete_entry(cur, group);
            }
        }
    }
#endif
}

static void addr_max_entries_check(protocol_interface_info_entry_t *cur, if_address_source_t source)
{
    // Limit only auto configuration addresses
    if (source != ADDR_SOURCE_SLAAC || cur->ip_addresses_max_slaac_entries == 0) {
        return;
    }

    uint8_t count = 0;
    if_address_entry_t *first_slaac_entry = NULL;
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == ADDR_SOURCE_SLAAC) {
            if (!first_slaac_entry) {
                first_slaac_entry = e;
            }
            if (++count == 0xff) {
                break;
            }
        }
    }

    if (count > cur->ip_addresses_max_slaac_entries) {
        addr_delete_entry(cur, first_slaac_entry);
    }
}

void addr_max_slaac_entries_set(protocol_interface_info_entry_t *cur, uint8_t max_slaac_entries)
{
    cur->ip_addresses_max_slaac_entries = max_slaac_entries;
}

if_address_entry_t *addr_add(protocol_interface_info_entry_t *cur, const uint8_t address[static 16], uint_fast8_t prefix_len, if_address_source_t source, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad)
{
    if (addr_get_entry(cur, address)) {
        return NULL;
    }

    addr_max_entries_check(cur, source);

    if_address_entry_t *entry = ns_dyn_mem_alloc(sizeof(if_address_entry_t));
    if (!entry) {
        return NULL;
    }

    memset(entry, 0, sizeof * entry);
    entry->cb = NULL;
    memcpy(entry->address, address, 16);
    entry->prefix_len = prefix_len;
    entry->source = source;
    entry->valid_lifetime = valid_lifetime;
    entry->preferred_lifetime = preferred_lifetime;
    entry->group_added = false;
#ifndef HAVE_IPV6_ND
    skip_dad = true;
#endif
    if (skip_dad || cur->dup_addr_detect_transmits == 0) {
        entry->tentative = false;
        if (addr_add_solicited_node_group(cur, entry->address)) {
            entry->group_added = true;
        }
        // XXX not right? Want to do delay + MLD join, so don't special-case?
        /* entry->cb isn't set yet, but global notifiers will want call */
        addr_cb(cur, entry, ADDR_CALLBACK_DAD_COMPLETE);
    } else {
        entry->tentative = true;
        // Initial timer is for the multicast join delay
        entry->count = 0;
        entry->state_timer = randLIB_get_random_in_range(1, 10); // MAX_RTR_SOLICITATION_DELAY (1s) in ticks
    }

    tr_info("%sAddress added to IF %d: %s", (entry->tentative ? "Tentative " : ""), cur->id, trace_ipv6(address));

    ns_list_add_to_end(&cur->ip_addresses, entry);

    return entry;
}

int_fast8_t addr_delete(protocol_interface_info_entry_t *cur, const uint8_t address[static 16])
{
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (memcmp(e->address, address, 16) == 0) {
            addr_delete_entry(cur, e);
            return 0;
        }
    }

    return -1;
}

int_fast8_t addr_deprecate(protocol_interface_info_entry_t *cur, const uint8_t address[static 16])
{
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (memcmp(e->address, address, 16) == 0) {
            tr_debug("Deprecate address %s", trace_ipv6(e->address));
            addr_lifetime_update(cur, e, 0, 0, 30 * 60); //Accept max 30 min lifetime
            return 0;
        }
    }

    return -1;
}

void addr_delete_matching(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, if_address_source_t source)
{
    ns_list_foreach_safe(if_address_entry_t, e, &cur->ip_addresses) {
        if ((source == ADDR_SOURCE_UNKNOWN || e->source == source) &&
                bitsequal(e->address, prefix, prefix_len)) {
            addr_delete_entry(cur, e);
        }
    }

}

void addr_set_non_preferred(protocol_interface_info_entry_t *cur, if_address_source_t source)
{
    ns_list_foreach_safe(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == source) {
            // Sets preferred lifetime to zero or deletes tentative addresses
            addr_set_preferred_lifetime(cur, e, 0);
        }
    }
}

void addr_duplicate_detected(struct protocol_interface_info_entry *interface, const uint8_t addr[static 16])
{
    if_address_entry_t *entry = addr_get_entry(interface, addr);
    if (!entry) {
        return;
    }

    tr_warn("DAD failed: %s", trace_ipv6(addr));

    interface->dad_failures++;
    addr_cb(interface, entry, ADDR_CALLBACK_DAD_FAILED);

    /* Callback may have done something drastic like shut down the interface.
     * Don't assume entry is still valid - remove it by IP address.
     */
    addr_delete(interface, addr);
}

void addr_notification_register(if_address_notification_fn *fn)
{
    ns_list_foreach(addr_notification_t, n, &addr_notifications) {
        if (n->fn == fn) {
            return;
        }
    }

    addr_notification_t *n = ns_dyn_mem_alloc(sizeof(addr_notification_t));
    if (!n) {
        tr_error("addr_notification_register mem");
        return;
    }
    n->fn = fn;
    ns_list_add_to_end(&addr_notifications, n);
}

void addr_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{
    ns_list_foreach(addr_notification_t, n, &addr_notifications) {
        n->fn(interface, addr, reason);
    }
    if (addr->cb) {
        addr->cb(interface, addr, reason);
    }
}

void addr_set_valid_lifetime(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, uint32_t valid_lifetime)
{
    if (valid_lifetime != addr->valid_lifetime) {
        addr->valid_lifetime = valid_lifetime;
        addr_cb(interface, addr, ADDR_CALLBACK_REFRESHED);
    }
}

void addr_set_preferred_lifetime(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, uint32_t preferred_lifetime)
{
    if (preferred_lifetime != addr->preferred_lifetime) {
        addr->preferred_lifetime = preferred_lifetime;
        if (preferred_lifetime == 0) {
            // Entry is deleted if it is tentative
            if (addr->tentative) {
                addr_delete_entry(interface, addr);
            } else {
                addr_cb(interface, addr, ADDR_CALLBACK_DEPRECATED);
            }
        }
    }
}

void addr_lifetime_update(protocol_interface_info_entry_t *interface, if_address_entry_t *address, uint32_t valid_lifetime, uint32_t preferred_lifetime, uint32_t threshold)
{
    //Update Current lifetimes (see RFC 4862 for rules detail)
    if (valid_lifetime > threshold || valid_lifetime > address->valid_lifetime) {
        addr_set_valid_lifetime(interface, address, valid_lifetime);
    } else if (address->valid_lifetime <= threshold) {
        //NOT Update Valid Lifetime
    } else {
        addr_set_valid_lifetime(interface, address, threshold);
    }

    addr_set_preferred_lifetime(interface, address, preferred_lifetime);
}

void memswap(uint8_t *restrict a, uint8_t *restrict b, uint_fast8_t len)
{
    while (len--) {
        uint8_t t = *a;
        *a++ = *b;
        *b++ = t;
    }
}

/* Optimised for quick discard of mismatching addresses (eg in a cache lookup):
 * searches BACKWARDS, as last bytes are most likely to differ.
 */
bool addr_ipv6_equal(const uint8_t a[static 16], const uint8_t b[static 16])
{
    for (int_fast8_t n = 15; n >= 0; n--) {
        if (a[n] != b[n]) {
            return false;
        }
    }
    return true;
}

bool addr_iid_matches_eui64(const uint8_t iid[static 8], const uint8_t eui64[static 8])
{
    for (int_fast8_t n = 7; n >= 1; n--) {
        if (iid[n] != eui64[n]) {
            return false;
        }
    }
    return iid[0] == (eui64[0] ^ 2);
}

bool addr_iid_matches_lowpan_short(const uint8_t iid[static 8], uint16_t short_addr)
{
    if (iid[7] != (short_addr & 0xFF) ||
            iid[6] != (short_addr >> 8)) {
        return false;
    }

    for (int_fast8_t n = 5; n >= 0; n--) {
        if (iid[n] != ADDR_SHORT_ADR_SUFFIC[n]) {
            return false;
        }
    }
    return true;
}

bool addr_iid_reserved(const uint8_t iid[static 8])
{
    static const uint8_t reserved_iana[5] = { 0x02, 0x00, 0x5e, 0xff, 0xfe };
    static const uint8_t reserved_subnet_anycast[7] = { 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    if (memcmp(iid, ADDR_UNSPECIFIED + 8, 8) == 0) {
        return true; // subnet-router anycast
    }

    if (memcmp(iid, reserved_iana, 5) == 0) {
        return true;
    }

    if (memcmp(iid, reserved_subnet_anycast, 7) == 0 && iid[7] >= 0x80) {
        return true;
    }

    return false;
}

int_fast8_t addr_opaque_iid_key_set(const void *secret_key, uint8_t key_len)
{
    /* Delete existing info */
    if (addr_iid_secret_key) {
        ns_dyn_mem_free((void *) addr_iid_secret_key);
        addr_iid_secret_key = NULL;
        addr_iid_secret_key_len = 0;
    }

    /* If disabling, that's it */
    if (secret_key == NULL) {
        return 0;
    }
    /* Attempt to copy new info */
    uint8_t *copy = ns_dyn_mem_alloc(key_len);
    if (!copy) {
        return -1;
    }
    addr_iid_secret_key = memcpy(copy, secret_key, key_len);
    addr_iid_secret_key_len = key_len;
    return 0;
}

int_fast8_t addr_opaque_initial_iid_set(const void *iid)
{
    /* Delete existing info */
    if (addr_initial_iid) {
        ns_dyn_mem_free((void *) addr_initial_iid);
        addr_initial_iid = NULL;
    }
    if (!iid) {
        return 0;
    }
    /* Attempt to copy new info */
    uint8_t *copy = ns_dyn_mem_alloc(8);
    if (!copy) {
        return -1;
    }
    addr_initial_iid = memcpy(copy, iid, 8);
    return 0;
}

bool addr_opaque_iid_key_is_set(void)
{
    return addr_iid_secret_key != NULL;
}

/* RFC 7217 generation: addr must be prepopulated with 8-byte prefix, and secret key must be set */
void addr_generate_opaque_iid(protocol_interface_info_entry_t *cur, uint8_t addr[static 16])
{
opaque_retry:

    if (addr_initial_iid && !cur->dad_failures) {
        // This is test implementations use only normally should not need this.
        memcpy(addr + 8, addr_initial_iid, 8);
        return;
    }
    {
        // Limit scope to try to minimise stack, given the goto
        ns_sha256_context ctx;
        ns_sha256_init(&ctx);
        ns_sha256_starts(&ctx);
        ns_sha256_update(&ctx, addr, 8);
        if (cur->interface_name) {
            /* This isn't ideal - there's no guarantee each instance of a driver has a distinct name */
            ns_sha256_update(&ctx, cur->interface_name, strlen(cur->interface_name));
        } else {
            ns_sha256_update(&ctx, &cur->id, sizeof cur->id);
        }
        ns_sha256_update(&ctx, &cur->dad_failures, sizeof cur->dad_failures);
        ns_sha256_update(&ctx, addr_iid_secret_key, addr_iid_secret_key_len);
        ns_sha256_finish_nbits(&ctx, addr + 8, 64);
        ns_sha256_free(&ctx);
    }
    /* Note that we only check for reserved IIDs - as per RFC 7217,
     * there's no restriction on U/G bits.
     */
    if (addr_iid_reserved(addr + 8)) {
        cur->dad_failures++;
        goto opaque_retry;
    }
}

/* Write a LoWPAN IPv6 address, based on a prefix and short address */
uint8_t *addr_ipv6_write_from_lowpan_short(uint8_t dst[static 16], const uint8_t prefix[static 8], uint16_t short_addr)
{
    common_write_16_bit(short_addr, dst + 14);
    memcpy(dst + 8, ADDR_SHORT_ADR_SUFFIC, 6);
    return memcpy(dst, prefix, 8);
}

/* Turn an address (either MAC or IP) into a base IP address for context compression */
bool addr_iid_from_outer(uint8_t iid_out[static 8], const sockaddr_t *addr_in)
{
    switch (addr_in->addr_type) {
        case ADDR_802_15_4_LONG:
            memcpy(iid_out, addr_in->address + 2, 8);
            iid_out[0] ^= 2;
            break;
        case ADDR_BROADCAST:
        case ADDR_802_15_4_SHORT:
            memcpy(iid_out, ADDR_SHORT_ADR_SUFFIC, 6);
            iid_out[6] = addr_in->address[2];
            iid_out[7] = addr_in->address[3];
            break;
        case ADDR_IPV6:
            memcpy(iid_out, addr_in->address + 8, 8);
            break;
        default:
            return false;
    }

    return true;
}

int addr_interface_set_ll64(protocol_interface_info_entry_t *cur, if_address_callback_fn *cb)
{
    int ret_val = -1;
    if_address_entry_t *address_entry = NULL;
    uint8_t temp_ll64[16];
    memcpy(temp_ll64, ADDR_LINK_LOCAL_PREFIX, 8);
    memcpy(temp_ll64 + 8, cur->iid_eui64, 8);

    address_entry = addr_add(cur, temp_ll64, 64, ADDR_SOURCE_UNKNOWN, 0xffffffff, 0xffffffff, false);
    if (address_entry) {
        tr_debug("LL64 Register OK!");
        ret_val = 0;
        address_entry->cb = cb;
        if (!address_entry->tentative) {
            addr_cb(cur, address_entry, ADDR_CALLBACK_DAD_COMPLETE);
        }
    }
    return ret_val;
}

/* address_type 0 means "any" address - we return short by preference */
/* address_type 1 means long address - we ignore short addresses */
int8_t addr_interface_get_ll_address(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, uint8_t address_type)
{
    const uint8_t *short_addr = NULL;
    const uint8_t *long_addr = NULL;

    if (!cur) {
        return -1;
    }

    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (!e->tentative && addr_is_ipv6_link_local(e->address)) {
            if (cur->nwk_id == IF_6LoWPAN && memcmp(e->address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
                short_addr = e->address;
            } else {
                long_addr = e->address;
            }

            if (long_addr && short_addr) {
                break;
            }
        }
    }

    if (short_addr && address_type != 1) {
        if (address_ptr) {
            memcpy(address_ptr, short_addr, 16);
        }
        return 0;
    } else if (long_addr) {
        if (address_ptr) {
            memcpy(address_ptr, long_addr, 16);
        }
        return 0;
    } else {
        return -1;
    }
}

bool addr_interface_all_address_ready(protocol_interface_info_entry_t *cur)
{
    if (!cur) {
        return false;
    }

    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->tentative) {
            return false;
        }
    }
    return true;
}

int8_t addr_interface_gp_prefix_compare(protocol_interface_info_entry_t *cur, const uint8_t *prefix)
{
    if (cur->global_address_available) {
        ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
            if (memcmp(e->address, prefix, 8) == 0) {
                return 0;
            }
        }
    }
    return -1;
}

int8_t addr_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    /* First check the specified interface */
    if (addr_is_assigned_to_interface(cur, addr)) {
        return 0;
    }

    /* Then check other interfaces, enforcing scope zones */
    uint_fast8_t scope = addr_ipv6_scope(addr, cur);
    ns_list_foreach(protocol_interface_info_entry_t, other, &protocol_interface_info_list) {
        if (other != cur &&
                other->zone_index[scope] == cur->zone_index[scope] &&
                addr_is_assigned_to_interface(other, addr)) {
            // special handling for Thread - external global-scope ULA coming in,
            // which would match, but we need to restrict if that ULA is mesh-local
            // on the Thread side.
            if (thread_info(other) && addr_ipv6_scope(addr, other) <= IPV6_SCOPE_REALM_LOCAL) {
                continue;
            }
            return 0;
        }
    }

    return -1;
}

int8_t addr_interface_select_source(protocol_interface_info_entry_t *cur, uint8_t *src_ptr, const uint8_t *dest, uint32_t addr_preferences)
{
    int8_t ret_val = -1;
    if (cur) {
        const uint8_t *src = addr_select_source(cur, dest, addr_preferences);
        if (src) {
            memcpy(src_ptr, src, 16);
            ret_val = 0;
        }
    }
    return ret_val;
}

void addr_policy_remove_by_label(uint8_t label)
{
    ns_list_foreach_safe(addr_policy_table_entry_t, entry, &addr_policy_table) {
        if (entry->label == label) {
            /*
             * Remove label policy if no local address matches"
             */
            if (!protocol_interface_any_address_match(entry->prefix, entry->prefix_len)) {
                ns_list_remove(&addr_policy_table, entry);
                ns_dyn_mem_free(entry);
            }
        }
    }
}

// This last function must always be compiled with tracing enabled
#ifndef FEA_TRACE_SUPPORT
#define FEA_TRACE_SUPPORT 1
#include "mbed-trace/mbed_trace.h"
#endif
char *trace_sockaddr(const sockaddr_t *addr, bool panid_prefix)
{
    uint8_t length = addr_len_from_type(addr->addr_type);

    if (length == 0) {
        return "<n/a>";
    }

    /* Awkward hack for 802.15.4 address types */
    if (addr->addr_type == ADDR_802_15_4_SHORT ||
            addr->addr_type == ADDR_802_15_4_LONG) {
        length -= (panid_prefix) ? 0 : 2;
    }

    /* Start from index 0 (prints PAN ID if exists) */
    return trace_array(&addr->address[0], length);
}
