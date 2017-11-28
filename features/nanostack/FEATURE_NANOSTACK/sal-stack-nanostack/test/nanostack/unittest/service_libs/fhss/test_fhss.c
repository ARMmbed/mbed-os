/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "test_fhss.h"
#include "fhss_config_stub.h"
#include "fhss_channel_stub.h"
#include "fhss_platform_stub.h"
#include "channel_list_stub.h"
#include "nsdynmemLIB_stub.h"
#include "ns_timer_stub.h"
#include "fhss_callbacks_stub.h"
#include "fhss_beacon_tasklet_stub.h"

extern fhss_structure_t *fhss_struct;

bool test_fhss_enable()
{
    fhss_api_t *fhss_api = ns_fhss_create(0, 0);
    fhss_timer_t fhss_timer;
    fhss_configuration_t fhss_configuration;
    memset(&fhss_timer, 0, sizeof(fhss_timer_t));
    memset(&fhss_configuration, 0, sizeof(fhss_configuration_t));
    fhss_beacon_tasklet_stub.int8_value = 0;
    fhss_timer.fhss_timer_stop = &fhss_timer_stop_stub;

    // Test NULL configurations
    if (fhss_enable(NULL, &fhss_configuration, &fhss_timer, NULL) != -1) {
        return false;
    }
    if (fhss_enable(fhss_api, NULL, &fhss_timer, NULL) != -1) {
        return false;
    }
    if (fhss_enable(fhss_api, &fhss_configuration, NULL, NULL) != -1) {
        return false;
    }
    // Test without channels
    channel_list_stub.uint8_value = 0;
    if (fhss_enable(fhss_api, &fhss_configuration, &fhss_timer, NULL) != -2) {
        return false;
    }
    // Test without memory
    nsdynmemlib_stub.returnCounter = 0;
    channel_list_stub.uint8_value = 50;
    if (fhss_enable(fhss_api, &fhss_configuration, &fhss_timer, NULL) != -3) {
        return false;
    }
    // Test with proper configuration
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_enable(fhss_api, &fhss_configuration, &fhss_timer, NULL) != 0) {
        return false;
    }
    // Test that API is stored
    if (fhss_struct != fhss_get_object_with_api(fhss_api)) {
        return false;
    }
    // Test NULL API
    if (fhss_struct == fhss_get_object_with_api(NULL)) {
        return false;
    }
    // Test fake API
    fhss_api_t fhss_fake_api;
    if (fhss_struct == fhss_get_object_with_api(&fhss_fake_api)) {
        return false;
    }
    fhss_disable(fhss_struct);
    // Test tasklet creation failed
    fhss_beacon_tasklet_stub.int8_value = -1;
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_enable(fhss_api, &fhss_configuration, &fhss_timer, NULL) != -5) {
        return false;
    }
    fhss_disable(fhss_struct);
    return true;
}

bool test_fhss_disable()
{
    if (fhss_disable(NULL) == 0) {
        return false;
    }
    return true;
}

