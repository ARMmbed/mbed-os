/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/* rpl_data.c deals with handling of the packet data (hop-by-hop header)
 * and Source Routing headers in RPL-routed packets.
 *
 * rpl_domain_t is accessible, but not normally manipulated - all routines in
 * this file works on a specific instance.
 *
 * rpl_instance_t, rpl_dodag_t, rpl_dodag_version_t, rpl_neighbour_t are all accessible.
 */

#include "nsconfig.h"

#ifdef HAVE_RPL

#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include <string.h>

#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Common_Protocols/icmpv6.h"

#include "RPL/rpl_protocol.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_structures.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_data.h"

#define TRACE_GROUP "RPLa"

#define RPL_DATA_SR_INIT_SIZE (16*4)

#ifdef HAVE_RPL_ROOT
typedef struct rpl_data_sr {
    rpl_dao_target_t *target;   /* Target - note may be a prefix */
    uint16_t iaddr_size;
    uint8_t ihops;          /* Number of intermediate hops (= addresses in SRH) */
    uint8_t final_dest[16]; /* Final destination (used only temporarily during header construction) */
    uint8_t iaddr[];        /* Intermediate address list is built backwards, contiguous with final_dest */
} rpl_data_sr_t;

static rpl_data_sr_t *rpl_data_sr;
#endif

static const uint8_t *rpl_data_get_dodagid(const buffer_t *buf);

bool rpl_data_is_rpl_route(ipv6_route_src_t source) {
    switch (source) {
        case ROUTE_RPL_DAO:
        case ROUTE_RPL_DAO_SR:
        case ROUTE_RPL_SRH:
        case ROUTE_RPL_DIO:
        case ROUTE_RPL_INSTANCE:
        case ROUTE_RPL_ROOT:
        case ROUTE_RPL_FWD_ERROR:
            return true;
        default:
            return false;
    }
}

bool rpl_data_is_rpl_parent_route(ipv6_route_src_t source) {
    switch (source) {
        case ROUTE_RPL_DIO:
        case ROUTE_RPL_INSTANCE:
        case ROUTE_RPL_ROOT:
            return true;
        default:
            return false;
    }
}

static bool rpl_data_is_rpl_downward_route(ipv6_route_src_t source) {
    switch (source) {
        case ROUTE_RPL_DAO:
        case ROUTE_RPL_DAO_SR:
        case ROUTE_RPL_SRH:
            return true;
        default:
            return false;
    }
}

static bool rpl_data_handle_fwd_error(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *opt, const sockaddr_t *ll_src)
{
    if (!ll_src) {
        tr_warn("Forwarding-Error - dst=%s, neighbour unknown", trace_ipv6(buf->dst_sa.address));
        return false;
    } else {
        tr_warn("Forwarding-Error - dst=%s, neighbour=%s", trace_ipv6(buf->dst_sa.address), trace_sockaddr(ll_src, true));
    }

    rpl_instance_t *instance = rpl_lookup_instance(cur->rpl_domain, opt[1], rpl_data_get_dodagid(buf));
    if (!instance) {
        return false;
    }

#if 1
    return false;
#else
    /* Work needed */
    if (rpl_instance_am_root(instance)) {
        /* We are looking for a target that has us as its transit */
    }
    bool deleted = ipv6_route_delete_by_info_and_ll(buf->dst_sa.address, ROUTE_RPL_DAO, ll_src);
    deleted |= ipv6_route_delete_by_info_and_ll(buf->dst_sa.address, ROUTE_RPL_DAO_SR, ll_src);
    opt[0] &=~ RPL_OPT_FWD_ERROR;

    return true;
#endif
}

bool rpl_data_process_hbh(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *opt, const sockaddr_t *ll_src)
{
    buf->rpl_instance = opt[1];
    buf->rpl_instance_known = true;

    /* Act on the forwarding error */
    if (opt[0] & RPL_OPT_FWD_ERROR) {
        if (!rpl_data_handle_fwd_error(buf, cur, opt, ll_src)) {
            return false;
        }
     }

    /* We don't actually do much now. If the packet is addressed
     * to us, we don't need really need much (or any) info.
     *
     * If we are going to forward forwarding_down will find the option
     * manually. If we come to unwrap the tunnel, then we need to take a
     * copy before the outer IP header is stripped.
     */
    buf->options.ip_extflags |= IPEXT_HBH_RPL;
    buf->options.need_predecessor = true;
    buf->rpl_flag_error = opt[0];

    return true;
}

/* We assume the packet is basically well-formed, as it will have either
 * cleared initial input parsing, or we formed it ourselves. hbh and srh
 * are set to point to the RPL Hop-by-Hop option and/or RPL Source Routing
 * Header, if present.
 */
