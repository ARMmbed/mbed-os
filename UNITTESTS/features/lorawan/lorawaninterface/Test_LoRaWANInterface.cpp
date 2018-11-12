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
#include "LoRaWANInterface.h"

class my_radio : public LoRaRadio {
public:

    virtual void init_radio(radio_events_t *events)
    {
    };

    virtual void radio_reset()
    {
    };

    virtual void sleep(void)
    {
    };

    virtual void standby(void)
    {
    };

    virtual void set_rx_config(radio_modems_t modem, uint32_t bandwidth,
                               uint32_t datarate, uint8_t coderate,
                               uint32_t bandwidth_afc, uint16_t preamble_len,
                               uint16_t symb_timeout, bool fix_len,
                               uint8_t payload_len,
                               bool crc_on, bool freq_hop_on, uint8_t hop_period,
                               bool iq_inverted, bool rx_continuous)
    {

    };

    virtual void set_tx_config(radio_modems_t modem, int8_t power, uint32_t fdev,
                               uint32_t bandwidth, uint32_t datarate,
                               uint8_t coderate, uint16_t preamble_len,
                               bool fix_len, bool crc_on, bool freq_hop_on,
                               uint8_t hop_period, bool iq_inverted, uint32_t timeout)
    {
    };

    virtual void send(uint8_t *buffer, uint8_t size)
    {
    };

    virtual void receive(void)
    {
    };

    virtual void set_channel(uint32_t freq)
    {
    };

    virtual uint32_t random(void)
    {
    };

    virtual uint8_t get_status(void)
    {
    };

    virtual void set_max_payload_length(radio_modems_t modem, uint8_t max)
    {
    };

    virtual void set_public_network(bool enable)
    {
    };

    virtual uint32_t time_on_air(radio_modems_t modem, uint8_t pkt_len)
    {
    };

    virtual bool perform_carrier_sense(radio_modems_t modem,
                                       uint32_t freq,
                                       int16_t rssi_threshold,
                                       uint32_t max_carrier_sense_time)
    {
    };

    virtual void start_cad(void)
    {
    };

    virtual bool check_rf_frequency(uint32_t frequency)
    {
    };

    virtual void set_tx_continuous_wave(uint32_t freq, int8_t power, uint16_t time)
    {
    };

    virtual void lock(void)
    {
    };

    virtual void unlock(void)
    {
    };
};

class my_LoRaPHY : public LoRaPHY {
public:
    my_LoRaPHY()
    {
    };

    virtual ~my_LoRaPHY()
    {
    };
};

class Test_LoRaWANInterface : public testing::Test {
protected:
    LoRaWANInterface *object;
    my_radio radio;

    virtual void SetUp()
    {
        object = new LoRaWANInterface(radio);
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaWANInterface, constructor)
{
    EXPECT_TRUE(object);

    my_radio radio;
    my_LoRaPHY phy;
    LoRaWANInterface object(radio, phy);
}

TEST_F(Test_LoRaWANInterface, initialize)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize(NULL));
}

TEST_F(Test_LoRaWANInterface, connect)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect());

    lorawan_connect_t conn;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect(conn));
}

TEST_F(Test_LoRaWANInterface, disconnect)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->disconnect());
}

TEST_F(Test_LoRaWANInterface, add_link_check_request)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->add_link_check_request());
}

TEST_F(Test_LoRaWANInterface, remove_link_check_request)
{
    object->remove_link_check_request();
}

TEST_F(Test_LoRaWANInterface, set_datarate)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_datarate(1));
}

TEST_F(Test_LoRaWANInterface, enable_adaptive_datarate)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->enable_adaptive_datarate());
}

TEST_F(Test_LoRaWANInterface, disable_adaptive_datarate)
{
    object->disable_adaptive_datarate();
}

TEST_F(Test_LoRaWANInterface, set_confirmed_msg_retries)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_confirmed_msg_retries(1));
}

TEST_F(Test_LoRaWANInterface, set_channel_plan)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_channel_plan(plan));
}

TEST_F(Test_LoRaWANInterface, get_channel_plan)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->get_channel_plan(plan));
}

TEST_F(Test_LoRaWANInterface, remove_channel_plan)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->remove_channel_plan());
}

TEST_F(Test_LoRaWANInterface, remove_channel)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->remove_channel(1));
}

TEST_F(Test_LoRaWANInterface, send)
{
    EXPECT_TRUE(0 == object->send(1, NULL, 0, 0));
}

TEST_F(Test_LoRaWANInterface, receive)
{
    EXPECT_TRUE(0 == object->receive(1, NULL, 0, 0));

    uint8_t port;
    int flags;
    EXPECT_TRUE(0 == object->receive(NULL, 0, port, flags));
}

TEST_F(Test_LoRaWANInterface, add_app_callbacks)
{
    lorawan_app_callbacks_t cbs;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->add_app_callbacks(&cbs));
}

TEST_F(Test_LoRaWANInterface, set_device_class)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_device_class(CLASS_A));
}

TEST_F(Test_LoRaWANInterface, get_tx_metadata)
{
    lorawan_tx_metadata data;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->get_tx_metadata(data));
}

TEST_F(Test_LoRaWANInterface, get_rx_metadata)
{
    lorawan_rx_metadata data;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->get_rx_metadata(data));
}

TEST_F(Test_LoRaWANInterface, get_backoff_metadata)
{
    int i;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->get_backoff_metadata(i));
}

TEST_F(Test_LoRaWANInterface, cancel_sending)
{
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->cancel_sending());
}

