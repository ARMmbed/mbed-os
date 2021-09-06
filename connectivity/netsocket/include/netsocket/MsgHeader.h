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
 *
 * @par Members types
 *
 * MsgHeaderIterator works on the list which members are of type nsapi_msghdr_t or other types
 * extending this struct. For example nsapi_pktinfo:
 *
 * @code
    typedef struct nsapi_pktinfo {
        nsapi_msghdr_t hdr;
        nsapi_addr_t   ipi_addr;
        int            ipi_ifindex;
        void          *network_interface;
    } nsapi_pktinfo_t;
 * @endcode
 *
 * There are two requirements for such structures to work well with MsgHeaderIterator.
 * - First element needs to be of type nsapi_msghdr_t.
 * - Value of the field len of the nsapi_msghdr_t needs to bet set to the size of the whole extending type.
 *   For example:
 *
 * @code
    nsapi_pktinfo_t pkt_info;
    pkt_info.hdr.len = sizeof(nsapi_pktinfo_t);
 * @endcode
 *
 * This value is used in the MsgHeaderIterator to calculate proper addresses of the list elements.
 *
 * @par Example
 *
 * Code presenting minimal usage example.
 *
 * @code
    struct default_buffer_t {
        default_buffer_t()
        {
            el1.hdr.len = sizeof(nsapi_pktinfo_t);
            el2.len = sizeof(nsapi_msghdr_t);
            el3.len = sizeof(nsapi_msghdr_t);
            el4.hdr.len = sizeof(nsapi_pktinfo_t);
        }
        nsapi_pktinfo_t el1;
        nsapi_msghdr_t el2;
        nsapi_msghdr_t el3;
        nsapi_pktinfo_t el4;
    };

    default_buffer buff;
    nsapi_msghdr_t *hdr_p = reinterpret_cast<nsapi_msghdr_t *>(&buff);

    MsgHeaderIterator it(hdr_p, sizeof(buff));

    it.has_next() // returns true
    auto p1 = it.next()  // returns pointer to el1
    auto p2 = it.next()  // returns pointer to el2
    auto p3 = it.next()  // returns pointer to el3
    auto p4 = it.next()  // returns pointer to el4

    it.has_next() // returns false
     auto p5 = it.next() // returns nullptr
 * @endcode
 *
 * @note More usage examples are implemented in the MsgHeaderIterator unit test
 *  in netsocket/tests/UNITTESTS/NetworkStack/test_MsgHeaderIterator.cpp
 */

struct MsgHeaderIterator {
    /** Create a MsgHeaderIterator over given nsapi_msghdr_t list.
     *
     *  @param hdr  Pointer to the first list element.
     *  @param size Size of the whole list.
     */
    MsgHeaderIterator(nsapi_msghdr_t *hdr, nsapi_size_t size) :
        start(hdr),
        current(nullptr),
        size(size)
    {}

    /** Checks if the next address of the iterator is a valid list member.
     *
     *  @retval True if the next address is a valid member.
     *  @retval False otherwise.
     */
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

    /** Returns next element of the list.
     *
     *  @retval nullptr if the list doesn't contain next element.
     *  @retval Pointer to the next element otherwise.
     */
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

