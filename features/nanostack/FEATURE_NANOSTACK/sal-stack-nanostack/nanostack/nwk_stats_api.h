/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#ifndef NWK_STATS_API_H
#define NWK_STATS_API_H
/**
 * \file nwk_stats_api.h
 * \brief 6LowPAN library network stats API
 *
 * - protocol_stats_start(), Enable stats update.
 * - protocol_stats_stop(), Stop stack stats update.
 * - protocol_stats_reset(), Reset all stats information to null.
 *
 *
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * /struct nwk_stats_t
 * /brief Struct for network stats buffer structure.
 */
typedef struct nwk_stats_t {
    /* 6Lowpan */
    uint32_t ip_rx_count;           /**< IP RX packet count. */
    uint32_t ip_tx_count;           /**< IP TX packet count. */
    uint32_t ip_rx_drop;            /**< IP RX packet drops count. */
    uint32_t ip_cksum_error;        /**< IP checksum error count. */
    /* IP Payload Flow */
    uint32_t ip_tx_bytes;           /**< IP TX bytes count. */
    uint32_t ip_rx_bytes;           /**< IP RX bytes count. */
    uint32_t ip_routed_up;          /**< IP routed UP bytes count. */
    uint32_t ip_no_route;           /**< IP no route count. */
    /* Fragments */
    uint32_t frag_rx_errors;        /**< Fragmentation RX error count. */
    uint32_t frag_tx_errors;        /**< Fragmentation TX error count. */
    /*RPL stats*/
    uint32_t rpl_route_routecost_better_change; /**< RPL parent change count. */
    uint32_t ip_routeloop_detect;               /**< RPL route loop detection count. */
    uint32_t rpl_memory_overflow;   /**< RPL memory overflow count. */
    uint32_t rpl_parent_tx_fail;    /**< RPL transmit errors to DODAG parents. */
    uint32_t rpl_unknown_instance;  /**< RPL unknown instance ID count. */
    uint32_t rpl_local_repair;      /**< RPL local repair count. */
    uint32_t rpl_global_repair;     /**< RPL global repair count. */
    uint32_t rpl_malformed_message; /**< RPL malformed message count. */
    uint32_t rpl_time_no_next_hop;  /**< RPL seconds without a next hop. */
    uint32_t rpl_total_memory;      /**< RPL current memory usage total. */
    /* Buffers */
    uint32_t buf_alloc;             /**< Buffer allocation count. */
    uint32_t buf_headroom_realloc;  /**< Buffer headroom realloc count. */
    uint32_t buf_headroom_shuffle;  /**< Buffer headroom shuffle count. */
    uint32_t buf_headroom_fail;     /**< Buffer headroom failure count. */
    /* ETX */
    uint16_t etx_1st_parent;        /**< Primary parent ETX. */
    uint16_t etx_2nd_parent;        /**< Secondary parent ETX. */

} nwk_stats_t;

/**
  *  \brief Enable stats collection.
  *
  * \param stats_ptr A static pointer to stack update.
  *
  */
extern void protocol_stats_start(nwk_stats_t *stats_ptr);
/**
  *  \brief Disable stats collection.
  *
  */
extern void protocol_stats_stop(void);
/**
  *  \brief Reset stats info fiels.
  *
  */
extern void protocol_stats_reset(void);
#ifdef __cplusplus
}
#endif
#endif
