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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "6LoWPAN/ws/ws_common.h"
#include "ws_management_api.h"

#ifdef HAVE_WS


#define TRACE_GROUP "wsne"


bool ws_neighbor_class_alloc(ws_neighbor_class_t *class_data, uint8_t list_size)
{

    class_data->neigh_info_list = ns_dyn_mem_alloc(sizeof(ws_neighbor_class_entry_t) * list_size);
    if (!class_data->neigh_info_list) {
        return false;
    }

    class_data->list_size = list_size;
    ws_neighbor_class_entry_t *list_ptr = class_data->neigh_info_list;
    for (uint8_t i = 0; i < list_size; i++) {
        memset(list_ptr, 0, sizeof(ws_neighbor_class_entry_t));
        list_ptr->rsl_in = RSL_UNITITIALIZED;
        list_ptr->rsl_out = RSL_UNITITIALIZED;
        list_ptr++;
    }
    return true;
}


void ws_neighbor_class_dealloc(ws_neighbor_class_t *class_data)
{
    ns_dyn_mem_free(class_data->neigh_info_list);
    class_data->neigh_info_list = NULL;
    class_data->list_size = 0;
}

ws_neighbor_class_entry_t *ws_neighbor_class_entry_get(ws_neighbor_class_t *class_data, uint8_t attribute_index)
{
    if (!class_data->neigh_info_list || attribute_index >= class_data->list_size) {
        return NULL;
    }

    ws_neighbor_class_entry_t *entry = class_data->neigh_info_list + attribute_index;
    return entry;
}

uint8_t ws_neighbor_class_entry_index_get(ws_neighbor_class_t *class_data, ws_neighbor_class_entry_t *entry)
{
    if (!class_data->neigh_info_list) {
        return 0xff;
    }
    return entry - class_data->neigh_info_list;
}

void ws_neighbor_class_entry_remove(ws_neighbor_class_t *class_data, uint8_t attribute_index)
{
    ws_neighbor_class_entry_t *entry = ws_neighbor_class_entry_get(class_data, attribute_index);
    if (entry) {
        memset(entry, 0, sizeof(ws_neighbor_class_entry_t));
        entry->rsl_in = RSL_UNITITIALIZED;
        entry->rsl_out = RSL_UNITITIALIZED;
    }
}

void ws_neighbor_class_neighbor_unicast_time_info_update(ws_neighbor_class_entry_t *ws_neighbor, ws_utt_ie_t *ws_utt, uint32_t timestamp)
{
    ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp = timestamp;
    ws_neighbor->fhss_data.uc_timing_info.ufsi = ws_utt->ufsi;
}

void ws_neighbor_class_neighbor_unicast_schedule_set(ws_neighbor_class_entry_t *ws_neighbor, ws_us_ie_t *ws_us)
{
    ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function = ws_us->channel_function;
    if (ws_us->channel_function == WS_FIXED_CHANNEL) {
        ws_neighbor->fhss_data.uc_timing_info.fixed_channel = ws_us->function.zero.fixed_channel;
        ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = 1;
    } else {
        if (ws_us->channel_plan == 0) {
            ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = ws_common_channel_number_calc(ws_us->plan.zero.regulator_domain, ws_us->plan.zero.operation_class);
        } else if (ws_us->channel_plan == 1) {
            ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = ws_us->plan.one.number_of_channel;
        } else {
            ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = 0;
        }
    }
    ws_neighbor->fhss_data.uc_timing_info.unicast_dwell_interval = ws_us->dwell_interval;
}


void ws_neighbor_class_neighbor_broadcast_time_info_update(ws_neighbor_class_entry_t *ws_neighbor, ws_bt_ie_t *ws_bt_ie, uint32_t timestamp)
{
    ws_neighbor->broadcast_timing_info_stored = true;
    ws_neighbor->fhss_data.bc_timing_info.bt_rx_timestamp = timestamp;
    ws_neighbor->fhss_data.bc_timing_info.broadcast_slot = ws_bt_ie->broadcast_slot_number;
    ws_neighbor->fhss_data.bc_timing_info.broadcast_interval_offset = ws_bt_ie->broadcast_interval_offset;
}


