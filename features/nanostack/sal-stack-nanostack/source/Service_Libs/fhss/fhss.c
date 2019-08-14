/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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
#include "fhss_channel.h"
#include "channel_list.h"
#include "nsdynmemLIB.h"
#include "fhss_statistics.h"
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_callback_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "randLIB.h"
#include "common_functions.h"
#include <string.h>

#define TRACE_GROUP "fhss"

static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots);
static int fhss_reset(fhss_structure_t *fhss_structure);
static bool fhss_is_bc_sending_superframe(fhss_structure_t *fhss_structure);
static bool fhss_check_remaining_tx_time(fhss_structure_t *fhss_structure, uint16_t tx_length, uint8_t phy_header_length, uint8_t phy_tail_length);
static bool fhss_is_there_common_divisor(uint16_t i, uint8_t j);
static void fhss_beacon_received(fhss_structure_t *fhss_structure, const uint8_t *synch_info, const uint32_t elapsed_time);
static void fhss_superframe_handler(const fhss_api_t *fhss_api, uint16_t delay);
static int8_t fhss_beacon_create_tasklet(fhss_structure_t *fhss_structure);
static void fhss_beacon_tasklet_func(arm_event_s *event);
static int fhss_beacon_periodic_start(fhss_structure_t *fhss_structure, uint32_t time_to_first_beacon);
static void fhss_beacon_periodic_stop(fhss_structure_t *fhss_structure);
static int fhss_reset_synch_monitor(fhss_synch_monitor_s *synch_monitor);

fhss_structure_t *fhss_enable(fhss_api_t *fhss_api, const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics)
{
    if (!fhss_api || !fhss_configuration || !fhss_timer) {
        tr_err("Invalid FHSS enable configuration");
        return NULL;
    }
    int channel_count = channel_list_count_channels(fhss_configuration->channel_mask);
    if (channel_count <= 0) {
        // There must be at least one configured channel in channel list
        return NULL;
    }
    fhss_structure_t *fhss_struct = fhss_allocate_instance(fhss_api, fhss_timer);
    if (!fhss_struct) {
        return NULL;
    }
    fhss_struct->bs = ns_dyn_mem_alloc(sizeof(fhss_bs_t));
    if (!fhss_struct->bs) {
        fhss_free_instance(fhss_api);
        return NULL;
    }
    memset(fhss_struct->bs, 0, sizeof(fhss_bs_t));

    fhss_struct->fhss_event_timer = eventOS_callback_timer_register(fhss_event_timer_cb);
    fhss_struct->bs->fhss_configuration = *fhss_configuration;
    fhss_struct->fhss_stats_ptr = fhss_statistics;
    fhss_struct->number_of_channels = channel_count;

    // set a invalid id to tasklet_id, so we know that one is not started yet
    fhss_struct->beacon_tasklet_id = -1;

    // Default synch interval is 240 seconds
    if (!fhss_struct->bs->fhss_configuration.fhss_max_synch_interval) {
        fhss_struct->bs->fhss_configuration.fhss_max_synch_interval = 240;
    }
    fhss_reset_synch_monitor(&fhss_struct->bs->synch_monitor);
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    fhss_struct->own_hop = 0xff;
    fhss_reset(fhss_struct);

    if (fhss_beacon_create_tasklet(fhss_struct) < 0) {
        ns_dyn_mem_free(fhss_struct->bs);
        fhss_free_instance(fhss_api);
        return NULL;
    }

    return fhss_struct;
}

bool fhss_is_synch_root(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->own_hop > 0) {
        return false;
    }
    return true;
}

static void fhss_event_timer_cb(int8_t timer_id, uint16_t slots)
{
    (void) slots;
    fhss_structure_t *fhss_structure = fhss_get_object_with_timer_id(timer_id);
    if (fhss_structure) {
        fhss_structure->callbacks.tx_poll(fhss_structure->fhss_api);
    }
}

static bool fhss_is_bc_sending_superframe(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->bs->current_superframe >= fhss_structure->bs->broadcast_start_superframe) {
        return true;
    }

    return false;
}

static bool fhss_check_bad_channel(fhss_structure_t *fhss_structure, uint8_t handle)
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

static bool fhss_check_channel_type(fhss_structure_t *fhss_structure, bool is_bc, int frame_type)
{
    if (!fhss_structure) {
        return false;
    }

    // Channel type check is valid only when FHSS is synchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
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
            if (fhss_structure->bs->beacons_received_timer) {
                return false;
            }
        }
    }
    if (frame_type == FHSS_SYNCH_FRAME) {

    }
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME) {
        // Drop: If we have unicast channels in our configuration and current channel is broadcast channel
        if ((fhss_structure->number_of_channels > fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) && (fhss_is_current_channel_broadcast(fhss_structure) == true)) {
            return false;
        }

        uint8_t current_superframe = fhss_structure->bs->current_superframe;
        uint8_t synch_attempt = fhss_structure->bs->beacons_received_timer;
        if (fhss_structure->bs->synch_configuration.fhss_number_of_tx_slots > 1) {
            // Send synch request either before or after the middle of the channel depending on attempt number.
            uint8_t middle_of_superframes = fhss_structure->bs->synch_configuration.fhss_number_of_superframes / 2;

            if (synch_attempt & 1) {
                if (fhss_structure->bs->current_superframe < middle_of_superframes) {
                    return false;
                }
            } else {
                if (fhss_structure->bs->current_superframe >= middle_of_superframes) {
                    return false;
                }
            }
        } else if ((current_superframe == 0) || (current_superframe == (fhss_structure->bs->synch_configuration.fhss_number_of_superframes - 1))) {
            return false;
        }
    }
    return true;

}

static bool fhss_check_tx_allowed(fhss_structure_t *fhss_structure, bool is_bc, uint16_t frame_length, int frame_type, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    if (!fhss_structure) {
        return false;
    }
    // Check is valid only when FHSS is synchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
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
    if (fhss_structure->bs->tx_allowed == false) {
        return false;
    }
    // Deny: If not enough time before TX slot end
    if (fhss_check_remaining_tx_time(fhss_structure, frame_length, phy_header_length, phy_tail_length) == false) {
        return false;
    }
    return true;
}

static int fhss_reset_synch_monitor(fhss_synch_monitor_s *synch_monitor)
{
    if (synch_monitor) {
        synch_monitor->avg_synch_fix = 0;
        // Initialize to -1 instead of 0 to drop the first beacon after network scan (from synch monitoring)
        synch_monitor->avg_synch_fix_counter = -1;
        synch_monitor->channel_counter = 0;
        return 0;
    }
    return -1;
}

