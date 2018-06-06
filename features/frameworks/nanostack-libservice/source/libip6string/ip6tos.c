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
#include <stdio.h>
#include <string.h>
#include "common_functions.h"
#include "ip6string.h"

/**
 * Print binary IPv6 address to a string.
 * String must contain enough room for full address, 40 bytes exact.
 * IPv4 tunneling addresses are not covered.
 * \param addr IPv6 address.
 * \p buffer to write string to.
 */
uint_fast8_t ip6tos(const void *ip6addr, char *p)
{
    char *p_orig = p;
    uint_fast8_t zero_start = 255, zero_len = 1;
    const uint8_t *addr = ip6addr;
    uint_fast16_t part;

    /* Follow RFC 5952 - pre-scan for longest run of zeros */
    for (uint_fast8_t n = 0; n < 8; n++) {
        part = *addr++;
        part = (part << 8) | *addr++;
        if (part != 0) {
            continue;
        }

        /* We're at the start of a run of zeros - scan to non-zero (or end) */
        uint_fast8_t n0 = n;
        for (n = n0 + 1; n < 8; n++) {
            part = *addr++;
            part = (part << 8) | *addr++;
            if (part != 0) {
                break;
            }
        }

        /* Now n0->initial zero of run, n->after final zero in run. Is this the
         * longest run yet? If equal, we stick with the previous one - RFC 5952
         * S4.2.3. Note that zero_len being initialised to 1 stops us
         * shortening a 1-part run (S4.2.2.)
         */
        if (n - n0 > zero_len) {
            zero_start = n0;
            zero_len = n - n0;
        }

        /* Continue scan for initial zeros from part n+1 - we've already
         * consumed part n, and know it's non-zero. */
    }

    /* Now go back and print, jumping over any zero run */
    addr = ip6addr;
    for (uint_fast8_t n = 0; n < 8;) {
        if (n == zero_start) {
            if (n == 0) {
                *p++ = ':';
            }
            *p++ = ':';
            addr += 2 * zero_len;
            n += zero_len;
            continue;
        }

        part = *addr++;
        part = (part << 8) | *addr++;
        n++;

        p += sprintf(p, "%"PRIxFAST16, part);

        /* One iteration writes "part:" rather than ":part", and has the
         * explicit check for n == 8 below, to allow easy extension for
         * IPv4-in-IPv6-type addresses ("xxxx::xxxx:a.b.c.d"): we'd just
         * run the same loop for 6 parts, and output would then finish with the
         * required : or ::, ready for "a.b.c.d" to be tacked on.
         */
        if (n != 8) {
            *p++ = ':';
        }
    }
    *p = '\0';

    // Return length of generated string, excluding the terminating null character
    return p - p_orig;
}

uint_fast8_t ip6_prefix_tos(const void *prefix, uint_fast8_t prefix_len, char *p)
{
    char *wptr = p;
    uint8_t addr[16] = {0};

    if (prefix_len > 128) {
        return 0;
    }

    // Generate prefix part of the string
    bitcopy(addr, prefix, prefix_len);
    wptr += ip6tos(addr, wptr);
    // Add the prefix length part of the string
    wptr += sprintf(wptr, "/%"PRIuFAST8, prefix_len);

    // Return total length of generated string
    return wptr - p;
}
