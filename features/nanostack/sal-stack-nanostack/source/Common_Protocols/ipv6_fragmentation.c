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
/* IPv6 fragmentation and defragmentation
 *
 * (Could fairly easily be modified to also do IPv4)
 *
 * References:
 *
 * RFC  815   IP Datagram Reassembly Algorithms
 * RFC 2460   Internet Protocol. Version 6 (IPv6) Specification
 * RFC 3168   The Addition of Explicit Congestion Notification (ECN) to IP
 * RFC 5722   Handling of Overlapping IPv6 Fragments
 * RFC 6040   Tunnelling of Explicit Congestion Notification
 * RFC 6145   IP/ICMP Translation Algorithm [sections on Path MTU]
 * RFC 6660   Encoding Three Pre-Congestion Notification (PCN) States in the
 *            IP Header Using a Single Diffserv Codepoint (DSCP)
 * RFC 6946   Processing of IPv6 "Atomic" Fragments
 * RFC 7112   Implications of Oversized IPv6 Header Chains
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "ns_trace.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ip.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"

#include "NWK_INTERFACE/Include/protocol_stats.h"

#define TRACE_GROUP "Ifrg"

/*                         FRAGMENT REASSEMBLY
 *
 * Allow fragment RX to be disabled for really constrained systems.
 * This would violate RFC 2460 and RFC 6434 - all IPv6 nodes must be able to
 * process fragment headers and reassemble 1500-octet datagrams.
 */
#ifndef NO_IP_FRAGMENT_RX

static uint16_t ipv6_frag_mru = IPV6_FRAG_MRU;

typedef struct ip_fragmented_datagram {
    uint8_t age;
    bool discard;               /* Set to ignore all future fragments (and not send Time Exceeded) */
    bool had_last;
    int8_t ecn;
    uint32_t id;
    uint16_t fragmentable;      /* Offset in buf->buf[] of fragmentable part */
    uint16_t first_hole;        /* Offset of first hole (relative to fragmentable part) */
    buffer_t *buf;
    ns_list_link_t link;
} ip_fragmented_datagram_t;

/* We reassemble into the datagram buffer in basically the style of RFC 815 */
/* An 6-byte hole descriptor is placed directly in buffer holes */
/* We link them them by buffer offset (relative to start of fragmentable section) */
/* Note the possible need to align means we can't use more than 7 bytes */
typedef struct hole {
    uint16_t first;
    uint16_t last;
    uint16_t next;
} hole_t;

/* Given the offset of a hole in the datagram buffer, return an aligned pointer
 * to put a hole_t in it. We assume a "normal" platform requiring 2-byte
 * alignment for hole_t, and letting us manipulate uintptr_t in the conventional
 * fashion.
 */
static hole_t *hole_pointer(const ip_fragmented_datagram_t *dgram, uint16_t offset)
{
    uintptr_t ptr = (uintptr_t)(dgram->buf->buf + dgram->fragmentable + offset);

    return (hole_t *)((ptr + 1) & ~(uintptr_t) 1);
}

static NS_LIST_DEFINE(frag_list, ip_fragmented_datagram_t, link);

/* Maximum time to hold fragments in seconds */
#define FRAG_TTL 60

/* How many partially-assembled datagrams we will hold */
#define MAX_FRAG_DATAGRAMS 4

/* Dummy negative ECN value used during assembly */
#define IP_ECN__ILLEGAL (-1)

/* RFC 5722 - discard already-received *and future* fragments */
static void invalidate_datagram(ip_fragmented_datagram_t *dgram)
{
    // Would like to free the buffer here, but it contains the
    // source and destination address we need to match the datagram entry.
    dgram->discard = true;
}

static void free_datagram(ip_fragmented_datagram_t *dgram)
{
    ns_list_remove(&frag_list, dgram);
    if (dgram->buf) {
        buffer_free(dgram->buf);
    }
    ns_dyn_mem_free(dgram);
}

/* We would be in trouble if last fragment is < 8 bytes, and we didn't have
 * room for the hole descriptor. Avoid a problem by ensuring that we always
 * allocate a multiple-of-8 reassembly buffer.
 */
