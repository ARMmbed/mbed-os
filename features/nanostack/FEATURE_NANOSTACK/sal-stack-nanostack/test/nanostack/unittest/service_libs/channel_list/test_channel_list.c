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
#include "Service_Libs/fhss/channel_list.h"
#include "test_channel_list.h"

bool test_channel_list_count_channels()
{
    int channel_count = 0;
    int i = 0, j = 0;
    uint32_t channel_mask[8];
    memset(channel_mask, 0, 8*4);

    // Test filling channel list 1-by-1 from 1 to 256 channel, starting from channel number 0
    for (i=0; i<256; i++) {
        j = i / 32;
        channel_mask[j] |= 1 << (i % 32);
        channel_count = channel_list_count_channels(channel_mask);

        if (channel_count != i+1) {
            return false;
        }
    }
    return true;
}

bool test_empty_channel_list()
{
    int channel_count = 0;
    uint32_t channel_mask[8];
    memset(channel_mask, 0, 8*4);
    // Test empty channel list
    channel_count = channel_list_count_channels(channel_mask);
    if (channel_count != 0) {
        return false;
    }
    return true;
}

bool test_channel_list_count_channels_reverse()
{
    int channel_count = 0;
    int i = 255, j = 7;
    uint32_t channel_mask[8];
    memset(channel_mask, 0, 8*4);

    // Test filling channel list 1-by-1 from 1 to 256 channel, starting from channel number 255
    for (i=255; i>=0; i--) {
        j = i / 32;
        channel_mask[j] |= 1 << (i % 32);
        channel_count = channel_list_count_channels(channel_mask);

        if (channel_count != (256-i)) {
            return false;
        }
    }
    return true;
}

bool test_gap_in_channel_list()
{
    int number_of_bits = 0;
    int channel_count = 0;
    int i = 0, j = 0;
    uint32_t channel_mask[8];
    memset(channel_mask, 0, 8*4);
    // Test channel pattern with gaps in it
    channel_mask[0] = 0x80000001;
    channel_mask[1] = 0xc0000003;
    channel_mask[2] = 0xe0000007;
    channel_mask[3] = 0xf000000f;
    channel_mask[4] = 0xf800001f;
    channel_mask[5] = 0xfc00003f;
    channel_mask[6] = 0xfe00007f;
    channel_mask[7] = 0xff0000ff;

    for (i=0; i<8; i++) {
        for (j=0; j<32; j++) {
            if (channel_mask[i] & (1 <<j)) {
                number_of_bits++;
            }
        }
    }
    channel_count = channel_list_count_channels(channel_mask);
    if (channel_count != number_of_bits) {
        return false;
    }
    return true;
}

bool test_channel_get()
{
    uint32_t channel_mask[8];
    int i;
    memset(channel_mask, 0, sizeof(channel_mask));
    if (channel_list_get_channel(channel_mask, 256) != 0) {
        return false;
    }
    // Test full channel list
    for (i=0; i<8; i++) {
        channel_mask[i] = 0xffffffff;
    }
    // With full channel list returned channel number should be same as given index
    for (i=0; i<256; i++) {
        if (channel_list_get_channel(channel_mask, i) != i) {
            return false;
        }
    }
    memset(channel_mask, 0, 8*4);
    // Test with channels 128-255 enabled
    for (i=4; i<8; i++) {
        channel_mask[i] = 0xffffffff;
    }
    for (i=0; i<128; i++) {
        if (channel_list_get_channel(channel_mask, i) != i+128) {
            return false;
        }
    }
    memset(channel_mask, 0, 8*4);
    // Test with only odd channels used (1, 3, 5,...)
    for (i=0; i<8; i++) {
        channel_mask[i] = 0xaaaaaaaa;
    }
    for (i=0; i<128; i++) {
        if (channel_list_get_channel(channel_mask, i) != ((i*2)+1)) {
            return false;
        }
    }
    return true;
}
