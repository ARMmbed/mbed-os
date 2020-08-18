/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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

/*
 * \file thread_nd.c
 * \brief Add short description about this file!!!
 *
 */

#include "nsconfig.h"
#ifdef HAVE_THREAD
#ifdef HAVE_THREAD_NEIGHBOR_DISCOVERY
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "eventOS_event.h"
#include "common_functions.h"
#include "socket_api.h"
#include "Core/include/ns_socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "net_thread_test.h"
#include "ns_trace.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_resolution_client.h"
#include "6LoWPAN/Thread/thread_resolution_server.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "6LoWPAN/Thread/thread_bbr_commercial.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "Common_Protocols/icmpv6.h"
#include "MLE/mle.h"
#include "dhcp_service_api.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"

#define TRACE_GROUP "thdn"

static thread_resolution_server_addr_query_cb  thread_nd_address_query_lookup;
static thread_resolution_client_error_cb thread_nd_address_error;
static thread_resolution_client_notification_cb thread_nd_coap_notification_callback;

void thread_nd_service_delete(int8_t interfaceId)
{
    (void)interfaceId;
}

int thread_nd_client_service_activate(int8_t interfaceId)
{
    thread_resolution_client_init(interfaceId);
    thread_resolution_client_set_error_cb(interfaceId, thread_nd_address_error);
    return 0;
}

int thread_nd_service_activate(int8_t interfaceId)
{
    /* We assume this is set before we activate, and doesn't change */
    thread_resolution_server_init(interfaceId, thread_nd_address_query_lookup);
    thread_resolution_client_init(interfaceId);
    thread_resolution_client_set_error_cb(interfaceId, thread_nd_address_error);
    thread_resolution_client_set_notification_cb(interfaceId, thread_nd_coap_notification_callback);
    return 0;
}

int thread_nd_service_disable(int8_t interfaceId)
{
    /* Doesn't hurt if we call thread_resolution_xxx_delete unconditionally */
    thread_resolution_server_delete(interfaceId);
    thread_resolution_client_delete(interfaceId);
    return 0;
}

static bool thread_nd_probe_transmit(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *entry)
{
    //Generate Ping
    tr_debug("Sending Ping Echo (NUD): %s", trace_ipv6(entry->ip_address));

    //ADD NeighCache
    icmpv6_build_echo_req(cur, entry->ip_address);

    return true;
}

static void thread_nd_coap_response_callback(int8_t interface_id, int8_t status, const uint8_t ip_addr[16], uint16_t loc_addr, uint32_t last_transaction_time, uint8_t *mleid)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }
    (void) last_transaction_time;
    (void) mleid;

    ipv6_neighbour_t *neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, ip_addr);
    if (neighbour_entry) {
        if (status == 0) {
            uint8_t ll_addr[4];
            common_write_16_bit(cur->mac_parameters->pan_id, ll_addr + 0);
            common_write_16_bit(loc_addr, ll_addr + 2);
            ipv6_neighbour_update_from_na(&cur->ipv6_neighbour_cache, neighbour_entry, NA_S | NA_O | NA_R, ADDR_802_15_4_SHORT, ll_addr);
        } else {
            ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
        }
    }
}

static void thread_nd_coap_notification_callback(int8_t interface_id, const uint8_t ip_addr[16], uint16_t loc_addr, const uint8_t ml_eid[8])
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    /* First check to see if we have an existing entry with different RLOC - we need to unicast error
     * notification to that old entry if so. */
    ipv6_neighbour_t *entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, ip_addr);
    if (entry && entry->ll_type == ADDR_802_15_4_SHORT) {
        uint16_t old_entry_rloc = common_read_16_bit(entry->ll_address + 2);
        if (old_entry_rloc != loc_addr) {
            uint8_t old_entry_ip[16];
            thread_addr_write_mesh_local_16(old_entry_ip, common_read_16_bit(entry->ll_address + 2), cur->thread_info);
            tr_info("Proactive address change %s %04x->%04x", trace_ipv6(ip_addr), old_entry_rloc, loc_addr);
            thread_resolution_client_address_error(interface_id, old_entry_ip, ip_addr, ml_eid);
        }
    }

    /* Now treat as an unsolicited update (by address, because entry may be NULL) */
    uint8_t ll_addr[4];
    common_write_16_bit(cur->mac_parameters->pan_id, ll_addr + 0);
    common_write_16_bit(loc_addr, ll_addr + 2);
    ipv6_neighbour_update_unsolicited(&cur->ipv6_neighbour_cache, ip_addr, ADDR_802_15_4_SHORT, ll_addr);

    if (nd_proxy_enabled_for_upstream(cur->id)) {
        ipv6_route_add(ip_addr, 128, cur->id, NULL, ROUTE_THREAD_PROXIED_HOST, 3600, 0);
    }
}

