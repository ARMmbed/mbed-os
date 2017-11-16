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
#ifndef TEST_ETHERNET_MAC_API_H
#define TEST_ETHERNET_MAC_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ethernet_mac_create();

bool test_ethernet_mac_destroy();

bool test_data_request();

bool test_data_confirm();

bool test_data_indication();

bool test_eth_mac_mac48_address_set();

bool test_eth_mac_mac48_address_get();

bool test_eth_mac_api_initialize();

bool test_rx_cb();

bool test_tx_done_cb();

#ifdef __cplusplus
}
#endif

#endif // TEST_ETHERNET_MAC_API_H

