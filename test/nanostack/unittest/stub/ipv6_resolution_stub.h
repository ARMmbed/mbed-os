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
#ifndef __IPV6_RESOLUTION_STUB_H__
#define __IPV6_RESOLUTION_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "inttypes.h"

typedef struct {
    uint8_t uint8_value;
    int8_t int8_value;
    int int_value;
    uint8_t *uint8_ptr;
    char *char_ptr;
    bool bool_value;
    uint8_t uint8_input_value;
    ipv6_neighbour_cache_t *ipv6_neighbour_cache;
} ipv6_resolution_stub_def;

extern ipv6_resolution_stub_def ipv6_resolution_stub;

#ifdef __cplusplus
}
#endif

#endif // __IPV6_RESOLUTION_STUB_H__
