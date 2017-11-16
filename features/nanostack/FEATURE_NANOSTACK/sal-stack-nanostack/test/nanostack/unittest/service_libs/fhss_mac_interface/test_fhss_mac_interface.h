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
// Test broadcast channel check callback
bool test_fhss_is_broadcast_channel_cb();
// Test broadcast queue check callback
bool test_fhss_use_broadcast_queue_cb();
// Test TX handle callback
bool test_fhss_tx_handle_cb();
// Test TX conditions check callback
bool test_fhss_check_tx_conditions_cb();
// Test frame receive callback
bool test_fhss_receive_frame_cb();
// Test TX done callback
bool test_fhss_data_tx_done_cb();
// Test TX fail callback
bool test_fhss_data_tx_fail_cb();
// Test state set callback
bool test_fhss_synch_state_set_cb();
// Test timestamp callback
bool test_fhss_read_timestamp_cb();
// Test callback init callback
bool test_fhss_init_callbacks_cb();
// Test getting retransmission period callback
bool test_fhss_get_retry_period_cb();

#ifdef __cplusplus
}
#endif

#endif // TEST_FHSS_H

