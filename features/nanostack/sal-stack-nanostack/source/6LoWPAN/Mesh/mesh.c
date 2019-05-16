/*
 * Copyright (c) 2014-2017, 2019, Arm Limited and affiliates.
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
 * \file mesh.c
 * \brief 6LoWPAN Mesh header handling (RFC 4944: S5.2, S11)
 *
 */

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "Core/include/ns_socket.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "mesh"

#ifdef HAVE_MESH

/* Nit - this should really be per-interface */
static uint8_t mesh_bc0_sequence;

/* Thread wants to treat RFC 4944 S.9 multicast addresses as unicast, so
 * have to allow a hook to disable multicast functionality. (Also, should
 * really be per-interface)
 */
static bool support_mesh_multicast = true;

bool mesh_supports_multicast(void)
{
    return support_mesh_multicast;
}

void mesh_all_addresses_unicast(bool flag)
{
    support_mesh_multicast = !flag;
}

bool mesh_short_address_is_multicast(uint16_t addr)
{
    return ((addr & 0xE000) == 0x8000) && support_mesh_multicast;
}

bool mesh_short_address_is_broadcast_or_multicast(uint16_t addr)
{
    return addr == 0xffff || mesh_short_address_is_multicast(addr);
}

buffer_t *mesh_up(buffer_t *buf)
{
    uint8_t *ptr = buffer_data_pointer(buf);
    uint_fast16_t len = buffer_data_length(buf);
    uint_fast8_t orig_addrlen, final_addrlen, last_hop_addrlen;
    uint_fast8_t hops;
    const uint8_t *orig_addr, *final_addr, *last_hop_addr;
    bool for_us = false;
    protocol_interface_info_entry_t *cur = buf->interface;
    if (!cur) {
        goto drop;
    }

    /* Sanity check - must have 5 bytes (header + 2x16-bit addresses) */
    if (len < 5) {
        goto drop;
    }

    orig_addrlen  = ptr[0] & 0x20 ? 2 : 8;
    final_addrlen = ptr[0] & 0x10 ? 2 : 8;
    hops          = ptr[0] & 0x0F;
    ptr++, len--;

    /* Hops up to 14 can be stored in first byte; can use extra byte for more */
    if (hops == 0x0F) {
        hops = *ptr++;
        len--;
    }

    if (hops == 0) {
        goto drop;
    }

    if (len < orig_addrlen + final_addrlen) {
        goto drop;
    }

    orig_addr = ptr;
    ptr += orig_addrlen;

    final_addr = ptr;
    ptr += final_addrlen;

    /* Also consume any BC0 header (we don't need its contents) */
    if (len >= 2 && ptr[0] == LOWPAN_DISPATCH_BC0) {
        ptr += 2;
    }

    /* Routing functions may need to see last hop to avoid loops */
    last_hop_addrlen = buf->src_sa.addr_type == ADDR_802_15_4_LONG ? 8 : 2;
    last_hop_addr = buf->src_sa.address + 2;

    if (!for_us && final_addrlen == 2) {
        uint16_t addr = common_read_16_bit(final_addr);
        /* At this layer, we accept all broadcasts, and all multicasts.
         * No point in software filtering multicasts here - let the higher
         * layers do it. Simplifies code below too.
         */
        if (mesh_short_address_is_broadcast_or_multicast(addr)) {
            if (addr == 0xFFFF) {
                buf->options.ll_broadcast_rx = true;
            } else {
                buf->options.ll_broadcast_rx = false;
                buf->options.ll_multicast_rx = true;
            }
            for_us = true;
            /* Potentially let a multicast forwarder deal with it too - if they
             * want to retransmit, they'll have to take a copy.
             */
            if (cur->mesh_callbacks && cur->mesh_callbacks->multicast) {
                cur->mesh_callbacks->multicast(cur, addr, buf);
            }
        } else if (addr == 0xFFFE) { /* definitely reserved */
            goto drop;
        } else if ((addr & 0x8000) && mesh_supports_multicast()) {
            /* Reserved, but allow use as unicast if mesh_supports_multicast() is false */
            goto drop;
        }
    }

    /* Mirror IP's behaviour with respect to link-layer addresses - it doesn't
     * like receiving IP unicasts that aren't link-layer unicasts. Similarly,
     * we don't like receiving mesh unicasts that aren't IEEE 802.15.4 unicasts.
     */
    if (!for_us && buf->options.ll_broadcast_rx) {
        goto drop;
    }

    if (!for_us) {
        for_us = nwk_interface_compare_mac_address(cur, final_addrlen, final_addr);
    }

    if (for_us) {
intercept:
        /* Set the source MAC address from the Mesh header */
        /* XXX any more sanity checks needed here? */
        buf->src_sa.addr_type = orig_addrlen == 2 ? ADDR_802_15_4_SHORT : ADDR_802_15_4_LONG;
        /* src_sa is in 802.15.4 header form - ie 2-byte PAN ID, followed by
         * 2 or 8 byte address. Leave PAN ID in place and overwrite address */
        memcpy(buf->src_sa.address + 2, orig_addr, orig_addrlen);

        /* Set the buffer dest MAC address from the Mesh header */
        buf->dst_sa.addr_type = final_addrlen == 2 ? ADDR_802_15_4_SHORT : ADDR_802_15_4_LONG;
        memcpy(buf->dst_sa.address + 2, final_addr, final_addrlen);

        buf->options.lowpan_mesh_rx = true;

        /* Remove the Mesh header */
        buffer_data_pointer_set(buf, ptr);
        /* XXX check - any side-effects of not being B_FROM_MAC? */
        buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_MESH_ROUTING | B_TO_IPV6_TXRX);
        return buf;
    }

    /* Now we know it's not addressed to us, we can check security - had
     * to allow bypass so far for MLE packets to us.
     */
    if (buf->options.ll_security_bypass_rx) {
        goto drop;
    }

    /* We are not the final destination; if we've been given no routing
     * routing function pointer, then we just drop the packet. */
    if (!cur->mesh_callbacks || !cur->mesh_callbacks->route) {
        goto drop;
    }

    /*
     * Note that all multicast/broadcast are treated as "for us", so only
     * unicasts reach here.
     */

    /* Do not forward unicast packets that were sent to a broadcast address  */
    if (addr_check_broadcast(buf->dst_sa.address, buf->dst_sa.addr_type) == 0) {
        goto drop;
    }

    /* Decrement and check the hops count */
    if (--hops == 0) {
        goto drop;
    }

    /* Ask the routing function where to send this */
    mesh_routing_route_response_t route;
    route.intercept = false;

    if (cur->mesh_callbacks->route(cur, last_hop_addrlen, last_hop_addr, final_addrlen, final_addr, &route)) {
        goto drop;
    }

    /* If the function says to intercept the packet, do so */
    if (route.intercept) {
        buf->options.ll_not_ours_rx = true;
        goto intercept;
    }

    /* Rewind to rewrite the Mesh Hops Left field */
    ptr = buffer_data_pointer(buf);
    if ((ptr[0] & 0x0F) != 0x0F) {
        /* Modify 4-bit hops field */
        ptr[0] = (ptr[0] & 0xF0) | hops;
    } else {
        /* Modify 8-bit hops field */
        if (hops < 0x0F) {
            /* 8 bits no longer needed - convert to 4-bit header, save a byte */
            ptr[1] = (ptr[0] & 0xF0) | hops;
            buffer_data_strip_header(buf, 1);
        } else {
            ptr[1] = hops;
        }
    }

    /* Set dest MAC address from routing response */
    buf->dst_sa.addr_type = route.addr_len == 2 ? ADDR_802_15_4_SHORT : ADDR_802_15_4_LONG;
    memcpy(buf->dst_sa.address + 2, route.address, route.addr_len);

    /* Set src MAC address from our interface */
    buf->src_sa.addr_type = ADDR_NONE;
    if (!mac_helper_write_our_addr(cur, &buf->src_sa)) {
        goto drop;
    }

    /* Send back down to MAC */
    buf->ip_routed_up = true;
    buf->info = (buffer_info_t)(B_FROM_MESH_ROUTING | B_TO_MAC);

    buf->options.code = 0;
    return buf;

