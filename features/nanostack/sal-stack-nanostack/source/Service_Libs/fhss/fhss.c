/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "fhss_channel.h"
#include "channel_list.h"
#include "nsdynmemLIB.h"
#include "fhss_beacon.h"
#include "fhss_statistics.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_callback_timer.h"
#include "randLIB.h"
#include <string.h>

#define TRACE_GROUP "fhss"
// Uncomment this to get timestamped output on superframe events.
// Note: the result may be massive, as there will be dozens or hundreds of lines of trace per second.
// #define FHSS_MASSIVE_TRACE

// TODO: create linked list
// FHSS object pointer
fhss_structure_t *fhss_struct = 0;

static bool fhss_check_remaining_tx_time(fhss_structure_t *fhss_structure, uint16_t tx_length, uint8_t phy_header_length, uint8_t phy_tail_length);
static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots);
static fhss_structure_t *fhss_get_object_with_timer_id(const int8_t timer_id);
static int fhss_generate_scramble_table(fhss_structure_t *fhss_structure);
static bool fhss_is_there_common_divisor(uint16_t i, uint8_t j);
static void fhss_update_channel(fhss_structure_t *fhss_structure);
static int fhss_reset_synch_monitor(fhss_synch_monitor_s *synch_monitor, bool reset_compensation);
static int fhss_reset(fhss_structure_t *fhss_structure);
static void fhss_failed_list_free(fhss_structure_t *fhss_structure);


int8_t fhss_enable(fhss_api_t *fhss_api, const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics)
{
    if (!fhss_api || !fhss_configuration || !fhss_timer || fhss_struct) {
        tr_err("Invalid FHSS enable configuration");
        return -1;
    }
    int channel_count = channel_list_count_channels(fhss_configuration->channel_mask);
    if (channel_count <= 0) {
        // There must be at least one configured channel in channel list
        return -2;
    }
    fhss_struct = ns_dyn_mem_alloc(sizeof(fhss_structure_t));
    if (!fhss_struct) {
        return -3;
    }
    fhss_struct->fhss_api = fhss_api;
    fhss_struct->fhss_configuration = *fhss_configuration;
    fhss_struct->platform_functions = *fhss_timer;
    fhss_struct->fhss_stats_ptr = fhss_statistics;
    fhss_struct->number_of_channels = channel_count;

    // set a invalid id to tasklet_id, so we know that one is not started yet
    fhss_struct->beacon_tasklet_id = -1;

    if (!fhss_struct->platform_functions.fhss_resolution_divider) {
        fhss_struct->platform_functions.fhss_resolution_divider = 1;
    }
    // Default synch interval is 240 seconds
    if (!fhss_struct->fhss_configuration.fhss_max_synch_interval) {
        fhss_struct->fhss_configuration.fhss_max_synch_interval = 240;
    }
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    fhss_struct->own_hop = 0xff;
    fhss_reset(fhss_struct);
    fhss_reset_synch_monitor(&fhss_struct->synch_monitor, true);
    fhss_struct->active_fhss_events = 0;
    fhss_struct->fhss_beacon_info_store = NULL;
    fhss_struct->fhss_event_timer = eventOS_callback_timer_register(fhss_event_timer_cb);

    fhss_generate_scramble_table(fhss_struct);

    if (fhss_beacon_create_tasklet(fhss_struct) < 0) {
        // XXX: should we free the fhss_structure here?
        return -5;
    }

    return 0;
}

int8_t fhss_set_datarate(fhss_structure_t *fhss_structure, uint32_t datarate)
{
    if (!fhss_structure) {
        return -1;
    }
    // If datarate is not set, use default 250kbit/s. Datarate is used as divider later.
    if (!datarate) {
        datarate = 250000;
    }
    fhss_structure->datarate = datarate;
    return 0;
}

int8_t fhss_set_synch_configuration(fhss_structure_t *fhss_structure, const fhss_synch_configuration_t *fhss_synch_configuration)
{
    if (!fhss_structure) {
        return -1;
    }
    if (!fhss_synch_configuration) {
        return -2;
    }
    // None of the configurations can be set zero
    if( fhss_synch_configuration->fhss_number_of_bc_channels == 0 || fhss_synch_configuration->fhss_number_of_tx_slots == 0
            || fhss_synch_configuration->fhss_number_of_superframes == 0 || fhss_synch_configuration->fhss_superframe_length == 0) {
        return -3;
    }
    // Number of channels must be divisible with the number of broadcast channels.
    // Number of superframes must be divisible with the number of TX slots
    if (((fhss_structure->number_of_channels % fhss_synch_configuration->fhss_number_of_bc_channels) != 0) ||
            ((fhss_synch_configuration->fhss_number_of_superframes % fhss_synch_configuration->fhss_number_of_tx_slots) != 0) ||
            (fhss_synch_configuration->fhss_number_of_superframes <= fhss_synch_configuration->fhss_number_of_tx_slots)) {
        return -4;
    }
    fhss_structure->synch_configuration = *fhss_synch_configuration;
    fhss_structure->own_hop = 0;
    return 0;
}

fhss_structure_t *fhss_get_object_with_api(const fhss_api_t *fhss_api)
{
    if (!fhss_api || !fhss_struct) {
        return NULL;
    }
    if (fhss_struct->fhss_api == fhss_api) {
        return fhss_struct;
    }
    return NULL;
}

bool fhss_is_synch_root(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->own_hop > 0) {
        return false;
    }
    return true;
}

void fhss_set_active_event(fhss_structure_t *fhss_structure, uint8_t event_type)
{
    fhss_structure->active_fhss_events |= (1 << event_type);
}