bool test_fhss_check_channel_type()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));

    // Test NULL API
    if (fhss_check_channel_type(NULL, true, FHSS_DATA_FRAME) == true) {
        return false;
    }
    // Test unsynchronized state. (Should allow transmission)
    fhss_struct->fhss_state = FHSS_UNSYNCHRONIZED;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == false) {
        return false;
    }
    // Change to synchronized mode
    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;

    // Test data frame, broadcast destination, unicast channel. (Should deny transmission)
    fhss_channel_stub.channel_bool_value = false;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == true) {
        return false;
    }

    // Test data frame, unicast destination, broadcast channel, waiting synchronization Beacon. (Should deny transmission)
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->beacons_received_timer = 1;
    if (fhss_check_channel_type(fhss_struct, false, FHSS_DATA_FRAME) == true) {
        return false;
    }

    // Test synch request frame, currently on broadcast channel. (Should deny transmission)
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_struct->number_of_channels = 50;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_REQUEST_FRAME) == true) {
        return false;
    }

    // Test synch request frame, currently on unicast channel, superframe is 0. (Should deny transmission)
    fhss_channel_stub.channel_bool_value = false;
    fhss_struct->current_superframe = 0;
    fhss_struct->beacons_received_timer = 1;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_REQUEST_FRAME) == true) {
        return false;
    }

    // Test data frame, broadcast destination, broadcast channel. (Should allow transmission)
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->broadcast_start_superframe = 2;
    fhss_struct->current_superframe = 3;
    fhss_channel_stub.channel_bool_value = true;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == false) {
        return false;
    }

    // Test data frame, unicast destination, unicast channel. (Should allow transmission)
    fhss_channel_stub.channel_bool_value = false;
    if (fhss_check_channel_type(fhss_struct, false, FHSS_DATA_FRAME) == false) {
        return false;
    }

    // Test data frame, unicast destination, broadcast channel, not waiting synchronization Beacon. (Should allow transmission)
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->beacons_received_timer = 0;
    if (fhss_check_channel_type(fhss_struct, false, FHSS_DATA_FRAME) == false) {
        return false;
    }

    // Test immediate synch frame, sent on any channel. (Should allow transmission)
    fhss_channel_stub.channel_bool_value = false;
    fhss_struct->current_superframe = 1;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_FRAME) == false) {
        return false;
    }

    // Test synch request frame, currently on broadcast channel, number of BC channel equals number of channels, current superframe is 1. (Should allow transmission)
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->synch_configuration.fhss_number_of_bc_channels = 50;
    fhss_struct->number_of_channels = 50;
    fhss_struct->current_superframe = 1;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_REQUEST_FRAME) == false) {
        return false;
    }

    // Test sending broadcast when number of superframes is low.
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->broadcast_start_superframe = 0;
    fhss_struct->current_superframe = 0;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 4;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == false) {
        return false;
    }

    // Test sending broadcast when not on broadcast sending superframe.
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->broadcast_start_superframe = 2;
    fhss_struct->current_superframe = 0;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == true) {
        return false;
    }

    // Test sending broadcast when on broadcast sending superframe.
    fhss_channel_stub.channel_bool_value = true;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->broadcast_start_superframe = 2;
    fhss_struct->current_superframe = 2;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_DATA_FRAME) == false) {
        return false;
    }

    // Test synch request frame, above the middle of superframes, should return false
    fhss_channel_stub.channel_bool_value = false;
    fhss_struct->current_superframe = 5;
    fhss_struct->beacons_received_timer = 0;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_REQUEST_FRAME) == true) {
        return false;
    }

    // Test synch request frame, when number of TX slots 1 and on first superframe, should return false
    fhss_channel_stub.channel_bool_value = false;
    fhss_struct->current_superframe = 0;
    fhss_struct->beacons_received_timer = 0;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->synch_configuration.fhss_number_of_tx_slots = 1;
    if (fhss_check_channel_type(fhss_struct, true, FHSS_SYNCH_REQUEST_FRAME) == true) {
        return false;
    }

    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_check_tx_allowed()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    // Initialize FHSS configuration
    fhss_struct->datarate = 250000;
    fhss_struct->fhss_configuration.fhss_tuning_parameters.tx_processing_delay = 1000;
    fhss_struct->fhss_configuration.fhss_tuning_parameters.ack_processing_delay = 1000;
    /*...|    TX    |    TX    |    RX    |    RX    |    TX    |    TX    |    RX    |    RX    |...
     *...|   SF n   |   SF n+1 |   SF n+2 |   SF n+3 |   SF n+4 |   SF n+5 |   SF n+6 |   SF n+7 |...
     */
    fhss_struct->current_superframe = 1; // SF n+1
    fhss_struct->synch_configuration.fhss_number_of_tx_slots = 2;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->synch_configuration.fhss_superframe_length = 50000;
    fhss_struct->own_hop = 2; // For even hop count TX superframes are 0, 1 and 4, 5
    fhss_struct->platform_functions.fhss_get_remaining_slots = &fhss_get_remaining_slots_stub;
    fhss_struct->platform_functions.fhss_resolution_divider = 1;

    // Remaining time(us) to next superframe change. This is what get_remaining_slot -call returns
    fhss_platform_stub.uint32_value = 40000;

    // When transmitting 100 bytes + Ack (+ processing delays), needed transmission time is around 7.4ms

    // FHSS is on RX slot
    fhss_struct->tx_allowed = false;
    // Test NULL API
    if (fhss_check_tx_allowed(NULL, true, 100, FHSS_DATA_FRAME, 0, 0) == true) {
        return false;
    }

    // Test unsynchronized state. (Should allow transmission)
    fhss_struct->fhss_state = FHSS_UNSYNCHRONIZED;
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == false) {
        return false;
    }

    // Change to synchronized mode
    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;

    // Test synchronization frame. (Should allow transmission)
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_SYNCH_FRAME, 0, 0) == false) {
        return false;
    }

    // Test broadcast frame. (Should allow transmission)
    if (fhss_check_tx_allowed(fhss_struct, true, 100, FHSS_DATA_FRAME, 0, 0) == false) {
        return false;
    }

    // Test TX allowed flag. (Should deny transmission)
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == true) {
        return false;
    }

    // FHSS is on TX slot
    fhss_struct->tx_allowed = true;
    // Test TX allowed flag. (Should allow transmission)
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == false) {
        return false;
    }

    // Return too small remaining time(us). (Should deny transmission)
    fhss_platform_stub.uint32_value = 7000;
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == true) {
        return false;
    }

    // Use higher datarate. (Should allow transmission)
    fhss_struct->datarate = 300000;
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == false) {
        return false;
    }

    // Test adding header and tail bytes (100 bytes for data and ack frame means 6.4ms more TX time needed. 13.8ms total). (Should deny transmission)
    fhss_struct->datarate = 250000;
    fhss_platform_stub.uint32_value = 13700;
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 50, 50) == true) {
        return false;
    }

    // Test odd hop count. (Should deny transmission)
    fhss_struct->own_hop = 1;
    if (fhss_check_tx_allowed(fhss_struct, false, 100, FHSS_DATA_FRAME, 0, 0) == true) {
        return false;
    }

    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_check_bad_channel()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    memset(&fhss_struct->fhss_failed_tx_list, 0, sizeof(fhss_failed_tx_list_t));
    // Test NULL API
    if (fhss_check_bad_channel(NULL, 0) == true) {
        return false;
    }
    // Test with proper API
    if (fhss_check_bad_channel(fhss_struct, 0) == false) {
        return false;
    }
    // Test when bad channel is found
    uint8_t handle = 10;
    fhss_struct->rx_channel = 40;
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    nsdynmemlib_stub.returnCounter = 1;

    if (fhss_failed_handle_add(fhss_struct, handle) != 0) {
        return false;
    }
    if (fhss_check_bad_channel(fhss_struct, handle) == true) {
        return false;
    }
    // After channel change should return false
    fhss_struct->rx_channel = 41;
    if (fhss_check_bad_channel(fhss_struct, handle) == false) {
        return false;
    }
    if (fhss_failed_handle_remove(fhss_struct, handle) != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
}

