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

#ifndef BLACKLIST_H_
#define BLACKLIST_H_

#define BLACKLIST_DEFAULT_TIMER_TIMEOUT           4
#define BLACKLIST_DEFAULT_TIMER_MAX_TIMEOUT       40
#define BLACKLIST_DEFAULT_ENTRY_LIFETIME          8
#define BLACKLIST_DEFAULT_ENTRY_MAX_NBR           15
#define BLACKLIST_DEFAULT_PURGE_NBR               3
#define BLACKLIST_DEFAULT_PURGE_TIMER_TIMEOUT     6

typedef struct blacklist_entry {
    uint8_t               eui64[8];
    uint16_t              ttl;        // time to live in is MLE advertisement cycles
    uint16_t              interval;   // current interval in MLE advertisement cycles
    ns_list_link_t        link;
} blacklist_entry_t;

typedef NS_LIST_HEAD(blacklist_entry_t, link) blacklist_list_t;

typedef struct blacklist_data {
    blacklist_list_t  blacklist;            // Blacklist entries
    uint16_t blacklist_purge_ttl;           // Blacklist purge ttl
    uint16_t blacklist_entry_lifetime;      // Base lifetime value
    uint16_t blacklist_timer_max_timeout;   // Max. timeout value
    uint16_t blacklist_timer_timeout;       // Initial timeout value
    uint16_t blacklist_entry_max_nbr;       // Max. blacklist entries
    uint16_t blacklist_purge_nbr;           // # of entries to purge
    uint16_t blacklist_purge_timer_timeout; // Timeout for purge timer
} blacklist_data_t;

uint8_t blacklist_init(void);
void blacklist_params_set(uint16_t entry_lifetime, uint16_t timer_max_timeout, uint16_t timer_timeout, uint16_t entry_max_nbr, uint16_t purge_nbr, uint16_t purge_timer_timeout);
bool blacklist_reject(const uint8_t *ll64_address);
void blacklist_update(const uint8_t *ll64_address, bool success);
void blacklist_ttl_update(uint16_t ticks);
void blacklist_clear(void);
void blacklist_free(void);

#endif /* BLACKLIST_H_ */
