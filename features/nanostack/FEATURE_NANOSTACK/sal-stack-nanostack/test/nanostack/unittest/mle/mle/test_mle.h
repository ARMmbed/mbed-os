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
#ifndef TEST_MLE_H
#define TEST_MLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


bool test_mle_class_init();
bool test_mle_class_deallocate();
bool test_mle_class_mode_set();
bool test_mle_class_list_clean();

bool test_mle_class_get_entry_by_ll64();
bool test_mle_class_discover_entry_by_ll64();
bool test_mle_class_get_by_link_address();
bool test_mle_class_get_by_device_attribute_id();
bool test_mle_class_remove_entry();
bool test_mle_class_remove_neighbour();
bool test_mle_class_active_list_get();
bool test_mle_class_active_neigh_counter();
bool test_mle_refresh_entry_timeout();
bool test_mle_class_set_new_key_pending();
bool test_mle_class_free_entry_count_get();

bool test_mle_entry_timeout_update();

bool test_mle_event_handler();

bool test_mle_neigh_entry_frame_counter_update();
bool test_mle_general_write_source_address();

bool test_mle_general_write_link_layer_framecounter();


#ifdef __cplusplus
}
#endif

#endif // TEST_MLE_H

