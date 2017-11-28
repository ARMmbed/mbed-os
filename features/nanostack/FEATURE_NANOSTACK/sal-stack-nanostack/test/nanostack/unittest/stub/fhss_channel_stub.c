/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss_channel_stub.h"
#include "Service_Libs/fhss/fhss.h"

fhss_channel_stub_def fhss_channel_stub;

bool fhss_is_current_channel_broadcast(fhss_structure_t *fhss_structure)
{
    return fhss_channel_stub.channel_bool_value;
}

bool fhss_change_to_next_channel(fhss_structure_t *fhss_structure)
{
    return false;
}

uint8_t fhss_get_offset(fhss_structure_t *fhss_structure, const uint8_t *ptr)
{
        return 0;
}

int fhss_change_to_tx_channel(fhss_structure_t *fhss_structure, uint8_t *destination_address)
{
    return 0;
}

int fhss_change_to_parent_channel(fhss_structure_t *fhss_structure)
{
    return 0;
}

int fhss_change_to_rx_channel(fhss_structure_t *fhss_structure)
{
    return -1;
}