uint16_t ipv6_frag_set_mru(uint16_t frag_mru)
{
    frag_mru = (frag_mru + 7) &~ UINT16_C(7);
    if (frag_mru < IPV6_MIN_FRAG_MRU) {
        frag_mru = (IPV6_MIN_FRAG_MRU + 7) &~ UINT16_C(7);
    }
    if (ipv6_frag_mru != frag_mru) {
        /* I don't want to worry about the complications of changing MRU while
         * we've got ongoing reassembly. Simplest just to drop any pending.
         */
        ns_list_foreach_safe(ip_fragmented_datagram_t, dgram, &frag_list) {
            free_datagram(dgram);
        }
        ipv6_frag_mru = frag_mru;
    }
    return ipv6_frag_mru;
}

void ipv6_frag_timer(uint8_t secs)
{
    ns_list_foreach_safe(ip_fragmented_datagram_t, dgram, &frag_list) {
        if ((dgram->age += secs) > FRAG_TTL) {
            uint16_t first_hole = dgram->first_hole;
            /* If we've received the first fragment, can send "time exceeded" */
            if (first_hole != 0 && !dgram->discard) {
                /* Take as much as we've got, up to first hole; icmpv6_error will limit to min MTU */
                dgram->buf->buf_end = dgram->fragmentable + first_hole;
                /* Fill in IP header length */
                common_write_16_bit(buffer_data_length(dgram->buf) - 40, buffer_data_pointer(dgram->buf) + 4);

                buffer_t *err = icmpv6_error(dgram->buf, NULL, ICMPV6_TYPE_ERROR_TIME_EXCEEDED, ICMPV6_CODE_TME_EXCD_FRG_REASS_TME_EXCD, 0);
                protocol_push(err);
                dgram->buf = NULL;
            }
            free_datagram(dgram);
        }
    }
}

static void delete_hole(ip_fragmented_datagram_t *dgram, uint16_t hole, uint16_t *prev_ptr)
{
    hole_t *hole_ptr = hole_pointer(dgram, hole);

    *prev_ptr = hole_ptr->next;
}

static hole_t *create_hole(ip_fragmented_datagram_t *dgram, uint16_t first, uint16_t last, uint16_t *prev_ptr)
{
    hole_t *hole_ptr = hole_pointer(dgram, first);
    hole_ptr->first = first;
    hole_ptr->last = last;
    hole_ptr->next = *prev_ptr;

    *prev_ptr = first;
    return hole_ptr;
}

