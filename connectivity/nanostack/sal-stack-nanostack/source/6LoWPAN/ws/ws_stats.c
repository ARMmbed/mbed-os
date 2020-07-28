/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_stats.h"
#include "6LoWPAN/ws/ws_common.h"
#include "ws_management_api.h"

#define TRACE_GROUP "wsst"

#ifdef HAVE_WS

int ws_statistics_start(int8_t interface_id, ws_statistics_t *stats_ptr)
{
    if (!stats_ptr) {
        return -1;
    }
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    cur->ws_info->stored_stats_ptr = stats_ptr;
    return 0;
}

int ws_statistics_stop(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !ws_info(cur)) {
        return -1;
    }
    cur->ws_info->stored_stats_ptr = NULL;
    return 0;
}

void ws_stats_update(protocol_interface_info_entry_t *cur, ws_stats_type_t type, uint32_t update_val)
{
    if (!cur || !ws_info(cur)) {
        return;
    }
    ws_statistics_t *stored_stats = cur->ws_info->stored_stats_ptr;

    if (stored_stats) {
        switch (type) {
            case STATS_WS_ASYNCH_TX:
                stored_stats->asynch_tx_count += update_val;
                break;
            case STATS_WS_ASYNCH_RX:
                stored_stats->asynch_rx_count += update_val;
                break;
        }
    }
}
#endif // HAVE_WS