static bool thread_nd_coap_transmit(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *entry, uint8_t seq)
{

    if (thread_resolution_client_resolve(cur->id, entry->ip_address, thread_nd_coap_response_callback) < 0) {
        goto failed;
    }
    (void) seq;

    // "In progress" is indicated by returning true
    return true;

failed:
    // Failure is indicated by removing the neighbour cache entry,
    // and returning true meaning "don't send normal ICMPv6 NS".
    ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, entry);
    return true;
}


bool thread_nd_ns_transmit(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *entry,  bool unicast, uint8_t seq)
{
    // neighbor discovery active only for Routers and FEDs
    if (cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_FULL_END_DEVICE &&
            cur->thread_info->thread_device_mode != THREAD_DEVICE_MODE_ROUTER) {
        return true;
    }
    if (unicast) {
        return thread_nd_probe_transmit(cur, entry);
    } else {
        return thread_nd_coap_transmit(cur, entry, seq);
    }
}

static mac_neighbor_table_entry_t *thread_nd_child_mleid_get(protocol_interface_info_entry_t *cur, uint8_t *childAddress, uint8_t *mlmeid_ptr)
{
    mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur), childAddress, ADDR_802_15_4_SHORT);
    if (entry && !entry->ffd_device) {
        uint8_t *ptr = thread_neighbor_class_get_mleid(&cur->thread_info->neighbor_class, entry->index);
        if (ptr) {
            memcpy(mlmeid_ptr, ptr, 8);
            return entry;
        }
    }
    return NULL;
}

static int thread_nd_address_query_lookup(int8_t interface_id, const uint8_t target_addr[static 16], uint16_t *rloc, uint16_t *addr_out, bool *proxy, uint32_t *last_transaction_time, uint8_t *mleid_ptr)
{
    (void) rloc;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    uint16_t mac16 = mac_helper_mac16_address_get(cur);

    if (addr_is_assigned_to_interface(cur, target_addr)) {
        *addr_out = mac16;
        memcpy(mleid_ptr, cur->iid_slaac, 8);
        *proxy = false;
        return 0;
    }

    /* Scan IPv6 neighbour cache for registered entries of children */
    ns_list_foreach(ipv6_neighbour_t, n, &cur->ipv6_neighbour_cache.list) {
        if (n->type == IP_NEIGHBOUR_REGISTERED && addr_ipv6_equal(n->ip_address, target_addr)) {
            mac_neighbor_table_entry_t *mle_entry;
            *addr_out = mac16;
            mle_entry = thread_nd_child_mleid_get(cur, &n->ll_address[2], mleid_ptr);
            if (mle_entry) {
                //Get MLEID from Child
                uint32_t last_contact = thread_neighbor_last_communication_time_get(&cur->thread_info->neighbor_class, mle_entry->index);
                *last_transaction_time = (protocol_core_monotonic_time - last_contact) / 10;  /* Both variables are count of 100ms ticks. */
                *proxy = true;
                return 0;
            }
        }
    }

    // IF we are acting as border router and we have enabled ND proxy. we respond to address
    // queries that are not registered to us if we can route those packets out.
    /* Verify DHCP Server Status */
    bool can_route_of_mesh = false;

    if (!nd_proxy_enabled_for_upstream(interface_id)) {
        //ND proxy not enabled can not answer
        return -1;
    }

    // TODO needed for eternal border router applications can be removed later
    if (libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface_id, target_addr)) {
        can_route_of_mesh = true;
    }

    // Check if we are registered as border router in network data.
    if (thread_bbr_routing_enabled(cur)) {
        can_route_of_mesh = true;
    }

    if (thread_bbr_commercial_nd_query_process(cur, target_addr, *rloc)) {
        return -1;
    }

    if (can_route_of_mesh) {
        ipv6_route_t *route = ipv6_route_choose_next_hop(target_addr, -1, NULL);
        //Respond default route only when we would route it of mesh.
        if (route && route->on_link && route->info.interface_id != interface_id) {
            *addr_out = mac16;
            memcpy(mleid_ptr, cur->iid_slaac, 8);
            *proxy = false;
            return 0;
        }
    }

    return -1;
}

