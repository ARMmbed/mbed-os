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
#include "gtest/gtest.h"
#include "ATHandler_stub.h"
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

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestAT_CellularSIM : public testing::Test {
protected:

    void SetUp()
    {
        ATHandler_stub::read_string_index = kRead_string_table_size;
        ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
        ATHandler_stub::read_string_value = NULL;
        ATHandler_stub::ssize_value = 0;
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

TEST_F(TestAT_CellularSIM, Create)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM *sim = new AT_CellularSIM(at);

    EXPECT_TRUE(sim != NULL);
    delete sim;
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_set_pin)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin("12"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.set_pin("12"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"READY";
    ATHandler_stub::ssize_value = 5;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin("12"));

    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin(NULL));
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_change_pin)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.change_pin("12", "34"));
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.change_pin(NULL, "34"));
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.change_pin("12", NULL));
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.change_pin(NULL, NULL));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.change_pin("12", "34"));
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_set_pin_query)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin_query("12", true));
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin_query(NULL, true));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin_query("12", false));
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.set_pin_query(NULL, false));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.set_pin_query("12", false));
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.set_pin_query("12", true));
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_get_sim_state)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularSIM sim(at);
    CellularSIM::SimState state;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::ssize_value = -1;
    ATHandler_stub::read_string_value = NULL;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_sim_state(state));
    EXPECT_TRUE(CellularSIM::SimStateUnknown == state);

    ATHandler_stub::read_string_value = (char *)"READY";
    ATHandler_stub::ssize_value = 5;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_sim_state(state));
    EXPECT_TRUE(CellularSIM::SimStateReady == state);

    ATHandler_stub::read_string_value = (char *)"SIM PIN";
    ATHandler_stub::ssize_value = 7;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_sim_state(state));
    EXPECT_TRUE(CellularSIM::SimStatePinNeeded == state);

    ATHandler_stub::read_string_value = (char *)"SIM PUK";
    ATHandler_stub::ssize_value = 7;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_sim_state(state));
    EXPECT_TRUE(CellularSIM::SimStatePukNeeded == state);

    ATHandler_stub::read_string_value = (char *)"SOME CRAP";
    ATHandler_stub::ssize_value = 9;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_sim_state(state));
    EXPECT_TRUE(CellularSIM::SimStateUnknown == state);
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_get_imsi)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    char imsi[16];
    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"123456789012345";
    ATHandler_stub::ssize_value = 15;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_imsi(imsi));
    EXPECT_TRUE(strcmp(ATHandler_stub::read_string_value, imsi) == 0);

    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = -1;
    ATHandler_stub::read_string_index = -1;
    imsi[0] = 0;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.get_imsi(imsi));
    EXPECT_TRUE(strlen(imsi) == 0);

    EXPECT_TRUE(NSAPI_ERROR_PARAMETER == sim.get_imsi(NULL));

    // this would fail as get_imsi should take another param which is the size of the buffer which we could use for validation.
    // Now we have improved documentation that that the given imsi buffer size must be over 15.
    //char imsi2[5];
    //EXPECT_TRUE(NSAPI_ERROR_PARAMETER == sim.get_imsi(imsi2));
}

TEST_F(TestAT_CellularSIM, test_AT_CellularSIM_get_iccid)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    char buf[16];
    AT_CellularSIM sim(at);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"123456789012345";
    ATHandler_stub::ssize_value = 15;
    EXPECT_TRUE(NSAPI_ERROR_OK == sim.get_iccid(buf, 16));
    EXPECT_TRUE(strcmp(ATHandler_stub::read_string_value, buf) == 0);

    buf[0] = 0;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ATHandler_stub::read_string_value = NULL;
    ATHandler_stub::ssize_value = -1;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == sim.get_iccid(buf, 16));
    EXPECT_TRUE(strlen(buf) == 0);
}
