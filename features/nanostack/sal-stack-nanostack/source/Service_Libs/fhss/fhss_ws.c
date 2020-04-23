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
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_common.h"
#include "fhss_statistics.h"
#include "channel_list.h"
#include "channel_functions.h"
#include "fhss_ws.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "eventOS_callback_timer.h"
#include "randLIB.h"
#include "ns_trace.h"
#include "platform/arm_hal_interrupt.h"
#include <string.h>

#define TRACE_GROUP "fhss"

// Enable this flag to use channel traces
// #define FHSS_CHANNEL_DEBUG
// Enable this flag to use debug callbacks
// #define FHSS_CHANNEL_DEBUG_CBS

#ifdef FHSS_CHANNEL_DEBUG_CBS
void (*fhss_uc_switch)(void) = NULL;
void (*fhss_bc_switch)(void) = NULL;
#endif /*FHSS_CHANNEL_DEBUG_CBS*/
// Seconds to milliseconds
#define S_TO_MS(x) (((int64_t)x)*1000)
// Milliseconds to seconds
#define MS_TO_S(x) divide_integer(x, 1000)
// Seconds to microseconds
#define S_TO_US(x) (((int64_t)x)*1000000)
// Microseconds to seconds
#define US_TO_S(x) divide_integer(x, 1000000)
// Milliseconds to microseconds
#define MS_TO_US(x) (((int64_t)x)*1000)
// Microseconds to milliseconds
#define US_TO_MS(x) divide_integer(x, 1000)
// Milliseconds to nanoseconds
#define MS_TO_NS(x) (((int64_t)x)*1000000)
// Nanoseconds to milliseconds
#define NS_TO_MS(x) divide_integer(x, 1000000)
// Microseconds to nanoseconds
#define US_TO_NS(x) (((int64_t)x)*1000)
// Nanoseconds to microseconds
#define NS_TO_US(x) divide_integer(x, 1000)
#define DEF_2E24 0x1000000
#define IE_HEADER_LENGTH_MASK 0x007f
#define IE_HEADER_ID_MASK 0x7f80
#define WH_IE_ID        0x2a
#define WH_SUB_ID_UTT   1
#define WH_SUB_ID_BT    2

#ifdef HAVE_WS

struct ws_ie_t {
    uint8_t *content_ptr;
    unsigned length: 7;
    uint8_t id;
};

static int fhss_ws_manage_channel_table_allocation(fhss_structure_t *fhss_structure, uint16_t channel_count);
static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots);
static void fhss_ws_update_uc_channel_callback(fhss_structure_t *fhss_structure);
static void fhss_unicast_handler(const fhss_api_t *fhss_api, uint16_t delay);
static bool fhss_ws_check_tx_allowed(fhss_structure_t *fhss_structure);

// This function supports rounding up
static int64_t divide_integer(int64_t dividend, int32_t divisor)
{
    if (!divisor) {
        return dividend;
    }
    if (dividend < 0) {
        return (dividend - divisor / 2) / divisor;
    }
    return (dividend + divisor / 2) / divisor;
}

static uint32_t get_remaining_slots_us(fhss_structure_t *fhss_structure, void (*callback)(const fhss_api_t *api, uint16_t), uint32_t max_timeout_us)
{
    uint32_t remaining_time_us = fhss_structure->platform_functions.fhss_get_remaining_slots(callback, fhss_structure->fhss_api);
    // When remaining time goes negative, use 0.
    if (remaining_time_us > max_timeout_us) {
        remaining_time_us = 0;
    }
    return remaining_time_us;
}

void fhss_ws_start_timer(fhss_structure_t *fhss_structure, uint32_t time, void (*callback)(const fhss_api_t *fhss_api, uint16_t))
{
    // Number of millisecond slots in timeout(us)
    int32_t time_in_ms = divide_integer(time, 1000);
    // Reduce the compensation (per millisecond) from timeout.
    time -= NS_TO_US(time_in_ms * fhss_structure->ws->drift_per_millisecond_ns);
    fhss_start_timer(fhss_structure, time, callback);
}

fhss_structure_t *fhss_ws_enable(fhss_api_t *fhss_api, const fhss_ws_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer)
{
    if (!fhss_api || !fhss_configuration || !fhss_timer) {
        tr_err("Invalid FHSS enable configuration");
        return NULL;
    }
    int channel_count = channel_list_count_channels(fhss_configuration->channel_mask);
    int uc_channel_count = channel_list_count_channels(fhss_configuration->unicast_channel_mask);

    if (channel_count <= 0) {
        // There must be at least one configured channel in channel list
        return NULL;
    }
    fhss_structure_t *fhss_struct = fhss_allocate_instance(fhss_api, fhss_timer);
    if (!fhss_struct) {
        return NULL;
    }
    fhss_struct->ws = ns_dyn_mem_alloc(sizeof(fhss_ws_t));
    if (!fhss_struct->ws) {
        fhss_free_instance(fhss_api);
        return NULL;
    }
    memset(fhss_struct->ws, 0, sizeof(fhss_ws_t));
    if (fhss_ws_manage_channel_table_allocation(fhss_struct, channel_count)) {
        ns_dyn_mem_free(fhss_struct->ws);
        fhss_free_instance(fhss_api);
        tr_error("Failed to allocate channel tables");
        return NULL;
    }

    fhss_struct->fhss_event_timer = eventOS_callback_timer_register(fhss_event_timer_cb);
    fhss_struct->ws->fhss_configuration = *fhss_configuration;
    if (uc_channel_count == 0) {
        //If Unicast channel is empty use Domain mask
        for (uint8_t i = 0; i < 8; i++) {
            fhss_struct->ws->fhss_configuration.unicast_channel_mask[i] = fhss_configuration->channel_mask[i];
        }
        uc_channel_count = channel_count;
    }
    fhss_struct->number_of_channels = channel_count;
    fhss_struct->number_of_uc_channels = uc_channel_count;
    fhss_struct->optimal_packet_length = OPTIMAL_PACKET_LENGTH;
    fhss_ws_set_hop_count(fhss_struct, 0xff);
    fhss_struct->rx_channel = fhss_configuration->unicast_fixed_channel;
    fhss_struct->ws->min_synch_interval = DEFAULT_MIN_SYNCH_INTERVAL;
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    return fhss_struct;
}

