/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "fhss_api.h"
#include "fhss_config.h"
#include "Service_Libs/fhss/fhss.h"
#include "Service_Libs/fhss/fhss_beacon.h"
#include "MAC/IEEE802_15_4/mac_fhss_callbacks.h"
#include "fhss_platform_stub.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "fhss_stub.h"

bool test_fhss_beacon_tasklet_func()
{
    fhss_structure_t fhss_structure;
    fhss_structure.beacon_tasklet_id = -1;
    fhss_structure.callbacks.synch_lost_notification = mac_synch_lost;
    fhss_structure.callbacks.send_fhss_frame = mac_fhss_frame_tx;
    fhss_structure.callbacks.broadcast_notify = mac_broadcast_notification;
    fhss_structure.platform_functions.fhss_get_timestamp = fhss_get_timestamp_stub;
    fhss_structure.number_of_channels = 50;
    fhss_structure.synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_structure.synch_monitor.avg_synch_fix_counter = 1;

    fhss_beacon_create_tasklet(&fhss_structure);


    //Event send
    arm_event_s event = {
            .receiver = 0,
            .sender = 0,
            .event_id = 0,
            .data_ptr = NULL,
            .event_type = 0,
            .priority = 0,
    };
    // Test without fhss_struct
    eventOS_event_send(&event);
    event.data_ptr = &fhss_structure;
    // Test FHSS timer event
    fhss_stub.int_value = 0;
    event.event_type = FHSS_TIMER_EVENT;
    fhss_structure.beacons_received_timer = FHSS_SYNCHRONIZATION_LOST;
    eventOS_event_send(&event);
    fhss_structure.beacons_received_timer = FHSS_SYNCHRONIZATION_LOST - 1;
    eventOS_event_send(&event);
    if (fhss_structure.beacons_received_timer != FHSS_SYNCHRONIZATION_LOST) {
        return false;
    }
    fhss_stub.int_value = -1;
    fhss_structure.beacons_received_timer = FHSS_SYNCHRONIZATION_LOST - 1;
    eventOS_event_send(&event);

    // Test comparing synch parent
    fhss_stub.int_value = 0;
    fhss_stub.int8_value = 1;
    event.event_type = FHSS_COMPARE_SYNCH_PARENT;
    eventOS_event_send(&event);
    fhss_stub.int_value = -1;
    eventOS_event_send(&event);

    // Test handling broadcast channel event
    event.event_type = FHSS_BROADCAST_CHANNEL;
    eventOS_event_send(&event);

    // Test handling synch info storage update
    event.event_type = FHSS_UPDATE_SYNCH_INFO_STORAGE;
    eventOS_event_send(&event);

    return true;
}

bool test_fhss_beacon_build()
{
    // Not much to test in this function
    fhss_beacon_build(NULL, NULL);
    return true;
}

bool test_fhss_beacon_received()
{
    fhss_structure_t fhss_structure;
    uint8_t synch_info[21];
    // Test NULL parameters;
    fhss_beacon_received(NULL, NULL, 0);
    // Test NULL synch info
    fhss_beacon_received(&fhss_structure, NULL, 0);
    // Test with "proper" parameters
    fhss_beacon_received(&fhss_structure, synch_info, 0);
    return true;
}
