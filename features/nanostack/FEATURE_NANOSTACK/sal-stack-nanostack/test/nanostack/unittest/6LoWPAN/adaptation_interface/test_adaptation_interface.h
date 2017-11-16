/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#ifndef TEST_ADAPTATION_INTERFACE_H
#define TEST_ADAPTATION_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_lowpan_adaptation_interface_init();

bool test_lowpan_adaptation_interface_free();

bool test_lowpan_adaptation_interface_reset();

bool test_lowpan_adapatation_data_process_tx_preprocess();

bool test_lowpan_adaptation_interface_tx();

bool test_lowpan_adaptation_interface_tx_confirm();

bool test_lowpan_adaptation_tx_active();

bool test_lowpan_adaptation_indirect_purge();

#ifdef __cplusplus
}
#endif

#endif // TEST_ADAPTATION_INTERFACE_H