bool test_fhss_set_synch_configuration()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));

    fhss_struct->platform_functions.fhss_timer_start = &fhss_timer_start;
    fhss_struct->number_of_channels = 50;
    fhss_struct->datarate = 250000;

    fhss_synch_configuration_t fhss_synch_configuration;
    memset(&fhss_synch_configuration, 0, sizeof(fhss_synch_configuration_t));

    // NULL API should fail configuration check
    if (fhss_set_synch_configuration(NULL, &fhss_synch_configuration) == 0) {
        return false;
    }
    // NULL configuration should fail configuration check
    if (fhss_set_synch_configuration(fhss_struct, NULL) == 0) {
        return false;
    }
    // Test giving invalid setting
    fhss_synch_configuration.fhss_number_of_bc_channels = 0;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -3) {
        return false;
    }
    fhss_synch_configuration.fhss_number_of_bc_channels = 10;
    fhss_synch_configuration.fhss_number_of_superframes = 0;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -3) {
        return false;
    }
    fhss_synch_configuration.fhss_number_of_superframes = 8;
    fhss_synch_configuration.fhss_number_of_tx_slots = 0;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -3) {
        return false;
    }
    fhss_synch_configuration.fhss_number_of_tx_slots = 2;
    fhss_synch_configuration.fhss_superframe_length = 0;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -3) {
        return false;
    }
    fhss_synch_configuration.fhss_superframe_length = 50000;
    // Test number of channels not divisible with number of broadcast channels
    fhss_synch_configuration.fhss_number_of_bc_channels = 9;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -4) {
        return false;
    }
    fhss_synch_configuration.fhss_number_of_bc_channels = 10;
    // Test number superframes not divisible with number of TX slots
    fhss_synch_configuration.fhss_number_of_tx_slots = 3;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -4) {
        return false;
    }
    // Test number superframes = number of TX slots
    fhss_synch_configuration.fhss_number_of_tx_slots = 8;
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration) != -4) {
        return false;
    }
    fhss_synch_configuration.fhss_number_of_tx_slots = 2;
    // Test with proper configuration
    if (fhss_set_synch_configuration(fhss_struct, &fhss_synch_configuration)) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = 0;
    return true;
}

