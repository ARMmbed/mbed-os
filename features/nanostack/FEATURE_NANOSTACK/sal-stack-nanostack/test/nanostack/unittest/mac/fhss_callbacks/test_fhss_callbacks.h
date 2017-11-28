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
#ifndef TEST_MAC_FILTER_H
#define TEST_MAC_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Test reading queue sizes callback
bool test_mac_read_tx_queue_sizes();
// Test reading MAC address callback
bool test_mac_read_64bit_mac_address();
// Test reading datarate callback
bool test_mac_read_phy_datarate();
// Test channel setting callback
bool test_mac_set_channel();
// Test sending FHSS frame callback
bool test_mac_fhss_frame_tx();
bool test_mac_synch_lost();
// Test polling TX queue callback
bool test_mac_poll_tx_queue();

bool test_mac_broadcast_notification();
bool test_mac_get_coordinator_mac_address();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_FILTER_H

