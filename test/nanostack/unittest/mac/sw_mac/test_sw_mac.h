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
#ifndef TEST_SW_MAC_H
#define TEST_SW_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ns_sw_mac_create();

bool test_get_sw_mac_api();

bool test_get_sw_mac_ptr_by_mac_api();

bool test_get_sw_mac_ptr_by_fhss_api();

bool test_get_sw_mac_ptr_by_timer();

bool test_get_sw_mac_ptr_by_driver_id();

//Test also mac functions
bool test_initialize();

bool test_mlme_req();

bool test_mcps_data_req();

bool test_mac64_get();

bool test_mac64_set();

//test low level callbacks
bool test_sw_mac_net_phy_rx();

bool test_sw_mac_net_phy_tx_done();

bool test_sw_mac_storage_decription_sizes_get();

bool test_ns_sw_mac_virtual_client_register();

bool test_ns_sw_mac_virtual_client_unregister();
// Test registering FHSS to software MAC
bool test_ns_sw_mac_fhss_register();

#ifdef __cplusplus
}
#endif

#endif // TEST_SW_MAC_H