drop:
    protocol_stats_update(STATS_IP_RX_DROP, 1);
    return buffer_free(buf);
}

static bool always_add_mesh_header_as_originator;

void force_mesh_headers(bool force)
{
    always_add_mesh_header_as_originator = force;
}

bool mesh_forwardable_address(const protocol_interface_info_entry_t *cur, addrtype_t addr_type, const uint8_t *addr)
{
    if (cur->mesh_callbacks) {
        if (cur->mesh_callbacks->forwardable_address) {
            return cur->mesh_callbacks->forwardable_address(cur, addr_type, addr);
        }

        /* Default to true if no callback */
        return true;
    }

    return false;
}

bool mesh_address_map(protocol_interface_info_entry_t *cur, addrtype_t *addr_type, uint8_t *addr)
{
    if (cur->mesh_callbacks && cur->mesh_callbacks->address_map) {
        return cur->mesh_callbacks->address_map(cur, addr_type, addr);
    }

    /* Default to true if no callback - address is untouched */
    return true;
}

bool mesh_header_needed(const buffer_t *buf)
{
    protocol_interface_info_entry_t *cur;
    if (always_add_mesh_header_as_originator) {
        return true;
    }

    cur = buf->interface;
    if (cur && cur->mesh_callbacks && cur->mesh_callbacks->header_needed) {
        if (cur->mesh_callbacks->header_needed(cur, buf)) {
            return true;
        }
    }

    return false;
}

