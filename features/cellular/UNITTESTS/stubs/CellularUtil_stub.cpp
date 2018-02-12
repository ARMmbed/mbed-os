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
    buf[0] = '0';
    buf[1] = '2';
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
    //The code is dependent on this, so this is easiest just to put here
    if (!str || !buf) {
        return 0;
    }

    char *ptr = buf;
    int i=0;
    while (i < len) {
        if (omit_leading_zero == true && i == 0 && !(str[i]>>4 & 0x0F)) {
            *ptr++ = hex_values[(str[i]) & 0x0F];
        } else {
            *ptr++ = hex_values[((str[i])>>4) & 0x0F];
            *ptr++ = hex_values[(str[i]) & 0x0F];
        }
        i++;
    }
    return ptr-buf;
}

uint16_t get_dynamic_ip_port()
{
    return 0;
}

} // namespace mbed_cellular_util