void fhss_clear_active_event(fhss_structure_t *fhss_structure, uint8_t event_type)
{
    fhss_structure->active_fhss_events &= ~(1 << event_type);
}

bool fhss_read_active_event(fhss_structure_t *fhss_structure, uint8_t event_type)
{
    if (fhss_structure->active_fhss_events & (1 << event_type)) {
        return true;
    }
    return false;
}

static fhss_structure_t *fhss_get_object_with_timer_id(const int8_t timer_id)
{
    if (timer_id <0 || !fhss_struct) {
        return NULL;
    }
    if (fhss_struct->fhss_event_timer == timer_id) {
        return fhss_struct;
    }
    return NULL;
}

static bool fhss_is_bc_sending_superframe(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->current_superframe >= fhss_structure->broadcast_start_superframe) {
        return true;
    }

    return false;
}

bool fhss_check_bad_channel(fhss_structure_t *fhss_structure, uint8_t handle)
{
    if (!fhss_structure) {
        return false;
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

bool fhss_check_channel_type(fhss_structure_t *fhss_structure, bool is_bc, int frame_type)
{
    if (!fhss_structure) {
        return false;
    }

    // Channel type check is valid only when FHSS is synchronized
    if(fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return true;
    }

    if (frame_type == FHSS_DATA_FRAME) {
        if (is_bc == true) {
            // Drop: if packet has broadcast destination but current channel is unicast channel or current superframe is not broadcast sending superframe
            if ((fhss_is_current_channel_broadcast(fhss_structure) == false) || (fhss_is_bc_sending_superframe(fhss_structure) == false)) {
                return false;
            }
        } else if (fhss_is_current_channel_broadcast(fhss_structure) == true) {
            // Drop: If waiting synchronization Beacon and packet has unicast destination
            if (fhss_structure->beacons_received_timer) {
                return false;
            }
        }
    }
    if (frame_type == FHSS_SYNCH_FRAME) {

    }
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME) {
        // Drop: If we have unicast channels in our configuration and current channel is broadcast channel
        if ((fhss_structure->number_of_channels > fhss_structure->synch_configuration.fhss_number_of_bc_channels) && (fhss_is_current_channel_broadcast(fhss_structure) == true)) {
            return false;
        }

        uint8_t current_superframe = fhss_structure->current_superframe;
        uint8_t synch_attempt = fhss_structure->beacons_received_timer;
        if (fhss_structure->synch_configuration.fhss_number_of_tx_slots > 1) {
            // Send synch request either before or after the middle of the channel depending on attempt number.
            uint8_t middle_of_superframes = fhss_structure->synch_configuration.fhss_number_of_superframes / 2;

            if (synch_attempt & 1) {
                if (fhss_structure->current_superframe < middle_of_superframes) {
                    return false;
                }
            } else {
                if (fhss_structure->current_superframe >= middle_of_superframes) {
                    return false;
                }
            }
        } else if ((current_superframe == 0) || (current_superframe == (fhss_structure->synch_configuration.fhss_number_of_superframes - 1))){
            return false;
        }
    }
    return true;

}

bool fhss_check_tx_allowed(fhss_structure_t *fhss_structure, bool is_bc, uint16_t frame_length, int frame_type, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    if (!fhss_structure) {
        return false;
    }
    // Check is valid only when FHSS is synchronized
    if(fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return true;
    }
    // Allow: If sending synchronization frame
    if (frame_type == FHSS_SYNCH_FRAME) {
        return true;
    }
    // Allow: If sending broadcast (on broadcast channel)
    if (is_bc == true) {
        return true;
    }
    // Deny: If FHSS is not on TX slot
    if (fhss_structure->tx_allowed == false) {
        return false;
    }
    // Deny: If not enough time before TX slot end
    if (fhss_check_remaining_tx_time(fhss_structure, frame_length, phy_header_length, phy_tail_length) == false) {
        return false;
    }
    return true;
}

static int fhss_reset_synch_monitor(fhss_synch_monitor_s *synch_monitor, bool reset_compensation)
{
	if (synch_monitor) {
	    synch_monitor->avg_synch_fix = 0;
	    // Initialize to -1 instead of 0 to drop the first beacon after network scan (from synch monitoring)
	    synch_monitor->avg_synch_fix_counter = -1;
	    if (reset_compensation == true) {
	        synch_monitor->drift_compensation = 0;
	    }
	    synch_monitor->channel_counter = 0;
		return 0;
	}
	return -1;
}

static int fhss_reset(fhss_structure_t *fhss_structure)
{
    if (fhss_structure) {
        fhss_structure->platform_functions.fhss_timer_stop(fhss_structure->fhss_api);
        fhss_struct->synch_panid = 0xffff;
        fhss_beacon_periodic_stop(fhss_structure);
        fhss_struct->current_superframe = 0;
        fhss_struct->current_channel_index = 0;
        fhss_struct->channel_list_counter = 0;
        if (fhss_is_synch_root(fhss_structure) == false) {
            fhss_struct->own_hop = 0xff;
        }
        fhss_struct->tx_allowed = false;
        fhss_struct->synch_interval = (uint32_t) (fhss_struct->fhss_configuration.fhss_max_synch_interval/BEACON_INTERVAL_INIT_DIVIDER) * 1000;
        fhss_struct->rx_channel = 0;
        fhss_struct->beacons_received_timer = 0;
        memset(fhss_struct->synch_parent, 0xff, 8);
        fhss_struct->send_synch_info_on_next_broadcast_channel = false;
        memset(&fhss_struct->synch_configuration, 0, sizeof(fhss_synch_configuration_t));
        fhss_struct->synch_infos_sent_counter = 0;
        fhss_struct->broadcast_start_superframe = 0;
        fhss_failed_list_free(fhss_structure);
        fhss_struct->fhss_state = FHSS_UNSYNCHRONIZED;
        return 0;
    }
    return -1;
}

int fhss_down(fhss_structure_t *fhss_structure)
{
    if (fhss_structure) {
        fhss_reset(fhss_structure);
        fhss_reset_synch_monitor(&fhss_struct->synch_monitor, false);
        fhss_stats_update(fhss_structure, STATS_FHSS_DRIFT_COMP, fhss_structure->synch_monitor.drift_compensation);
        fhss_stats_update(fhss_structure, STATS_FHSS_AVG_SYNCH_FIX, fhss_structure->synch_monitor.avg_synch_fix);
        fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_INTERVAL, fhss_structure->synch_interval / 1000);
        return 0;
    }
    return -1;
}


