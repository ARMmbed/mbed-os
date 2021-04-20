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
/*
 * MLD version 1 (RFC 2710) implementation
 *
 * RFC 6434 (IPv6 Node Requirements) says nodes MUST support MLDv1, and that
 * nodes SHOULD support MLDv2 (possibly lightweight).
 *
 * As we don't support Source-Specific Multicast, nothing is really gained
 * by supporting MLDv2. The statement in RFC 6434 that "the presence of a single
 * MLDv1 participant on a link requires that all other nodes on the link operate
 * in version 1 compatibility mode" seems to be misleading. Our presence doesn't
 * affect hosts, and it doesn't have much worse effect on the router or network
 * than the presence of an MLDv2 Any-Source-Multicast host for that group.
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "common_functions.h"
#include "randLIB.h"
#include <string.h>

#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ip.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/mld.h"
#include "6LoWPAN/Thread/thread_common.h"

#define MLD_UNSOLICITED_REPORT_INTERVAL (10 * 10) /* 10 seconds */


#if 0
// MLDv2 fragments - not yet

/* If Maximum Response Code >=32768, Maximum Response Code represents a
 * floating-point value as follows:
 *
 *     0 1 2 3 4 5 6 7 8 9 A B C D E F
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |1| exp |          mant         |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Maximum Response Delay = (mant | 0x1000) << (exp+3)
 */

/* Convert Maximum Response Code to Maximum Response Delay (milliseconds) */
/* Maximum value is 0x7FFC00 (~2 hours) */
static inline uint_least24_t mldv2_mrd_from_mrc(uint16_t mrc)
{
    if (mrc < 0x8000) {
        return mrc;
    } else {
        uint_fast16_t exp = (mrc & 0x7000) >> 12;
        uint_fast16_t mant = mrc & 0x0FFF;
        return (UINT24_C(0x1000) | mant) << (exp + 3);
    }
}

/* Convert Maximum Response Delay (milliseconds) to Maximum Response Code */
/* Maximum representable value is (0x1FFF << 10) = 0x7FFC00 (~2 hours) */
static inline uint16_t mldv2_mrc_from_mrd(uint_least24_t mrd)
{
    if (mrd < 0x8000) {
        return mrd;
    } else if (mrd >= 0x800000) {
        return 0xFFFF;
    } else {
        /* mrd is in range 0x8000 to 0x7FFFFF, so clz32 returns 9-16, so exp is 0-7 */
        /* Example: 0x123456 - clz = 11; exp = 5; mant = 0x234; result = 0x95 */
        uint16_t exp = 16 - common_count_leading_zeros_32(mrd);
        uint16_t mant = (mrd >> (exp + 3)) & 0x0FFF;
        return 0x8000 | (exp << 12) | mant;
    }
}

/*
 * If QQIC >= 128, QQIC represents a floating-point value as follows:
 *
 *     0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+
 *    |1| exp | mant  |
 *    +-+-+-+-+-+-+-+-+
 *
 * QQI = (mant | 0x10) << (exp + 3)
 */

/* Convert Querier's Query Interval Code to Querier's Query Interval (seconds) */
/* Maximum value is 0x7C00 (~9 hours) */
static inline uint16_t mldv2_qqi_from_qqic(uint8_t qqic)
{
    if (qqic < 0x80) {
        return qqic;
    } else {
        uint_fast8_t exp = (qqic & 0x70) >> 4;
        uint_fast8_t mant = qqic & 0x0F;
        return (UINT16_C(0x10) | mant) << (exp + 3);
    }
}

/* Convert Querier's Query Interval (seconds) to Querier's Query Interval Code */
/* Maximum representable value is (0x1f << 10) = 0x7C00 (~9 hours) */
static inline uint8_t mldv2_qqic_from_qqi(uint16_t qqi)
{
    if (qqi < 0x80) {
        return qqi;
    } else if (qqi >= 0x8000) {
        return 0xFF;
    } else {
        /* qqi is in range 0x80 to 0x7FFF, so clz16 returns 1-8, so exp is 0-7 */
        /* Example: 0x0151 - clz = 7; exp = 1; mant = 0x5; result = 0x95 */
        uint8_t exp = 8 - common_count_leading_zeros_16(qqi);
        uint8_t mant = (qqi >> (exp + 3)) & 0x0F;
        return 0x80 | (exp << 4) | mant;
    }
}
#endif

