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

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "Service_Libs/pan_blacklist/pan_blacklist_api.h"

void pan_blacklist_cache_init(pan_blaclist_cache_s *blacklist_cache)
{

}

void pan_coordinator_blacklist_cache_init(pan_coordinator_blaclist_cache_s *blacklist_cache)
{

}

void pan_blacklist_pan_set(pan_blaclist_cache_s *list_ptr, uint16_t panid, uint16_t timeout)
{

}

void pan_blacklist_time_update(pan_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds)
{

}

void pan_coordinator_blacklist_time_update(pan_coordinator_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds)
{

}

bool pan_blacklist_filter(pan_blaclist_cache_s *list_ptr, uint16_t panid)
{

	return false;
}

bool pan_cordinator_blacklist_filter(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *compare_data)
{
    return false;
}

void pan_cordinator_blacklist_pan_set(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *cordinator_data, uint16_t timeout)
{

}
