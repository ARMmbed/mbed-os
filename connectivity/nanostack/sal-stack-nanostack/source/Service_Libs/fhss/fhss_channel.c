/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "randLIB.h"
#include "ns_trace.h"

#define TRACE_GROUP "fhss"

#ifdef FHSS_CHANNEL_DEBUG_CBS
void (*fhss_uc_switch)(void) = NULL;
void (*fhss_bc_switch)(void) = NULL;
#endif /*FHSS_CHANNEL_DEBUG_CBS*/

#ifdef FHSS_CHANNEL_DEBUG
uint8_t debug_destination_channel = 0;
#endif /*FHSS_CHANNEL_DEBUG*/

static uint8_t fhss_get_bc_index(const fhss_structure_t *fhss_structure);

uint8_t fhss_calc_channel_shuffle(uint8_t index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels)
{
    /* To break the ...1, 2, 3, 4,... hopping sequence and to spread broadcast channels equally on frequency band.
     *
     * Also to randomise the sequence, for Unicast channels:
     * - Even indexes are divided by 2
     * - Odd indexes are divided by 2 and result is subtracted from highest Unicast index (which is (number_of_channels-number_of_broadcast_channels) - 1)
     *
     *
     * Example:
     * Hopping sequence:      Without Unicast randomising:      With Unicast randomising:
     *  0,  1,  2,  3,        0,  4,  8, 12,                    0, 14,  4, 10,
     *  4,  5,  6,  7,        1,  5,  9, 13,                    8,  6, 12,  2,
     *  8,  9, 10, 11,        2,  6, 10, 14,                    1, 13,  5,  9,
     * 12, 13, 14, 15         3,  7, 11, 15,                    3,  7, 11, 15,
     *
     * If number of broadcast channels is 4 (last 4 indexes 3, 7, 11, 15), the hopping sequence is: 3, 0, 14, 4, 7, 10, 8, 6, 11, 12, 2, 1, 15, 13, 5, 9
     */
#ifndef DISABLE_CHANNEL_SHUFFLE
    // Unicast randomising
    if (index < (number_of_channels - number_of_broadcast_channels)) {
        if (!(index % 2)) {
            index /= 2;
        } else {
            index = ((number_of_channels - number_of_broadcast_channels) - 1) - (index / 2);
        }
    }
    // Spread Broadcast channels
    index = (index % number_of_broadcast_channels) * (number_of_channels / number_of_broadcast_channels) + (index / number_of_broadcast_channels);
#endif /*DISABLE_CHANNEL_SHUFFLE*/
    return index;
}

uint8_t fhss_add_channel_list_counter(uint8_t index, uint16_t number_of_channels, uint16_t channel_list_counter, uint8_t *scramble_table)
{
    /* To avoid repeating same channel list constantly, channel list counter is added to given index*/
#ifndef DISABLE_CHANNEL_COUNTER

    /* Break repeating cycle when channel list counter reaches the number of channels.
     * Using known channel list counter and generated scramble table, create pseudo-random index that changes the repeated cycle longer.
     * All channels are equally used as broadcast and unicast channels
     *
     *  If number of channels is 50, number of scramble table indexes is 10 and channel dwell time is 400ms, the repeated cycle starts from beginning after 50 * 50 * 10 * 400ms = 166min
     */
    uint32_t index_tmp;
    uint8_t calc_tmp = (channel_list_counter / number_of_channels) % MAX_SCRAMBLE_TABLE_INDEXES;
    index_tmp = (uint32_t) channel_list_counter * scramble_table[calc_tmp];

    index_tmp += index;
    index_tmp %= number_of_channels;
    index = index_tmp;
#endif /*DISABLE_CHANNEL_COUNTER*/
    return index;
}

static void fhss_generate_broadcast_start_superframe(fhss_structure_t *fhss_structure)
{
    // If the number of superframes is low, allow broadcast on any superframe
    if (fhss_structure->bs->synch_configuration.fhss_number_of_superframes < 8) {
        fhss_structure->bs->broadcast_start_superframe = 0;
    } else {
        fhss_structure->bs->broadcast_start_superframe = randLIB_get_random_in_range(0, NUMBER_OF_BC_START_SUPERFRAMES - 1);
    }
}

