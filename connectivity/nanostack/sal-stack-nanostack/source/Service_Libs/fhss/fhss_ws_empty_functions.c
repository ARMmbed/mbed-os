/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_common.h"
#include "channel_list.h"
#include "channel_functions.h"
#include "fhss_ws.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "eventOS_callback_timer.h"
#include "randLIB.h"
#include <string.h>

#ifndef HAVE_WS

fhss_structure_t *fhss_ws_enable(fhss_api_t *fhss_api, const fhss_ws_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer)
{
    (void) fhss_api;
    (void) fhss_configuration;
    (void) fhss_timer;

    return NULL;
}

int fhss_ws_set_callbacks(fhss_structure_t *fhss_structure)
{
    (void) fhss_structure;
    return -1;
}

int fhss_ws_set_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8], const broadcast_timing_info_t *bc_timing_info, const bool force_synch)
{
    (void) fhss_structure;
    (void) eui64;
    (void) bc_timing_info;
    (void) force_synch;

    return -1;
}

int fhss_ws_remove_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8])
{
    (void) fhss_structure;
    (void) eui64;

    return -1;
}

int fhss_ws_configuration_set(fhss_structure_t *fhss_structure, const fhss_ws_configuration_t *fhss_configuration)
{
    (void) fhss_structure;
    (void) fhss_configuration;

    return -1;
}

int fhss_ws_set_hop_count(fhss_structure_t *fhss_structure, const uint8_t hop_count)
{
    (void) fhss_structure;
    (void) hop_count;

    return -1;
}

#endif // HAVE_WS