static int fhss_update_txrx_slots(fhss_structure_t *fhss_structure)
{
    uint8_t cur_superframe = fhss_structure->bs->current_superframe;
    uint8_t number_of_tx_slots = fhss_structure->bs->synch_configuration.fhss_number_of_tx_slots;
    uint8_t number_of_superframes = fhss_structure->bs->synch_configuration.fhss_number_of_superframes;
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
    while (number_of_tx_slots--) {
        if ((cur_superframe >= (tx_slot_up_limit - tx_slot_length)) && (cur_superframe < tx_slot_up_limit)) {
            tx_allowed = true;
            break;
        } else {
            tx_slot_up_limit += (tx_slot_length * 2);
        }
    }
#ifdef FHSS_CHANNEL_DEBUG_CBS
    if (fhss_bc_switch && fhss_structure->bs->tx_allowed != tx_allowed) {
        fhss_bc_switch();
    }
#endif /*FHSS_CHANNEL_DEBUG_CBS*/
    fhss_structure->bs->tx_allowed = tx_allowed;
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
    bc_density = (fhss_structure->number_of_channels / fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
    channel_dwell_time = ((uint32_t)fhss_structure->bs->synch_configuration.fhss_superframe_length * fhss_structure->bs->synch_configuration.fhss_number_of_superframes) / 1000;
    // Calculate last synchronisation period
    if (fhss_structure->bs->synch_interval != ((uint32_t)fhss_structure->bs->fhss_configuration.fhss_max_synch_interval * 1000)) {
        // Last period was half of the current started period and max random period is shorter
        synch_period = (fhss_structure->bs->synch_interval / 2) + (bc_density * channel_dwell_time) * (fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels / 2);
    } else {
        synch_period = fhss_structure->bs->synch_interval + (bc_density * channel_dwell_time) * fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels;
    }

    // E.g. 240000ms / (50000us * 8) = 600 channels per fhss_beacon_send_interval
    channels_per_synch_period = (synch_period * 1000) / ((uint32_t)fhss_structure->bs->synch_configuration.fhss_superframe_length * fhss_structure->bs->synch_configuration.fhss_number_of_superframes);
    // Calculate compensation value: how much to compensate on each channel. E.g. 6000us / 600channels = 10us/channel
    fhss_structure->bs->synch_monitor.drift_compensation += (fhss_structure->bs->synch_monitor.avg_synch_fix / channels_per_synch_period);
    fhss_stats_update(fhss_structure, STATS_FHSS_DRIFT_COMP, fhss_structure->bs->synch_monitor.drift_compensation);
    if ((fhss_structure->bs->synch_monitor.avg_synch_fix > FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT) || (fhss_structure->bs->synch_monitor.avg_synch_fix < -FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT)) {
        // Indicates that more frequent synchronization is needed
        retval = -1;
    }
    return retval;
}

static int fhss_update_synch_monitor(fhss_structure_t *fhss_structure, const fhss_synchronization_beacon_payload_s *payload, uint8_t superframe_own, int32_t remaining_time_own, const int32_t time_to_next_superframe)
{
    fhss_synch_configuration_t *configuration = &fhss_structure->bs->synch_configuration;
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
        if (fhss_structure->bs->synch_monitor.avg_synch_fix_counter >= 0) {
            fhss_structure->bs->synch_monitor.avg_synch_fix += prev_synch_fix;
        }
        fhss_structure->bs->synch_monitor.avg_synch_fix_counter++;
        if (fhss_structure->bs->synch_monitor.avg_synch_fix_counter == SYNCH_MONITOR_AVG_SAMPLES) {
            fhss_structure->bs->synch_monitor.avg_synch_fix /= SYNCH_MONITOR_AVG_SAMPLES;
            fhss_stats_update(fhss_structure, STATS_FHSS_AVG_SYNCH_FIX, fhss_structure->bs->synch_monitor.avg_synch_fix);
            retval = fhss_update_drift_compensation(fhss_structure);
            fhss_structure->bs->synch_monitor.avg_synch_fix_counter = 0;
            fhss_structure->bs->synch_monitor.avg_synch_fix = 0;
        }
    }
    return retval;
}

