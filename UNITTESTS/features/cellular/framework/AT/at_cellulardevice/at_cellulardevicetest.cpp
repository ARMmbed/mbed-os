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
#include "AT_CellularDevice.h"
#include "ATHandler_stub.h"
#include "AT_CellularDevice_stub.h"

using namespace mbed;
using namespace events;

class TestAT_CellularDevice : public testing::Test {
protected:

    void SetUp()
    {
        filehandle_stub_table = NULL;
        filehandle_stub_table_pos = 0;

        ATHandler_stub::read_string_index = kRead_string_table_size;
    }

    void TearDown()
    {
    }
};

TEST_F(TestAT_CellularDevice, Create)
{
    FileHandle_stub fh1;

    AT_CellularDevice dev(&fh1);

    CellularDevice *dev2 = new AT_CellularDevice(&fh1);

    EXPECT_TRUE(dev2 != NULL);
    delete dev2;
    ATHandler *at = dev.get_at_handler();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_at_handler)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler *at = dev.get_at_handler();
    EXPECT_TRUE(at->get_file_handle() == &fh1);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_network)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularNetwork *nw = dev.open_network();
    EXPECT_TRUE(nw);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_sms)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularSMS *sms = dev.open_sms();
    EXPECT_TRUE(sms);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_information)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularInformation *info = dev.open_information();
    EXPECT_TRUE(info);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_network)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    EXPECT_TRUE(dev.open_network());
    dev.close_network();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_sms)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    EXPECT_TRUE(dev.open_sms());
    dev.close_sms();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_information)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::int_value = 0;

    EXPECT_TRUE(dev.open_information());

    ATHandler_stub::fh_value = NULL;
    dev.close_information();

    ATHandler_stub::fh_value = &fh1;

    EXPECT_TRUE(dev.open_information());

    dev.close_information();

    ATHandler_stub::fh_value = NULL;
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_set_timeout)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::timeout = 0;
    ATHandler_stub::default_timeout = false;

    dev.set_timeout(5000);
    EXPECT_TRUE(ATHandler_stub::timeout == 5000);
    EXPECT_TRUE(ATHandler_stub::default_timeout == true);

    EXPECT_TRUE(dev.open_sms());

    dev.set_timeout(5000);
    EXPECT_TRUE(ATHandler_stub::timeout == 5000);
    EXPECT_TRUE(ATHandler_stub::default_timeout == true);

    dev.close_sms();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_modem_debug_on)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::debug_on = false;

    dev.modem_debug_on(true);
    EXPECT_TRUE(ATHandler_stub::debug_on == true);

    EXPECT_TRUE(dev.open_sms());

    dev.modem_debug_on(true);
    EXPECT_TRUE(ATHandler_stub::debug_on == true);

    dev.close_sms();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_init)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    EXPECT_EQ(dev.init(), NSAPI_ERROR_OK);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_shutdown)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    EXPECT_EQ(dev.shutdown(), NSAPI_ERROR_OK);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_is_ready)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularDevice dev(&fh1);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == dev.is_ready());
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_set_power_save_mode)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");

    AT_CellularDevice dev(&fh1);
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(0, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(10, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(912, 0));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(1834, 1834));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(18345, 18345));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(101234, 101234));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(1012345, 1012345));

    EXPECT_TRUE(NSAPI_ERROR_OK == dev.set_power_save_mode(39612345, 39612345));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    EXPECT_TRUE(NSAPI_ERROR_DEVICE_ERROR == dev.set_power_save_mode(0));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_create_delete_context)
{
    FileHandle_stub fh1;
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    ATHandler *at = dev->get_at_handler();

    CellularContext *ctx = dev->create_context(NULL);
    delete dev;

    dev = new AT_CellularDevice(&fh1);
    at = dev->get_at_handler();
    ctx = dev->create_context(NULL);
    CellularContext *ctx1 = dev->create_context();
    CellularContext *ctx2 = dev->create_context();

    EXPECT_TRUE(ctx);
    EXPECT_TRUE(ctx1);
    EXPECT_TRUE(ctx1 != ctx);
    EXPECT_TRUE(ctx1 != ctx2);

    CellularContext *xx = dev->get_context_list();
    EXPECT_TRUE(xx);

    dev->delete_context(ctx);
    dev->delete_context(ctx1);
    dev->delete_context(NULL);
    dev->delete_context(ctx2);

    ctx = dev->create_context(NULL);
    ctx1 = dev->create_context();
    ctx2 = dev->create_context();

    EXPECT_TRUE(ctx);
    EXPECT_TRUE(ctx1);
    EXPECT_TRUE(ctx1 != ctx);
    EXPECT_TRUE(ctx1 != ctx2);

    delete dev;
}