uint_fast8_t mesh_header_size(const buffer_t *buf)
{
    if (!mesh_header_needed(buf)) {
        return 0;
    }

    uint_fast8_t hdrlen = 1;
    if (MESH_DEFAULT_HOPS_LEFT > 14) {
        hdrlen++;
    }

    if (buf->src_sa.addr_type == ADDR_802_15_4_LONG) {
        hdrlen += 8;
    } else {
        hdrlen += 2;
    }

    if (buf->dst_sa.addr_type == ADDR_802_15_4_LONG) {
        hdrlen += 8;
    } else {
        hdrlen += 2;
        if (buf->dst_sa.addr_type == ADDR_BROADCAST) {
            hdrlen += 2;    // LOWPAN_BC0 header
        }
    }

    return hdrlen;
}

uint_fast8_t mesh_header_len_from_type_byte(uint8_t type)
{
    if ((type & LOWPAN_MESH_MASK) != LOWPAN_MESH) {
        return 0;
    }

    uint_fast8_t hdrlen = 1;
    hdrlen += type & 0x20 ? 2 : 8;
    hdrlen += type & 0x10 ? 2 : 8;
    if ((type & 0x0F) == 0x0F) {
        hdrlen += 1;
    }

    return hdrlen;
}

uint_fast8_t mesh_header_len_from_buffer_type_byte(const buffer_t *buf)
{
    if (buffer_data_length(buf) == 0) {
        return 0;
    }

    uint8_t type = buffer_data_pointer(buf)[0];
    return mesh_header_len_from_type_byte(type);
}

/* If header points to a Mesh Header, and it has a following BC0 header, give
 * it a new sequence number.
 */
void mesh_rewrite_bc0_header(uint8_t *header)
{
    uint_fast8_t mesh_len = mesh_header_len_from_type_byte(header[0]);
    if (mesh_len) {
        header += mesh_len;
        if (header[0] == LOWPAN_DISPATCH_BC0) {
            header[1] = mesh_bc0_sequence++;
        }
    }
}