static int fhss_sync_with_beacon(fhss_structure_t *fhss_structure,
                                 const fhss_synchronization_beacon_payload_s *payload)
{
    int ret_val = -1;
    int32_t remaining_time_own;
    uint8_t superframe_own;

    if (fhss_structure) {
        // Do not allow synchronising devices above 253 hops.
        if (payload->hop_count > (FHSS_MAX_ALLOWED_HOPS - 1)) {
            return 0;
        }
        // To make synchronization monitoring more effective, drop extra Beacons.
        if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED && (fhss_is_current_channel_broadcast(fhss_structure) == false || (fhss_structure->bs->beacon_received_on_this_bc_channel == true))) {
            return 0;
        }

        fhss_synch_configuration_t *configuration = &fhss_structure->bs->synch_configuration;
        fhss_structure->bs->beacon_received_on_this_bc_channel = true;

        superframe_own = fhss_structure->bs->current_superframe;
        fhss_structure->bs->current_superframe = payload->current_superframe;
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
        fhss_structure->bs->channel_list_counter = payload->channel_list_counter;
        fhss_structure->bs->current_channel_index = payload->channel_index;
        uint8_t mac_address[8];
        fhss_structure->callbacks.read_mac_address(fhss_structure->fhss_api, mac_address);

        fhss_structure->bs->uc_channel_index = fhss_calculate_uc_index(fhss_structure->bs->current_channel_index, fhss_structure->number_of_channels,
                                                                       payload->number_of_broadcast_channels) + fhss_get_offset(fhss_structure, mac_address);

        // If current channel is not broadcast, fhss_update_channel will increase UC channel index, otherwise do it here
        if (fhss_is_current_channel_broadcast(fhss_structure) == true || (fhss_structure->fhss_state == FHSS_SYNCHRONIZED)) {
            fhss_structure->bs->uc_channel_index += 1;
        }
        if (fhss_structure->bs->uc_channel_index >= (fhss_structure->number_of_channels - payload->number_of_broadcast_channels)) {
            fhss_structure->bs->uc_channel_index -= (fhss_structure->number_of_channels - payload->number_of_broadcast_channels);
        }

        fhss_structure->platform_functions.fhss_timer_stop(fhss_superframe_handler, fhss_structure->fhss_api);
        // start timer to elapse at approximately same time as the parent will.
        const int32_t time_to_next_superframe = payload->remaining_slots;
        remaining_time_own = fhss_get_remaining_time_to_next_superframe(fhss_structure);
        fhss_start_timer(fhss_structure, time_to_next_superframe, fhss_superframe_handler);
        // Reset beacon received timer when FHSS synchronization is updated
        fhss_structure->bs->beacons_received_timer = 0;
        uint16_t bc_density = (fhss_structure->number_of_channels / fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
        uint8_t fhss_number_of_bc_channels = fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels;
        uint16_t channel_dwell_time = ((uint32_t)fhss_structure->bs->synch_configuration.fhss_superframe_length * fhss_structure->bs->synch_configuration.fhss_number_of_superframes) / 1000;

        if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
            fhss_structure->fhss_state = FHSS_SYNCHRONIZED;
            fhss_change_to_next_channel(fhss_structure);
        }
        if (fhss_is_synch_root(fhss_structure) == false) {
            // Initially synch drift might be massive. Request first few Beacons more frequently until compensation starts fixing the error.
            uint32_t beacon_interval_random;

            if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
                if (fhss_update_synch_monitor(fhss_structure, payload, superframe_own, remaining_time_own, time_to_next_superframe)) {
                    fhss_structure->bs->synch_interval = (uint32_t)(fhss_structure->bs->fhss_configuration.fhss_max_synch_interval / BEACON_INTERVAL_INIT_DIVIDER) * 1000;
                }
            }

            if (fhss_structure->bs->synch_interval != ((uint32_t)fhss_structure->bs->fhss_configuration.fhss_max_synch_interval * 1000)) {
                fhss_structure->bs->synch_interval *= 2;
                if (fhss_structure->bs->synch_interval > ((uint32_t)fhss_structure->bs->fhss_configuration.fhss_max_synch_interval * 1000)) {
                    fhss_structure->bs->synch_interval = ((uint32_t)fhss_structure->bs->fhss_configuration.fhss_max_synch_interval * 1000);
                }
                beacon_interval_random = (bc_density * channel_dwell_time) * randLIB_get_random_in_range(0, fhss_number_of_bc_channels / 2);
            } else {
                beacon_interval_random = (bc_density * channel_dwell_time) * randLIB_get_random_in_range(0, fhss_number_of_bc_channels);
            }
            fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_INTERVAL, fhss_structure->bs->synch_interval / 1000);
            fhss_beacon_periodic_start(fhss_structure, fhss_structure->bs->synch_interval + beacon_interval_random);
        }
        // Our hop has changed, needs to inform possible children by sending Beacon
        if ((own_hop_tmp != 0) && (own_hop_tmp != fhss_structure->own_hop)) {
            fhss_structure->bs->send_synch_info_on_next_broadcast_channel = true;
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
        uint8_t cur_superframe = fhss_structure->bs->current_superframe;
        uint8_t number_of_tx_slots = fhss_structure->bs->synch_configuration.fhss_number_of_tx_slots;
        uint8_t number_of_superframes = fhss_structure->bs->synch_configuration.fhss_number_of_superframes;
        uint8_t tx_slot_length = ((number_of_superframes / 2) / number_of_tx_slots);
        uint8_t tx_slot_up_limit = tx_slot_length;
        uint16_t superframe_length = fhss_structure->bs->synch_configuration.fhss_superframe_length;

        if ((fhss_structure->own_hop % 2)) {
            tx_slot_up_limit += tx_slot_length;
        }
        while (number_of_tx_slots--) {
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
        tx_processing_delay = fhss_structure->bs->fhss_configuration.fhss_tuning_parameters.tx_processing_delay;
        ack_processing_delay = fhss_structure->bs->fhss_configuration.fhss_tuning_parameters.ack_processing_delay;
        // Calculate needed TX time (us): CCA static period + TX processing delays + transmission time + Ack processing delays + Ack transmission time
        needed_tx_time = CCA_FHSS_PERIOD + tx_processing_delay + fhss_get_tx_time(fhss_structure, tx_length, phy_header_length, phy_tail_length)
                         + ack_processing_delay + fhss_get_tx_time(fhss_structure, ACK_LENGTH, phy_header_length, phy_tail_length);
        remaining_tx_time = fhss_get_remaining_tx_time(fhss_structure);
        if (needed_tx_time <= remaining_tx_time) {
            retval = true;
        }
    }
    return retval;
}

static bool fhss_is_there_common_divisor(uint16_t i, uint8_t j)
{
    if (i < j) {
        uint_fast8_t t = j;
        j = (uint8_t) i;
        i = t;
    }
    for (uint_fast8_t k = 2; k <= j; k++) {
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
    for (int i = 0; i < MAX_SCRAMBLE_TABLE_INDEXES;) {
        // Common divisors are skipped
        if (fhss_is_there_common_divisor(fhss_structure->number_of_channels, j) == false) {
            fhss_structure->bs->fhss_scramble_table[i] = j;
            i++;
        }
        j++;
    }
    return 0;
}

static fhss_beacon_info_t *fhss_get_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id)
{
    fhss_beacon_info_t *beacon_info;
    beacon_info = fhss_structure->bs->fhss_beacon_info_store;
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
    if (!fhss_structure->bs->fhss_beacon_info_store) {
        fhss_structure->bs->fhss_beacon_info_store = beacon_info;
        return;
    }
    beacon_info_cur = fhss_structure->bs->fhss_beacon_info_store;
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
    if (!fhss_structure || !fhss_structure->bs->fhss_beacon_info_store) {
        return -1;
    }

    if (fhss_structure->bs->fhss_beacon_info_store->pan_id == pan_id) {
        fhss_beacon_info_t *next = fhss_structure->bs->fhss_beacon_info_store->next;
        ns_dyn_mem_free(fhss_structure->bs->fhss_beacon_info_store);
        fhss_structure->bs->fhss_beacon_info_store = next;
        return 0;
    }

    fhss_beacon_info_t *removed_beacon_info = fhss_structure->bs->fhss_beacon_info_store->next;
    fhss_beacon_info_t *prev_beacon_info = fhss_structure->bs->fhss_beacon_info_store;

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

static int fhss_flush_beacon_info_storage(fhss_structure_t *fhss_structure)
{
    if (!fhss_structure) {
        return -1;
    }
    fhss_beacon_info_t *beacon_info = fhss_structure->bs->fhss_beacon_info_store;
    while (beacon_info) {
        fhss_beacon_info_t *next = beacon_info->next;
        ns_dyn_mem_free(beacon_info);
        beacon_info = next;
    }
    fhss_structure->bs->fhss_beacon_info_store = NULL;
    return 0;
}

static int fhss_reset(fhss_structure_t *fhss_structure)
{
    if (!fhss_structure) {
        return -1;
    }
    fhss_structure->platform_functions.fhss_timer_stop(fhss_superframe_handler, fhss_structure->fhss_api);
    fhss_structure->bs->synch_panid = 0xffff;
    fhss_beacon_periodic_stop(fhss_structure);
    fhss_structure->bs->current_superframe = 0;
    fhss_structure->bs->current_channel_index = 0;
    fhss_structure->bs->channel_list_counter = 0;
    if (fhss_is_synch_root(fhss_structure) == false) {
        fhss_structure->own_hop = 0xff;
    }
    fhss_structure->bs->tx_allowed = false;
    fhss_structure->bs->synch_interval = (uint32_t)(fhss_structure->bs->fhss_configuration.fhss_max_synch_interval / BEACON_INTERVAL_INIT_DIVIDER) * 1000;
    fhss_structure->rx_channel = 0;
    fhss_structure->bs->beacons_received_timer = 0;
    memset(fhss_structure->synch_parent, 0xff, 8);
    fhss_structure->bs->send_synch_info_on_next_broadcast_channel = false;
    memset(&fhss_structure->bs->synch_configuration, 0, sizeof(fhss_synch_configuration_t));
    fhss_structure->bs->synch_infos_sent_counter = 0;
    fhss_structure->bs->broadcast_start_superframe = 0;
    fhss_failed_list_free(fhss_structure);
    fhss_structure->fhss_state = FHSS_UNSYNCHRONIZED;
    return 0;
}

static int fhss_add_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info)
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

