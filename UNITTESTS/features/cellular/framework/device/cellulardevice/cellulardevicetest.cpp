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
#include <string.h>

#include "CellularDevice.h"
#include "FileHandle_stub.h"
#include "myCellularDevice.h"
#include "CellularStateMachine_stub.h"

using namespace mbed;

// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestCellularDevice : public testing::Test {
protected:

    void SetUp()
    {
        CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
        CellularStateMachine_stub::get_current_target_state = STATE_INIT;
        CellularStateMachine_stub::get_current_current_state = STATE_INIT;
        CellularStateMachine_stub::bool_value = false;
    }

    void TearDown()
    {
    }
};

// *INDENT-ON*
TEST_F(TestCellularDevice, test_create_delete)
{
    FileHandle_stub fh1;

    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);
    delete dev;
    dev = NULL;

    CellularDevice *dev1 = CellularDevice::get_default_instance();
    EXPECT_FALSE(dev1);
}

TEST_F(TestCellularDevice, test_set_sim_pin)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    // MAX_PIN_SIZE = 8; so let's try a longer one
    dev->set_sim_pin("123480375462074360276407");
    dev->set_sim_pin(NULL);
    dev->set_sim_pin("1234");
    delete dev;
}

TEST_F(TestCellularDevice, test_set_plmn)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);


    // MAX_PLMN_SIZE = 16; so let's try a longer one
    dev->set_plmn("123480327465023746502734602736073264076340");
    dev->set_plmn("1234");
    dev->set_plmn(NULL);
    delete dev;
}

TEST_F(TestCellularDevice, test_set_device_ready)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_NO_MEMORY;
    ASSERT_EQ(dev->set_device_ready(), NSAPI_ERROR_NO_MEMORY);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->set_device_ready(), NSAPI_ERROR_OK);

    CellularStateMachine_stub::get_current_current_state = STATE_SIM_PIN;
    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->set_device_ready(), NSAPI_ERROR_ALREADY);

    CellularStateMachine_stub::bool_value = true;
    CellularStateMachine_stub::get_current_target_state = STATE_SIM_PIN;
    CellularStateMachine_stub::get_current_current_state = STATE_POWER_ON;
    ASSERT_EQ(dev->set_device_ready(), NSAPI_ERROR_IN_PROGRESS);
    delete dev;
}

TEST_F(TestCellularDevice, test_set_sim_ready)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_NO_MEMORY;
    ASSERT_EQ(dev->set_sim_ready(), NSAPI_ERROR_NO_MEMORY);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->set_sim_ready(), NSAPI_ERROR_OK);

    CellularStateMachine_stub::get_current_current_state = STATE_REGISTERING_NETWORK;
    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->set_sim_ready(), NSAPI_ERROR_ALREADY);

    CellularStateMachine_stub::bool_value = true;
    CellularStateMachine_stub::get_current_target_state = STATE_REGISTERING_NETWORK;
    CellularStateMachine_stub::get_current_current_state = STATE_POWER_ON;
    ASSERT_EQ(dev->set_sim_ready(), NSAPI_ERROR_IN_PROGRESS);
    delete dev;
}

TEST_F(TestCellularDevice, test_register_to_network)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_NO_MEMORY;
    ASSERT_EQ(dev->register_to_network(), NSAPI_ERROR_NO_MEMORY);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->register_to_network(), NSAPI_ERROR_OK);

    CellularStateMachine_stub::get_current_current_state = STATE_ATTACHING_NETWORK;
    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->register_to_network(), NSAPI_ERROR_ALREADY);

    CellularStateMachine_stub::bool_value = true;
    CellularStateMachine_stub::get_current_target_state = STATE_ATTACHING_NETWORK;
    CellularStateMachine_stub::get_current_current_state = STATE_POWER_ON;
    ASSERT_EQ(dev->register_to_network(), NSAPI_ERROR_IN_PROGRESS);
    delete dev;
}

TEST_F(TestCellularDevice, test_attach_to_network)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_NO_MEMORY;
    ASSERT_EQ(dev->attach_to_network(), NSAPI_ERROR_NO_MEMORY);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->attach_to_network(), NSAPI_ERROR_OK);

    CellularStateMachine_stub::get_current_current_state = STATE_ATTACHING_NETWORK;
    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->attach_to_network(), NSAPI_ERROR_ALREADY);

    CellularStateMachine_stub::bool_value = true;
    CellularStateMachine_stub::get_current_target_state = STATE_ATTACHING_NETWORK;
    CellularStateMachine_stub::get_current_current_state = STATE_POWER_ON;
    ASSERT_EQ(dev->attach_to_network(), NSAPI_ERROR_IN_PROGRESS);
    delete dev;
}

TEST_F(TestCellularDevice, test_get_context_list)
{
    FileHandle_stub fh1;

    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);
    CellularContext *ctx = dev->create_context();
    EXPECT_TRUE(dev->get_context_list());
    delete dev;

    dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);
    EXPECT_FALSE(dev->get_context_list());
    delete dev;
}

TEST_F(TestCellularDevice, test_cellular_callback)
{
    FileHandle_stub fh1;
    myCellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    ASSERT_EQ(dev->attach_to_network(), NSAPI_ERROR_OK);

    cell_callback_data_t data;
    dev->cellular_callback((nsapi_event_t)CellularRegistrationStatusChanged, (intptr_t)&data);

    data.error = NSAPI_ERROR_OK;
    dev->set_plmn("1234");
    dev->cellular_callback((nsapi_event_t)CellularDeviceReady, (intptr_t)&data);

    dev->set_sim_pin("1234");
    data.status_data = CellularDevice::SimStatePinNeeded;
    dev->cellular_callback((nsapi_event_t)CellularSIMStatusChanged, (intptr_t)&data);

    CellularContext *ctx = dev->create_context();
    dev->cellular_callback(NSAPI_EVENT_CONNECTION_STATUS_CHANGE, NSAPI_STATUS_DISCONNECTED);

    delete dev;
}

TEST_F(TestCellularDevice, test_shutdown)
{
    FileHandle_stub fh1;
    CellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(dev->shutdown(), NSAPI_ERROR_OK);

    delete dev;
}

TEST_F(TestCellularDevice, test_timeout_array)
{
    FileHandle_stub fh1;
    myCellularDevice *dev = new myCellularDevice(&fh1);
    EXPECT_TRUE(dev);

    CellularStateMachine_stub::nsapi_error_value = NSAPI_ERROR_OK;

    // Max size
    uint16_t set_timeouts[CELLULAR_RETRY_ARRAY_SIZE + 1];
    for (int i = 0; i < CELLULAR_RETRY_ARRAY_SIZE; i++) {
        set_timeouts[i] = i + 100;
    }
    dev->set_retry_timeout_array(set_timeouts, CELLULAR_RETRY_ARRAY_SIZE);

    uint16_t verify_timeouts[CELLULAR_RETRY_ARRAY_SIZE + 1];
    for (int i = 0; i < CELLULAR_RETRY_ARRAY_SIZE; i++) {
        verify_timeouts[i] = i + 100;
    }
    dev->verify_timeout_array(verify_timeouts, CELLULAR_RETRY_ARRAY_SIZE);

    // Empty
    dev->set_retry_timeout_array(NULL, 0);
    dev->verify_timeout_array(NULL, 0);

    // Oversize (returns only CELLULAR_RETRY_ARRAY_SIZE)
    dev->set_retry_timeout_array(set_timeouts, CELLULAR_RETRY_ARRAY_SIZE + 1);
    dev->verify_timeout_array(verify_timeouts, CELLULAR_RETRY_ARRAY_SIZE);

    delete dev;
}