/*
 * RFC 2710 Multicast Listener Discovery message
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Type      |     Code      |          Checksum             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Maximum Response Delay    |          Reserved             |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                                                               |
 * +                                                               +
 * |                                                               |
 * +                       Multicast Address                       +
 * |                                                               |
 * +                                                               +
 * |                                                               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
buffer_t *mld_build(protocol_interface_info_entry_t *cur, uint8_t type, uint16_t max_response_delay, const uint8_t address[static 16])
{
    uint8_t *ptr;

    /* Validity checks - must be multicast, scope >= link-local */
    if (!addr_is_ipv6_multicast(address) || addr_ipv6_multicast_scope(address) <= IPV6_SCOPE_INTERFACE_LOCAL) {
        /* Except we allow unspecified for general query */
        if (!(type == ICMPV6_TYPE_INFO_MCAST_LIST_QUERY && address == ADDR_UNSPECIFIED)) {
            return NULL;
        }
    }

    buffer_t *buf = buffer_get(20);
    if (!buf) {
        return NULL;
    }

    buf->options.type = type;
    buf->options.code = 0;
    buf->options.hop_limit = 1;
    buf->options.ip_extflags |= IPEXT_HBH_ROUTER_ALERT;
    buf->options.multicast_loop = true;
    buf->options.mpl_permitted = false;
    buf->options.traffic_class = IP_DSCP_CS5 << IP_TCLASS_DSCP_SHIFT; /* RFC 4594 */
    buf->interface = cur;
    buf->info = (buffer_info_t)(B_DIR_DOWN | B_FROM_ICMP | B_TO_ICMP);

    buf->src_sa.addr_type = ADDR_IPV6;
    if (addr_interface_get_ll_address(cur, buf->src_sa.address, 0) < 0) {
        memset(buf->src_sa.address, 0, 16);
    }
    buf->dst_sa.addr_type = ADDR_IPV6;
    if (type == ICMPV6_TYPE_INFO_MCAST_LIST_DONE) {
        memcpy(buf->dst_sa.address, ADDR_LINK_LOCAL_ALL_ROUTERS, 16);
    } else if (type == ICMPV6_TYPE_INFO_MCAST_LIST_QUERY && address == ADDR_UNSPECIFIED) {
        memcpy(buf->dst_sa.address, ADDR_LINK_LOCAL_ALL_NODES, 16);
    } else {
        memcpy(buf->dst_sa.address, address, 16);
    }

    /* MLD packets are implicitly on-link - bypass routing */
    ipv6_buffer_route_to(buf, buf->dst_sa.address, cur);
    if (!buf->route) {
        goto invalid;
    }

    ptr = buffer_data_pointer(buf);

    ptr = common_write_16_bit(max_response_delay, ptr);
    ptr = common_write_16_bit(0, ptr);
    memcpy(ptr, address, 16);
    ptr += 16;
    buffer_data_end_set(buf, ptr);

    return buf;

invalid:
    if (buf) {
        buffer_free(buf);
    }

    return NULL;
}

static void mld_query_entry(if_group_entry_t *entry, uint16_t mrd)
{
    /* Don't do MLD for low-scope groups - ff02::1 already excluded by being in the implicit list */
    if (addr_ipv6_scope(entry->group, NULL) < IPV6_SCOPE_LINK_LOCAL) {
        return;
    }

    /* "Query received" event */
    /* Timer starts/resets if it's not running, or it's set to greater than MRD */
    if (entry->mld_timer == 0 || mrd < entry->mld_timer) {
        entry->mld_timer = randLIB_get_random_in_range(1, mrd);
    }
}

bool mld_querier = false;

