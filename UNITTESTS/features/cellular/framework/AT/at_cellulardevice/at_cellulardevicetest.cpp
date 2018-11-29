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
#include "AT_CellularBase_stub.h"

using namespace mbed;
using namespace events;

class TestAT_CellularDevice : public testing::Test {
protected:

    void SetUp()
    {
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
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_at_handler)
{
    FileHandle_stub fh1;
    FileHandle_stub fh2;
    FileHandle_stub fh3;
    AT_CellularDevice dev(&fh1);

    EXPECT_TRUE(dev.open_network(&fh1));
    EXPECT_TRUE(dev.open_sms(&fh2));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(dev.open_sim(&fh3));
    ATHandler_stub::fh_value = &fh1;
    EXPECT_TRUE(dev.open_power(&fh1));

    ATHandler_stub::fh_value = NULL;

    AT_CellularDevice *dev2 = new AT_CellularDevice(&fh1);
    EXPECT_TRUE(dev2->open_information(&fh1));
    ATHandler *at = dev2->get_at_handler();
    EXPECT_TRUE(at->get_ref_count() == 4);
    delete dev2;
    EXPECT_TRUE(at->get_ref_count() == 3);
    AT_CellularDevice dev3(&fh1);
    EXPECT_TRUE(dev3.release_at_handler(at) == NSAPI_ERROR_OK);
    EXPECT_TRUE(ATHandler_stub::ref_count == 2);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_network)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularNetwork *nw = dev.open_network(NULL);
    CellularNetwork *nw1 = dev.open_network(&fh1);

    EXPECT_TRUE(nw);
    EXPECT_TRUE(nw1);
    EXPECT_TRUE(nw1 == nw);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_sms)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularSMS *sms = dev.open_sms(NULL);
    CellularSMS *sms1 = dev.open_sms(&fh1);

    EXPECT_TRUE(sms);
    EXPECT_TRUE(sms1);
    EXPECT_TRUE(sms1 == sms);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_power)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularPower *pwr = dev.open_power(NULL);
    CellularPower *pwr1 = dev.open_power(&fh1);

    EXPECT_TRUE(pwr);
    EXPECT_TRUE(pwr1);
    EXPECT_TRUE(pwr1 == pwr);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_sim)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularSIM *sim =  dev.open_sim(NULL);
    CellularSIM *sim1 =  dev.open_sim(&fh1);

    EXPECT_TRUE(sim);
    EXPECT_TRUE(sim1);
    EXPECT_TRUE(sim1 == sim);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_open_information)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);

    CellularInformation *info = dev.open_information(NULL);
    CellularInformation *info1 = dev.open_information(&fh1);

    EXPECT_TRUE(info);
    EXPECT_TRUE(info1);
    EXPECT_TRUE(info1 == info);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_network)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_network(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_network();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_sms)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_sms(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_sms();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_power)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::ref_count = 0;

    EXPECT_TRUE(dev.open_power(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    EXPECT_TRUE(ATHandler_stub::ref_count == 1);

    dev.close_power();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_close_sim)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
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
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    ATHandler_stub::int_value = 0;

    EXPECT_TRUE(dev.open_information(&fh1));

    ATHandler_stub::fh_value = NULL;
    AT_CellularBase_stub::handler_value = NULL;
    dev.close_information();

    EventQueue que;
    ATHandler_stub::fh_value = &fh1;
    ATHandler at(&fh1, que, 0, ",");
    AT_CellularBase_stub::handler_value = &at;

    EXPECT_TRUE(dev.open_information(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;

    dev.close_information();
    EXPECT_TRUE(ATHandler_stub::ref_count == kATHandler_destructor_ref_ount);

    ATHandler_stub::fh_value = NULL;
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_set_timeout)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
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
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
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

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_get_send_delay)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    EXPECT_TRUE(0 == dev.get_send_delay());
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_init_module)
{
    FileHandle_stub fh1;
    AT_CellularDevice dev(&fh1);
    EXPECT_TRUE(NSAPI_ERROR_OK == dev.init_module());
}

TEST_F(TestAT_CellularDevice, test_AT_CellularDevice_create_delete_context)
{
    FileHandle_stub fh1;
    AT_CellularDevice *dev = new AT_CellularDevice(&fh1);

    ATHandler *at = dev->get_at_handler();
    EXPECT_TRUE(at->get_ref_count() == 1);
    EXPECT_TRUE(dev->release_at_handler(at) == NSAPI_ERROR_OK);

    CellularContext *ctx = dev->create_context(NULL);
    delete dev;

    dev = new AT_CellularDevice(&fh1);
    at = dev->get_at_handler();
    EXPECT_TRUE(at->get_ref_count() == 1);
    ctx = dev->create_context(NULL);
    CellularContext *ctx1 = dev->create_context(&fh1);
    EXPECT_TRUE(at->get_ref_count() == 3);
    CellularContext *ctx2 = dev->create_context(&fh1);
    EXPECT_TRUE(at->get_ref_count() == 4);

    EXPECT_TRUE(ctx);
    EXPECT_TRUE(ctx1);
    EXPECT_TRUE(ctx1 != ctx);
    EXPECT_TRUE(ctx1 != ctx2);

    CellularContext *xx = dev->get_context_list();
    EXPECT_TRUE(xx);

    dev->delete_context(ctx);
    EXPECT_TRUE(at->get_ref_count() == 3);
    dev->delete_context(ctx1);
    EXPECT_TRUE(at->get_ref_count() == 2);
    dev->delete_context(NULL);
    EXPECT_TRUE(at->get_ref_count() == 2);
    dev->delete_context(ctx2);
    EXPECT_TRUE(at->get_ref_count() == 1);

    ctx = dev->create_context(NULL);
    EXPECT_TRUE(at->get_ref_count() == 2);
    ctx1 = dev->create_context(&fh1);
    EXPECT_TRUE(at->get_ref_count() == 3);
    ctx2 = dev->create_context(&fh1);
    EXPECT_TRUE(at->get_ref_count() == 4);
    EXPECT_TRUE(dev->release_at_handler(at) == NSAPI_ERROR_OK);
    EXPECT_TRUE(ctx);
    EXPECT_TRUE(ctx1);
    EXPECT_TRUE(ctx1 != ctx);
    EXPECT_TRUE(ctx1 != ctx2);

    delete dev;
}