static void rpl_data_locate_info(buffer_t *buf, uint8_t **hbh, uint8_t **srh)
{
    uint8_t *ptr = buffer_data_pointer(buf);
    uint16_t len = buffer_data_length(buf);

    if (hbh) {
        *hbh = NULL;
    }
    if (srh) {
        *srh = NULL;
    }

    if (len < IPV6_HDRLEN) {
        return;
    }
    uint16_t ip_len = common_read_16_bit(ptr + IPV6_HDROFF_PAYLOAD_LENGTH);
    uint8_t nh = ptr[6];
    ptr += IPV6_HDRLEN;
    len -= IPV6_HDRLEN;
    if (ip_len > len) {
        return;
    }
    len = ip_len;
    while (len) {
        uint16_t hdrlen;
        switch (nh) {
            case IPV6_NH_HOP_BY_HOP:
            {
                if (len < 8) {
                    return;
                }
                nh = ptr[0];
                hdrlen = (ptr[1] + 1) * 8;
                /* Move on if they're not interested in HbH (looking for SRH) */
                if (!hbh) {
                    break;
                }
                if (hdrlen > len) {
                    return;
                }
                uint8_t *opt_ptr = ptr + 2;
                uint8_t *opt_end = ptr + hdrlen;
                while (opt_ptr < opt_end) {
                    switch (opt_ptr[0]) {
                        case IPV6_OPTION_PAD1:
                            opt_ptr++;
                            break;
                        case IPV6_OPTION_RPL:
                            *hbh = opt_ptr;
                            goto found_option;
                        default:
                            opt_ptr += 2 + opt_ptr[1];
                            break;
                    }
                }
            found_option:
                /* If they're not looking for SRH, finish now */
                if (!srh) {
                    return;
                }
                break;
            }
            case IPV6_NH_DEST_OPT:
                // Destination option permitted to appear before routing
                if (len < 8) {
                    return;
                }
                nh = ptr[0];
                hdrlen = (ptr[1] + 1) * 8;
                /* If they're not looking for SRH, finish now - past HbH */
                if (!srh) {
                    return;
                }
                break;
            case IPV6_NH_ROUTING:
                if (!srh) {
                    return;
                }
                if (ptr[2] == IPV6_ROUTING_TYPE_RPL) {
                    *srh = ptr;
                }
                // No need to examine past routing header
                return;
            default:
                // No other headers can appear before routing - last we care about
                return;
        }
        if (hdrlen > len) {
            return;
        }
        ptr += hdrlen;
        len -= hdrlen;
    }
    return;
}

bool rpl_data_remember_outer(buffer_t *buf)
{
    /* We're stripping the IP header - need the HBH header for future reference */
     uint8_t *hbh;
     rpl_data_locate_info(buf, &hbh, NULL);
     if (hbh) {
         uint8_t instance_id = hbh[3];
         /* For local instances, also need to extract the DODAG ID from src/dst */
         bool local = rpl_instance_id_is_local(instance_id);
         /* Copy the length byte and the option data (and optionally DODAG ID) */
         buf->rpl_option = ns_dyn_mem_temporary_alloc(hbh[1] + 1 + (local ? 16 : 0));
         if (buf->rpl_option) {
             memcpy(buf->rpl_option, hbh + 1, hbh[1] + 1);
             if (local) {
                 uint8_t *dodagid = instance_id & RPL_INSTANCE_DEST ? buf->dst_sa.address : buf->src_sa.address;
                 memcpy(buf->rpl_option + hbh[1] + 1, dodagid, 16);
             }
         }
     }

     if ((buf->options.ip_extflags & IPEXT_HBH_RPL) && !buf->rpl_option) {
         tr_warn("RPL tunnel exit HbH fail");
         return false;
     }

     return true;
}

/* Get the DODAG ID if it's a local DODAG packet */
static const uint8_t *rpl_data_get_dodagid(const buffer_t *buf)
{
    if (!buf->rpl_instance_known || rpl_instance_id_is_global(buf->rpl_instance)) {
        return NULL;
    }
    /* rpl_data_remember_outer() stores it in the rpl_option metatdata */
    if (buf->rpl_option) {
        return buf->rpl_option + 1 + buf->rpl_option[1];
    }
    else {
        return buf->rpl_instance & RPL_INSTANCE_DEST ? buf->dst_sa.address
                                                     : buf->src_sa.address;
    }
}

/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *                                  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                  |  Option Type  |  Opt Data Len |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |O|R|F|0|0|0|0|0| RPLInstanceID |          SenderRank           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         (sub-TLVs)                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                     Figure 1: RPL Option
 */
