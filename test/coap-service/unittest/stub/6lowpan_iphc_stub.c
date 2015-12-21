/*
 * Copyright (c) 2013-2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#define HAVE_CIPV6

#ifdef HAVE_CIPV6
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "Core/include/socket.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "6LoWPAN/ND/icmp.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/IPHC_Decode/cudp.h"
#include "6LoWPAN/IPHC_Decode/iphc_compress.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread.h"

#ifdef HAVE_RPL
#include "6LoWPAN/RPL/rpl.h"
#include "6LoWPAN/RPL/rpl_obj.h"
#endif

#define TRACE_GROUP  "iphc"

#include "MulticastTrigle/multicast.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"

typedef struct ip_src_addr_t {
    addrtype_t    addr_type;              /*!< Type of address */
    uint8_t       address[8];                /*!< Source or destination address */
} ip_src_addr_t;


NS_LARGE    uint8_t ip_hc[2];
NS_LARGE route_info_entry_t route_info_entry;
NS_LARGE uint8_t flow_len;
NS_LARGE uint8_t flow_ctrl[4];

void cipv6_set_next_hop(buffer_t *buf);
uint8_t cipv6_forward_rl_check(void);
buffer_t *cipv6_reallocate_ip_headers(buffer_t *buf, uint8_t type, forward_dest_t dest);


buffer_t *cipv6_check_link_layer_length(buffer_t *buf)
{
    return NULL;
}

void cipv6_set_next_hop(buffer_t *buf)
{
}

uint8_t cipv6_forward_rl_check(void)
{
    return 0;
}

buffer_t *cipv6_reallocate_ip_headers(buffer_t *buf, uint8_t type, forward_dest_t dest)
{
    return NULL;
}

buffer_t *cipv6_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *lowpan_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *cipv6_up(buffer_t *buf)
{
    return NULL;
}

#endif /* HAVE_CIPV6 */