static ip_fragmented_datagram_t *ip_frag_dgram_lookup(buffer_t *buf, uint32_t id, uint16_t unfrag_len)
{
    int_fast8_t count = 0;
    ns_list_foreach(ip_fragmented_datagram_t, dgram, &frag_list) {
        if (id == dgram->id &&
                addr_ipv6_equal(buf->src_sa.address, dgram->buf->src_sa.address) &&
                addr_ipv6_equal(buf->dst_sa.address, dgram->buf->dst_sa.address)) {
            return dgram;
        }
        count++;
    }

    /* Not found - create one */
    if (count >= MAX_FRAG_DATAGRAMS) {
        free_datagram(ns_list_get_last(&frag_list));
    }

    ip_fragmented_datagram_t *new_dgram = ns_dyn_mem_temporary_alloc(sizeof(ip_fragmented_datagram_t));
    if (!new_dgram) {
        return NULL;
    }

    /* We track payload holes as per RFC 815, roughly, and reserve header
     * room in front, based on the unfragmentable size of the first-received
     * fragment.
     *
     * So initial state is:
     *
     * buf_ptr -> default buffer headroom + first-received-fragment header size
     * fragmentable = buf_end = buf_ptr = offset of where fragments are assembled.
     *
     * When we receive the first (0-offset) fragment, we move down buf_ptr to
     * put in its header, and when we receive the final (M=0) fragment, we
     * set buf_end accordingly.
     *
     * Two odd cases to worry about:
     *
     *   1) First fragment is not received first, and has a larger
     *      header than our first-received fragment. In this case, we
     *      shuffle data if required when we get that first fragment.
     *      (Actual shuffle will normally be avoided by buffer headroom slack).
     *   2) First fragment is not received first, and has a smaller
     *      header than our first-received fragment, meaning an IPV6_MRU-sized
     *      datagram may have more fragmented payload than we expected. Avoid
     *      a problem in this case by allocating a bigger-than-IPV6_MRU buffer
     *      if first-received fragment has extension headers.
     */
    new_dgram->buf = buffer_get(unfrag_len + ipv6_frag_mru - 40);
    if (!new_dgram->buf) {
        ns_dyn_mem_free(new_dgram);
        return NULL;
    }

    new_dgram->fragmentable = new_dgram->buf->buf_end = new_dgram->buf->buf_ptr += unfrag_len;
    new_dgram->first_hole = 0xffff;
    create_hole(new_dgram, 0, 0xffff, &new_dgram->first_hole);

    new_dgram->buf->src_sa = buf->src_sa;
    new_dgram->buf->dst_sa = buf->dst_sa;
    new_dgram->id = id;
    new_dgram->age = 0;
    new_dgram->discard = false;
    new_dgram->had_last = false;
    new_dgram->ecn = buf->options.traffic_class & IP_TCLASS_ECN_MASK;
    ns_list_add_to_start(&frag_list, new_dgram);

    return new_dgram;
}

/*
 * 4x4 combination array implementing the ECN combination rules from RFC 3168.
 *
 * Summary visualisation:      N10C
 *                            +----
 *                           N|NNN-
 *                           1|N11C
 *                           0|N10C
 *                           C|-CCC
 *
 * Each of the 16 entries, with justification:
 */
static const int8_t frag_ecn_combination[4][4] = {
    // We MUST preserve the ECN codepoint when all fragments match.
    [IP_ECN_NOT_ECT][IP_ECN_NOT_ECT] = IP_ECN_NOT_ECT,
    [IP_ECN_ECT_0  ][IP_ECN_ECT_0  ] = IP_ECN_ECT_0,
    [IP_ECN_ECT_1  ][IP_ECN_ECT_1  ] = IP_ECN_ECT_1,
    [IP_ECN_CE     ][IP_ECN_CE     ] = IP_ECN_CE,

    // We MUST set CE if any fragment has CE...
    [IP_ECN_CE     ][IP_ECN_ECT_0  ] = IP_ECN_CE,
    [IP_ECN_CE     ][IP_ECN_ECT_1  ] = IP_ECN_CE,
    [IP_ECN_ECT_0  ][IP_ECN_CE     ] = IP_ECN_CE,
    [IP_ECN_ECT_1  ][IP_ECN_CE     ] = IP_ECN_CE,

    // ...except we MUST drop the packet if we see CE + Not-ECT.
    [IP_ECN_CE     ][IP_ECN_NOT_ECT] = IP_ECN__ILLEGAL,
    [IP_ECN_NOT_ECT][IP_ECN_CE     ] = IP_ECN__ILLEGAL,

    // For the remaining cases, RFC 3168 leaves us free to do anything.
    // To make the above CE+Not-ECT rule work in all delivery orders, with
    // intervening ECT fragments, Not-ECT overrides ECT.
    [IP_ECN_NOT_ECT][IP_ECN_ECT_0  ] = IP_ECN_NOT_ECT,
    [IP_ECN_NOT_ECT][IP_ECN_ECT_1  ] = IP_ECN_NOT_ECT,
    [IP_ECN_ECT_0  ][IP_ECN_NOT_ECT] = IP_ECN_NOT_ECT,
    [IP_ECN_ECT_1  ][IP_ECN_NOT_ECT] = IP_ECN_NOT_ECT,

    // Last two cases - RFC 3168 doesn't specify, but we follow the
    // model of RFC 6040 and RFC 6660 which for tunnelling make ECT(1)
    // take priority, as it can be used as a mild congestion indication.
    [IP_ECN_ECT_0  ][IP_ECN_ECT_1  ] = IP_ECN_ECT_1,
    [IP_ECN_ECT_1  ][IP_ECN_ECT_0  ] = IP_ECN_ECT_1
};