static buffer_t *rpl_data_exthdr_provider_hbh_2(buffer_t *buf, rpl_instance_t *instance, rpl_neighbour_t *neighbour, ipv6_exthdr_stage_t stage, int16_t *result)
{
    ipv6_route_info_t *route_info = &buf->route->route_info;

    /* This can be called both for routes which only use HbH headers (eg DIO)
     * as well as one-hop DAO_SR routes which would normally use source routing
     * headers, if there was more than one hop. For DAO_SR, neighbour will be
     * NULL.
     */

    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    if (!dodag) {
        *result = -1;
        return buf;
    }

    bool destination_in_instance = false;
    uint16_t ext_size = 0;
    if (addr_ipv6_equal(route_info->next_hop_addr, buf->dst_sa.address) ||
        addr_ipv6_equal(buf->dst_sa.address, dodag->id)) {
        destination_in_instance = true;

        if (buf->rpl_option) {
            /* Forwarding an existing option - preserve it */
            uint8_t opt_size = buf->rpl_option[0];
            ext_size = 2 + opt_size;
            ext_size = (ext_size + 7) &~ 7;
        } else {
            /* Generating our own option - fixed size, no TLVs */
            ext_size = 8;
        }
    }

    switch (stage) {
        case IPV6_EXTHDR_SIZE:
            *result = ext_size;
            return buf;

        case IPV6_EXTHDR_INSERT: {
            if (!destination_in_instance) {
                /* We don't add a header - we'll do it on the tunnel */
                *result = 0;
                return buf;
            }
            buf = buffer_headroom(buf, ext_size);
            if (!buf) {
                return NULL;
            }
            uint8_t *ext = buffer_data_reserve_header(buf, ext_size);
            ext[0] = buf->options.type;
            buf->options.type = IPV6_NH_HOP_BY_HOP;
            ext[1] = ext_size / 8 - 1;
            uint8_t *opt = ext + 2;
            opt[0] = IPV6_OPTION_RPL;
            if (buf->rpl_option) {
                /* Get back the RPL option we stripped off an outer IP header */
                memcpy(opt + 1, buf->rpl_option, 1 + buf->rpl_option[0]);
                ns_dyn_mem_free(buf->rpl_option);
                buf->rpl_option = NULL;
            } else {
                opt[1] = 4; // option length
                opt[2] = 0; // placeholder
                opt[3] = instance->id;
                /* For upwards routes we can deduce that DODAGID must be
                 * the destination, so set the D flag.
                 */
                if (rpl_instance_id_is_local(instance->id) && !rpl_data_is_rpl_downward_route(route_info->source)) {
                    opt[3] |= RPL_INSTANCE_DEST;
                }
                common_write_16_bit(RPL_RANK_INFINITE, opt + 4); // SenderRank (placeholder)
            }
            /* Pad HbH header if necessary. */
            uint8_t pad_len = ext + ext_size - (opt + 2 + opt[1]);
            if (pad_len == 1) {
                opt[0] = IPV6_OPTION_PAD1;
            } else if (pad_len > 1) {
                opt[0] = IPV6_OPTION_PADN;
                opt[1] = pad_len - 2;
                memset(opt + 2, 0, pad_len - 2);
            }
            // don't forget to set the "RPL option present" marker
            buf->options.ip_extflags |= IPEXT_HBH_RPL;
            *result = 0;
            return buf;
        }

        case IPV6_EXTHDR_MODIFY: {
            uint8_t *opt;
            uint16_t sender_rank;

            rpl_data_locate_info(buf, &opt, NULL);
            if (!opt) {
                *result = IPV6_EXTHDR_MODIFY_TUNNEL;
                // Tunnel to next hop in general case, but if going to DODAGID,
                // it can tunnel all the way (and it HAS to if it is a local
                // DODAG).
                if (!addr_ipv6_equal(buf->dst_sa.address, dodag->id)) {
                    memcpy(buf->dst_sa.address, route_info->next_hop_addr, 16);
                }
                buf->src_sa.addr_type = ADDR_NONE; // force auto-selection
                return buf;
            }

            if (buf->ip_routed_up) {
                /* Check for rank errors - RFC 6550 11.2.2.2. */
                /* Note that RPL spec does not say that packets from nodes of
                 * equal rank are errors, but we treat them as such to get
                 * reliable sibling loop detection - we require sender rank to be
                 * strictly less for Down packets and strictly greater for Up.
                 */
                sender_rank = common_read_16_bit(opt + 4);
                rpl_cmp_t cmp = rpl_rank_compare_dagrank_rank(dodag, sender_rank, instance->current_rank);
                rpl_cmp_t expected_cmp = (opt[2] & RPL_OPT_DOWN) ? RPL_CMP_LESS : RPL_CMP_GREATER;
                if (cmp != expected_cmp) {
                    /* Set the Rank-Error bit; if already set, drop */
                    if (opt[2] & RPL_OPT_RANK_ERROR) {
                        protocol_stats_update(STATS_RPL_ROUTELOOP, 1);
                        tr_info("Forwarding inconsistency R");
                        rpl_instance_inconsistency(instance);
                        *result = -1;
                        return buf;
                    } else {
                        opt[2] |= RPL_OPT_RANK_ERROR;
                    }
                }
            }

            if (buf->rpl_flag_error & RPL_OPT_FWD_ERROR) {
                opt[2] |= RPL_OPT_FWD_ERROR;
            } else if (rpl_data_is_rpl_downward_route(route_info->source)) {
                opt[2] |= RPL_OPT_DOWN;
            } else {
                opt[2] &= ~RPL_OPT_DOWN;
            }

            /* Set the D flag for local instances */
            if (rpl_instance_id_is_local(instance->id)) {
                if (addr_ipv6_equal(dodag->id, buf->dst_sa.address)) {
                    opt[3] |= RPL_INSTANCE_DEST;
                } else if (addr_ipv6_equal(dodag->id, buf->src_sa.address)) {
                    opt[3] &=~ RPL_INSTANCE_DEST;
                } else {
                    tr_error("Local instance invalid %s[%d]: %s -> %s", trace_ipv6(dodag->id), instance->id, trace_ipv6(buf->src_sa.address), trace_ipv6(buf->dst_sa.address));
                    *result = -1;
                    return buf;
                }
            }

            /* RPL 11.2.2.2. says we set SenderRank to infinite when forwarding
             * across a version discontinuity. (Must be up - we don't know versions
             * of downward routes).
             */
            if ((buf->rpl_flag_error & RPL_OPT_FWD_ERROR) || rpl_data_is_rpl_downward_route(route_info->source) || !neighbour || neighbour->dodag_version == instance->current_dodag_version) {
                sender_rank = nrpl_dag_rank(dodag, instance->current_rank);
            } else {
                sender_rank = RPL_RANK_INFINITE;
            }
            common_write_16_bit(sender_rank, opt + 4);
            *result = 0;
            return buf;
        }
        default:
            return buffer_free(buf);
    }
}

static buffer_t *rpl_data_exthdr_provider_hbh(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result)
{
    ipv6_route_info_t *route_info = &buf->route->route_info;
    rpl_neighbour_t *neighbour = route_info->info;

    rpl_instance_t *instance = rpl_neighbour_instance(neighbour);
    if (!instance) {
        *result = -1;
        return buf;
    }

    return rpl_data_exthdr_provider_hbh_2(buf, instance, neighbour, stage, result);
}

