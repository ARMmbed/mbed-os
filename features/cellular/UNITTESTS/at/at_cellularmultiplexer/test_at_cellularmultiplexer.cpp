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
#include "test_at_cellularmultiplexer.h"
#include <string.h>
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"
#include "AT_CellularMultiplexer.h"
#include "FileHandle_stub.h"

using namespace mbed;
using namespace events;

Test_AT_CellularMultiplexer::Test_AT_CellularMultiplexer()
{

}

Test_AT_CellularMultiplexer::~Test_AT_CellularMultiplexer()
{
}

void Test_AT_CellularMultiplexer::test_AT_CellularMultiplexer_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularMultiplexer *cm = new AT_CellularMultiplexer(at);
    delete cm;
}

void Test_AT_CellularMultiplexer::test_AT_CellularMultiplexer_multiplexer_mode_start()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularMultiplexer cm(at);
    CHECK(!cm.multiplexer_mode_start());
}

