/*
 * Copyright (c) 2013-2019, Arm Limited and affiliates.
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
#include "string.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "RPL/rpl_control.h"
#ifdef HAVE_RPL
#include "RPL/rpl_data.h"
#endif
#include "RPL/rpl_protocol.h"
#ifdef HAVE_MPL
#include "MPL/mpl.h"
#endif
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/ip.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/mld.h"
#include "Core/include/ns_socket.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "ip_fsc.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_common.h"

#define TRACE_GROUP "icmp"

static buffer_t *icmpv6_echo_request_handler(struct buffer *buf);

/* Check to see if a message is recognisable ICMPv6, and if so, fill in code/type */
/* This used ONLY for the e.1 + e.2 tests in RFC 4443, to try to avoid ICMPv6 error loops */
/* Packet may be ill-formed, because we are considering an ICMPv6 error response. */
static bool is_icmpv6_msg(buffer_t *buf)
{
    const uint8_t *ptr = buffer_data_pointer(buf);
    uint16_t len = buffer_data_length(buf);

    /* IP header format:
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |Version| Traffic Class |           Flow Label                  |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |         Payload Length        |  Next Header  |   Hop Limit   |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     *    + Source Address (16) + Destination Address (16), total 40
     */
    if (len < IPV6_HDRLEN) {
        return false;
    }
    uint16_t ip_len = common_read_16_bit(ptr + IPV6_HDROFF_PAYLOAD_LENGTH);
    uint8_t nh = ptr[IPV6_HDROFF_NH];
    ptr += IPV6_HDRLEN;
    len -= IPV6_HDRLEN;
    if (ip_len > len) {
        return false;
    }
    len = ip_len;
    while (len) {
        uint16_t hdrlen;
        switch (nh) {
            case IPV6_NH_ICMPV6:
                if (len < 4) {
                    return false;
                }
                buf->options.type = ptr[0];
                buf->options.code = ptr[1];
                return true;
            case IPV6_NH_HOP_BY_HOP:
            case IPV6_NH_DEST_OPT:
            case IPV6_NH_ROUTING:
            case IPV6_NH_MOBILITY:
            case IPV6_NH_HIP:
            case IPV6_NH_SHIM6:
                if (len < 8) {
                    return false;
                }
                nh = ptr[0];
                hdrlen = (ptr[1] + 1) * 8;
                break;
            case IPV6_NH_AUTH:
                if (len < 8) {
                    return false;
                }
                nh = ptr[0];
                hdrlen = (ptr[1] + 2) * 4;
                break;
            default:
                return false;
        }
        if (hdrlen > len || (hdrlen & 7)) {
            return false;
        }
        ptr += hdrlen;
        len -= hdrlen;
    }
    return false;
}

buffer_t *icmpv6_error(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t type, uint8_t code, uint32_t aux)
{
    uint8_t *ptr;

    /* Don't send ICMP errors to improperly-secured packets (they either reach MLE etc successfully, or we just drop) */
    if (buf->options.ll_security_bypass_rx) {
        return buffer_free(buf);
    }

    if (cur == NULL) {
        cur = buf->interface;
    }

    /* Any ICMPv6 error in response to an UP packet implies an RX drop... */
    if ((buf->info & B_DIR_MASK) == B_DIR_UP) {
        protocol_stats_update(STATS_IP_RX_DROP, 1);
    }

    /* RFC 4443 processing rules e.1-2: don't send errors for ICMPv6 errors or redirects */
    if (is_icmpv6_msg(buf) && (buf->options.type < 128 || buf->options.type == ICMPV6_TYPE_INFO_REDIRECT)) {
        return buffer_free(buf);
    }

    /* RFC 4443 processing rules e.3-5: don't send errors for IP multicasts or link-layer multicasts+broadcasts (with exceptions) */
    if (addr_is_ipv6_multicast(buf->dst_sa.address) || buf->options.ll_broadcast_rx || buf->options.ll_multicast_rx) {
        if (!(type == ICMPV6_TYPE_ERROR_PACKET_TOO_BIG ||
                (type == ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM && code == ICMPV6_CODE_PARAM_PRB_UNREC_IPV6_OPT))) {
            return buffer_free(buf);
        }
    }

    /* RFC 4443 processing rule e.6 - source doesn't identify a single node */
    if (addr_is_ipv6_unspecified(buf->src_sa.address) || addr_is_ipv6_multicast(buf->src_sa.address)) {
        return buffer_free(buf);
    }

    if (addr_interface_address_compare(cur, buf->dst_sa.address) == 0) {
        // RFC 4443 2.2 - if addressed to us, use that address as source
        memswap(buf->dst_sa.address, buf->src_sa.address, 16);
    } else {
        // Otherwise we will use normal address selection rule
        buf->dst_sa = buf->src_sa;

        // This makes buffer_route choose the address
        buf->src_sa.addr_type = ADDR_NONE;
    }

    buffer_turnaround(buf);

    if (!ipv6_buffer_route(buf)) {
        return buffer_free(buf);
    }
    cur = buf->interface;

    /* Token-bucket rate limiting */
    if (!cur->icmp_tokens) {
        return buffer_free(buf);
    }
    cur->icmp_tokens--;

    /* Include as much of the original packet as possible, without exceeding
     * minimum MTU of 1280. */
    uint16_t max_payload = ipv6_max_unfragmented_payload(buf, IPV6_MIN_LINK_MTU);
    if (buffer_data_length(buf) > max_payload - 8) {
        buffer_data_length_set(buf, max_payload - 8);
    }

    if ((buf = buffer_headroom(buf, 4)) == NULL) {
        return NULL;
    }
    ptr = buffer_data_reserve_header(buf, 4);
    ptr = common_write_32_bit(aux, ptr);
    buf->options.traffic_class = 0;
    buf->options.type = type;
    buf->options.code = code;
    buf->options.hop_limit = cur->cur_hop_limit;
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);

    return (buf);
}

#ifndef NO_IPV6_PMTUD
/* Look at a (potentially-partial) packet that should be a copy of
 * something we sent from an ICMP error. Identify final destination if we can.
 */
static bool icmpv6_identify_final_destination(buffer_t *buf, uint8_t *dest)
{
    const uint8_t *ptr = buffer_data_pointer(buf);

    /* Start with destination in IP header */
    memcpy(dest, ptr + 24, 16);
#ifdef HAVE_RPL
    /* Have to look for routing header */
    uint8_t nh = ptr[6];
    uint16_t hlen = 40;
    uint16_t len = buffer_data_length(buf);
    ptr += 40;
    len -= 40;
    for (;;) {
        if (len < hlen) {
            return false;
        }
        ptr += hlen;
        len -= hlen;

        /* Only need to process stuff we can send... */
        switch (nh) {
            case IPV6_NH_HOP_BY_HOP:
            case IPV6_NH_DEST_OPT:
                if (len < 2) {
                    return false;
                }
                hlen = (ptr[1] + 1) * 8;
                nh = ptr[0];
                break;
            case IPV6_NH_ROUTING:
                if (len < 4) {
                    return false;
                }
                /* If segments left is zero, IP dest is okay */
                if (ptr[3] == 0) {
                    return true;
                }
                if (ptr[2] != IPV6_ROUTING_TYPE_RPL) {
                    return false;
                }
                hlen = (ptr[1] + 1) * 8;
                if (len < hlen) {
                    return false;
                }
                return rpl_data_get_srh_last_address(ptr, dest);
            case IPV6_NH_FRAGMENT:
            case IPV6_NH_IPV6:
            case IPV6_NH_ICMPV6:
            case IPV6_NH_UDP:
            case IPV6_NH_TCP:
                /* If we've reached this header, it's too late for routing */
                return true;
            default:
                /* Unrecognised header - can't have come from us... */
                return false;
        }
    }
#else
    return true;
#endif
}