static buffer_t *rpl_data_exthdr_provider_fwd_error_hbh(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result)
{
    ipv6_route_info_t *route_info = &buf->route->route_info;
    rpl_instance_t *instance =route_info->info;

    return rpl_data_exthdr_provider_hbh_2(buf, instance, NULL, stage, result);

}

/* This could live in address.c or ipv6.c */
/* Can get false negatives if we don't already have a neighbour cache entry, but in practice
 * will be solid as we are basically matching link-local 6LoWPAN addresses, which can be mapped
 * to link-layer without an existing entry. Could conceivably get a false positive if we have
 * a stale entry and MAC addresses have been reassigned, but very unlikely.
 */
static bool rpl_downward_ip_addr_matches_ll_addr(protocol_interface_info_entry_t *cur, const uint8_t ip_addr_a[static 16], const sockaddr_t *ll_addr_b)
{
    if (!ll_addr_b) {
        return false;
    }
    addrtype_t ll_type_a;
    const uint8_t *ll_addr_a;
    if (!ipv6_map_ip_to_ll(cur, NULL, ip_addr_a, &ll_type_a, &ll_addr_a)) {
        return false;
    }

    return ll_type_a == ll_addr_b->addr_type &&
           memcmp(ll_addr_a, ll_addr_b->address, addr_len_from_type(ll_type_a)) == 0;
}

/* ROUTE_RPL_INSTANCE routes are the default for the instance - not valid, unless
 * instance is already known.
 */
static bool rpl_data_route_predicate_instance_default(const ipv6_route_info_t *route, bool valid)
{
    (void)route;
    (void)valid;

    return false;
}

static rpl_instance_t *predicate_instance;
static bool predicate_down;
static const sockaddr_t *predicate_predecessor;

/* Override predicate for choosing routes given a specific instance (ie when
 * forwarding a packet that had a specified instance ID). That instance is
 * loaded into the static predicate_instance above.
 */
static bool rpl_data_route_predicate_specific_instance(const ipv6_route_info_t *route, bool valid)
{
    /* We will permit forwarding out of RPL into a non-RPL interface (eg border routers) */
    /* XXX - what if we're the boundary between two RPL domains? */
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(route->interface_id);
    if (!cur || !cur->rpl_domain) {
        return valid;
    }

    /* If forwarding onto the same interface, think more */
    switch (route->source) {
        /* These are the type of routes we will forward onto out of RPL - they
         * must include all types of non-owned routes that could be advertised
         * through a DAO.
         */
        case ROUTE_ARO:
            return valid;

        /* Upward routes */
        case ROUTE_RPL_DIO:
        case ROUTE_RPL_INSTANCE:
        case ROUTE_RPL_ROOT: {
            /* Packets going down can't go back up */
            if (predicate_down) {
                return false;
            }
            /* Never route to predecessor */
            if (rpl_downward_ip_addr_matches_ll_addr(cur, route->next_hop_addr, predicate_predecessor)) {
                return false;
            }
            /* Info for these upward routes is a back pointer to the neighbour */
            rpl_neighbour_t *neighbour = route->info;
            /* From there, we can get the instance info */
            rpl_instance_t *instance = rpl_neighbour_instance(neighbour);
            /* Note that this overrides the default "false" for INSTANCE routes */
            return instance == predicate_instance;
        }

        /* Downward routes */
        case ROUTE_RPL_DAO:
        case ROUTE_RPL_DAO_SR: {
            /* Info for these downward routes is a back pointer to the target */
            rpl_dao_target_t *target = route->info;
            /* Going to predecessor is fine if it was going up - we are reversing to down. Otherwise block */
            if (predicate_down) {
                if (rpl_downward_ip_addr_matches_ll_addr(cur, route->next_hop_addr, predicate_predecessor)) {
                    return false;
                }
            }
            return target->instance == predicate_instance;
        }
        /* Unknown */
        default:
            return false;
    }
}

ipv6_route_predicate_fn_t *rpl_data_get_route_predicate(rpl_domain_t *domain, const buffer_t *buf)
{
    const uint8_t *dodagid = rpl_data_get_dodagid(buf);

    predicate_instance = rpl_lookup_instance(domain, buf->rpl_instance, dodagid);
    predicate_down = buf->rpl_flag_error & RPL_OPT_DOWN;
    predicate_predecessor = buf->predecessor;

    return rpl_data_route_predicate_specific_instance;
}

/* Returns true if we are going to pass this back with the 'F' bit set */
/* If we return false, a "no route" ICMP error will occur as normal */
/* For the ICMP error case, we may treat as a DIO Trickle inconsistency, as
 * suggested by RFC 6550 11.1(8)  */