TEST_F(TestAT_CellularDevice, TestAT_CellularDevice_set_pin_verify_debug)
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    ATHandler_stub::ssize_value = 8;
    ATHandler_stub::read_string_value = (char *)"internet";

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::get_debug_clear();
    EXPECT_FALSE(ATHandler_stub::is_get_debug_run());
    ATHandler_stub::debug_call_count_clear();
    at.set_debug(true);
    EXPECT_TRUE(NSAPI_ERROR_OK == dev->set_pin("12"));
    EXPECT_TRUE(ATHandler_stub::is_get_debug_run());
    EXPECT_TRUE(ATHandler_stub::set_debug_call_count_get() == 3);
    EXPECT_TRUE(at.get_debug());

    ATHandler_stub::get_debug_clear();
    EXPECT_FALSE(ATHandler_stub::is_get_debug_run());
    ATHandler_stub::debug_call_count_clear();
    at.set_debug(false);
    EXPECT_TRUE(NSAPI_ERROR_OK == dev->set_pin("11"));
    EXPECT_TRUE(ATHandler_stub::is_get_debug_run());
    EXPECT_TRUE(ATHandler_stub::set_debug_call_count_get() == 3);
    EXPECT_FALSE(at.get_debug());

    delete dev;
}

TEST_F(TestAT_CellularDevice, TestAT_CellularDevice_set_pin)
{
    FileHandle_stub fh1;
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->set_pin("12"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_DEVICE_ERROR;
    ASSERT_EQ(NSAPI_ERROR_DEVICE_ERROR, dev->set_pin("12"));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"SIM PIN";
    ATHandler_stub::ssize_value = 7;
    ASSERT_EQ(NSAPI_ERROR_PARAMETER, dev->set_pin(NULL));

    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::read_string_value = (char *)"READY";
    ATHandler_stub::ssize_value = 5;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->set_pin("12"));

    ASSERT_EQ(NSAPI_ERROR_OK, dev->set_pin(NULL));

    delete dev;
}

TEST_F(TestAT_CellularDevice, TestAT_CellularDevice_get_sim_state)
{
    FileHandle_stub fh1;
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    ATHandler_stub::ssize_value = 8;
    ATHandler_stub::read_string_value = (char *)"internet";

    CellularDevice::SimState state;
    ATHandler_stub::nsapi_error_value = NSAPI_ERROR_OK;
    ATHandler_stub::ssize_value = -1;
    ATHandler_stub::read_string_value = NULL;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->get_sim_state(state));
    ASSERT_EQ(CellularDevice::SimStateUnknown, state);

    ATHandler_stub::read_string_value = (char *)"READY";
    ATHandler_stub::ssize_value = 5;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->get_sim_state(state));
    ASSERT_EQ(CellularDevice::SimStateReady, state);

    ATHandler_stub::read_string_value = (char *)"SIM PIN";
    ATHandler_stub::ssize_value = 7;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->get_sim_state(state));
    ASSERT_EQ(CellularDevice::SimStatePinNeeded, state);

    ATHandler_stub::read_string_value = (char *)"SIM PUK";
    ATHandler_stub::ssize_value = 7;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->get_sim_state(state));
    ASSERT_EQ(CellularDevice::SimStatePukNeeded, state);

    ATHandler_stub::read_string_value = (char *)"SOME CRAP";
    ATHandler_stub::ssize_value = 9;
    ASSERT_EQ(NSAPI_ERROR_OK, dev->get_sim_state(state));
    ASSERT_EQ(CellularDevice::SimStateUnknown, state);

    delete dev;
}

static void device_ready_cb()
{
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_set_ready_cb)
{
    EventQueue que;
    FileHandle_stub fh1;
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    dev->set_ready_cb(&device_ready_cb);
    dev->set_ready_cb(0);

    delete dev;
}
