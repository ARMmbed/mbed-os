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

#ifndef SERVICE_LIBS_PAN_BLACKLIST_PAN_BLACKLIST_API_H_
#define SERVICE_LIBS_PAN_BLACKLIST_PAN_BLACKLIST_API_H_
#include "ns_list.h"
typedef struct pan_blaclist_entry {
    uint16_t pan_id;
    uint16_t timeout_in_seconds;
    ns_list_link_t link;
} pan_blaclist_entry_t;

typedef struct pan_coordinator_blacklist_entry {
    uint8_t coordinator_pan_address[10];
    uint16_t timeout_in_seconds;
    ns_list_link_t link;
} pan_coordinator_blacklist_entry_t;

/*!
 * \struct pan_blaclist_cache_s
 *
 * \brief Top-level pan blacklist cache descriptor.
 *
 * This passed by reference to all APIs. Users should not manipulate contents.
 */
typedef struct pan_blaclist_cache_s {
    NS_LIST_HEAD(pan_blaclist_entry_t, link) head;
} pan_blaclist_cache_s;

/*!
 * \struct pan_coordinator_blaclist_cache_s
 *
 * \brief Top-level pan coordinator blacklist cache descriptor.
 *
 * This passed by reference to all APIs. Users should not manipulate contents.
 */
typedef struct pan_coordinator_blacklist_cache_s {
    NS_LIST_HEAD(pan_coordinator_blacklist_entry_t, link) head;
} pan_coordinator_blaclist_cache_s;


void pan_blacklist_cache_init(pan_blaclist_cache_s *blacklist_cache);

void pan_coordinator_blacklist_cache_init(pan_coordinator_blaclist_cache_s *blacklist_cache);

void pan_coordinator_blacklist_free(pan_coordinator_blaclist_cache_s *list_ptr);

void pan_blacklist_pan_set(pan_blaclist_cache_s *list_ptr, uint16_t panid, uint16_t timeout);

void pan_cordinator_blacklist_pan_set(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *cordinator_data, uint16_t timeout);

void pan_blacklist_time_update(pan_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds);

void pan_coordinator_blacklist_time_update(pan_coordinator_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds);

bool pan_blacklist_filter(pan_blaclist_cache_s *list_ptr, uint16_t panid);

bool pan_cordinator_blacklist_filter(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *compare_data);

#endif /* SERVICE_LIBS_PAN_BLACKLIST_PAN_BLACKLIST_API_H_ */