bool rpl_data_forwarding_error(buffer_t *buf)
{
    /* This is called when we have no route to send a packet - first check
     * if we were forwarding it for RPL - indicated by rpl_instance_known.
     */
    if (!buf->rpl_instance_known) {
        return false;
    }

    protocol_interface_info_entry_t *cur = buf->interface;
    if (!cur) {
        return false;
    }

    rpl_instance_t *instance = rpl_lookup_instance(cur->rpl_domain, buf->rpl_instance, rpl_data_get_dodagid(buf));
    if (!instance) {
        tr_err("rpl_data_forwarding_error: unknown instance");
        return false;
    }

    /* To use Forwarding-Error, packet must be Down, and must know predecessor */
    if (!((buf->rpl_flag_error & RPL_OPT_DOWN) && buf->predecessor)) {
        goto not_forwarding_error;
    }

    /* Must then be able to map predecessor to IP address */
    uint8_t predecessor_ip[16];
    if (!ipv6_map_ll_to_ip_link_local(cur, buf->predecessor->addr_type, buf->predecessor->address, predecessor_ip)) {
        return false;
    }

    buf->rpl_flag_error |= RPL_OPT_FWD_ERROR;
    buffer_free_route(buf);
    if (!ipv6_buffer_route_to(buf, predecessor_ip, buf->interface)) {
        return false;
    }

    buf->route->route_info.info = instance;
    buf->route->route_info.source = ROUTE_RPL_FWD_ERROR;

    return true;

not_forwarding_error:
    /* If we're not signalling a Forwarding-Error, we will be sending
     * an ICMP Destination Unreachable to the source as normal. But we
     * may still want to attempt some sort of RPL repair. If it was coming
     * upwards, and we're _not_ the root of the instance, a failure to
     * pass it on can only mean some sort of RPL routing problem (eg refusing
     * to pass to the predecessor), as we should normally be able to send
     * towards the DODAG root. We attempt to aid repair by triggering
     * a DIO inconsistency.
     *
     * If we are the root, then it will just be a perfectly normal
     * "destination unreachable" - it doesn't suggest a RPL repair is needed.
     */
    if (!(buf->rpl_flag_error & RPL_OPT_DOWN) && !rpl_instance_am_root(instance)) {
        protocol_stats_update(STATS_RPL_ROUTELOOP, 1);
        tr_info("Forwarding inconsistency 2");
        rpl_instance_inconsistency(instance);
    }
    return false;
}

#ifdef HAVE_RPL_ROOT
/* TODO - every target involved here should be non-External. Add checks */
static bool rpl_data_compute_source_route(const uint8_t *final_dest, rpl_dao_target_t * const target)
{
    if (!rpl_data_sr) {
        rpl_data_sr = rpl_alloc(sizeof(rpl_data_sr_t) + RPL_DATA_SR_INIT_SIZE);
        if (!rpl_data_sr) {
            return false;
        }
        rpl_data_sr->iaddr_size = RPL_DATA_SR_INIT_SIZE;
        rpl_data_sr->target = NULL;
    } else if (rpl_data_sr->target == target && addr_ipv6_equal(rpl_data_sr->final_dest, final_dest)) {
        return true;
    }

    /* This does all the heavy lifting - after running, the optimum path from
     * every target node is at the front of the transit list, and the connected
     * flag is set if we've any prospect.
     */
    rpl_downward_compute_paths(target->instance);
    if (!target->connected) {
        return false;
    }

    /* Wipe the "data valid" marker */
    rpl_data_sr->target = NULL;
    rpl_data_sr->ihops = 0;

    /* Final destination written explicitly (last target could be a prefix) */
    memcpy(rpl_data_sr->final_dest, final_dest, 16);

    /* We just work backwards from the target, following the first transit
     * each time, which is the shortest path after the compute_paths call.
     */
    rpl_dao_target_t *t = target;
    for (;;) {
        /* First transit is best path, thanks to root computation above */
        rpl_dao_root_transit_t *transit = ns_list_get_first(&t->info.root.transits);
        rpl_dao_target_t *parent = transit->parent;
        /* Finished if we hit NULL - ourselves */
        if (parent == NULL) {
            /* Mark "valid" */
            rpl_data_sr->target = target;
            return true;
        }
        if (!parent->connected) {
            tr_err("Parent %s disconnected", trace_ipv6_prefix(parent->prefix, parent->prefix_len));
            return false;
        }
        /* Check transit address isn't already in table. Should not be possible */
        for (int i = 16 * rpl_data_sr->ihops; i >= 0; i -= 16){
            if (addr_ipv6_equal(rpl_data_sr->final_dest + i, transit->transit)) {
                protocol_stats_update(STATS_RPL_ROUTELOOP, 1);
                tr_err("SR loop %s->%s", trace_ipv6_prefix(t->prefix, t->prefix_len), trace_ipv6(transit->transit));
                return false;
            }
        }
        /* Increase size of table if necessary */
        if (16 * (rpl_data_sr->ihops + 1) > rpl_data_sr->iaddr_size) {
            rpl_data_sr = rpl_realloc(rpl_data_sr, sizeof(rpl_data_sr_t) + rpl_data_sr->iaddr_size, sizeof(rpl_data_sr_t) + 2 * rpl_data_sr->iaddr_size);
            if (!rpl_data_sr) {
                return false;
            }
            rpl_data_sr->iaddr_size *= 2;
        }
        memcpy(rpl_data_sr->iaddr + 16 * rpl_data_sr->ihops, transit->transit, 16);
        rpl_data_sr->ihops += 1;

        t = parent;
    }
}

/* Return the next hop, if there is an intermediate. If it's direct, NULL
 * is returned. This call must follow a successful call to
 * rpl_data_compute_source_route().
 */
const uint8_t *rpl_data_sr_next_hop(void)
{
    if (rpl_data_sr->ihops == 0) {
        return NULL;
    }
    return rpl_data_sr->iaddr + 16 * (rpl_data_sr->ihops - 1);
}

static bool rpl_data_route_next_hop(const uint8_t *dest, ipv6_route_info_t *route)
{
    rpl_dao_target_t *target = route->info;

    if (!rpl_data_compute_source_route(dest, target)) {
        return false;
    }

    const uint8_t *next_hop = rpl_data_sr_next_hop();
    if (next_hop) {
        memcpy(route->next_hop_addr, next_hop, 16);
    } else {
        memcpy(route->next_hop_addr, dest, 16);
    }

    return true;
}

