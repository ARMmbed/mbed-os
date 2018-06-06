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
#ifndef _CIPV6_H
#define _CIPV6_H

/* MTU defined in RFC 4944 */
#ifndef LOWPAN_MTU
#define LOWPAN_MTU 1280
#endif

/* The max MTU we'll obey from a Router Advertisement */
#ifndef LOWPAN_MAX_MTU
#define LOWPAN_MAX_MTU LOWPAN_HARD_MTU_LIMIT
#endif

/* The "physical" limit - due to the datagram_size field in the FRAG headers */
#ifndef LOWPAN_HARD_MTU_LIMIT
#define LOWPAN_HARD_MTU_LIMIT 2047
#endif

buffer_t *lowpan_up(buffer_t *buf);
buffer_t *lowpan_down(buffer_t *buf);

/* Bits in IPHC header, first byte  (RFC 6282)
 *     0                                       1
 *     0   1   2   3   4   5   6   7   8   9   0   1   2   3   4   5
 *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *   | 0 | 1 | 1 |  TF   |NH | HLIM  |CID|SAC|  SAM  | M |DAC|  DAM  |
 *   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *
 */
#define HC_LOWPAN_DISPATCH              LOWPAN_DISPATCH_IPHC /* 0x60 */

#define HC_TF_MASK                      0x18
#define HC_TF_ECN_DSCP_FLOW_LABEL       0x00
#define HC_TF_ECN_FLOW_LABEL            0x08
#define HC_TF_ECN_DSCP                  0x10
#define HC_TF_ELIDED                    0x18

#define HC_NEXT_HEADER_MASK             0x04

#define HC_HOP_LIMIT_MASK               0x03
#define HC_HOP_LIMIT_CARRIED_IN_LINE    0x00
#define HC_HOP_LIMIT_1                  0x01
#define HC_HOP_LIMIT_64                 0x02
#define HC_HOP_LIMIT_255                0x03

/* Bits in IPHC Dispatch header, second byte */
#define HC_CIDE_COMP                    0x80
#define HC_SRCADR_COMP                  0x40
#define HC_SRC_ADR_MODE_MASK            0x30
#define HC_SRC_ADR_128_BIT              0x00
#define HC_SRC_ADR_64_BIT               0x10
#define HC_SRC_ADR_16_BIT               0x20
#define HC_SRC_ADR_FROM_MAC             0x30

#define HC_MULTICAST_COMP               0x08
#define HC_DSTADR_COMP                  0x04
#define HC_DST_ADR_MODE_MASK            0x03
#define HC_DST_ADR_128_BIT              0x00
#define HC_DST_ADR_64_BIT               0x01
#define HC_DST_ADR_16_BIT               0x02
#define HC_DST_ADR_FROM_MAC             0x03

#define HC_128BIT_MULTICAST             0x00
#define HC_48BIT_MULTICAST              0x01 //FFxx::xx:xxxx:xxxx
#define HC_32BIT_MULTICAST              0x02 //FFxx::xx:xxxx
#define HC_8BIT_MULTICAST               0x03 //FF02::xx
#define HC_48BIT_CONTEXT_MULTICAST      0x04 //FFxx:xxLL:PPPP:PPPP:PPPP:PPPP:xxxx:xxxx


/* 6LoWPAN Types
 *
 *         Pattern    Header Type
 *       +------------+-----------------------------------------------+
 *       | 00  xxxxxx | NALP       - Not a LoWPAN frame               |
 *       | 01  000000 | ESC        - Additional Dispatch byte follows |
 *       | 01  000001 | IPv6       - Uncompressed IPv6 Addresses      |
 *       | 01  000010 | LOWPAN_HC1 - LOWPAN_HC1 compressed IPv6       |
 *       | 01  000011 | LOWPAN_DFF - Depth-First Forwarding (RFC6971) |
 *       | 01  000100 | reserved   - Reserved for future use          |
 *       |   ...      | reserved   - Reserved for future use          |
 *       | 01  001111 | reserved   - Reserved for future use          |
 *       | 01  010000 | LOWPAN_BC0 - LOWPAN_BC0 broadcast             |
 *       | 01  010001 | reserved   - Reserved for future use          |
 *       |   ...      | reserved   - Reserved for future use          |
 *       | 01  011111 | reserved   - Reserved for future use          |
 *       | 01  1xxxxx | LOWPAN_IPHC- RFC 6282 IPv6 Header Compression |
 *       | 10  xxxxxx | MESH       - Mesh Header                      |
 *       | 11  000xxx | FRAG1      - Fragmentation Header (first)     |
 *       | 11  001000 | reserved   - Reserved for future use          |
 *       |   ...      | reserved   - Reserved for future use          |
 *       | 11  011111 | reserved   - Reserved for future use          |
 *       | 11  100xxx | FRAGN      - Fragmentation Header (subsequent)|
 *       | 11  101000 | reserved   - Reserved for future use          |
 *       |   ...      | reserved   - Reserved for future use          |
 *       | 11  111111 | reserved   - Reserved for future use          |
 *       +------------+-----------------------------------------------+
 *
 */
