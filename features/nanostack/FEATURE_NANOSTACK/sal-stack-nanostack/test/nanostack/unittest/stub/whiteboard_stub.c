/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include <stdint.h>
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "Common_Protocols/icmpv6.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "platform/os_whiteboard.h"

#ifdef WHITEBOARD

void whiteboard_init(int8_t id)
{
}

void whiteboard_init_by_prefix(int8_t id, const uint8_t address[static 8])
{
}

void whiteboard_rm_entry(int8_t id, const uint8_t address[static 16])
{
}

whiteboard_entry_t *whiteboard_table_check_address(const uint8_t address[static 16])
{
    return NULL;
}

whiteboard_entry_t *whiteboard_get(whiteboard_entry_t *cur)
{
    return NULL;
}

#ifndef EVOLUTION_VERSION
void whiteboard_ttl_max_size(uint16_t new_size_limit)
{
}
#endif

uint16_t whiteboard_size_get(void)
{
    return 0;
}

int8_t whiteboard_interface_register(const uint8_t address[static 16], int8_t nwk_id)
{
    return 0;
}

int8_t whiteboard_interface_unregister_all_address(int8_t nwk_id)
{
    return 0;
}

bool whiteboard_interface_address_cmp(const uint8_t address[static 16])
{
    return false;
}

whiteboard_entry_t *whiteboard_table_update(const uint8_t address[static 16], const uint8_t eui64[static 8], uint8_t *status)
{
    return NULL;
}

void whiteboard_ttl_update(uint16_t ttl_time)
{
}
#endif
