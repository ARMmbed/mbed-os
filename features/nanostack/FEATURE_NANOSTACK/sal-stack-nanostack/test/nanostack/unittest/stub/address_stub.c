/*
 * Copyright (c) 2008, 2010-2017, Arm Limited and affiliates.
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
 * \file address.c
 * \brief Utility functions concernig addresses
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
#include "socket_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "common_functions.h"

#include "address_stub.h"

#define TRACE_GROUP_ADDRESS "addr"

address_stub_def address_stub;

typedef struct addr_policy_table_entry_t {
    uint8_t prefix[16];
    uint8_t prefix_len;
    uint8_t precedence;
    uint8_t label;
    ns_list_link_t link;
} addr_policy_table_entry_t;

static NS_LIST_DEFINE(addr_policy_table, addr_policy_table_entry_t, link);

uint32_t addr_preferences_default = SOCKET_IPV6_PREFER_SRC_TMP | SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT;

const uint8_t ADDR_LINK_LOCAL_PREFIX[8]         = { 0xfe, 0x80 };
const uint8_t ADDR_SHORT_ADR_SUFFIC[6]          = { 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00};

const uint8_t ADDR_MULTICAST_SOLICITED[13]      = { 0xff, 0x02, [11] = 0x01, 0xff};
const uint8_t ADDR_LINK_LOCAL_MDNS[16]          = { 0xff, 0x02, [15] = 0xfb };
const uint8_t ADDR_LINK_LOCAL_ALL_NODES[16]     = { 0xff, 0x02, [15] = 0x01 };
const uint8_t ADDR_LINK_LOCAL_ALL_ROUTERS[16]   = { 0xff, 0x02, [15] = 0x02 };
const uint8_t ADDR_LINK_LOCAL_ALL_MLDV2_ROUTERS[16] = { 0xff, 0x02, [15] = 0x16 };
const uint8_t ADDR_REALM_LOCAL_ALL_NODES[16]    = { 0xff, 0x03, [15] = 0x01 };
const uint8_t ADDR_REALM_LOCAL_ALL_ROUTERS[16]  = { 0xff, 0x03, [15] = 0x02 };
const uint8_t ADDR_SITE_LOCAL_ALL_ROUTERS[16]   = { 0xff, 0x05, [15] = 0x02 };
const uint8_t ADDR_ALL_MPL_FORWARDERS[16]       = { 0xff, 0x03, [15] = 0xfc };
const uint8_t ADDR_ALL_DHCP_RELAY_AGENTS_AND_SERVERS[16] = { 0xff, 0x02, [13] = 0x01, 0x00, 0x02 };

const uint8_t ADDR_IPV4_MAPPED_PREFIX[12]       = { [10] = 0xff, 0xff };
const uint8_t ADDR_LOOPBACK[16]                 = { [15] = 1 };
const uint8_t ADDR_UNSPECIFIED[16]              = { 0 };
#define ADDR_IPV4_COMPATIBLE                    ADDR_LOOPBACK /* First 96 bits match...*/


uint8_t addr_len_from_type(addrtype_t addr_type)
{
    return address_stub.uint8_value;
}

char* trace_sockaddr(const sockaddr_t* addr, bool panid_prefix)
{
    return address_stub.char_ptr;
}

uint8_t addr_check_broadcast(const address_t addr, addrtype_t addr_type)
{
    return address_stub.uint8_value;
}

bool addr_is_ipv6_link_local(const uint8_t addr[__static 16])
{
    return address_stub.bool_value;
}

/* Scope(A), as defined in RFC 6724 plus RFC 4007 */
uint_fast8_t addr_ipv6_scope(const uint8_t addr[__static 16], const protocol_interface_info_entry_t *interface)
{
    return address_stub.uint8_value;
}

void address_module_init(void)
{
}

bool addr_multicast_fwd_list_check(if_group_fwd_list_t *list, const uint8_t group[16])
{
    return address_stub.bool_value;
}

bool addr_multicast_fwd_list_add(if_group_fwd_list_t *list, const uint8_t group[16])
{
    return address_stub.bool_value;
}

bool addr_multicast_fwd_list_remove(if_group_fwd_list_t *list, const uint8_t group[16])
{
    return address_stub.bool_value;
}

void addr_multicast_fwd_list_delete_entry(if_group_fwd_list_t *list, if_group_fwd_entry_t *entry)
{
}

int_fast8_t addr_policy_table_add_entry(const uint8_t *prefix, uint8_t len, uint8_t precedence, uint8_t label)
{
    return address_stub.int8_value;
}

int_fast8_t addr_policy_table_delete_entry(const uint8_t *prefix, uint8_t len)
{
    return address_stub.int8_value;
}

void addr_policy_table_print(void)
{
}

if_address_entry_t *addr_get_entry(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return address_stub.if_address_entry_value;
}

bool addr_is_assigned_to_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return address_stub.bool_value;
}