buffer_t *icmpv6_packet_too_big_handler(buffer_t *buf)
{
    tr_info("ICMP packet too big from: %s", trace_ipv6(buf->src_sa.address));

    /* Need 4 for MTU, plus at least the IP header */
    if (buffer_data_length(buf) < 4 + 40) {
        return buffer_free(buf);
    }

    protocol_interface_info_entry_t *cur = buf->interface;

    const uint8_t *ptr = buffer_data_pointer(buf);
    uint32_t mtu = common_read_32_bit(ptr);

    /* RFC 8201 - ignore MTU smaller than minimum */
    if (mtu < IPV6_MIN_LINK_MTU) {
        return buffer_free(buf);
    }

    ptr = buffer_data_strip_header(buf, 4);

    /* Check source is us */
    if (addr_interface_address_compare(cur, ptr + 8)) {
        return buffer_free(buf);
    }

    uint8_t final_dest[16];
    if (!icmpv6_identify_final_destination(buf, final_dest)) {
        return buffer_free(buf);
    }

    ipv6_destination_t *dest = ipv6_destination_lookup_or_create(final_dest, cur->id);

    if (dest && mtu < dest->pmtu) {

        tr_info("Reducing PMTU to %"PRIu32" for: %s", mtu, trace_ipv6(final_dest));
        dest->pmtu = mtu;
        dest->pmtu_lifetime = cur->pmtu_lifetime;
    }

    return buffer_free(buf);
}
#endif

static buffer_t *icmpv6_echo_request_handler(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;
    if (!cur) {
        return buffer_free(buf);
    }

    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
    buf->options.type = ICMPV6_TYPE_INFO_ECHO_REPLY;
    buf->options.code = 0x00;
    buf->options.hop_limit = cur->cur_hop_limit;

    if (addr_is_ipv6_multicast(buf->dst_sa.address)) {
        const uint8_t *ipv6_ptr;
        memcpy(buf->dst_sa.address, buf->src_sa.address, 16);
        ipv6_ptr = addr_select_source(cur, buf->dst_sa.address, 0);
        if (!ipv6_ptr) {
            tr_debug("No address");
            return buffer_free(buf);
        }
        memcpy(buf->src_sa.address, ipv6_ptr, 16);
    } else {
        memswap(buf->dst_sa.address, buf->src_sa.address, 16);
    }

    return buffer_turnaround(buf);
}

#ifdef HAVE_IPV6_ND

static void icmpv6_na_wisun_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *dptr, const uint8_t *src_addr)
{
    (void) src_addr;
    dptr += 2;
    uint16_t life_time;
    uint8_t nd_status  = *dptr;
    dptr += 4;
    life_time = common_read_16_bit(dptr);
    dptr += 2;
    if (memcmp(dptr, cur_interface->mac, 8) != 0) {
        return;
    }

    (void)life_time;
    if (nd_status != ARO_SUCCESS) {
        ws_common_aro_failure(cur_interface, src_addr);
    }
}

static void icmpv6_na_aro_handler(protocol_interface_info_entry_t *cur_interface, const uint8_t *dptr, const uint8_t *dst_addr)
{
    (void)dst_addr;
    dptr += 2;
    uint16_t life_time;
    uint8_t nd_status  = *dptr;
    dptr += 4;
    life_time = common_read_16_bit(dptr);
    dptr += 2;
    if (memcmp(dptr, cur_interface->mac, 8) != 0) {
        return;
    }

    /* Failure responses go to LL64, and they thus don't actually indicate the
     * address that we were trying to register. So we have to rely on having
     * "current DAD address" stored. We don't get it from the packet in any case.
     */
    if (!cur_interface->if_6lowpan_dad_process.active) {
        return;
    }

    if_address_entry_t *addr_entry = addr_get_entry(cur_interface, cur_interface->if_6lowpan_dad_process.address);
    if (!addr_entry) {
        return;
    }

    switch (nd_status) {
        case ARO_SUCCESS:
            addr_cb(cur_interface, addr_entry, ADDR_CALLBACK_DAD_COMPLETE);
            if (addr_entry->cb) {
                /* Lifetime is in minutes, state_timer in 1/10 s: a factor of 600 */
                /* Set renewal to 75-85% of full lifetime by multiplying by [450..510] */
                addr_entry->state_timer = life_time * randLIB_get_random_in_range(450, 510);
            }

            break;

        case ARO_DUPLICATE:
            addr_duplicate_detected(cur_interface, addr_entry->address);
            break;

        case ARO_FULL:
            addr_cb(cur_interface, addr_entry, ADDR_CALLBACK_PARENT_FULL);
            break;
    }
}

/*
 *      Neighbor Solicitation Message Format
 *
 *        0                   1                   2                   3
 *        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *       |     Type      |     Code      |          Checksum             |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *       |                           Reserved                            |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *       |                                                               |
 *       +                                                               +
 *       |                                                               |
 *       +                       Target Address                          +
 *       |                                                               |
 *       +                                                               +
 *       |                                                               |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *       |   Options ...
 *       +-+-+-+-+-+-+-+-+-+-+-+-
 *
 *
 *      Source/Target Link-layer Address
 *
 *       0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |     Type      |    Length     |    Link-Layer Address ...
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
static buffer_t *icmpv6_ns_handler(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur;
    uint8_t target[16];
    uint8_t dummy_sllao[16];
    bool proxy = false;
    const uint8_t *sllao;
    const uint8_t *aro;
    uint8_t *dptr = buffer_data_pointer(buf);
    aro_t aro_out = { .present = false };

    cur = buf->interface;

    if (buf->options.code != 0 || buf->options.hop_limit != 255) {
        goto drop;
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 20)) {
        goto drop;
    }

    sllao = icmpv6_find_option_in_buffer(buf, 20, ICMPV6_OPT_SRC_LL_ADDR, 0);

    /* If no SLLAO, ignore ARO (RFC 6775 6.5) */
    /* This rule can be bypassed by setting flag "use_eui64_as_slla_in_aro" to true */
    if (cur->ipv6_neighbour_cache.recv_addr_reg &&
            (cur->ipv6_neighbour_cache.use_eui64_as_slla_in_aro || sllao)) {
        aro = icmpv6_find_option_in_buffer(buf, 20, ICMPV6_OPT_ADDR_REGISTRATION, 0);
    } else {
        aro = NULL;
    }

    /* ARO's length must be 2 and status must be 0 */
    if (aro && (aro[1] != 2 || aro[2] != 0)) {
        goto drop;
    }

    /* If there was no SLLAO on ARO, use mac address to create dummy one... */
    if (aro && !sllao && cur->ipv6_neighbour_cache.use_eui64_as_slla_in_aro) {
        dummy_sllao[0] = ICMPV6_OPT_SRC_LL_ADDR;    // Type
        dummy_sllao[1] = 2;                         // Length = 2x8 bytes
        memcpy(dummy_sllao + 2, aro + 8, 8);        // EUI-64
        memset(dummy_sllao + 10, 0, 6);             // Padding

        sllao = dummy_sllao;
    }
    // Skip the 4 reserved bytes
    dptr += 4;

    // Copy the target IPv6 address
    memcpy(target, dptr, 16);
    dptr += 16;

    if (addr_is_ipv6_multicast(target)) {
        goto drop;
    }

    if (addr_is_ipv6_unspecified(buf->src_sa.address)) {
        /* Dest must be to solicited-node multicast, without source LL-addr */
        if (sllao || memcmp(buf->dst_sa.address, ADDR_MULTICAST_SOLICITED, 13) != 0) {
            goto drop;
        }

        /* If unspecified source, ignore ARO (RFC 6775 6.5) */
        aro = NULL;
    }

    /* See RFC 4862 5.4.3 - hook for Duplicate Address Detection */
    if (addr_is_tentative_for_interface(cur, target)) {
        if (addr_is_ipv6_unspecified(buf->src_sa.address)) {
            tr_debug("Received DAD NS for our tentative address");
            /* Someone else is performing DAD */
            addr_duplicate_detected(cur, target);
        }
        goto drop;
    }

    /* This first check's a bit dodgy - it responds to our address on the other
     * interface, which we should only do in the whiteboard case.
     */
    if (addr_interface_address_compare(cur, target) != 0) {
        //tr_debug("Received  NS for proxy %s", trace_ipv6(target));

        proxy = true;
        //Filter Link Local scope
        if (addr_is_ipv6_link_local(target)) {
            goto drop;
        }

        if (!nd_proxy_enabled_for_downstream(cur->id) || !nd_proxy_target_address_validation(cur->id, target)) {
            goto drop;
        }
    }

    if (aro) {
        /* If it had an ARO, and we're paying attention to it, possibilities:
         * 1) No reply to NS now, we need to contact border router (false return)
         * 2) Reply to NS now, with ARO (true return, aro_out.present true)
         * 3) Reply to NS now, without ARO (true return, aro_out.present false)
         */
        if (!nd_ns_aro_handler(cur, aro, sllao, buf->src_sa.address, &aro_out)) {
            goto drop;
        }
    }

    /* If we're returning an ARO, then we assume the ARO handler has done the
     * necessary to the Neighbour Cache. Otherwise, normal RFC 4861 processing. */
    if (!aro_out.present &&
            sllao && cur->if_llao_parse(cur, sllao, &buf->dst_sa)) {
        ipv6_neighbour_update_unsolicited(&cur->ipv6_neighbour_cache, buf->src_sa.address, buf->dst_sa.addr_type, buf->dst_sa.address);
    }

    buffer_t *na_buf = icmpv6_build_na(cur, true, !proxy, addr_is_ipv6_multicast(buf->dst_sa.address), target, aro_out.present ? &aro_out : NULL, buf->src_sa.address);

    buffer_free(buf);

    return na_buf;

