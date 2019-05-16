/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"

#define TRACE_GROUP "iphc"

typedef struct iphc_compress_state {
    const lowpan_context_list_t *const context_list;
    const uint8_t *in;
    uint8_t *out;
    uint16_t len;
    uint16_t out_space;
    uint16_t consumed;
    uint16_t produced;
    const uint8_t *outer_src_iid;
    const uint8_t *outer_dst_iid;
    const bool stable_only;
} iphc_compress_state_t;

static bool compress_nh(uint8_t nh, iphc_compress_state_t *restrict cs);

static inline bool context_ok_for_compression(const lowpan_context_t *ctx, bool stable_only)
{
    return ctx->lifetime && ctx->compression && (!stable_only || ctx->stable);
}

/* Using a specified context, what's the best possible compression of addr? */
static uint_fast8_t addr_bytes_needed(const uint8_t *addr, const uint8_t *outer_iid, const uint8_t *ctx_prefix, uint_fast8_t ctx_len)
{
    /* Quick test: context always gets priority, so all of context must match */
    /* This handles intrusions into IID space, so don't have to recheck this below */
    if (!bitsequal(addr, ctx_prefix, ctx_len)) {
        return 16;
    }

    /* Okay, potential match, but getting next bit optimal is more complex. */
    /* Try each of 3 possible cases in turn */
    uint8_t template[16];

    /* 0-bit case: 64 bits of IID from outer header */
    /* Template contains 0s, then outer IID, then context on top */
    if (ctx_len < 64) {
        memset(template, 0, 8);
    }
    if (ctx_len < 128) {
        memcpy(template + 8, outer_iid, 8);
    }
    bitcopy(template, ctx_prefix, ctx_len);

    if (addr_ipv6_equal(template, addr)) {
        return 0;
    }

    /* 16-bit case: need to match top 14 bytes. Respell top 6 bytes of IID */
    /* from last test to contain 0000:00ff:fe00, unless they're fully */
    /* covered by context, in which case template already hold the correct */
    /* context bytes */
    if (ctx_len < 112) {
        memcpy(template + 8, ADDR_SHORT_ADR_SUFFIC, 6);
        if (ctx_len > 64) {
            bitcopy(template + 8, ctx_prefix + 8, ctx_len - 64);
        }
    }
    if (memcmp(template, addr, 14) == 0) {
        return 2;
    }

    /* 64-bit case: only need to match top 8 bytes */
    if (memcmp(template, addr, 8) == 0) {
        return 8;
    }

    /* Had a matching context, but didn't match enough. Example:
     * Context = 2002:db4::/32, Addr = 2002:db4:1::1
     * Matches all 32 bits of context length, but not enough to compress 64-bit
     */
    return 16;
}

static uint8_t compress_mc_addr(const lowpan_context_list_t *context_list, const uint8_t *addr, uint8_t *cmp_addr_out, uint8_t *context, uint8_t *mode, bool stable_only)
{
    *mode |= HC_MULTICAST_COMP;
    if (memcmp(addr, ADDR_LINK_LOCAL_ALL_NODES, 15) == 0) {
        // ff02:0000:0000:0000:0000:0000:0000:00XX
        cmp_addr_out[0] = addr[15];
        *mode |= HC_8BIT_MULTICAST;
        return 1;
    }
    if (memcmp(addr + 2, ADDR_LINK_LOCAL_ALL_NODES + 2, 11) == 0) {
        // ffXX:0000:0000:0000:0000:0000:00XX:XXXX
        cmp_addr_out[0] = addr[1];
        cmp_addr_out[1] = addr[13];
        cmp_addr_out[2] = addr[14];
        cmp_addr_out[3] = addr[15];
        *mode |= HC_32BIT_MULTICAST;
        return 4;
    }
    if (memcmp(addr + 2, ADDR_LINK_LOCAL_ALL_NODES + 2, 9) == 0) {
        // ffXX:0000:0000:0000:0000:00XX:XXXX:XXXX
        cmp_addr_out[0] = addr[1];
        memcpy(cmp_addr_out + 1, addr + 11, 5);
        *mode |= HC_48BIT_MULTICAST;
        return 6;
    }
    /* Looking for addresses of the form ffxx:xxLL:PPPP:PPPP:PPPP:PPPP:xxxx:xxxx */
    /* Context info is padded with zeros, so the 8-byte memcmp is okay for short contexts. */
    /* RFCs are ambiguous about how this should work if prefix length is > 64 bits, */
    /* so don't attempt compression against long contexts. */
    ns_list_foreach(lowpan_context_t, ctx, context_list) {
        if (context_ok_for_compression(ctx, stable_only) &&
                ctx->length <= 64 &&
                addr[3] == ctx->length && memcmp(addr + 4, ctx->prefix, 8) == 0) {
            cmp_addr_out[0] = addr[1];
            cmp_addr_out[1] = addr[2];
            memcpy(cmp_addr_out + 2, addr + 12, 4);
            *context |= ctx->cid;
            *mode |= HC_48BIT_CONTEXT_MULTICAST;
            return 6;
        }
    }

    memcpy(cmp_addr_out, addr, 16);
    *mode |= HC_128BIT_MULTICAST;
    return 16;
}