static int fhss_ws_manage_channel_table_allocation(fhss_structure_t *fhss_structure, uint16_t channel_count)
{
    // Must allocate channel table for TR51
    if (!fhss_structure->ws->tr51_channel_table && !fhss_structure->ws->tr51_output_table) {
        fhss_structure->ws->tr51_channel_table = ns_dyn_mem_alloc(sizeof(int16_t) * tr51_calc_nearest_prime_number(channel_count));
        if (!fhss_structure->ws->tr51_channel_table) {
            return -1;
        }
        fhss_structure->ws->tr51_output_table = ns_dyn_mem_alloc(sizeof(int16_t) * channel_count);
        if (!fhss_structure->ws->tr51_output_table) {
            ns_dyn_mem_free(fhss_structure->ws->tr51_channel_table);
            return -1;
        }
        tr51_init_channel_table(fhss_structure->ws->tr51_channel_table, channel_count);
    }
    return 0;
}

void fhss_set_txrx_slot_length(fhss_structure_t *fhss_structure)
{
    // No broadcast schedule, no TX slots
    if (fhss_structure->ws->fhss_configuration.fhss_broadcast_interval == 0 || fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval == 0) {
        return;
    }
    uint32_t txrx_slot_length_ms_tmp = WS_TXRX_SLOT_LEN_MS;
    if (fhss_structure->callbacks.read_datarate) {
        /* Calculate minimum TX slot length which can fit optimal packet length twice.
         * Twice, because 0, 1, 4, 5... hop starts transmission at the beginning of TX slot and 2, 3, 6, 7... hop at the middle of TX slot
         *
         * hop 0
         * tx'ing            |    |            | |               |   |
         * |   BC   |   RX   |   TX   |   RX   |   TX   |   RX   |   TX   |   BC   |
         * hop 1
         * tx'ing   |    |            | |               |   |
         * |   BC   |   TX   |   RX   |   TX   |   RX   |   TX   |   RX   |   BC   |
         * hop 2
         * tx'ing                 |   |             | |               |   |
         * |   BC   |   RX   |   TX   |   RX   |   TX   |   RX   |   TX   |   BC   |
         * hop 3
         * tx'ing        |   |             | |               |   |
         * |   BC   |   TX   |   RX   |   TX   |   RX   |   TX   |   RX   |   BC   |
         */
        uint32_t datarate = fhss_structure->callbacks.read_datarate(fhss_structure->fhss_api);
        if (datarate) {
            txrx_slot_length_ms_tmp = ((fhss_structure->optimal_packet_length * 2) * (8000000 / datarate)) / 1000;
            // Do not allow using too high TX slot length.
            if (txrx_slot_length_ms_tmp > WS_TXRX_SLOT_LEN_MS) {
                tr_debug("TX slot length setting too high %"PRIu32"ms, using %"PRIu32"ms", txrx_slot_length_ms_tmp, (uint32_t)WS_TXRX_SLOT_LEN_MS);
                txrx_slot_length_ms_tmp = WS_TXRX_SLOT_LEN_MS;
            }
        }
    }
    uint32_t number_of_tx_slots = ((fhss_structure->ws->fhss_configuration.fhss_broadcast_interval - fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) / txrx_slot_length_ms_tmp) / 2;
    if (!number_of_tx_slots) {
        return;
    }
    fhss_structure->ws->txrx_slot_length_ms = (fhss_structure->ws->fhss_configuration.fhss_broadcast_interval - fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) / (number_of_tx_slots * 2);
    tr_info("TX slot length: %"PRIu32"ms", fhss_structure->ws->txrx_slot_length_ms);
}

static uint32_t fhss_get_number_of_tx_slots(fhss_structure_t *fhss_structure)
{
    return ((fhss_structure->ws->fhss_configuration.fhss_broadcast_interval - fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) / fhss_structure->ws->txrx_slot_length_ms) / 2;
}

static int32_t fhss_ws_calc_bc_channel(fhss_structure_t *fhss_structure)
{
    int32_t next_channel = fhss_structure->ws->fhss_configuration.broadcast_fixed_channel;

    if (fhss_structure->ws->fhss_configuration.ws_bc_channel_function == WS_TR51CF) {
        next_channel = tr51_get_bc_channel_index(fhss_structure->ws->tr51_channel_table, fhss_structure->ws->tr51_output_table, fhss_structure->ws->bc_slot, fhss_structure->ws->fhss_configuration.bsi, fhss_structure->number_of_channels, NULL);
        if (++fhss_structure->ws->bc_slot == fhss_structure->number_of_channels) {
            fhss_structure->ws->bc_slot = 0;
        }
    } else if (fhss_structure->ws->fhss_configuration.ws_bc_channel_function == WS_DH1CF) {
        fhss_structure->ws->bc_slot++;
        next_channel = dh1cf_get_bc_channel_index(fhss_structure->ws->bc_slot, fhss_structure->ws->fhss_configuration.bsi, fhss_structure->number_of_channels);
    } else if (fhss_structure->ws->fhss_configuration.ws_bc_channel_function == WS_VENDOR_DEF_CF) {
        if (fhss_structure->ws->fhss_configuration.vendor_defined_cf) {
            next_channel = fhss_structure->ws->fhss_configuration.vendor_defined_cf(fhss_structure->fhss_api, fhss_structure->ws->bc_slot, NULL, fhss_structure->ws->fhss_configuration.bsi, fhss_structure->number_of_channels);
        }
    }
#ifdef FHSS_CHANNEL_DEBUG
    tr_info("%"PRIu32" BC %u %u %u", fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api), next_channel, fhss_structure->ws->bc_slot, fhss_structure->ws->fhss_configuration.bsi);
#endif /*FHSS_CHANNEL_DEBUG*/
    return next_channel;
}

static uint8_t calc_own_tx_trig_slot(uint8_t own_hop)
{
    if (own_hop == 0xff) {
        return 0;
    }
    if (own_hop & 1) {
        own_hop--;
    }
    own_hop /= 2;
    return (own_hop & 1);
}