void rpl_data_sr_invalidate(void)
{
    if (rpl_data_sr) {
        rpl_data_sr->target = NULL;
        rpl_data_sr->ihops = 0;
    }
    /* We could invalidate the next hops remembered in the system routing table.
     * but it's not necessary - recomputation happens every time. Does mean that
     * the routing table printout may contain stale info, though.
     */
}

typedef struct rpl_srh_info
{
    uint8_t hlen;
    uint8_t segments;
    uint8_t cmprI;
    uint8_t cmprE;
    uint8_t pad;
} rpl_srh_info_t;

/* Count matching bytes (max 15) for SRH compression */
static uint_fast8_t rpl_data_matching_addr_bytes(const uint8_t *a, const uint8_t *b, uint_fast8_t len)
{
    uint_fast8_t m = 0;
    while (m < len && a[m] == b[m]) {
        m++;
    }
    return m;
}

static const rpl_srh_info_t *rpl_data_sr_compute_header_size(const uint8_t final_dest[16], uint8_t hop_limit)
{
    static rpl_srh_info_t info;
    uint8_t hops = 1 + rpl_data_sr->ihops;
    if (hops > hop_limit) {
        hops = hop_limit;
    }
    if (hops <= 1) {
        return NULL;
    }
    memcpy(rpl_data_sr->final_dest, final_dest, 16);
    /* first_hop is the address that will go into the IP destination */
    const uint8_t *first_hop = rpl_data_sr->iaddr + 16 * (rpl_data_sr->ihops - 1);
    /* addr is the first address for the SRH */
    const uint8_t *addr = first_hop - 16;

    /* Must be at least 2 hops, so at least 1 segment in the SRH */
    info.segments = hops - 1;

    /* First, scan for compression of all except last against initial destination */
    /* (CmprI bytes will remain unchanged at each hop, rest can change) */
    info.cmprI = 15;
    for (uint8_t seg = 0; seg < info.segments - 1; seg++) {
        info.cmprI = rpl_data_matching_addr_bytes(addr, first_hop, info.cmprI);
        hops--;
        addr -= 16;
    }

    /* Compress last hop against previous destination */
    /* Debatable whether we should let cmprE be > cmprI - it means the final
     * address won't be IP_dest[0:cmprI)+Address_n[cmprI:64) until the final
     * hop (segments left = 1):
     *
     *      CmprI = 14, CmprE = 15
     *      IP dest  Segs Left    Addresses
     *      x:1234      3            2345, 3456, 22 <- meaning "3422", not "1222"
     *      x:2345      2            1234, 3456, 22
     *      x:3456      1            1234, 2345, 22
     *      x:3422      0            1234, 2345, 56
     *
     * But then similar issues arise if cmprE < cmprI:
     *
     *      CmprI = 15, CmprE = 14
     *      IP dest  Segs Left    Addresses
     *      x:1234      3            45, 56, ABCD
     *      x:1245      2            34, 56, ABCD
     *      x:1256      1            34, 45, ABCD
     *      x:ABCD      0            34, 45, 1256  <- "45" means "1245", not "AB45"
     *
     * Basically, there's no loss of information, but it's not as straightforward
     * as RFC 6554 says. If cmprI != cmprE, not all Address entries represent
     * addresses with the same prefix as the IP destination at any given
     * instant. (But the next address to process does line up with the current
     * IP destination).
     *
     */
    info.cmprE = rpl_data_matching_addr_bytes(addr, addr + 16, 15 /* info.cmprI */);

    uint16_t total_size;

    total_size = (16 - info.cmprE) + (16 - info.cmprI) * (info.segments - 1);
    if (total_size & 7) {
        info.pad = 8 - (total_size & 7);
        total_size += info.pad;
    } else {
        info.pad = 0;
    }
    info.hlen = total_size >> 3;

    return &info;
}