static uint8_t compress_addr(const lowpan_context_list_t *context_list, const uint8_t *addr, bool is_dst, const uint8_t *outer_iid, uint8_t *cmp_addr_out, uint8_t *context, uint8_t *mode, bool stable_only)
{
    if (is_dst && addr[0] == 0xff) {
        return compress_mc_addr(context_list, addr, cmp_addr_out, context, mode, stable_only);
    }

    uint_fast8_t best_bytes = addr_bytes_needed(addr, outer_iid, ADDR_LINK_LOCAL_PREFIX, 64);
    lowpan_context_t *best_ctx = NULL;
    bool checked_ctx0 = false;

    if (best_bytes > 0) {
        ns_list_foreach(lowpan_context_t, ctx, context_list) {
            if (!context_ok_for_compression(ctx, stable_only)) {
                continue;
            }

            uint_fast8_t bytes = addr_bytes_needed(addr, outer_iid, ctx->prefix, ctx->length);
            if (ctx->cid == 0) {
                checked_ctx0 = true;
            }
            /* This context is better if:
             * a) it means fewer inline bytes, or
             * b) it needs the same inline bytes and might avoid a CID extension
             */
            if (bytes < best_bytes || (bytes == best_bytes && ctx->cid == 0 && best_ctx && best_ctx->cid != 0)) {
                best_ctx = ctx;
                best_bytes = bytes;
                /* Don't need to check further if we've reached 0 bytes, and we've considered context 0 */
                if (best_bytes == 0 && checked_ctx0) {
                    break;
                }
            }
        }
    }

    /* If not found a 0-byte match, one more (unlikely) possibility for source - special case for "unspecified" */
    if (best_bytes > 0 && !is_dst && addr_is_ipv6_unspecified(addr)) {
        *mode |= HC_SRCADR_COMP | HC_SRC_ADR_128_BIT;
        return 0;
    }

    uint8_t mode_bits;
    uint8_t context_bits;
    if (best_ctx) {
        context_bits = best_ctx->cid;
        mode_bits = HC_DSTADR_COMP;
    } else {
        context_bits = 0;
        mode_bits = 0;
    }

    switch (best_bytes) {
        default:
            mode_bits |= HC_DST_ADR_128_BIT;
            break;
        case 8:
            mode_bits |= HC_DST_ADR_64_BIT;
            break;
        case 2:
            mode_bits |= HC_DST_ADR_16_BIT;
            break;
        case 0:
            mode_bits |= HC_DST_ADR_FROM_MAC;
            break;
    }

    /* Convert from IPHC DST bits to SRC bits */
    if (!is_dst) {
        mode_bits <<= 4;
        context_bits <<= 4;
    }

    *mode |= mode_bits;
    *context |= context_bits;

    memcpy(cmp_addr_out, addr + (16 - best_bytes), best_bytes);

    return best_bytes;
}