int8_t fhss_disable(fhss_structure_t *fhss_structure)
{
    if (!fhss_structure) {
        return -1;
    }
    ns_dyn_mem_free(fhss_structure);
    fhss_structure = 0;
    fhss_struct = 0;
    return 0;
}

void fhss_start_timer(fhss_structure_t *fhss_structure, uint32_t time, void (*callback)(const fhss_api_t *fhss_api, uint16_t))
{
    if (callback){
        // Don't allow starting with zero slots
        if (time < fhss_structure->platform_functions.fhss_resolution_divider) {
            time = fhss_structure->platform_functions.fhss_resolution_divider;
        }
        fhss_structure->platform_functions.fhss_timer_start(time / fhss_structure->platform_functions.fhss_resolution_divider, callback, fhss_structure->fhss_api);
    }
}

uint32_t fhss_get_remaining_time_to_next_superframe(const fhss_structure_t *fhss_structure)
{
    const uint32_t slots = fhss_structure->platform_functions.fhss_get_remaining_slots(fhss_structure->fhss_api);
    const uint32_t time = slots * fhss_structure->platform_functions.fhss_resolution_divider;
    return time;
}

void fhss_superframe_handler(const fhss_api_t *fhss_api, uint16_t delay)
{
    int compensation = 0;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return;
    }
/* Drift compensation doesn't work with Linux platform */
#ifndef __linux__
    // Drift compensation on first superframe
    if (fhss_structure->current_superframe == 0) {
        /* Idea is to compensate number of drift_compensation (microseconds) on each channel.
         * However, fhss_resolution_divider defines the minimum timer resolution.
         * E.g. if fhss_resolution_divider = 64, compensate (drift_compensation * 64) on each 64th channel.
         */
        if (++fhss_structure->synch_monitor.channel_counter == fhss_structure->platform_functions.fhss_resolution_divider) {
            compensation = fhss_structure->synch_monitor.drift_compensation;
            fhss_structure->synch_monitor.channel_counter = 0;
        }
    }
#endif //__linux__
    // Restart timer asap to minimize the effect of dynamic execution time of
    // the rest of function.
    fhss_start_timer(fhss_structure, (fhss_structure->synch_configuration.fhss_superframe_length) - (delay * fhss_structure->platform_functions.fhss_resolution_divider) + (compensation * fhss_structure->platform_functions.fhss_resolution_divider), fhss_superframe_handler);

    // check, if the current frame was the last one
    if (fhss_structure->current_superframe >= (fhss_structure->synch_configuration.fhss_number_of_superframes - 1)) {

        // last superframe has passed, change channel
        fhss_structure->current_superframe = 0;
        fhss_structure->current_channel_index++;
        if (fhss_structure->current_channel_index >= fhss_structure->number_of_channels) {
            fhss_structure->synch_infos_sent_counter = 0;
            fhss_structure->current_channel_index = 0;
            fhss_structure->channel_list_counter++;
            // Repeated cycle is started from beginning, reset counter. Don't let the channel_list_counter overflow.
            if (fhss_structure->channel_list_counter >= ((uint16_t) fhss_structure->number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES)) {
                fhss_structure->channel_list_counter = 0;
            }
            // Hop 0 don't have parent
            if (fhss_is_synch_root(fhss_structure) == false) {
                fhss_trig_event(fhss_structure, FHSS_COMPARE_SYNCH_PARENT);
            }
            fhss_trig_event(fhss_structure, FHSS_UPDATE_SYNCH_INFO_STORAGE);
        }
        fhss_update_channel(fhss_structure);
    } else  {
        // bump up the superframe counter
        fhss_structure->current_superframe++;

#ifdef FHSS_MASSIVE_TRACE
        tr_debug("%"PRIu32": handler, super: %"PRIu8,
                fhss_structure->fhss_api->read_timestamp(fhss_structure->fhss_api), fhss_structure->current_superframe);
#endif
    }
    if ((fhss_structure->send_synch_info_on_next_broadcast_channel == true) && (fhss_is_current_channel_broadcast(fhss_structure) == true)) {
        /* Randomize sending superframe of synchronization frame:
         * on first superframe probability is 1/number of superframes
         * on second superframe probability is 1/(number of superframes-1)
         * on third superframe probability is 1/(number of superframes-2)
         * on last superframe probability is 1/1
         */
        if (randLIB_get_random_in_range(1, fhss_structure->synch_configuration.fhss_number_of_superframes - fhss_structure->current_superframe) == 1) {
            fhss_structure->send_synch_info_on_next_broadcast_channel = false;
            fhss_structure->synch_infos_sent_counter++;
            fhss_structure->callbacks.send_fhss_frame(fhss_structure->fhss_api, FHSS_SYNCH_FRAME);
        }
    }
    fhss_update_txrx_slots(fhss_structure);
    uint16_t queue_size = fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, false) + fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, true);
    if ((fhss_structure->tx_allowed == true || fhss_is_current_channel_broadcast(fhss_structure) == true) && queue_size) {
        /* Start timer with random timeout to trigger TX queue poll event.
         * Max random is half of the superframe length. Event timer resolution is 50us.
         * Divide Max random with TX queue size to transmit faster when TX queue is growing
         */
        uint16_t max_random = ((fhss_structure->synch_configuration.fhss_superframe_length / 2) / 50) / queue_size;
        eventOS_callback_timer_start(fhss_structure->fhss_event_timer, randLIB_get_random_in_range(1, max_random));
    }
    if (fhss_structure->fhss_timeout) {
        fhss_structure->fhss_timer += fhss_structure->synch_configuration.fhss_superframe_length;
        if (fhss_structure->fhss_timer >= fhss_structure->fhss_timeout) {
            fhss_trig_event(fhss_structure, FHSS_TIMER_EVENT);
            fhss_structure->fhss_timeout = 0;
            fhss_structure->fhss_timer = 0;
        }
    }
}