drop:
    buf = buffer_free(buf);

    return buf;

}

int icmpv6_slaac_prefix_update(struct protocol_interface_info_entry *cur, const uint8_t *prefix_ptr, uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime)
{
    int ret_val = -1;

    //Validate first current list If prefix is already defined adress
    ns_list_foreach_safe(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == ADDR_SOURCE_SLAAC && (e->prefix_len == prefix_len) && bitsequal(e->address, prefix_ptr, prefix_len)) {

            addr_lifetime_update(cur, e, valid_lifetime, preferred_lifetime, 2 * 60 * 60);
            ret_val = 0;
        }
    }
    return ret_val;
}

void icmpv6_slaac_prefix_register_trig(struct protocol_interface_info_entry *cur, uint8_t *prefix_ptr, uint8_t prefix_len)
{

    //Validate first current list If prefix is already defined adress
    ns_list_foreach(if_address_entry_t, e, &cur->ip_addresses) {
        if (e->source == ADDR_SOURCE_SLAAC && (e->prefix_len == prefix_len) && bitsequal(e->address, prefix_ptr, prefix_len)) {
            e->state_timer = 150;
        }
    }
}
#endif // HAVE_IPV6_ND

if_address_entry_t *icmpv6_slaac_address_add(protocol_interface_info_entry_t *cur, const uint8_t *prefix_ptr, uint8_t prefix_len, uint32_t valid_lifetime, uint32_t preferred_lifetime, bool skip_dad, slaac_src_e slaac_src)
{
    if_address_entry_t *address_entry;
    uint8_t ipv6_address[16];
    //define Autonomous address generation

    if (prefix_len != 64) {
        return NULL;
    }

    if (slaac_src == SLAAC_IID_DEFAULT && cur->opaque_slaac_iids && addr_opaque_iid_key_is_set()) {
        slaac_src = SLAAC_IID_OPAQUE;
    }

    memcpy(ipv6_address, prefix_ptr, 8);
    switch (slaac_src) {
        case SLAAC_IID_DEFAULT:
        case SLAAC_IID_FIXED:
            memcpy(ipv6_address + 8, cur->iid_slaac, 8);
            break;
        case SLAAC_IID_EUI64:
            memcpy(ipv6_address + 8, cur->iid_eui64, 8);
            break;
        case SLAAC_IID_OPAQUE:
            addr_generate_opaque_iid(cur, ipv6_address);
            break;
        case SLAAC_IID_6LOWPAN_SHORT:
            if (cur->nwk_id != IF_6LoWPAN || !cur->mac_parameters) {
                return NULL;
            }
            memcpy(ipv6_address + 8, ADDR_SHORT_ADR_SUFFIC, 6);
            common_write_16_bit(cur->lowpan_desired_short_address, ipv6_address + 14);
            break;

        default:
            return NULL;
    }

    //tr_debug("Add add: %s", trace_ipv6(ipv6_address));

    address_entry = addr_add(cur, ipv6_address, 64, ADDR_SOURCE_SLAAC, valid_lifetime, preferred_lifetime, skip_dad);
    if (address_entry) {
        address_entry->cb = NULL;
    }
    return address_entry;
}

