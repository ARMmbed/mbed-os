/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/*
 * \file icmpv6_prefix.h
 * \brief This module will allocate dynamic linked list for ICMPv6 prefix.
 *
 * API:
 *  * ADD / UPDATE, icmpv6_prefix_add()
 *  * Discover prefix from the list, icmpv6_prefix_compare()
 *  * Free Linked list, icmpv6_prefix_list_free()
 *
 */

#ifndef ICMPV6_PREFIX_DEFINITION_H_
#define ICMPV6_PREFIX_DEFINITION_H_
#include "ns_list.h"

/* Prefix Information option bits (RFC4861) */
#define PIO_L 0x80  /* On-link flag */
#define PIO_A 0x40  /* Autonomous address-configuration flag */
#define PIO_R 0x20  /* Router Address flag (RFC6275) */

typedef struct prefix_entry_t {
    uint8_t prefix_len;
    uint8_t options;
    uint32_t lifetime;
    uint32_t preftime;
    uint8_t prefix[16];
    ns_list_link_t link;
} prefix_entry_t;

typedef NS_LIST_HEAD(prefix_entry_t, link) prefix_list_t;

prefix_entry_t *icmpv6_prefix_add(prefix_list_t *list, const uint8_t *prefixPtr, uint8_t prefix_len, uint32_t lifeTime, uint32_t prefTime, uint8_t flags);

prefix_entry_t *icmpv6_prefix_compare(prefix_list_t *list, const uint8_t *addr, uint8_t prefix_len);

void icmpv6_prefix_list_free(prefix_list_t *list);

#endif /* ICMPV6_PREFIX_DEFINITION_H_ */