static void thread_nd_address_error(int8_t interface_id, const uint8_t ip_addr[16], const uint8_t ml_eid[8])
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }

    /* Processing for ourselves - if we have the address, and the ML-EID in the
     * message is _not_ ours, we delete.
     */
    if_address_entry_t *addr_entry = addr_get_entry(cur, ip_addr);
    if (addr_entry && memcmp(ml_eid, cur->iid_slaac, 8)) {
        addr_duplicate_detected(cur, ip_addr);
        thread_bootstrap_dua_address_generate(cur, ip_addr, 64);
    }

    /* Scan IPv6 neighbour cache for registered entries of children */
    ns_list_foreach_safe(ipv6_neighbour_t, n, &cur->ipv6_neighbour_cache.list) {
        if (n->type == IP_NEIGHBOUR_REGISTERED && addr_ipv6_equal(n->ip_address, ip_addr)) {
            uint8_t child_mleid[8];
            mac_neighbor_table_entry_t *child = thread_nd_child_mleid_get(cur, &n->ll_address[2], child_mleid);
            /* If this address belongs to an RFD child, with a different ML-EID, we must send it a duplicate message, and remove the EID */
            if (child && memcmp(child_mleid, ml_eid, 8)) {
                uint8_t child_ml_addr[16];
                thread_addr_write_mesh_local_16(child_ml_addr, child->mac16, cur->thread_info);
                tr_warn("Forwarding address error to child %04x", common_read_16_bit(&n->ll_address[2]));
                thread_resolution_client_address_error(interface_id, child_ml_addr, ip_addr, ml_eid);
                ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, n);
            }
        }
    }

    return;
}

/* Flush neighbour cache entries pointing at this 16-bit address, or (optionally) its children */
void thread_nd_flush_neighbour_cache_for_short_addr(protocol_interface_info_entry_t *cur, uint16_t flushee, bool children)
{
    ns_list_foreach_safe(ipv6_neighbour_t, entry, &cur->ipv6_neighbour_cache.list) {
        if (entry->ll_type != ADDR_802_15_4_SHORT || entry->type != IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
            continue;
        }

        uint16_t n16 = common_read_16_bit(entry->ll_address + 2);
        if (flushee == n16 || (children && flushee == thread_router_addr_from_addr(n16))) {
            ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, entry);
        }
    }
}

static buffer_t *thread_nd_icmp_error_to_mesh_originator(buffer_t *buf, protocol_interface_info_entry_t *cur, const sockaddr_t *ll_src)
{
    /* First generate a normal ICMP error (to the IP source) */
    buf = icmpv6_error(buf, cur, ICMPV6_TYPE_ERROR_DESTINATION_UNREACH, ICMPV6_CODE_DST_UNREACH_NO_ROUTE, 0);
    if (!buf) {
        return NULL;
    }

    /* Now the messed-up bit. The source gets nothing. This is going to the mesh originator */
    /* DST = ML16 derived from the mesh originator address */
    memcpy(buf->dst_sa.address, cur->thread_info->threadPrivatePrefixInfo.ulaPrefix, 8);
    memcpy(buf->dst_sa.address + 8, ADDR_SHORT_ADR_SUFFIC, 6);
    buf->dst_sa.address[14] = ll_src->address[2];
    buf->dst_sa.address[15] = ll_src->address[3];
    /* Want 16-bit ML16 source - this should select it if available */
    const uint8_t *src = addr_select_source(cur, buf->dst_sa.address, SOCKET_IPV6_PREFER_SRC_6LOWPAN_SHORT);
    if (!src) {
        return buffer_free(buf);
    }
    memcpy(buf->src_sa.address, src, 16);

    return buffer_free_route(buf);
}

