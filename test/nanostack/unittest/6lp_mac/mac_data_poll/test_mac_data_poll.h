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
#ifndef TEST_MAC_DATA_POLL_H
#define TEST_MAC_DATA_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_poll_timer_trig();

bool test_mac_mlme_poll_confirm();

bool test_mac_data_poll_disable();

bool test_mac_data_poll_init_protocol_poll();

bool test_mac_data_poll_host_poll_time_max();

bool test_mac_data_poll_host_timeout();

bool test_mac_data_poll_protocol_poll_mode_decrement();

bool test_mac_data_poll_protocol_poll_mode_disable();

bool test_mac_data_poll_enable_check();

bool test_mac_data_poll_host_mode_get();

bool test_mac_data_poll_host_mode_set();

bool test_mac_data_poll_init();

bool test_mac_data_poll_get_max_sleep_period();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_DATA_POLL_H

