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
#ifndef TEST_AT_CELLULARSMS_H
#define TEST_AT_CELLULARSMS_H

class Test_AT_CellularSMS
{
public:
    Test_AT_CellularSMS();

    virtual ~Test_AT_CellularSMS();

    void test_AT_CellularSMS_constructor();

    void test_AT_CellularSMS_initialize();

    void test_AT_CellularSMS_send_sms();

    void test_AT_CellularSMS_get_sms();

    void test_AT_CellularSMS_set_sms_callback();

    void test_AT_CellularSMS_set_cpms();

    void test_AT_CellularSMS_set_csca();

    void test_AT_CellularSMS_set_cscs();

    void test_AT_CellularSMS_delete_all_messages();

    void test_AT_CellularSMS_set_extra_sim_wait_time();
};

#endif // TEST_AT_CELLULARSMS_H