/* Called when link layer passes up a unicast packet that was not for our
 * link-layer address. Our mesh handlers do this when they see a packet
 * addressed to a non-existent child or minimal function device child.
 */
buffer_t *thread_nd_snoop(protocol_interface_info_entry_t *cur, buffer_t *buf, const sockaddr_t *ll_dst, const sockaddr_t *ll_src, bool *bounce)
{
    /* If snooping, security check hasn't happened yet */
    if (buf->options.ll_security_bypass_rx) {
        return buffer_free(buf);
    }

    /* We just drop it if it wasn't mesh-addressed (impossible)  */
    if (!(buf->options.lowpan_mesh_rx && ll_src->addr_type == ADDR_802_15_4_SHORT)) {
        return buffer_free(buf);
    }

    if (!mac_neighbor_table_address_discover(mac_neighbor_info(cur), &ll_dst->address[2], ADDR_802_15_4_SHORT)) {
        /* We now know this was a packet for a non-existent child */
        goto bounce;
    }

    /* Packet must have been mesh-addressed to a minimal-function child. This always */
    /* indicates a cache error, unless the IP destination was an ML16 (RLOC16). */
    if (!thread_addr_is_mesh_local_16(buf->dst_sa.address, cur) ||
            buf->dst_sa.address[14] != ll_dst->address[2] ||
            buf->dst_sa.address[15] != ll_dst->address[3]) {
        goto bounce;
    }

    /* Forward the packet according to IP destination - we've consistency checked
     * it is an ML16 matching the original mesh destination, so will get there fine. */
    buf->options.ll_not_ours_rx = false;
    return buf;

bounce:
    *bounce = true;
    return thread_nd_icmp_error_to_mesh_originator(buf, cur, ll_src);
}

/* Called when we've determined the packet isn't IP addressed to us, but has okay
 * security. Precedes any other checks like forwarding enabled, or destination
 * type, scope.
 */
