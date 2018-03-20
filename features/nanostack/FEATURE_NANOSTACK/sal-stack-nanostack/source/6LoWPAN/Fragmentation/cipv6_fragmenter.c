/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
 *
 * \file cipv6_fragmenter.c
 * \brief Packet Fragmentation and Reassembly.
 *
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "Core/include/socket.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "nsdynmemLIB.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "6frg"

typedef struct {
    uint16_t ttl;   /*!< Reassembly timer (seconds) */
    uint16_t tag;   /*!< Fragmentation datagram TAG ID */
    uint16_t size;  /*!< Datagram Total Size (uncompressed) */
    uint16_t orig_size; /*!< Datagram Original Size (compressed) */
    uint16_t frag_max;  /*!< Maximum fragment size (MAC payload) */
    uint16_t offset; /*!< Data offset from datagram start */
    int16_t pattern; /*!< Size of compressed LoWPAN headers */
    buffer_t *buf;
    ns_list_link_t      link; /*!< List link entry */
} reassembly_entry_t;

typedef NS_LIST_HEAD(reassembly_entry_t, link) reassembly_list_t;

typedef struct {
    int8_t interface_id;
    uint16_t timeout;
    reassembly_list_t rx_list;
    reassembly_list_t free_list;
    reassembly_entry_t *entry_pointer_buffer;
    ns_list_link_t      link; /*!< List link entry */
} reassembly_interface_t;

static NS_LIST_DEFINE(reassembly_interface_list, reassembly_interface_t, link);


/* Reassembly structures and helpers - basically the same as in
 * ipv6_fragmentation.c, as we are also using a variation of RFC 815, but there
 * are enough minor differences that it doesn't seem worth trying to share code.
 */

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
static hole_t *hole_pointer(const buffer_t *buf, uint16_t offset)
{
    uintptr_t ptr = (uintptr_t)(buffer_data_pointer(buf) + offset);

    return (hole_t *)((ptr + 1) & ~(uintptr_t) 1);
}

static void delete_hole(buffer_t *buf, uint16_t hole, uint16_t *prev_ptr)
{
    hole_t *hole_ptr = hole_pointer(buf, hole);

    *prev_ptr = hole_ptr->next;
}

static hole_t *create_hole(buffer_t *buf, uint16_t first, uint16_t last, uint16_t *prev_ptr)
{
    hole_t *hole_ptr = hole_pointer(buf, first);
    hole_ptr->first = first;
    hole_ptr->last = last;
    hole_ptr->next = *prev_ptr;

    *prev_ptr = first;
    return hole_ptr;
}