#ifdef HAVE_IPV6_ND
static buffer_t *icmpv6_ra_handler(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur;
    uint8_t flags, hoplimit;
    uint32_t reachable_time, retrans_timer;
    uint16_t data_len, router_lifetime;
    uint8_t *dptr;
    ipv6_neighbour_t *ncache_entry = NULL;
    int_fast8_t preference = 0;
    uint32_t longest_route_lifetime = 0;

    if (!buf) {
        return NULL;
    }

    cur = buf->interface;

    // If both route and prefix receive are disabled do not process any RA options
    if (!cur->recv_ra_routes && !cur->recv_ra_prefixes) {
        return buffer_free(buf);
    }

    // Drop RA If LL address generation is not ready
    if (addr_interface_get_ll_address(cur, NULL, 0) < 0) {
        return buffer_free(buf);
    }

    if (cur->nwk_id == IF_6LoWPAN) {
        // XXX this check needs to be for host bootstrap only
        if ((cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) == 0) {
            if (protocol_6lowpan_interface_compare_cordinator_netid(cur, &(buf->src_sa.address[8])) != 0) {
                return buffer_free(buf);
            }
        }
    }

    if (!addr_is_ipv6_link_local(buf->src_sa.address) || buf->options.hop_limit != 255 || buf->options.code != 0) {
        return buffer_free(buf);
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 12)) {
        tr_debug("Malformed RA");
        return buffer_free(buf);
    }

    /* Token-bucket rate limiting */
    if (!cur->icmp_ra_tokens) {
        return buffer_free(buf);
    }
    cur->icmp_ra_tokens--;

    data_len = buffer_data_length(buf);
    dptr = buffer_data_pointer(buf);
    //tr_debug("RX RA: %s", trace_ipv6(buf->src_sa.address));

    /* XXX we always set variables based on RAs; fine for a host,
     * but wrong/iffy for a router. But then so is doing SLAAC as a
     * router...
     */
    hoplimit = *dptr++;
    if (hoplimit) {
        cur->cur_hop_limit = hoplimit;
    }

    //Read Flags
    flags = *dptr++;
    router_lifetime = common_read_16_bit(dptr);
    dptr += 2;

    reachable_time = common_read_32_bit(dptr);
    dptr += 4;
    if (reachable_time != 0 && reachable_time != cur->base_reachable_time) {
        protocol_stack_interface_set_reachable_time(cur, reachable_time);
    }

    retrans_timer = common_read_32_bit(dptr);
    dptr += 4;
    if (retrans_timer != 0) {
        cur->ipv6_neighbour_cache.retrans_timer = retrans_timer;
    }

    const uint8_t *mtu_option = icmpv6_find_option_in_buffer(buf, 12, ICMPV6_OPT_MTU, 1);
    uint32_t mtu = mtu_option ? common_read_32_bit(mtu_option + 4) : 0;

    sockaddr_t ll_addr = { .addr_type = ADDR_NONE };
    const uint8_t *sllao = icmpv6_find_option_in_buffer(buf, 12, ICMPV6_OPT_SRC_LL_ADDR, 0);
    if (sllao) {
        if (cur->if_llao_parse(cur, sllao, &ll_addr)) {
            ncache_entry = ipv6_neighbour_update_unsolicited(&cur->ipv6_neighbour_cache, buf->src_sa.address, ll_addr.addr_type, ll_addr.address);
        }
    }

    buffer_data_strip_header(buf, 12);
    data_len -= 12;

    if (cur->nwk_id == IF_6LoWPAN) {
        const uint8_t *abro;
        bool uptodate;

        /* ABRO processing - aiming to separate this from standard processing; only 6LRs need to use ABROs */
        abro = icmpv6_find_option_in_buffer(buf, 0, ICMPV6_OPT_AUTHORITATIVE_BORDER_RTR, 3);
        if (abro) {
            uptodate = nd_ra_process_abro(cur, buf, abro + 2, flags, router_lifetime);
            /* If ABRO processing indicated stale info, skip normal processing */
            if (!uptodate) {
                goto drop;
            }
#ifndef NO_RADV_TX
            if (hoplimit != 0) {
                cur->adv_cur_hop_limit = hoplimit;
            }
            if (reachable_time != 0) {
                cur->adv_reachable_time = reachable_time;
            }
            if (retrans_timer != 0) {
                cur->adv_retrans_timer = retrans_timer;
            }
            if (mtu != 0) {
                cur->adv_link_mtu = mtu;
            }
#endif
        }
    }
    if (cur->recv_ra_routes) {
        if (router_lifetime) {
            tr_debug("Possible Default Router");
            switch (flags & RA_PRF_MASK) {
                case RA_PRF_LOW:
                    preference = -1;
                    break;
                case RA_PRF_HIGH:
                    preference = +1;
                    break;
                default:
                    preference =  0;
                    break; // invalid is treated as 0
            }
            if (router_lifetime > longest_route_lifetime) {
                longest_route_lifetime = router_lifetime;
            }
        }
        ipv6_route_add(NULL, 0, cur->id, buf->src_sa.address, ROUTE_RADV, router_lifetime, preference);
    }

    if (mtu >= IPV6_MIN_LINK_MTU && mtu <= cur->max_link_mtu) {
        cur->ipv6_neighbour_cache.link_mtu = mtu;
    }

    //Scan All options
    while (data_len) {
        uint8_t type = *dptr++;
        uint16_t length = *dptr++ * 8;

        if (type == ICMPV6_OPT_PREFIX_INFO && cur->recv_ra_prefixes && length == 32) {
            uint8_t *ptr = dptr;
            uint8_t prefix_length = *ptr++;
            uint8_t prefix_flags = *ptr++;

            uint32_t valid_lifetime = common_read_32_bit(ptr);
            ptr += 4;
            uint32_t preferred_lifetime = common_read_32_bit(ptr);
            ptr += 8; //Update 32-bit time and reserved 32-bit
            const uint8_t *prefix_ptr = ptr;

            //Check is L Flag active
            if (prefix_flags & PIO_L) {
                //define ONLink Route Information
                //tr_debug("Register On Link Prefix to routing table");
                ipv6_route_add(prefix_ptr, prefix_length, cur->id, NULL, ROUTE_RADV, valid_lifetime, 0);
            }

            //Check if A-Flag
            if (prefix_flags & PIO_A) {
                if (icmpv6_slaac_prefix_update(cur, prefix_ptr, prefix_length, valid_lifetime, preferred_lifetime) != 0) {
                    ipv6_interface_slaac_handler(cur, prefix_ptr, prefix_length, valid_lifetime, preferred_lifetime);
                }

                //tr_debug("Prefix: %s", trace_ipv6(prefix_ptr));
            }

            // If the R flag is set and we have SLLAO, let's add a neighbour cache entry.
            // This helps reduce RPL noise with source routing - may otherwise be good.
            // Note that existence of a neighbour cache entry doesn't affect routing - we
            // won't use it unless we otherwise decide they're on-link, eg from a source
            // routing header directing to them
            if ((prefix_flags & PIO_R) && ll_addr.addr_type != ADDR_NONE) {
                ipv6_neighbour_update_unsolicited(&cur->ipv6_neighbour_cache, prefix_ptr, ll_addr.addr_type, ll_addr.address);
            }
        } else if (type == ICMPV6_OPT_ROUTE_INFO && cur->recv_ra_routes) {
            uint8_t prefix_length = dptr[0];
            uint8_t route_flags = dptr[1];
            uint32_t route_lifetime = common_read_32_bit(dptr + 2);
            uint8_t *prefix_ptr = dptr + 6;
            // Check option is long enough for prefix
            if (length < 8 + (prefix_length + 7u) / 8) {
                goto next_option;
            }
            switch (route_flags & RA_PRF_MASK) {
                case RA_PRF_LOW:
                    preference = -1;
                    break;
                case RA_PRF_MEDIUM:
                    preference =  0;
                    break;
                case RA_PRF_HIGH:
                    preference = +1;
                    break;
                default:
                    goto next_option; // invalid not accepted
            }
            if (route_lifetime > longest_route_lifetime) {
                longest_route_lifetime = route_lifetime;
            }

            //Call route Update
            tr_info("Route: %s Lifetime: %lu Pref: %d", trace_ipv6_prefix(prefix_ptr, prefix_length), (unsigned long) route_lifetime, preference);
            if (route_lifetime) {
                ipv6_route_add(prefix_ptr, prefix_length, cur->id, buf->src_sa.address, ROUTE_RADV, route_lifetime, preference);
            } else {
                ipv6_route_delete(prefix_ptr, prefix_length, cur->id, buf->src_sa.address, ROUTE_RADV);
            }
        } else if (type == ICMPV6_OPT_6LOWPAN_CONTEXT) {
            nd_ra_process_lowpan_context_option(cur, dptr - 2);
        }
next_option:
        //UPdate length and
        data_len -= length;
        dptr += length - 2;
    }

    /* RFC 4861 says to always set IsRouter on receipt of any RA, but this
     * seems to make more sense - a shutting-down router (sending the final
     * MAX_FINAL_RTR_ADVERTISEMENTS), or a router advertising only prefixes
     * shouldn't really be marked as a router. So only set IsRouter if
     * a route was advertised (with non-0 lifetime).
     */
    if (longest_route_lifetime) {
        if (!ncache_entry) {
            ncache_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, buf->src_sa.address);
        }

        if (ncache_entry) {
            ncache_entry->is_router = true;
        }
    }

drop:
    return buffer_free(buf);
}

void icmpv6_recv_ra_routes(protocol_interface_info_entry_t *cur, bool enable)
{
    if (cur->recv_ra_routes != enable) {
        cur->recv_ra_routes = enable;
        if (!enable) {
            ipv6_route_table_remove_info(cur->id, ROUTE_RADV, NULL);
        }
    }
}

void icmpv6_recv_ra_prefixes(protocol_interface_info_entry_t *cur, bool enable)
{
    if (cur->recv_ra_prefixes != enable) {
        cur->recv_ra_prefixes = enable;
        if (!enable) {
            addr_set_non_preferred(cur, ADDR_SOURCE_SLAAC);
        }
    }
}