int fhss_timeout_start(fhss_structure_t *fhss_structure, uint32_t time)
{
    if (!fhss_structure) {
        return -1;
    }
    fhss_structure->fhss_timeout = time;
    fhss_structure->fhss_timer = 0;
    return 0;
}

int fhss_timeout_stop(fhss_structure_t *fhss_structure)
{
    if (!fhss_structure) {
        return -1;
    }
    fhss_structure->fhss_timeout = 0;
    fhss_structure->fhss_timer = 0;
    return 0;
}

int fhss_update_txrx_slots(fhss_structure_t *fhss_structure)
{
    uint8_t cur_superframe = fhss_structure->current_superframe;
    uint8_t number_of_tx_slots = fhss_structure->synch_configuration.fhss_number_of_tx_slots;
    uint8_t number_of_superframes = fhss_structure->synch_configuration.fhss_number_of_superframes;
    uint8_t tx_slot_length = ((number_of_superframes / 2) / number_of_tx_slots);
    uint8_t tx_slot_up_limit = tx_slot_length;
    bool tx_allowed = false;

    /* FHSS Phase 1 solution. This will be updated in future since we need to be able to communicate with nodes on same hop distance.
     * Even (0, 2, 4,...) hops starts with TX slot and uneven (1, 3, 5, ...) hops starts with RX slot. This way we are able to communicate to our parent and child nodes.
     *
     * If number of superframes is 16 and number of TX slots is 2, this will be generated (Note: hop 0 is always border router):
     *
     * Hop         Superframe
     *  0  |TX|TX|TX|TX|RX|RX|RX|RX|TX|TX|TX|TX|RX|RX|RX|RX|
     *  1  |RX|RX|RX|RX|TX|TX|TX|TX|RX|RX|RX|RX|TX|TX|TX|TX|
     *  2  |TX|TX|TX|TX|RX|RX|RX|RX|TX|TX|TX|TX|RX|RX|RX|RX|
     *  3  |RX|RX|RX|RX|TX|TX|TX|TX|RX|RX|RX|RX|TX|TX|TX|TX|
     *
     */

    if ((fhss_structure->own_hop % 2)) {
        tx_slot_up_limit += tx_slot_length;
    }
    while(number_of_tx_slots--)
    {
        if ((cur_superframe >= (tx_slot_up_limit - tx_slot_length)) && (cur_superframe < tx_slot_up_limit)) {
            tx_allowed = true;
            break;
        } else {
            tx_slot_up_limit += (tx_slot_length * 2);
        }
    }
    fhss_structure->tx_allowed = tx_allowed;
    return 0;
}

static int fhss_update_drift_compensation(fhss_structure_t *fhss_structure)
{
    int retval = 0;
    int channels_per_synch_period;
    uint16_t bc_density;
    uint16_t channel_dwell_time;
    uint32_t synch_period;

    if (!fhss_structure) {
        return 0;
    }
    bc_density = (fhss_structure->number_of_channels / fhss_structure->synch_configuration.fhss_number_of_bc_channels);
    channel_dwell_time = ((uint32_t)fhss_structure->synch_configuration.fhss_superframe_length * fhss_structure->synch_configuration.fhss_number_of_superframes) / 1000;
    // Calculate last synchronisation period
    if (fhss_structure->synch_interval != ((uint32_t)fhss_structure->fhss_configuration.fhss_max_synch_interval * 1000)) {
        // Last period was half of the current started period and max random period is shorter
        synch_period = (fhss_structure->synch_interval / 2) + (bc_density * channel_dwell_time) * (fhss_structure->synch_configuration.fhss_number_of_bc_channels / 2);
    } else {
        synch_period = fhss_structure->synch_interval + (bc_density * channel_dwell_time) * fhss_structure->synch_configuration.fhss_number_of_bc_channels;
    }

    // E.g. 240000ms / (50000us * 8) = 600 channels per fhss_beacon_send_interval
    channels_per_synch_period = (synch_period * 1000) / ((uint32_t)fhss_structure->synch_configuration.fhss_superframe_length * fhss_structure->synch_configuration.fhss_number_of_superframes);
    // Calculate compensation value: how much to compensate on each channel. E.g. 6000us / 600channels = 10us/channel
    fhss_structure->synch_monitor.drift_compensation += (fhss_structure->synch_monitor.avg_synch_fix / channels_per_synch_period);
    fhss_stats_update(fhss_structure, STATS_FHSS_DRIFT_COMP, fhss_structure->synch_monitor.drift_compensation);
    if ((fhss_structure->synch_monitor.avg_synch_fix > FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT) || (fhss_structure->synch_monitor.avg_synch_fix < -FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT)) {
        // Indicates that more frequent synchronization is needed
        retval = -1;
    }
    return retval;
}