buffer_t *mld_query_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    /* Thread has its own mechanism (as ever), and borrows the mld_timer field,
     * so we can't do MLD.
     */
    if (thread_info(cur)) {
        goto invalid;
    }

    /* RFC 4541 (MLD Snooping Switches) says we should accept unspecified source */
    if (!(addr_is_ipv6_link_local(buf->src_sa.address) || addr_is_ipv6_unspecified(buf->src_sa.address)) || buffer_data_length(buf) < 20) {
        goto invalid;
    }

    const uint8_t *ptr = buffer_data_pointer(buf);
    uint16_t mrd = common_read_16_bit(ptr);
    const uint8_t *group = ptr + 4;
    if (!addr_is_ipv6_multicast(group) && !addr_is_ipv6_unspecified(group)) {
        goto invalid;
    }

    if (mld_querier && !addr_is_ipv6_unspecified(buf->src_sa.address)) {
        uint8_t our_addr[16];
        if (addr_interface_get_ll_address(cur, our_addr, 0) >= 0) {
            if (memcmp(buf->src_sa.address, our_addr, 16) < 0) {
                mld_querier = false;
            }
        }
    }

    /* Convert Maximum Response Delay from milliseconds to ticks. If they say "0" they get 1 tick */
    mrd /= 100;
    if (mrd == 0) {
        mrd = 1;
    }

    if (addr_is_ipv6_multicast(group)) {
        /* Address-Specific Query */
        if_group_entry_t *entry = addr_get_group_entry(cur, group);
        if (!entry) {
            goto invalid;
        }
        mld_query_entry(entry, mrd);
    } else {
        /* General Query */
        ns_list_foreach(if_group_entry_t, entry, &cur->ip_groups) {
            mld_query_entry(entry, mrd);
        }
    }

invalid:
    if (buf) {
        buffer_free(buf);
    }

    return NULL;
}

buffer_t *mld_report_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    if (thread_info(cur)) {
        goto invalid;
    }

    if (!addr_is_ipv6_link_local(buf->src_sa.address) || buffer_data_length(buf) < 20) {
        goto invalid;
    }

    const uint8_t *ptr = buffer_data_pointer(buf);
    const uint8_t *group = ptr + 4;
    if (!addr_is_ipv6_multicast(group)) {
        goto invalid;
    }

    if_group_entry_t *entry = addr_get_group_entry(cur, group);
    if (!entry) {
        goto invalid;
    }

    /* "Report received" event - stop timer, clear flag, become Idle Listener */

    if (entry->mld_timer && !addr_is_assigned_to_interface(cur, buf->src_sa.address)) {
        entry->mld_timer = 0;
        entry->mld_last_reporter = false;
    }

invalid:
    if (buf) {
        buffer_free(buf);
    }

    return NULL;
}

void mld_start_listening(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
    if (thread_info(interface)) {
        thread_mcast_group_change(interface, entry, true);
        return;
    }

    /* "Send MLD" flag only controls sending unsolicited reports when we join. We will still always respond if queried */
    if (interface->send_mld && addr_ipv6_multicast_scope(entry->group) >= IPV6_SCOPE_LINK_LOCAL) {
        entry->mld_timer = randLIB_get_random_in_range(1, MLD_UNSOLICITED_REPORT_INTERVAL);
        entry->mld_last_reporter = true;
        protocol_push(mld_build(interface, ICMPV6_TYPE_INFO_MCAST_LIST_REPORT, 0, entry->group));
    } else {
        entry->mld_timer = 0;
        entry->mld_last_reporter = false;
    }
}

void mld_stop_listening(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
    if (thread_info(interface)) {
        thread_mcast_group_change(interface, entry, false);
        return;
    }

    if (entry->mld_last_reporter) {
        protocol_push(mld_build(interface, ICMPV6_TYPE_INFO_MCAST_LIST_DONE, 0, entry->group));
    }
}

void mld_slow_timer(protocol_interface_info_entry_t *interface, uint_fast16_t seconds)
{
    static uint8_t query_timer;
    query_timer += seconds;
    if (query_timer >= 125) {
        query_timer = 0;
        if (mld_querier) {
            protocol_push(mld_build(interface, ICMPV6_TYPE_INFO_MCAST_LIST_QUERY, 10000, ADDR_UNSPECIFIED));
        }
    }
}

void mld_fast_timer(protocol_interface_info_entry_t *interface, uint_fast16_t ticks)
{
    if (thread_info(interface)) {
        return;
    }

    ns_list_foreach(if_group_entry_t, entry, &interface->ip_groups) {
        if (entry->mld_timer == 0) {
            continue;
        }

        if (entry->mld_timer > ticks) {
            entry->mld_timer -= ticks;
            continue;
        }

        entry->mld_timer = 0;
        entry->mld_last_reporter = true;
        protocol_push(mld_build(interface, ICMPV6_TYPE_INFO_MCAST_LIST_REPORT, 0, entry->group));
    }
}