static bool compress_udp(iphc_compress_state_t *restrict cs)
{
    if (cs->len < 8) {
        return false;
    }

    /* UDP length should be set to payload length - if it's not, we can't compress it */
    uint16_t udp_len = common_read_16_bit(cs->in + 4);
    if (udp_len != cs->len) {
        return false;
    }

    uint16_t src_port = common_read_16_bit(cs->in + 0);
    uint16_t dst_port = common_read_16_bit(cs->in + 2);

    uint8_t *ptr = cs->out;
    if ((src_port & 0xfff0) == 0xf0b0 &&
            (dst_port & 0xfff0) == 0xf0b0) {
        if (cs->out_space < 4) {
            return false;
        }
        *ptr++ = NHC_UDP | NHC_UDP_PORT_COMPRESS_BOTH;
        *ptr++ = (src_port & 0xF) << 4 |
                 (dst_port & 0xF);
    } else if ((src_port & 0xff00) == 0xf000) {
        if (cs->out_space < 7) {
            return false;
        }
        *ptr++ = NHC_UDP | NHC_UDP_PORT_COMPRESS_SRC;
        *ptr++ = src_port & 0xff;
        ptr = common_write_16_bit(dst_port, ptr);
    } else if ((dst_port & 0xff00) == 0xf000) {
        if (cs->out_space < 7) {
            return false;
        }
        *ptr++ = NHC_UDP | NHC_UDP_PORT_COMPRESS_DST;
        ptr = common_write_16_bit(src_port, ptr);
        *ptr++ = dst_port & 0xff;
    } else {
        if (cs->out_space < 8) {
            return false;
        }
        *ptr++ = NHC_UDP | NHC_UDP_PORT_COMPRESS_NONE;
        ptr = common_write_16_bit(src_port, ptr);
        ptr = common_write_16_bit(dst_port, ptr);
    }

    /* Checksum */
    *ptr++ = cs->in[6];
    *ptr++ = cs->in[7];
    uint_fast8_t outlen = ptr - cs->out;

    cs->consumed += 8;
    cs->in += 8;
    cs->len -= 8;
    cs->out_space -= outlen;
    cs->out += outlen;
    cs->produced += outlen;

    return true;
}


static bool compress_exthdr(uint8_t nhc, iphc_compress_state_t *restrict cs)
{
    if (cs->len < 8) {
        return false;
    }

    const uint8_t *in = cs->in;
    uint8_t nh = in[0];
    uint16_t hdrlen;

    if (nhc == NHC_EXT_FRAG) {
        /* For fragment header, seems sensible to only handle first fragment;
         * "Next header" field doesn't describe following data otherwise.
         * Could still "compress" this header in all cases, but no point if we don't
         * have following headers to compress - it takes 8 bytes either way.
         */
        uint16_t offset = common_read_16_bit(in + 2) & 0xFFF8;
        if (offset != 0) {
            return false;
        }

        /* Second byte is reserved; it's not a length byte */
        hdrlen = 8;
    } else {
        hdrlen = (in[1] + 1) * 8;
    }

    if (hdrlen > cs->len || hdrlen >= 256) {
        return false;
    }

    /* If Options are well-formed, parse and remove final option if it's padding */
    uint8_t hc_bytes = hdrlen;
    if (nhc == NHC_EXT_DEST_OPT || nhc == NHC_EXT_HOP_BY_HOP) {
        const uint8_t *opt = in + 2, *lastopt = opt;
        uint8_t optrem = hdrlen - 2;

        while (optrem) {
            lastopt = opt;
            if (opt[0] == IPV6_OPTION_PAD1) {
                opt++;
                optrem--;
                continue;
            }
            if (optrem < 2 || optrem < 2 + opt[1]) {
                lastopt = NULL;
                break;
            }
            optrem -= 2 + opt[1];
            opt += 2 + opt[1];
        }
        if (lastopt && (lastopt[0] == IPV6_OPTION_PAD1 || lastopt[0] == IPV6_OPTION_PADN)) {
            hc_bytes = lastopt - in;
        }
    }

    /* Include one for "next header" - we will either need this ourselves, or
     * the next hdr will*/
    if (cs->out_space < hc_bytes + 1) {
        return false;
    }

    uint8_t *out = cs->out;

    cs->in += hdrlen;
    cs->len -= hdrlen;
    cs->consumed += hdrlen;
    cs->out += hc_bytes;
    cs->out_space -= hc_bytes;
    cs->produced += hc_bytes;

    out[0] = nhc;
    if (compress_nh(nh, cs)) {
        out[0] |= NHC_EXT_NH;
    }

    uint8_t *ptr = out + 1;
    if (!(out[0] & NHC_EXT_NH)) {
        *ptr++ = nh;
        cs->out++;
        cs->out_space--;
        cs->produced++;
    }
    if (nhc == NHC_EXT_FRAG) {
        *ptr++ = in[1];
    } else {
        *ptr++ = hc_bytes - 2;
    }
    memcpy(ptr, in + 2, hc_bytes - 2);
    return true;
}

