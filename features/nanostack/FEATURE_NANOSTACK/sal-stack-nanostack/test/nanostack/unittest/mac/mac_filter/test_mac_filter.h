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
#ifndef TEST_MAC_FILTER_H
#define TEST_MAC_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_filter_modify_link_quality();

bool test_mac_filter_start();

bool test_mac_filter_stop();

bool test_mac_filter_clear();

bool test_mac_filter_set_address_mapping();

bool test_mac_filter_delete_short();

bool test_mac_filter_delete_long();

bool test_mac_filter_add_short();

bool test_mac_filter_add_long();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_FILTER_H