static int fhss_update_synch_monitor(fhss_structure_t *fhss_structure, const fhss_synchronization_beacon_payload_s *payload, uint8_t superframe_own, int32_t remaining_time_own, const int32_t time_to_next_superframe)
{
    fhss_synch_configuration_t *configuration = &fhss_structure->synch_configuration;
    int super_frame_changed = payload->current_superframe - superframe_own;
    int retval = 0;
    if (((super_frame_changed < 0) && ((super_frame_changed * -1) < (payload->number_of_superframes_per_channel / 2))) || (super_frame_changed > payload->number_of_superframes_per_channel / 2)) {
        /* Our superframe has changed before parent superframe -> we are running too fast
         * Own channel:     | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 0 |
         * Parent channel:    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 0 |
         * Time:            ----------------------------------->
         */
        if (super_frame_changed > 0) {
            super_frame_changed = (superframe_own + payload->number_of_superframes_per_channel) - payload->current_superframe;
        }
    } else {
        /* Parent superframe has changed before our superframe -> we are running too slow
         * Own channel:       | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 0 |
         * Parent channel:  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 0 |
         * Time:            ----------------------------------->
         */
        if (super_frame_changed < 0) {
            super_frame_changed = (payload->current_superframe + payload->number_of_superframes_per_channel) - superframe_own;
        }
    }

    /* If superframe changed during synchronization but remaining time to next superframe is high, it is likely that
     * superframe change is not valid anymore. Don't use this Beacon for syncronization monitoring.
     */
    if ((configuration->fhss_superframe_length - remaining_time_own) > CLOSE_TO_SUPERFRAME_LENGTH) {
        remaining_time_own += (int32_t) configuration->fhss_superframe_length * super_frame_changed;

        int32_t prev_synch_fix = (time_to_next_superframe - remaining_time_own);
        // After network scan counter was initialized to -1 to drop this fix from monitoring
        if (fhss_structure->synch_monitor.avg_synch_fix_counter >= 0) {
            fhss_structure->synch_monitor.avg_synch_fix += prev_synch_fix;
        }
        fhss_structure->synch_monitor.avg_synch_fix_counter++;
        if (fhss_structure->synch_monitor.avg_synch_fix_counter == SYNCH_MONITOR_AVG_SAMPLES) {
            fhss_structure->synch_monitor.avg_synch_fix /= SYNCH_MONITOR_AVG_SAMPLES;
            fhss_stats_update(fhss_structure, STATS_FHSS_AVG_SYNCH_FIX, fhss_structure->synch_monitor.avg_synch_fix);
            retval = fhss_update_drift_compensation(fhss_structure);
            fhss_structure->synch_monitor.avg_synch_fix_counter = 0;
            fhss_structure->synch_monitor.avg_synch_fix = 0;
        }
    }
    return retval;
}

