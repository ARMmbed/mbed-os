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

uint16_t common_read_16_bit(const uint8_t data_buf[__static 2])
{
    uint16_t temp_16;
    temp_16 = (uint16_t)(*data_buf++) << 8;
    temp_16 += *data_buf++;
    return temp_16;
}

uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2])
{
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

uint32_t common_read_32_bit(const uint8_t data_buf[__static 4])
{
    uint32_t temp_32;
    temp_32 = (uint32_t)(*data_buf++) << 24;
    temp_32 += (uint32_t)(*data_buf++) << 16;
    temp_32 += (uint32_t)(*data_buf++) << 8;
    temp_32 += *data_buf++;
    return temp_32;
}

uint8_t *common_write_32_bit(uint32_t value, uint8_t ptr[__static 4])
{
    *ptr++ = value >> 24;
    *ptr++ = value >> 16;
    *ptr++ = value >> 8;
    *ptr++ = value;
    return ptr;
}

bool test_fhss_beacon_update_payload()
{
    uint8_t test_value = 1;
    fhss_synchronization_beacon_payload_s payload;
    memset(&payload, 0, sizeof(fhss_synchronization_beacon_payload_s));
    fhss_structure_t *fhss_struct = malloc(sizeof(fhss_structure_t));

    // Test NULL FHSS structure
    if (fhss_beacon_update_payload(NULL, &payload) != -1) {
        return false;
    }
    // Test NULL payload
    if (fhss_beacon_update_payload(fhss_struct, NULL) != -1) {
        return false;
    }

    fhss_struct->current_channel_index = test_value;
    fhss_struct->current_superframe = test_value;
    fhss_struct->channel_list_counter = test_value;
    fhss_struct->own_hop = test_value;
    fhss_struct->synch_configuration.fhss_number_of_bc_channels = test_value;
    fhss_struct->synch_configuration.fhss_number_of_tx_slots = test_value;
    fhss_struct->fhss_configuration.fhss_tuning_parameters.tx_processing_delay = test_value;
    fhss_struct->synch_configuration.fhss_superframe_length = test_value;
    fhss_struct->synch_configuration.fhss_number_of_superframes = test_value;

    // Test "proper" parameters
    if (fhss_beacon_update_payload(fhss_struct, &payload) != 0) {
        return false;
    }

    fhss_struct->current_channel_index = test_value;

    if (payload.channel_index != test_value) {
        return false;
    }
    if (payload.current_superframe != test_value) {
        return false;
    }
    if (payload.channel_list_counter != test_value) {
        return false;
    }
    if (payload.hop_count != test_value) {
        return false;
    }
    if (payload.number_of_broadcast_channels != test_value) {
        return false;
    }
    if (payload.number_of_tx_slots != test_value) {
        return false;
    }
    if (payload.processing_delay != test_value) {
        return false;
    }
    if (payload.superframe_length != test_value) {
        return false;
    }
    if (payload.number_of_superframes_per_channel != test_value) {
        return false;
    }

    free(fhss_struct);
    fhss_struct = NULL;
    return true;
}

bool test_fhss_beacon_encode_raw()
{
    uint8_t test_value = 1;
    uint8_t buffer[21];
    memset(buffer, 0, sizeof(buffer));
    fhss_synchronization_beacon_payload_s source;

    source.channel_index = test_value;
    source.sender_unicast_channel = test_value;
    source.current_superframe = test_value;
    source.remaining_slots = test_value;
    source.channel_list_counter = test_value;
    source.hop_count = test_value;
    source.number_of_broadcast_channels = test_value;
    source.number_of_tx_slots = test_value;
    source.time_since_last_beacon = test_value;
    source.processing_delay = test_value;
    source.superframe_length = test_value;
    source.number_of_superframes_per_channel = test_value;

    fhss_beacon_encode_raw(buffer, &source);
    // Test values that were not written using common_write -functions
    if (buffer[0] != FHSS_DATA_START_DELIMETER) {
        return false;
    }
    if (buffer[1] != test_value) {
        return false;
    }
    if (buffer[2] != test_value) {
        return false;
    }
    if (buffer[9] != test_value) {
        return false;
    }
    if (buffer[10] != test_value) {
        return false;
    }
    if (buffer[11] != test_value) {
        return false;
    }
    if (buffer[20] != test_value) {
        return false;
    }
    return true;
}

bool test_fhss_calculate_uc_index()
{
    if (fhss_calculate_uc_index(0, 50, 10) != 39) {
        return false;
    }
    if (fhss_calculate_uc_index(1, 50, 10) != 0) {
        return false;
    }
    return true;
}

bool test_fhss_get_time_to_next_channel_change()
{
    if (fhss_get_time_to_next_channel_change(100, 8, 5, 50000) != 100100) {
        return false;
    }
    return true;
}

bool test_fhss_beacon_decode()
{
    uint8_t test_value = 1;
    uint8_t buffer[21];
    // Delimeter
    buffer[0] = FHSS_DATA_START_DELIMETER;
    // Channel index
    buffer[1] = 0;
    // Sender unicast
    buffer[2] = 1;
    // Current superframe
    buffer[3] = 0;
    buffer[4] = 5;
    // Remaining slots
    buffer[5] = 0x86;
    buffer[6] = 0xa0;
    // Channel list counter
    buffer[7] = 0x03;
    buffer[8] = 0xe7;
    // Hop count
    buffer[9] = 1;
    // Number of broadcast channels
    buffer[10] = 10;
    // Number of TX slots
    buffer[11] = 2;
    // Time since last Beacon
    buffer[12] = 0;
    buffer[13] = 0;
    buffer[14] = 0;
    buffer[15] = 0;
    // Processing delay
    buffer[16] = 0;
    buffer[17] = 0;
    // Superframe length
    buffer[18] = 0xc3;
    buffer[19] = 0x50;
    // Number of superframes
    buffer[20] = 8;
    fhss_synchronization_beacon_payload_s dest;
    memset(&dest, 0, sizeof(fhss_synchronization_beacon_payload_s));
    fhss_beacon_decode(&dest, buffer, 1000, 50);

    // Test basic configuration values
    if (dest.current_superframe != 5) {
        return false;
    }
    if (dest.number_of_superframes_per_channel != 8) {
        return false;
    }
    if (dest.superframe_length != 50000) {
        return false;
    }

    // Current superframe
    buffer[3] = 0;
    buffer[4] = 7;
    // Remaining slots
    buffer[5] = 0x00;
    buffer[6] = 0x0f;

    memset(&dest, 0, sizeof(fhss_synchronization_beacon_payload_s));
    fhss_beacon_decode(&dest, buffer, 1000, 50);
    // Test superframe oveflow
    if (dest.current_superframe != 0) {
        return false;
    }
    if (dest.channel_index != 1) {
        return false;
    }

    // Channel index
    buffer[1] = 49;

    memset(&dest, 0, sizeof(fhss_synchronization_beacon_payload_s));
    fhss_beacon_decode(&dest, buffer, 1000, 50);
    // Test channel index and channel list counter overflow
    if (dest.channel_index != 0) {
        return false;
    }
    if (dest.channel_list_counter != 0) {
        return false;
    }
#if 0
    printf("data_start_delimeter: %u\n" ,dest.data_start_delimeter);
    printf("channel_index: %u\n" ,dest.channel_index);
    printf("sender_unicast_channel: %u\n" ,dest.sender_unicast_channel);
    printf("current_superframe: %u\n" ,dest.current_superframe);
    printf("remaining_slots: %u\n" ,dest.remaining_slots);
    printf("channel_list_counter: %u\n" ,dest.channel_list_counter);
    printf("hop_count: %u\n" ,dest.hop_count);
    printf("number_of_broadcast_channels: %u\n" ,dest.number_of_broadcast_channels);
    printf("number_of_tx_slots: %u\n" ,dest.number_of_tx_slots);
    printf("time_since_last_beacon: %u\n" ,dest.time_since_last_beacon);
    printf("processing_delay: %u\n" ,dest.processing_delay);
    printf("superframe_length: %u\n" ,dest.superframe_length);
    printf("number_of_superframes_per_channel: %u\n" ,dest.number_of_superframes_per_channel);
#endif
    return true;
}
