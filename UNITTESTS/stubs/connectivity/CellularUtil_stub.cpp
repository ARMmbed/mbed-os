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
#include "CellularUtil_stub.h"

int CellularUtil_stub::int_value = 0;
uint16_t CellularUtil_stub::uint16_value = 0;
char *CellularUtil_stub::char_ptr = NULL;
int CellularUtil_stub::char_pos = 0;
char *CellularUtil_stub::char_table[50] = {};
int CellularUtil_stub::table_idx = 0;

namespace mbed_cellular_util {

#define MAX_STRING_LEN 200

void str_copy_skip_char(char *dest, uint16_t dest_size, const char *src, char c)
{
}

void str_remove_char(char *src, char c)
{
}

void uint_to_binary_str(uint32_t num, char *str, uint8_t str_size, uint8_t bit_cnt)
{
}

// converts the given str to hex string to buf
uint16_t char_str_to_hex(const char *str, uint16_t len, char *buf, bool omit_leading_zero)
{
    return CellularUtil_stub::uint16_value;
}

nsapi_version_t convert_ipv6(char *ip)
{
    return NSAPI_UNSPEC;
}

char *find_dot_number(char *str, int dot_number)
{
    return CellularUtil_stub::char_ptr;
}

void separate_ip4like_addresses(char *orig, char *ip, size_t ip_size, char *ip2, size_t ip2_size)
{
    if (CellularUtil_stub::table_idx > 1) {
        CellularUtil_stub::table_idx--;
        memcpy(ip, CellularUtil_stub::char_table[CellularUtil_stub::table_idx],
               strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx]));
        ip[strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx])] = '\0';
        CellularUtil_stub::table_idx--;
        if (ip2) {
            memcpy(ip2, CellularUtil_stub::char_table[CellularUtil_stub::table_idx],
                   strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx]));
            ip2[strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx])] = '\0';
        }
    }
}

void separate_ip_addresses(char *orig, char *ip, size_t ip_size, char *ip2, size_t ip2_size)
{
    if (CellularUtil_stub::table_idx > 1) {
        CellularUtil_stub::table_idx--;
        memcpy(ip, CellularUtil_stub::char_table[CellularUtil_stub::table_idx],
               strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx]));
        ip[strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx])] = '\0';
        CellularUtil_stub::table_idx--;
        if (ip2) {
            memcpy(ip2, CellularUtil_stub::char_table[CellularUtil_stub::table_idx],
                   strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx]));
            ip2[strlen(CellularUtil_stub::char_table[CellularUtil_stub::table_idx])] = '\0';
        }
    }
}

void prefer_ipv6(char *ip, size_t ip_size, char *ip2, size_t ip2_size)
{
}

void int_to_hex_str(uint8_t num, char *buf)
{
    buf[0] = '0';
    buf[1] = '2';
}

int hex_str_to_int(const char *hex_string, int hex_string_length)
{
    return CellularUtil_stub::int_value;
}

int hex_str_to_char_str(const char *str, uint16_t len, char *buf)
{
    buf[0] = CellularUtil_stub::char_ptr[CellularUtil_stub::char_pos++];
    return 1;
}

void hex_to_char(const char *hex, char &buf)
{
    buf = CellularUtil_stub::char_ptr[CellularUtil_stub::char_pos++];
}

void uint_to_binary_str(uint32_t num, char *str, int str_size, int bit_cnt)
{

}

int char_str_to_hex_str(const char *str, uint16_t len, char *buf, bool omit_leading_zero)
{
    return CellularUtil_stub::int_value;
}

uint16_t get_dynamic_ip_port()
{
    return CellularUtil_stub::uint16_value;
}

} // namespace mbed_cellular_util
