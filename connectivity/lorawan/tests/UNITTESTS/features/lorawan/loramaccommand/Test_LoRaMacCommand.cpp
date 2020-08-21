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
#include "LoRaMacCommand.h"

#include "LoRaPHY_stub.h"

class my_LoRaPHY : public LoRaPHY {
public:
    my_LoRaPHY()
    {
    };

    virtual ~my_LoRaPHY()
    {
    };
};

uint8_t my_cb()
{
    return 1;
}

class Test_LoRaMacCommand : public testing::Test {
protected:
    LoRaMacCommand *object;

    virtual void SetUp()
    {
        object = new LoRaMacCommand();
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaMacCommand, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaMacCommand, get_mac_cmd_length)
{
    object->add_link_check_req();
    EXPECT_TRUE(object->get_mac_cmd_length() == 1);
    object->clear_command_buffer();
    EXPECT_TRUE(object->get_mac_cmd_length() == 0);
}

TEST_F(Test_LoRaMacCommand, parse_mac_commands_to_repeat)
{
    loramac_mlme_confirm_t mlme;
    lora_mac_system_params_t params;
    my_LoRaPHY phy;
    uint8_t buf[20];

    object->parse_mac_commands_to_repeat();

    buf[0] = 2;
    buf[1] = 16;
    buf[2] = 32;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 3, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 3;
    LoRaPHY_stub::uint8_value = 7;
    LoRaPHY_stub::linkAdrNbBytesParsed = 5;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 4;
    buf[1] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 5;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 6;
    object->set_batterylevel_callback(my_cb);
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 7;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 6, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 8;
    buf[1] = 0;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 9;
    buf[1] = 48;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 10;
    buf[1] = 4;
    buf[2] = 2;
    buf[3] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 4, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    object->parse_mac_commands_to_repeat();
}

TEST_F(Test_LoRaMacCommand, clear_repeat_buffer)
{
    object->clear_repeat_buffer();
}

TEST_F(Test_LoRaMacCommand, copy_repeat_commands_to_buffer)
{
    loramac_mlme_confirm_t mlme;
    lora_mac_system_params_t params;
    my_LoRaPHY phy;
    uint8_t buf[20];

    object->clear_command_buffer();
    buf[0] = 5;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    object->parse_mac_commands_to_repeat();

    object->clear_command_buffer();
    EXPECT_TRUE(object->get_mac_cmd_length() == 0);

    object->copy_repeat_commands_to_buffer();

    EXPECT_TRUE(object->get_mac_cmd_length() != 0);
}

TEST_F(Test_LoRaMacCommand, get_repeat_commands_length)
{
    EXPECT_TRUE(object->get_repeat_commands_length() == 0);
}

TEST_F(Test_LoRaMacCommand, clear_sticky_mac_cmd)
{
    loramac_mlme_confirm_t mlme;
    lora_mac_system_params_t params;
    my_LoRaPHY phy;
    uint8_t buf[20];

    EXPECT_TRUE(object->has_sticky_mac_cmd() == false);

    object->clear_command_buffer();
    buf[0] = 5;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    EXPECT_TRUE(object->has_sticky_mac_cmd() == true);

    object->clear_sticky_mac_cmd();
    EXPECT_TRUE(object->has_sticky_mac_cmd() == false);
}

TEST_F(Test_LoRaMacCommand, has_sticky_mac_cmd)
{
    loramac_mlme_confirm_t mlme;
    lora_mac_system_params_t params;
    my_LoRaPHY phy;
    uint8_t buf[20];

    EXPECT_TRUE(object->has_sticky_mac_cmd() == false);

    object->clear_command_buffer();
    buf[0] = 5;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    EXPECT_TRUE(object->has_sticky_mac_cmd() == true);
}

TEST_F(Test_LoRaMacCommand, process_mac_commands)
{
    loramac_mlme_confirm_t mlme;
    lora_mac_system_params_t params;
    my_LoRaPHY phy;
    uint8_t buf[20];
    EXPECT_TRUE(object->process_mac_commands(NULL, 0, 0, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 2;
    buf[1] = 16;
    buf[2] = 32;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 3, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    buf[0] = 3;
    LoRaPHY_stub::uint8_value = 7;
    LoRaPHY_stub::linkAdrNbBytesParsed = 5;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_link_adr_ans function here
    object->clear_command_buffer();
    buf[0] = 3;
    for (int i = 0; i < 64; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 4;
    buf[1] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_duty_cycle_ans()
    object->clear_command_buffer();
    for (int i = 0; i < 128; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 5;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_rx_param_setup_ans
    object->clear_command_buffer();
    LoRaPHY_stub::uint8_value = 7;
    for (int i = 0; i < 64; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 5, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 6;
    object->set_batterylevel_callback(my_cb);
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //overflow add_dev_status_ans
    object->clear_command_buffer();
    for (int i = 0; i < 42; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 7;
    buf[1] = 2;
    buf[2] = 2;
    buf[3] = 2;
    buf[4] = 2;
    buf[5] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 6, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_new_channel_ans
    object->clear_command_buffer();
    LoRaPHY_stub::uint8_value = 7;
    for (int i = 0; i < 64; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 6, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 6, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 8;
    buf[1] = 0;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_rx_timing_setup_ans
    object->clear_command_buffer();
    LoRaPHY_stub::uint8_value = 7;
    for (int i = 0; i < 128; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 9;
    buf[1] = 48;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_tx_param_setup_ans
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    object->clear_command_buffer();
    LoRaPHY_stub::uint8_value = 7;
    for (int i = 0; i < 128; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
        LoRaPHY_stub::bool_counter = 0;
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 2, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 10;
    buf[1] = 4;
    buf[2] = 2;
    buf[3] = 2;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 4, 0, mlme, params, phy) == LORAWAN_STATUS_OK);

    //Overflow add_dl_channel_ans
    object->clear_command_buffer();
    for (int i = 0; i < 64; i++) {
        EXPECT_TRUE(object->process_mac_commands(buf, 0, 4, 0, mlme, params, phy) == LORAWAN_STATUS_OK);
    }
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 4, 0, mlme, params, phy) == LORAWAN_STATUS_LENGTH_ERROR);

    object->clear_command_buffer();
    buf[0] = 80;
    EXPECT_TRUE(object->process_mac_commands(buf, 0, 1, 0, mlme, params, phy) == LORAWAN_STATUS_UNSUPPORTED);
}

TEST_F(Test_LoRaMacCommand, add_link_check_req)
{
    object->add_link_check_req();
    EXPECT_TRUE(object->get_mac_commands_buffer()[0] == 2);
    EXPECT_TRUE(object->get_mac_cmd_length() == 1);
    object->clear_command_buffer();
    EXPECT_TRUE(object->get_mac_cmd_length() == 0);
}

TEST_F(Test_LoRaMacCommand, set_batterylevel_callback)
{
    object->set_batterylevel_callback(my_cb);
}