static void fhss_update_beacon_info_lifetimes(fhss_structure_t *fhss_structure, uint32_t timestamp)
{
    fhss_beacon_info_t *beacon_info;
    beacon_info = fhss_structure->bs->fhss_beacon_info_store;
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

static int16_t fhss_synch_state_set_callback(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return -1;
    }
    // State is already set
    if (fhss_structure->fhss_state == fhss_state) {
        tr_debug("Synch same state %u", fhss_state);
        return -1;
    }

    if (fhss_state == FHSS_UNSYNCHRONIZED) {
        tr_debug("FHSS down");
        fhss_reset(fhss_structure);
        fhss_reset_synch_monitor(&fhss_structure->bs->synch_monitor);
        fhss_stats_update(fhss_structure, STATS_FHSS_DRIFT_COMP, fhss_structure->bs->synch_monitor.drift_compensation);
        fhss_stats_update(fhss_structure, STATS_FHSS_AVG_SYNCH_FIX, fhss_structure->bs->synch_monitor.avg_synch_fix);
        fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_INTERVAL, fhss_structure->bs->synch_interval / 1000);
    } else {
        // Do not synchronize to current pan
        if (fhss_structure->bs->synch_panid == pan_id) {
            tr_debug("Synch same panid %u", pan_id);
            return -1;
        }
        fhss_generate_scramble_table(fhss_structure);

        uint8_t mac_address[8];
        fhss_structure->callbacks.read_mac_address(fhss_structure->fhss_api, mac_address);
        fhss_structure->bs->uc_channel_index = fhss_get_offset(fhss_structure, mac_address);
        // Get Beacon info from storage
        fhss_beacon_info_t *beacon_info = fhss_get_beacon_info(fhss_structure, pan_id);
        if (beacon_info) {
            memcpy(fhss_structure->synch_parent, beacon_info->source_address, 8);
            platform_enter_critical();
            // Calculate time since the Beacon was received
            uint32_t elapsed_time = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api) - beacon_info->timestamp;
            // Synchronize to given PAN
            fhss_beacon_received(fhss_structure, beacon_info->synch_info, elapsed_time);
            platform_exit_critical();
            // Delete stored Beacon infos
            fhss_flush_beacon_info_storage(fhss_structure);
            fhss_structure->bs->synch_panid = pan_id;
        } else if (fhss_is_synch_root(fhss_structure) == true) {
            // Synch root will start new network
            fhss_start_timer(fhss_structure, fhss_structure->bs->synch_configuration.fhss_superframe_length, fhss_superframe_handler);
        } else {
            tr_error("Synch info not found");
            return -1;
        }
    }
    fhss_structure->fhss_state = fhss_state;
    return fhss_structure->rx_channel;
}

static void fhss_beacon_decode_raw(fhss_synchronization_beacon_payload_s *dest, const uint8_t *buffer)
{
    dest->data_start_delimeter = *buffer++;
    dest->channel_index = *buffer++;
    dest->sender_unicast_channel = *buffer++;
    dest->current_superframe = common_read_16_bit(buffer);
    buffer += BEACON_FIELD_SIZE(current_superframe);
    dest->remaining_slots = common_read_16_bit(buffer);
    buffer += BEACON_FIELD_SIZE(remaining_slots);
    dest->channel_list_counter = common_read_16_bit(buffer);
    buffer += BEACON_FIELD_SIZE(channel_list_counter);
    dest->hop_count = *buffer++;
    dest->number_of_broadcast_channels = *buffer++;
    dest->number_of_tx_slots = *buffer++;
    dest->time_since_last_beacon = common_read_32_bit(buffer);
    buffer += BEACON_FIELD_SIZE(time_since_last_beacon);
    dest->processing_delay += common_read_16_bit(buffer);
    buffer += BEACON_FIELD_SIZE(processing_delay);
    dest->superframe_length = common_read_16_bit(buffer);
    buffer += BEACON_FIELD_SIZE(superframe_length);
    dest->number_of_superframes_per_channel = *buffer;
}

static uint32_t fhss_get_time_to_next_channel_change(uint16_t remaining_slots_to_next_superframe, uint8_t number_of_superframes, uint8_t current_superframe, uint16_t superframe_length)
{
    return remaining_slots_to_next_superframe + ((uint32_t)((number_of_superframes - 1) - current_superframe) * superframe_length);
}