static void fhss_broadcast_handler(const fhss_api_t *fhss_api, uint16_t delay)
{
    int32_t next_channel;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return;
    }

    if (fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval == 0 || fhss_structure->ws->fhss_configuration.fhss_broadcast_interval == 0) {
        // stop broadcast schedule
        fhss_structure->ws->is_on_bc_channel = false;
        fhss_structure->ws->synchronization_time = 0;
        return;
    }
    if (fhss_structure->ws->is_on_bc_channel == false) {
        fhss_ws_start_timer(fhss_structure, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) - (delay * fhss_structure->platform_functions.fhss_resolution_divider), fhss_broadcast_handler);
        fhss_structure->ws->is_on_bc_channel = true;
        next_channel = fhss_structure->ws->bc_channel = fhss_ws_calc_bc_channel(fhss_structure);

        /* Start timer with random timeout to trigger broadcast TX queue poll event.
         * Min random is 1/50 of the channel dwell interval.
         * Max random is 1/10 of the channel dwell interval.
         * Event timer resolution is 50us.
         */
        uint32_t bc_dwell_us = MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval);
        uint16_t bc_min_random = (bc_dwell_us / 50) / 50;
        uint16_t bc_max_random = (bc_dwell_us / 10) / 50;
        eventOS_callback_timer_start(fhss_structure->fhss_event_timer, randLIB_get_random_in_range(bc_min_random, bc_max_random));
    } else {
        fhss_structure->ws->unicast_start_time_us = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
        uint32_t timeout = MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval - fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval);
        fhss_ws_start_timer(fhss_structure, timeout - (delay * fhss_structure->platform_functions.fhss_resolution_divider), fhss_broadcast_handler);
        fhss_structure->ws->is_on_bc_channel = false;
        // Should return to own (unicast) listening channel after broadcast channel
        next_channel = fhss_structure->rx_channel;
        /* Start timer with random timeout to trigger unicast TX queue poll event.
         * For hops 0,1,4,5,8,9,...
         * Min random is 1/100 of the TX slot length.
         * Max random is 1/5 of the TX slot length.
         *
         * For hops 2,3,6,7,10,11,...
         * Min random is 1/100 of the TX slot length plus 0.5*TX slot length.
         * Max random is 1/10 of the TX slot length plus 0.5*TX slot length.
         * Event timer resolution is 50us.
         */
        // returns 1 if polling of TX queue is done on latter half of the TX slot
        uint8_t own_tx_trig_slot = calc_own_tx_trig_slot(fhss_structure->own_hop);
        uint32_t txrx_slot_length_us = MS_TO_US(fhss_structure->ws->txrx_slot_length_ms);
        uint16_t uc_min_random = (((txrx_slot_length_us / 2) * own_tx_trig_slot) / 50) + ((txrx_slot_length_us / 100) / 50);
        uint16_t uc_max_random = (((txrx_slot_length_us / 2) * own_tx_trig_slot) / 50) + ((txrx_slot_length_us / 5) / 50);
        bool tx_allowed = fhss_ws_check_tx_allowed(fhss_structure);
        if (!tx_allowed) {
            uc_min_random += (txrx_slot_length_us) / 50;
            uc_max_random += (txrx_slot_length_us) / 50;
        }
        eventOS_callback_timer_start(fhss_structure->fhss_event_timer, randLIB_get_random_in_range(uc_min_random, uc_max_random));

#ifdef FHSS_CHANNEL_DEBUG
        tr_info("%"PRIu32" UC %u", fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api), fhss_structure->rx_channel);
#endif /*FHSS_CHANNEL_DEBUG*/
    }
    fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, next_channel);
#ifdef FHSS_CHANNEL_DEBUG_CBS
    if (fhss_bc_switch) {
        fhss_bc_switch();
    }
#endif /*FHSS_CHANNEL_DEBUG_CBS*/
}

static int own_floor(float value)
{
    return (int)value;
}

static int own_ceil(float value)
{
    int ivalue = (int)value;
    if (value == (float)ivalue) {
        return ivalue;
    }
    return ivalue + 1;
}

static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots)
{
    (void) slots;
    uint16_t queue_size = 0;

    fhss_structure_t *fhss_structure = fhss_get_object_with_timer_id(timer_id);
    if (!fhss_structure) {
        return;
    }

    if (fhss_structure->ws->is_on_bc_channel == true) {
        queue_size = fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, true);
    } else {
        // On unicast, start timer to trigger polling event on next TX slot
        uint32_t delay_between_tx_slots_us = MS_TO_US(fhss_structure->ws->txrx_slot_length_ms) * 2;
        // Timer could drift to RX slot when broadcast interval is high. Return timer to TX slot.
        if (fhss_ws_check_tx_allowed(fhss_structure) == false) {
            delay_between_tx_slots_us -= MS_TO_US(fhss_structure->ws->txrx_slot_length_ms - (calc_own_tx_trig_slot(fhss_structure->own_hop) * (fhss_structure->ws->txrx_slot_length_ms / 2)));
        }
        if (delay_between_tx_slots_us < get_remaining_slots_us(fhss_structure, fhss_broadcast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval))) {
            eventOS_callback_timer_start(fhss_structure->fhss_event_timer, delay_between_tx_slots_us / 50);
        }
        queue_size = fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, false);
    }
    if (queue_size) {
        fhss_structure->callbacks.tx_poll(fhss_structure->fhss_api);
    }
}

static uint32_t fhss_ws_calculate_ufsi(fhss_structure_t *fhss_structure, uint32_t tx_time)
{
    uint8_t dwell_time = fhss_structure->ws->fhss_configuration.fhss_uc_dwell_interval;
    uint16_t cur_slot = fhss_structure->ws->uc_slot;
    if (cur_slot == 0) {
        cur_slot = fhss_structure->number_of_uc_channels;
    }
    cur_slot--;
    uint32_t remaining_time_ms = US_TO_MS(get_remaining_slots_us(fhss_structure, fhss_unicast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_uc_dwell_interval)));
    uint32_t time_to_tx = 0;
    uint32_t cur_time = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
    if (cur_time < tx_time) {
        time_to_tx = US_TO_MS(tx_time - cur_time);
    }
    uint64_t ms_since_seq_start = (cur_slot * dwell_time) + (dwell_time - remaining_time_ms) + time_to_tx;
    uint32_t seq_length = 0x10000;
    if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_TR51CF) {
        ms_since_seq_start %= (dwell_time * fhss_structure->number_of_uc_channels);
        seq_length = fhss_structure->number_of_uc_channels;
    }
    return own_floor((float)(ms_since_seq_start * DEF_2E24) / (seq_length * dwell_time));
}

