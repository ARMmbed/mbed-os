/*
 * Copyright (c) 2018, Arm Limited and affiliates
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
#include "LoRaMacChannelPlan.h"
#include "LoRaPHY_stub.h"
#include "LoRaPHY.h"

class my_LoRaPHY : public LoRaPHY {
public:
    my_LoRaPHY()
    {
    };

    virtual ~my_LoRaPHY()
    {
    };
};


class Test_LoRaMacChannelPlan : public testing::Test {
protected:
    LoRaMacChannelPlan *object;
    my_LoRaPHY phy;

    virtual void SetUp()
    {
        object = new LoRaMacChannelPlan();
        object->activate_channelplan_subsystem(&phy);

        LoRaPHY_stub::uint8_value = 0;
        LoRaPHY_stub::bool_counter = 0;
        LoRaPHY_stub::lorawan_status_value = LORAWAN_STATUS_OK;
        LoRaPHY_stub::uint16_value = 0;
        memcpy(LoRaPHY_stub::bool_table, "0", 20);
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaMacChannelPlan, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaMacChannelPlan, set_plan)
{
    lorawan_channelplan_t plan;
    memset(&plan, 0, sizeof(plan));
    memset(&LoRaPHY_stub::bool_table, 0, sizeof(LoRaPHY_stub::bool_table));
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = false;
    EXPECT_TRUE(object->set_plan(plan) == LORAWAN_STATUS_SERVICE_UNKNOWN);

    plan.nb_channels = 1;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::uint8_value = 0;
    EXPECT_TRUE(object->set_plan(plan) == LORAWAN_STATUS_PARAMETER_INVALID);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::uint8_value = 10;
    LoRaPHY_stub::lorawan_status_value = LORAWAN_STATUS_PARAMETER_INVALID;
    loramac_channel_t chan;
    memset(&chan, 0, sizeof(chan));
    plan.channels = &chan;
    EXPECT_TRUE(object->set_plan(plan) == LORAWAN_STATUS_PARAMETER_INVALID);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    plan.nb_channels = 2;
    LoRaPHY_stub::lorawan_status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(object->set_plan(plan) == LORAWAN_STATUS_OK);
}

TEST_F(Test_LoRaMacChannelPlan, get_plan)
{
    lorawan_channelplan_t plan;
    channel_params_t params;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = false;
    EXPECT_TRUE(object->get_plan(plan, &params) == LORAWAN_STATUS_SERVICE_UNKNOWN);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = false;

    LoRaPHY_stub::uint8_value = 1;
    LoRaPHY_stub::uint16_value = 0xABCD;
    EXPECT_TRUE(object->get_plan(plan, &params) == LORAWAN_STATUS_OK);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = true;
    LoRaPHY_stub::bool_table[2] = false;
    loramac_channel_t ch;
    plan.channels = &ch;
    EXPECT_TRUE(object->get_plan(plan, &params) == LORAWAN_STATUS_OK);
}

TEST_F(Test_LoRaMacChannelPlan, remove_plan)
{
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = false;
    EXPECT_TRUE(object->remove_plan() == LORAWAN_STATUS_SERVICE_UNKNOWN);

    LoRaPHY_stub::uint8_value = 4;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = true; //first continue
    LoRaPHY_stub::bool_table[2] = false;
    LoRaPHY_stub::bool_table[3] = false;//second continue
    LoRaPHY_stub::bool_table[4] = false;
    LoRaPHY_stub::bool_table[5] = true;
    LoRaPHY_stub::bool_table[6] = false;//false for remove_single_channel(i)

    EXPECT_TRUE(object->remove_plan() == LORAWAN_STATUS_SERVICE_UNKNOWN);

    LoRaPHY_stub::uint8_value = 3;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = false;
    LoRaPHY_stub::bool_table[2] = true;
    LoRaPHY_stub::bool_table[3] = true;
    LoRaPHY_stub::bool_table[4] = true;
    LoRaPHY_stub::bool_table[5] = true;
    LoRaPHY_stub::bool_table[7] = true;
    LoRaPHY_stub::bool_table[8] = true;
    LoRaPHY_stub::bool_table[9] = true;
    LoRaPHY_stub::bool_table[10] = true;

    EXPECT_TRUE(object->remove_plan() == LORAWAN_STATUS_OK);

}

TEST_F(Test_LoRaMacChannelPlan, remove_single_channel)
{
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = false;
    EXPECT_TRUE(object->remove_single_channel(4) == LORAWAN_STATUS_SERVICE_UNKNOWN);

    LoRaPHY_stub::uint8_value = 2;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;

    EXPECT_TRUE(object->remove_single_channel(4) == LORAWAN_STATUS_PARAMETER_INVALID);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = false;
    EXPECT_TRUE(object->remove_single_channel(1) == LORAWAN_STATUS_PARAMETER_INVALID);

    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = true;
    EXPECT_TRUE(object->remove_single_channel(1) == LORAWAN_STATUS_OK);
}