buffer_t *thread_nd_special_forwarding(protocol_interface_info_entry_t *cur, buffer_t *buf, const sockaddr_t *ll_src, bool *bounce)
{
    /* Funny Thread rule
     *
     *    A cache error is detected if a device receives a packet containing
     *    a 6LoWPAN mesh header[1] and an EID destination IPv6 address where
     *    the mesh destination is the device's RLOC16 address [2] but the EID
     *    does not belong to the receiver or, if the receiver is a router,
     *    the EID has not been registered by any Child of the receiver.
     *
     *    When a cache error is detected, the receiving device must respond by
     *    sending an ICMPv6 Destination Unreachable message with the ICMPv6
     *    Code set to 0 (No route to destination) [3] to the packet's mesh source
     *
     * Return "No route" if on-link destination, received from mesh, and it's not
     * for us or a child. Note that we return this even if not a router;
     * normally hosts should silently drop wrongly-addressed packets.
     *
     * (If the addressed child doesn't exist at all, the packet gets sent to
     * thread_nd_snoop, generating an error there).
     *
     * XXX [1] Neighbouring routers optimise out the mesh header, breaking this
     * response.
     *
     * XXX [1b] Children also optimise out mesh headers, so a full-function child
     * that has a stale resolution of a minimal device to our router address needs
     * to get a failure response.
     *
     * XXX [2] Mesh destination may have been RLOC16 of a child, and we have snooped
     * because they're MTD.
     *
     * XXX [3] "No route (0)" is a silly choice of code; "address unreachable (3)"
     * would make more sense.
     */

    if (ll_src->addr_type != ADDR_802_15_4_SHORT) {
        return buf;
    }

    mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(mac_neighbor_info(cur), &ll_src->address[2], ADDR_802_15_4_SHORT);

    /* Due to note 1 and 1b above, full-function children / neighbor routers
     * who did resolve to an RLOC16 may have optimised out the mesh header.
     * So we check for either the mesh header being present, or it coming
     * from a full-function device (FFD) neighbor.
     *
     * (If it comes from a MTD child without a mesh header we always
     * forward as normal, attempting address resolution if necessary.
     * If it comes from a child with a mesh header and the IP destination
     * is not in our neighbour cache, we need to send the child an error
     * to clear its cache.)
     */
    if (!(buf->options.lowpan_mesh_rx || (entry && entry->ffd_device))) {
        return buf;
    }

    /* Link-local and ML16 addresses are not EIDs, so no error generation */
    if (addr_is_ipv6_link_local(buf->dst_sa.address) || thread_addr_is_mesh_local_16(buf->dst_sa.address, cur)) {
        return buf;
    }

    buffer_routing_info_t *route = ipv6_buffer_route(buf);
    if (!route) {
        return buf;
    }

    if (route->route_info.interface_id != cur->id) {
        return buf;
    }

    /* On-link test - only perform this check for on-link prefixes - those will have next_hop == destination */
    /* Being on-link is what tells us an address is an "EID" rather than an arbitrary global IPv6 address */
    if (!addr_ipv6_equal(route->route_info.next_hop_addr, buf->dst_sa.address)) {
        return buf;
    }

    /* If it was addressed to us as an IP router, it's okay if the packet is for a child */
    if (thread_i_am_router(cur)) {
        //Check Registered Addresses
        ns_list_foreach(ipv6_neighbour_t, n, &cur->ipv6_neighbour_cache.list) {
            if (n->type == IP_NEIGHBOUR_REGISTERED
                    && memcmp(n->ip_address, buf->dst_sa.address, 16) == 0) {
                return buf;
            }
        }
    }

    *bounce = true;
    return thread_nd_icmp_error_to_mesh_originator(buf, cur, ll_src);
}

/* The handler for the packets transmitted above (draft-kelsey-thread-network-data-00) */
buffer_t *thread_nd_icmp_handler(protocol_interface_info_entry_t *cur, buffer_t *buf, bool *bounce)
{
    (void) bounce;
    /* Try to make sure we specifically "such an ICMP message", not normal ones */
    if (!thread_addr_is_mesh_local_16(buf->dst_sa.address, cur) || !thread_addr_is_mesh_local_16(buf->src_sa.address, cur)) {
        return buf;
    }

    if (buf->options.type != ICMPV6_TYPE_ERROR_DESTINATION_UNREACH) {
        return buf;
    }

    if (buf->options.code != ICMPV6_CODE_DST_UNREACH_NO_ROUTE) {
        return buf;
    }

    /* Make sure we have the original destination address from invoking packet */
    if (buffer_data_length(buf) < 4 + 40) {
        return buf;
    }

    const uint8_t *targetIP = buffer_data_pointer(buf) + 4 + 24;

    /* Try to look up the ML64 entry in the cache. */
    ipv6_neighbour_t *n = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, targetIP);
    if (!n || n->ll_type != ADDR_802_15_4_SHORT || n->type != IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
        return buf;
    }

    /* Invalidate that ML64 entry if its 16-bit address is equal to, or a child
     * of, the source of the ICMP message. If it doesn't match, suggests
     * a bogus packet - exit now.
     */
    uint16_t icmp16 = common_read_16_bit(buf->src_sa.address + 14);
    uint16_t entry16 = common_read_16_bit(n->ll_address + 2);
    if (!thread_addr_is_equal_or_child(icmp16, entry16)) {
        return buf;
    }

    ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, n);

    return buf;
}

