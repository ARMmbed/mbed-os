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
#include "Service_Libs/fhss/channel_list.h"

#include "common_functions.h"
#include "ns_trace.h"

#include <stdint.h>
#include <string.h>

const int CHANNEL_LIST_SIZE_IN_BITS = 8 * 32;

static bool channel_list_bit_test32(uint32_t word, int_fast8_t bit_number);
static bool channel_list_bit_test(const uint32_t *list, int bit_number);

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

static bool channel_list_bit_test(const uint32_t *list, int bit_number)
{
    const int_fast8_t word_index = bit_number / 32;
    const int_fast8_t bit_index = bit_number % 32;

    return channel_list_bit_test32(list[word_index], bit_index);
}

static uint8_t channel_list_search(const uint32_t *list, int index)
{
    uint8_t channel = 0;
    int enabled_channels = 0;
    int i, j;

    for (j = 0; j < 8; j++) {
        for (i = 0; i < 32; i++) {
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

uint8_t channel_list_get_channel(const uint32_t *list, int current_index)
{
    uint8_t found_index;

    if (current_index >= CHANNEL_LIST_SIZE_IN_BITS) {
        current_index = 0;
    }

    found_index = channel_list_search(list, current_index);

    return found_index;
}

void channel_list_set_channel(uint32_t *list, int channel, bool active)
{
    if (channel >= CHANNEL_LIST_SIZE_IN_BITS) {
        return;
    }
    if (active) {
        list[channel / 32] |= (1 << channel % 32);
    } else {
        list[channel / 32] &= ~(1 << channel % 32);
    }
    return;
}

// count the amount of channels enabled in a list
int channel_list_count_channels(const uint32_t *list)
{

    int channel_count = 0;

    for (int index = 0; index < CHANNEL_LIST_SIZE_IN_BITS; index++) {

        if (channel_list_bit_test(list, index)) {
            channel_count++;
        }
    }

    return channel_count;
}
