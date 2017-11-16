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
#include "ns_types.h"

#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/mld.h"

buffer_t *mld_build(protocol_interface_info_entry_t *cur, uint8_t type, uint16_t max_response_delay, const uint8_t address[static 16])
{
    return NULL;
}

buffer_t *mld_query_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

buffer_t *mld_report_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

void mld_start_listening(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
}

void mld_stop_listening(protocol_interface_info_entry_t *interface, if_group_entry_t *entry)
{
}

void mld_slow_timer(protocol_interface_info_entry_t *interface, uint_fast16_t seconds)
{
}

void mld_fast_timer(protocol_interface_info_entry_t *interface, uint_fast16_t ticks)
{
}
