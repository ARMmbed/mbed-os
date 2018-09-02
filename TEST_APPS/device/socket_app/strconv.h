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
#ifndef STRCONVERSION_H
#define STRCONVERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ns_types.h"

/** Convert hex string to binary array
 * e.g.
 * char str[] = {30, 31, 32, 33};
 * uint8_t buffer[4];
 * strtohex( buffer, str, 4 );
 */
int8_t  strtohex(uint8_t *out, const char *str, int8_t max_length);

/** Convert space separated hex string (eg. "76 ab ff") to binary array.
 */
int string_to_bytes(const char *str, uint8_t *buf, int bytes);

/** Convert a colon/space separated hex string (eg. "76:ab:ff") to binary
 * array (inplace) returning the number of the bytes in the array.
 */
int hexstr_to_bytes_inplace(char *str);

/** Convert hex array to string
 */
int16_t hextostr(const uint8_t *buf, uint16_t buf_length, char *out, int16_t out_length, char delimiter);
/** Replace hex characters from string
 * e.g.
 *  "hello\\n" -> "hello\n"
 *  "hello\\r" -> "hello\r"
 *  "val: \\10" -> "val: \x0a"  //integer
 *  "val: \\x10" -> "val: \x10" //hex value
 *  @param str  string that will be replaced
 *  @return length
 */
int replace_hexdata(char *str);
/**
 * check if array contains only visible characters
 * = isalpha | isdigit
 */
bool is_visible(uint8_t *buf, int len);

/** Convert ipv6 address to string format
 */
char   *print_ipv6(const void *addr_ptr);
/** Convert ipv6 prefix to string format
 */
char *print_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len);
/** Convert binary array to string format and return static results
 */
char   *print_array(const uint8_t *buf, uint16_t len);
/** The strdupl() function shall return a pointer to a new string,
 * which is a duplicate of the string pointed to by s1. The returned pointer can be passed to free().
 * A null pointer is returned if the new string cannot be created.
 * strdupl are same than linux strdup, but this way we avoid to duplicate reference in linux
 */
char *strdupl(const char *str);
/** The strdup() function returns a pointer to a new string which is a duplicate of the string.
 * Memory for the new string is obtained with malloc(3), and can be freed with free(3).
 * The strndup() function is similar, but only copies at most n bytes. If s is longer than n,
 * only n bytes are copied, and a terminating null byte ('\0') is added.
 */
char *strndupl(const char *s, int n);
/** strnlen - determine the length of a fixed-size string
 * The strnlen() function returns the number of bytes in the string pointed to by s, excluding the terminating null bye ('\0'), but at most maxlen.
 * In doing this, strnlen() looks only at the first maxlen bytes at s and never beyond s+maxlen.
 * The strnlen() function returns strlen(s), if that is less than maxlen, or maxlen if there is no null byte ('\0')
 * among the first maxlen bytes pointed to by s.
 */
int strnlen_(const char *s, int n);
/** strnicmp compares a and b without sensitivity to case.
 * All alphabetic characters in the two arguments a and b are converted to lowercase before the comparison.
 */
int strnicmp_(char const *a, char const *b, int n);

#ifdef __cplusplus
}
#endif
#endif

