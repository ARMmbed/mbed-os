/*
 * Copyright (c) 2008, 2010-2015 ARM Limited. All rights reserved.
 */
/**
 * \file address.c
 * \brief Utility functions concernig addresses
 *
 * This file contains all the utility functions that can be used to
 * check, manipulate etc. addresses.
 */
#include "config.h"
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
#include "6LoWPAN/Thread/thread.h"
#include "common_functions.h"

#include "address_stub.h"

#define TRACE_GROUP_ADDRESS "addr"

#define ADDR_SHORT_MULTICAST_MAX 10
#define ADDR_MAX_DYNAMIC_MULTICAST_ADDRESSES 100

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
const uint8_t ADDR_LINK_LOCAL_ALL_NODES[16]     = { 0xff, 0x02, [15] = 0x01 };
const uint8_t ADDR_LINK_LOCAL_ALL_ROUTERS[16]   = { 0xff, 0x02, [15] = 0x02 };
const uint8_t ADDR_MULTICAST_ALL_DHCPV6_SERVER[16]  = { 0xff, 0x03, [13] = 0x01, 0x00, 0x03 };
const uint8_t ADDR_ALL_DHCP_RELAY_AGENTS_AND_SERVERS[16] = { 0xff, 0x02, [13] = 0x01, 0x00, 0x02 };

const uint8_t ADDR_MULTICAST_SITEPREFIX[15]     = { 0xff, 0x05 };
const uint8_t ADDR_MULTICAST_SUBPREFIX[15]      = { 0xff, 0x03 };
const uint8_t ADDR_IPV4_MAPPED_PREFIX[12]       = { [10] = 0xff, 0xff };
const uint8_t ADDR_LOOPBACK[16]                 = { [15] = 1 };
const uint8_t ADDR_UNSPECIFIED[16]              = { 0 };
#define ADDR_IPV4_COMPATIBLE                    ADDR_LOOPBACK /* First 96 bits match...*/


uint8_t addr_len_from_type(addrtype_t addr_type)
{
    return 0;
}

uint8_t addr_check_broadcast(const address_t addr, addrtype_t addr_type)
{
    address_stub.expectedUint8ptr = &addr;
    address_stub.expectedUint8 = addr_type;

    return address_stub.returnUint8;
}

bool addr_is_ipv6_link_local(const uint8_t addr[__static 16])
{
    return false;
}

/* Scope(A), as defined in RFC 6724 plus RFC 4007 */
uint_fast8_t addr_ipv6_scope(const uint8_t addr[__static 16], const protocol_interface_info_entry_t *interface)
{
    return 0;
}

void address_module_init(void)
{
}

void addr_multicast_list_init(void)
{
}

uint8_t addr_multicast_list_allocate(const uint8_t *ptr, uint8_t relay_allocate)
{
    return -1;
}

uint8_t addr_multicast_list_free(const uint8_t *ptr)
{
    return 1;
}

uint8_t addr_multicast_list_check(const uint8_t *ptr, bool rx_adr, protocol_interface_info_entry_t *cur_interface)
{
    return -1;
}

int_fast8_t addr_policy_table_add_entry(const uint8_t *prefix, uint8_t len, uint8_t precedence, uint8_t label)
{
    return -1;
}

int_fast8_t addr_policy_table_delete_entry(const uint8_t *prefix, uint8_t len)
{
    return -1;
}

void addr_policy_table_print(void)
{
}

if_address_entry_t *addr_get_entry(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return NULL;
}

bool addr_is_assigned_to_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return false;
}

bool addr_is_tentative_for_interface(const protocol_interface_info_entry_t *interface, const uint8_t addr[__static 16])
{
    return false;
}

/* RFC 6724 Default source address selection */
const uint8_t *addr_select_source(protocol_interface_info_entry_t *interface, const uint8_t dest[__static 16], uint32_t addr_preferences)
{
    return NULL;
}

const uint8_t *addr_select_with_prefix(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, uint32_t addr_preferences)
{
    return NULL;
}

void addr_timer(protocol_interface_info_entry_t *cur, uint_fast16_t ticks)
{
}

if_address_entry_t *addr_add(protocol_interface_info_entry_t *cur, const uint8_t address[__static 16], uint_fast8_t prefix_len, if_address_source_t source, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad)
{
    return NULL;
}

int_fast8_t addr_delete(protocol_interface_info_entry_t *cur, const uint8_t address[__static 16])
{
    return -1;
}

void addr_delete_matching(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len, if_address_source_t source)
{
}

void addr_duplicate_detected(struct protocol_interface_info_entry *interface, const uint8_t addr[__static 16])
{
}

void memswap(uint8_t *restrict a, uint8_t *restrict b, uint_fast8_t len)
{
}

bool addr_ipv6_equal(const uint8_t a[__static 16], const uint8_t b[__static 16])
{
    return false;
}

bool addr_iid_matches_eui64(const uint8_t iid[__static 8], const uint8_t eui64[__static 8])
{
    return false;
}

bool addr_iid_matches_lowpan_short(const uint8_t iid[__static 8], uint16_t short_addr)
{
    return false;
}

uint8_t *addr_ipv6_write_from_lowpan_short(uint8_t dst[__static 16], const uint8_t prefix[__static 8], uint16_t short_addr)
{
    return NULL;
}

bool addr_iid_from_outer(uint8_t iid_out[__static 8], const sockaddr_t *addr_in)
{
    return false;
}


int addr_interface_set_ll64(protocol_interface_info_entry_t *cur, if_address_callback_fn *cb)
{
    return -1;
}

int8_t addr_interface_get_ll_address(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, uint8_t address_type)
{
    return -1;
}

int8_t addr_interface_get_any_gp_address(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, uint8_t address_type)
{
    return -1;
}

int8_t addr_interface_gp_prefix_compare(protocol_interface_info_entry_t *cur, const uint8_t *prefix)
{
    return -1;
}

int8_t addr_interface_address_compare(protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    return -1;
}

int8_t addr_interface_select_source(protocol_interface_info_entry_t *cur, uint8_t *src_ptr, const uint8_t *dest, uint32_t addr_preferences)
{
    return -1;
}