/*
 * RFC 4944 is oddly designed - it has blurred the header compression
 * and fragmentation layers. The datagram_size and datagram_offset field are
 * specified in terms of the uncompressed IPv6 datagram, not the actual 6LoWPAN
 * payload.
 *
 * This complicates reassembly if you don't decompress first; we don't because
 * the original upper layer works on IPHC headers directly, rather than native
 * IPv6.
 *
 * To handle the general case, including arbitrary fragment order so we don't
 * always know the 6LoWPAN size of the first fragment, the reassembly buffer
 * always leaves space for the uncompressed IPv6 header, and 1 byte more for a
 * 6LoWPAN "uncompressed IPv6" dispatch byte. This means non-first fragments
 * are always placed at buffer_data_pointer() + datagram_offset.
 *
 * This routine doesn't explicitly distinguish the compressed and uncompressed
 * cases - the difference arises purely from the output of "iphc_header_scan",
 * which sets "pattern" to the difference between IPv6 and 6LoWPAN size -
 * but to aid understanding, here's what it ends up doing in the two cases:
 *
 * IPHC compressed case
 * --------------------
 *
 *   -4         0                0x50
 *    +---------+--------+--------+       0x50 bytes of 6LoWPAN data
 *    |  FRAG1  |  IPHC  |  data1 |       0x70 bytes of uncompressed IPv6 data
 *    +---------+--------+--------+       "pattern" = 0x70 - 0x50 = 0x20
 *
 *   -5          0            0x60        (datagram_size = 0xD0)
 *    +----------+-------------+
 *    |FRAGN 0x70|    data2    |
 *    +----------+-------------+
 *
 * During assembly, the data pointer points at the "0" position representing the
 * virtual start of the IPv6 packet:
 *
 *   -1 0        0x20             0x70           0xD0
 *    +-+---------+--------+--------+-------------+
 *    | | padding |  IPHC  |  data1 |    data2    |
 *    +-+---------+--------+--------+-------------+
 *
 * On completion of assembly, the start pointer moves forward to point at the
 * IPHC header. (This means buffer size is slightly inefficient for an IPHC
 * upper layer, but it does reserve headroom for decompression to native IPv6.)
 *
 *  -0x21         0               0x50           0xB0
 *    +-----------+--------+--------+-------------+
 *    | headroom  |  IPHC  |  data1 |    data2    |
 *    +-----------+--------+--------+-------------+

 * Uncompressed case
 * -----------------

 *   -4         0 1             0x71  D = "Uncompressed IPv6" dispatch type 0x41
 *    +---------+-+--------------+    0x71 bytes of 6LoWPAN data
 *    |  FRAG1  |D|    data1     |    0x70 bytes of uncompressed IPv6 data
 *    +---------+-+--------------+    "pattern" = 0x70 - 0x71 = -1
 *
 *    -5         0              0x60        (datagram_size = 0xD0)
 *    +----------+---------------+
 *    |FRAGN 0x70|     data2     |
 *    +----------+---------------+
 *
 * During assembly, the data pointer points at the "0" position representing the
 * start of the IPv6 packet:
 *
 *   -1 0             0x70            0xD0
 *    +-+--------------+---------------+
 *    |D|    data1     |     data2     |
 *    +-+--------------+---------------+
 *
 * On completion of assembly, the start pointer moves back to point to the
 * 6LoWPAN dispatch byte:
 *
 *    0 1             0x71            0xD1
 *    +-+--------------+---------------+
 *    |D|    data1     |     data2     |
 *    +-+--------------+---------------+

 * (And if it's neither of these cases, a "native" 6LoWPAN reassembly happens
 * with "pattern" set to 0 - what probably should have happened in the first
 * place; offsets are treated as 6LoWPAN offsets from the start of the
 * fragmented 6LoWPAN data).
 */

//Discover
static reassembly_interface_t *reassembly_interface_discover(int8_t interfaceId)
{

    ns_list_foreach(reassembly_interface_t, interface_ptr, &reassembly_interface_list) {
        if (interfaceId == interface_ptr->interface_id) {
            return interface_ptr;
        }
    }

    return NULL;
}

static void reassembly_entry_free(reassembly_interface_t *interface_ptr, reassembly_entry_t *entry)
{
    ns_list_remove(&interface_ptr->rx_list, entry);
    ns_list_add_to_start(&interface_ptr->free_list, entry);
    if (entry->buf) {
        entry->buf = buffer_free(entry->buf);
    }
}

static void reassembly_list_free(reassembly_interface_t *interface_ptr )
{
    ns_list_foreach_safe(reassembly_entry_t, reassembly_entry, &interface_ptr->rx_list) {
        reassembly_entry_free(interface_ptr, reassembly_entry);
    }
}


static reassembly_entry_t *reassembly_already_action(reassembly_list_t *reassembly_list, buffer_t *buf, uint16_t tag, uint16_t size)
{
    ns_list_foreach(reassembly_entry_t, reassembly_entry, reassembly_list) {
        if ((reassembly_entry->tag == tag) && (reassembly_entry->size == size) &&
                reassembly_entry->buf->src_sa.addr_type == buf->src_sa.addr_type &&
                reassembly_entry->buf->dst_sa.addr_type == buf->dst_sa.addr_type) {
            /* Type will be either long or short 802.15.4 - we skip the PAN ID */
            if (memcmp(reassembly_entry->buf->src_sa.address + 2, buf->src_sa.address + 2, addr_len_from_type(buf->src_sa.addr_type) - 2) == 0 &&
                    memcmp(reassembly_entry->buf->dst_sa.address + 2, buf->dst_sa.address + 2, addr_len_from_type(buf->dst_sa.addr_type) - 2) == 0) {
                return reassembly_entry;
            }
        }
    }

    return NULL;

}