int fhss_sync_with_beacon(fhss_structure_t *fhss_structure,
        const fhss_synchronization_beacon_payload_s *payload)
{
    int ret_val = -1;
    int32_t remaining_time_own;
    uint8_t superframe_own;

    if (fhss_structure) {
        // Do not allow synchronising devices above 253 hops.
        if (payload->hop_count > 253) {
            return 0;
        }
        // To make synchronization monitoring more effective, drop extra Beacons.
        if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED && (fhss_is_current_channel_broadcast(fhss_structure) == false || (fhss_structure->beacon_received_on_this_bc_channel == true))) {
            return 0;
        }

        fhss_synch_configuration_t *configuration = &fhss_structure->synch_configuration;
        fhss_structure->beacon_received_on_this_bc_channel = true;

        superframe_own = fhss_structure->current_superframe;
        fhss_structure->current_superframe = payload->current_superframe;
        // Clone the static config values from parent which has the authority.
        configuration->fhss_number_of_bc_channels = payload->number_of_broadcast_channels;
        configuration->fhss_number_of_tx_slots = payload->number_of_tx_slots;
        configuration->fhss_superframe_length = payload->superframe_length;
        configuration->fhss_number_of_superframes = payload->number_of_superframes_per_channel;
        // todo:
        // * payload->time_since_last_beacon
        uint8_t own_hop_tmp = fhss_structure->own_hop;
        if (fhss_is_synch_root(fhss_structure) == false) {
            // my own hop count is one more than the parent's
            fhss_structure->own_hop = payload->hop_count + 1;
        }
        fhss_stats_update(fhss_structure, STATS_FHSS_HOP_COUNT, fhss_structure->own_hop);
        fhss_structure->channel_list_counter = payload->channel_list_counter;
        fhss_structure->current_channel_index = payload->channel_index;
        uint8_t mac_address[8];
        fhss_structure->callbacks.read_mac_address(fhss_structure->fhss_api, mac_address);

        fhss_structure->uc_channel_index = fhss_calculate_uc_index(fhss_structure->current_channel_index, fhss_structure->number_of_channels,
                payload->number_of_broadcast_channels) + fhss_get_offset(fhss_structure, mac_address);

        // If current channel is not broadcast, fhss_update_channel will increase UC channel index, otherwise do it here
        if (fhss_is_current_channel_broadcast(fhss_structure) == true || (fhss_structure->fhss_state == FHSS_SYNCHRONIZED)) {
            fhss_structure->uc_channel_index += 1;
        }
        if (fhss_structure->uc_channel_index >= (fhss_structure->number_of_channels - payload->number_of_broadcast_channels)) {
            fhss_structure->uc_channel_index -= (fhss_structure->number_of_channels - payload->number_of_broadcast_channels);
        }

        fhss_structure->platform_functions.fhss_timer_stop(fhss_structure->fhss_api);
        // start timer to elapse at approximately same time as the parent will.
        const int32_t time_to_next_superframe = payload->remaining_slots;
        remaining_time_own = fhss_get_remaining_time_to_next_superframe(fhss_structure);
        fhss_start_timer(fhss_structure, time_to_next_superframe, fhss_superframe_handler);
        // Reset beacon received timer when FHSS synchronization is updated
        fhss_structure->beacons_received_timer = 0;
        uint16_t bc_density = (fhss_structure->number_of_channels / fhss_structure->synch_configuration.fhss_number_of_bc_channels);
        uint8_t fhss_number_of_bc_channels = fhss_structure->synch_configuration.fhss_number_of_bc_channels;
        uint16_t channel_dwell_time = ((uint32_t)fhss_structure->synch_configuration.fhss_superframe_length * fhss_structure->synch_configuration.fhss_number_of_superframes) / 1000;

        if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
            fhss_structure->fhss_state = FHSS_SYNCHRONIZED;
            fhss_update_channel(fhss_structure);
        }
        if (fhss_is_synch_root(fhss_structure) == false) {
            // Initially synch drift might be massive. Request first few Beacons more frequently until compensation starts fixing the error.
            uint32_t beacon_interval_random;

            if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
                if (fhss_update_synch_monitor(fhss_structure, payload, superframe_own, remaining_time_own, time_to_next_superframe)) {
                    fhss_structure->synch_interval = (uint32_t) (fhss_structure->fhss_configuration.fhss_max_synch_interval/BEACON_INTERVAL_INIT_DIVIDER) * 1000;
                }
            }

            if (fhss_structure->synch_interval != ((uint32_t)fhss_structure->fhss_configuration.fhss_max_synch_interval * 1000)) {
                fhss_structure->synch_interval *= 2;
                if (fhss_structure->synch_interval > ((uint32_t)fhss_structure->fhss_configuration.fhss_max_synch_interval * 1000)) {
                    fhss_structure->synch_interval = ((uint32_t)fhss_structure->fhss_configuration.fhss_max_synch_interval * 1000);
                }
                beacon_interval_random = (bc_density * channel_dwell_time) * randLIB_get_random_in_range(0, fhss_number_of_bc_channels/2);
            } else {
                beacon_interval_random = (bc_density * channel_dwell_time) * randLIB_get_random_in_range(0, fhss_number_of_bc_channels);
            }
            fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_INTERVAL, fhss_structure->synch_interval / 1000);
            fhss_beacon_periodic_start(fhss_structure, fhss_structure->synch_interval + beacon_interval_random);

#ifdef FHSS_MASSIVE_TRACE
            tr_debug("start timer, time now: %"PRIu32", sfr: %"PRIu8", ch-ind: %"PRIu8", time after: %"PRIu32,
                    fhss_structure->fhss_api->read_timestamp(fhss_structure->fhss_api), fhss_structure->current_superframe,
                    payload->channel_index, time_to_next_superframe);
#endif
        }
        // Our hop has changed, needs to inform possible children by sending Beacon
        if ((own_hop_tmp != 0) && (own_hop_tmp != fhss_structure->own_hop)) {
            fhss_structure->send_synch_info_on_next_broadcast_channel = true;
        }
        ret_val = 0;
    }
    return ret_val;
}

static uint32_t fhss_get_remaining_tx_time(fhss_structure_t *fhss_structure)
{
    /* To get the remaining TX time, following calculation is performed
     *
     *         rem. time(Y)
     *        |----------------|
     *...|    TX    |    TX    |    RX    |    RX    |...
     *...|   SF n   |   SF n+1 |   SF n+2 |   SF n+3 |...
     *
     * Y = remaining time of SF(n) + length of SF(n+1)
     *
     */

    uint32_t remaining_tx_time = 0;

    if (fhss_structure) {
        uint8_t cur_superframe = fhss_structure->current_superframe;
        uint8_t number_of_tx_slots = fhss_structure->synch_configuration.fhss_number_of_tx_slots;
        uint8_t number_of_superframes = fhss_structure->synch_configuration.fhss_number_of_superframes;
        uint8_t tx_slot_length = ((number_of_superframes / 2) / number_of_tx_slots);
        uint8_t tx_slot_up_limit = tx_slot_length;
        uint16_t superframe_length = fhss_structure->synch_configuration.fhss_superframe_length;

        if ((fhss_structure->own_hop % 2)) {
            tx_slot_up_limit += tx_slot_length;
        }
        while(number_of_tx_slots--)
        {
            if ((cur_superframe >= (tx_slot_up_limit - tx_slot_length)) && (cur_superframe < tx_slot_up_limit)) {
                remaining_tx_time = ((uint32_t)((tx_slot_up_limit - 1) - cur_superframe) * superframe_length) + fhss_get_remaining_time_to_next_superframe(fhss_structure);
                break;
            } else {
                tx_slot_up_limit += (tx_slot_length * 2);
            }
        }
    }
    return remaining_tx_time;
}

uint32_t fhss_get_tx_time(fhss_structure_t *fhss_structure, uint16_t bytes_to_send, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    return ((1000000 / (fhss_structure->datarate / 8)) * (bytes_to_send + phy_header_length + phy_tail_length));
}

