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
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "randLIB.h"

#include "thread_common.h"
#include "thread_config.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "Core/include/address.h"

void thread_address_registration_init()
{
}

void thread_address_registration_deinit()
{
}

void thread_address_registration_send_proactive_an(struct protocol_interface_info_entry *interface, const uint8_t *addr, const uint8_t *child_mac64)
{
    (void)interface;
    (void)addr;
    (void)child_mac64;
}

void thread_address_registration_mcast_change(protocol_interface_info_entry_t *interface, if_group_entry_t *group, bool added)
{
    (void)interface;
    (void)group;
    (void)added;
}
void thread_address_registration_timer_set(protocol_interface_info_entry_t *interface, uint16_t seconds)
{
    (void)interface;
    (void)seconds;
}

void thread_address_registration_timer(protocol_interface_info_entry_t *interface, uint16_t seconds)
{
    (void)interface;
    (void)seconds;
}
