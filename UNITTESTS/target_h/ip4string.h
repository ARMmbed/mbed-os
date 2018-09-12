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
#ifndef IP4STRING_H
#define IP4STRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

uint_fast8_t ip4tos(const void *ip4addr, char *p)
{
    return 0;
}

bool stoip4(const char *ip4addr, size_t len, void *dest)
{
    return false;
}

#ifdef __cplusplus
}
#endif
#endif