// CCA adds extra 2ms with FHSS
#define CCA_FHSS_PERIOD 2000
// Ack frame length
#define ACK_LENGTH  5

static bool fhss_check_remaining_tx_time(fhss_structure_t *fhss_structure, uint16_t tx_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    bool retval = false;
    uint32_t remaining_tx_time;
    uint32_t needed_tx_time;
    uint32_t tx_processing_delay;
    uint32_t ack_processing_delay;

    if (!fhss_structure) {
        return retval;
    }

    // Don't care when FHSS is unsynchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        retval = true;
    } else {
        tx_processing_delay = fhss_structure->fhss_configuration.fhss_tuning_parameters.tx_processing_delay;
        ack_processing_delay = fhss_structure->fhss_configuration.fhss_tuning_parameters.ack_processing_delay;
        // Calculate needed TX time (us): CCA static period + TX processing delays + transmission time + Ack processing delays + Ack transmission time
        needed_tx_time = CCA_FHSS_PERIOD + tx_processing_delay + fhss_get_tx_time(fhss_structure, tx_length, phy_header_length, phy_tail_length)
                        + ack_processing_delay + fhss_get_tx_time(fhss_structure, ACK_LENGTH, phy_header_length, phy_tail_length);
        remaining_tx_time = fhss_get_remaining_tx_time(fhss_structure);
        if(needed_tx_time <= remaining_tx_time) {
            retval = true;
        }
    }
    return retval;
}

int fhss_update_synch_parent_address(fhss_structure_t *fhss_structure)
{
    uint8_t parent_address[8];

    if (!fhss_get_parent_address(fhss_structure, parent_address)) {
        memcpy(fhss_structure->synch_parent, parent_address, 8);
        return 0;
    }
    return -1;
}

void fhss_trig_event(fhss_structure_t *fhss_structure, uint8_t event_type)
{
    if (fhss_read_active_event(fhss_structure, event_type) == true) {
        return;
    }
    arm_event_s event;
    event.receiver = fhss_structure->beacon_tasklet_id;
    event.sender = 0;
    event.event_type = event_type;
    event.event_id = 0;
    event.data_ptr = fhss_structure;
    event.priority = ARM_LIB_HIGH_PRIORITY_EVENT;
    event.event_data = 0;
    if (eventOS_event_send(&event) != 0) {
        tr_error("Event trigger failed: eventOS_event_send() failed");
    } else {
        fhss_set_active_event(fhss_structure, event_type);
    }
}

int fhss_get_parent_address(fhss_structure_t *fhss_structure, uint8_t *p_addr)
{
    int ret_val = -1;
    if (!fhss_structure || !p_addr) {
        return -1;
    }

    ret_val = fhss_structure->callbacks.read_coord_mac_address(fhss_structure->fhss_api, p_addr);

    if (ret_val) {
        // Use default synchronization parent when RPL parent not found
        memcpy(p_addr, fhss_structure->synch_parent, 8);
        ret_val = 0;
    }
    return ret_val;
}

int fhss_compare_with_synch_parent_address(fhss_structure_t *fhss_structure, const uint8_t *source_addr)
{
    int ret_val = -1;
    if (!fhss_structure || !source_addr) {
        return ret_val;
    }
    uint8_t parent_address[8];

    if (fhss_is_synch_root(fhss_structure) == false) {
        if (!fhss_get_parent_address(fhss_structure, parent_address)) {
            ret_val = memcmp(source_addr, parent_address, 8);
        }
    }
    return ret_val;
}

static void fhss_update_channel(fhss_structure_t *fhss_structure)
{
    // If channel is broadcast channel (true), send event
    if (fhss_change_to_next_channel(fhss_structure) == true) {
        // Only if device is border router
        if (fhss_structure->own_hop == 0) {
            fhss_trig_event(fhss_structure, FHSS_BROADCAST_CHANNEL);
        }
    }
}

static bool fhss_is_there_common_divisor(uint16_t i, uint8_t j)
{
    if (i < j) {
        uint_fast8_t t = j; j = (uint8_t) i; i = t;
    }
    for(uint_fast8_t k=2; k<=j; k++) {
        if (i % k == 0 && j % k == 0) {
            return true;
        }
    }
    return false;
}


static int fhss_generate_scramble_table(fhss_structure_t *fhss_structure)
{
    uint8_t j = 2;
    // Generate indexes to extend the channel sequence. Generated indexes cannot have common divisors with value number_of_channels.
    for(int i=0; i<MAX_SCRAMBLE_TABLE_INDEXES;)
    {
        // Common divisors are skipped
        if (fhss_is_there_common_divisor(fhss_structure->number_of_channels, j) == false) {
            fhss_structure->fhss_scramble_table[i] = j;
            i++;
        }
        j++;
    }
    return 0;
}

static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots)
{
    (void) slots;
    fhss_structure_t *fhss_structure = fhss_get_object_with_timer_id(timer_id);
    if (fhss_structure) {
        fhss_structure->callbacks.tx_poll(fhss_structure->fhss_api);
    }
}

fhss_beacon_info_t *fhss_get_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id)
{
    fhss_beacon_info_t *beacon_info;
    beacon_info = fhss_structure->fhss_beacon_info_store;
    while (beacon_info) {
        if (beacon_info->pan_id == pan_id) {
            break;
        }
        beacon_info = beacon_info->next;
    }
    return beacon_info;
}