/* On entry, buf is an unfragmented 6LoWPAN packet, starting with an
 * IPHC header.
 * buf->dst_sa is set, and type may be ADDR_802_15_4_SHORT, LONG or BROADCAST
 *   (if BROADCAST, it may contain a RFC 4944 S.9 multicast address)
 * buf->src_sa is set, and type may be ADDR_802_15_4_SHORT or LONG
 */
buffer_t *mesh_down(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur;
    uint8_t mesh_sz = mesh_header_size(buf);
    if (mesh_sz == 0) {
        /* Shouldn't have come here - should have gone straight to MAC */
        tr_error("mesh_down unexpected");
        return buffer_free(buf);
    }
    buf = buffer_headroom(buf, mesh_sz);
    if (!buf) {
        tr_warn("mesh_down headroom");
        return NULL;
    }

    cur = buf->interface;
    if (!cur) {
        return buffer_free(buf);
    }

    uint8_t *ptr = buffer_data_reserve_header(buf, mesh_sz);
    uint_fast8_t src_addrlen = 8, dst_addrlen = 8;
    *ptr = LOWPAN_MESH;
    if (buf->src_sa.addr_type != ADDR_802_15_4_LONG) {
        /* Source is ADDR_802_15_4_SHORT */
        *ptr |= 0x20;
        src_addrlen = 2;
    }
    if (buf->dst_sa.addr_type != ADDR_802_15_4_LONG) {
        /* Dest is ADDR_802_15_4_SHORT or ADDR_BROADCAST */
        *ptr |= 0x10;
        dst_addrlen = 2;
    }
    if (MESH_DEFAULT_HOPS_LEFT > 14) {
        *ptr++ |= 0xF;
        *ptr++ = MESH_DEFAULT_HOPS_LEFT;
    } else {
        *ptr++ |= MESH_DEFAULT_HOPS_LEFT;
    }

    memcpy(ptr, buf->src_sa.address + 2, src_addrlen);
    ptr += src_addrlen;

    memcpy(ptr, buf->dst_sa.address + 2, dst_addrlen);
    ptr += dst_addrlen;

    if (buf->dst_sa.addr_type == ADDR_BROADCAST) {
        /* Multicast or broadcast */
        *ptr++ = LOWPAN_DISPATCH_BC0;
        *ptr++ = mesh_bc0_sequence++;

        /* We've noted any multicast address in the mesh header; for the MAC
         * it has to be a broadcast. (Actually, in principle could unicast
         * if the routing said it was appropriate, but RFC 4944 says broadcast)
         */
        common_write_16_bit(0xffff, buf->dst_sa.address + 2);
    } else {
        /* Determine first hop - default behaviour is to just transmit to the
         * final destination (useful for testing). But if we have a first-hop
         * function, ask it where to send it. If it doesn't know, then drop the
         * packet.
         */
        if (cur->mesh_callbacks && cur->mesh_callbacks->first_hop) {
            mesh_routing_route_response_t route;
            if (cur->mesh_callbacks->first_hop(cur, dst_addrlen, buf->dst_sa.address + 2, &route)) {
                tr_warn("mesh_down no first hop to %s", trace_array(buf->dst_sa.address + 2, dst_addrlen));
                socket_tx_buffer_event_and_free(buf, SOCKET_NO_ROUTE);
                return NULL;
            }

            /* Modify buffer dest MAC address to first hop response */
            buf->dst_sa.addr_type = route.addr_len == 2 ? ADDR_802_15_4_SHORT : ADDR_802_15_4_LONG;
            memcpy(buf->dst_sa.address + 2, route.address, route.addr_len);
        }
    }

    /* We're the originator here, so address in buf->src_sa that we already used
     * as in the Mesh header is also what we want in the MAC header */
    buf->info = (buffer_info_t)(B_FROM_MESH_ROUTING | B_TO_MAC | B_DIR_DOWN);
    buf->options.code = 0;

    return buf;
}

#endif
