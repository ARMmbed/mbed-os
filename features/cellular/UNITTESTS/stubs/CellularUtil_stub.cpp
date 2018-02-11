/*
 * Copyright (c) , Arm Limited and affiliates.
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
#include "CellularUtil.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

namespace mbed_cellular_util {

#define MAX_STRING_LEN 200

void str_copy_skip_char(char *dest, uint16_t dest_size, const char *src, char c)
{
}

void str_remove_char(char *src, char c)
{
}

void uint_to_binary_str(uint32_t num, char* str, uint8_t str_size, uint8_t bit_cnt)
{
}

// converts the given str to hex string to buf
uint16_t char_str_to_hex(const char* str, uint16_t len, char *buf, bool omit_leading_zero)
{
    return 0;
}

void convert_ipv6(char* ip)
{
    if (!ip) {
        return;
    }

    int len = strlen(ip);
    int pos = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (ip[i] == '.') {
            pos++;
        }
        if (pos > 3) {
            break;
        }
    }

    // more that 3 periods mean that it was ipv6 but in format of a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16
    // we need to convert it to hexadecimal format separated with colons
    if (pos > 3) {
        pos = 0;
        int ip_pos = 0;
        char b;
        bool set_colon = false;
        for (i = 0; i < len; i++) {
            if (ip[i] == '.') {
                b = (char)strtol (ip+ip_pos, NULL, 10); // convert to char to int so we can change it to hex string
                pos += char_str_to_hex(&b, 1, ip+pos, !set_colon); // omit leading zeroes with using set_colon flag
                if (set_colon) {
                    ip[pos++] = ':';
                    set_colon = false;
                } else {
                    set_colon = true;
                }
                ip_pos = i+1; // skip the '.'
            }

            // handle the last part which does not end with '.' but '\0'
            if (i == len -1) {
                b = (char)strtol(ip+ip_pos, NULL, 10);
                pos += char_str_to_hex(&b, 1, ip+pos, !set_colon);
                ip[pos] = '\0';
            }
        }
    }
}

char* find_dot_number(char* str, int dot_number)
{
    return NULL;
}

void separate_ip4like_addresses(char* orig, char* ip, size_t ip_size, char* ip2, size_t ip2_size)
{
}

void separate_ip_addresses(char* orig, char* ip, size_t ip_size, char* ip2, size_t ip2_size)
{
}

void prefer_ipv6(char* ip, size_t ip_size, char* ip2, size_t ip2_size)
{
}

void int_to_hex_str(uint8_t num, char* buf)
{

}

int hex_str_to_int(const char *hex_string, int hex_string_length)
{
    return 0;
}

int hex_str_to_char_str(const char* str, uint16_t len, char *buf)
{
    return 0;
}

void uint_to_binary_str(uint32_t num, char* str, int str_size, int bit_cnt)
{

}

int char_str_to_hex_str(const char* str, uint16_t len, char *buf, bool omit_leading_zero)
{
    return 0;
}

uint16_t get_dynamic_ip_port()
{
    return 0;
}

} // namespace mbed_cellular_util
