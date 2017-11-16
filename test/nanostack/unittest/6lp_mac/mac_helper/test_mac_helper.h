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
#ifndef TEST_MAC_HELPER_H
#define TEST_MAC_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_create_scan_request();

bool test_mac_helper_select_best_lqi();

bool test_mac_helper_drop_selected_from_the_scanresult();

bool test_mac_helper_free_scan_confirm();

bool test_mac_helper_free_pan_descriptions();

bool test_mac_helper_nwk_id_filter_set();

bool test_mac_helper_panid_set();

bool test_mac_helper_mac16_address_set();

bool test_mac_helper_mac16_address_get();

bool test_mac_helper_panid_get();

bool test_mac_helper_set_default_key_source();

bool test_mac_helper_default_security_level_set();

bool test_mac_helper_default_security_level_get();

bool test_mac_helper_default_security_key_id_mode_set();

bool test_mac_helper_default_security_key_id_mode_get();

bool test_mac_helper_default_key_index_set();

bool test_mac_helper_default_key_index_get();

bool test_mac_helper_security_default_key_set();

bool test_mac_helper_security_pairwisekey_set();

bool test_mac_helper_security_next_key_set();

bool test_mac_helper_security_prev_key_set();

bool test_mac_helper_security_key_swap_next_to_default();

bool test_mac_helper_security_key_clean();

bool test_mac_helper_coordinator_address_set();

bool test_mac_helper_coordinator_address_get();

bool test_mac_helper_beacon_payload_reallocate();

bool test_mac_helper_beacon_payload_register();

bool test_mac_helper_beacon_payload_pointer_get();

bool test_mac_helper_beacon_payload_length_get();

bool test_mac_helper_pib_boolean_set();

bool test_mac_helper_mac_channel_set();

bool test_mac_helper_write_our_addr();

bool test_mac_helper_mac64_set();

bool test_mac_helper_max_payload_size();

bool test_mac_helper_frame_overhead();

bool test_mac_helper_link_frame_counter_read();

bool test_mac_helper_link_frame_counter_set();

bool test_mac_helper_devicetable_remove();

bool test_mac_helper_devicetable_set();

bool test_mac_helper_mac_mlme_max_retry_set();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_HELPER_H