// Decode the given raw byte buffer into a struct into dest struct and calculate
// the new values for elapsed_time, channel_index, current_superframe and remaining_slots
// from current state and given data.
static void fhss_beacon_decode(fhss_synchronization_beacon_payload_s *dest, const uint8_t *buffer, uint32_t elapsed_time, uint16_t number_of_channels)
{
    fhss_beacon_decode_raw(dest, buffer);

    elapsed_time += dest->processing_delay;

    /* To calculate channel index after beacon scan, following calculation is performed
     *
     *                           rem. slots to channel change(X)    Channel length (V)
     *                              |---------------------|     |-----------------------------------------------|
     *    |    RX'd channel index (Y)                     | ... |                      Y+n                      |
     * ...|    sf1    |    sf2    |    sf3    |    sf4    | ... |    sf1    |    sf2    |    sf3    |    sf4    |...
     *                              ^                                     ^
     *                              |beacon received                      |beacon scan done
     *                              |-------------------------------------|
     *                               measured time after beacon RX'd(Z)
     * V = superframe length * number of superframes
     * X = remaining slots to superframe change + length of the remaining full superframes to channel change
     *
     * Y+n = Y + ((Z - X) / V) + 1
     *
     * Or if (Z < X)
     *       Y+n = Y
     */

    uint32_t remaining_slots_to_next_channel = fhss_get_time_to_next_channel_change(dest->remaining_slots, dest->number_of_superframes_per_channel, dest->current_superframe, dest->superframe_length);
    uint16_t temp_channel_index = dest->channel_index;
    if (elapsed_time >= remaining_slots_to_next_channel) {
        uint32_t channel_length = (uint32_t) dest->number_of_superframes_per_channel * dest->superframe_length;
        temp_channel_index = dest->channel_index + ((elapsed_time - remaining_slots_to_next_channel) / channel_length) + 1;
    }
    while (temp_channel_index >= number_of_channels) {
        temp_channel_index -= number_of_channels;
        dest->channel_list_counter++;
    }
    dest->channel_index = temp_channel_index;
    while (dest->channel_list_counter >= (number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES)) {
        dest->channel_list_counter -= (number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES);
    }

    /* To calculate superframe after beacon scan, following calculation is performed
     *
     *           rem. slots(X)         sf. length(V)
     *        |---------------|     |-----------------|
     *...| RX'd superframe (Y)| ... |      Y+n        |      Y+n+1       |....
     *        ^                                     ^
     *        |beacon received                      |beacon scan done
     *        |-------------------------------------|
     *          measured time after beacon RX'd(Z)
     *
     * Y+n = Y + ((Z - X) / V) + 1
     *
     * Or if (Z < X)
     *       Y+n = Y
     */

    if (elapsed_time >= dest->remaining_slots) {
        dest->current_superframe = dest->current_superframe + ((elapsed_time - dest->remaining_slots) / dest->superframe_length) + 1;
    }
    while (dest->current_superframe >= dest->number_of_superframes_per_channel) {
        dest->current_superframe -= dest->number_of_superframes_per_channel;
    }

    /* To get the remaining slots after beacon scan, following calculation is performed
     *
     *         rem. slots(Y)               sf. length(V)    new rem. slots(X)
     *        |----------|               |---------------| |-------------|
     *...| superframe 1  | superframe 2  | superframe 3  | superframe 4  |...
     *        ^                                            ^
     *        |beacon received                             |beacon scan done
     *        |--------------------------------------------|
     *          measured time after beacon RX'd(Z)
     *
     * X = V - ((Z - Y) % V)
     *
     * Or if (Z < Y)
     *        X = Y - Z
     */

    if (elapsed_time < dest->remaining_slots) {
        dest->remaining_slots = dest->remaining_slots - elapsed_time;
    } else {
        dest->remaining_slots = dest->superframe_length - ((elapsed_time - dest->remaining_slots) % dest->superframe_length);
    }
}

static int fhss_synch_info_validate(fhss_synchronization_beacon_payload_s *payload)
{
    if (!payload) {
        return -1;
    }
    if (payload->data_start_delimeter != 0) {
        return -1;
    }
    if (payload->current_superframe >= payload->number_of_superframes_per_channel) {
        return -1;
    }
    if (payload->remaining_slots >= payload->superframe_length) {
        return -1;
    }
    if (payload->hop_count > FHSS_MAX_ALLOWED_HOPS - 1) {
        return -1;
    }
    if (payload->number_of_broadcast_channels == 0) {
        return -1;
    }
    if (payload->number_of_tx_slots == 0) {
        return -1;
    }
    if (payload->number_of_superframes_per_channel == 0) {
        return -1;
    }
    return 0;
}

static void fhss_beacon_received(fhss_structure_t *fhss_structure, const uint8_t *synch_info, const uint32_t elapsed_time)
{

    if (fhss_structure) {

        if (synch_info) {
            fhss_synchronization_beacon_payload_s temp_payload;
            temp_payload.processing_delay = fhss_structure->bs->fhss_configuration.fhss_tuning_parameters.rx_processing_delay;
            fhss_beacon_decode(&temp_payload, synch_info, elapsed_time, fhss_structure->number_of_channels);
            if (!fhss_synch_info_validate(&temp_payload)) {
                fhss_sync_with_beacon(fhss_structure, &temp_payload);
            } else {
                tr_err("Invalid synch info received");
            }
        }
    }
}

static uint32_t fhss_get_sf_timeout_callback(fhss_structure_t *fhss_structure)
{
    uint32_t compensation = 0;
    /* Drift compensation doesn't work with Linux platform */
#ifndef __linux__
    // Drift compensation on first superframe
    if (fhss_structure->bs->current_superframe == 0) {
        /* Idea is to compensate number of drift_compensation (microseconds) on each channel.
         * However, fhss_resolution_divider defines the minimum timer resolution.
         * E.g. if fhss_resolution_divider = 64, compensate (drift_compensation * 64) on each 64th channel.
         */
        if (++fhss_structure->bs->synch_monitor.channel_counter == fhss_structure->platform_functions.fhss_resolution_divider) {
            compensation = fhss_structure->bs->synch_monitor.drift_compensation;
            fhss_structure->bs->synch_monitor.channel_counter = 0;
        }
    }
#else
    (void) fhss_structure;
#endif //__linux__
    return (fhss_structure->bs->synch_configuration.fhss_superframe_length) + (compensation * fhss_structure->platform_functions.fhss_resolution_divider);
}

static void fhss_superframe_callback(fhss_structure_t *fhss_structure)
{
    if ((fhss_structure->bs->send_synch_info_on_next_broadcast_channel == true) && (fhss_is_current_channel_broadcast(fhss_structure) == true)) {
        /* Randomize sending superframe of synchronization frame:
         * on first superframe probability is 1/number of superframes
         * on second superframe probability is 1/(number of superframes-1)
         * on third superframe probability is 1/(number of superframes-2)
         * on last superframe probability is 1/1
         */
        if (randLIB_get_random_in_range(1, fhss_structure->bs->synch_configuration.fhss_number_of_superframes - fhss_structure->bs->current_superframe) == 1) {
            fhss_structure->bs->send_synch_info_on_next_broadcast_channel = false;
            fhss_structure->bs->synch_infos_sent_counter++;
            fhss_structure->callbacks.send_fhss_frame(fhss_structure->fhss_api, FHSS_SYNCH_FRAME);
        }
    }
    fhss_update_txrx_slots(fhss_structure);
    uint16_t queue_size = fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, false) + fhss_structure->callbacks.read_tx_queue_size(fhss_structure->fhss_api, true);
    if ((fhss_structure->bs->tx_allowed == true || fhss_is_current_channel_broadcast(fhss_structure) == true) && queue_size) {
        /* Start timer with random timeout to trigger TX queue poll event.
         * Max random is half of the superframe length. Event timer resolution is 50us.
         * Divide Max random with TX queue size to transmit faster when TX queue is growing
         */
        uint16_t max_random = ((fhss_structure->bs->synch_configuration.fhss_superframe_length / 2) / 50) / queue_size;
        eventOS_callback_timer_start(fhss_structure->fhss_event_timer, randLIB_get_random_in_range(1, max_random));
    }
}