static buffer_t *icmpv6_redirect_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    const uint8_t *ptr = buffer_data_pointer(buf);
    const uint8_t *tgt = ptr + 4;
    const uint8_t *dest = ptr + 20;
    sockaddr_t tgt_ll = { .addr_type = ADDR_NONE };

    if (buf->options.hop_limit != 255) {
        goto drop;
    }

    if (!addr_is_ipv6_link_local(buf->src_sa.address)) {
        goto drop;
    }

    if (buf->options.code != 0) {
        goto drop;
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 36)) {
        goto drop;
    }

    if (addr_is_ipv6_multicast(dest)) {
        goto drop;
    }

    const uint8_t *tllao = icmpv6_find_option_in_buffer(buf, 36, ICMPV6_OPT_TGT_LL_ADDR, 0);
    if (tllao) {
        cur->if_llao_parse(cur, tllao, &tgt_ll);
    }
    ipv6_destination_redirect(tgt, buf->src_sa.address, dest, buf->interface->id, tgt_ll.addr_type, tgt_ll.address);
    return buffer_free(buf);

drop:
    tr_warn("Redirect drop");
    return buffer_free(buf);
}

static buffer_t *icmpv6_na_handler(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur;
    uint8_t *dptr = buffer_data_pointer(buf);
    uint8_t flags;
    const uint8_t *target;
    const uint8_t *tllao;
    if_address_entry_t *addr_entry;
    ipv6_neighbour_t *neighbour_entry;

    //"Parse NA at IPv6\n");

    if (buf->options.code != 0 || buf->options.hop_limit != 255) {
        goto drop;
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 20)) {
        goto drop;
    }

    // Skip the 4 reserved bytes
    flags = *dptr;
    dptr += 4;

    // Note the target IPv6 address
    target = dptr;

    if (addr_is_ipv6_multicast(target)) {
        goto drop;
    }

    /* Solicited flag must be clear if sent to a multicast address */
    if (addr_is_ipv6_multicast(buf->dst_sa.address) && (flags & NA_S)) {
        goto drop;
    }

    cur = buf->interface;

    /* RFC 4862 5.4.4 DAD checks */
    addr_entry = addr_get_entry(cur, target);
    if (addr_entry) {
        if (addr_entry->tentative) {
            tr_debug("Received NA for our tentative address");
            addr_duplicate_detected(cur, target);
        } else {
            tr_debug("NA received for our own address: %s", trace_ipv6(target));
        }
        goto drop;
    }

    const uint8_t *aro = icmpv6_find_option_in_buffer(buf, 20, ICMPV6_OPT_ADDR_REGISTRATION, 2);
    if (aro) {
        if (cur->ipv6_neighbour_cache.recv_na_aro) {
            icmpv6_na_aro_handler(cur, aro, buf->dst_sa.address);
        }
        if (ws_info(cur)) {
            icmpv6_na_wisun_aro_handler(cur, aro, buf->src_sa.address);
        }
    }

    /* No need to create a neighbour cache entry if one doesn't already exist */
    neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, target);
    if (!neighbour_entry) {
        goto drop;
    }

    tllao = icmpv6_find_option_in_buffer(buf, 20, ICMPV6_OPT_TGT_LL_ADDR, 0);
    if (!tllao || !cur->if_llao_parse(cur, tllao, &buf->dst_sa)) {
        buf->dst_sa.addr_type = ADDR_NONE;
    }

    ipv6_neighbour_update_from_na(&cur->ipv6_neighbour_cache, neighbour_entry, flags, buf->dst_sa.addr_type, buf->dst_sa.address);
    if (ws_info(cur) && neighbour_entry->state == IP_NEIGHBOUR_REACHABLE) {
        tr_debug("NA neigh update");
        ws_common_neighbor_update(cur, target);
    }

drop:
    return buffer_free(buf);
}
#endif // HAVE_IPV6_ND

buffer_t *icmpv6_up(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = NULL;
    uint8_t *dptr = buffer_data_pointer(buf);
    uint16_t data_len = buffer_data_length(buf);

    cur = buf->interface;



    if (data_len < 4) {
        //tr_debug("Ic1");
        goto drop;

    }

    buf->options.type = *dptr++;
    buf->options.code = *dptr++;

    if (buf->options.ll_security_bypass_rx) {
        if (!ws_info(buf->interface)
                || (buf->options.type == ICMPV6_TYPE_INFO_RPL_CONTROL
                    && (buf->options.code != ICMPV6_CODE_RPL_DIO
                        && buf->options.code != ICMPV6_CODE_RPL_DIS))) {
            goto drop;
        }
    }

    /* Check FCS first */
    if (buffer_ipv6_fcf(buf, IPV6_NH_ICMPV6)) {
        tr_warn("ICMP cksum error!");
        protocol_stats_update(STATS_IP_CKSUM_ERROR, 1);
        goto drop;
    }

    //Skip ICMP Header Static 4 bytes length
    buffer_data_strip_header(buf, 4);

    if (cur->if_icmp_handler) {
        bool bounce = false;
        buf = cur->if_icmp_handler(cur, buf, &bounce);
        if (!buf || bounce) {
            return buf;
        }
    }

    switch (buf->options.type) {
#ifdef HAVE_IPV6_ND
        case ICMPV6_TYPE_INFO_RS:
            buf = icmpv6_rs_handler(buf, cur);
            break;

        case ICMPV6_TYPE_INFO_RA:
            buf = icmpv6_ra_handler(buf);
            break;

        case ICMPV6_TYPE_INFO_NS:
            buf = icmpv6_ns_handler(buf);
            break;

        case ICMPV6_TYPE_INFO_NA:
            buf = icmpv6_na_handler(buf);
            break;

        case ICMPV6_TYPE_INFO_REDIRECT:
            buf = icmpv6_redirect_handler(buf, cur);
            break;
#endif

        case ICMPV6_TYPE_INFO_ECHO_REQUEST:
            tr_debug("ICMP echo request from: %s", trace_ipv6(buf->src_sa.address));
            buf = icmpv6_echo_request_handler(buf);
            break;

        case ICMPV6_TYPE_INFO_ECHO_REPLY:
            ipv6_neighbour_reachability_confirmation(buf->src_sa.address, buf->interface->id);
        /* fall through */

        case ICMPV6_TYPE_ERROR_DESTINATION_UNREACH:
#ifdef HAVE_RPL_ROOT
            if (buf->options.type == ICMPV6_TYPE_ERROR_DESTINATION_UNREACH && buf->options.code == ICMPV6_CODE_DST_UNREACH_SRC_RTE_HDR_ERR) {
                buf = rpl_control_source_route_error_handler(buf, cur);
            }
#endif
        /* fall through */

        default:
            if (buf) {
                buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_APP | B_DIR_UP);
                buf->options.type = (uint8_t) SOCKET_FAMILY_IPV6;
                buf->options.code = IPV6_NH_ICMPV6;
                buf->dst_sa.port = 0xffff;
                /* Put back ICMP header */
                buffer_data_reserve_header(buf, 4);
            }
            break;

        case ICMPV6_TYPE_INFO_MCAST_LIST_REPORT:
            buf = mld_report_handler(buf, cur);
            break;

        case ICMPV6_TYPE_INFO_MCAST_LIST_QUERY:
            buf = mld_query_handler(buf, cur);
            break;

#ifndef NO_IPV6_PMTUD
        case ICMPV6_TYPE_ERROR_PACKET_TOO_BIG:
            buf = icmpv6_packet_too_big_handler(buf);
            break;
#endif

#ifdef HAVE_RPL
        case ICMPV6_TYPE_INFO_RPL_CONTROL:
            buf = rpl_control_handler(buf);
            break;
#endif

#ifdef HAVE_MPL
        case ICMPV6_TYPE_INFO_MPL_CONTROL:
            buf = mpl_control_handler(buf, cur);
            break;
#endif

#ifdef HAVE_6LOWPAN_BORDER_ROUTER
        case ICMPV6_TYPE_INFO_DAR:

            if (cur->nwk_id == IF_6LoWPAN) {
                if (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
                    buf = nd_dar_parse(buf, cur);
                    break;
                }
            }
            goto drop;
#endif
#ifdef HAVE_6LOWPAN_ROUTER
        case ICMPV6_TYPE_INFO_DAC:
            if (cur->nwk_id == IF_6LoWPAN) {
                if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY) {
                    buf = nd_dac_handler(buf, cur);
                    break;
                }
            }
            goto drop;
