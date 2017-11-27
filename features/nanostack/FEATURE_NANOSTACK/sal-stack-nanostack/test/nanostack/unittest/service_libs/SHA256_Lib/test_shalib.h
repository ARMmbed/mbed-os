/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#ifndef TEST_ETX_H
#define TEST_ETX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_sha256_test_vectors_1(void);
bool test_sha256_test_vectors_2(void);
bool test_sha256_test_vectors_3(void);
bool test_sha256_test_vectors_4(void);

#ifdef __cplusplus
}
#endif

#endif // TEST_ETX_H