static reassembly_entry_t *lowpan_adaptation_reassembly_get(reassembly_interface_t *interface_ptr)
{
    reassembly_entry_t *entry = ns_list_get_first(&interface_ptr->free_list);
    if (!entry) {
        return NULL;
    }

    ns_list_remove(&interface_ptr->free_list, entry);
    memset(entry, 0, sizeof(reassembly_entry_t));
    //Add to first
    ns_list_add_to_start(&interface_ptr->rx_list, entry);

    return entry;
}


buffer_t *cipv6_frag_reassembly(int8_t interface_id, buffer_t *buf)
{
    reassembly_interface_t *interface_ptr = reassembly_interface_discover(interface_id);
    if (!interface_ptr) {
        return buffer_free(buf);
    }

    uint16_t datagram_size, datagram_tag;
    uint16_t fragment_first;
    uint8_t frag_header;

    uint8_t *ptr = buffer_data_pointer(buf);

    frag_header = ptr[0];
    datagram_size = common_read_16_bit(ptr) & 0x07FF;

    if (datagram_size == 0) {
        goto resassembly_error;
    }

    ptr += 2;
    datagram_tag = common_read_16_bit(ptr);
    ptr += 2;
    if (frag_header & LOWPAN_FRAGN_BIT) {
        fragment_first = *ptr++ << 3;
    } else {
        fragment_first = 0;
    }

    /* Consume the fragment header. We don't distinguish FRAG1/FRAGN after this
     * point (we treat FRAGN with offset 0 the same as FRAG1)
     */
    buffer_data_pointer_set(buf, ptr);
    reassembly_entry_t *frag_ptr = reassembly_already_action(&interface_ptr->rx_list, buf, datagram_tag, datagram_size);

    if (!frag_ptr) {

        frag_ptr = lowpan_adaptation_reassembly_get(interface_ptr);
        if (!frag_ptr) {
            goto resassembly_error;
        }

        buffer_t *reassembly_buffer = buffer_get(1 + ((datagram_size+7) & ~7));
        if (!reassembly_buffer) {
            //Put allocated back to free
            reassembly_entry_free(interface_ptr, frag_ptr);
            goto resassembly_error;
        }

        // Allocate the reassembly buffer.
        // Allow 1 byte extra for an "Uncompressed IPv6" dispatch byte - the
        // 6LoWPAN data can be 1 byte longer than the IPv6 data.
        // Also, round datagram size up to a multiple of 8 to ensure we have
        // room for a final hole descriptor (it can spill past the indicated
        // datagram size if the last fragment is smaller than 8 bytes).

        reassembly_buffer->src_sa = buf->src_sa;
        reassembly_buffer->dst_sa = buf->dst_sa;
        frag_ptr->ttl = interface_ptr->timeout;
        frag_ptr->tag = datagram_tag;
        frag_ptr->size = datagram_size;
        // Set buffer length and adjust start pointer, so it represents the
        // uncompressed IPv6 packet. (See comment block before this function).
        buffer_data_length_set(reassembly_buffer, 1 + datagram_size);
        buffer_data_strip_header(reassembly_buffer, 1);
        // Write initial hole descriptor into buffer
        frag_ptr->offset = 0xffff;
        create_hole(reassembly_buffer, 0, datagram_size - 1, &frag_ptr->offset);
        frag_ptr->buf = reassembly_buffer;
    }

    /* For the first link fragment, work out and remember the "pattern"
     * (difference between6LoWPAN and IPv6 size), and also copy the buffer
     * header metadata.
     */
    uint16_t lowpan_size, ipv6_size;
    if (fragment_first == 0) {
        uint16_t uncompressed_header_size;
        uint8_t compressed_header_size;
        compressed_header_size = iphc_header_scan(buf, &uncompressed_header_size);
        lowpan_size = buffer_data_length(buf);
        ipv6_size = lowpan_size - compressed_header_size + uncompressed_header_size;
        frag_ptr->pattern = ipv6_size - lowpan_size;

        /* Clone the buffer header from this first fragment, preserving only size + pointers */
        /* Also the security flag - this fragment's flag is merged in later */
        bool buf_security = frag_ptr->buf->options.ll_security_bypass_rx;
        buffer_copy_metadata(frag_ptr->buf, buf, true);
        frag_ptr->buf->options.ll_security_bypass_rx = buf_security;
    } else {
        ipv6_size = lowpan_size = buffer_data_length(buf);
    }

    uint16_t fragment_last = fragment_first + ipv6_size - 1;
    if (fragment_last >= datagram_size) {
        tr_err("Frag out-of-range: last=%u, size=%u", fragment_last, datagram_size);
        //Free Current entry
        reassembly_entry_free(interface_ptr, frag_ptr);
        goto resassembly_error;
    }

    /* Hole-filling algorithm, basically as per RFC 815, but with added
     * checks for overlap. The hole list is kept sorted, as per
     * ipv6_fragmentation.c, but that's not relevant in this version.
     */
    uint16_t hole_off = frag_ptr->offset;
    uint16_t *prev_ptr = &frag_ptr->offset;
    do {
        hole_t *hole = hole_pointer(frag_ptr->buf, hole_off);
        uint_fast16_t hole_first = hole->first;
        uint_fast16_t hole_last = hole->last;

        /* Fragment is beyond this hole - move to next (RFC 815 step 2) */
        /* Fragment is before this hole - move to next (RFC 815 step 3) */
        if (fragment_first > hole_last || fragment_last < hole_first) {
            prev_ptr = &hole->next;
            hole_off = hole->next;
            continue;
        }

        /* If any of the fragment lies outside the hole, it indicates a problem;
         * we only expect repeat data from retransmission, so fragments should
         * always lie entirely within a hole or existing data, not straddle
         * them. If we see this happen then junk existing data, making this the
         * first fragment of a new reassembly (RFC 4944).
         */
        if (fragment_first < hole_first || fragment_last > hole_last) {
            tr_err("Frag overlap: hole %"PRIuFAST16"-%"PRIuFAST16", frag %"PRIu16"-%"PRIu16, hole_first, hole_last, fragment_first, fragment_last);
            protocol_stats_update(STATS_FRAG_RX_ERROR, 1);
            /* Forget previous data by marking as "all hole" */
            frag_ptr->offset = 0xffff;
            create_hole(frag_ptr->buf, hole_off = hole_first = 0, hole_last = datagram_size - 1, prev_ptr = &frag_ptr->offset);
        }

        /* Unhook this hole from the hole list (RFC 815 step 4) */
        delete_hole(frag_ptr->buf, hole_off, prev_ptr);

        /* Create a new hole in front if necessary (RFC 815 step 5) */
        if (fragment_first > hole_first) {
            prev_ptr = &create_hole(frag_ptr->buf, hole_first, fragment_first - 1, prev_ptr)->next;
        }

        /* Create a following hole if necessary (RFC 815 step 6) */
        if (fragment_last < hole_last) {
            create_hole(frag_ptr->buf, fragment_last + 1, hole_last, prev_ptr);
        }

        /* Unlike RFC 815, we're now done. We don't allow overlaps, so we finish
         * as soon as we identify one hole that it entirely or partially fills */
        break;
    } while (hole_off != 0xffff);

    /* Hole list updated, can now copy in the fragment data -  to make sure the
     * initial fragment goes in the right place we use the end offset, rather
     * than the start offset. */
    memcpy(buffer_data_pointer(frag_ptr->buf) + fragment_last + 1 - lowpan_size, buffer_data_pointer(buf), lowpan_size);

    /* Combine the "improper security" flags, so reassembled buffer's flag is set if any fragment wasn't secure */
    /* XXX should have some sort of overall "merge buffer metadata" routine handling this and whatever else */
    frag_ptr->buf->options.ll_security_bypass_rx |= buf->options.ll_security_bypass_rx;

    /* We've finished with the original fragment buffer */
    buf = buffer_free(buf);

    /* Completion check - any holes left? */
    if (frag_ptr->offset != 0xffff) {
        /* Not yet complete - processing finished on this fragment */
        return NULL;
    }

    /* No more holes, so our reassembly is complete */
    buf = frag_ptr->buf;
    frag_ptr->buf = NULL;
    reassembly_entry_free(interface_ptr, frag_ptr);

    /* Buffer start pointer is currently at the "start of uncompressed IPv6
     * packet" position. Move it either forwards or backwards to match
     * the IPHC data (could be compressed, or uncompressed with added dispatch
     * byte).
     */
    buf->buf_ptr += frag_ptr->pattern;
    buf->info = (buffer_info_t)(B_DIR_UP | B_FROM_FRAGMENTATION | B_TO_IPV6_TXRX);
    return buf;

resassembly_error:
    protocol_stats_update(STATS_FRAG_RX_ERROR, 1);
    return buffer_free(buf);
}

