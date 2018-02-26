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
#ifndef TEST_cellular_mux_DATA_SERVICE_H
#define TEST_cellular_mux_DATA_SERVICE_H

class Test_cellular_mux_data_service
{
public:
    Test_cellular_mux_data_service();

    virtual ~Test_cellular_mux_data_service();

    void test_cellular_mux_write();

    void test_cellular_mux_read();

    void test_cellular_mux_seek();

    void test_cellular_mux_close();

    void test_cellular_mux_sigio();
};

#endif // TEST_cellular_mux_DATA_SERVICE_H