#endif

    }

    return buf;

drop:
    return buffer_free(buf);
}

buffer_t *icmpv6_down(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;

    buf = buffer_headroom(buf, 4);
    if (buf) {
        uint8_t *dptr;
        dptr = buffer_data_reserve_header(buf, 4);
        buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_IPV6 | B_DIR_DOWN);

        if (buf->src_sa.addr_type != ADDR_IPV6) {
            if (addr_interface_select_source(cur, buf->src_sa.address, buf->dst_sa.address, 0) != 0) {
                tr_err("ICMP:InterFace Address Get Fail--> free Buffer");
                return buffer_free(buf);
            } else {
                buf->src_sa.addr_type = ADDR_IPV6;
            }

        }

        *dptr++ = buf->options.type;
        *dptr++ = buf->options.code;
        common_write_16_bit(0, dptr);
        common_write_16_bit(buffer_ipv6_fcf(buf, IPV6_NH_ICMPV6), dptr);
        buf->options.type = IPV6_NH_ICMPV6;
        buf->options.code = 0;
        buf->options.traffic_class &= ~IP_TCLASS_ECN_MASK;
    }
    return (buf);
}

#ifdef HAVE_IPV6_ND
buffer_t *icmpv6_build_rs(protocol_interface_info_entry_t *cur, const uint8_t *dest)
{

    buffer_t *buf = buffer_get(127);
    if (!buf) {
        return NULL;
    }

    const uint8_t *src_address;
    uint8_t *ptr = buffer_data_pointer(buf);

    memcpy(buf->dst_sa.address, dest ? dest : ADDR_LINK_LOCAL_ALL_ROUTERS, 16);
    buf->dst_sa.addr_type = ADDR_IPV6;

    //select Address by Interface pointer and destination
    src_address = addr_select_source(cur, buf->dst_sa.address, 0);
    if (!src_address) {
        tr_debug("No source address defined");
        return buffer_free(buf);
    }

    memcpy(buf->src_sa.address, src_address, 16);
    buf->src_sa.addr_type = ADDR_IPV6;

    buf->options.type = ICMPV6_TYPE_INFO_RS;
    buf->options.code = 0;
    buf->options.hop_limit = 255;
    ptr = common_write_32_bit(0, ptr);

    /* RFC 6775 mandates SLLAO in RS */
    ptr = icmpv6_write_icmp_lla(cur, ptr, ICMPV6_OPT_SRC_LL_ADDR, true, src_address);

    buf->buf_end = ptr - buf->buf;
    buf->interface = cur;
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);

    return buf;
}

uint8_t *icmpv6_write_icmp_lla(protocol_interface_info_entry_t *cur, uint8_t *dptr, uint8_t icmp_opt, bool must, const uint8_t *ip_addr)
{
    dptr += cur->if_llao_write(cur, dptr, icmp_opt, must, ip_addr);

    return dptr;
}

/*
 * Write either an ICMPv6 Prefix Information Option for a Router Advertisement
 * (RFC4861+6275), or an RPL Prefix Information Option (RFC6550).
 * Same payload, different type/len.
 */
uint8_t *icmpv6_write_prefix_option(const prefix_list_t *prefixes,  uint8_t *dptr, uint8_t rpl_prefix, protocol_interface_info_entry_t *cur)
{
    uint8_t flags;

    ns_list_foreach(prefix_entry_t, prefix_ptr, prefixes) {
        flags = prefix_ptr->options;
        if (prefix_ptr->prefix_len == 64) {
            /* XXX this seems dubious - shouldn't get_address_with_prefix be called every time? What if the address changes or is deleted? */
            if (prefix_ptr->options & PIO_R) {
                const uint8_t *addr = addr_select_with_prefix(cur, prefix_ptr->prefix, prefix_ptr->prefix_len, 0);
                if (addr) {
                    memcpy(prefix_ptr->prefix, addr, 16);
                } else {
                    flags &= ~PIO_R;
                }
            }
        }
        if (rpl_prefix) {
            *dptr++ = RPL_PREFIX_INFO_OPTION;
            *dptr++ = 30; // Length in bytes, excluding these 2
        } else {
            *dptr++ = ICMPV6_OPT_PREFIX_INFO;
            *dptr++ = 4; // Length in 8-byte units
        }

        *dptr++ = prefix_ptr->prefix_len; //length
        *dptr++ = flags; //Flags
        dptr = common_write_32_bit(prefix_ptr->lifetime, dptr);
        dptr = common_write_32_bit(prefix_ptr->preftime, dptr);
        dptr = common_write_32_bit(0, dptr); // Reserved2
        memcpy(dptr, prefix_ptr->prefix, 16);
        dptr += 16;
    }
    return dptr;
}

