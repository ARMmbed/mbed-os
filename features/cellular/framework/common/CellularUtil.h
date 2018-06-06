/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef CELLULAR_UTIL_H_
#define CELLULAR_UTIL_H_

#include <stddef.h>
#include <inttypes.h>

namespace mbed_cellular_util {

// some helper macros
#define EMPTY_CHECK(val) (val ## 1)
#define EMPTY(val) (EMPTY_CHECK(val) == 1)
#define _CELLULAR_STRINGIFY(a) #a
#define CELLULAR_STRINGIFY(a) _CELLULAR_STRINGIFY(a)

static const char hex_values[] = "0123456789ABCDEF";

/** Converts the given IP address to proper IPv6 address if needed.
 *  Conversion is done only if it's NOT IPv4 and separated with colons.
 *  AT command +CGPADDR can give IP address in format of a1.a2.a3.a4.a5.a6.a7.a8.a9.a10.a11.a12.a13.a14.a15.a16 for IPv6
 *  where ax are in decimal format. In this case, function converts decimals to hex with separated with colons.
 *
 *  @param ip       IP address that can be IPv4 or IPv6 in different formats from AT command +CGPADDR. Converted result uses same buffer.
 */
void convert_ipv6(char* ip);

/** Separates IP addresses from the given 'orig' string. 'orig' may contain zero, one or two IP addresses in various formats.
 *  See AT command +CGPIAF from 3GPP TS 27.007 for details. Does also needed conversions for IPv6 addresses.
 *
 *  @param orig          original string that contains zero, one or two IP addressees in various formats
 *  @param ip            preallocated buffer that might contain IP address after return
 *  @param ip_size       size of preallocated buffer IP
 *  @param ip2           preallocated buffer that might contain IP address after return
 *  @param ip2_size      size of preallocated buffer ip2
 *
 */
void separate_ip_addresses(char* orig, char* ip, size_t ip_size, char* ip2, size_t ip2_size);

/** Swaps the arrays if param IP does not contain IPv6 address but param ip2 does.
 *
 *  @param ip        IP address
 *  @param ip_size   size of buffer ip
 *  @param ip2       IP address
 *  @param ip2_size  size of buffer ip2
 */
void prefer_ipv6(char* ip, size_t ip_size, char* ip2, size_t ip2_size);

/** Converts the given int to two hex characters
 *
 *  @param num number to be converted to hex string
 *  @param buf preallocated buffer that will contain 2 char length hex value
 */
void int_to_hex_str(uint8_t num, char* buf);

/** Converts the given buffer 'str' to hex buffer 'buf. First 'len' char's are converted to two hex bytes.
 *
 *  @param str                   char buffer that is to be converted to hex
 *  @param len                   how many chars from str are to be converted
 *  @param buf                   destination buffer for hex converted chars. Buffer should be double the size of str to fit hex-encoded string.
 *  @param omit_leading_zero     if true then possible leading zeroes are omitted
 */
int char_str_to_hex_str(const char* str, uint16_t len, char *buf, bool omit_leading_zero = false);

/** Converts the given hex string to integer
 *
 *  @param hex_string        hex string from where chars are converted to int
 *  @param hex_string_length length of the param hex_string
 *  @return                  hex_string converted to int
 */
int hex_str_to_int(const char *hex_string, int hex_string_length);

/** Converts the given hex string str to char string to buf
 *
 *  @param str hex string that is converted to char string to buf
 *  @param len length of the param str/how many hex are converted
 *  @param buf preallocated buffer where result conversion is stored
 *  @return    length of the buf
 */
int hex_str_to_char_str(const char* str, uint16_t len, char *buf);

/** Converts the given uint to binary string. Fills the given str starting from [0] with the number of bits defined by bit_cnt
 *  For example uint_to_binary_string(9, str, 10) would fill str "0000001001"
 *  For example uint_to_binary_string(9, str, 3) would fill str "001"
 *
 *  @param num       uint to converts to binary string
 *  @param str       buffer for converted binary string
 *  @param str_size  size of the str buffer
 *  @param bit_cnt   defines how many bits are filled to buffer started from lsb
 */
void uint_to_binary_str(uint32_t num, char* str, int str_size, int bit_cnt);

/** Get dynamic port for socket
 *
 *  @return next port number above 49152
 */
uint16_t get_dynamic_ip_port();

} // namespace mbed_cellular_util

#endif
