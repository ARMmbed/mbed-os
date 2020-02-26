/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_common.h"
#include "fhss_ws.h"
#include "fhss_statistics.h"
#include "fhss_channel.h"
#include "channel_list.h"
#include <string.h>

#define TRACE_GROUP "fhta"

int8_t fhss_set_optimal_packet_length(const fhss_api_t *fhss_api, uint16_t packet_length)
{
    (void) fhss_api;
    (void) packet_length;
#ifdef HAVE_WS
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return -1;
    }
    fhss_structure->optimal_packet_length = packet_length;
    fhss_set_txrx_slot_length(fhss_structure);
    tr_debug("Setting FHSS optimal packet length to: %u", fhss_structure->optimal_packet_length);
#endif
    return 0;
}
