/*
 * Copyright (c) 2018-2020, Pelion and affiliates.
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
#include "common_functions.h"
#include "fhss_config.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_ie_lib.h"
#include "ws_management_api.h"
#include "mac_api.h"

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

#ifdef FEA_TRACE_SUPPORT
static int own_ceil(float value)
{
    int ivalue = (int)value;
    if (value == (float)ivalue) {
        return ivalue;
    }
    return ivalue + 1;
}

static void ws_neighbor_calculate_ufsi_drift(ws_neighbor_class_entry_t *ws_neighbor, ws_utt_ie_t *ws_utt, uint32_t timestamp, uint8_t address[8])
{
    if (ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp && ws_neighbor->fhss_data.uc_timing_info.ufsi) {
        // No UFSI on fixed channel
        if (ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function == WS_FIXED_CHANNEL) {
            return;
        }
        uint32_t seq_length = 0x10000;
        if (ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function == WS_TR51CF) {
            seq_length = ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels;
        }
        uint32_t ufsi_prev_tmp = ws_neighbor->fhss_data.uc_timing_info.ufsi;
        uint32_t ufsi_cur_tmp = ws_utt->ufsi;
        if (ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function == WS_DH1CF) {
            if (ufsi_cur_tmp < ufsi_prev_tmp) {
                ufsi_cur_tmp += 0xffffff;
            }
        }
        // Convert 24-bit UFSI to real time before drift calculation
        uint32_t time_since_seq_start_prev_ms = own_ceil((float)((uint64_t)ufsi_prev_tmp * seq_length * ws_neighbor->fhss_data.uc_timing_info.unicast_dwell_interval) / 0x1000000);
        uint32_t time_since_seq_start_cur_ms = own_ceil((float)((uint64_t)ufsi_cur_tmp * seq_length * ws_neighbor->fhss_data.uc_timing_info.unicast_dwell_interval) / 0x1000000);
        uint32_t time_since_last_ufsi_us = timestamp - ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp;

        if (ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function == WS_TR51CF) {
            uint32_t full_uc_schedule_ms = ws_neighbor->fhss_data.uc_timing_info.unicast_dwell_interval * ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels;
            uint32_t temp_ms = (time_since_last_ufsi_us / 1000) / full_uc_schedule_ms;
            if (time_since_seq_start_cur_ms >= time_since_seq_start_prev_ms) {
                temp_ms--;
            }
            time_since_seq_start_cur_ms += temp_ms * full_uc_schedule_ms + (full_uc_schedule_ms - time_since_seq_start_prev_ms) + time_since_seq_start_prev_ms;
        }

        uint32_t ufsi_diff_ms = time_since_seq_start_cur_ms - time_since_seq_start_prev_ms;
        int32_t ufsi_drift_ms = (int32_t)(time_since_last_ufsi_us / 1000 - ufsi_diff_ms);
        // Only trace if there is significant error
        if (ufsi_drift_ms < -5 || ufsi_drift_ms > 5) {
            tr_debug("UFSI updated: %s, drift: %"PRIi32"ms in %"PRIu32" seconds", trace_array(address, 8), ufsi_drift_ms, time_since_last_ufsi_us / 1000000);
        }
    }
}
#endif

void ws_neighbor_class_neighbor_unicast_time_info_update(ws_neighbor_class_entry_t *ws_neighbor, ws_utt_ie_t *ws_utt, uint32_t timestamp, uint8_t address[8])
{
#ifdef FEA_TRACE_SUPPORT
    ws_neighbor_calculate_ufsi_drift(ws_neighbor, ws_utt, timestamp, address);
#else
    (void) address;
#endif
    ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp = timestamp;
    ws_neighbor->fhss_data.uc_timing_info.ufsi = ws_utt->ufsi;
}

static void ws_neighbour_excluded_mask_by_range(ws_channel_mask_t *channel_info, ws_excluded_channel_range_t *range_info, uint16_t number_of_channels)
{
    uint16_t range_start, range_stop;
    uint8_t mask_index = 0;
    uint32_t compare_mask_bit;
    uint8_t *range_ptr = range_info->range_start;
    while (range_info->number_of_range) {
        range_start = common_read_16_bit_inverse(range_ptr);
        range_ptr += 2;
        range_stop = common_read_16_bit_inverse(range_ptr);
        range_ptr += 2;
        range_info->number_of_range--;
        for (uint16_t channel = 0; channel < number_of_channels; channel++) {

            if (channel >= range_start && channel <= range_stop) {
                //Cut channel
                compare_mask_bit = 1U << (channel % 32);
                mask_index = 0 + (channel / 32);

                if (channel_info->channel_mask[mask_index] & compare_mask_bit) {
                    channel_info->channel_mask[mask_index] ^= compare_mask_bit;
                    channel_info->channel_count--;
                }
            } else if (channel > range_stop) {
                break;
            }
        }
    }
}

static uint32_t ws_reserve_order_32_bit(uint32_t value)
{
    uint32_t ret_val = 0;
    for (uint8_t i = 0; i < 32; i++) {
        if ((value & (1 << i))) {
            ret_val |= 1U << ((32 - 1) - i);
        }
    }
    return ret_val;
}

static void ws_neighbour_excluded_mask_by_mask(ws_channel_mask_t *channel_info, ws_excluded_channel_mask_t *mask_info, uint16_t number_of_channels)
{
    if (mask_info->mask_len_inline == 0) {
        return;
    }

    uint16_t channel_at_mask;
    uint8_t mask_index = 0;
    uint32_t channel_compare_mask, compare_mask_bit;
    uint8_t *mask_ptr =  mask_info->channel_mask;

    channel_at_mask = mask_info->mask_len_inline * 8;

    for (uint16_t channel = 0; channel < number_of_channels; channel += 32) {
        if (channel) {
            mask_index++;
            mask_ptr += 4;
        }

        //Read allaways 32-bit
        if (channel_at_mask >= 32) {
            channel_compare_mask = common_read_32_bit(mask_ptr);
            channel_at_mask -= 32;
        } else {
            //Read Rest bytes seprately
            channel_compare_mask = 0;
            uint8_t move_mask = 0;
            //Convert 8-24bit to 32-bit
            while (channel_at_mask) {
                channel_compare_mask |= (uint32_t)(*mask_ptr++ << (24 - move_mask));
                channel_at_mask -= 8;
                move_mask += 8;
            }
        }
        //Reserve bit order for compare
        channel_compare_mask = ws_reserve_order_32_bit(channel_compare_mask);
        //Compare now 32-bit mask's bits one by one
        for (uint8_t i = 0; i < 32; i++) {
            //Start from MSB
            compare_mask_bit = 1 << (i);
            if ((channel_compare_mask & compare_mask_bit) && (channel_info->channel_mask[mask_index] & compare_mask_bit)) {
                channel_info->channel_mask[mask_index] ^= compare_mask_bit;
                channel_info->channel_count--;
            }
        }
        //Stop compare if all bits in line are compared
        if (channel_at_mask == 0) {
            break;
        }
    }
}

void ws_neighbor_class_neighbor_unicast_schedule_set(ws_neighbor_class_entry_t *ws_neighbor, ws_us_ie_t *ws_us, ws_hopping_schedule_t *own_shedule)
{
    ws_neighbor->fhss_data.uc_timing_info.unicast_channel_function = ws_us->channel_function;
    if (ws_us->channel_function == WS_FIXED_CHANNEL) {
        ws_neighbor->fhss_data.uc_timing_info.fixed_channel = ws_us->function.zero.fixed_channel;
        ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = 1;
    } else {

        if (ws_us->channel_plan == 0) {
            ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = ws_common_channel_number_calc(ws_us->plan.zero.regulator_domain, ws_us->plan.zero.operation_class, own_shedule->channel_plan_id);
        } else if (ws_us->channel_plan == 1) {
            ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels = ws_us->plan.one.number_of_channel;

        } else if (ws_us->channel_plan == 2) {
            //TODO add Channel plan 2 channel count function call here
        }

        //Handle excluded channel and generate activate channel list
        if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
            ws_common_generate_channel_list(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels, own_shedule->regulatory_domain, own_shedule->operating_class, own_shedule->channel_plan_id);
            ws_neighbor->fhss_data.uc_channel_list.channel_count = ws_common_active_channel_count(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels);
            ws_neighbour_excluded_mask_by_range(&ws_neighbor->fhss_data.uc_channel_list, &ws_us->excluded_channels.range, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels);
        } else if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
            ws_common_generate_channel_list(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels, own_shedule->regulatory_domain, own_shedule->operating_class, own_shedule->channel_plan_id);
            ws_neighbor->fhss_data.uc_channel_list.channel_count = ws_common_active_channel_count(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels);
            ws_neighbour_excluded_mask_by_mask(&ws_neighbor->fhss_data.uc_channel_list, &ws_us->excluded_channels.mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels);
        } else if (ws_us->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_NONE) {
            if (ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels != ws_neighbor->fhss_data.uc_channel_list.channel_count) {
                ws_common_generate_channel_list(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels, own_shedule->regulatory_domain, own_shedule->operating_class, own_shedule->channel_plan_id);
                ws_neighbor->fhss_data.uc_channel_list.channel_count = ws_common_active_channel_count(ws_neighbor->fhss_data.uc_channel_list.channel_mask, ws_neighbor->fhss_data.uc_timing_info.unicast_number_of_channels);
            }
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

void ws_neighbor_class_rf_sensitivity_calculate(uint8_t dev_min_sens_config, int8_t dbm_heard)
{
    if (dev_min_sens_config != 0) {
        // Automatic mode disabled
        return;
    }
    uint8_t rsl_heard = ws_neighbor_class_rsl_from_dbm_calculate(dbm_heard);
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


bool ws_neighbor_class_neighbor_duplicate_packet_check(ws_neighbor_class_entry_t *ws_neighbor, uint8_t mac_dsn, uint32_t rx_timestamp)
{
    if (ws_neighbor->last_DSN != mac_dsn) {
        // New packet allways accepted
        ws_neighbor->last_DSN = mac_dsn;
        return true;
    }

    if (!ws_neighbor->unicast_data_rx) {
        // No unicast info stored always accepted
        return true;
    }

    rx_timestamp -= ws_neighbor->fhss_data.uc_timing_info.utt_rx_timestamp;
    rx_timestamp /= 1000000; //Convert to s

    //Compare only when last rx timestamp is less than 5 seconds
    if (rx_timestamp < 5) {
        //Packet is sent too fast filter it out
        return false;
    }

    return true;
}

#ifdef HAVE_WS_VERSION_1_1
void ws_neighbor_class_pcap_ie_store(struct ws_neighbor_class_entry *ws_neighbor, const struct mcps_data_ie_list *ie_ext)
{
    ws_wp_nested_pcap_read(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_neighbor->pcap_info);
}
#endif


#endif /* HAVE_WS */

