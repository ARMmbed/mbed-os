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

#ifndef WS_NEIGHBOR_CLASS_H_
#define WS_NEIGHBOR_CLASS_H_

#include "fhss_ws_extension.h"
#include "6LoWPAN/ws/ws_common_defines.h"

#define RSL_UNITITIALIZED 0x7fff

typedef struct ws_neighbor_class_entry {
    fhss_ws_neighbor_timing_info_t   fhss_data;
    uint16_t rsl_in;                                       /*!< RSL EWMA heard from neighbour*/
    uint16_t rsl_out;                                      /*!< RSL EWMA heard by neighbour*/
    uint16_t routing_cost;                                 /*!< ETX to border Router. */
    bool candidate_parent: 1;
    bool broadcast_timing_info_stored: 1;
    bool broadcast_shedule_info_stored: 1;
    bool synch_done : 1;
    bool accelerated_etx_probe : 1;
    bool negative_aro_send : 1;
    bool unicast_data_rx : 1;
} ws_neighbor_class_entry_t;

/**
 * Neighbor hopping info data base
 */
typedef struct ws_neighbor_class_s {
    ws_neighbor_class_entry_t *neigh_info_list;           /*!< Allocated hopping info array*/
    uint8_t list_size;                                    /*!< List size*/
} ws_neighbor_class_t;


/**
 * ws_neighbor_class_alloc a function for allocate giving list size
 *
 * \param class_data pointer to structure which will be initialized by this function
 * \param list_size define list size
 *
 * \return true Allocate Ok
 * \return false Allocate Fail
 *
 */
bool ws_neighbor_class_alloc(ws_neighbor_class_t *class_data, uint8_t list_size);

/**
 * ws_neighbor_class_dealloc a function for free allocated neighbor hopping info
 *
 * \param class_data pointer to structure which will be initialized by this function
 *
 */
void ws_neighbor_class_dealloc(ws_neighbor_class_t *class_data);

/**
 * ws_neighbor_class_entry_t a function for search hopping info for giving neighbor attribut
 *
 * \param class_data pointer to structure which will be initialized by this function
 * \param attribute_index define pointer to storage info
 *
 * \return NULL when Attribute is not correct
 * \return Pointer to neighbor hopping info
 *
 */
ws_neighbor_class_entry_t *ws_neighbor_class_entry_get(ws_neighbor_class_t *class_data, uint8_t attribute_index);

/**
 * ws_neighbor_class_entry_t a function for search hopping info for giving neighbor attribute index
 *
 * \param class_data pointer to structure which will be initialized by this function
 * \param entry which attribute index is counted.
 *
 * \return Attribute index of entry
 *
 */
uint8_t ws_neighbor_class_entry_index_get(ws_neighbor_class_t *class_data, ws_neighbor_class_entry_t *entry);

/**
 * ws_neighbor_class_entry_remove a function for clean information should be call when neighbor is removed
 *
 * \param class_data pointer to structure which will be initialized by this function
 * \param attribute_index define pointer to storage info
 *
 */
void ws_neighbor_class_entry_remove(ws_neighbor_class_t *class_data, uint8_t attribute_index);

/**
 * ws_neighbor_class_neighbor_unicast_time_info_update a function for update neighbor unicast time information
 *
 * \param ws_neighbor pointer to neighbor
 * \param ws_utt Unicast time IE data
 * \param timestamp timestamp for received data
 *
 */
void ws_neighbor_class_neighbor_unicast_time_info_update(ws_neighbor_class_entry_t *ws_neighbor, ws_utt_ie_t *ws_utt, uint32_t timestamp);

/**
 * ws_neighbor_class_neighbor_unicast_schedule_set a function for update neighbor unicast shedule information
 *
 * \param ws_neighbor pointer to neighbor
 * \param ws_us Unicast schedule IE data
 *
 */
void ws_neighbor_class_neighbor_unicast_schedule_set(ws_neighbor_class_entry_t *ws_neighbor, ws_us_ie_t *ws_us);


/**
 * ws_neighbor_class_neighbor_broadcast_time_info_update a function for update neighbor broadcast time information
 *
 * \param ws_neighbor pointer to neighbor
 * \param ws_bt_ie Broadcast time IE data
 * \param timestamp timestamp for received data
 *
 */
void ws_neighbor_class_neighbor_broadcast_time_info_update(ws_neighbor_class_entry_t *ws_neighbor, ws_bt_ie_t *ws_bt_ie, uint32_t timestamp);

/**
 * ws_neighbor_class_neighbor_broadcast_schedule_set a function for update neighbor broadcast shedule information
 *
 * \param ws_neighbor pointer to neighbor
 * \param ws_bs_ie Broadcast schedule IE data
 *
 */
void ws_neighbor_class_neighbor_broadcast_schedule_set(ws_neighbor_class_entry_t *ws_neighbor, ws_bs_ie_t *ws_bs_ie);

/**
 * ws_neighbor_class_rf_sensitivity_calculate
 *
 * Calculates minimum heard RSL value from all packets.
 * This will dynamically adjusts min sensitivity if value is not properly set
 *
 * \param rsl_heard; rsl_heard heard from Radio
 *
 */
void ws_neighbor_class_rf_sensitivity_calculate(uint8_t rsl_heard);

/**
 * ws_neighbor_class_rsl_from_dbm_calculate
 *
 * Calculates rsl value from dbm heard.
 * This provides a range of -174 (0) to +80 (254) dBm.
 *
 * \param dbm_heard; dbm heard from the neighbour
 *
 */
uint8_t ws_neighbor_class_rsl_from_dbm_calculate(int8_t dbm_heard);

/** Helper macros to read RSL values from neighbour class.
 *
 */
#define ws_neighbor_class_rsl_in_get(ws_neighbour) (ws_neighbour->rsl_in >> WS_RSL_SCALING)
#define ws_neighbor_class_rsl_out_get(ws_neighbour) (ws_neighbour->rsl_out >> WS_RSL_SCALING)

/**
 * ws_neighbor_class_neighbor_broadcast_schedule_set a function for update neighbor broadcast shedule information
 *
 * \param ws_neighbor pointer to neighbor
 * \param dbm_heard; dbm heard from the neighbour
 *
 */
void ws_neighbor_class_rsl_in_calculate(ws_neighbor_class_entry_t *ws_neighbor, int8_t dbm_heard);
/**
 * ws_neighbor_class_neighbor_broadcast_schedule_set a function for update neighbor broadcast shedule information
 *
 * \param ws_neighbor pointer to neighbor
 * \param rsl_reported; rsl value reported by neighbour in packet from RSL-IE
 *
 */
void ws_neighbor_class_rsl_out_calculate(ws_neighbor_class_entry_t *ws_neighbor, uint8_t rsl_reported);

#endif /* WS_NEIGHBOR_CLASS_H_ */
