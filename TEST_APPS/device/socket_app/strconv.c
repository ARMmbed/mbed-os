/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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
#include <stdio.h>
#include <ctype.h>

#include "ip6string.h"
#include "strconv.h"


int8_t strtohex(uint8_t *out, const char *str, int8_t max_length)
{
    int8_t i = 0;
    char *rd = (char *)str;
    uint8_t *wr = out;
    while (*rd != 0) {
        if (i > max_length) {
            break;
        }
        *wr++ = strtoul(rd, &rd, 16);
        while (!isxdigit((int)*rd) && *rd != 0) {
            rd++; //skip some invalid characters
        }
        i++;
    }
    return i;
}

int hexstr_to_bytes_inplace(char *str)
{
    int16_t len, i, j;
    if (str == NULL) {
        return -1;
    }
    len = strlen(str);
    if (len < 2 || (len + 1) % 3 != 0) {
        return -1;
    }
    for (i = 0, j = 0; i < len; i += 3, ++j) {
        str[j] = (char)strtol(str + i, 0, 16);
    }
    return j;
}

// convert hex string (eg. "76 ab ff") to binary array
int string_to_bytes(const char *str, uint8_t *buf, int bytes)
{
    int len = strlen(str);
    if (len <= (3 * bytes - 1)) {
        int i;
        for (i = 0; i < bytes; i++) {
            if (i * 3 < len) {
                buf[i] = (uint8_t)strtoul(str + i * 3, 0, 16);
            } else {
                buf[i] = 0;
            }
        }
        return 0;
    }
    return -1;
}

int16_t hextostr(const uint8_t *buf, uint16_t buf_length, char *out, int16_t out_length, char delimiter)
{
    int16_t outLeft = out_length;
    int16_t arrLeft = buf_length;
    const uint8_t *rd = buf;
    int retcode = 0;
    char *wr = out;
    while (arrLeft > 0 && outLeft > 0) {
        retcode = snprintf(wr, outLeft, "%02x", *rd);

        if (retcode <= 0 || retcode >= outLeft) {
            break;
        }
        outLeft -= retcode;
        wr += retcode;
        arrLeft --;
        rd++;
        if (delimiter && arrLeft > 0 && outLeft > 0) {
            *wr++ = delimiter;
            outLeft--;
            *wr = 0;
        }
    }
    return (int16_t)(wr - out);
}
int replace_hexdata(char *str)
{
    char *ptr = str;
    if (str == NULL) {
        return 0;
    }
    while (*ptr) {
        if (ptr[0] == '\\') {
            if (ptr[1] == 'n') {
                ptr[0] = 0x0a;
                memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
            } else if (ptr[1] == 'r') {
                ptr[0] = 0x0d;
                memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
            } else if (ptr[1] == 'x') {
                char *end;
                ptr[0] = (char)strtoul(ptr + 2, &end, 16);
                memmove(ptr + 1, end, strlen(end) + 1);
            } else if (isdigit((int)ptr[1])) {
                char *end;
                ptr[0] = strtoul(ptr + 1, &end, 10);
                memmove(ptr + 1, end, strlen(end) + 1);
            }
        }
        ptr++;
    }
    return ptr - str;
}
bool is_visible(uint8_t *buf, int len)
{
    while (len--) {
        if (!isalnum(*buf) && *buf != ' ') {
            return false;
        }
        buf++;
    }
    return true;
}

char *strdupl(const char *str)
{
    if (!str) {
        return NULL;
    }
    char *p = malloc(strlen(str) + 1);
    if (!p) {
        return p;
    }
    strcpy(p, str);
    return p;
}
int strnlen_(const char *s, int n)
{
    char *end = memchr(s, 0, n);
    return end ? end - s : n;
}
char *strndupl(const char *s, int n)
{
    char *p = NULL;
    int len = strnlen_(s, n);
    p = malloc(len + 1);
    if (!p) {
        return p;
    }
    p[len] = 0;
    return memcpy(p, s, len);
}
int strnicmp_(char const *a, char const *b, int n)
{
    for (; (n > 0 && *a != 0 && *b != 0) ; a++, b++, n--) {
        int d = tolower((int) * a) - tolower((int) * b);
        if (d != 0 || !*a) {
            return d;
        }
    }
    return 0;
}


/* HELPING PRINT FUNCTIONS for cmd_printf */

static inline uint8_t context_split_mask(uint_fast8_t split_value)
{
    return (uint8_t) - (0x100u >> split_value);
}

static uint8_t *bitcopy(uint8_t *restrict dst, const uint8_t *restrict src, uint_fast8_t bits)
{
    uint_fast8_t bytes = bits / 8;
    bits %= 8;

    if (bytes) {
        dst = (uint8_t *) memcpy(dst, src, bytes) + bytes;
        src += bytes;
    }

    if (bits) {
        uint_fast8_t split_bit = context_split_mask(bits);
        *dst = (*src & split_bit) | (*dst & ~ split_bit);
    }

    return dst;
}

char tmp_print_buffer[128] = {0};

char *print_ipv6(const void *addr_ptr)
{
    ip6tos(addr_ptr, tmp_print_buffer);
    return tmp_print_buffer;
}
char *print_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
{
    char *str = tmp_print_buffer;
    int retval;
    char tmp[40];
    uint8_t addr[16] = {0};

    if (prefix_len != 0) {
        if (prefix == NULL || prefix_len > 128) {
            return "<err>";
        }
        bitcopy(addr, prefix, prefix_len);
    }

    ip6tos(addr, tmp);
    retval = snprintf(str, 128, "%s/%u", tmp, prefix_len);
    if (retval <= 0) {
        return "";
    }
    return str;
}
char *print_array(const uint8_t *buf, uint16_t len)
{
    int16_t retcode = hextostr(buf, len, tmp_print_buffer, 128, ':');
    if (retcode > 0) {
        //yeah, something is converted
    }
    return tmp_print_buffer;
}

