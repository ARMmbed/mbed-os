/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "randLIB.h"
#include <string.h>
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/ND/nd_router_object.h" // for nd_ra_timing()
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"

#ifdef RADV_TX

typedef struct icmp_queued_ra {
    uint8_t         addr[16];           /* destination address */
    uint8_t         abro[16];           /* RFC 6775 ABRO 6LBR address (or ADDR_UNSPECIFIED if no ABRO) */
    bool            rs_triggered;       /* was queued by an RS */
    uint16_t        ticks;              /* ticks until transmission (relative to last list entry) */
    protocol_interface_info_entry_t *interface;
    ns_list_link_t  link;
} icmp_queued_ra_t;

static NS_LIST_DEFINE(icmp_ra_queue, icmp_queued_ra_t, link);

void icmpv6_radv_init(protocol_interface_info_entry_t *cur)
{
}

void icmpv6_queue_ra(icmp_queued_ra_t *new_ra)
{
}

void icmpv6_unqueue_ra(icmp_queued_ra_t *ra)
{
}

/* Trigger a single RA from an RS - must be called multiple times if we have multiple ABROs */
void icmpv6_trigger_ra_from_rs(protocol_interface_info_entry_t *cur, const uint8_t dest[16], const uint8_t abro[16])
{

}

buffer_t *icmpv6_rs_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{

}

void icmpv6_restart_router_advertisements(protocol_interface_info_entry_t *cur, const uint8_t abro[16])
{

}

void icmpv6_stop_router_advertisements(protocol_interface_info_entry_t *cur, const uint8_t *abro)
{
}

void icmpv6_radv_timer(uint16_t ticks)
{

}

#endif /* RADV_TX */