/**
 * Update channel
 *
 * This function is called by superframe handler on first(0) superframe
 * of every channel to resolve and change new channel.
 *
 * @param cur network interface to work on
 * @return true if changed to broadcast channel, false otherwise
 */
bool fhss_change_to_next_channel(fhss_structure_t *fhss_structure)
{
    int next_channel;
    bool broadcast_channel = false;

    uint16_t number_of_channels = fhss_structure->number_of_channels;
    uint8_t number_of_broadcast_channels = fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels;
    uint8_t unicast_channel_index = fhss_structure->bs->uc_channel_index;
    uint8_t channel_index_tmp;

    /* Get the channel number using channel index. Latter (number_of_broadcast_channels) indexes in channel table are broadcast channels and
     * first (number_of_channels - number_of_broadcast_channels) are unicast channels.
     * In channel hopping sequence, every (number_of_channels / number_of_broadcast_channels) channel is broadcast channel and
     * channel hopping sequence is e.g. |uc0|uc1|uc2|bc0|uc3|uc4|uc5|bc1|uc6|...
     */
    /* Get broadcast channel */
    if (fhss_is_current_channel_broadcast(fhss_structure) == true) {
        channel_index_tmp = fhss_calc_channel_shuffle((number_of_channels - number_of_broadcast_channels) + fhss_get_bc_index(fhss_structure), fhss_structure->number_of_channels, fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
        fhss_generate_broadcast_start_superframe(fhss_structure);
        broadcast_channel = true;
    } else { /* Get unicast channel */
        channel_index_tmp = fhss_calc_channel_shuffle(unicast_channel_index, fhss_structure->number_of_channels, fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
        if (++fhss_structure->bs->uc_channel_index >= number_of_channels - number_of_broadcast_channels) {
            fhss_structure->bs->uc_channel_index = 0;
        }
    }
    // Reset Beacon received flag when channel has changed
    fhss_structure->bs->beacon_received_on_this_bc_channel = false;
    channel_index_tmp = fhss_add_channel_list_counter(channel_index_tmp, fhss_structure->number_of_channels, fhss_structure->bs->channel_list_counter, fhss_structure->bs->fhss_scramble_table);
    next_channel = channel_list_get_channel(fhss_structure->bs->fhss_configuration.channel_mask, channel_index_tmp);

    fhss_structure->rx_channel = next_channel;

    if (fhss_is_current_channel_broadcast(fhss_structure) == true) {
#ifdef FHSS_CHANNEL_DEBUG
        tr_info("%"PRIu32" BC %u", fhss_structure->platform_functions.fhss_get_timestamp(fhss_structure->fhss_api), next_channel);
#endif /*FHSS_CHANNEL_DEBUG*/
    } else {
#ifdef FHSS_CHANNEL_DEBUG_CBS
        if (fhss_uc_switch) {
            fhss_uc_switch();
        }
#endif /*FHSS_CHANNEL_DEBUG_CBS*/
#ifdef FHSS_CHANNEL_DEBUG
        tr_info("%"PRIu32" UC %u", fhss_structure->platform_functions.fhss_get_timestamp(fhss_structure->fhss_api), next_channel);
#endif /*FHSS_CHANNEL_DEBUG*/
    }

    fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, next_channel);
    return broadcast_channel;
}

static uint8_t fhss_get_bc_index(const fhss_structure_t *fhss_structure)
{
    uint16_t number_of_channels = fhss_structure->number_of_channels;
    uint8_t number_of_bc_channels = fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels;
    uint8_t cur_channel_index = fhss_structure->bs->current_channel_index;

    return cur_channel_index / (number_of_channels / number_of_bc_channels);
}

uint8_t fhss_get_offset(fhss_structure_t *fhss_structure, const uint8_t *ptr)
{
    uint8_t i;
    uint8_t index = *ptr++;

    if (fhss_structure->number_of_channels == fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) {
        // If all channels are defined as broadcast channels then return 0 to avoid division by 0.
        // This could happen e.g. in OTA case when fast download is needed.
        return 0;
    }

    // Offset to unicast channel index is calculated using XOR operation
    for (i = 0; i < 7; i++) {
        index ^= *ptr++;
    }
    // Offset must be < number of unicast channels
    index %= (fhss_structure->number_of_channels - fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);

    return index;
}

bool fhss_is_current_channel_broadcast(fhss_structure_t *fhss_structure)
{
    // Every channel is broadcast channel when FHSS is not enabled
    if (!fhss_structure) {
        return true;
    }

    // Should always have broadcast channels with FHSS
    if (!fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) {
        return true;
    }

    uint8_t channel_index = fhss_structure->bs->current_channel_index;
    uint16_t number_of_channels = fhss_structure->number_of_channels;
    uint8_t number_of_broadcast_channels = fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels;

    if (!(channel_index % (number_of_channels / number_of_broadcast_channels))) {
        return true;
    }
    return false;
}

static uint8_t fhss_get_destination_channel(fhss_structure_t *fhss_structure, uint8_t *destination_address)
{
    uint8_t destination_offset;
    uint8_t uc_index;

    if (fhss_structure) {
        if (fhss_is_current_channel_broadcast(fhss_structure) == false) {
            destination_offset = fhss_get_offset(fhss_structure, destination_address);
            uc_index = fhss_calculate_uc_index(fhss_structure->bs->current_channel_index, fhss_structure->number_of_channels,
                                               fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) + destination_offset;
            if (uc_index >= (fhss_structure->number_of_channels - fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels)) {
                uc_index -= (fhss_structure->number_of_channels - fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
            }

            uc_index = fhss_calc_channel_shuffle(uc_index, fhss_structure->number_of_channels, fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
            uc_index = fhss_add_channel_list_counter(uc_index, fhss_structure->number_of_channels, fhss_structure->bs->channel_list_counter, fhss_structure->bs->fhss_scramble_table);
            return channel_list_get_channel(fhss_structure->bs->fhss_configuration.channel_mask, uc_index);
        }
        return fhss_structure->rx_channel;
    }
    return 0;
}

int fhss_change_to_tx_channel(fhss_structure_t *fhss_structure, uint8_t *destination_address)
{
    if (fhss_structure) {
        if (fhss_structure->fhss_state != FHSS_UNSYNCHRONIZED) {
            uint8_t destination_channel = fhss_get_destination_channel(fhss_structure, destination_address);
            fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, destination_channel);
#ifdef FHSS_CHANNEL_DEBUG
            debug_destination_channel = destination_channel;
#endif /*FHSS_CHANNEL_DEBUG*/
        }
    }
    return 0;
}

int fhss_change_to_parent_channel(fhss_structure_t *fhss_structure)
{
    uint8_t uc_index;
    uint8_t destination_channel;
    uint8_t destination_offset;
    if (fhss_structure) {
        if (fhss_structure->number_of_channels != fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) {
            uint8_t parent_address[8];
            if (fhss_get_parent_address(fhss_structure, parent_address)) {
                return -1;
            }

            destination_offset = fhss_get_offset(fhss_structure, parent_address);

            uc_index = fhss_calculate_uc_index(fhss_structure->bs->current_channel_index, fhss_structure->number_of_channels,
                                               fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels) + destination_offset;
            if (uc_index >= (fhss_structure->number_of_channels - fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels)) {
                uc_index -= (fhss_structure->number_of_channels - fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
            }
            uc_index = fhss_calc_channel_shuffle(uc_index, fhss_structure->number_of_channels, fhss_structure->bs->synch_configuration.fhss_number_of_bc_channels);
            uc_index = fhss_add_channel_list_counter(uc_index, fhss_structure->number_of_channels, fhss_structure->bs->channel_list_counter, fhss_structure->bs->fhss_scramble_table);
            destination_channel = channel_list_get_channel(fhss_structure->bs->fhss_configuration.channel_mask, uc_index);
            fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, destination_channel);
#ifdef FHSS_CHANNEL_DEBUG
            tr_info("Parent channel: %u", destination_channel);
#endif /*FHSS_CHANNEL_DEBUG*/
        }
    }
    return 0;
}

int fhss_change_to_rx_channel(fhss_structure_t *fhss_structure)
{
    if (fhss_structure) {
        if (fhss_structure->fhss_state == FHSS_SYNCHRONIZED) {
            fhss_structure->callbacks.change_channel(fhss_structure->fhss_api, fhss_structure->rx_channel);
        }
        return 0;
    }
    return -1;
}