static uint32_t fhss_ws_calculate_broadcast_interval_offset(fhss_structure_t *fhss_structure, uint32_t tx_time)
{
    uint8_t dwell_time = fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval;
    uint32_t broadcast_interval = fhss_structure->ws->fhss_configuration.fhss_broadcast_interval;
    uint32_t remaining_time_ms = US_TO_MS(get_remaining_slots_us(fhss_structure, fhss_broadcast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval)));
    if (fhss_structure->ws->is_on_bc_channel == true) {
        remaining_time_ms += (broadcast_interval - dwell_time);
    }
    uint32_t time_to_tx = 0;
    uint32_t cur_time = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
    if (cur_time < tx_time) {
        time_to_tx = US_TO_MS(tx_time - cur_time);
    }
    return (broadcast_interval - remaining_time_ms) + time_to_tx;
}

static uint16_t fhss_ws_calculate_destination_slot(fhss_ws_neighbor_timing_info_t *neighbor_timing_info, uint32_t tx_time)
{
    uint_fast24_t ufsi = neighbor_timing_info->uc_timing_info.ufsi;
    uint32_t ufsi_timestamp = neighbor_timing_info->uc_timing_info.utt_rx_timestamp;
    uint8_t dwell_time = neighbor_timing_info->uc_timing_info.unicast_dwell_interval;
    uint32_t seq_length = 0x10000;
    if (neighbor_timing_info->uc_timing_info.unicast_channel_function  == WS_TR51CF) {
        seq_length = neighbor_timing_info->uc_timing_info.unicast_number_of_channels;
    }
    uint32_t dest_ms_since_seq_start = own_ceil((float)((uint64_t)ufsi * seq_length * dwell_time) / DEF_2E24);
    return (own_floor(((float)(US_TO_MS(tx_time - ufsi_timestamp) + dest_ms_since_seq_start) / dwell_time)) % seq_length);
}

static uint32_t fhss_ws_get_sf_timeout_callback(fhss_structure_t *fhss_structure)
{
    return MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_uc_dwell_interval);
}

static int16_t fhss_ws_synch_state_set_callback(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id)
{
    (void) pan_id;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return -1;
    }
    if (fhss_state == FHSS_SYNCHRONIZED) {
        uint32_t fhss_broadcast_interval = fhss_structure->ws->fhss_configuration.fhss_broadcast_interval;
        uint8_t fhss_bc_dwell_interval = fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval;
        fhss_set_txrx_slot_length(fhss_structure);
        // Start broadcast schedule when BC intervals are known
        if (fhss_broadcast_interval && fhss_bc_dwell_interval) {
            fhss_broadcast_handler(fhss_structure->fhss_api, 0);
        }
        // Start unicast schedule
        if ((fhss_structure->ws->fhss_configuration.ws_uc_channel_function != WS_FIXED_CHANNEL)) {
            fhss_ws_update_uc_channel_callback(fhss_structure);
            fhss_ws_start_timer(fhss_structure, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_uc_dwell_interval), fhss_unicast_handler);
            fhss_structure->ws->unicast_timer_running = true;
        }
    } else if (fhss_state == FHSS_UNSYNCHRONIZED) {
        fhss_structure->ws->synchronization_time = 0;
        eventOS_callback_timer_stop(fhss_structure->fhss_event_timer);
        fhss_stop_timer(fhss_structure, fhss_unicast_handler);
        fhss_stop_timer(fhss_structure, fhss_broadcast_handler);
    }

    fhss_structure->fhss_state = fhss_state;
    int16_t current_channel = fhss_structure->rx_channel;
    if (fhss_structure->ws->is_on_bc_channel == true) {
        current_channel = fhss_structure->ws->bc_channel;
    }
    return current_channel;
}

static void fhss_ws_update_uc_channel_callback(fhss_structure_t *fhss_structure)
{
    uint8_t mac_address[8];
    int32_t next_channel = fhss_structure->rx_channel;
    fhss_structure->callbacks.read_mac_address(fhss_structure->fhss_api, mac_address);
    if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_FIXED_CHANNEL) {
        return;
    } else if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_TR51CF) {
        next_channel = fhss_structure->rx_channel = tr51_get_uc_channel_index(fhss_structure->ws->tr51_channel_table, fhss_structure->ws->tr51_output_table, fhss_structure->ws->uc_slot, mac_address, fhss_structure->number_of_uc_channels, NULL);
        if (++fhss_structure->ws->uc_slot == fhss_structure->number_of_uc_channels) {
            fhss_structure->ws->uc_slot = 0;
        }
    } else if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_DH1CF) {
        next_channel = fhss_structure->rx_channel = dh1cf_get_uc_channel_index(fhss_structure->ws->uc_slot, mac_address, fhss_structure->number_of_uc_channels);
        fhss_structure->ws->uc_slot++;
    } else if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_VENDOR_DEF_CF) {
        if (fhss_structure->ws->fhss_configuration.vendor_defined_cf) {
            next_channel = fhss_structure->rx_channel = fhss_structure->ws->fhss_configuration.vendor_defined_cf(fhss_structure->fhss_api, fhss_structure->ws->bc_slot, mac_address, fhss_structure->ws->fhss_configuration.bsi, fhss_structure->number_of_uc_channels);
        }
    }
    // Do not switch unicast channel when broadcast channel is active.
    if (fhss_structure->ws->is_on_bc_channel == true) {
        return;
    }
#ifdef FHSS_CHANNEL_DEBUG
    tr_info("%"PRIu32" UC %u %u", fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api), next_channel, fhss_structure->ws->uc_slot);
#endif /*FHSS_CHANNEL_DEBUG*/
    fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, next_channel);
#ifdef FHSS_CHANNEL_DEBUG_CBS
    if (fhss_uc_switch) {
        fhss_uc_switch();
    }
#endif /*FHSS_CHANNEL_DEBUG_CBS*/
}