/*
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |  Next Header  |  Hdr Ext Len  | Routing Type  | Segments Left |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   | CmprI | CmprE |  Pad  |               Reserved                |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   .                                                               .
 *   .                        Addresses[1..n]                        .
 *   .                                                               .
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
static uint8_t *rpl_data_sr_write_header(const rpl_srh_info_t *info, uint8_t *ptr, uint8_t nh)
{
    ptr[0] = nh;
    ptr[1] = info->hlen;
    ptr[2] = IPV6_ROUTING_TYPE_RPL;
    ptr[3] = info->segments;
    ptr[4] = (info->cmprI << 4) | info->cmprE;
    ptr[5] = (info->pad << 4);
    common_write_16_bit(0, ptr + 6);
    ptr += 8;
    const uint8_t *addr = rpl_data_sr->iaddr + 16 * (rpl_data_sr->ihops - 2);
    for (int n = 0; n < info->segments - 1; n++) {
        memcpy(ptr, addr + info->cmprI, 16 - info->cmprI);
        ptr += 16 - info->cmprI;
        addr -= 16;
    }
    memcpy(ptr, addr + info->cmprE, 16 - info->cmprE);
    ptr += 16 - info->cmprE;
    if (info->pad) {
        memset(ptr, 0, info->pad);
        ptr += info->pad;
    }
    return ptr;
}

static buffer_t *rpl_data_exthdr_provider_srh(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result)
{
    ipv6_route_info_t *route_info = &buf->route->route_info;
    rpl_dao_target_t *target = route_info->info;
    rpl_instance_t *instance = target->instance;
    if (!instance) {
        *result = -1;
        return buf;
    }

    uint16_t ext_size = 0;
    const rpl_srh_info_t *srh_info = NULL;

    const uint8_t *final_rpl_dest = buf->dst_sa.address;

    if (target->external) {
        /* If we haven't yet tunnelled, then there's no insertion */
        if (!buf->options.tunnelled) {
            if (stage == IPV6_EXTHDR_SIZE || stage == IPV6_EXTHDR_INSERT) {
                *result = 0;
                return buf;
            }
        }

        /* If it's an external target, we need to only go as far as its transit */
        /* Modify target to point to that instead */
        rpl_dao_root_transit_t *transit = ns_list_get_first(&target->info.root.transits);
        if (!transit) {
            *result = -1;
            return buf;
        }
        final_rpl_dest = transit->transit;
        target = rpl_instance_match_dao_target(instance, final_rpl_dest, 128);
        if (!target) {
            *result = -1;
            return buf;
        }
    }

    if (!rpl_data_compute_source_route(final_rpl_dest, target)) {
        *result = -1;
        return buf;
    }

    /* When tunnelling (only), we truncate the route in the outer packet,
     * according to the hop limit, so it exits the tunnel at the hop limit -
     * that router will then generate "time exceeded" on the inner packet.
     * (RFC 6554 4.1). When not tunnelling, we include all hops regardless,
     * which means the final destination is there as needed.
     */
    srh_info = rpl_data_sr_compute_header_size(final_rpl_dest, buf->options.tunnelled && buf->options.type == IPV6_NH_IPV6 ? buf->options.hop_limit : 0xFF);
    if (!srh_info) {
        /* No source routing header required - this must be because it's one hop. */
        /* In this case, we do need to add a HbH option header */
        return rpl_data_exthdr_provider_hbh_2(buf, instance, NULL, stage, result);
    }
    ext_size = 8 * (srh_info->hlen + 1);

    switch (stage) {
        case IPV6_EXTHDR_SIZE:
            *result = ext_size;
            return buf;

        case IPV6_EXTHDR_INSERT: {
            buf = buffer_headroom(buf, ext_size);
            if (!buf) {
                return NULL;
            }
            uint8_t *ext = buffer_data_reserve_header(buf, ext_size);
            rpl_data_sr_write_header(srh_info, ext, buf->options.type);
            buf->route->ip_dest = rpl_data_sr_next_hop();
            buf->options.type = IPV6_NH_ROUTING;
            // don't forget to set the "RPL option present" marker
            buf->options.ip_extflags |= IPEXT_SRH_RPL;
            *result = 0;
            return buf;
        }

        case IPV6_EXTHDR_MODIFY:
            if (buf->options.ip_extflags & IPEXT_SRH_RPL) {
                *result = 0;
                return buf;
            }
            if (final_rpl_dest != buf->dst_sa.address) {
                memcpy(buf->dst_sa.address, final_rpl_dest, 16);
            }
            *result = IPV6_EXTHDR_MODIFY_TUNNEL;
            buf->src_sa.addr_type = ADDR_NONE; // force auto-selection
            return buf;

        default:
            return buffer_free(buf);
    }
}
#endif // HAVE_RPL_ROOT

