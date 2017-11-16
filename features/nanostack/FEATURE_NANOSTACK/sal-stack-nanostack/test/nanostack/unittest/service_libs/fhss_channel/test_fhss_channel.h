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
#ifndef TEST_FHSS_H
#define TEST_FHSS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Test creating of randomized channel sequence
bool test_channel_randomization();
// Test channel sequence is not repeated too early
bool test_channel_list_repeating_50_channels();
bool test_channel_list_repeating_64_channels();
// Test all channels are used equally
bool test_channels_used_equally();
// Test checking type of current channel
bool test_fhss_is_current_channel_broadcast();
// Test channel update
bool test_fhss_update_channel();
// Test changing to TX channel
bool test_fhss_change_to_tx_channel();
// Test changing to parent channel
bool test_fhss_change_to_parent_channel();
// Test changing to RX channel
bool test_fhss_change_to_rx_channel();
// Test getting channel list offset
bool test_fhss_get_offset();
#ifdef __cplusplus
}
#endif

#endif // TEST_FHSS_H