int thread_nd_address_registration(protocol_interface_info_entry_t *cur, const uint8_t *ipv6Address, uint16_t mac16, uint16_t panId, const uint8_t *mac64, bool *new_neighbour_created)
{
    ipv6_neighbour_t *neigh;
    uint8_t ll_address[4];
    bool neighbor_created = false;
    common_write_16_bit(panId, ll_address + 0);
    common_write_16_bit(mac16, ll_address + 2);
    neigh = ipv6_neighbour_lookup_or_create(&cur->ipv6_neighbour_cache, ipv6Address);
    if (!neigh) {
        return -1;
    }

    uint8_t *nce_eui64 = ipv6_neighbour_eui64(&cur->ipv6_neighbour_cache, neigh);
    if (neigh->type == IP_NEIGHBOUR_REGISTERED && memcmp(nce_eui64, mac64, 8) != 0) {
        return -2;
    }

    /* New entry */
    if (neigh->state == IP_NEIGHBOUR_NEW) {
        neighbor_created = true;
    }
    memcpy(nce_eui64, mac64, 8);
    /* Set the LL address, ensure it's marked STALE */
    ipv6_neighbour_entry_update_unsolicited(&cur->ipv6_neighbour_cache, neigh, ADDR_802_15_4_SHORT, ll_address);
    neigh->type = IP_NEIGHBOUR_REGISTERED;
    neigh->lifetime = 0xffffffff; //Set Infinite
    ipv6_neighbour_set_state(&cur->ipv6_neighbour_cache, neigh, IP_NEIGHBOUR_STALE);
    if (new_neighbour_created) {
        *new_neighbour_created = neighbor_created;
    }
    return 0;
}

void thread_nd_address_remove(protocol_interface_info_entry_t *cur_interface, addrtype_t ll_type, const uint8_t *ll_address)
{
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cur_interface->ipv6_neighbour_cache.list) {
        if ((cur->type == IP_NEIGHBOUR_REGISTERED || cur->type == IP_NEIGHBOUR_TENTATIVE) && ipv6_neighbour_ll_addr_match(cur, ll_type, ll_address)) {
            ipv6_neighbour_entry_remove(&cur_interface->ipv6_neighbour_cache, cur);
        }
    }
}

bool thread_pbbr_aloc_map(protocol_interface_info_entry_t *cur, uint16_t *addr16)
{
    if (*addr16 == 0xfc38) {
        uint8_t addr[16];
        if (0 == thread_common_primary_bbr_get(cur, addr, NULL, NULL, NULL)) {
            *addr16 = common_read_16_bit(addr + 14);
            return true;
        }
    }
    return false;
}

int thread_nd_map_anycast_address(protocol_interface_info_entry_t *cur, uint16_t *addr16)
{
    // Nothing implemented for now
    uint16_t anycastAddress = *addr16;
    if (anycastAddress == 0xfc00) {
        // this is leader anycast address
        *addr16 = thread_router_addr_from_id(cur->thread_info->thread_leader_data->leaderRouterId);
        return 0;
    }
    if ((anycastAddress & 0xfff0) == 0xfc00) {
        // Check DHCPv6 anycast address mapping
        uint8_t context = anycastAddress;
        if (thread_nd_dhcp_anycast_address_mapping_from_network_data(&cur->thread_info->networkDataStorage, addr16, context)) {
            return 0;
        }
    }

    if ((anycastAddress & 0xfff0) == 0xfc10 || (anycastAddress & 0xfff0) == 0xfc20) {
        // service mapping to anycast address
        uint8_t S_id = anycastAddress & 0x0f;
        if (thread_nd_service_anycast_address_mapping_from_network_data(&cur->thread_info->networkDataStorage, addr16, S_id)) {
            return 0;
        }
    }

    if ((anycastAddress & 0xfff8) == 0xfc30) {
        // Commissioner anycast address mapping
        if (!cur->thread_info->registered_commissioner.commissioner_valid) {
            return -1;
        }
        if ((anycastAddress & 0x0007) != cur->thread_info->registered_commissioner.session_id % 8) {
            return -1;
        }
        *addr16 = common_read_16_bit(cur->thread_info->registered_commissioner.border_router_address + 14);
        return 0;
    }
    if (thread_pbbr_aloc_map(cur, addr16)) {
        return 0;
    }

    return -1;
}

#endif //HAVE_THREAD_NEIGHBOR_DISCOVERY
#endif //HAVE_THREAD

