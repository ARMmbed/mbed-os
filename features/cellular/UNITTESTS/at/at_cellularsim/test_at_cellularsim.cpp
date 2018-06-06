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
#include "test_at_cellularsim.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularSIM.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"
#include "ATHandler_stub.h"

using namespace mbed;
using namespace events;

Test_AT_CellularSIM::Test_AT_CellularSIM()
{
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = 0;
}

Test_AT_CellularSIM::~Test_AT_CellularSIM()
{
}

void Test_AT_CellularSIM::test_AT_CellularSIM_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM *sim = new AT_CellularSIM(at);

    delete sim;
}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin("12"));

    char table2[] = "READY";
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = table2;
    ATHandler_stub::ssize_value = 5;
    CHECK(NSAPI_ERROR_OK == sim.set_pin("12"));

    CHECK(NSAPI_ERROR_OK == sim.set_pin(NULL));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_change_pin()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.change_pin("12", "34"));

    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.change_pin(NULL, "34"));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.change_pin("12", NULL));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.change_pin(NULL, NULL));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin_query()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query("12", true));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query(NULL, true));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query("12", false));
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.set_pin_query(NULL, false));
}

void Test_AT_CellularSIM::test_AT_CellularSIM_get_sim_state()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    CellularSIM::SimState state;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_AUTH_FAILURE;
    ATHandler_stub::ssize_value = -1;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateUnknown == state);

    char table2[] = "READY";
    ATHandler_stub::read_string_value = table2;
    ATHandler_stub::ssize_value = 5;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateReady == state);

    char table3[] = "SIM PIN";
    ATHandler_stub::read_string_value = table3;
    ATHandler_stub::ssize_value = 7;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStatePinNeeded == state);

    char table4[] = "SIM PUK";
    ATHandler_stub::read_string_value = table4;
    ATHandler_stub::ssize_value = 7;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStatePukNeeded == state);

    char table5[] = "SOME CRAP";
    ATHandler_stub::read_string_value = table5;
    ATHandler_stub::ssize_value = 9;
    CHECK(NSAPI_ERROR_AUTH_FAILURE == sim.get_sim_state(state));
    CHECK(CellularSIM::SimStateUnknown == state);
}

void Test_AT_CellularSIM::test_AT_CellularSIM_get_imsi()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    char imsi[16];
    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = "123456789012345";
    ATHandler_stub::ssize_value = 15;
    CHECK(NSAPI_ERROR_OK == sim.get_imsi(imsi));
    CHECK(strcmp(ATHandler_stub::read_string_value, imsi) == 0);

    CHECK(NSAPI_ERROR_PARAMETER == sim.get_imsi(NULL));

    // this would fail as get_imsi should take another param which is the size of the buffer which we could use for validation.
    // Now we have improved documentation that that the given imsi buffer size must be over 15.
    //char imsi2[5];
    //CHECK(NSAPI_ERROR_PARAMETER == sim.get_imsi(imsi2));
}
