/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "stdint.h"
#include "ip_fsc.h"

/** \brief Compute IP checksum for arbitary data
 *
 * Compute an IP checksum, given a arbitrary gather list.
 *
 * See ipv6_fcf for discussion of use.
 *
 * This will work for any arbitrary gather list - it can handle odd
 * alignments. The one limitation is that the 32-bit accumulator limits
 * it to basically 64K of total data.
 */
uint16_t ip_fcf_v(uint_fast8_t count, const ns_iovec_t vec[static count])
{
    uint_fast32_t acc32 = 0;
    bool odd = false;
    while (count) {
        const uint8_t *data_ptr = vec->iov_base;
        uint_fast16_t data_length = vec->iov_len;
        if (odd && data_length > 0) {
            acc32 += *data_ptr++;
            data_length--;
            odd = false;
        }
        while (data_length >= 2) {
            acc32 += (uint_fast16_t) data_ptr[0] << 8 | data_ptr[1];
            data_ptr += 2;
            data_length -= 2;
        }
        if (data_length) {
            acc32 += (uint_fast16_t) data_ptr[0] << 8;
            odd = true;
        }
        vec++;
        count--;
    }

    // Fold down up to 0xffff carries in the 32-bit accumulator
    acc32 = (acc32 >> 16) + (acc32 & 0xffff);

    // Could be one more carry from the previous addition (result <= 0x1fffe)
    uint16_t sum16 = (uint16_t)((acc32 >> 16) + (acc32 & 0xffff));
    return ~sum16;
}

/** \brief Compute IPv6 checksum
 *
 * Compute an IPv6 checksum, given fields of an IPv6 pseudoheader and payload.
 *
 * This returns the 1's-complement of the checksum, as required when
 * generating the checksum for transmission. The result can be 0x0000;
 * for UDP (only) this must be transformed to 0xFFFF to distinguish from
 * a packet with no checksum.
 *
 * To check a packet, this function will return 0 when run on a
 * packet with a valid checksum. Checksums should be checked like this rather
 * than setting the checksum field to zero and comparing generated checksum with
 * the original value - this would fail in the case the received packet had
 * checksum 0xFFFF.
 */
uint16_t ipv6_fcf(const uint8_t src_address[static 16], const uint8_t dest_address[static 16],
                  uint16_t data_length, const uint8_t data_ptr[static data_length],  uint8_t next_protocol)
{
    // Use gather vector to lay out IPv6 pseudo-header (RFC 2460) and data
    uint8_t hdr_data[] = { data_length >> 8, data_length, 0, next_protocol };
    ns_iovec_t vec[4] = {
        { (void *) src_address, 16 },
        { (void *) dest_address, 16 },
        { hdr_data, 4 },
        { (void *) data_ptr, data_length }
    };

    return ip_fcf_v(4, vec);
}
