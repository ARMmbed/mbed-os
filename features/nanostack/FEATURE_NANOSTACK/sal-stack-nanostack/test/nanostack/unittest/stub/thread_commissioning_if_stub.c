/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include <string.h>
#include <nsdynmemLIB.h>
#include <ns_list.h>
#include <net_thread_test.h>
#include "ns_trace.h"
#include "Core/include/ns_buffer.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"

#include "net_interface.h" // TODO remove when fixing properly
#include "thread_commissioning_if.h"
#include "randLIB.h"

#define TRACE_GROUP "thrC"

int thread_commissioning_if_steering_data_set(int8_t interface_id, uint8_t *buf_ptr, uint16_t length)
{
    return 0;
}

int thread_commissioning_if_register(int8_t interface_id, uint8_t border_router_address[static 16], uint8_t *commissioner_id_ptr, uint16_t commissioner_id_len, uint16_t *session_id)
{
    return 0;
}

bool thread_commissioning_if_session_refresh(int8_t interface_id, uint16_t session_id)
{
    return true;
}

int thread_commissioning_if_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, uint8_t eui64[8], uint8_t key[16])
{
    return 0;
}

bool thread_commissioning_if_joiner_port_set(int8_t interface_id, uint16_t joiner_udp_port)
{
    return true;
}

int thread_commissioning_if_pairwise_key_del(int8_t interface_id, uint8_t eui64[static 8])
{
    return 0;
}

int thread_commissioning_if_pairwise_key_delete_all(int8_t interface_id)
{
    return 0;
}

int thread_commissioning_if_border_router_locator_get(int8_t interface_id, uint8_t *address_ptr)
{
    return 0;
}

int thread_commissioning_if_enable_security(int8_t interface_id)
{
    return 0;
}

char* thread_commissioning_if_commissioner_id_get(int8_t interface_id)
{
    return NULL;
}
bool thread_commissioning_if_unregister(int8_t interface_id, uint16_t session_id)
{
    return true;
}
bool thread_commissioning_if_address_changed(int8_t interface_id, uint8_t border_router_address[static 16])
{
    return true;
}

