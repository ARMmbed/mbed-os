/* Copyright (c) 2018 ARM Limited
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

#include "EMACMemoryManager.h"

void EMACMemoryManager::copy_to_buf(emac_mem_buf_t *to_buf, const void *ptr, uint32_t len)
{
    while (to_buf && len) {
        void *copy_to_ptr = get_ptr(to_buf);
        uint32_t copy_to_len = get_len(to_buf);

        if (copy_to_len > len) {
            copy_to_len = len;
            len = 0;
        } else {
            len -= copy_to_len;
        }

        memcpy(copy_to_ptr, ptr, copy_to_len);
        ptr = static_cast<const uint8_t *>(ptr) + copy_to_len;

        to_buf = get_next(to_buf);
    }
}

uint32_t EMACMemoryManager::copy_from_buf(void *ptr, uint32_t len, const emac_mem_buf_t *from_buf) const
{
    uint32_t copied_len = 0;

    while (from_buf && len) {
        void *copy_from_ptr = get_ptr(from_buf);
        uint32_t copy_from_len = get_len(from_buf);

        if (copy_from_len > len) {
            copy_from_len = len;
            len = 0;
        } else {
            len -= copy_from_len;
        }

        memcpy(ptr, copy_from_ptr, copy_from_len);
        ptr = static_cast<uint8_t *>(ptr) + copy_from_len;
        copied_len += copy_from_len;

        from_buf = get_next(from_buf);
    }

    return copied_len;
}