static bool compress_ipv6(iphc_compress_state_t *restrict cs, bool from_nhc)
{
    const uint8_t *const in = cs->in;
    uint8_t *const out = cs->out;

    /* Need at least 2 plus 1 for next header field or following NHC byte */
    /* plus another 1 if coming from NHC (for NHC_EXT_IPV6 byte) */
    if (cs->out_space < from_nhc + 3) {
        return false;
    }

    if (cs->len < 40 || (in[0] & 0xF0) != 0x60) {
        tr_debug("Not IPv6");
        return false;
    }
    uint8_t iphc[2] = { LOWPAN_DISPATCH_IPHC, 0 };
    uint_fast8_t iphc_bytes = from_nhc + 2;

    /* Payload length field must match, or we can't compress */
    if (common_read_16_bit(in + 4) != cs->len - 40) {
        return false;
    }

    /* Compress addresses, get context byte */
    uint8_t cid = 0;
    uint8_t cmp_src[16], cmp_dst[16], cmp_src_len, cmp_dst_len;
    cmp_src_len = compress_addr(cs->context_list, in + 8, false, cs->outer_src_iid, cmp_src, &cid, &iphc[1], cs->stable_only);
    cmp_dst_len = compress_addr(cs->context_list, in + 24, true, cs->outer_dst_iid, cmp_dst, &cid, &iphc[1], cs->stable_only);
    iphc_bytes += cmp_src_len + cmp_dst_len;
    if (cid != 0) {
        iphc_bytes += 1;
        iphc[1] |= HC_CIDE_COMP;
    }

    /* Compress Hop Limit */
    switch (cs->in[7]) {
        case 255:
            iphc[0] |= HC_HOP_LIMIT_255;
            break;
        case 64:
            iphc[0] |= HC_HOP_LIMIT_64;
            break;
        case 1:
            iphc[0] |= HC_HOP_LIMIT_1;
            break;
        default:
            iphc[0] |= HC_HOP_LIMIT_CARRIED_IN_LINE;
            iphc_bytes += 1;
            break;
    }

    uint8_t ecn = (in[1] & 0x30) << 2;
    uint8_t dscp = (in[0] & 0x0F) << 2 | (in[1] & 0xC0) >> 6;
    uint_fast24_t flow = common_read_24_bit(in + 1) & 0xFFFFF;

    if (flow == 0 && ecn == 0 && dscp == 0) {
        iphc[0] |= HC_TF_ELIDED;
    } else if (flow == 0) {
        iphc[0] |= HC_TF_ECN_DSCP;
        iphc_bytes += 1;
    } else if (dscp == 0) {
        iphc[0] |= HC_TF_ECN_FLOW_LABEL;
        flow |= (uint_fast24_t) ecn << 16;
        iphc_bytes += 3;
    } else {
        iphc[0] |= HC_TF_ECN_DSCP_FLOW_LABEL;
        iphc_bytes += 4;
    }

    /* Include one for "next header" - we will either need this ourselves, or
     * the next H will*/
    if (cs->out_space < iphc_bytes + 1) {
        return false;
    }

    /* Update state for next header */
    cs->in += 40;
    cs->len -= 40;
    cs->consumed += 40;

    cs->out += iphc_bytes;
    cs->out_space -= iphc_bytes;
    cs->produced += iphc_bytes;

    cs->outer_src_iid = in + 16;
    cs->outer_dst_iid = in + 32;

    /* Check if next header can be compressed - potentially recursing */
    if (compress_nh(in[6], cs)) {
        iphc[0] |= HC_NEXT_HEADER_MASK;
    } else {
        /* We produce one more byte for Next Header */
        iphc_bytes++;
        cs->out++;
        cs->out_space--;
        cs->produced++;
    }

    uint8_t *ptr = out;
    if (from_nhc) {
        *ptr++ = NHC_EXT_IPV6;
    }
    *ptr++ = iphc[0];
    *ptr++ = iphc[1];
    if (cid != 0) {
        *ptr++ = cid;
    }

    if ((iphc[0] & HC_TF_MASK) == HC_TF_ECN_DSCP_FLOW_LABEL || (iphc[0] & HC_TF_MASK) == HC_TF_ECN_DSCP) {
        *ptr++ = (ecn | dscp);
    }
    if ((iphc[0] & HC_TF_MASK) == HC_TF_ECN_DSCP_FLOW_LABEL || (iphc[0] & HC_TF_MASK) == HC_TF_ECN_FLOW_LABEL) {
        ptr = common_write_24_bit(flow, ptr);    // "flow" includes ecn in 3-byte case
    }

    if (!(iphc[0] & HC_NEXT_HEADER_MASK)) {
        *ptr++ = in[6];
    }

    if ((iphc[0] & HC_HOP_LIMIT_MASK) == HC_HOP_LIMIT_CARRIED_IN_LINE) {
        *ptr++ = in[7];
    }

    ptr = (uint8_t *) memcpy(ptr, cmp_src, cmp_src_len) + cmp_src_len;
    ptr = (uint8_t *) memcpy(ptr, cmp_dst, cmp_dst_len) + cmp_dst_len;

    if (ptr != out + iphc_bytes) {
        tr_debug("iphc error");
    }

    return true;
}

