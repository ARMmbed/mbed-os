/*
 * Copyright (c) 2013-2016, Arm Limited and affiliates.
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
#include "debug.h"
#include "string.h"
#include "Security/PANA/pana_relay_table.h"
#include "nsdynmemLIB.h"

#ifdef PANA

void pana_relay_table_init(void)
{
}


relay_table_entry_t *pana_relay_table_update(uint8_t *address)
{
    return NULL;
}

relay_table_entry_t *pana_relay_detect(uint8_t *address)
{
    return NULL;
}


void pana_relay_ttl_update(uint16_t ttl_time)
{
}
#endif
