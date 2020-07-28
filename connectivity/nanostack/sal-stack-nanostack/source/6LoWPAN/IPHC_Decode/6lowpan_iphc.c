/*
 * Copyright (c) 2013-2018, Arm Limited and affiliates.
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
#define HAVE_CIPV6

#ifdef HAVE_CIPV6
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/IPHC_Decode/iphc_compress.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "MLE/mle.h"

#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"

#define TRACE_GROUP  "iphc"

/* Input: Final IP packet for transmission on link.
 *        Buffer destination = final destination
 *        Buffer source undefined.
 *        Route next hop address set.
 * Output: Buffer destination+source = link-layer addresses
 *         Sent to mesh, LowPAN fragmentation or MAC layers
 */
buffer_t *lowpan_down(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;

    buf->options.type = 0;

    if (!buf->route) {
        tr_debug("lowpan_down route");
        return buffer_free(buf);
    }

    const uint8_t *ip_src = buffer_data_pointer(buf) + 8;
    const uint8_t *next_hop = buf->route->route_info.next_hop_addr;
    bool link_local = addr_is_ipv6_link_local(next_hop);
    bool stable_only = false;

    /* We have IP next hop - figure out the MAC address */
    if (addr_is_ipv6_multicast(next_hop)) {
        buf->dst_sa.addr_type = ADDR_BROADCAST;
        common_write_16_bit(cur->mac_parameters->pan_id, buf->dst_sa.address);
        buf->dst_sa.address[2] = 0x80 | (next_hop[14] & 0x1f);
        buf->dst_sa.address[3] = next_hop[15];
        stable_only = true;
    } else { /* unicast */
        ipv6_neighbour_t *n = ipv6_interface_resolve_new(cur, buf);
        if (!n) {
            return NULL;
        }
        if (thread_info(cur)) {
            stable_only = thread_stable_context_check(cur, buf);
        }
    }

    if (!buf->link_specific.ieee802_15_4.useDefaultPanId) {
        /* Override dest PAN ID (from multicast map above, or neighbour cache) */
        common_write_16_bit(buf->link_specific.ieee802_15_4.dstPanId, buf->dst_sa.address);
    }

    /* Figure out which source MAC address to use. Usually try to match the
     * source, for best compression, and to ensure if the layer above uses LL64
     * (like MLE), it forces us to use our MAC64.
     */
    if (thread_info(cur) && !(link_local && thread_insist_that_mesh_isnt_a_link(cur)) && buf->dst_sa.addr_type == ADDR_802_15_4_SHORT) {
        /* For Thread, we want to always use short source address for unicast
         * to non-link-local 16-bit addresses, which is the case where we want
         * to use mesh headers.
         */
        buf->src_sa.addr_type = ADDR_802_15_4_SHORT;
    } else if (addr_iid_matches_eui64(ip_src + 8, cur->mac)) {
        buf->src_sa.addr_type = ADDR_802_15_4_LONG;
    } else if (cur->mac_parameters->mac_short_address < 0xfffe && addr_iid_matches_lowpan_short(ip_src + 8, cur->mac_parameters->mac_short_address)) {
        buf->src_sa.addr_type = ADDR_802_15_4_SHORT;
    } else {
        /* This lets mac_mlme_write_our_addr choose based on address mode */
        buf->src_sa.addr_type = ADDR_NONE;
    }

    if (!mac_helper_write_our_addr(cur, &buf->src_sa)) {
        return buffer_free(buf);
    }

    /* Clear Link Layer Re Transmission Counter */
    //buf->fhss_channel_retries_left = 1+ cur->mac_parameters->number_of_fhss_channel_retries;


    if (buf->dst_sa.addr_type != ADDR_802_15_4_LONG && buf->dst_sa.addr_type != ADDR_802_15_4_SHORT && buf->dst_sa.addr_type != ADDR_BROADCAST) {
        tr_debug("IP:Dest Pro. addr_type: %02x", buf->dst_sa.addr_type);
        return buffer_free(buf);
    }

    uint_fast8_t mesh_size;
    if (link_local && thread_insist_that_mesh_isnt_a_link(cur)) {
        mesh_size = 0;
    } else {
        /* Allow the link-layer destination addresses passed from upper layers
         * to be remapped - used to implement Thread anycast.
         *
         * Mapping function can change address and type - if it returns false,
         * packet is dropped.
         *
         * Note that this mapping has to be done before IPHC compression, which
         * is why it moved from mesh.c.
         */
        if (!mesh_address_map(cur, &buf->dst_sa.addr_type, buf->dst_sa.address)) {
            tr_debug("mesh_address_map fail");
            return buffer_free(buf);
        }

        /* After mapping, compute final mesh header size (which depends on
         * the final address).
         */
        mesh_size = mesh_header_size(buf);
    }

    if (mesh_size == 0) {
        if (buf->dst_sa.addr_type == ADDR_BROADCAST) {
            /* Thread says multicasts other than MLE are sent to our parent, if we're an end device */
            if (cur->ip_multicast_as_mac_unicast_to_parent && !buf->options.ll_broadcast_tx) {
                if (protocol_6lowpan_interface_get_mac_coordinator_address(cur, &buf->dst_sa) < 0) {
                    tr_warn("IP: No parent for multicast as unicast");
                    return buffer_free(buf);
                }
            } else {
                /*
                 * Not using a mesh header, so have to "purify" RFC 4944 multicast - we
                 * set a 100xxxxxxxxxxxxx RFC 4944 multicast address above, but
                 * IEEE 802.15.4 only supports broadcast in the real MAC header.
                 */
                common_write_16_bit(0xFFFF, buf->dst_sa.address + 2);
            }
        }
    }

    /* RFC 6282+4944 require that we limit compression to the first fragment.
     * This check is slightly conservative - always allow 4 for first-fragment header
     */
    uint_fast16_t overhead = mac_helper_frame_overhead(cur, buf);
    uint_fast16_t max_iphc_size = mac_helper_max_payload_size(cur, overhead) - mesh_size - 4;

    buf = iphc_compress(&cur->lowpan_contexts, buf, max_iphc_size, stable_only);
    if (!buf) {
        return NULL;
    }

    if (mesh_size != 0) {
        buf->info = (buffer_info_t)(B_FROM_IPV6_TXRX | B_TO_MESH_ROUTING | B_DIR_DOWN);
        return buf;
    }

    buf->info = (buffer_info_t)(B_FROM_IPV6_TXRX | B_TO_MAC | B_DIR_DOWN);

    return buf;
}