static int fhss_tx_handle_callback(const fhss_api_t *api, bool is_broadcast_addr, uint8_t *destination_address, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length, uint32_t tx_time)
{
    (void) tx_time;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return -2;
    }
    // TODO: needs some more logic to push buffer back to queue
    if (frame_type == FHSS_DATA_FRAME) {
        if (is_broadcast_addr == true) {
            if (fhss_is_current_channel_broadcast(fhss_structure) == false) {
                return -3;
            }
        }
    }
    if (fhss_check_tx_allowed(fhss_structure, is_broadcast_addr, frame_length, frame_type, phy_header_length, phy_tail_length) == false) {
        return -1;
    }
    // If sending Beacon request on parents Unicast channel
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME && fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
        fhss_change_to_parent_channel(fhss_structure);
    } else if (frame_type == FHSS_DATA_FRAME) {
        fhss_change_to_tx_channel(fhss_structure, destination_address);
    }
    return 0;
}

static bool fhss_check_tx_conditions_callback(const fhss_api_t *api, bool is_broadcast_addr, uint8_t handle, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return true;
    }
    // This condition will check that message is not sent on bad channel
    if (fhss_check_bad_channel(fhss_structure, handle) == false) {
        return false;
    }

    // This condition will check that broadcast messages are sent only broadcast channels
    if (fhss_check_channel_type(fhss_structure, is_broadcast_addr, frame_type) == false) {
        return false;
    }

    // This condition will check that FHSS is on TX slot and there is enough time to transmit before channel or slot change
    if (fhss_check_tx_allowed(fhss_structure, is_broadcast_addr, frame_length, frame_type, phy_header_length, phy_tail_length) == false) {
        return false;
    }

    return true;
}

static void fhss_update_channel_callback(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->bs->current_channel_index == 0) {
        fhss_structure->bs->synch_infos_sent_counter = 0;
        if (++fhss_structure->bs->channel_list_counter >= ((uint16_t) fhss_structure->number_of_channels * MAX_SCRAMBLE_TABLE_INDEXES)) {
            fhss_structure->bs->channel_list_counter = 0;
        }
        if (fhss_is_synch_root(fhss_structure) == false) {
            fhss_trig_event(fhss_structure, FHSS_COMPARE_SYNCH_PARENT);
        }
        fhss_trig_event(fhss_structure, FHSS_UPDATE_SYNCH_INFO_STORAGE);
    }
    // If channel is broadcast channel (true), send event
    if (fhss_change_to_next_channel(fhss_structure) == true) {
        // Only if device is border router
        if (fhss_structure->own_hop == 0) {
            fhss_trig_event(fhss_structure, FHSS_BROADCAST_CHANNEL);
        }
    }
}

static uint8_t *fhss_beacon_encode_raw(uint8_t *buffer, const fhss_synchronization_beacon_payload_s *source)
{
    *buffer++ = FHSS_DATA_START_DELIMETER;
    *buffer++ = source->channel_index;
    *buffer++ = source->sender_unicast_channel;
    buffer = common_write_16_bit(source->current_superframe, buffer);
    buffer = common_write_16_bit(source->remaining_slots, buffer);
    buffer = common_write_16_bit(source->channel_list_counter, buffer);
    *buffer++ = source->hop_count;
    *buffer++ = source->number_of_broadcast_channels;
    *buffer++ = source->number_of_tx_slots;
    buffer = common_write_32_bit(source->time_since_last_beacon, buffer);
    buffer = common_write_16_bit(source->processing_delay, buffer);
    buffer = common_write_16_bit(source->superframe_length, buffer);
    *buffer++ = source->number_of_superframes_per_channel;

    return buffer;
}

static void fhss_beacon_build(fhss_structure_t *fhss_structure, uint8_t *dest, uint32_t tx_time)
{
    fhss_synchronization_beacon_payload_s temp_payload;
    platform_enter_critical();
    const fhss_synch_configuration_t *config = &fhss_structure->bs->synch_configuration;
    temp_payload.channel_index = fhss_structure->bs->current_channel_index;
    temp_payload.sender_unicast_channel = 0;
    temp_payload.current_superframe = fhss_structure->bs->current_superframe;
    // This assumes that the time is always in the range of 0..2**16, which
    // should be the case as the superframe length field is also in that range.
    temp_payload.remaining_slots = (uint16_t) fhss_get_remaining_time_to_next_superframe(fhss_structure);
    temp_payload.channel_list_counter = fhss_structure->bs->channel_list_counter;
    temp_payload.hop_count = fhss_structure->own_hop;
    temp_payload.number_of_broadcast_channels = config->fhss_number_of_bc_channels;
    temp_payload.number_of_tx_slots = config->fhss_number_of_tx_slots;
    temp_payload.time_since_last_beacon = 0; // XXX not available yet
    uint32_t time_to_tx = 0;
    uint32_t cur_time = fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api);
    if (cur_time < tx_time) {
        time_to_tx = tx_time - cur_time;
    }
    temp_payload.processing_delay = fhss_structure->bs->fhss_configuration.fhss_tuning_parameters.tx_processing_delay + time_to_tx;
    temp_payload.superframe_length = config->fhss_superframe_length;
    temp_payload.number_of_superframes_per_channel = config->fhss_number_of_superframes;
    platform_exit_critical();
    fhss_beacon_encode_raw(dest, &temp_payload);
}

static int16_t fhss_write_synch_info_callback(const fhss_api_t *api, uint8_t *ptr, uint8_t length, int frame_type, uint32_t tx_time)
{
    (void) length;
    (void) tx_time;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure || !ptr || (frame_type != FHSS_SYNCH_FRAME)) {
        return -1;
    }
    fhss_beacon_build(fhss_structure, ptr, tx_time);
    return FHSS_SYNCH_INFO_LENGTH;
}

static void fhss_data_tx_done_callback(const fhss_api_t *api, bool waiting_ack, bool tx_completed, uint8_t handle)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }
    if (waiting_ack == false) {
        fhss_change_to_rx_channel(fhss_structure);
    }
    // Buffer was successfully transmitted. Remove stored failure handle if exists.
    if (tx_completed == true) {
        fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
        if (fhss_failed_tx) {
            fhss_failed_handle_remove(fhss_structure, handle);
        }
    }
}

static bool fhss_data_tx_fail_callback(const fhss_api_t *api, uint8_t handle, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Only use channel retries when device is synchronized
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return false;
    }
