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
#include "CppUTest/TestHarness.h"
#include "test_cellular_mux_data_service.h"
#include "cellular_mux.h"
#include "cellular_mux_stub.h"
#include <string.h>

using namespace mbed;

Test_cellular_mux_data_service::Test_cellular_mux_data_service()
{

}

Test_cellular_mux_data_service::~Test_cellular_mux_data_service()
{
}

void Test_cellular_mux_data_service::test_cellular_mux_write()
{
    MuxDataService mux;
    char table[] = "ss";
    cellular_mux_stub::size_value = 6;
    CHECK(6 == mux.write(table, 2));
}

void Test_cellular_mux_data_service::test_cellular_mux_read()
{
    MuxDataService mux;
    char table[6];

    cellular_mux_stub::size_value = 67;
    CHECK(67 == mux.read(table, 6));
}

void Test_cellular_mux_data_service::test_cellular_mux_seek()
{
    MuxDataService mux;
    CHECK(0 == mux.seek(0));
}

void Test_cellular_mux_data_service::test_cellular_mux_close()
{
    MuxDataService mux;
    CHECK(0 == mux.close());
}

void cb()
{

}

void Test_cellular_mux_data_service::test_cellular_mux_sigio()
{
    MuxDataService mux;
    mux.sigio(cb);
}

