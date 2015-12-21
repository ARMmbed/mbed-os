/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include <string.h>
#include "ns_types.h"
#define HAVE_DEBUG 1
#include "buffer.h"
#include "ns_list.h"
//#include "platform/ns_debug.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "mesh.h"

void debug(const char *msg)
{
}

protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id)
{
    return NULL;
}

extern COMMON_FUNCTIONS_FN uint16_t common_read_16_bit(const uint8_t data_buf[__static 2]);
extern COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2]);
extern COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2]);

bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t addr[])
{
    return false;
}

uint8_t addr_check_broadcast(const address_t addr, addrtype_t addr_type)
{
    return 0;
}

#ifndef HAVE_DEBUG
uint8_t *buffer_corrupt_check(buffer_t *buf)
{
    return NULL;
}
#endif

bool mac_mlme_write_our_addr(struct protocol_interface_info_entry *cur, sockaddr_t *sockaddr)
{
}

mac_direct_check_res_e mac_indirect_data_check(struct buffer *buf)
{
    mac_direct_check_res_e res = {0};
    return res;
}

void protocol_stats_update(nwk_stats_type_t type, uint16_t update_val)
{
}

#if 0
buffer_t *buffer_headroom(buffer_t *buf, uint16_t size)
{
    return NULL;
}
#endif

buffer_t *cipv6_check_link_layer_length(buffer_t *buf)
{
    return NULL;
}
