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
#include "fhss_api.h"
#include "fhss_config.h"
#include "fhss.h"
#include "fhss_beacon.h"

#include "common_functions.h"
#include <string.h> // memcpy
#include "ns_trace.h"

#define TRACE_GROUP "fhss"

// This function should be called just prior actually sending a beacon packet
// to get precise information. Especially the time variables are essential.
int fhss_beacon_update_payload(fhss_structure_t *fhss_structure,
        fhss_synchronization_beacon_payload_s *payload)
{

    int ret_val = 0;
    if (!fhss_structure || !payload) {
        return -1;
    }

    const fhss_synch_configuration_t *config = &fhss_structure->synch_configuration;

    payload->channel_index = fhss_structure->current_channel_index;

    payload->sender_unicast_channel = 0;

    payload->current_superframe = fhss_structure->current_superframe;

    // This assumes that the time is always in the range of 0..2**16, which
    // should be the case as the superframe length field is also in that range.
    payload->remaining_slots = (uint16_t) fhss_get_remaining_time_to_next_superframe(fhss_structure);
    payload->channel_list_counter = fhss_structure->channel_list_counter;

    payload->hop_count = fhss_structure->own_hop;
    payload->number_of_broadcast_channels = config->fhss_number_of_bc_channels;
    payload->number_of_tx_slots = config->fhss_number_of_tx_slots;
    payload->time_since_last_beacon = 0; // XXX not available yet
    // TODO: Get Beacon length from MAC
    uint32_t tx_time = fhss_get_tx_time(fhss_structure, 71, 0, 0);
    payload->processing_delay = fhss_structure->fhss_configuration.fhss_tuning_parameters.tx_processing_delay + tx_time;

    payload->superframe_length = config->fhss_superframe_length;

    payload->number_of_superframes_per_channel = config->fhss_number_of_superframes;

    return ret_val;
}

uint8_t* fhss_beacon_encode_raw(uint8_t* buffer, const fhss_synchronization_beacon_payload_s* source)
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

uint8_t fhss_calculate_uc_index(uint8_t channel_index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels)
{
    // When channel index is 0, return last unicast index
    if (channel_index == 0) {
        return (number_of_channels - number_of_broadcast_channels - 1);
    }
    uint16_t bc_channel_density = (number_of_channels/number_of_broadcast_channels);
    return channel_index - (channel_index/bc_channel_density) - 1;
}


uint32_t fhss_get_time_to_next_channel_change(uint16_t remaining_slots_to_next_superframe, uint8_t number_of_superframes,
        uint8_t current_superframe, uint16_t superframe_length)
{
    return remaining_slots_to_next_superframe + ((uint32_t)((number_of_superframes - 1) - current_superframe) * superframe_length);
}

void fhss_beacon_decode_raw(fhss_synchronization_beacon_payload_s* dest, const uint8_t* buffer)
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

// Decode the given raw byte buffer into a struct into dest struct and calculate
// the new values for elapsed_time, channel_index, current_superframe and remaining_slots 
// from current state and given data.
void fhss_beacon_decode(fhss_synchronization_beacon_payload_s* dest, const uint8_t* buffer, uint32_t elapsed_time, uint16_t number_of_channels)
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
