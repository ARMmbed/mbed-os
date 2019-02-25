/*
 * Copyright (c) 2015, 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include "nsconfig.h"
#if defined(HAVE_THREAD) && defined(HAVE_DHCPV6_SERVER)
#include <string.h>
#include <ns_types.h>
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/ipv6.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"

#define TRACE_GROUP "thds"

static void thread_service_remove_GUA_from_neighcache(protocol_interface_info_entry_t *cur, uint8_t *targetAddress)
{
    ipv6_neighbour_t *neighbour_entry;

    neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, targetAddress);
    if (neighbour_entry) {
        tr_debug("Remove from neigh Cache: %s", tr_ipv6(targetAddress));
        ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
    }
}

static void thread_dhcp_address_prefer_remove_cb(int8_t interfaceId, uint8_t *targetAddress, void *prefix_info)
{
    protocol_interface_info_entry_t *curPtr = protocol_stack_interface_info_get_by_id(interfaceId);
    if (!curPtr) {
        return;
    }
    if (!targetAddress) {
        //Clear All targets routes
        ipv6_route_table_remove_info(interfaceId, ROUTE_THREAD_PROXIED_HOST, prefix_info);
    } else {
        tr_debug("Address Preferred Timeout");
        ipv6_route_delete(targetAddress, 128, interfaceId, NULL, ROUTE_THREAD_PROXIED_HOST);
        thread_service_remove_GUA_from_neighcache(curPtr, targetAddress);

    }

}

static bool thread_dhcp_address_add_cb(int8_t interfaceId, dhcp_address_cache_update_t *address_info, void *route_src)
{
    protocol_interface_info_entry_t *curPtr = protocol_stack_interface_info_get_by_id(interfaceId);
    if (!curPtr) {
        return false;
    }

    // If this is solicit from existing address, flush ND cache.
    if (address_info->allocatedNewAddress) {
        // coverity[returned_null] for ignoring protocol_stack_interface_info_get_by_id NULL return
        thread_service_remove_GUA_from_neighcache(curPtr, address_info->allocatedAddress);
    }

    if (thread_bbr_nd_entry_add(interfaceId, address_info->allocatedAddress, address_info->validLifeTime, route_src) == -1) {
        // No nanostack BBR present we will put entry for application implemented BBR
        ipv6_route_t *route = ipv6_route_add_with_info(address_info->allocatedAddress, 128, interfaceId, NULL, ROUTE_THREAD_PROXIED_HOST, route_src, 0, address_info->validLifeTime, 0);
        if (!route) {
            return false;
        }

    }
    return true;
}

int thread_dhcp6_server_init(int8_t interface_id, uint8_t prefix[8], uint8_t eui64[8], uint32_t validLifeTimne)
{
    if (DHCPv6_server_service_init(interface_id, prefix, eui64, DHCPV6_DUID_HARDWARE_EUI64_TYPE) != 0) {
        return -1;
    }
    //Register Callbacks
    DHCPv6_server_service_callback_set(interface_id, prefix, thread_dhcp_address_prefer_remove_cb, thread_dhcp_address_add_cb);
    //SET Timeout
    DHCPv6_server_service_set_address_validlifetime(interface_id, prefix, validLifeTimne);

    return 0;
}

#endif