/*
 * RFC 2460 notes:
 *
 *  fragment packets:
 *
 *  +------------------+--------+--------------+
 *  |  Unfragmentable  |Fragment|    first     |
 *  |       Part       | Header |   fragment   |
 *  +------------------+--------+--------------+
 *
 *  +------------------+--------+--------------+
 *  |  Unfragmentable  |Fragment|    second    |
 *  |       Part       | Header |   fragment   |
 *  +------------------+--------+--------------+
 *                        o
 *                        o
 *                        o
 *  +------------------+--------+----------+
 *  |  Unfragmentable  |Fragment|   last   |
 *  |       Part       | Header | fragment |
 *  +------------------+--------+----------+
 *
 *  reassembled original packet:
 *
 *  +------------------+----------------------//------------------------+
 *  |  Unfragmentable  |                 Fragmentable                   |
 *  |       Part       |                     Part                       |
 *  +------------------+----------------------//------------------------+
 *
 *    The following rules govern reassembly:
 *
 *     An original packet is reassembled only from fragment packets that
 *     have the same Source Address, Destination Address, and Fragment
 *     Identification.
 *
 *     The Unfragmentable Part of the reassembled packet consists of all
 *     headers up to, but not including, the Fragment header of the first
 *     fragment packet (that is, the packet whose Fragment Offset is
 *     zero), with the following two changes:
 *
 *        The Next Header field of the last header of the Unfragmentable
 *        Part is obtained from the Next Header field of the first
 *        fragment's Fragment header.
 *
 *        The Payload Length of the reassembled packet is computed from
 *        the length of the Unfragmentable Part and the length and offset
 *        of the last fragment.
 *
 *   Fragment Header
 *
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  Next Header  |   Reserved    |      Fragment Offset    |Res|M|
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Identification                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *  Fragment Offset      13-bit unsigned integer.  The offset, in 8-octet
 *                       units, of the data following this header,
 *                       relative to the start of the Fragmentable Part
 *                       of the original packet.
 *
 *  M flag               1 = more fragments; 0 = last fragment.
 */

/* On entry: frag_hdr -> fragment header
 *           nh_ptr -> Next Header octet in previous header
 *           payload_length = length of remaining data, including this header
 *           buffer data pointers describe entire IP fragment packet
 *           buffer src/dst filled in
 * Returns: Either reassembled packet (B_DIR_UP | B_TO_IPV6_FWD)
 *          or ICMP error response (B_DIR_DOWN | B_TO_ICMP)
 *          or NULL (fragment accepted, reassembly in progress)
 */