#ifdef FHSS_CHANNEL_DEBUG
    tr_info("TX failed on ch: %u", debug_destination_channel);
#endif /*FHSS_CHANNEL_DEBUG*/
    // Channel retries are disabled -> return
    if (fhss_structure->bs->fhss_configuration.fhss_number_of_channel_retries == 0) {
        return false;
    }
    // Use channel retries only for data frames
    if (FHSS_DATA_FRAME != frame_type) {
        return false;
    }

    fhss_failed_tx_t *fhss_failed_tx = fhss_failed_handle_find(fhss_structure, handle);
    if (fhss_failed_tx) {
        fhss_failed_tx->retries_done++;
        if (fhss_failed_tx->retries_done >= fhss_structure->bs->fhss_configuration.fhss_number_of_channel_retries) {
            // No more retries. Return false to stop retransmitting.
            fhss_failed_handle_remove(fhss_structure, handle);
            return false;
        }
    } else {
        // Create new failure handle and return true to retransmit
        fhss_failed_handle_add(fhss_structure, handle, fhss_structure->rx_channel);
    }
    return true;
}

static void fhss_receive_frame_callback(const fhss_api_t *api, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return;
    }
    if (FHSS_SYNCH_FRAME == frame_type) {
        if ((fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) || fhss_structure->bs->synch_panid != pan_id) {
            fhss_add_beacon_info(fhss_structure, pan_id, source_address, timestamp, synch_info);
        } else {
            if (!fhss_compare_with_synch_parent_address(fhss_structure, source_address)) {
                // Synch parent address needs to be updated in case parent has changed
                fhss_update_synch_parent_address(fhss_structure);
                platform_enter_critical();
                // Calculate time since the Beacon was received
                uint32_t elapsed_time = fhss_structure->callbacks.read_timestamp(api) - timestamp;
                // Synchronize to given PAN
                fhss_beacon_received(fhss_structure, synch_info, elapsed_time);
                platform_exit_critical();
            }
        }
    } else if (FHSS_SYNCH_REQUEST_FRAME == frame_type) {
        // If current channel is broadcast, we don't need to send another synch info on next broadcast channel.
        // Only send number of MAX_SYNCH_INFOS_PER_CHANNEL_LIST synch infos per one channel list cycle
        if ((fhss_structure->fhss_state == FHSS_SYNCHRONIZED) && (fhss_is_current_channel_broadcast(fhss_structure) == false)
                && (fhss_structure->bs->synch_infos_sent_counter < MAX_SYNCH_INFOS_PER_CHANNEL_LIST)) {
            fhss_structure->bs->send_synch_info_on_next_broadcast_channel = true;
        }
    }
}

static uint32_t fhss_get_retry_period_callback(const fhss_api_t *api, uint8_t *destination_address, uint16_t phy_mtu)
{
    uint16_t retry_period = 0;
    uint16_t random_number = randLIB_get_16bit();
    uint16_t rnd_mask;

    /* Generate retry back-off period. FHSS is using the known synchronization parent info to delay retransmissions upstream.
     *
     */
    if (phy_mtu < 128) {
        // Max. random when PHY MTU below 128 is 6.4ms
        rnd_mask = 0x7f;
    } else if (phy_mtu < 256) {
        // Max. random when PHY MTU below 256 is 12.8ms
        rnd_mask = 0xff;
    } else {
        // Max. random when PHY MTU above 255 is 25.6ms
        rnd_mask = 0x1ff;
    }

    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (fhss_structure) {
        uint32_t datarate = fhss_structure->callbacks.read_datarate(fhss_structure->fhss_api);
        uint16_t max_tx_length;

        if (datarate && phy_mtu) {
            if (fhss_compare_with_synch_parent_address(fhss_structure, destination_address) == 0) {
                // E.g. (1000000 / (250000bit/s / 8 bits)) * 255 bytes = 8160us
                max_tx_length = ((1000000 / (datarate / 8)) * phy_mtu);
                /* Retrying upstream: delay the transmission until assumed hidden node has retried downstream:
                 * Static period: max random + max tx length
                 * 50 comes from MAC timer resolution (50us)
                 */
                retry_period = (rnd_mask + (max_tx_length / 50));
            }
        }
    }

    // Add 1 to not to ever return zero value.
    retry_period += ((random_number & rnd_mask) + 1);
    return retry_period;
}

static bool fhss_is_broadcast_channel_callback(const fhss_api_t *api)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return true;
    }
    // FHSS is unsynchronized, broadcasts allowed
    if (fhss_structure->fhss_state == FHSS_UNSYNCHRONIZED) {
        return true;
    }
    return fhss_is_current_channel_broadcast(fhss_structure);
}

static bool fhss_use_broadcast_queue_cb(const fhss_api_t *api, bool is_broadcast_addr, int frame_type)
{
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(api);
    if (!fhss_structure) {
        return false;
    }
    // Synch requests are always stored in unicast queue
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME) {
        return false;
    }
    // Broadcast packets are stored in broadcast queue
    return is_broadcast_addr;
}

static void fhss_superframe_handler(const fhss_api_t *fhss_api, uint16_t delay)
{
    uint32_t timeout = 0;
    fhss_structure_t *fhss_structure = fhss_get_object_with_api(fhss_api);
    if (!fhss_structure) {
        return;
    }

    timeout = fhss_get_sf_timeout_callback(fhss_structure);

    fhss_start_timer(fhss_structure, timeout - (delay * fhss_structure->platform_functions.fhss_resolution_divider), fhss_superframe_handler);

    if (fhss_structure->bs->current_superframe++ >= (fhss_structure->bs->synch_configuration.fhss_number_of_superframes - 1)) {
        fhss_structure->bs->current_superframe = 0;
        if (++fhss_structure->bs->current_channel_index >= fhss_structure->number_of_channels) {
            fhss_structure->bs->current_channel_index = 0;
        }
        fhss_update_channel_callback(fhss_structure);
    }
    fhss_superframe_callback(fhss_structure);

    if (fhss_structure->fhss_timeout) {
        fhss_structure->fhss_timer += fhss_structure->bs->synch_configuration.fhss_superframe_length;
        if (fhss_structure->fhss_timer >= fhss_structure->fhss_timeout) {
            fhss_trig_event(fhss_structure, FHSS_TIMER_EVENT);
            fhss_structure->fhss_timeout = 0;
            fhss_structure->fhss_timer = 0;
        }
    }
}

