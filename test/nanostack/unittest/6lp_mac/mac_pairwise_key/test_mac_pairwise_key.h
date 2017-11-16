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
#ifndef TEST_MAC_PAIRWISE_KEY_H
#define TEST_MAC_PAIRWISE_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_mac_pairwise_key_add();

bool test_mac_pairwise_key_del();

bool test_mac_pairwise_key_flush_list();

bool test_mac_pairwise_key_interface_register();

bool test_mac_pairwise_key_interface_unregister();


#ifdef __cplusplus
}
#endif

#endif // TEST_MAC_PAIRWISE_KEY_H

