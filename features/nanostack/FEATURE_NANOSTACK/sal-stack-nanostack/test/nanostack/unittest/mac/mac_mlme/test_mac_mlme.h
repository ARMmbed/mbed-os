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
#ifndef TEST_MAC_MLME_H
#define TEST_MAC_MLME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_mlme_link_beacon_join_priority_tx_callback_set();

bool test_mac_mlme_link_beacon_compare_rx_callback_set();

bool test_mac_mlme_scan_confirmation_handle();

bool test_mac_mlme_scan_request();

bool test_mac_mlme_energy_scan_request();

bool test_mac_mlme_start_req();

bool test_mac_mlme_reset();

bool test_mac_mlme_set_req();

bool test_mac_mlme_get_req();

bool test_mac_extended_mac_set();

bool test_mac_mlme_poll_req();

bool test_mac_mlme_poll_process_confirm();

bool test_mac_mlme_mac_radio_disabled();

bool test_mac_mlme_mac_radio_enable();

bool test_mac_mlme_rf_channel_change();

bool test_mac_mlme_active_scan_response_timer_start();

bool test_mac_mlme_beacon_notify();

bool test_mac_mlme_event_cb();

bool test_mac_mlme_timer_cb();

bool test_mac_mlme_set_active_state();

bool test_mac_mlme_data_base_allocate();

bool test_mac_mlme_data_base_deallocate();

bool test_mac_mlme_set_new_sqn();

bool test_mac_mlme_set_new_beacon_sqn();

bool test_mac_mlme_get_panid();

bool test_mac_frame_src_address_set_from_interface();

bool test_mac_mlme_beacon_tx();

bool test_mac_mlme_beacon_req_tx();

bool test_mac_mlme_virtual_confirmation_handle();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_MLME_H