buffer_t *ipv6_frag_up(buffer_t *frag_buf, const uint8_t *frag_hdr, uint8_t *nh_ptr, uint16_t payload_length)
{
    if (payload_length <= 8) {
        return icmpv6_error(frag_buf, NULL, ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM, ICMPV6_CODE_PARAM_PRB_HDR_ERR, 4);
    }

    payload_length -= 8;

    uint8_t *ip_hdr = buffer_data_pointer(frag_buf);
    uint16_t unfrag_len = frag_hdr - ip_hdr;
    uint16_t fragment_first = common_read_16_bit(frag_hdr + 2) & 0xFFF8;
    uint16_t fragment_last = fragment_first + payload_length - 1;
    bool more = frag_hdr[3] & 1;

    /* All fragments apart from last must be multiples of 8 */
    if (more && (payload_length & 7)) {
        return icmpv6_error(frag_buf, NULL, ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM, ICMPV6_CODE_PARAM_PRB_HDR_ERR, 4);
    }

    /* Check we don't overflow 16-bit size */
    if (fragment_last < fragment_first) {
        return icmpv6_error(frag_buf, NULL, ICMPV6_TYPE_ERROR_PARAMETER_PROBLEM, ICMPV6_CODE_PARAM_PRB_HDR_ERR, frag_hdr + 2 - ip_hdr);
    }

    if (fragment_first == 0) {
        /* Replace "Next Header" byte in previous header */
        *nh_ptr = frag_hdr[0];

        if (!more) {
            /* Atomic fragment handling - strip out the fragment header.
             * See RFC 6946, which says that we require a special case for atomic
             * fragments:
             *
             *   A host that receives an IPv6 packet that includes a Fragment
             *   Header with the "Fragment Offset" equal to 0 and the "M" flag
             *   equal to 0 MUST process that packet in isolation from any other
             *   packets/fragments, even if such packets/fragments contain the same
             *   set {IPv6 Source Address, IPv6 Destination Address, Fragment
             *   Identification}.
             *
             * (Conceivably, we could just skip the header and keep parsing,
             * but this keeps it consistent with real fragments).
             */

            /* Move unfragmentable part up, eliminating fragment header */
            memmove(ip_hdr + 8, ip_hdr, unfrag_len);
            ip_hdr = buffer_data_strip_header(frag_buf, 8);

            /* Reduce Payload Length in IP header */
            uint16_t len = common_read_16_bit(ip_hdr + 4);
            common_write_16_bit(len - 8, ip_hdr + 4);

            frag_buf->offset = unfrag_len;
            frag_buf->options.ip_extflags |= IPEXT_FRAGMENT;
            frag_buf->info = (buffer_info_t)(B_DIR_UP | B_TO_IPV6_FWD | B_FROM_IPV6_FWD);
            return frag_buf;
        }
    }

    /* Adjust buffer pointer to point to fragment data. ip_ptr remains
     * pointing at IP header, which we need for first fragment. */
    buffer_data_pointer_set(frag_buf, frag_hdr + 8);

    /* Locate or create datagram assembly buffer */
    uint32_t id = common_read_32_bit(frag_hdr + 4);
    ip_fragmented_datagram_t *dgram = ip_frag_dgram_lookup(frag_buf, id, unfrag_len);
    if (!dgram || dgram->discard) {
        protocol_stats_update(STATS_IP_RX_DROP, 1);
        return buffer_free(frag_buf);
    }

    buffer_t *dgram_buf = dgram->buf;

    /* Length checks. For predictability, best to ensure we always try to
     * respect IPV6_MRU as a hard limit, which means a bit of care. */
    uint16_t limit;
    if (dgram_buf->buf_ptr == dgram->fragmentable) {
        /* Haven't yet got final header size - good enough to do rough check;
         * we have enough buffer to fit MRU - min IP header size */
        limit = ipv6_frag_mru - 40;
    } else {
        /* We do know final header size, so can do precise MRU check */
        limit = ipv6_frag_mru - (dgram->fragmentable - dgram_buf->buf_ptr);
    }
    /* Make sure we have room for following data, and hence a hole descriptor */
    if (more) {
        limit -= 8;
    }

    if (fragment_last >= limit) {
        /* Fragment would make datagram exceed MRU */
        tr_warn("Datagram size %u too big", fragment_last + 1);
fail:
        invalidate_datagram(dgram);
        protocol_stats_update(STATS_IP_RX_DROP, 1);
        return buffer_free(frag_buf);
    }

    /* Hole-filling algorithm, basically as per RFC815, but with added
     * checks for overlap (RFC 5722). We keep the hole list sorted to aid this,
     * (and Time Exceeded messages) - something RFC 815 doesn't strictly require.
     */
    uint16_t hole_off = dgram->first_hole;
    uint16_t *prev_ptr = &dgram->first_hole;
    bool okay = false;
    do {
        hole_t *hole = hole_pointer(dgram, hole_off);
        uint_fast16_t hole_first = hole->first;
        uint_fast16_t hole_last = hole->last;

        /* Fragment is beyond this hole - move to next (RFC 815 step 2) */
        if (fragment_first > hole_last) {
            prev_ptr = &hole->next;
            hole_off = hole->next;
            continue;
        }

        /* RFC 815 step 3 would have us check for fragment_last < hole_first,
         * and skipping, but we don't need/want to do that - it's covered by
         * the next check.
         */

        /* Unlike RFC 815, we now check for any overlap (RFC 5722) */
        if (fragment_first < hole_first || fragment_last > hole_last) {
            break;
        }

        /* Unhook this hole from the hole list (RFC 815 step 4) */
        delete_hole(dgram, hole_off, prev_ptr);
        hole = NULL;

        /* Create a new hole in front if necessary (RFC 815 step 5) */
        if (fragment_first > hole_first) {
            prev_ptr = &create_hole(dgram, hole_first, fragment_first - 1, prev_ptr)->next;
        }

        if (more) {
            /* Create a following hole if necessary (RFC 815 step 6) */
            if (fragment_last < hole_last) {
                create_hole(dgram, fragment_last + 1, hole_last, prev_ptr);
            }
        } else {
            /* If we already have some later data, it's broken. */
            if (hole_last != 0xffff) {
                break;
            }
            dgram->had_last = true;
        }

        /* Update end of buffer, if this is the last-placed fragment so far */
        if (hole_last == 0xffff) {
            dgram_buf->buf_end = dgram->fragmentable + fragment_last + 1;
        }

        /* Unlike RFC 815, we're now done. We don't allow overlaps, so we finish
         * as soon as we identify one hole that it entirely or partially fills */
        okay = true;
        break;
    } while (hole_off != 0xffff);

    /* If /any/ reassembly problems - overlaps etc - abandon the datagram */
    if (!okay) {
        tr_warn("Reassembly error");
        goto fail;
    }

    /* Hole list updated, can now copy in the fragment data */
    memcpy(dgram_buf->buf + dgram->fragmentable + fragment_first, buffer_data_pointer(frag_buf), fragment_last + 1 - fragment_first);

    /* Combine the "improper security" flags, so reassembled buffer's flag is set if any fragment wasn't secure */
    /* XXX should have some sort of overall "merge buffer metadata" routine handling this and whatever else */
    dgram_buf->options.ll_security_bypass_rx |= frag_buf->options.ll_security_bypass_rx;

    /* Combine the ECN field */
    dgram->ecn = frag_ecn_combination[dgram->ecn][frag_buf->options.traffic_class & IP_TCLASS_ECN_MASK];
    if (dgram->ecn == IP_ECN__ILLEGAL) {
        tr_warn("Illegal ECN");
        goto fail;
    }

    /* Overlap checks above ensure first-packet processing only happens once */
    if (fragment_first == 0) {
        /* Now know final header size, so repeat MRU check */
        uint16_t frag_so_far = dgram_buf->buf_end - dgram->fragmentable;
        if (!dgram->had_last) {
            /* This fudge factor represents our expectation of more data, and
             * also makes sure we memmove the trailing hole descriptor. */
            frag_so_far += 8;
        }
        if (unfrag_len + frag_so_far > ipv6_frag_mru) {
            tr_warn("Datagram size %u too big", unfrag_len + frag_so_far);
            goto fail;
        }

        if (dgram_buf->buf_ptr < unfrag_len) {
            /* Didn't reserve enough space for header. Shuffle data up into what will be final position */
            /* We know we have buffer room, thanks to previous checks against IPV6_MRU */
            uint16_t new_frag_offset = dgram_buf->size - ipv6_frag_mru + unfrag_len;
            memmove(dgram_buf->buf + new_frag_offset, dgram_buf->buf + dgram->fragmentable, frag_so_far);
            dgram->buf->buf_ptr = dgram->fragmentable = new_frag_offset;
        }

        /* Move the start pointer, and copy the header */
        memcpy(buffer_data_reserve_header(dgram_buf, unfrag_len), ip_hdr, unfrag_len);

        /* Clone the buffer header from this first fragment, preserving only size + pointers */
        /* Also the security flag, already merged above */
        bool buf_security = dgram_buf->options.ll_security_bypass_rx;
        buffer_copy_metadata(dgram_buf, frag_buf, true);
        dgram_buf->options.ll_security_bypass_rx = buf_security;
        /* Mark position of fragment header - allows skipping previous headers */
        dgram_buf->offset = unfrag_len;
        dgram_buf->options.ip_extflags |= IPEXT_FRAGMENT;
    }

    /* Free the original fragment buffer - we've extracted its juice */
    buffer_free(frag_buf);

    /* Thanks to David Clark, completion check is now simple */
    if (dgram->first_hole != 0xffff) {
        /* Not yet complete - processing finished on this fragment */
        return NULL;
    }

    /* First 8 bytes of the IP header, currently from the first fragment,
     * that we need to patch:
     * .               .               .               .               .
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |Version|   DSCP    |ECN|           Flow Label                  |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     * |         Payload Length        |  Next Header  |   Hop Limit   |
     * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
     */

    /* Fill in the combined ECN - 2 bits in the middle of the second byte */
    buffer_data_pointer(dgram_buf)[1] &= ~(3 << 4);
    buffer_data_pointer(dgram_buf)[1] |= (dgram->ecn << 4);

    /* Fill in final IP header length */
    common_write_16_bit(buffer_data_length(dgram_buf) - 40, buffer_data_pointer(dgram_buf) + 4);

    /* We've completed the datagram. Free the assembly structures (but not the buffer!) */
    dgram->buf = NULL;
    free_datagram(dgram);

    /* Send on the completed datagram */
    dgram_buf->info = (buffer_info_t)(B_DIR_UP | B_TO_IPV6_FWD | B_FROM_IPV6_FWD);
    return dgram_buf;
}
#endif /* NO_IP_FRAGMENT_RX */