static int fhss_ws_tx_handle_callback(const fhss_api_t *api, bool is_broadcast_addr, uint8_t *destination_address, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length, uint32_t tx_time)
{
    (void) frame_type;
    (void) frame_length;
    (void) phy_header_length;
    (void) phy_tail_length;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return -1;
    }
    if (is_broadcast_addr) {
        return 0;
    }
    // Do not allow unicast destination on broadcast channel
    if (!is_broadcast_addr && (fhss_structure->ws->is_on_bc_channel == true)) {
        return -1;
    }
    // Check TX/RX slot
    if (!fhss_ws_check_tx_allowed(fhss_structure)) {
        return -1;
    }
    if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
        fhss_ws_neighbor_timing_info_t *neighbor_timing_info = fhss_structure->ws->get_neighbor_info(api, destination_address);
        if (!neighbor_timing_info) {
            fhss_stats_update(fhss_structure, STATS_FHSS_UNKNOWN_NEIGHBOR, 1);
            return -2;
        }

        if (neighbor_timing_info->uc_timing_info.unicast_number_of_channels == 0) {
            return -1;
        }

        uint16_t destination_slot = fhss_ws_calculate_destination_slot(neighbor_timing_info, tx_time);
        int32_t tx_channel = neighbor_timing_info->uc_timing_info.fixed_channel;
        if (neighbor_timing_info->uc_timing_info.unicast_channel_function == WS_TR51CF) {
            tx_channel = tr51_get_uc_channel_index(fhss_structure->ws->tr51_channel_table, fhss_structure->ws->tr51_output_table, destination_slot, destination_address, neighbor_timing_info->uc_timing_info.unicast_number_of_channels, NULL);
        } else if (neighbor_timing_info->uc_timing_info.unicast_channel_function == WS_DH1CF) {
            tx_channel = dh1cf_get_uc_channel_index(destination_slot, destination_address, neighbor_timing_info->uc_channel_list.channel_count);
        } else if (neighbor_timing_info->uc_timing_info.unicast_channel_function == WS_VENDOR_DEF_CF) {
            if (fhss_structure->ws->fhss_configuration.vendor_defined_cf) {
                tx_channel = fhss_structure->ws->fhss_configuration.vendor_defined_cf(fhss_structure->fhss_api, fhss_structure->ws->bc_slot, destination_address, fhss_structure->ws->fhss_configuration.bsi, neighbor_timing_info->uc_timing_info.unicast_number_of_channels);
            } else {
                tr_err("FHSS: vendor defined configuration failed");
                return -1;
            }
        }
#ifdef FHSS_CHANNEL_DEBUG
        tr_debug("TX channel: %u %u %s", tx_channel, destination_slot + 1, trace_array(destination_address, 8));
#endif /*FHSS_CHANNEL_DEBUG*/
        fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, tx_channel);
    }
    return 0;
}

static bool fhss_check_bad_channel(fhss_structure_t *fhss_structure, uint8_t handle)
{
    // When operating on fixed channel, we must ignore the bad channel check.
    if (fhss_structure->ws->fhss_configuration.ws_uc_channel_function == WS_FIXED_CHANNEL) {
        return true;
    }
    fhss_failed_tx_t *failed_tx = fhss_failed_handle_find(fhss_structure, handle);
    if (!failed_tx) {
        return true;
    }
    if (failed_tx->bad_channel == fhss_structure->rx_channel) {
        return false;
    }
    return true;
}

static bool fhss_ws_check_tx_allowed(fhss_structure_t *fhss_structure)
{
    // Ignore TX/RX slots
    if (fhss_structure->own_hop == 0xff) {
        return true;
    }
    // Currently on broadcast channel
    if (fhss_structure->ws->is_on_bc_channel == true) {
        return true;
    }
    uint32_t number_of_tx_slots = fhss_get_number_of_tx_slots(fhss_structure);
    // Allow transmission when broadcast interval is very short comparing to MAX slot length
    if (!number_of_tx_slots) {
        return true;
    }

    uint32_t remaining_time_ms = get_remaining_slots_us(fhss_structure, fhss_broadcast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval)) / 1000;
    uint32_t tx_slot_begin_ms = (fhss_structure->ws->fhss_configuration.fhss_broadcast_interval - fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) - (fhss_structure->ws->txrx_slot_length_ms * (fhss_structure->own_hop & 1));
    /* Return false when our first TX slot has not yet started
     *         |------remaining_time_ms---|
     *         |
     *         v
     * | BC | RX | TX | RX | TX | RX | TX | BC |
     */
    if (tx_slot_begin_ms < remaining_time_ms) {
        return false;
    }
    tx_slot_begin_ms = tx_slot_begin_ms - (((tx_slot_begin_ms - remaining_time_ms) / (2 * fhss_structure->ws->txrx_slot_length_ms)) * (2 * fhss_structure->ws->txrx_slot_length_ms));
    uint32_t rx_slot_begin_ms = tx_slot_begin_ms - fhss_structure->ws->txrx_slot_length_ms;
    /* Check if we are currently on TX slot.
     *              |        |        |
     *              v        v        v
     * | BC | RX | TX | RX | TX | RX | TX | BC |
     */
    if ((remaining_time_ms <= tx_slot_begin_ms) && (remaining_time_ms > rx_slot_begin_ms)) {
        return true;
    }

    return false;
}

static bool fhss_ws_check_tx_time(fhss_structure_t *fhss_structure, uint16_t tx_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    /*
     * Check if there is enough time for transmitting before the next multicast slot.
     */
#ifdef FHSS_WS_PROTECT_MC_SLOTS
    if (!fhss_structure->ws->fhss_configuration.fhss_broadcast_interval || !fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) {
        return true;
    }
    uint32_t tx_time = fhss_get_tx_time(fhss_structure, tx_length, phy_header_length, phy_tail_length);
    uint32_t time_to_bc_channel_us = get_remaining_slots_us(fhss_structure, fhss_broadcast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval));
    if (tx_time > time_to_bc_channel_us) {
        return false;
    }
    return true;
#else
    (void) fhss_structure;
    (void) tx_length;
    (void) phy_header_length;
    (void) phy_tail_length;
    return true;
#endif
}