buffer_t *lowpan_up(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;

    /* Reject:
     *    Packets without address
     *    Source broadcast PAN ID
     *    Short source addresses 0xfffe (illegal) and 0xffff (broadcast)
     */
    if (buf->dst_sa.addr_type == ADDR_NONE || buf->src_sa.addr_type == ADDR_NONE ||
            common_read_16_bit(buf->src_sa.address) == 0xffff ||
            (buf->dst_sa.addr_type == ADDR_802_15_4_SHORT && common_read_16_bit(buf->src_sa.address + 2) > 0xfffd)) {
        goto drop;
    }

    /* If our PAN ID is set to 0xffff (eg during beacon scan), the MAC will be
     * receiving all packets to all PANs. "Mute" 6LoWPAN reception in this state.
     */
    if (cur->mac_parameters->pan_id == 0xffff) {
        goto drop;
    }

    const uint8_t *ip_hc = buffer_data_pointer(buf);

    //tr_debug("IP-UP";

    if (buffer_data_length(buf) < 4 || addr_check_broadcast(buf->src_sa.address, buf->src_sa.addr_type) == 0) {
        tr_debug("cipv6_up() Too short or broadcast src");
        goto drop;
    } else if ((ip_hc[0] & LOWPAN_FRAG_MASK) == LOWPAN_FRAG) {
        /* 11 x00xxx: FRAG1/FRAGN (RFC 4944) */
        buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_IPV6_TXRX | B_TO_FRAGMENTATION);
        return buf;
    } else if ((ip_hc[0] & LOWPAN_MESH_MASK) == LOWPAN_MESH) {
        /* 10 xxxxxx: MESH (RFC 4944) */
        buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_IPV6_TXRX | B_TO_MESH_ROUTING);
        return buf;
    } else if (ip_hc[0] == LOWPAN_DISPATCH_IPV6) {
        /* Send this to new handler */
        buffer_data_strip_header(buf, 1);
        buf->ip_routed_up = true;
        buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_IPV6_TXRX | B_TO_IPV6_FWD);
        return buf;
    } else if ((ip_hc[0] & LOWPAN_DISPATCH_IPHC_MASK) != LOWPAN_DISPATCH_IPHC) {
        /* Not handled: LOWPAN_HC1/LOWPAN_BC0/IPv6 (RFC 4944), or extension */
        tr_debug("LOWPAN: %02x %02x", ip_hc[0], ip_hc[1]);
        goto drop;
    }

    /* Divert to new routing system - in final system, MAC/Mesh/Frag should send to IPV6_TXRX layer */
    buf->ip_routed_up = true;
    buf = iphc_decompress(&cur->lowpan_contexts, buf);
    if (buf) {
        buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_IPV6_TXRX | B_TO_IPV6_FWD);
    }
    return buf;

drop:
    protocol_stats_update(STATS_IP_RX_DROP, 1);
    return buffer_free(buf);
}

#endif /* HAVE_CIPV6 */

