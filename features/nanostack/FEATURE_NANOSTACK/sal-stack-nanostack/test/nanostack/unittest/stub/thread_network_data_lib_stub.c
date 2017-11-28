/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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
 * \file thread_network_data_lib.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "common_functions.h"
#include "ns_trace.h"

#define TRACE_GROUP "tndl"

uint8_t prefixBits_to_bytes(uint8_t prefixLenInBits)
{
    return 0;
}

int thread_network_data_malformed_check(uint8_t *network_data_ptr, uint16_t network_data_length)
{
    return 0;
}

int thread_network_data_sub_tlv_malformed_check(uint8_t *network_data_ptr, uint8_t network_data_length)
{
    return 0;
}


uint8_t *thread_nd_commission_data_write_steering_data(uint8_t *ptr, const uint8_t *steeringData, uint8_t length)
{
    return NULL;
}

uint8_t *thread_nd_commission_data_write_commissioning_credentials(uint8_t *ptr, char *crendentials, uint8_t length)
{
    return NULL;
}

uint8_t *thread_nd_network_data_border_router_tlv_write(uint8_t *ptr, uint16_t routerId, uint16_t flags)
{
    return NULL;
}

uint8_t *thread_nd_network_data_has_route_tlv_write(uint8_t *ptr, uint16_t routerId, uint8_t prf)
{
    return NULL;
}
