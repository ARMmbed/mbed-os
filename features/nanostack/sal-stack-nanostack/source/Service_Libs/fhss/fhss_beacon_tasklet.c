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
#include "common_functions.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "ns_trace.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_beacon.h"
#include "fhss_statistics.h"
#include "fhss_mac_interface.h"
#include "platform/arm_hal_interrupt.h"

#include <string.h> // memset

#define TRACE_GROUP "fhss"

static void fhss_beacon_tasklet_func(arm_event_s* event);

int8_t fhss_beacon_create_tasklet(fhss_structure_t *fhss_structure)
{
    if (fhss_structure->beacon_tasklet_id < 0) {
        fhss_structure->beacon_tasklet_id = eventOS_event_handler_create(fhss_beacon_tasklet_func, FHSS_TASKLET_INIT_EVENT);
    }
    return fhss_structure->beacon_tasklet_id;
}

int fhss_beacon_periodic_start(fhss_structure_t *fhss_structure,
                                uint32_t time_to_first_beacon)
{
    int ret_val = -1;

    if (fhss_structure) {
        fhss_beacon_periodic_stop(fhss_structure);
        ret_val = fhss_timeout_start(fhss_structure, time_to_first_beacon * 1000);
    }
    return ret_val;
}

void fhss_beacon_periodic_stop(fhss_structure_t *fhss_structure)
{
    if (fhss_structure) {
        fhss_timeout_stop(fhss_structure);
    }
}

static void fhss_beacon_tasklet_func(arm_event_s* event)
{
    fhss_structure_t *fhss_structure = (fhss_structure_t *)event->data_ptr;
    if (!fhss_structure) {
        return;
    }
#ifdef FEA_TRACE_SUPPORT
    uint8_t parent_address[8];
#endif
    fhss_clear_active_event(fhss_structure, event->event_type);
    // skip the init event as there will be a timer event after
    if (event->event_type == FHSS_TIMER_EVENT) {
        // Stop network when lost number of FHSS_SYNCHRONIZATION_LOST synchronization beacons from parent in a row.
        if (fhss_structure->beacons_received_timer >= FHSS_SYNCHRONIZATION_LOST) {
            fhss_structure->callbacks.synch_lost_notification(fhss_structure->fhss_api);
            fhss_stats_update(fhss_structure, STATS_FHSS_SYNCH_LOST, 1);
            tr_err("FHSS synchronization lost");
        } else {
            uint16_t bc_density = (fhss_structure->number_of_channels / fhss_structure->synch_configuration.fhss_number_of_bc_channels);
            uint16_t channel_dwell_time = ((uint32_t)fhss_structure->synch_configuration.fhss_superframe_length * fhss_structure->synch_configuration.fhss_number_of_superframes) / 1000;

            fhss_beacon_periodic_start(fhss_structure, (bc_density * channel_dwell_time) * 2);
            // Send synchronization request
            fhss_structure->callbacks.send_fhss_frame(fhss_structure->fhss_api, FHSS_SYNCH_REQUEST_FRAME);
            fhss_structure->beacons_received_timer++;
#ifdef FEA_TRACE_SUPPORT
            if (!fhss_get_parent_address(fhss_structure, parent_address)) {
                tr_debug("Update synch, attempt: %u, %s", fhss_structure->beacons_received_timer, trace_array(parent_address, 8));
            } else {
                tr_err("No synch parent found");
            }
#endif /*FEA_TRACE_SUPPORT*/
        }
    }
    // Compare if synchronization parent has changed and request beacon if needed
    else if(event->event_type == FHSS_COMPARE_SYNCH_PARENT)
    {
        if (fhss_compare_with_synch_parent_address(fhss_structure, fhss_structure->synch_parent)) {
            fhss_structure->synch_monitor.avg_synch_fix = 0;
            if(fhss_structure->synch_monitor.avg_synch_fix_counter > 0) {
                fhss_structure->synch_monitor.avg_synch_fix_counter = 0;
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
    }
    else if(event->event_type == FHSS_BROADCAST_CHANNEL)
    {
        uint16_t superframe_length = fhss_structure->synch_configuration.fhss_superframe_length;
        uint8_t number_of_superframes = fhss_structure->synch_configuration.fhss_number_of_superframes;
        // Given broadcast time is channel length minus 1 superframe
        fhss_structure->callbacks.broadcast_notify(fhss_structure->fhss_api, (uint32_t)superframe_length * (number_of_superframes - 1));
    }
    // Update Beacon info lifetimes
    else if(event->event_type == FHSS_UPDATE_SYNCH_INFO_STORAGE)
    {
        fhss_update_beacon_info_lifetimes(fhss_structure, fhss_read_timestamp_cb(fhss_structure->fhss_api));
    }
}

void fhss_beacon_build(fhss_structure_t *fhss_structure, uint8_t* dest)
{
    fhss_synchronization_beacon_payload_s temp_payload;
    platform_enter_critical();
    fhss_beacon_update_payload(fhss_structure, &temp_payload);
    platform_exit_critical();
    fhss_beacon_encode_raw(dest, &temp_payload);
}

// this assumes that the buffer's data pointer is seeked to the beacon payload
void fhss_beacon_received(fhss_structure_t *fhss_structure, const uint8_t *synch_info, const uint32_t elapsed_time) {

    if (fhss_structure) {

        if (synch_info) {
            fhss_synchronization_beacon_payload_s temp_payload;
            temp_payload.processing_delay = fhss_structure->fhss_configuration.fhss_tuning_parameters.rx_processing_delay;
            fhss_beacon_decode(&temp_payload, synch_info, elapsed_time, fhss_structure->number_of_channels);

            // use the received information
            fhss_sync_with_beacon(fhss_structure, &temp_payload);
        }
    }
}


