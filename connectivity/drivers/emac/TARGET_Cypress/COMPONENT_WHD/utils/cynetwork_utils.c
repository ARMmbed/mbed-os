/*
 * Copyright (c) 2018-2019 ARM Limited
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

#include "cynetwork_utils.h"

uint8_t unsigned_to_decimal_string(uint32_t value, char *output, uint8_t min_length, uint8_t max_length)
{
    uint8_t digits_left;
    char buffer[] = "0000000000";

    if (output == NULL) {
        return 0;
    }

    max_length = (uint8_t) MIN(max_length, sizeof(buffer));
    digits_left = max_length;
    while ((value != 0) && (digits_left != 0)) {
        --digits_left;
        buffer[digits_left] = (char)((value % 10) + '0');
        value = value / 10;
    }

    digits_left = (uint8_t) MIN((max_length - min_length), digits_left);
    memcpy(output, &buffer[digits_left], (size_t)(max_length - digits_left));

    /* Add terminating null */
    output[(max_length - digits_left)] = '\x00';

    return (uint8_t)(max_length - digits_left);
}

void ipv4_to_string(char buffer[16], uint32_t ipv4_address)
{
    uint8_t *ip = (uint8_t *)&ipv4_address;

    /* unsigned_to_decimal_string() null-terminates the string
     * Save the original last character and replace it */
    char last_char = buffer[16];
    unsigned_to_decimal_string(ip[0], &buffer[0], 3, 3);
    buffer[3] = '.';
    unsigned_to_decimal_string(ip[1], &buffer[4], 3, 3);
    buffer[7] = '.';
    unsigned_to_decimal_string(ip[2], &buffer[8], 3, 3);
    buffer[11] = '.';
    unsigned_to_decimal_string(ip[3], &buffer[12], 3, 3);
    buffer[16] = last_char;
}

uint32_t string_to_ipv4(const char *buffer)
{
    uint32_t temp = 0;
    int char_count = 0;
    const char *ptr = buffer;

    while ((ptr != NULL)  && (*ptr != 0) && (char_count++ < 16)) {
        uint8_t byte = 0;
        while ((*ptr != 0) && (*ptr != '.') && (char_count++ < 16)) {
            byte *= 10;
            if ((*ptr >= '0') && (*ptr <= '9')) {
                byte += (*ptr - '0');
            } else {
                break;
            }
            ptr++;
        }
        temp <<= 8;
        temp |= byte;
        if (*ptr == '.') {
            ptr++;  /* skip '.' */
        }
    }
    return temp;
}

