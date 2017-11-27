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
#ifndef TEST_CIPV6_REASSEMBLY_INTERFACE_H
#define TEST_CIPV6_REASSEMBLY_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_reassembly_interface_init();

bool test_reassembly_interface_reset();

bool test_reassembly_interface_free();

bool test_cipv6_frag_reassembly();

bool test_cipv6_frag_timer();


#ifdef __cplusplus
}
#endif

#endif // TEST_CIPV6_REASSEMBLY_INTERFACE_H

