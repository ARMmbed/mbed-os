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
#ifndef TEST_AT_CELLULARBASE_H
#define TEST_AT_CELLULARBASE_H

class Test_AT_CellularBase
{
public:
    Test_AT_CellularBase();

    virtual ~Test_AT_CellularBase();

    void test_AT_CellularBase_get_at_handler();

    void test_AT_CellularBase_get_device_error();
};

#endif // TEST_AT_CELLULARBASE_H

