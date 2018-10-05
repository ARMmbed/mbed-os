/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "fnv_hash.h"

#if 0
uint32_t fnv_hash_1a_32_block(const uint8_t *data, uint16_t len)
{
    return fnv_hash_1a_32_block_update(UINT32_C(0x811c9dc5), data, len);
}

uint32_t fnv_hash_1a_32_block_update(uint32_t hash, const uint8_t *data, uint16_t len)
{
    for (; len; --len) {
        hash ^= *data++;
        hash *= UINT32_C(0x01000193);
    }
    return hash;
}
#endif

uint32_t fnv_hash_1a_32_reverse_block(const uint8_t *data, uint16_t len)
{
    return fnv_hash_1a_32_reverse_block_update(UINT32_C(0x811c9dc5), data, len);
}

uint32_t fnv_hash_1a_32_reverse_block_update(uint32_t hash, const uint8_t *data, uint16_t len)
{
    for (data += len; len; --len) {
        hash ^= *--data;
        hash *= UINT32_C(0x01000193);
    }
    return hash;
}