/* 0                   1                   2                   3
 * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |    Length     |           Reserved            |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                              MTU                              |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
uint8_t *icmpv6_write_mtu_option(uint32_t mtu, uint8_t *dptr)
{
    *dptr++ = ICMPV6_OPT_MTU;
    *dptr++ = 1; // length
    dptr = common_write_16_bit(0, dptr);
    dptr = common_write_32_bit(mtu, dptr);
    return dptr;
}

void ack_receive_cb(struct buffer *buffer_ptr, uint8_t status)
{
    /*icmpv6_na_handler functionality based on ACK*/
    ipv6_neighbour_t *neighbour_entry;
    uint8_t ll_target[16];

    if (status != SOCKET_TX_DONE) {
        /*NS failed*/
        return;
    }

    if (buffer_ptr->dst_sa.addr_type == ADDR_IPV6) {
        /*Full IPv6 address*/
        memcpy(ll_target, buffer_ptr->dst_sa.address, 16);
    } else if (buffer_ptr->dst_sa.addr_type == ADDR_802_15_4_LONG) {
        // Build link local address from long MAC address
        memcpy(ll_target, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(ll_target + 8, &buffer_ptr->dst_sa.address[2], 8);
        ll_target[8] ^= 2;
    } else {
        tr_warn("wrong address %d %s", buffer_ptr->dst_sa.addr_type, trace_array(buffer_ptr->dst_sa.address, 16));
        return;
    }

    neighbour_entry = ipv6_neighbour_lookup(&buffer_ptr->interface->ipv6_neighbour_cache, ll_target);
    if (neighbour_entry) {
        ipv6_neighbour_update_from_na(&buffer_ptr->interface->ipv6_neighbour_cache, neighbour_entry, NA_S, buffer_ptr->dst_sa.addr_type, buffer_ptr->dst_sa.address);
    }

    if (ws_info(buffer_ptr->interface)) {
        ws_common_neighbor_update(buffer_ptr->interface, ll_target);
    }
}
void ack_remove_neighbour_cb(struct buffer *buffer_ptr, uint8_t status)
{
    /*icmpv6_na_handler functionality based on ACK*/
    uint8_t ll_target[16];
    (void)status;

    if (buffer_ptr->dst_sa.addr_type == ADDR_IPV6) {
        /*Full IPv6 address*/
        memcpy(ll_target, buffer_ptr->dst_sa.address, 16);
    } else if (buffer_ptr->dst_sa.addr_type == ADDR_802_15_4_LONG) {
        // Build link local address from long MAC address
        memcpy(ll_target, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(ll_target + 8, &buffer_ptr->dst_sa.address[2], 8);
        ll_target[8] ^= 2;
    } else {
        tr_warn("wrong address %d %s", buffer_ptr->dst_sa.addr_type, trace_array(buffer_ptr->dst_sa.address, 16));
        return;
    }
    if (ws_info(buffer_ptr->interface)) {
        ws_common_neighbor_remove(buffer_ptr->interface, ll_target);
    }

}

static void icmpv6_aro_cb(buffer_t *buf, uint8_t status)
{
    uint8_t ll_address[16];
    if (buf->dst_sa.addr_type == ADDR_IPV6) {
        /*Full IPv6 address*/
        memcpy(ll_address, buf->dst_sa.address, 16);
    } else if (buf->dst_sa.addr_type == ADDR_802_15_4_LONG) {
        // Build link local address from long MAC address
        memcpy(ll_address, ADDR_LINK_LOCAL_PREFIX, 8);
        memcpy(ll_address + 8, &buf->dst_sa.address[2], 8);
        ll_address[8] ^= 2;
    }
    rpl_control_address_register_done(buf->interface, ll_address, status);
    if (status != SOCKET_TX_DONE) {
        ws_common_aro_failure(buf->interface, ll_address);
    }
}

buffer_t *icmpv6_build_ns(protocol_interface_info_entry_t *cur, const uint8_t target_addr[16], const uint8_t *prompting_src_addr, bool unicast, bool unspecified_source, const aro_t *aro)
{
    if (!cur || addr_is_ipv6_multicast(target_addr)) {
        return NULL;
    }

    buffer_t *buf = buffer_get(127);
    if (!buf) {
        return buf;
    }

    buf->options.type = ICMPV6_TYPE_INFO_NS;
    buf->options.code = 0;
    buf->options.hop_limit = 255;

    uint8_t *ptr = buffer_data_pointer(buf);
    ptr = common_write_32_bit(0, ptr);
    memcpy(ptr, target_addr, 16);
    ptr += 16;

    if (aro) {
        *ptr++ = ICMPV6_OPT_ADDR_REGISTRATION;
        *ptr++ = 2;
        *ptr++ = aro->status; /* Should be ARO_SUCCESS in an NS */
        *ptr++ = 0;
        ptr = common_write_16_bit(0, ptr);
        ptr = common_write_16_bit(aro->lifetime, ptr);
        memcpy(ptr, aro->eui64, 8);
        ptr += 8;
    }

    if (unicast) {
        memcpy(buf->dst_sa.address, target_addr, 16);
    } else {
        memcpy(buf->dst_sa.address, ADDR_MULTICAST_SOLICITED, 13);
        memcpy(buf->dst_sa.address + 13, target_addr + 13, 3);
    }
    buf->dst_sa.addr_type = ADDR_IPV6;

    if (unspecified_source) {
        memset(buf->src_sa.address, 0, 16);
    } else {
        /* RFC 4861 7.2.2. says we should use the source of traffic prompting the NS, if possible */
        /* This is also used to specify the address for ARO messages */
        if (aro || (prompting_src_addr && addr_is_assigned_to_interface(cur, prompting_src_addr))) {
            memcpy(buf->src_sa.address, prompting_src_addr, 16);
        } else {
            /* Otherwise, according to RFC 4861, we could use any address.
             * But there is a 6LoWPAN/RPL hiccup - a node may have registered
             * to us with an ARO, and we might send it's global address a NUD
             * probe. But it doesn't know _our_ global address, which default
             * address selection would favour.
             * If it was still a host, we'd get away with using our global
             * address, as we'd be its default route, so its reply comes to us.
             * But if it's switched to being a RPL router, it would send its
             * globally-addressed reply packet up the RPL DODAG.
             * Avoid the problem by using link-local source.
             * This will still leave us with an asymmetrical connection - its
             * global address on-link for us, and we send to it directly (and
             * can NUD probe it), whereas it regards us as off-link and will
             * go via RPL (and won't probe us). But it will work fine.
             */
            if (addr_interface_get_ll_address(cur, buf->src_sa.address, 0) < 0) {
                tr_debug("No address for NS");
                return buffer_free(buf);
            }
        }
        /* SLLAO is required if we're sending an ARO */
        /* This rule can be bypassed with flag use_eui64_as_slla_in_aro */
        if (!cur->ipv6_neighbour_cache.use_eui64_as_slla_in_aro) {
            ptr = icmpv6_write_icmp_lla(cur, ptr, ICMPV6_OPT_SRC_LL_ADDR, aro, buf->src_sa.address);
        }
        /* If ARO Success sending is omitted, MAC ACK is used instead */
        /* Setting callback for receiving ACK from adaptation layer */
        if (aro && cur->ipv6_neighbour_cache.omit_na_aro_success) {
            buf->ack_receive_cb = icmpv6_aro_cb;
        }
    }
    if (unicast && (!aro && cur->ipv6_neighbour_cache.omit_na)) {
        /*MAC ACK is processed as success response*/
        buf->ack_receive_cb = ack_receive_cb;
    }

    buf->src_sa.addr_type = ADDR_IPV6;

    /* NS packets are implicitly on-link. If we ever find ourselves sending an
     * NS to a global address, it's because we are in some way regarding
     * it as on-link. (eg, redirect, RPL source routing header). We force
     * transmission as on-link here, regardless of routing table, to avoid any
     * potential oddities.
     */
    ipv6_buffer_route_to(buf, buf->dst_sa.address, cur);

    buffer_data_end_set(buf, ptr);
    buf->interface = cur;
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);

    return buf;
}
#endif // HAVE_IPV6_ND

void icmpv6_build_echo_req(protocol_interface_info_entry_t *cur, const uint8_t target_addr[16])
{
    const uint8_t *src;
    buffer_t *buf = buffer_get(127);
    if (!buf) {
        return;
    }

    buf->options.type = ICMPV6_TYPE_INFO_ECHO_REQUEST;
    buf->options.code = 0;
    buf->options.hop_limit = 255;

    uint8_t *ptr = buffer_data_pointer(buf);
    memcpy(ptr, target_addr, 16);
    ptr += 16;

    memcpy(buf->dst_sa.address, target_addr, 16);
    buf->dst_sa.addr_type = ADDR_IPV6;
    //Select Address By Destination
    src = addr_select_source(cur, buf->dst_sa.address, 0);
    if (src) {
        memcpy(buf->src_sa.address, src, 16);
    } else {
        tr_debug("No address for NS");
        buffer_free(buf);
        return;
    }
    buf->src_sa.addr_type = ADDR_IPV6;
    buffer_data_end_set(buf, ptr);
    buf->interface = cur;
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
    protocol_push(buf);
}

#ifdef HAVE_6LOWPAN_ROUTER
buffer_t *icmpv6_build_dad(protocol_interface_info_entry_t *cur, buffer_t *buf, uint8_t type, const uint8_t dest_addr[16], const uint8_t eui64[8], const uint8_t reg_addr[16], uint8_t status, uint16_t lifetime)
{
    if (!cur) {
        return NULL;
    }

    if (!buf) {
        buf = buffer_get(4 + 8 + 16);
        if (!buf) {
            return buf;
        }
    }

    uint8_t *ptr = buffer_data_pointer(buf);
    buf->options.type = type;
    buf->options.code = 0;
    buf->options.hop_limit = 64; /* RFC 6775 MULTIHOP_HOPLIMIT */

    *ptr++ = status;
    *ptr++ = 0;
    ptr = common_write_16_bit(lifetime, ptr);
    memcpy(ptr, eui64, 8);
    ptr += 8;
    memcpy(ptr, reg_addr, 16);
    ptr += 16;
    buffer_data_end_set(buf, ptr);

    memcpy(buf->dst_sa.address, dest_addr, 16);
    buf->dst_sa.addr_type = ADDR_IPV6;

    const uint8_t *src = addr_select_source(cur, buf->dst_sa.address, 0);
    if (src && !addr_is_ipv6_link_local(src)) {
        memcpy(buf->src_sa.address, src, 16);
    } else {
        tr_debug("No address for DAD");
        return buffer_free(buf);
    }
    buf->src_sa.addr_type = ADDR_IPV6;
    buf->interface = cur;
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);

    return buf;
}
#endif // HAVE_6LOWPAN_ROUTER

