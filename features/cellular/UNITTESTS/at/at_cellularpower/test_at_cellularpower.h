/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#ifndef TEST_AT_CELLULARPOWER_H
#define TEST_AT_CELLULARPOWER_H

class Test_AT_CellularPower
{
public:
    Test_AT_CellularPower();

    virtual ~Test_AT_CellularPower();

    void test_AT_CellularPower_constructor();

    void test_AT_CellularPower_on();

    void test_AT_CellularPower_off();

    void test_AT_CellularPower_set_at_mode();

    void test_AT_CellularPower_set_power_level();

    void test_AT_CellularPower_reset();

    void test_AT_CellularPower_opt_power_save_mode();

    void test_AT_CellularPower_opt_receive_period();

    void test_AT_CellularPower_is_device_ready();

    void test_AT_CellularPower_set_device_ready_urc_cb();

    void test_AT_CellularPower_remove_device_ready_urc_cb();
};

#endif // TEST_AT_CELLULARPOWER_H

