/*
 * Copyright (c) 2016, 2020, Pelion and affiliates.
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
#ifndef TEST_NS_NVM_HELPER_H
#define TEST_NS_NVM_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ns_nvm_helper_write();
bool test_ns_nvm_helper_read();
bool test_ns_nvm_helper_delete();
bool test_ns_nvm_helper_concurrent_requests();
bool test_ns_nvm_helper_platform_error();
bool test_ns_nvm_helper_platform_error_in_write();

#ifdef __cplusplus
}
#endif

#endif // TEST_NS_NVM_HELPER_H