#ifdef HAVE_IPV6_ND
/*
 * Neighbor Advertisement Message Format
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Type      |     Code      |          Checksum             |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |R|S|O|                     Reserved                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                       Target Address                          +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Options ...
 *  +-+-+-+-+-+-+-+-+-+-+-+-
 *
 *    R              Router flag.
 *    S              Solicited flag.
 *    O              Override flag.
 */

buffer_t *icmpv6_build_na(protocol_interface_info_entry_t *cur, bool solicited, bool override, bool tllao_required, const uint8_t target[static 16], const aro_t *aro, const uint8_t src_addr[static 16])
{
    uint8_t *ptr;
    uint8_t flags;

    /* Check if ARO response and status == success, then sending can be omitted with flag */
    if (aro && cur->ipv6_neighbour_cache.omit_na_aro_success && aro->status == ARO_SUCCESS) {
        tr_debug("Omit NA ARO success");
        return NULL;
    }
    /* All other than ARO NA messages are omitted and MAC ACK is considered as success */
    if (!tllao_required && (!aro && cur->ipv6_neighbour_cache.omit_na)) {
        return NULL;
    }


    buffer_t *buf = buffer_get(8 + 16 + 16 + 16); /* fixed, target addr, target ll addr, aro */
    if (!buf) {
        return NULL;
    }

    ptr = buffer_data_pointer(buf);
    buf->options.hop_limit = 255;

    // Set the ICMPv6 NA type and code fields as per RFC4861
    buf->options.type = ICMPV6_TYPE_INFO_NA;
    buf->options.code = 0x00;

    // If we're sending RAs, then we have to set the Router bit here
    // (RFC 4861 makes host trigger action when they see the IsRouter flag
    // go from true to false - RAs from us imply "IsRouter", apparently even if
    // Router Lifetime is 0. So keep R set as long as we're sending RAs)
    flags = icmpv6_radv_is_enabled(cur) ? NA_R : 0;

    if (override) {
        flags |= NA_O;
    }

    if (addr_is_ipv6_unspecified(src_addr)) {
        // Solicited flag must be 0 if responding to DAD
        memcpy(buf->dst_sa.address, ADDR_LINK_LOCAL_ALL_NODES, 16);
    } else {
        if (solicited) {
            flags |= NA_S;
        }

        /* See RFC 6775 6.5.2 - errors are sent to LL64 address
         * derived from EUI-64, success to IP source address */
        if (aro && aro->status != ARO_SUCCESS) {
            memcpy(buf->dst_sa.address, ADDR_LINK_LOCAL_PREFIX, 8);
            memcpy(buf->dst_sa.address + 8, aro->eui64, 8);
            buf->dst_sa.address[8] ^= 2;
        } else {
            memcpy(buf->dst_sa.address, src_addr, 16);
        }
    }
    buf->dst_sa.addr_type = ADDR_IPV6;

    /* In theory we could just use addr_select_source(), as RFC 4861 allows
     * any address assigned to the interface as source. But RFC 6775 shows LL64
     * as the source in its appendix, sending NA to a global address, and our
     * lower layers go a bit funny with RPL during bootstrap if we send from a
     * global address to a global address. By favouring the target address as
     * source, we catch that 6LoWPAN case (the target is LL), as well as making
     * it look neater anyway.
     */
    if (addr_is_assigned_to_interface(cur, target)) {
        memcpy(buf->src_sa.address, target, 16);
    } else {
        const uint8_t *src = addr_select_source(cur, buf->dst_sa.address, 0);
        if (!src) {
            tr_debug("No address");
            return buffer_free(buf);
        }
        memcpy(buf->src_sa.address, src, 16);
    }
    buf->src_sa.addr_type = ADDR_IPV6;

    ptr = common_write_32_bit((uint32_t) flags << 24, ptr);
    // Set the target IPv6 address
    memcpy(ptr, target, 16);
    ptr += 16;

    // Set the target Link-Layer address
    ptr = icmpv6_write_icmp_lla(cur, ptr, ICMPV6_OPT_TGT_LL_ADDR, tllao_required, target);

    if (aro) {
        *ptr++ = ICMPV6_OPT_ADDR_REGISTRATION;
        *ptr++ = 2;
        *ptr++ = aro->status;
        *ptr++ = 0;
        ptr = common_write_16_bit(0, ptr);
        ptr = common_write_16_bit(aro->lifetime, ptr);
        memcpy(ptr, aro->eui64, 8);
        ptr += 8;
    }
    if (ws_info(cur) && aro && aro->status != ARO_SUCCESS) {
        /*If Aro failed we will kill the neigbour after we have succeeded in sending message*/
        if (!ws_common_negative_aro_mark(cur, aro->eui64)) {
            tr_debug("Neighbour removed for negative response send");
            return buffer_free(buf);
        }
        buf->ack_receive_cb = ack_remove_neighbour_cb;
    }

    //Force Next Hop is destination
    ipv6_buffer_route_to(buf, buf->dst_sa.address, cur);

    buffer_data_end_set(buf, ptr);
    buf->info = (buffer_info_t)(B_DIR_DOWN | B_FROM_ICMP | B_TO_ICMP);
    buf->interface = cur;

    tr_debug("Build NA");

    return (buf);
}

#endif // HAVE_IPV6_ND

#ifdef HAVE_IPV6_ND
/* Check whether the options section of an ICMPv6 message is well-formed */
bool icmpv6_options_well_formed(const uint8_t *dptr, uint_fast16_t dlen)
{
    if (dlen % 8) {
        return false;
    }

    while (dlen) {
        uint_fast16_t opt_len = dptr[1] * 8;
        if (opt_len == 0 || opt_len > dlen) {
            return false;
        }
        dptr += opt_len;
        dlen -= opt_len;
    }

    return true;
}

bool icmpv6_options_well_formed_in_buffer(const buffer_t *buf, uint16_t offset)
{
    if (buffer_data_length(buf) < offset) {
        return false;
    }

    return icmpv6_options_well_formed(buffer_data_pointer(buf) + offset,
                                      buffer_data_length(buf) - offset);
}

/*
 * Search for the first option of the specified type (and optionally length).
 * Caller must have already checked the options are well-formed.
 * If optlen is non-zero, then options with different lengths are ignored.
 * Note that optlen is in 8-octet units.
 */
const uint8_t *icmpv6_find_option(const uint8_t *dptr, uint_fast16_t dlen, uint8_t option, uint8_t optlen)
{
    while (dlen) {
        uint8_t type = dptr[0];
        uint8_t len  = dptr[1];

        if (type == option && (optlen == 0 || optlen == len)) {
            return dptr;
        } else {
            dptr += len * 8;
            dlen -= len * 8;
        }
    }
    return NULL;

}

const uint8_t *icmpv6_find_option_in_buffer(const buffer_t *buf, uint_fast16_t offset, uint8_t option, uint8_t optlen)
{
    return icmpv6_find_option(buffer_data_pointer(buf) + offset,
                              buffer_data_length(buf) - offset, option, optlen);
}
#endif // HAVE_IPV6_ND
