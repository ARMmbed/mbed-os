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
#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"

nwk_stats_t *NS_LARGE nwk_stats_ptr = 0;

void protocol_stats_init(void)
{
    nwk_stats_ptr = 0;
    //Init timers
}
void protocol_stats_start(nwk_stats_t *stats_ptr) //Enable stats collection
{
    nwk_stats_ptr = stats_ptr;
}
void protocol_stats_stop(void)//Clean structure pointer
{
    nwk_stats_ptr = 0;
}
void protocol_stats_reset(void) //Clean all entrys to zero
{
    if (nwk_stats_ptr) {
        memset(nwk_stats_ptr, 0, sizeof(nwk_stats_t));
    }
}

void protocol_stats_update(nwk_stats_type_t type, uint16_t update_val)
{
    if (nwk_stats_ptr) {
        switch (type) {
            case STATS_IP_RX_COUNT: //RX Payload
                nwk_stats_ptr->ip_rx_count++;
                nwk_stats_ptr->ip_rx_bytes += update_val;
                break;

            case STATS_IP_ROUTE_UP:
                nwk_stats_ptr->ip_routed_up += update_val;
            /* fall through */
            case STATS_IP_TX_COUNT:
                nwk_stats_ptr->ip_tx_count++;
                nwk_stats_ptr->ip_tx_bytes += update_val;
                break;

            case STATS_IP_RX_DROP:
                nwk_stats_ptr->ip_rx_drop++;
                break;

            case STATS_IP_CKSUM_ERROR:
                nwk_stats_ptr->ip_cksum_error++;
                break;

            case STATS_FRAG_RX_ERROR:
                nwk_stats_ptr->frag_rx_errors++;
                break;

            case STATS_FRAG_TX_ERROR:
                nwk_stats_ptr->frag_tx_errors++;
                break;

            case STATS_RPL_PARENT_CHANGE:
                nwk_stats_ptr->rpl_route_routecost_better_change++;
                break;

            case STATS_RPL_ROUTELOOP:
                nwk_stats_ptr->ip_routeloop_detect++;
                break;

            case STATS_IP_NO_ROUTE:
                nwk_stats_ptr->ip_no_route++;
                break;

            case STATS_RPL_MEMORY_OVERFLOW:
                nwk_stats_ptr->rpl_memory_overflow += update_val;
                break;

            case STATS_RPL_PARENT_TX_FAIL:
                nwk_stats_ptr->rpl_parent_tx_fail += update_val;
                break;

            case STATS_RPL_UNKNOWN_INSTANCE:
                nwk_stats_ptr->rpl_unknown_instance += update_val;
                break;

            case STATS_RPL_LOCAL_REPAIR:
                nwk_stats_ptr->rpl_local_repair += update_val;
                break;

            case STATS_RPL_GLOBAL_REPAIR:
                nwk_stats_ptr->rpl_global_repair += update_val;
                break;

            case STATS_RPL_MALFORMED_MESSAGE:
                nwk_stats_ptr->rpl_malformed_message += update_val;
                break;

            case STATS_RPL_TIME_NO_NEXT_HOP:
                nwk_stats_ptr->rpl_time_no_next_hop += update_val;
                break;

            case STATS_RPL_MEMORY_ALLOC:
                nwk_stats_ptr->rpl_total_memory += update_val;
                break;

            case STATS_RPL_MEMORY_FREE:
                nwk_stats_ptr->rpl_total_memory -= update_val;
                break;

            case STATS_BUFFER_ALLOC:
                nwk_stats_ptr->buf_alloc++;
                break;

            case STATS_BUFFER_HEADROOM_REALLOC:
                nwk_stats_ptr->buf_headroom_realloc++;
                break;

            case STATS_BUFFER_HEADROOM_SHUFFLE:
                nwk_stats_ptr->buf_headroom_shuffle++;
                break;

            case STATS_BUFFER_HEADROOM_FAIL:
                nwk_stats_ptr->buf_headroom_fail++;
                break;

            case STATS_ETX_1ST_PARENT:
                nwk_stats_ptr->etx_1st_parent = update_val;
                break;

            case STATS_ETX_2ND_PARENT:
                nwk_stats_ptr->etx_2nd_parent = update_val;
                break;
        }
    }
}


