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
#ifndef TEST_RF_DRIVER_STORAGE_H
#define TEST_RF_DRIVER_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_arm_net_phy_register();

bool test_arm_net_phy_init();

bool test_arm_net_phy_driver_pointer();

//bool test_dev_get_phy_datarate();

bool test_arm_net_phy_mac64_set();

bool test_arm_net_phy_mac64_get();

bool test_arm_net_phy_rf_type();

bool test_arm_net_phy_mtu_size();

bool test_arm_net_observer_cb_set();

#ifdef __cplusplus
}
#endif

#endif // TEST_RF_DRIVER_STORAGE_H