static bool fhss_ws_check_tx_conditions_callback(const fhss_api_t *api, bool is_broadcast_addr, uint8_t handle, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    (void) frame_type;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return true;
    }
    // Do not allow broadcast destination on unicast channel
    if (is_broadcast_addr && (fhss_structure->ws->is_on_bc_channel == false)) {
        return false;
    }
    // Do not allow unicast destination on broadcast channel
    if (!is_broadcast_addr && (fhss_structure->ws->is_on_bc_channel == true)) {
        return false;
    }
    // This condition will check that message is not sent on bad channel
    if (fhss_check_bad_channel(fhss_structure, handle) == false) {
        return false;
    }
    // Check that there is enough unicast TX time before next broadcast channel. We try to avoid delaying the change to broadcast channel because of ongoing transmission.
    if (!is_broadcast_addr && !fhss_ws_check_tx_time(fhss_structure, frame_length, phy_header_length, phy_tail_length)) {
        return false;
    }
    // Check TX/RX slot for unicast frames
    if (!is_broadcast_addr && !fhss_ws_check_tx_allowed(fhss_structure)) {
        return false;
    }
    return true;
}

static uint8_t fhss_ws_ie_header_discover(uint8_t *header_ptr, uint16_t length, struct ws_ie_t *header_ie, uint8_t sub_id)
{
    struct ws_ie_t ie_element;
    uint8_t *sub_id_ptr;
    uint16_t ie_dummy;
    while (length > 2) {
        ie_dummy = common_read_16_bit_inverse(header_ptr);
        ie_element.length = (ie_dummy & IE_HEADER_LENGTH_MASK);
        ie_element.id = ((ie_dummy & IE_HEADER_ID_MASK) >> 7);
        ie_element.content_ptr = header_ptr + 2;
        sub_id_ptr = ie_element.content_ptr;
        if (ie_element.length && (header_ie->id == ie_element.id) && (*sub_id_ptr == sub_id)) {
            sub_id_ptr++;
            ie_element.length--;
            header_ie->content_ptr = sub_id_ptr;
            header_ie->length = ie_element.length;
            return ie_element.length;
        }
        length -= ie_element.length + 2;
        header_ptr += ie_element.length + 2;
    }
    return 0;
}

static int16_t fhss_ws_write_synch_info_callback(const fhss_api_t *api, uint8_t *ptr, uint8_t length, int frame_type, uint32_t tx_time)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure || !ptr || (frame_type != FHSS_DATA_FRAME)) {
        return -1;
    }
    platform_enter_critical();
    struct ws_ie_t header_ie;
    header_ie.id = WH_IE_ID;
    if (fhss_ws_ie_header_discover(ptr, length, &header_ie, WH_SUB_ID_UTT)) {
        uint32_t ufsi = fhss_ws_calculate_ufsi(fhss_structure, tx_time);
        common_write_24_bit_inverse(ufsi, header_ie.content_ptr + 1);
    }
    if (fhss_ws_ie_header_discover(ptr, length, &header_ie, WH_SUB_ID_BT)) {
        uint32_t broadcast_interval_offset = fhss_ws_calculate_broadcast_interval_offset(fhss_structure, tx_time);
        common_write_16_bit_inverse(fhss_structure->ws->bc_slot, header_ie.content_ptr);
        common_write_24_bit_inverse(broadcast_interval_offset, header_ie.content_ptr + 2);
    }
    platform_exit_critical();
    //TODO return destination channel here
    return fhss_structure->rx_channel;
}

static void fhss_ws_data_tx_done_callback(const fhss_api_t *api, bool waiting_ack, bool tx_completed, uint8_t handle)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }
    if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
        if (waiting_ack == false) {
            if (fhss_structure->ws->is_on_bc_channel == false) {
                fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, fhss_structure->rx_channel);
            } else {
                fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, fhss_structure->ws->bc_channel);
            }
        }
    }
    // Buffer was successfully transmitted. Remove stored failure handle if exists.
    if (tx_completed == true) {
        fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
        if (fhss_failed_tx) {
            fhss_failed_handle_remove(fhss_structure, handle);
        }
    }
}

static bool fhss_ws_data_tx_fail_callback(const fhss_api_t *api, uint8_t handle, int frame_type, uint8_t channel)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Only use channel retries when device is synchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return false;
    }

    // Use channel retries only for data frames
    if (FHSS_DATA_FRAME != frame_type) {
        return false;
    }

    fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
    if (fhss_failed_tx) {
        fhss_failed_tx->retries_done++;
        if (fhss_failed_tx->retries_done >= WS_NUMBER_OF_CHANNEL_RETRIES) {
            // No more retries. Return false to stop retransmitting.
            fhss_failed_handle_remove(fhss_structure, handle);
            return false;
        }
        fhss_failed_tx->bad_channel = channel;
    } else {
        // Create new failure handle and return true to retransmit
        fhss_failed_handle_add(fhss_structure, handle, fhss_structure->rx_channel);
    }
    fhss_stats_update(fhss_structure, STATS_FHSS_CHANNEL_RETRY, 1);
    return true;
}

static void fhss_ws_receive_frame_callback(const fhss_api_t *api, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info, int frame_type)
{
    (void) api;
    (void) pan_id;
    (void) source_address;
    (void) timestamp;
    (void) synch_info;
    (void) frame_type;
}

static bool fhss_ws_is_broadcast_channel_callback(const fhss_api_t *api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return true;
    }
    return fhss_structure->ws->is_on_bc_channel;
}

static bool fhss_ws_use_broadcast_queue_cb(const fhss_api_t *api, bool is_broadcast_addr, int frame_type)
{
    (void) frame_type;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Broadcast packets are stored in broadcast queue
    return is_broadcast_addr;
}