uint32_t fhss_get_remaining_time_to_next_superframe(const fhss_structure_t *fhss_structure)
{
    const uint32_t slots = fhss_structure->platform_functions.fhss_get_remaining_slots(fhss_superframe_handler, fhss_structure->fhss_api);
    return (slots * fhss_structure->platform_functions.fhss_resolution_divider);
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
    if (fhss_synch_configuration->fhss_number_of_bc_channels == 0 || fhss_synch_configuration->fhss_number_of_tx_slots == 0
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
    fhss_structure->bs->synch_configuration = *fhss_synch_configuration;
    fhss_structure->own_hop = 0;
    return 0;
}

uint8_t fhss_calculate_uc_index(uint8_t channel_index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels)
{
    // When channel index is 0, return last unicast index
    if (channel_index == 0) {
        return (number_of_channels - number_of_broadcast_channels - 1);
    }
    uint16_t bc_channel_density = (number_of_channels / number_of_broadcast_channels);
    return channel_index - (channel_index / bc_channel_density) - 1;
}

int fhss_set_callbacks(fhss_structure_t *fhss_structure)
{
    // Set external API
    fhss_structure->fhss_api->is_broadcast_channel = &fhss_is_broadcast_channel_callback;
    fhss_structure->fhss_api->use_broadcast_queue = &fhss_use_broadcast_queue_cb;
    fhss_structure->fhss_api->tx_handle = &fhss_tx_handle_callback;
    fhss_structure->fhss_api->check_tx_conditions = &fhss_check_tx_conditions_callback;
    fhss_structure->fhss_api->receive_frame = &fhss_receive_frame_callback;
    fhss_structure->fhss_api->data_tx_done = &fhss_data_tx_done_callback;
    fhss_structure->fhss_api->data_tx_fail = &fhss_data_tx_fail_callback;
    fhss_structure->fhss_api->synch_state_set = &fhss_synch_state_set_callback;
    fhss_structure->fhss_api->read_timestamp = &fhss_read_timestamp_cb;
    fhss_structure->fhss_api->get_retry_period = &fhss_get_retry_period_callback;
    fhss_structure->fhss_api->write_synch_info = &fhss_write_synch_info_callback;
    fhss_structure->fhss_api->init_callbacks = &fhss_init_callbacks_cb;

    return 0;
}

static int8_t fhss_beacon_create_tasklet(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->beacon_tasklet_id < 0) {
        fhss_structure->beacon_tasklet_id = eventOS_event_handler_create(fhss_beacon_tasklet_func, FHSS_TASKLET_INIT_EVENT);
    }
    return fhss_structure->beacon_tasklet_id;
}

static int fhss_beacon_periodic_start(fhss_structure_t *fhss_structure, uint32_t time_to_first_beacon)
{
    int ret_val = -1;

    if (fhss_structure) {
        fhss_beacon_periodic_stop(fhss_structure);
        ret_val = fhss_timeout_start(fhss_structure, time_to_first_beacon * 1000);
    }
    return ret_val;
}

static void fhss_beacon_periodic_stop(fhss_structure_t *fhss_structure)
{
    if (fhss_structure) {
        fhss_timeout_stop(fhss_structure);
    }
}

static void fhss_beacon_tasklet_func(arm_event_s *event)
{
    fhss_structure_t *fhss_structure = (fhss_structure_t *)event->data_ptr;
    if (!fhss_structure) {
        return;
    }
    uint8_t parent_address[8];
    fhss_clear_active_event(fhss_structure, event->event_type);
    // skip the init event as there will be a timer event after
    if (event->event_type == FHSS_TIMER_EVENT) {
        // Stop network when lost number of FHSS_SYNCHRONIZATION_LOST synchronization beacons from parent in a row.
        if (fhss_structure->bs->beacons_received_timer >= FHSS_SYNCHRONIZATION_LOST) {
            fhss_structure->callbacks.synch_lost_notification(fhss_structure->fhss_api);
            fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_LOST, 1);
            tr_err("FHSS synchronization lost");
        } else {
            uint16_t bc_density = (fhss_structure->number_of_channels / fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
            uint16_t channel_dwell_time = ((uint32_t)fhss_structure->bs->synch_configuration.fhss_superframe_length * fhss_structure->bs->synch_configuration.fhss_number_of_superframes) / 1000;

            fhss_beacon_periodic_start(fhss_structure, (bc_density * channel_dwell_time) * 2);
            // Send synchronization request
            fhss_structure->callbacks.send_fhss_frame(fhss_structure->fhss_api, FHSS_SYNCH_REQUEST_FRAME);
            fhss_structure->bs->beacons_received_timer++;
#ifdef FEA_TRACE_SUPPORT
            if (!fhss_get_parent_address(fhss_structure, parent_address)) {
                tr_debug("Update synch, attempt: %u, %s", fhss_structure->bs->beacons_received_timer, trace_array(parent_address, 8));
            } else {
                tr_err("No synch parent found");
            }
#endif /*FEA_TRACE_SUPPORT*/
        }
    }
    // Compare if synchronization parent has changed and request beacon if needed
    else if (event->event_type == FHSS_COMPARE_SYNCH_PARENT) {
        if (fhss_compare_with_synch_parent_address(fhss_structure, fhss_structure->synch_parent)) {
            fhss_structure->bs->synch_monitor.avg_synch_fix = 0;
            if (fhss_structure->bs->synch_monitor.avg_synch_fix_counter > 0) {
                fhss_structure->bs->synch_monitor.avg_synch_fix_counter = 0;
            }
            // Send synchronization request
            fhss_structure->callbacks.send_fhss_frame(fhss_structure->fhss_api, FHSS_SYNCH_REQUEST_FRAME);
#ifdef FEA_TRACE_SUPPORT
            if (!fhss_get_parent_address(fhss_structure, parent_address)) {
                tr_debug("Synch parent changed, New: %s, Old: %s\n", trace_array(parent_address, 8), trace_array(fhss_structure->synch_parent, 8));
            } else {
                tr_err("Synch parent changed : No parent found");
            }
#endif /*FEA_TRACE_SUPPORT*/
        }
    } else if (event->event_type == FHSS_BROADCAST_CHANNEL) {
        uint16_t superframe_length = fhss_structure->bs->synch_configuration.fhss_superframe_length;
        uint8_t number_of_superframes = fhss_structure->bs->synch_configuration.fhss_number_of_superframes;
        // Given broadcast time is channel length minus 1 superframe
        fhss_structure->callbacks.broadcast_notify(fhss_structure->fhss_api, (uint32_t)superframe_length * (number_of_superframes - 1));
    }
    // Update Beacon info lifetimes
    else if (event->event_type == FHSS_UPDATE_SYNCH_INFO_STORAGE) {
        fhss_update_beacon_info_lifetimes(fhss_structure, fhss_structure->callbacks.read_timestamp(fhss_structure->fhss_api));
    }
}
