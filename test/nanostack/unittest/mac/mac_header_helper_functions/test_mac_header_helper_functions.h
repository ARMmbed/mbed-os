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
#ifndef TEST_MAC_HEADER_HELPER_FUNCTIONS_H
#define TEST_MAC_HEADER_HELPER_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_security_mic_length_get();

bool test_mac_header_security_aux_header_length();

bool test_mac_header_address_length();

bool test_mac_header_security_parameter_set();

bool test_mac_header_parse_fcf_dsn();

bool test_mac_header_security_components_read();

bool test_mac_header_get_src_panid();

bool test_mac_header_get_dst_panid();

bool test_mac_header_get_src_address();

bool test_mac_header_get_dst_address();

bool test_mcps_payload_length_from_received_frame();

bool test_mcps_mac_header_length_from_received_frame();

bool test_mcps_mac_security_frame_counter_read();

bool test_mcps_mac_command_frame_id_get();

bool test_mcps_mac_payload_pointer_get();

bool test_mcps_security_mic_pointer_get();

bool test_mcps_mac_security_aux_header_start_pointer_get();

bool test_mcps_generic_header_write();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_HEADER_HELPER_FUNCTIONS_H