bool test_fhss_add_beacon_info()
{
    uint8_t compare_array[20];
    uint8_t mac_address[8];
    uint8_t synch_info[20];
    uint16_t panid = 0x1234;

    fhss_struct = malloc(sizeof(fhss_structure_t));

    memset(mac_address, 0, 8);

    fhss_struct->fhss_beacon_info_store = 0;
    // Test without FHSS structure
    if (fhss_add_beacon_info(NULL, panid, mac_address, 0, synch_info) != -1) {
        return false;
    }
    // Test without source address
    if (fhss_add_beacon_info(fhss_struct, panid, NULL, 0, synch_info) != -1) {
        return false;
    }
    // Test without synchronization info
    if (fhss_add_beacon_info(fhss_struct, panid, mac_address, 0, NULL) != -1) {
        return false;
    }
    // Test without memory
    if (fhss_add_beacon_info(fhss_struct, panid, mac_address, 0, synch_info) != -2) {
        return false;
    }
    // Test adding first Beacon info
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_add_beacon_info(fhss_struct, panid, mac_address, 0, synch_info) != 0) {
        return false;
    }
    memset(compare_array, 0, 8);
    // Verify storage by comparing pan id and source address
    if ((fhss_struct->fhss_beacon_info_store->pan_id != panid) || memcmp(fhss_struct->fhss_beacon_info_store->source_address, compare_array, 8)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    // Test adding another Beacon info (pan id is not same as first Beacon info)
    if (fhss_add_beacon_info(fhss_struct, panid + 1, mac_address, 0, synch_info) != 0) {
        return false;
    }
    memset(compare_array, 0, 8);
    // Verify storage by comparing pan id
    if ((fhss_struct->fhss_beacon_info_store->next->pan_id != panid + 1) || memcmp(fhss_struct->fhss_beacon_info_store->next->source_address, compare_array, 8)) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    // Test adding third Beacon info (pan id is not same as other Beacon infos)
    if (fhss_add_beacon_info(fhss_struct, panid + 2, mac_address, 0, synch_info) != 0) {
        return false;
    }
    memset(compare_array, 0, 8);
    // Verify storage by comparing pan id
    if ((fhss_struct->fhss_beacon_info_store->next->next->pan_id != panid + 2) || memcmp(fhss_struct->fhss_beacon_info_store->next->next->source_address, compare_array, 8)) {
        return false;
    }
    // Test replacing first Beacon info (source address has changed)
    memset(mac_address, 1, 8);
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_add_beacon_info(fhss_struct, panid, mac_address, 0, synch_info) != 0) {
        return false;
    }
    memset(compare_array, 1, 8);
    // Verify storage by comparing pan id and source address
    if ((fhss_struct->fhss_beacon_info_store->pan_id != panid) || memcmp(fhss_struct->fhss_beacon_info_store->source_address, compare_array, 8)) {
        return false;
    }
    // Test replacing second Beacon info (source address has changed)
    memset(mac_address, 1, 8);
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_add_beacon_info(fhss_struct, panid + 1, mac_address, 0, synch_info) != 0) {
        return false;
    }
    memset(compare_array, 1, 8);
    // Verify storage by comparing pan id and source address
    if ((fhss_struct->fhss_beacon_info_store->next->pan_id != panid + 1) || memcmp(fhss_struct->fhss_beacon_info_store->next->source_address, compare_array, 8)) {
        return false;
    }
    // Test flushing Beacon storage without FHSS structure
    if (fhss_flush_beacon_info_storage(NULL) != -1) {
        return false;
    }
    // Test flushing Beacon storage
    if (fhss_flush_beacon_info_storage(fhss_struct) != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = 0;
    return true;
}