#define LOWPAN_NALP                 0x00 /* 00 xxxxxx Not LoWPAN */
#define LOWPAN_NALP_MASK            0xC0
#define LOWPAN_DISPATCH_ESC         0x40 /* RFC 6282 supercedes RFC 4944 */
#define LOWPAN_DISPATCH_IPV6        0x41
#define LOWPAN_DISPATCH_HC1         0x42
#define LOWPAN_DISPATCH_DFF         0x43 /* RFC 6971 */
#define LOWPAN_DISPATCH_BC0         0x50
#define LOWPAN_DISPATCH_IPHC        0x60 /* 01 1xxxxx IPHC */
#define LOWPAN_DISPATCH_IPHC_MASK   0xE0
#define LOWPAN_MESH                 0x80 /* 10 xxxxxx MESH */
#define LOWPAN_MESH_MASK            0xC0
#define LOWPAN_FRAG1                0xC0 /* 11 000xxx FRAG1 */
#define LOWPAN_FRAG1_MASK           0xF8
#define LOWPAN_FRAGN                0xE0 /* 11 100xxx FRAGN */
#define LOWPAN_FRAGN_MASK           0xF8
#define LOWPAN_FRAGN_BIT            0x20 /* xx 1xxxxx distinguish FRAGN */
#define LOWPAN_FRAG                 0xC0 /* 11 x00xxx FRAG1/FRAGN */
#define LOWPAN_FRAG_MASK            0xD8

/*
 * 6LoWPAN Next Header Compression codes
 *
 *   00000000 to 11011111: (unassigned)
 *   1110000N: IPv6 Hop-by-Hop Options Header       [RFC6282]
 *   1110001N: IPv6 Routing Header                  [RFC6282]
 *   1110010N: IPv6 Fragment Header                 [RFC6282]
 *   1110011N: IPv6 Destination Options Header      [RFC6282]
 *   1110100N: IPv6 Mobility Header                 [RFC6282]
 *   1110111N: IPv6 Header                          [RFC6282]
 *   11110CPP: UDP Header                           [RFC6282]
 *   11111000 to 11111110: (unassigned)
 *
 * XXX cipv6_up() currently relies on IPv6 Next Header values and NHC codes
 * being disjoint - it does not distinguish the two code spaces.
 */
#define NHC_EXT_HEADER      0xE0 /* 1110xxxx */
#define NHC_EXT_HEADER_MASK 0xF0
#define NHC_UDP             0xF0 /* 11110xxx */
#define NHC_UDP_MASK        0xF8

#define NHC_EXT_NH          0x01
#define NHC_EXT_ID_MASK     0xFE
#define NHC_EXT_HOP_BY_HOP  0xE0
#define NHC_EXT_ROUTING     0xE2
#define NHC_EXT_FRAG        0xE4
#define NHC_EXT_DEST_OPT    0xE6
#define NHC_EXT_MOBILITY    0xE8
#define NHC_EXT_IPV6        0xEE

/*
 *  RFC 6282
 *
 *  4.3.3. UDP LOWPAN_NHC Format
 *
 *
 *                        0   1   2   3   4   5   6   7
 *                      +---+---+---+---+---+---+---+---+
 *                      | 1 | 1 | 1 | 1 | 0 | C |   P   |
 *                      +---+---+---+---+---+---+---+---+
 *
 *                       Figure 14: UDP Header Encoding
 */
#define NHC_UDP_PORT_COMPRESS_NONE  0
#define NHC_UDP_PORT_COMPRESS_DST   1
#define NHC_UDP_PORT_COMPRESS_SRC   2
#define NHC_UDP_PORT_COMPRESS_BOTH  3
#define NHC_UDP_PORT_COMPRESS_MASK  3
#define NHC_UDP_CKSUM_COMPRESS      4

#endif