buffer_t *rpl_data_process_routing_header(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t *hdrlen_out, bool *forward_out)
{
    /* Handling procedures based on RFC 6554 4.2 */

    /* Do not process RPL source routing headers unless they arrive on a RPL interface */
    if (!cur->rpl_domain) {
        tr_warn("SRH RX non-RPL if");
    drop:
        protocol_stats_update(STATS_IP_RX_DROP, 1);
        return buffer_free(buf);
    }

    buf->options.ip_extflags |= IPEXT_SRH_RPL;

    uint16_t hlen = (ptr[1] + 1) * 8;
    uint8_t segs_left = ptr[3];

    if (segs_left == 0) {
        *hdrlen_out = hlen;
        return buf;
    }
    uint8_t cmprI = ptr[4] >> 4;
    uint8_t cmprE = ptr[4] & 0xF;
    uint8_t pad = ptr[5] >> 4;


    /* Should really be more rigorous here */
    uint_fast16_t n_addrs = ((hlen - 8 - pad - (16 - cmprE)) / (16 - cmprI)) + 1;
    if (segs_left > n_addrs) {
        return icmpv6_error(buf, cur, ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM, ICMPV6_CODE_PARAM_PRB_HDR_ERR, (ptr + 3) - buffer_data_pointer(buf));
    }

    uint8_t *ip_dst = buffer_data_pointer(buf) + 24;

    /* Decrement segments left */
    segs_left = --ptr[3];

    if (addr_is_ipv6_multicast(buf->dst_sa.address)) {
        goto drop;
    }

    /* Next address index (starting at 1, as per RFC 6554) */
    uint_fast16_t next_addr_i = n_addrs - segs_left;

    /* Locate next address: aptr -> Address[i] */
    uint8_t *aptr = ptr + 8 + (next_addr_i - 1) * (16 - cmprI);
    uint8_t cmpr = next_addr_i == n_addrs ? cmprE : cmprI;
    uint8_t asize = 16 - cmpr;

    /* Next address can only be multicast if compression is 0, otherwise
     * it inherits an already-checked non-0xFF start byte from IP destination
     */
    if (cmpr == 0 && addr_is_ipv6_multicast(aptr)) {
        goto drop;
    }

    /* Look for a loop. Scanning all addresses 1..n would be a pain, given
     * the possibility of weird cmprI/E combinations. But there's logically
     * no need to look at _previous_ addresses. And we know the packet was
     * addressed to us to reach us - Address[i-1] must have been ours. So
     * to ensure we do the test in RFC 6554:
     *
     *    if 2 or more entries in Address[1..n] are assigned to
     *           local interface and are separated by at least one
     *           address not assigned to local interface
     *
     * We just check Address[i..n], knowing that i-1 was ours. It's impossible
     * for anything older than i-1 to be ours, as we would have failed the
     * check previously...
     *
     * There need to be at least 2 more addresses (i and i+1) for this check
     * to be useful.
     */
    if (next_addr_i + 1 <= n_addrs) {
        uint8_t addr[16];
        /* We know Address[i-1] was ours */
        bool prev_was_local = true;
        /* Initialise pointers to examine Address[i] */
        memcpy(addr, ip_dst, 16);
        const uint8_t *a = aptr;
        /* Then scan remaining */
        for (uint_fast16_t i = next_addr_i; i <= n_addrs; i++) {
            uint8_t cpr = i == n_addrs ? cmprE : cmprI;
            uint8_t asz = 16 - cpr;
            memcpy(addr + cpr, a, asz);
            a += asz;
            bool local_addr = addr_interface_address_compare(cur, addr) == 0;
            if (local_addr) {
                if (!prev_was_local) {
                    protocol_stats_update(STATS_RPL_ROUTELOOP, 1);
                    tr_warn("SRH loop");
                    return icmpv6_error(buf, cur, ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM, ICMPV6_CODE_PARAM_PRB_HDR_ERR, aptr - buffer_data_pointer(buf));
                }
            }
            prev_was_local = local_addr;
        }
    }

    /* Swap the destination and Address[i]  */
    memswap(ip_dst + cmpr, aptr, asize);

    /* And update the metadata */
    memcpy(buf->dst_sa.address, ip_dst, 16);

    /* Need to fake up routing here. Basically, for the common (shared-prefix)
     * case, we have to assume that the destination is on-link, on an assumed
     * interface. We have no direct record of the people trying to use us
     * as DAO parents, so we will get totally unknown addresses in SRHs.
     * We add routing info to the buffer to say that the IP destination is
     * the next hop, and the source being "SRH" is the IP layer's cue to send
     * "Error in Source Routing Header" codes instead of "Address Unreachable".
     */
    buffer_free_route(buf);

    /* THINK: May want to check to see if the address is already known to be on-link on
     * an interface.  Won't be the usual 6LoWPAN case though.
     */

    /* Policy gets to decide whether we will take this - it can do neighbour state checks */
    protocol_interface_info_entry_t *next_if =
            protocol_stack_interface_info_get_by_id(
                    rpl_policy_srh_next_hop_interface(cur->rpl_domain, cur->id,
                                                      buf->dst_sa.address));
    if (!next_if) {
        goto error;
    }

    buffer_routing_info_t *route = ipv6_buffer_route_to(buf, buf->dst_sa.address, next_if);
    if (!route) {
        /* Shouldn't happen here? Out of memory case? */
        goto error;
    }
    route->route_info.source = ROUTE_RPL_SRH;

    *forward_out = true;
    return buf;

error:
    return icmpv6_error(buf, cur, ICMPV6_TYPE_ERROR_DESTINATION_UNREACH, ICMPV6_CODE_DST_UNREACH_SRC_RTE_HDR_ERR, 0);
}

bool rpl_data_get_srh_last_address(const uint8_t *rh, uint8_t *addr_out)
{
    uint_fast8_t len = rh[1];
    uint_fast8_t segs_left = rh[3];
    uint_fast8_t cmpr_i = rh[4] >> 4;
    uint_fast8_t cmpr_e = rh[4] & 0xF;
    uint_fast8_t pad = rh[5] >> 4;

    const uint8_t *last_addr_ptr = rh + 8 + (len * 8) - pad - (16 - cmpr_e);

    if (segs_left == 0) {
        return true;
    }

    if (segs_left > 1) {
        /* Get last "I" destination in */
        memcpy(addr_out + cmpr_i, last_addr_ptr - (16 - cmpr_i), 16 - cmpr_i);
    }

    /* Then modify "E" destination */
    memcpy(addr_out + cmpr_e, last_addr_ptr, 16 - cmpr_e);
    return true;
}

/* Set up handlers for general RPL nodes (hop-by-hop headers, DIO routes) */
void rpl_data_init(void)
{
    ipv6_route_table_set_predicate_fn(ROUTE_RPL_INSTANCE, rpl_data_route_predicate_instance_default);
    ipv6_set_exthdr_provider(ROUTE_RPL_INSTANCE, rpl_data_exthdr_provider_hbh);
    ipv6_set_exthdr_provider(ROUTE_RPL_DIO, rpl_data_exthdr_provider_hbh);
    ipv6_set_exthdr_provider(ROUTE_RPL_ROOT, rpl_data_exthdr_provider_hbh);
    ipv6_set_exthdr_provider(ROUTE_RPL_FWD_ERROR, rpl_data_exthdr_provider_fwd_error_hbh);
}

#ifdef HAVE_RPL_ROOT
/* Set up handlers for DODAG root (creation of source routing headers) */
void rpl_data_init_root(void)
{
    ipv6_set_exthdr_provider(ROUTE_RPL_DAO_SR, rpl_data_exthdr_provider_srh);
    ipv6_route_table_set_next_hop_fn(ROUTE_RPL_DAO_SR, rpl_data_route_next_hop);
}
#endif

#endif /* HAVE_RPL */
