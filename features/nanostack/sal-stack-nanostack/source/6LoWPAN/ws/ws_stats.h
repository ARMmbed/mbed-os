/*
 * Copyright (c) 2018-2019, 2021, Pelion and affiliates.
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

#ifndef WS_STATS_H_
#define WS_STATS_H_

#ifdef HAVE_WS

typedef enum {
    STATS_WS_ASYNCH_TX,
    STATS_WS_ASYNCH_RX,
    STATS_WS_ASYNCH_RX_PAS,
    STATS_WS_ASYNCH_RX_PA,
    STATS_WS_ASYNCH_RX_PCS,
    STATS_WS_ASYNCH_RX_PC,
    STATS_WS_ASYNCH_TX_PAS,
    STATS_WS_ASYNCH_TX_PA,
    STATS_WS_ASYNCH_TX_PCS,
    STATS_WS_ASYNCH_TX_PC,
    STATS_WS_STATE_1,
    STATS_WS_STATE_2,
    STATS_WS_STATE_3,
    STATS_WS_STATE_4,
    STATS_WS_STATE_5,
    STATS_WS_NEIGHBOUR_ADD,
    STATS_WS_NEIGHBOUR_REMOVE,
    STATS_WS_CHILD_ADD,
    STATS_WS_CHILD_REMOVE,
} ws_stats_type_t;

void ws_stats_update(protocol_interface_info_entry_t *cur, ws_stats_type_t type, uint32_t update_val);

#endif // HAVE_WS

#endif // WS_STATS_H_
