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
#ifndef TEST_MAC_SECURITY_MIB_H
#define TEST_MAC_SECURITY_MIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_sec_mib_init();

bool test_mac_sec_mib_deinit();

bool test_mac_sec_mib_device_description_set();

bool test_mac_sec_mib_key_description_set();

bool test_mac_sec_mib_device_description_get_attribute_index();

bool test_mac_sec_mib_device_description_get();

bool test_mac_mib_device_descption_attribute_get_by_descriptor();

bool test_mac_sec_key_description_get();

bool test_mac_sec_mib_key_device_description_list_update();

bool test_mac_sec_mib_key_device_description_discover_from_list();

bool test_mac_sec_mib_device_description_blacklist();

#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_SECURITY_MIB_H

