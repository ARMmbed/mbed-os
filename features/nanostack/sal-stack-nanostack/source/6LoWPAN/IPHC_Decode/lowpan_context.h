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
 * \file lowpan_context.h
 * \brief This header define API how contexts are saved and listed at interface side.
 *
 * API:
 *  * Add/Update context, lowpan_context_update()
 *  * Delete full list Context, lowpan_context_list_free()
 *  * Timeout update, protocol_6lowpan_context_timer()
 *
 */

#ifndef LOWPAN_CONTEXT_DEFINE_H_
#define LOWPAN_CONTEXT_DEFINE_H_
#include "ns_list.h"

/* Flags for decode or encode to context's information to 8-bit data from message or to message (cid_flags)*/
#define LOWPAN_CONTEXT_C        0x10        // Compression
#define LOWPAN_CONTEXT_CID_MASK 0x0F        // Context ID MASK
#define LOWPAN_MAX_CONTEXT_COUNT 16

typedef struct lowpan_context {
    uint32_t lifetime;      // Remaining lifetime (100ms ticks)
    unsigned length: 8;     // Context length
    unsigned cid: 4;        // Context Identifier
    bool    compression: 1; // C (Compression) flag
    bool    expiring: 1;    // True if main lifetime expired, pending deletion
    bool    stable: 1;      // Thread stable network data (always true if not Thread)
    uint8_t prefix[16];     // Context prefix
    ns_list_link_t link;
} lowpan_context_t;

typedef NS_LIST_HEAD(lowpan_context_t, link) lowpan_context_list_t;

/**
 * \brief Update lowpan current context or add new one
 *
 * \param list pointer to linked list for context
 * \param cid_flags Define context id (LOWPAN_CONTEXT_CID_MASK) and Context compression mode (LOWPAN_CONTEXT_C)
 * \param lifetime in minutes for context 0 delete contexts
 * \param prefix pointer to context prefix
 * \param len prefin length in bits
 *
 * \return 0 Update OK
 * \return -2 Update fail Out of memory reason
 */
int_fast8_t lowpan_context_update(lowpan_context_list_t *list, uint8_t cid_flags, uint16_t lifetime, const uint8_t *prefix, uint_fast8_t len, bool stable);

/**
 * \brief Cleand free full linked list about context
 *
 * \param list pointer to linked list for context
 *
 */
void lowpan_context_list_free(lowpan_context_list_t *list);

/**
 * \brief Update lowpan context timeout ticks
 *
 * \param list pointer to linked list for context
 * \param ticks is in 1/10s
 *
 */
void lowpan_context_timer(lowpan_context_list_t *list, uint_fast16_t ticks);
/**
 * \brief Get Context entry from the list by context ID
 *
 * \param list pointer to linked list for context
 * \param id Define 4-bit context id LOWPAN_CONTEXT_CID_MASK
 *
 * \return >0 Pointer to Entry
 * \return NULL Not supported Context ID
 *
 */
lowpan_context_t *lowpan_context_get_by_id(const lowpan_context_list_t *list, uint8_t id);

/**
 * \brief Get Longest match Context entry from the list for given IPv6 address
 *
 * \param list pointer to linked list for context
 * \param ipv6Address pointer to
 *
 * \return >0 Pointer to Entry
 * \return NULL Not supported Context for this address
 *
 */
lowpan_context_t *lowpan_context_get_by_address(const lowpan_context_list_t *list, const uint8_t *ipv6Address);

#endif /* LOWPAN_CONTEXT_DEFINE_H_ */