void ws_neighbor_class_neighbor_broadcast_schedule_set(ws_neighbor_class_entry_t *ws_neighbor, ws_bs_ie_t *ws_bs_ie)
{
    ws_neighbor->broadcast_shedule_info_stored = true;
    ws_neighbor->fhss_data.bc_timing_info.broadcast_channel_function = ws_bs_ie->channel_function;
    if (ws_bs_ie->channel_function == WS_FIXED_CHANNEL) {
        ws_neighbor->fhss_data.bc_timing_info.fixed_channel = ws_bs_ie->function.zero.fixed_channel;
    }
    ws_neighbor->fhss_data.bc_timing_info.broadcast_dwell_interval = ws_bs_ie->dwell_interval;
    ws_neighbor->fhss_data.bc_timing_info.broadcast_interval = ws_bs_ie->broadcast_interval;
    ws_neighbor->fhss_data.bc_timing_info.broadcast_schedule_id = ws_bs_ie->broadcast_schedule_identifier;
}

void ws_neighbor_class_rf_sensitivity_calculate(uint8_t rsl_heard)
{
    if (DEVICE_MIN_SENS > rsl_heard) {
        // We are hearing packet with lower than min_sens dynamically learn the sensitivity
        DEVICE_MIN_SENS = rsl_heard;
    }
}

uint8_t ws_neighbor_class_rsl_from_dbm_calculate(int8_t dbm_heard)
{
    /* RSL MUST be calculated as the received signal level relative to standard
     * thermal noise (290oK) at 1 Hz bandwidth or 174 dBm.
     * This provides a range of -174 (0) to +80 (254) dBm.
     */

    return dbm_heard + 174;
}

static void ws_neighbor_class_parent_set_analyze(ws_neighbor_class_entry_t *ws_neighbor)
{
    if (ws_neighbor->rsl_in == RSL_UNITITIALIZED ||
            ws_neighbor->rsl_out == RSL_UNITITIALIZED) {
        ws_neighbor->candidate_parent = false;
        return;
    }

    if (ws_neighbor_class_rsl_in_get(ws_neighbor) < (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD - CAND_PARENT_HYSTERISIS) &&
            ws_neighbor_class_rsl_out_get(ws_neighbor) < (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD - CAND_PARENT_HYSTERISIS)) {
        ws_neighbor->candidate_parent = false;
    }

    if (ws_neighbor_class_rsl_in_get(ws_neighbor) > (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERISIS) &&
            ws_neighbor_class_rsl_out_get(ws_neighbor) > (DEVICE_MIN_SENS + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERISIS)) {
        ws_neighbor->candidate_parent = true;
    }
}

void ws_neighbor_class_rsl_in_calculate(ws_neighbor_class_entry_t *ws_neighbor, int8_t dbm_heard)
{
    uint8_t rsl = ws_neighbor_class_rsl_from_dbm_calculate(dbm_heard);
    // Calculate minimum sensitivity from heard packets.
    ws_neighbor_class_rf_sensitivity_calculate(rsl);
    if (ws_neighbor->rsl_in == RSL_UNITITIALIZED) {
        ws_neighbor->rsl_in = rsl << WS_RSL_SCALING;
    }
    ws_neighbor->rsl_in = ws_neighbor->rsl_in + rsl - (ws_neighbor->rsl_in >> WS_RSL_SCALING);
    ws_neighbor_class_parent_set_analyze(ws_neighbor);
    return;
}

void ws_neighbor_class_rsl_out_calculate(ws_neighbor_class_entry_t *ws_neighbor, uint8_t rsl_reported)
{
    if (ws_neighbor->rsl_out == RSL_UNITITIALIZED) {
        ws_neighbor->rsl_out = rsl_reported << WS_RSL_SCALING;
    }
    ws_neighbor->rsl_out = ws_neighbor->rsl_out + rsl_reported - (ws_neighbor->rsl_out >> WS_RSL_SCALING);
    ws_neighbor_class_parent_set_analyze(ws_neighbor);
    return;
}

#endif /* HAVE_WS */

