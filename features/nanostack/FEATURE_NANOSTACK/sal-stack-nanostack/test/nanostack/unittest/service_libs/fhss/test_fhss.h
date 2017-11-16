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

// Test enabling FHSS
bool test_fhss_enable();
// Test disabling FHSS
bool test_fhss_disable();
// Test channel type TX condition
bool test_fhss_check_channel_type();
// Test TX allowed TX condition
bool test_fhss_check_tx_allowed();
// Test bad channel TX condition
bool test_fhss_check_bad_channel();
// Test synchronization time configuration
bool test_fhss_set_synch_configuration();
// Test storing Beacon info
bool test_fhss_add_beacon_info();
// Test setting datarate
bool test_fhss_set_datarate();
// Test synchronization to Beacon
bool test_fhss_sync_with_beacon();
// Test requesting parent MAC address
bool test_fhss_get_parent_address();
// Test comparing synch parent address
bool test_fhss_compare_with_synch_parent_address();
// Test updating synch parent address
bool test_fhss_update_synch_parent_address();
// Test superframe handler
bool test_fhss_superframe_handler();
// Test event timer callback
bool test_fhss_event_timer_cb();
// Test FHSS down
bool test_fhss_down();
// Test starting FHSS timeout
bool test_fhss_timeout_start();
// Test stopping FHSS timeout
bool test_fhss_timeout_stop();
// Test clearing active event
bool test_fhss_clear_active_event();
// Test updating lifetimes
bool test_fhss_update_beacon_info_lifetimes();
// Test finding failed TX handle
bool test_fhss_failed_handle();
// Test synch root check
bool test_fhss_is_synch_root();

#ifdef __cplusplus
}
#endif

#endif // TEST_FHSS_H