bool test_fhss_set_datarate()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    // Test NULL parameter
    if (!fhss_set_datarate(NULL, 250000)) {
        return false;
    }
    // Test that zero is replaced with default datarate
    if (fhss_set_datarate(fhss_struct, 0)) {
        return false;
    }
    if (fhss_struct->datarate != 250000) {
        return false;
    }
    // Test that proper configuration is accepted
    if (fhss_set_datarate(fhss_struct, 200000)) {
        return false;
    }
    if (fhss_struct->datarate != 200000) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = 0;
    return true;
}

bool test_fhss_sync_with_beacon()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    fhss_synchronization_beacon_payload_s payload;

    fhss_struct->number_of_channels = 50;
    payload.number_of_broadcast_channels = 10;
    payload.superframe_length = 50000;
    payload.number_of_superframes_per_channel = 8;
    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;

    fhss_struct->callbacks.read_mac_address = &mac_read_64bit_mac_address;
    fhss_struct->callbacks.read_coord_mac_address = &mac_get_coordinator_mac_address;
    fhss_struct->platform_functions.fhss_timer_stop = &fhss_timer_stop_stub;
    fhss_struct->platform_functions.fhss_timer_start = &fhss_timer_start;
    fhss_struct->platform_functions.fhss_get_remaining_slots = &fhss_get_remaining_slots_stub;
    fhss_struct->fhss_configuration.fhss_max_synch_interval = 240;
    fhss_struct->synch_interval = 150000;

    // Test that synchronization is skipped if Beacon is already received on this channel
    fhss_struct->beacon_received_on_this_bc_channel = false;
    fhss_channel_stub.channel_bool_value = false;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    fhss_struct->beacon_received_on_this_bc_channel = true;
    fhss_channel_stub.channel_bool_value = true;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    // Test fixing synchronization
    fhss_struct->beacon_received_on_this_bc_channel = false;
    fhss_channel_stub.channel_bool_value = true;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    // Test synchronizing unsynchronized device
    fhss_struct->fhss_state = FHSS_UNSYNCHRONIZED;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }

    fhss_struct->fhss_state = FHSS_UNSYNCHRONIZED;
    fhss_struct->synch_interval = 240000;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }

    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;
    fhss_struct->beacon_received_on_this_bc_channel = false;
    // Test superframe changed loops
    fhss_struct->current_superframe = 0;
    payload.current_superframe = 7;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    // Test synch drift compensation
    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;
    fhss_struct->own_hop = 1;
    fhss_struct->beacon_received_on_this_bc_channel = false;
    fhss_struct->synch_monitor.avg_synch_fix_counter = 4;
    fhss_struct->synch_monitor.avg_synch_fix = 100;
    fhss_struct->platform_functions.fhss_resolution_divider = 1;
    payload.remaining_slots = fhss_platform_stub.uint32_value;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    // Should use maximum interval
    if (fhss_struct->synch_interval != 240000) {
        return false;
    }

    fhss_struct->fhss_state = FHSS_SYNCHRONIZED;
    fhss_struct->own_hop = 1;
    fhss_struct->beacon_received_on_this_bc_channel = false;
    fhss_struct->synch_monitor.avg_synch_fix_counter = 4;
    fhss_struct->synch_monitor.avg_synch_fix = 51000;
    fhss_struct->synch_interval = 120000;
    payload.remaining_slots = fhss_platform_stub.uint32_value + 1000000000;
    if (fhss_sync_with_beacon(fhss_struct, &payload)) {
        return false;
    }
    if (fhss_struct->synch_interval != 4000) {
        return false;
    }

    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_get_parent_address()
{
    uint8_t mac_address[8];
    uint8_t synch_parent[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t rpl_parent[8] = {7, 6, 5, 4, 3, 2, 1, 0};
    // Test without FHSS
    if (fhss_get_parent_address(NULL, mac_address) != -1) {
        return false;
    }
    fhss_struct = malloc(sizeof(fhss_structure_t));
    memcpy(fhss_struct->synch_parent, synch_parent, 8);
    memcpy(fhss_callbacks_stub.address, rpl_parent, 8);

    fhss_struct->callbacks.read_coord_mac_address = &mac_get_coordinator_mac_address;
    fhss_callbacks_stub.int8_value = -1;
    // Test when function pointer not set
    if (fhss_get_parent_address(fhss_struct, mac_address) != 0) {
        return false;
    }
    // MAC address should be now synch address
    if (memcmp(mac_address, synch_parent, 8)) {
        return false;
    }
    // Test when function pointer is set and RPL address can be found
    fhss_platform_stub.int_value = 0;
    fhss_callbacks_stub.int8_value = 0;
    if (fhss_get_parent_address(fhss_struct, mac_address) != 0) {
        return false;
    }
    // MAC address should be now rpl address
    if (memcmp(mac_address, rpl_parent, 8)) {
        return false;
    }
    // Test when RPL address can not be found
    fhss_callbacks_stub.int8_value = -1;
    fhss_platform_stub.int_value = -1;
    if (fhss_get_parent_address(fhss_struct, mac_address) != 0) {
        return false;
    }
    // MAC address should be now synch address
    if (memcmp(mac_address, synch_parent, 8)) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_compare_with_synch_parent_address()
{
    uint8_t mac_address[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t synch_parent[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    // Test without FHSS
    if (!fhss_compare_with_synch_parent_address(NULL, mac_address)) {
        return false;
    }
    fhss_struct = malloc(sizeof(fhss_structure_t));
    //Test when mac address is synch address

    fhss_struct->callbacks.read_coord_mac_address = &mac_get_coordinator_mac_address;

    memcpy(fhss_struct->synch_parent, synch_parent, 8);
    if (fhss_compare_with_synch_parent_address(fhss_struct, mac_address)) {
        return false;
    }

    //Test when mac address is not synch address
    mac_address[0] = 1;
    fhss_callbacks_stub.int8_value = 0;

    if (!fhss_compare_with_synch_parent_address(fhss_struct, mac_address)) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_update_synch_parent_address()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    uint8_t rpl_parent[8] = {7, 6, 5, 4, 3, 2, 1, 0};
    // Test without FHSS
    if (!fhss_update_synch_parent_address(NULL)) {
        return false;
    }
    //Test when synch address is RPL parent address
    memset(fhss_struct->synch_parent, 0, 8);
    memcpy(fhss_callbacks_stub.address, rpl_parent, 8);
    fhss_callbacks_stub.int8_value = 0;

    fhss_struct->callbacks.read_coord_mac_address = &mac_get_coordinator_mac_address;
    if (fhss_update_synch_parent_address(fhss_struct)) {
        return false;
    }
    // Synch address should be now rpl address
    if (memcmp(fhss_struct->synch_parent, rpl_parent, 8)) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_superframe_handler()
{
    fhss_api_t fhss_api;
    fhss_struct = malloc(sizeof(fhss_structure_t));
    fhss_struct->fhss_api = &fhss_api;
    fhss_struct->platform_functions.fhss_timer_start = &fhss_timer_start;
    fhss_struct->callbacks.read_tx_queue_size = &mac_read_tx_queue_sizes;
    fhss_struct->callbacks.send_fhss_frame = &mac_fhss_frame_tx;
    fhss_struct->synch_configuration.fhss_number_of_superframes = 8;
    fhss_struct->synch_configuration.fhss_superframe_length = 50000;
    fhss_struct->synch_configuration.fhss_number_of_tx_slots = 4;
    fhss_struct->number_of_channels = 50;
    fhss_struct->own_hop = 2;
    fhss_channel_stub.channel_bool_value = true;

    // Test drift compensation;
    fhss_struct->current_superframe = 0;
    fhss_struct->platform_functions.fhss_resolution_divider = 64;
    fhss_struct->synch_monitor.channel_counter = 63;
    // Should not work without FHSS api
    fhss_superframe_handler(NULL, 0);
    if (fhss_struct->synch_monitor.channel_counter != 63) {
        return false;
    }
    fhss_superframe_handler(&fhss_api, 0);

    fhss_struct->current_superframe = 7;
    fhss_struct->current_channel_index = 49;
    fhss_struct->synch_infos_sent_counter = 1;
    fhss_struct->channel_list_counter = 999;
    fhss_superframe_handler(&fhss_api, 0);
    // Current superframe, channel index, Beacons sent counter and channel list counter should be cleared
    if (fhss_struct->current_superframe != 0 || fhss_struct->current_channel_index != 0 ||
            fhss_struct->synch_infos_sent_counter != 0 || fhss_struct->channel_list_counter != 0) {
        return false;
    }
    fhss_struct->fhss_timeout = 50000;
    fhss_superframe_handler(&fhss_api, 0);
    // Timeout should be handled and cleared
    if (fhss_struct->fhss_timeout != 0 || fhss_struct->fhss_timer != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = 0;
    return true;
}

bool test_fhss_event_timer_cb()
{
    fhss_api_t fhss_api;
    fhss_configuration_t fhss_configuration;
    fhss_timer_t fhss_timer;
    nsdynmemlib_stub.returnCounter = 1;
    ns_timer_stub.cb_counter = 1;
    fhss_timer.fhss_timer_stop = &fhss_timer_stop_stub;

    // To register callback, execute fhss enable
    fhss_enable(&fhss_api, &fhss_configuration, &fhss_timer, NULL);

    fhss_struct->callbacks.tx_poll = mac_poll_tx_queue;
    fhss_struct->fhss_event_timer = 0;

    if( ns_timer_stub.cb ){
        // Test with invalid timer id
        ns_timer_stub.cb(-1, 0);
    }
    if( ns_timer_stub.cb ){
        // Test with wrong timer id
        ns_timer_stub.cb(1, 0);
    }
    if( ns_timer_stub.cb ){
        // Test with proper timer id
        ns_timer_stub.cb(0, 0);
    }

    fhss_disable(fhss_struct);
    return true;
}

bool test_fhss_down()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    fhss_struct->platform_functions.fhss_timer_stop = &fhss_timer_stop_stub;
    fhss_struct->fhss_beacon_info_store = NULL;
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    // Test without FHSS
    if (fhss_down(NULL) == 0) {
        return false;
    }
    // Test with proper FHSS struct
    if (fhss_down(fhss_struct) != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_timeout_start()
{
    uint32_t time = 1000;
    fhss_struct = malloc(sizeof(fhss_structure_t));
    // Test without FHSS
    if (fhss_timeout_start(NULL, 1000) == 0) {
        return false;
    }
    // Test with proper FHSS struct
    if (fhss_timeout_start(fhss_struct, time) != 0) {
        return false;
    }
    // Check that values were set/cleared
    if (fhss_struct->fhss_timeout != time || fhss_struct->fhss_timer != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_timeout_stop()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    // Test without FHSS
    if (fhss_timeout_stop(NULL) == 0) {
        return false;
    }
    // Test with proper FHSS struct
    if (fhss_timeout_stop(fhss_struct) != 0) {
        return false;
    }
    // Check that values were set/cleared
    if (fhss_struct->fhss_timeout != 0 || fhss_struct->fhss_timer != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_clear_active_event()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    fhss_struct->active_fhss_events |= (1 << FHSS_UPDATE_SYNCH_INFO_STORAGE);
    fhss_clear_active_event(fhss_struct, FHSS_UPDATE_SYNCH_INFO_STORAGE);
    if (fhss_struct->active_fhss_events & (1 << FHSS_UPDATE_SYNCH_INFO_STORAGE)) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_update_beacon_info_lifetimes()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    fhss_struct->fhss_beacon_info_store = malloc(sizeof(fhss_beacon_info_t));
    fhss_struct->fhss_beacon_info_store->timestamp = 100000;
    fhss_struct->fhss_beacon_info_store->next = NULL;
    // Should not remove Beacon info because lifetime has not passed yet
    fhss_update_beacon_info_lifetimes(fhss_struct, fhss_struct->fhss_beacon_info_store->timestamp + 100000);
    if (!fhss_struct->fhss_beacon_info_store) {
        return false;
    }
    // Should remove the Beacon info
    fhss_update_beacon_info_lifetimes(fhss_struct, fhss_struct->fhss_beacon_info_store->timestamp + 600000000);
    if (fhss_struct->fhss_beacon_info_store) {
        return false;
    }

    // Store two Beacon infos
    fhss_struct->fhss_beacon_info_store = malloc(sizeof(fhss_beacon_info_t));
    fhss_struct->fhss_beacon_info_store->timestamp = 100000;
    fhss_struct->fhss_beacon_info_store->pan_id = 1;
    fhss_struct->fhss_beacon_info_store->next = malloc(sizeof(fhss_beacon_info_t));
    fhss_struct->fhss_beacon_info_store->next->timestamp = 100100;
    fhss_struct->fhss_beacon_info_store->next->pan_id = 2;
    fhss_struct->fhss_beacon_info_store->next->next = NULL;
    // Only first Beacon info is removed after this call
    fhss_update_beacon_info_lifetimes(fhss_struct, fhss_struct->fhss_beacon_info_store->timestamp + 600000000);
    if (!fhss_struct->fhss_beacon_info_store || fhss_struct->fhss_beacon_info_store->next) {
        return false;
    }
    // Second Beacon info is removed after this call
    fhss_update_beacon_info_lifetimes(fhss_struct, fhss_struct->fhss_beacon_info_store->timestamp + 600001000);
    if (fhss_struct->fhss_beacon_info_store) {
        return false;
    }

    // Store two Beacon infos removed in reverse order
    fhss_struct->fhss_beacon_info_store = malloc(sizeof(fhss_beacon_info_t));
    fhss_struct->fhss_beacon_info_store->timestamp = 200000;
    fhss_struct->fhss_beacon_info_store->pan_id = 1;
    fhss_struct->fhss_beacon_info_store->next = malloc(sizeof(fhss_beacon_info_t));
    fhss_struct->fhss_beacon_info_store->next->timestamp = 100000;
    fhss_struct->fhss_beacon_info_store->next->pan_id = 2;
    fhss_struct->fhss_beacon_info_store->next->next = NULL;
    // Only first Beacon info is removed after this call
    fhss_update_beacon_info_lifetimes(fhss_struct, 600100000);
    if (!fhss_struct->fhss_beacon_info_store || fhss_struct->fhss_beacon_info_store->next) {
        return false;
    }
    // Second Beacon info is removed after this call
    fhss_update_beacon_info_lifetimes(fhss_struct, fhss_struct->fhss_beacon_info_store->timestamp + 600200000);
    if (fhss_struct->fhss_beacon_info_store) {
        return false;
    }

    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_failed_handle()
{
    uint8_t handle = 99;
    fhss_struct = malloc(sizeof(fhss_structure_t));
    ns_list_init(&fhss_struct->fhss_failed_tx_list);
    // Test without memory
    nsdynmemlib_stub.returnCounter = 0;
    if (fhss_failed_handle_add(fhss_struct, handle) != -2) {
        return false;
    }
    // Test with memory
    nsdynmemlib_stub.returnCounter = 1;
    if (fhss_failed_handle_add(fhss_struct, handle) != 0) {
        return false;
    }
    // Remove invalid handle
    if (fhss_failed_handle_remove(fhss_struct, handle-1) != -1) {
        return false;
    }
    // Remove valid handle
    if (fhss_failed_handle_remove(fhss_struct, handle) != 0) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_is_synch_root()
{
    fhss_struct = malloc(sizeof(fhss_structure_t));
    // Test when is root
    fhss_struct->own_hop = 0;
    if (fhss_is_synch_root(fhss_struct) != true) {
        return false;
    }
    // Test when is not root
    fhss_struct->own_hop = 1;
    if (fhss_is_synch_root(fhss_struct) != false) {
        return false;
    }
    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}
