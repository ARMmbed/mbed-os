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

#ifndef IP6STRING_H
#define IP6STRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint_fast8_t ip6tos(const void *ip6addr, char *p)
{
    return 0;
}

void stoip6(const char *ip6addr, size_t len, void *dest) {}

#ifdef __cplusplus
}
#endif
#endif
