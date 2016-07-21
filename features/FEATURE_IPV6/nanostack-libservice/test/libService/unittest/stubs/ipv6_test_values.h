/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#ifndef IPV6_TEST_VALUES_H
#define IPV6_TEST_VALUES_H

#include <stdint.h>

struct ip6_addresses_and_its_binary_form_t {
    char *addr;
    uint8_t bin[16];
} ipv6_test_values[] = {
    { "2001:db8::1:0:0:1",                      { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }},
    { "2001:db8:aaaa:bbbb:cccc:dddd:eeee:1",    { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0xee, 0xee, 0x00, 0x01 }},
    { "2001:db8::1",                            { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }},
    { "2001:db8::2:1",                          { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1 }},
    { "2001:db8:aaaa:bbbb:cccc:dddd:0:1",       { 0x20, 0x01, 0xd, 0xb8, 0xaa, 0xaa, 0xbb, 0xbb, 0xcc, 0xcc, 0xdd, 0xdd, 0, 0, 0x00, 0x01 }},
    { "2001:db8::aaaa:0:0:1",                   { 0x20, 0x01, 0xd, 0xb8, 0, 0, 0, 0, 0xaa, 0xaa, 0, 0, 0, 0, 0, 1 }},
    { "2001:0:0:1::1",                          { 0x20, 0x01, 0, 0 , 0, 0 , 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }},
    { "2001:0:0:1::",                           { 0x20, 0x01, 0, 0 , 0, 0 , 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }},
    { "2001:db8::",                             { 0x20, 0x01, 0xd, 0xb8 }},
    { "::aaaa:0:0:1",                           { 0, 0, 0, 0, 0, 0, 0, 0, 0xaa, 0xaa, 0, 0, 0, 0, 0, 1 }},
    { "::1",                                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }},
    { "::",                                     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
    { "FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:FFFF", { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},
    {NULL, {0}}
};

#endif /* IPV6_TEST_VALUES_H */
