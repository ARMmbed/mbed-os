/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#ifndef TEST_CHANNEL_LIST_H
#define TEST_CHANNEL_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Test filling channel list 1-by-1 from 1 to 256 channel, starting from channel number 0
bool test_channel_list_count_channels();
// Test empty channel list
bool test_empty_channel_list();
// Test filling channel list 1-by-1 from 1 to 256 channel, starting from channel number 255
bool test_channel_list_count_channels_reverse();
// Test if there is a gap in channel mask
bool test_gap_in_channel_list();
// Test getting a channel using channel index
bool test_channel_get();


#ifdef __cplusplus
}
#endif

#endif // TEST_CHANNEL_LIST_H