static void fhss_store_beacon_info(fhss_structure_t *fhss_structure, fhss_beacon_info_t *beacon_info)
{
    fhss_beacon_info_t *beacon_info_cur;
    beacon_info->next = NULL;
    if (!fhss_structure->fhss_beacon_info_store) {
        fhss_structure->fhss_beacon_info_store = beacon_info;
        return;
    }
    beacon_info_cur = fhss_structure->fhss_beacon_info_store;
    while (beacon_info_cur->next != NULL) {
        beacon_info_cur = beacon_info_cur->next;
    }
    beacon_info_cur->next = beacon_info;
}

static void fhss_write_beacon_info(fhss_beacon_info_t *beacon_info, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info)
{
    beacon_info->pan_id = pan_id;
    memcpy(beacon_info->source_address, source_address, 8);
    beacon_info->timestamp = timestamp;
    memcpy(beacon_info->synch_info, synch_info, sizeof(beacon_info->synch_info));
}

static fhss_beacon_info_t *fhss_create_beacon_info(fhss_structure_t *fhss_structure)
{
    fhss_beacon_info_t *beacon_info = ns_dyn_mem_temporary_alloc(sizeof(fhss_beacon_info_t));
    if (!beacon_info) {
        return NULL;
    }
    fhss_store_beacon_info(fhss_structure, beacon_info);
    return beacon_info;
}

static int fhss_remove_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id)
{
    if (!fhss_structure || !fhss_structure->fhss_beacon_info_store) {
        return -1;
    }

    if (fhss_structure->fhss_beacon_info_store->pan_id == pan_id) {
        fhss_beacon_info_t *next = fhss_structure->fhss_beacon_info_store->next;
        ns_dyn_mem_free(fhss_structure->fhss_beacon_info_store);
        fhss_structure->fhss_beacon_info_store = next;
        return 0;
    }

    fhss_beacon_info_t *removed_beacon_info = fhss_structure->fhss_beacon_info_store->next;
    fhss_beacon_info_t *prev_beacon_info = fhss_structure->fhss_beacon_info_store;

    while (removed_beacon_info) {
        if (removed_beacon_info->pan_id == pan_id) {
            fhss_beacon_info_t *next = removed_beacon_info->next;
            ns_dyn_mem_free(removed_beacon_info);
            prev_beacon_info->next = next;
            return 0;
        }
        prev_beacon_info = removed_beacon_info;
        removed_beacon_info = removed_beacon_info->next;
    }
    return -1;
}

int fhss_flush_beacon_info_storage(fhss_structure_t *fhss_structure)
{
    if (!fhss_structure) {
        return -1;
    }
    fhss_beacon_info_t *beacon_info = fhss_structure->fhss_beacon_info_store;
    while (beacon_info) {
        fhss_beacon_info_t *next = beacon_info->next;
        ns_dyn_mem_free(beacon_info);
        beacon_info = next;
    }
    fhss_structure->fhss_beacon_info_store = NULL;
    return 0;
}

int fhss_add_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info)
{
    if (!fhss_structure || !source_address || !synch_info) {
        return -1;
    }
    fhss_beacon_info_t *beacon_info = fhss_get_beacon_info(fhss_structure, pan_id);
    if (!beacon_info) {
        beacon_info = fhss_create_beacon_info(fhss_structure);
    }
    if (!beacon_info) {
        tr_error("Beacon data not allocated");
        return -2;
    }
    fhss_write_beacon_info(beacon_info, pan_id, source_address, timestamp, synch_info);
    return 0;
}

void fhss_update_beacon_info_lifetimes(fhss_structure_t *fhss_structure, uint32_t timestamp)
{
    fhss_beacon_info_t *beacon_info;
    beacon_info = fhss_structure->fhss_beacon_info_store;
    while (beacon_info) {
        uint32_t time_since_added = timestamp - beacon_info->timestamp;
        // timestamp is microseconds, lifetime is seconds
        if (time_since_added >= ((uint32_t)BEACON_INFO_LIFETIME * 1000000)) {
            if (fhss_remove_beacon_info(fhss_structure, beacon_info->pan_id) == 0) {
                return;
            }
        }
        beacon_info = beacon_info->next;
    }
}

fhss_failed_tx_t *fhss_failed_handle_find(fhss_structure_t *fhss_structure, uint8_t handle)
{
    ns_list_foreach(fhss_failed_tx_t, cur, &fhss_structure->fhss_failed_tx_list) {
        if (cur->handle == handle) {
            return cur;
        }
    }
    return NULL;
}

int fhss_failed_handle_add(fhss_structure_t *fhss_structure, uint8_t handle)
{
    fhss_failed_tx_t *failed_tx = ns_dyn_mem_alloc(sizeof(fhss_failed_tx_t));
    if (!failed_tx) {
        return -2;
    }
    failed_tx->bad_channel = fhss_structure->rx_channel;
    failed_tx->retries_done = 0;
    failed_tx->handle = handle;
    ns_list_add_to_end(&fhss_structure->fhss_failed_tx_list, failed_tx);
    return 0;
}

int fhss_failed_handle_remove(fhss_structure_t *fhss_structure, uint8_t handle)
{
    fhss_failed_tx_t *failed_tx = fhss_failed_handle_find(fhss_structure, handle);
    if (!failed_tx) {
        return -1;
    }
    ns_list_remove(&fhss_structure->fhss_failed_tx_list, failed_tx);
    ns_dyn_mem_free(failed_tx); // Free entry
    return 0;
}

static void fhss_failed_list_free(fhss_structure_t *fhss_structure)
{
    for (uint16_t i = 0; i<256; i++) {
        fhss_failed_handle_remove(fhss_structure, i);
    }
}
