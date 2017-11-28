/*
 * Copyright (c) 2011-2016, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "ns_trace.h"
#include "string.h"
#include "Core/include/address.h"
#include "Core/include/ns_buffer.h"
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "Common_Protocols/ipv6.h"
#include "common_functions.h"

#define TRACE_GROUP_ROUTING_TABLE "crou"

#if ADDRESS_SIZE == 8
#if MAX_HOP_CNT > 30
#error "DO not set MAX_HOP_CNT over 30 reason by 8-bit size resolution"
#endif
#endif

uint8_t *gen_source_route_set(uint8_t *ptr)
{
    return NULL;
}

void gen_routing_table_init(void)
{
}

int8_t gen_source_route_allocate(uint8_t alloc)
{
    return -1;
}

int8_t gen_route_exist_check(const uint8_t ipv6_address_ptr[16])
{
    return -1;
}

void gen_routing_ttl_update(uint8_t id)
{
}

void gen_routing_ttl_set_low_by_repair(uint8_t id)
{
}

void gen_route_remove_by_rpl_instance(uint8_t id)
{
}


void gen_neighbor_ttl_update(uint8_t ttl)
{
}

void routing_table_print(void)
{
}

