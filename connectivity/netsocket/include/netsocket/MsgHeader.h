/*
 * Copyright (c) 2021 ARM Limited
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

#ifndef MSG_HEADER_H
#define MSG_HEADER_H

#include "netsocket/nsapi_types.h"

/**
 * Allows iteration through the list of message headers received in the control parameter of the
 * socket_sendto_control / socket_recvfrom_control methods.
 */

struct MsgHeaderIterator {
    // Constructor takes pointer to the first header element and size of the whole list.
    MsgHeaderIterator(nsapi_msghdr_t *hdr, nsapi_size_t size) :
        start(hdr),
        current(nullptr),
        size(size)
    {}

    // Checks if the next address of the iterator is a valid list member.
    bool has_next()
    {
        if (current == nullptr) {
            if (start != nullptr && start->len <= size && start->len >= sizeof(*start)) {
                return true;
            } else {
                return false;
            }
        }

        if (current->len < sizeof(*current)) {
            return false;
        }

        if (get_next_aligned_addr() >= (reinterpret_cast<uint8_t *>(start) + size)) {
            return false;
        }

        return true;
    }

    // Returns pointer to the next member of the list.
    // If next member doesn't exist nullptr is returned.
    nsapi_msghdr_t *next()
    {
        if (!has_next()) {
            return nullptr;
        }

        if (current == nullptr) {
            current = start;
        } else {
            current = reinterpret_cast<nsapi_msghdr *>(get_next_aligned_addr());
        }

        return current;
    }

private:
    // Get address of the next member aligned to the size of msghdr_t.
    void *get_next_aligned_addr()
    {
        size_t remaining_size = size - (reinterpret_cast<uintptr_t>(current) - reinterpret_cast<uintptr_t>(start));
        void *next = reinterpret_cast<void *>(reinterpret_cast<uint8_t *>(current) + current->len);

        next = std::align(
                   alignof(nsapi_msghdr_t),
                   sizeof(nsapi_msghdr_t),
                   next,
                   remaining_size
               );

        return next;
    }

    nsapi_msghdr_t *start;
    nsapi_msghdr_t *current;
    nsapi_size_t size;
};


#endif

