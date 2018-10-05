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

#include "nsconfig.h"

#include "randLIB.h"
#include "Service_Libs/fnv_hash/fnv_hash.h"

#include "ipv6_flow.h"

bool ipv6_flow_auto_label = true;

/* Flow label hash computation for RFC 6347, but using algorithm 8 as
 * suggested by "Comparing Hash Function Algorithms for the IPv6 Flow Label"
 * (Anderson, Brownlee, Carpenter 2012).
 */

static inline uint_fast24_t fold_32_to_flow(uint32_t val32)
{
    uint_fast24_t flow = (uint_fast24_t) ((val32 ^ (val32 >> 20)) & 0xFFFFF);
    if (flow == 0) {
        flow = 1;
    }
    return flow;
}

/* Compute a flow from the traditional (src-IP,dst-IP,proto,src-port,dst-port)
 * 5-tuple. To be used when building our own IP headers from a transport module.
 */
uint_fast24_t ipv6_flow_5tuple(const uint8_t src_addr[static 16],
                               const uint8_t dst_addr[static 16],
                               uint8_t protocol,
                               uint16_t src_port,
                               uint16_t dst_port)
{
    uint32_t hash;
    const uint8_t bytes[] = { dst_port >> 8, dst_port, src_port >> 8, src_port, protocol };

    /* Hash algorithms suggest starting with the low-order bytes, as they're
     * most likely to vary, increasing potential dispersion. This means using
     * the "reverse" function on the IP addresses, and we use the same reverse
     * for the other 3 tuple members to re-use the code.
     */
    hash = fnv_hash_1a_32_reverse_block(src_addr, 16);
    hash = fnv_hash_1a_32_reverse_block_update(hash, dst_addr, 16);
    hash = fnv_hash_1a_32_reverse_block_update(hash, bytes, sizeof bytes);

    return fold_32_to_flow(hash);
}

/* Compute a flow from a basic (src-IP,dst-IP) 2-tuple, plus an existing flow
 * label. To be used on tunnel entry, using fields from inner header.
 */
uint_fast24_t ipv6_flow_2tuple_flow(const uint8_t src_addr[static 16],
                                    const uint8_t dst_addr[static 16],
                                    uint_fast24_t flow)
{
    uint32_t hash;

    flow &= 0xFFFFF;
    const uint8_t bytes[] = { flow >> 16, flow >> 8, flow };

    hash = fnv_hash_1a_32_reverse_block(bytes, sizeof bytes);
    hash = fnv_hash_1a_32_reverse_block_update(hash, src_addr, 16);
    hash = fnv_hash_1a_32_reverse_block_update(hash, dst_addr, 16);

    return fold_32_to_flow(hash);
}

/* Compute a random flow label. To be used on a connected socket. */
uint_fast24_t ipv6_flow_random(void)
{
    uint32_t rand32 = randLIB_get_32bit();

    return fold_32_to_flow(rand32);
}
