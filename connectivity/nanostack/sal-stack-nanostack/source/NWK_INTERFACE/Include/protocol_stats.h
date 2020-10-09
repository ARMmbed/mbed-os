/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#ifndef PROROCOL_STATS_H
#define PROROCOL_STATS_H

typedef enum {
    STATS_IP_RX_COUNT,
    STATS_IP_TX_COUNT,
    STATS_IP_ROUTE_UP,
    STATS_IP_RX_DROP,
    STATS_IP_NO_ROUTE,
    STATS_IP_CKSUM_ERROR,
    STATS_FRAG_RX_ERROR,
    STATS_FRAG_TX_ERROR,
    STATS_RPL_PARENT_CHANGE,
    STATS_RPL_ROUTELOOP,
    // RFC 6550 S18.5 stats
    STATS_RPL_MEMORY_OVERFLOW,
    STATS_RPL_PARENT_TX_FAIL,
    STATS_RPL_UNKNOWN_INSTANCE,
    STATS_RPL_LOCAL_REPAIR,
    STATS_RPL_GLOBAL_REPAIR,
    STATS_RPL_MALFORMED_MESSAGE,
    STATS_RPL_TIME_NO_NEXT_HOP,
    STATS_RPL_MEMORY_ALLOC,
    STATS_RPL_MEMORY_FREE,
    STATS_BUFFER_ALLOC,
    STATS_BUFFER_HEADROOM_REALLOC,
    STATS_BUFFER_HEADROOM_SHUFFLE,
    STATS_BUFFER_HEADROOM_FAIL,
    STATS_ETX_1ST_PARENT,
    STATS_ETX_2ND_PARENT,
    STATS_AL_TX_QUEUE_SIZE

} nwk_stats_type_t;


extern void protocol_stats_init(void);
extern void protocol_stats_update(nwk_stats_type_t type, uint16_t update_val);

#endif
