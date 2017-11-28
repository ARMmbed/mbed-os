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
#ifndef TEST_MAC_MCPS_SAP_H
#define TEST_MAC_MCPS_SAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_generic_event_trig();
// Test sending TX trig event
bool test_mcps_sap_trig_tx();

bool test_mac_virtual_sap_data_cb();

bool test_mac_mcps_sap_tasklet_init();

bool test_mcps_sap_pre_parsed_frame_buffer_free();

bool test_mcps_sap_prebuild_frame_buffer_get();

bool test_mcps_sap_prebuild_frame_buffer_free();

bool test_mcps_sap_pd_req_queue_write();

//bool test_mcps_data_confirm_handle();

bool test_mcps_sap_pre_parsed_frame_buffer_get();

bool test_mcps_sap_pd_ind();

bool test_mcps_sap_pd_confirm();

bool test_mcps_sap_data_req_handler();

bool test_mac_mcps_trig_buffer_from_queue();

bool test_mac_mcps_buffer_queue_free();

bool test_mac_mcps_data_indication();

bool test_mac_mcps_data_confirmation();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_MCPS_SAP_H

