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
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "common_functions.h"
#include "ip6string.h"

static uint16_t hex(const char *p);
static bool is_hex(char c);

/**
 * Convert numeric IPv6 address string to a binary.
 * IPv4 tunnelling addresses are not covered.
 * \param ip6addr IPv6 address in string format.
 * \param len Length of ipv6 string.
 * \param dest buffer for address. MUST be 16 bytes.
 * \return boolean set to true if conversion succeed, false if it didn't
 */
bool stoip6(const char *ip6addr, size_t len, void *dest)
{
    uint8_t *addr;
    const char *p, *q;
    int_fast8_t field_no, coloncolon = -1;

    addr = dest;

    if (len > 39) { // Too long, not possible. We do not support IPv4-mapped IPv6 addresses
        goto error;
    }

    // First go forward the string, until end, noting :: position if any
    // We're decrementing `len` as we go forward, and stop when it reaches 0
    for (field_no = 0, p = ip6addr; len && *p; p = q + 1) {

        for (q = p; len && *q && (*q != ':'); len -= 1) { // Seek for ':' or end
            if (!is_hex(*q++)) { // There must only be hex characters besides ':'
                goto error;
            }
        }

        if ((q - p) > 4) { // We can't have more than 4 hex digits per segment
            goto error;
        }

        if (field_no == 8) { // If the address goes farther than 8 segments
            goto error;
        }

        // Convert and write this part, (high-endian AKA network byte order)
        addr = common_write_16_bit(hex(p), addr);
        field_no++;

        // We handle the colons
        if (len) {
            // Check if we reached "::"
            if (q[0] == ':' && q[1] == ':') {
                if (coloncolon != -1) { // We are not supposed to see "::" more than once per address
                    goto error;
                }
                coloncolon = field_no;
                q++;
                len -= 2;
            } else {
                len -= 1;
            }
        }
    }

    if (coloncolon != -1) {
        /* Insert zeros in the appropriate place */
        uint_fast8_t head_size = 2 * coloncolon;
        uint_fast8_t inserted_size = 2 * (8 - field_no);
        uint_fast8_t tail_size = 16 - head_size - inserted_size;
        addr = dest;
        memmove(addr + head_size + inserted_size, addr + head_size, tail_size);
        memset(addr + head_size, 0, inserted_size);
    } else if (field_no != 8) { // Report an error if we didn't get 8 fields
        goto error;
    }
    return true;

error:
    // Fill the output buffer with 0 so we stick to the old failure behavior.
    // We are however more agressive and wipe the entire address, and do so more often.
    memset(dest, 0, 16);
    return false;
}

unsigned char sipv6_prefixlength(const char *ip6addr)
{
    char *ptr = strchr(ip6addr, '/');
    if (ptr) {
        return (unsigned char)strtoul(ptr + 1, 0, 10);
    }
    return 0;
}

int stoip6_prefix(const char *ip6addr, void *dest, int_fast16_t *prefix_len_out)
{
    size_t addr_len, total_len;
    int_fast16_t prefix_length;

    if (prefix_len_out) {
        *prefix_len_out = -1;
    }

    total_len = addr_len = strlen(ip6addr);
    const char *ptr = strchr(ip6addr, '/');
    if (ptr) {
        addr_len = ptr - ip6addr;
        if (prefix_len_out) {
            if (total_len - addr_len > 3) {
                /* too many digits in prefix */
                return -1;
            }

            prefix_length = strtoul(ptr + 1, 0, 10);
            if (prefix_length <  0 || prefix_length > 128) {
                /* prefix value illegal */
                return -1;
            }

            *prefix_len_out = prefix_length;
        }
    }

    if (!stoip6(ip6addr, addr_len, dest)) {
        /* parser failure */
        return -1;
    }

    return 0;
}

static bool is_hex(char c)
{
    // 'A' (0x41) and 'a' (0x61) are mapped in the ASCII table in such a way that masking the 0x20 bit turn 'a' in 'A'
    if ((c & ~0x20) >= 'A' && (c & ~0x20) <= 'F') {
        return true;
    }

    if (c >= '0' && c <= '9') {
        return true;
    }

    return false;
}

static uint16_t hex(const char *p)
{
    uint16_t val = 0;

    for (;;) {
        char c = *p++;
        if ((c >= '0') && (c <= '9')) {
            val = (val << 4) | (c - '0');
        } else if ((c >= 'A') && (c <= 'F')) {
            val = (val << 4) | (10 + (c - 'A'));
        } else if ((c >= 'a') && (c <= 'f')) {
            val = (val << 4) | (10 + (c - 'a'));
        } else {
            break; // Non hex character
        }
    }
    return val;
}
