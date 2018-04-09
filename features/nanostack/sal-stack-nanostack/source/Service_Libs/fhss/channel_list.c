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
#include "Service_Libs/fhss/channel_list.h"

#include "common_functions.h"
#include "ns_trace.h"

#include <stdint.h>
#include <string.h>

const int CHANNEL_LIST_SIZE_IN_BITS = 8*32;

static bool channel_list_bit_test32(uint32_t word, int_fast8_t bit_number);
static bool channel_list_bit_test(const uint32_t* list, int bit_number);

#if 0
static void channel_list_bit_set32(uint32_t* word, int_fast8_t bit_number);
static void channel_list_bit_set(uint32_t* list, int bit_number);
#endif

// test bit by number
static bool channel_list_bit_test32(uint32_t word, int_fast8_t bit_number)
{
    bool bitSet;

    if (word & ((uint32_t) 1 << bit_number)) {
        bitSet = true;
    } else {
        bitSet = false;
    }
    return bitSet;
}

static bool channel_list_bit_test(const uint32_t* list, int bit_number)
{
    const int_fast8_t word_index = bit_number / 32;
    const int_fast8_t bit_index = bit_number % 32;

    return channel_list_bit_test32(list[word_index], bit_index);
}
#if 0
// set bit by number
static void channel_list_bit_set32(uint32_t* word, int_fast8_t bit_number)
{
    *word |= ((uint32_t) 1 << bit_number);
}

static void channel_list_bit_set(uint32_t* list, int bit_number)
{
    const int_fast8_t word_index = bit_number / 32;
    const int_fast8_t bit_index = bit_number % 32;

    channel_list_bit_set32(&(list[word_index]), bit_index);
}


void channel_list_print(uint8_t dlevel, const char *grp, const uint32_t* list)
{

    int temp_channel = 0;

#define CHANNELS_PER_LINE 32

    uint8_t channels[CHANNELS_PER_LINE];

    for (int line_index = 0; line_index < (CHANNEL_LIST_SIZE_IN_BITS/CHANNELS_PER_LINE); line_index++) {

        int channels_found = 0;

        for (int row_index = 0; row_index < CHANNELS_PER_LINE; row_index++) {

            if (channel_list_bit_test(list, temp_channel)) {

                channels[channels_found] = temp_channel;
                channels_found++;
            }

            temp_channel++;
        }

        tr_info("arr[%d]: %s", line_index, trace_array(channels, channels_found));
    }
}

// this just avoids mistakes/copypaste on client side
void channel_list_clear_mask(uint32_t* list)
{
    const int mask_size = (sizeof(list));

    memset(list, 0, mask_size);
}

static int channel_list_search_in_range(const uint32_t* list, int start_index, int end_index)
{
    int found_index = -1;
    for (int index = start_index; index <= end_index; index++) {

        if (channel_list_bit_test(list, index)) {
            found_index = index;
            break;
        }
    }
    return found_index;
}

// utility for getting the first channel
int channel_list_get_first(const uint32_t* list)
{

    return channel_list_get_next(list, 0xff);
}
#endif
static uint8_t channel_list_search(const uint32_t* list, int index)
{
    uint8_t channel = 0;
    int enabled_channels = 0;
    int i, j;

    for(j=0; j<8; j++)
    {
        for(i=0; i<32; i++)
        {
            if (list[j] & ((uint32_t)1 << i)) {
                enabled_channels++;
                if (enabled_channels == (index + 1)) {
                    goto exit;
                }
            }
            channel++;
        }
    }
exit:
    return channel;

}

uint8_t channel_list_get_channel(const uint32_t* list, int current_index)
{
    uint8_t found_index;

    if (current_index >= CHANNEL_LIST_SIZE_IN_BITS) {
        current_index = 0;
    }

    found_index = channel_list_search(list, current_index);

    return found_index;
}
#if 0
int channel_list_get_next(const uint32_t* list, int current_index)
{
    int found_index;

    current_index++;

    if (current_index >= CHANNEL_LIST_SIZE_IN_BITS) {

        current_index = 0;
    }

    // One could use a optimization here to avoid looping through masks 1..7
    // if page is not 9 or 10. But is it really worth it?
    found_index = channel_list_search_in_range(list, current_index, CHANNEL_LIST_SIZE_IN_BITS-1);

    if ((found_index < 0) && (current_index > 0)) {

        found_index = channel_list_search_in_range(list, 0, current_index-1);
    }

    return found_index;
}

int channel_list_get_next_broadcast(const uint32_t* list, int broadcast_channel_count, int current_index)
{

    // XXX: all these could/should be pre-calculated on configuration time.
    const int first_broadcast = channel_list_get_first(list);
    const int total_channel_count = channel_list_count_channels(list);
    int channels_to_loop = total_channel_count / broadcast_channel_count; // crash if broadcast is configured to zero

    int next_broadcast = -1;

    // If there are more than one broadcast channels and we're not yet at the last channel,
    // iteratively search for next broadcast channel.
    if ((channels_to_loop > 1) && (current_index < (CHANNEL_LIST_SIZE_IN_BITS - 1))) {

        while (channels_to_loop > 0) {

            current_index = channel_list_get_next(list, current_index);
            channels_to_loop--;
        }
        next_broadcast = current_index;
    }
    else {
        next_broadcast = first_broadcast;
    }

    return next_broadcast;
}
#endif
// count the amount of channels enabled in a list
int channel_list_count_channels(const uint32_t* list)
{

    int channel_count = 0;

    for (int index=0; index < CHANNEL_LIST_SIZE_IN_BITS; index++) {

        if (channel_list_bit_test(list, index)) {
            channel_count++;
        }
    }

    return channel_count;
}
#if 0
int channel_list_enable_channel(uint32_t* list, int channel_number)
{
    int ret_val = -1;

    if ((channel_number >= 0) && (channel_number < CHANNEL_LIST_SIZE_IN_BITS)) {

        channel_list_bit_set(list, channel_number);

        ret_val = 0;
    }

    return ret_val;
}

bool channel_list_is_channel_enabled(const uint32_t* list, int channel_number) {

    int ret_val = false;

    if ((channel_number >= 0) && (channel_number < CHANNEL_LIST_SIZE_IN_BITS)) {

        ret_val = channel_list_bit_test(list, channel_number);
    }

    return ret_val;
}
#endif
