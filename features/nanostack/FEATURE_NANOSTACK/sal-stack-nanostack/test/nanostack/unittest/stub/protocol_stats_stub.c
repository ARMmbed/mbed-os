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
#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"

nwk_stats_t *NS_LARGE nwk_stats_ptr = 0;

void protocol_stats_init(void)
{
}

void protocol_stats_start(nwk_stats_t *stats_ptr)
{
}

void protocol_stats_stop(void)
{
}

void protocol_stats_reset(void)
{
}

void protocol_stats_update(nwk_stats_type_t type, uint16_t update_val)
{
}


