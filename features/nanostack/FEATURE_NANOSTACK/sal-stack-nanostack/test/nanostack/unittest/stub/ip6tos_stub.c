/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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
#include <stdio.h>
#include <inttypes.h>
#include "ip6string.h"

uint_fast8_t ip6tos(const void *ip6addr, char *p)
{
    return 0;
}

uint_fast8_t ip6_prefix_tos(const void *prefix, uint_fast8_t prefix_len, char *p)
{
    return 0;
}

void stoip6(const char *ip6addr, size_t len, void *dest)
{
}

unsigned char sipv6_prefixlength(const char *ip6addr)
{
    return 0;
}
