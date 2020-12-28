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

/**
 * \file fhss_ws_extension.h
 * \brief
 */

#ifndef FHSS_WS_EXT_H
#define FHSS_WS_EXT_H

#include "ns_types.h"
#include "fhss_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief ws_channel_mask_t WS neighbour supported channel mask
 */
typedef struct ws_channel_mask {
    uint16_t channel_count;                     /**<active channels at mask */
    uint32_t channel_mask[8];                   /**< Supported channels */
} ws_channel_mask_t;

/**
 * @brief unicast_timing_info Unicast timing/hopping schedule information structure.
 */
typedef struct unicast_timing_info {
    unsigned unicast_channel_function: 3;       /**< Unicast schedule channel function */
    uint8_t unicast_dwell_interval;             /**< Unicast dwell interval */
    uint16_t unicast_number_of_channels;        /**< Unicast number of channels */
    uint16_t fixed_channel;                     /**< Unicast fixed channel*/
    uint_fast24_t ufsi;                         /**< Unicast fractional sequence interval */
    uint32_t utt_rx_timestamp;                  /**< UTT-IE reception timestamp */
} unicast_timing_info_t;

/**
 * @brief broadcast_timing_info Broadcast timing/hopping schedule information structure.
 */
typedef struct broadcast_timing_info {
    unsigned broadcast_channel_function: 3;     /**< Broadcast schedule channel function */
    uint8_t broadcast_dwell_interval;           /**< Broadcast dwell interval */
    uint16_t fixed_channel;                     /**< Broadcast fixed channel*/
    uint16_t broadcast_slot;                    /**< Broadcast slot number */
    uint16_t broadcast_schedule_id;             /**< Broadcast schedule identifier */
    uint_fast24_t broadcast_interval_offset;    /**< Broadcast interval offset */
    uint32_t broadcast_interval;                /**< Broadcast interval */
    uint32_t bt_rx_timestamp;                   /**< BT-IE reception timestamp */
} broadcast_timing_info_t;

/**
 * @brief fhss_ws_neighbor_timing_info Neighbor timing/hopping schedule information structure.
 */
typedef struct fhss_ws_neighbor_timing_info {
    uint8_t clock_drift;                        /**< Neighbor clock drift */
    uint8_t timing_accuracy;                    /**< Neighbor timing accuracy */
    unicast_timing_info_t uc_timing_info;       /**< Neighbor unicast timing info */
    broadcast_timing_info_t bc_timing_info;     /**< Neighbor broadcast timing info */
    ws_channel_mask_t uc_channel_list;          /**< Neighbor Unicast channel list */
    uint32_t *excluded_channels;                /**< Neighbor excluded channels (bit mask) */
} fhss_ws_neighbor_timing_info_t;

/**
 * @brief Get neighbor timing/hopping schedule.
 * @param api FHSS instance.
 * @param eui64 EUI-64 address of node for which the info is requested.
 * @return Neighbor timing/hopping schedule.
 */
typedef fhss_ws_neighbor_timing_info_t *fhss_get_neighbor_info(const fhss_api_t *api, uint8_t eui64[8]);

/**
 * @brief Set parent which broadcast channels must be listened by FHSS.
 * @param fhss_api FHSS instance.
 * @param eui64 EUI-64 address of parent.
 * @param bc_timing_info Pointer to parent broadcast timing/hopping schedule info.
 * @param force_synch If false, synchronization is done only if minimum (internal) synchronization interval is exceed.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_ws_set_parent(const fhss_api_t *fhss_api, const uint8_t eui64[8], const broadcast_timing_info_t *bc_timing_info, const bool force_synch);

/**
 * @brief Remove parent which was set by ns_fhss_ws_set_parent function.
 * @param fhss_api FHSS instance.
 * @param eui64 EUI-64 address of parent.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_ws_remove_parent(const fhss_api_t *fhss_api, const uint8_t eui64[8]);

/**
 * @brief Set neighbor timing/hopping schedule request function.
 * @param fhss_api FHSS instance.
 * @param get_neighbor_info Neighbor info function pointer.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_set_neighbor_info_fp(const fhss_api_t *fhss_api, fhss_get_neighbor_info *get_neighbor_info);

/**
 * @brief Set node hop count. Hop count is used to specify TX/RX slot. When hop count is set to 0xFF, TX/RX slots are ignored.
 * @param fhss_api FHSS instance.
 * @param hop_count Hop count to be set.
 * @return 0 on success, -1 on fail.
 */
extern int ns_fhss_ws_set_hop_count(const fhss_api_t *fhss_api, const uint8_t hop_count);

#ifdef __cplusplus
}
#endif

#endif // FHSS_WS_EXT_H