static void reassembly_entry_timer_update(reassembly_interface_t *interface_ptr, uint16_t seconds)
{
    ns_list_foreach_safe(reassembly_entry_t, reassembly_entry, &interface_ptr->rx_list) {
        if (reassembly_entry->ttl > seconds) {
            reassembly_entry->ttl -= seconds;
        } else {
            protocol_stats_update(STATS_FRAG_RX_ERROR, 1);
            tr_debug("Reassembly TO: src %s size %u",
                     trace_sockaddr(&reassembly_entry->buf->src_sa, true),
                     reassembly_entry->size);
            reassembly_entry_free(interface_ptr, reassembly_entry);
        }
    }
}

void cipv6_frag_timer(uint16_t seconds)
{
    ns_list_foreach(reassembly_interface_t, interface_ptr, &reassembly_interface_list) {
        reassembly_entry_timer_update(interface_ptr, seconds);
    }
}

int8_t reassembly_interface_free(int8_t interface_id)
{
    //Discover
    reassembly_interface_t *interface_ptr = reassembly_interface_discover(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    ns_list_remove(&reassembly_interface_list, interface_ptr);

    //Free Dynamic allocated entry buffer
    ns_dyn_mem_free(interface_ptr->entry_pointer_buffer);
    ns_dyn_mem_free(interface_ptr);

    return 0;
}


int8_t reassembly_interface_init(int8_t interface_id, uint8_t reassembly_session_limit, uint16_t reassembly_timeout)
{

    if (!reassembly_session_limit || !reassembly_timeout) {
        return -2;
    }

    //Remove old interface
    reassembly_interface_free(interface_id);

    //Allocate new
    reassembly_interface_t *interface_ptr = ns_dyn_mem_alloc(sizeof(reassembly_interface_t));
    reassembly_entry_t *reassemply_ptr = ns_dyn_mem_alloc(sizeof(reassembly_entry_t) * reassembly_session_limit);
    if (!interface_ptr || !reassemply_ptr) {
        ns_dyn_mem_free(interface_ptr);
        ns_dyn_mem_free(reassemply_ptr);
        return -1;
    }

    memset(interface_ptr, 0 ,sizeof(reassembly_interface_t));
    interface_ptr->interface_id = interface_id;
    interface_ptr->timeout = reassembly_timeout;
    interface_ptr->entry_pointer_buffer = reassemply_ptr;
    ns_list_init(&interface_ptr->free_list);
    ns_list_init(&interface_ptr->rx_list);

    for (uint8_t i=0; i<reassembly_session_limit ; i++) {
        ns_list_add_to_end(&interface_ptr->free_list, reassemply_ptr);
        reassemply_ptr++;
    }

    ns_list_add_to_end(&reassembly_interface_list, interface_ptr);

    return 0;
}

int8_t reassembly_interface_reset(int8_t interface_id)
{
    //Discover
    reassembly_interface_t *interface_ptr = reassembly_interface_discover(interface_id);
    if (!interface_ptr) {
        return -1;
    }

    //Free Reaasembled queue
    reassembly_list_free(interface_ptr);
    return 0;
}