static uint32_t fhss_ws_get_retry_period_callback(const fhss_api_t *api, uint8_t *destination_address, uint16_t phy_mtu)
{
    (void) destination_address;
    (void) phy_mtu;
    uint32_t return_value = 0;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return return_value;
    }
    if (fhss_structure->own_hop == 0xff) {
        return return_value;
    }
    if (fhss_structure->ws->is_on_bc_channel == true) {
        return return_value;
    }

    uint32_t txrx_slot_length_us = MS_TO_US(fhss_structure->ws->txrx_slot_length_ms);
    uint32_t unicast_start_us = fhss_structure->ws->unicast_start_time_us;
    uint32_t cur_time_us = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
    uint32_t tx_trig_offset_us = (txrx_slot_length_us / 2) * calc_own_tx_trig_slot(fhss_structure->own_hop);

    uint32_t next_tx_trig_slot_start_us = unicast_start_us + (txrx_slot_length_us * !fhss_ws_check_tx_allowed(fhss_structure)) + tx_trig_offset_us;
    uint32_t next_tx_trig_slot_end_us = next_tx_trig_slot_start_us + (txrx_slot_length_us / 2);
    while ((next_tx_trig_slot_start_us < cur_time_us) || ((next_tx_trig_slot_start_us - cur_time_us) > (uint32_t) MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval))) {
        if (cur_time_us < next_tx_trig_slot_end_us) {
            return 0;
        }
        next_tx_trig_slot_start_us += (txrx_slot_length_us * 2);
        next_tx_trig_slot_end_us = next_tx_trig_slot_start_us + (txrx_slot_length_us / 2);
    }
    return_value = next_tx_trig_slot_start_us - cur_time_us;
    uint32_t time_to_bc_channel = get_remaining_slots_us(fhss_structure, fhss_broadcast_handler, MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_broadcast_interval));
    if (time_to_bc_channel <= return_value) {
        return_value += MS_TO_US(fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval);
    }

    return return_value;
}

static void fhss_unicast_handler(const fhss_api_t *fhss_api, uint16_t delay)
{
    uint32_t timeout = 0;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return;
    }
    timeout = fhss_ws_get_sf_timeout_callback(fhss_structure);
    if (!timeout) {
        fhss_stop_timer(fhss_structure, fhss_unicast_handler);
        fhss_structure->ws->unicast_timer_running = false;
        return;
    }
    fhss_ws_start_timer(fhss_structure, timeout - (delay * fhss_structure->platform_functions.fhss_resolution_divider), fhss_unicast_handler);
    fhss_structure->ws->unicast_timer_running = true;
    fhss_ws_update_uc_channel_callback(fhss_structure);
    // Unless we have broadcast schedule, we have to poll unicast queue when changing channel. This is randomized by the unicast schedule.
    if (!fhss_structure->ws->fhss_configuration.fhss_broadcast_interval || !fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval) {
        if (fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, false)) {
            fhss_structure->callbacks.tx_poll(fhss_structure->fhss_api);
        }
    }
}

int fhss_ws_set_callbacks(fhss_structure_t *fhss_structure)
{
    // Set external API
    fhss_structure->fhss_api->is_broadcast_channel = &fhss_ws_is_broadcast_channel_callback;
    fhss_structure->fhss_api->use_broadcast_queue = &fhss_ws_use_broadcast_queue_cb;
    fhss_structure->fhss_api->tx_handle = &fhss_ws_tx_handle_callback;
    fhss_structure->fhss_api->check_tx_conditions = &fhss_ws_check_tx_conditions_callback;
    fhss_structure->fhss_api->receive_frame = &fhss_ws_receive_frame_callback;
    fhss_structure->fhss_api->data_tx_done = &fhss_ws_data_tx_done_callback;
    fhss_structure->fhss_api->data_tx_fail = &fhss_ws_data_tx_fail_callback;
    fhss_structure->fhss_api->synch_state_set = &fhss_ws_synch_state_set_callback;
    fhss_structure->fhss_api->read_timestamp = NULL;
    fhss_structure->fhss_api->get_retry_period = &fhss_ws_get_retry_period_callback;
    fhss_structure->fhss_api->write_synch_info = &fhss_ws_write_synch_info_callback;
    fhss_structure->fhss_api->init_callbacks = &fhss_init_callbacks_cb;
    return 0;
}