static bool compress_nh(uint8_t nh, iphc_compress_state_t *restrict cs)
{
    switch (nh) {
            uint8_t nhc;
        case IPV6_NH_HOP_BY_HOP:
            nhc = NHC_EXT_HOP_BY_HOP;
            goto ext_hdr;
        case IPV6_NH_ROUTING:
            nhc = NHC_EXT_ROUTING;
            goto ext_hdr;
        case IPV6_NH_FRAGMENT:
            nhc = NHC_EXT_FRAG;
            goto ext_hdr;
        case IPV6_NH_DEST_OPT:
            nhc = NHC_EXT_DEST_OPT;
            goto ext_hdr;
        case IPV6_NH_MOBILITY:
            nhc = NHC_EXT_MOBILITY;
ext_hdr:
            return compress_exthdr(nhc, cs);
        /* no break */
        case IPV6_NH_IPV6:
            return compress_ipv6(cs, true);
        case IPV6_NH_UDP:
            return compress_udp(cs);
        default:
            return false;
    }
}


/* Input: An IPv6 frame, with outer layer 802.15.4 MAC (or IP) addresses in src+dst */
/* Output: 6LoWPAN frame - usually compressed. */
buffer_t *iphc_compress(const lowpan_context_list_t *context_list, buffer_t *buf, uint16_t hc_space, bool stable_only)
{
    uint8_t *ptr = buffer_data_pointer(buf);
    uint16_t len = buffer_data_length(buf);
    uint8_t src_iid[8], dst_iid[8];

    if (len < 40 || (ptr[0] & 0xF0) != 0x60) {
        tr_debug("Not IPv6");
        return buffer_free(buf);
    }

    /* No point allocating excessively */
    if (hc_space > len) {
        hc_space = len;
    }

    /* TODO: Could actually do it in-place with more care, working backwards
     * in each header.
     */
    uint8_t *hc_out = ns_dyn_mem_temporary_alloc(hc_space);
    if (!hc_out) {
        tr_debug("No mem");
        return buffer_free(buf);
    }

    if (!addr_iid_from_outer(src_iid, &buf->src_sa) || !addr_iid_from_outer(dst_iid, &buf->dst_sa)) {
        tr_debug("Bad outer addr");
        ns_dyn_mem_free(hc_out);
        return buffer_free(buf);
    }

    iphc_compress_state_t cs = {
        .context_list = context_list,
        .in = ptr,
        .len = len,
        .out = hc_out,
        .out_space = hc_space,
        .consumed = 0,
        .produced = 0,
        .outer_src_iid = src_iid,
        .outer_dst_iid = dst_iid,
        .stable_only = stable_only
    };

    if (!compress_ipv6(&cs, false) || cs.produced > cs.consumed) {
        /* Compression failed, or made it bigger somehow (impossible?) */
        /* Fall back to uncompressed, which means adding a dispatch byte */
        buf = buffer_headroom(buf, 1);
        if (buf) {
            ptr = buffer_data_reserve_header(buf, 1);
            *ptr = LOWPAN_DISPATCH_IPV6;
        }
        ns_dyn_mem_free(hc_out);
        return buf;
    }

    buffer_data_strip_header(buf, cs.consumed);
    buffer_data_reserve_header(buf, cs.produced);
    /* XXX see note above - should be able to improve this to avoid the temp buffer */
    memcpy(buffer_data_pointer(buf), hc_out, cs.produced);
    ns_dyn_mem_free(hc_out);
    return buf;

}
