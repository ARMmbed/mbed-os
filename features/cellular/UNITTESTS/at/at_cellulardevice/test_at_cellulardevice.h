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
#ifndef TEST_AT_CELLULARDEVICE_H
#define TEST_AT_CELLULARDEVICE_H

class Test_AT_CellularDevice
{
public:
    Test_AT_CellularDevice();

    virtual ~Test_AT_CellularDevice();

    void test_AT_CellularDevice_constructor();

    void test_AT_CellularDevice_get_at_handler(); //tests also releasing of those

    void test_AT_CellularDevice_open_network();

    void test_AT_CellularDevice_open_sms();

    void test_AT_CellularDevice_open_power();

    void test_AT_CellularDevice_open_sim();

    void test_AT_CellularDevice_open_information();

    void test_AT_CellularDevice_close_network();

    void test_AT_CellularDevice_close_sms();

    void test_AT_CellularDevice_close_power();

    void test_AT_CellularDevice_close_sim();

    void test_AT_CellularDevice_close_information();

    void test_AT_CellularDevice_set_timeout();

    void test_AT_CellularDevice_modem_debug_on();

    void test_AT_CellularDevice_get_stack();
};

#endif // TEST_AT_CELLULARDEVICE_H