int fhss_ws_set_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8], const broadcast_timing_info_t *bc_timing_info, const bool force_synch)
{
    (void) eui64;
    if (!fhss_structure->ws) {
        return -1;
    }
    if (!bc_timing_info->broadcast_interval || !bc_timing_info->broadcast_dwell_interval) {
        return -1;
    }
    if (((uint32_t)S_TO_US(fhss_structure->ws->min_synch_interval) > (fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api) - fhss_structure->ws->synchronization_time)) && !force_synch) {
        return 0;
    }
    platform_enter_critical();
    uint16_t own_bc_slot = fhss_structure->ws->bc_slot;
    uint32_t prev_synchronization_time = fhss_structure->ws->synchronization_time;
    fhss_structure->ws->synchronization_time = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
    uint32_t time_since_last_synch_us = fhss_structure->ws->synchronization_time - prev_synchronization_time;
    uint32_t own_bc_interval_offset = fhss_ws_calculate_broadcast_interval_offset(fhss_structure, fhss_structure->ws->synchronization_time);
    fhss_stop_timer(fhss_structure, fhss_broadcast_handler);
    uint32_t time_from_reception_ms = US_TO_MS(fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api) - bc_timing_info->bt_rx_timestamp);
    uint32_t true_bc_interval_offset = (bc_timing_info->broadcast_interval_offset + time_from_reception_ms) % bc_timing_info->broadcast_interval;
    if (true_bc_interval_offset >= bc_timing_info->broadcast_dwell_interval) {
        fhss_structure->ws->is_on_bc_channel = false;
    }
    uint32_t timeout = MS_TO_US(bc_timing_info->broadcast_interval - true_bc_interval_offset);

    if (fhss_structure->ws->is_on_bc_channel) {
        timeout -= MS_TO_US(bc_timing_info->broadcast_interval - bc_timing_info->broadcast_dwell_interval);
    }
    fhss_ws_start_timer(fhss_structure, timeout, fhss_broadcast_handler);
    uint16_t slots_since_reception = (bc_timing_info->broadcast_interval_offset + time_from_reception_ms) / bc_timing_info->broadcast_interval;
    // TODO: Calculate drift error
    fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval = bc_timing_info->broadcast_dwell_interval;
    fhss_structure->ws->fhss_configuration.fhss_broadcast_interval = bc_timing_info->broadcast_interval;
    fhss_set_txrx_slot_length(fhss_structure);
    if (fhss_structure->ws->fhss_configuration.ws_bc_channel_function == WS_FIXED_CHANNEL) {
        fhss_structure->ws->bc_slot = 0;
    } else {
        fhss_structure->ws->bc_slot = bc_timing_info->broadcast_slot + slots_since_reception;
    }
    if (fhss_structure->ws->fhss_configuration.ws_bc_channel_function == WS_TR51CF) {
        fhss_structure->ws->bc_slot %= fhss_structure->number_of_channels;
    }
    platform_exit_critical();
    //TODO: support multiple parents
    fhss_structure->ws->parent_bc_info = bc_timing_info;
    if (prev_synchronization_time && fhss_structure->ws->fhss_configuration.ws_bc_channel_function != WS_FIXED_CHANNEL) {
        //TODO: Compensation for fixed channel configuration
        if (SYNCH_COMPENSATION_MIN_INTERVAL <= US_TO_S(time_since_last_synch_us)) {
            // Update clock drift
            int32_t drift_per_ms_tmp = divide_integer((int32_t)MS_TO_NS((true_bc_interval_offset - own_bc_interval_offset) + ((int32_t)(fhss_structure->ws->bc_slot - own_bc_slot) * bc_timing_info->broadcast_interval)), US_TO_MS(time_since_last_synch_us));
            if (drift_per_ms_tmp > MAX_DRIFT_COMPENSATION_STEP) {
                drift_per_ms_tmp = MAX_DRIFT_COMPENSATION_STEP;
            } else if (drift_per_ms_tmp < -MAX_DRIFT_COMPENSATION_STEP) {
                drift_per_ms_tmp = -MAX_DRIFT_COMPENSATION_STEP;
            }
            fhss_structure->ws->drift_per_millisecond_ns += drift_per_ms_tmp;
            fhss_stats_update(fhss_structure, STATS_FHSS_DRIFT_COMP, NS_TO_US((int64_t)(fhss_structure->ws->drift_per_millisecond_ns * bc_timing_info->broadcast_dwell_interval)));
        }
        tr_debug("synch to parent: %s, drift: %"PRIi32"ms in %"PRIu64" seconds, compensation: %"PRIi32"ns per ms", trace_array(eui64, 8), true_bc_interval_offset - own_bc_interval_offset + ((int32_t)(fhss_structure->ws->bc_slot - own_bc_slot) * bc_timing_info->broadcast_interval), US_TO_S(time_since_last_synch_us), fhss_structure->ws->drift_per_millisecond_ns);
    }
    fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_INTERVAL, US_TO_S(time_since_last_synch_us));
    return 0;
}

int fhss_ws_remove_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8])
{
    (void) eui64;
    if (!fhss_structure->ws) {
        return -1;
    }
    fhss_structure->ws->parent_bc_info = NULL;
    return 0;
}

int fhss_ws_configuration_set(fhss_structure_t *fhss_structure, const fhss_ws_configuration_t *fhss_configuration)
{
    int channel_count = channel_list_count_channels(fhss_configuration->channel_mask);
    int channel_count_uc = channel_list_count_channels(fhss_configuration->unicast_channel_mask);
    if (channel_count <= 0) {
        return -1;
    }
    platform_enter_critical();
    if (fhss_configuration->ws_uc_channel_function == WS_FIXED_CHANNEL || fhss_configuration->fhss_uc_dwell_interval == 0) {
        fhss_stop_timer(fhss_structure, fhss_unicast_handler);
        fhss_structure->ws->unicast_timer_running = false;
    }
    if (fhss_configuration->ws_bc_channel_function == WS_FIXED_CHANNEL || (fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval == 0 || fhss_structure->ws->fhss_configuration.fhss_broadcast_interval == 0)) {
        fhss_structure->ws->synchronization_time = 0;
    }

    if ((fhss_structure->ws->unicast_timer_running == false) && (fhss_configuration->ws_uc_channel_function != WS_FIXED_CHANNEL) && fhss_configuration->fhss_uc_dwell_interval) {
        fhss_ws_start_timer(fhss_structure, MS_TO_US(fhss_configuration->fhss_uc_dwell_interval), fhss_unicast_handler);
        fhss_structure->ws->unicast_timer_running = true;
    }
    fhss_structure->ws->fhss_configuration = *fhss_configuration;
    if (channel_count_uc == 0) {
        //If Unicast channel is empty use Domain mask
        for (uint8_t i = 0; i < 8; i++) {
            fhss_structure->ws->fhss_configuration.unicast_channel_mask[i] = fhss_configuration->channel_mask[i];
        }
        channel_count_uc = channel_count;
    }

    fhss_structure->number_of_channels = channel_count;
    fhss_structure->number_of_uc_channels = channel_count_uc;
    if (fhss_configuration->ws_uc_channel_function == WS_FIXED_CHANNEL) {
        fhss_structure->rx_channel = fhss_configuration->unicast_fixed_channel;
    }
    platform_exit_critical();
    tr_info("fhss Configuration set, UC channel: %d, BC channel: %d, UC CF: %d, BC CF: %d, channels: BC %d UC %d, uc dwell: %d, bc dwell: %d, bc interval: %"PRIu32", bsi:%d",
            fhss_structure->ws->fhss_configuration.unicast_fixed_channel,
            fhss_structure->ws->fhss_configuration.broadcast_fixed_channel,
            fhss_structure->ws->fhss_configuration.ws_uc_channel_function,
            fhss_structure->ws->fhss_configuration.ws_bc_channel_function,
            fhss_structure->number_of_channels,
            fhss_structure->number_of_uc_channels,
            fhss_structure->ws->fhss_configuration.fhss_uc_dwell_interval,
            fhss_structure->ws->fhss_configuration.fhss_bc_dwell_interval,
            fhss_structure->ws->fhss_configuration.fhss_broadcast_interval,
            fhss_structure->ws->fhss_configuration.bsi);
    return 0;
}

int fhss_ws_set_hop_count(fhss_structure_t *fhss_structure, const uint8_t hop_count)
{
    fhss_structure->own_hop = hop_count;
    fhss_stats_update(fhss_structure, STATS_FHSS_HOP_COUNT, fhss_structure->own_hop);
    return 0;
}

#endif // HAVE_WS