/*                         FRAGMENT CREATION
 *
 * Allow fragment TX to be disabled for constrained systems.
 * This would violate RFC 6434, which says all IPv6 nodes must be able to
 * generate fragment headers. (Even if our only link has the minimum 1280-byte
 * MTU, we may still need to insert a fragment header).
 */
#ifndef NO_IP_FRAGMENT_TX
buffer_t *ipv6_frag_down(buffer_t *dgram_buf)
{
    uint8_t *ip_ptr = buffer_data_pointer(dgram_buf);
    uint16_t pmtu = ipv6_mtu(dgram_buf);
    uint8_t *frag_hdr;
    buffer_list_t frags_list = NS_LIST_INIT(frags_list);
    ipv6_destination_t *dest = ipv6_destination_lookup_or_create(dgram_buf->dst_sa.address, dgram_buf->interface->id);
    if (!dest) {
        return buffer_free(dgram_buf);
    }

    /* Skip over HbH and Routing headers to reach fragmentable part. Assume
     * packet well-formed (we created it...).
     */
    uint8_t *nh_ptr = &ip_ptr[6];
    uint8_t nh = *nh_ptr;
    uint8_t *fragmentable = ip_ptr + 40;
    while (nh == IPV6_NH_HOP_BY_HOP || nh == IPV6_NH_ROUTING) {
        nh_ptr = &fragmentable[0];
        nh = *nh_ptr;
        fragmentable += (fragmentable[1] + 1) * 8;
    }
    uint16_t unfrag_len = fragmentable - ip_ptr;
    uint16_t fragmentable_len = buffer_data_end(dgram_buf) - fragmentable;

    *nh_ptr = IPV6_NH_FRAGMENT;

    /* Special case for atomic fragments (caused by a small PMTU) */
    /* Note that we DO have the option of actually fragmenting and obeying
     * a small PMTU, which would avoid this special case.
     */
    if (buffer_data_length(dgram_buf) <= IPV6_MIN_LINK_MTU - 8) {
        dgram_buf = buffer_headroom(dgram_buf, 8);
        if (!dgram_buf) {
            return NULL;
        }

        /* Move unfragmentable section back 8 bytes; increase IP length field */
        ip_ptr = buffer_data_reserve_header(dgram_buf, 8);
        memmove(ip_ptr, ip_ptr + 8, unfrag_len);
        common_write_16_bit(common_read_16_bit(ip_ptr + 4) + 8, ip_ptr + 4);

        /* Write atomic fragment header into the gap */
        frag_hdr = ip_ptr + unfrag_len;
        frag_hdr[0] = nh;
        frag_hdr[1] = 0;
        common_write_16_bit(0, frag_hdr + 2);
        common_write_32_bit(++dest->fragment_id, frag_hdr + 4);
        return dgram_buf;
    }

    /* We won't fragment below minimum MTU. (Although we could...) */
    if (pmtu < IPV6_MIN_LINK_MTU) {
        pmtu = IPV6_MIN_LINK_MTU;
    }

    /* Check for silly situation - can't fit any fragment data (8 for fragment
     * header, 8 for minimum fragment payload) */
    if (unfrag_len + 8 + 8 > pmtu) {
        goto failed;
    }

    ++dest->fragment_id;

    /* RFC 7112 requires the entire header chain to be in the first fragment. */
    /* We don't explicitly check for this, but it would be spectacularly unlikely. */
    /* I think it would require a super-sized routing header */

    /* This is much simpler (more simplistic?) than the 6LoWPAN fragmentation,
     * which relies on co-operation with lower layers to ensure it works one
     * fragment at a time. We make all the fragments in one go, meaning higher
     * overhead, but IP fragmentation should be pretty rare - we don't need
     * to optimise this.
     */
    for (uint16_t frag_offset = 0; fragmentable_len;) {
        /* How much going in this packet? */
        uint16_t frag_len = (pmtu - unfrag_len - 8);
        if (fragmentable_len > frag_len) {
            frag_len &= ~7;
        } else {
            frag_len = fragmentable_len;
        }

        buffer_t *frag_buf = buffer_get(unfrag_len + 8 + frag_len);
        if (!frag_buf) {
            goto failed;
        }

        /* Clone the buffer header, apart from size+ptr */
        buffer_copy_metadata(frag_buf, dgram_buf, false);

        /* We splat the socket, so no upper-layer callbacks from the fragments */
        buffer_socket_set(frag_buf, NULL);

        /* Construct the new packet contents */
        buffer_data_length_set(frag_buf, unfrag_len + 8 + frag_len);
        uint8_t *ptr = buffer_data_pointer(frag_buf);
        /* Unfragmentable part */
        memcpy(ptr, ip_ptr, unfrag_len);
        /* Adjust length in IP header */
        common_write_16_bit(unfrag_len - 40 + 8 + frag_len, ptr + 4);
        /* Fragment header */
        frag_hdr = ptr + unfrag_len;
        frag_hdr[0] = nh;
        frag_hdr[1] = 0;
        common_write_16_bit(frag_offset | (frag_len != fragmentable_len), frag_hdr + 2);
        common_write_32_bit(dest->fragment_id, frag_hdr + 4);
        /* Fragment data */
        memcpy(frag_hdr + 8, fragmentable + frag_offset, frag_len);
        fragmentable_len -= frag_len;
        frag_offset += frag_len;

        /* Add to our fragment list */
        ns_list_add_to_start(&frags_list, frag_buf);
    }

    /* Now have a list of fragment buffers - report "success" to the socket */
    /* (TCP may save the dgram payload here? It strips off headers, so okay...) */
    socket_tx_buffer_event_and_free(dgram_buf, SOCKET_TX_DONE);

    /* Push the fragments. Backwards, as it happens, but who cares? */
    ns_list_foreach_safe(buffer_t, f, &frags_list) {
        ns_list_remove(&frags_list, f);
        protocol_push(f);
    }

    return NULL;

failed:
    /* Failed to allocate a buffer - no point sending any fragments if we
     * can't send all.
     */
    ns_list_foreach_safe(buffer_t, f, &frags_list) {
        ns_list_remove(&frags_list, f);
        buffer_free(f);
    }

    socket_tx_buffer_event_and_free(dgram_buf, SOCKET_NO_RAM);
    return NULL;
}
#endif /* NO_IP_FRAGMENT_TX */
