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
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "eventOS_scheduler.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/blacklist/blacklist.h"

#define TRACE_GROUP "bl"

uint8_t blacklist_init(void)
{
    return 0;
}

void blacklist_params_set(uint16_t entry_lifetime, uint16_t timer_max_timeout, uint16_t timer_timeout, uint16_t entry_max_nbr, uint16_t purge_nbr, uint16_t purge_timer_timeout)
{

}

bool blacklist_reject(const uint8_t *ll64_address)
{
    return false;
}

void blacklist_update(const uint8_t *ll64_address, bool success)
{

}

void blacklist_ttl_update(uint16_t ticks)
{

}

void blacklist_clear(void)
{

}
