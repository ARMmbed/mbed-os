/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#include "Service_Libs/fhss/channel_list.h"

#include "common_functions.h"
#include "ns_trace.h"
#include "channel_list_stub.h"

#include <stdint.h>
#include <string.h>

channel_list_stub_def channel_list_stub;

void channel_list_print(uint8_t dlevel, const char *grp, const uint32_t* list)
{
}

void channel_list_clear_mask(uint32_t* list)
{
}

int channel_list_get_first(const uint32_t* list)
{
    return 0;
}

uint8_t channel_list_get_channel(const uint32_t* list, int current_index)
{
    return channel_list_stub.uint8_value;
}

int channel_list_get_next(const uint32_t* list, int current_index)
{
    return 0;
}

int channel_list_get_next_broadcast(const uint32_t* list, int broadcast_channel_count, int current_index)
{
    return 0;
}

int channel_list_count_channels(const uint32_t* list)
{
    return channel_list_stub.uint8_value;
}

int channel_list_enable_channel(uint32_t* list, int channel_number)
{
    return 0;
}

bool channel_list_is_channel_enabled(const uint32_t* list, int channel_number)
{
    return false;
}

