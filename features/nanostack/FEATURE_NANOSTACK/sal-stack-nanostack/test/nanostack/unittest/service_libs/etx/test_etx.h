/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

bool test_ETX_etx_transm_attempts_update_1();
bool test_ETX_etx_transm_attempts_update_2();
bool test_ETX_etx_transm_attempts_update_3();
bool test_ETX_etx_transm_attempts_update_4();
bool test_ETX_etx_transm_attempts_update_5();
bool test_ETX_etx_read_1();
bool test_ETX_etx_read_2();
bool test_ETX_etx_read_3();
bool test_ETX_etx_remote_incoming_idr_update_1();
bool test_ETX_etx_local_incoming_idr_read_1();
bool test_ETX_etx_local_incoming_idr_read_2();
bool test_ETX_etx_lqi_dbm_update_1();
bool test_ETX_etx_lqi_dbm_update_2();
bool test_ETX_etx_value_change_callback_register_1();
bool test_ETX_etx_accum_failures_callback_register_1();
bool test_ETX_etx_neighbor_remove_1();
bool test_ETX_etx_neighbor_add_1();

#ifdef __cplusplus
}
#endif

#endif // TEST_ETX_H