bool addr_is_tentative_for_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return address_stub.bool_value;
}

if_group_entry_t *addr_add_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    return address_stub.if_group_entry_value;
}

void addr_remove_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
}

void addr_delete_group_entry(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
}

void addr_delete_group(protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
}

if_group_entry_t *addr_get_group_entry(const protocol_interface_info_entry_t *interface, const uint8_t group[static 16])
{
    return address_stub.if_group_entry_value;
}

void addr_add_router_groups(protocol_interface_info_entry_t *interface)
{
}

/* RFC 6724 Default source address selection */
const uint8_t *addr_select_source(protocol_interface_info_entry_t *interface, const uint8_t dest[__static 16], uint32_t addr_preferences)
{
    return address_stub.uint8_ptr;
}

const uint8_t *addr_select_with_prefix(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, uint32_t addr_preferences)
{
    return address_stub.uint8_ptr;
}

void addr_timer(protocol_interface_info_entry_t *cur, uint_fast16_t ticks)
{
}

void addr_fast_timer(protocol_interface_info_entry_t *cur, uint_fast16_t ticks)
{

}

void addr_slow_timer(protocol_interface_info_entry_t *cur, uint_fast16_t seconds)
{

}

void addr_max_slaac_entries_set(protocol_interface_info_entry_t *cur, uint8_t max_slaac_entries)
{

}

if_address_entry_t *addr_add(protocol_interface_info_entry_t *cur, const uint8_t address[__static 16], uint_fast8_t prefix_len, if_address_source_t source, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad)
{
    return address_stub.if_address_entry_value;
}

void addr_delete_entry(protocol_interface_info_entry_t *cur, if_address_entry_t *addr)
{
}

int_fast8_t addr_delete(protocol_interface_info_entry_t *cur, const uint8_t address[__static 16])
{
    return address_stub.int8_value;
}

void addr_delete_matching(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, if_address_source_t source)
{
}

void addr_set_non_preferred(protocol_interface_info_entry_t *cur, if_address_source_t source)
{
}

void addr_duplicate_detected(struct protocol_interface_info_entry *interface, const uint8_t addr[__static 16])
{
}

void addr_notification_register(if_address_notification_fn *fn)
{

}

void addr_cb(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{

}

void addr_set_valid_lifetime(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, uint32_t valid_lifetime)
{

}

void addr_set_preferred_lifetime(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, uint32_t preferred_lifetime)
{

}

void memswap(uint8_t *restrict a, uint8_t *restrict b, uint_fast8_t len)
{
}

bool addr_ipv6_equal(const uint8_t a[__static 16], const uint8_t b[__static 16])
{
    return memcmp(a, b, 16) == 0;
}

bool addr_iid_matches_eui64(const uint8_t iid[__static 8], const uint8_t eui64[__static 8])
{
    return address_stub.bool_value;
}

bool addr_iid_matches_lowpan_short(const uint8_t iid[__static 8], uint16_t short_addr)
{
    return address_stub.bool_value;
}

bool addr_iid_reserved(const uint8_t iid[static 8])
{
    return address_stub.bool_value;
}

int_fast8_t addr_opaque_iid_key_set(const void *secret_key, uint8_t key_len)
{
    return address_stub.int8_value;
}

bool addr_opaque_iid_key_is_set(void)
{
    return address_stub.bool_value;
}

void addr_generate_opaque_iid(protocol_interface_info_entry_t *cur, uint8_t addr[static 16])
{

}

uint8_t *addr_ipv6_write_from_lowpan_short(uint8_t dst[__static 16], const uint8_t prefix[__static 8], uint16_t short_addr)
{
    return address_stub.uint8_ptr;
}

bool addr_iid_from_outer(uint8_t iid_out[__static 8], const sockaddr_t *addr_in)
{
    return address_stub.bool_value;
}


int addr_interface_set_ll64(protocol_interface_info_entry_t *cur, if_address_callback_fn *cb)
{
    return address_stub.int_value;
}

int8_t addr_interface_get_ll_address(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, uint8_t address_type)
{
    return address_stub.int8_value;
}

bool addr_interface_all_address_ready(protocol_interface_info_entry_t *cur)
{
    return address_stub.bool_value;
}

int8_t addr_interface_get_any_gp_address(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, uint8_t address_type)
{
    return address_stub.int8_value;
}

int8_t addr_interface_gp_prefix_compare(protocol_interface_info_entry_t *cur, const uint8_t *prefix)
{
    return address_stub.int8_value;
}

int8_t addr_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    return address_stub.int8_value;
}

int8_t addr_interface_select_source(protocol_interface_info_entry_t *cur, uint8_t *src_ptr, const uint8_t *dest, uint32_t addr_preferences)
{
    return address_stub.int8_value;
}
void addr_multicast_fwd_set_forwarding(struct protocol_interface_info_entry *interface, bool enable)
{
}
