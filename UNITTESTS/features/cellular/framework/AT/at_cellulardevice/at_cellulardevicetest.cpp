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
#include "AT_CellularDevice.h"
#include "ATHandler_stub.h"
#include "AT_CellularBase_stub.h"
#include <string.h>

using namespace mbed;
using namespace events;

class TestAT_CellularDevice : public testing::Test {
protected:

    void SetUp() {
    }

    void TearDown() {
    }
};

TEST_F(TestAT_CellularDevice, Create)
{
    EventQueue que;
    AT_CellularDevice dev(que);

    CellularDevice *dev2 = new AT_CellularDevice(que);

    EXPECT_TRUE(dev2 != NULL);
    delete dev2;
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_at_handler)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    FileHandle_stub fh2;
    FileHandle_stub fh3;

    EXPECT_TRUE(dev.open_network(&fh1));
    EXPECT_TRUE(dev.open_sms(&fh2));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(dev.open_sim(&fh3));
    ATHandler_stub::fh_value = &fh1;
    EXPECT_TRUE(dev.open_power(&fh1));

    ATHandler_stub::fh_value = NULL;
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_network)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    EXPECT_TRUE(!dev.open_network(NULL));
    EXPECT_TRUE(dev.open_network(&fh1));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_sms)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    EXPECT_TRUE(!dev.open_sms(NULL));
    EXPECT_TRUE(dev.open_sms(&fh1));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_power)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    EXPECT_TRUE(!dev.open_power(NULL));
    EXPECT_TRUE(dev.open_power(&fh1));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_sim)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    EXPECT_TRUE(! dev.open_sim(NULL));
    EXPECT_TRUE(dev.open_sim(&fh1));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_information)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    EXPECT_TRUE(!dev.open_information(NULL));
    EXPECT_TRUE(dev.open_information(&fh1));
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_network)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_network(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_network();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_sms)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_sms(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_sms();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_power)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_power(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_power();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_sim)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::ref_count = 0;
    int ana = 0;

    EXPECT_TRUE(dev.open_sim(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;

    ana = ATHandler_stub::ref_count;

    dev.close_sms(); // this should not affect to refcount as it's not opened
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);
    ana = ATHandler_stub::ref_count;

    dev.close_sim();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_information)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::int_value = 0;

    EXPECT_TRUE(dev.open_information(&fh1));

    ATHandler_stub::fh_value = NULL;
    AT_CellularBase_stub::handler_value = NULL;
    dev.close_information();

    ATHandler_stub::fh_value = &fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularBase_stub::handler_value = &at;

    EXPECT_TRUE(dev.open_information(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;

    dev.close_information();
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    ATHandler_stub::fh_value = NULL;
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_set_timeout)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::timeout = 0;
    ATHandler_stub::default_timeout = false;

    // no interfaces open so settings timeout should not change anything
    dev.set_timeout(5000);
    EXPECT_TRUE(ATHandler_stub::timeout == 0);
    EXPECT_TRUE(ATHandler_stub::default_timeout == false);

    EXPECT_TRUE(dev.open_sim(&fh1));
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.set_timeout(5000);
    EXPECT_TRUE(ATHandler_stub::timeout == 5000);
    EXPECT_TRUE(ATHandler_stub::default_timeout == true);

    dev.close_sim();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_modem_debug_on)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::debug_on = false;

    // no interfaces open so debug toggling should not affect
    dev.modem_debug_on(true);
    EXPECT_TRUE(ATHandler_stub::debug_on == false);

    EXPECT_TRUE(dev.open_sim(&fh1));
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.modem_debug_on(true);
    EXPECT_TRUE(ATHandler_stub::debug_on == true);

    dev.close_sim();
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_stack)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    NetworkStack *stack = dev.get_stack();
    EXPECT_TRUE(stack == NULL);

    EXPECT_TRUE(dev.open_network(&fh1));

    stack = dev.get_stack();
    EXPECT_TRUE(stack == NULL); // Not in PPP so also null but this is got from the network class
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_send_delay)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    EXPECT_TRUE(0 == dev.get_send_delay());
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_init_module)
{
    EventQueue que;
    AT_CellularDevice dev(que);
    EXPECT_TRUE(NSAPI_ERROR_OK == dev.init_module(NULL));
}
