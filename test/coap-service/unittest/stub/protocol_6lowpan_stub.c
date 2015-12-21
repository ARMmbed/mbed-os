/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include "string.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#ifdef HAVE_RPL
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#else
/* This sort of thing should be handled in rpl.h itself */
#define rpl_object_poisons() 0
#endif
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#ifndef NO_MLE
#include "MLE/mle.h"
//#include "MLE/mle_challenge_data.h"
#endif
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread.h"
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
#include "NAP/nap_config.h"
#include "randLIB.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol_interface_driver.h"

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

void protocol_init(void)
{
}

void protocol_6lowpan_stack(buffer_t *b)
{
}

uint8_t protocol_6lowpan_llao_write(protocol_interface_info_entry_t *cur, uint8_t *opt_out, uint8_t opt_type)
{
    return 0;
}

uint8_t protocol_6lowpan_llao_parse(protocol_interface_info_entry_t *cur, const uint8_t *opt_in, sockaddr_t *ll_addr_out)
{
    return 0;
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

void protocol_6lowpan_priority_neigh_rm_callback(uint8_t *nwk_suffic, protocol_interface_info_entry_t *cur)
{
}

void protocol_6lowpan_neighbor_priority_update(protocol_interface_info_entry_t *cur, uint8_t *removed_priority, uint8_t *updated_priority)
{
}

int8_t protocol_6lowpan_neighbor_address_state_synch(protocol_interface_info_entry_t *cur, const uint8_t eui64[8], const uint8_t iid[8])
{
    return 0;
}

int8_t protocol_6lowpan_neighbor_remove(protocol_interface_info_entry_t *cur, uint8_t *address_ptr, addrtype_t type)
{
    return 0;
}

int8_t protocol_6lowpan_neighbor_address_validate(route_info_entry_t *info, protocol_interface_info_entry_t *cur)
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
