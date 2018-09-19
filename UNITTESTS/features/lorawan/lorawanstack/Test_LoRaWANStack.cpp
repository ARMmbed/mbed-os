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
#include "LoRaWANStack.h"
#include "EventQueue.h"

#include "LoRaPHY_stub.h"

using namespace events;

class my_LoRaPHY : public LoRaPHY
{
public:
    my_LoRaPHY(){};

    virtual ~my_LoRaPHY(){};
};

uint8_t my_cb()
{
    return 1;
}

class my_radio : public LoRaRadio
{
public:

    virtual void init_radio(radio_events_t *events){};

    virtual void radio_reset(){};

    virtual void sleep(void){};

    virtual void standby(void){};

    virtual void set_rx_config (radio_modems_t modem, uint32_t bandwidth,
                                   uint32_t datarate, uint8_t coderate,
                                   uint32_t bandwidth_afc, uint16_t preamble_len,
                                   uint16_t symb_timeout, bool fix_len,
                                   uint8_t payload_len,
                                   bool crc_on, bool freq_hop_on, uint8_t hop_period,
                                   bool iq_inverted, bool rx_continuous){};

    virtual void set_tx_config(radio_modems_t modem, int8_t power, uint32_t fdev,
                                  uint32_t bandwidth, uint32_t datarate,
                                  uint8_t coderate, uint16_t preamble_len,
                                  bool fix_len, bool crc_on, bool freq_hop_on,
                                  uint8_t hop_period, bool iq_inverted, uint32_t timeout){};

    virtual void send(uint8_t *buffer, uint8_t size){};

    virtual void receive(void){};

    virtual void set_channel(uint32_t freq){};

    virtual uint32_t random(void){};

    virtual uint8_t get_status(void){return uint8_value;};

    virtual void set_max_payload_length(radio_modems_t modem, uint8_t max){};

    virtual void set_public_network(bool enable){};

    virtual uint32_t time_on_air(radio_modems_t modem, uint8_t pkt_len){};

    virtual bool perform_carrier_sense(radio_modems_t modem,
                                           uint32_t freq,
                                           int16_t rssi_threshold,
                                           uint32_t max_carrier_sense_time){ return bool_value;};

    virtual void start_cad(void){};

    virtual bool check_rf_frequency(uint32_t frequency){ return bool_value; };

    virtual void set_tx_continuous_wave(uint32_t freq, int8_t power, uint16_t time){};

    virtual void lock(void){};

    virtual void unlock(void){};

    bool bool_value;
    uint8_t uint8_value;
};



class Test_LoRaWANStack : public testing::Test {
protected:
    LoRaWANStack *object;

    virtual void SetUp()
    {
        object = new LoRaWANStack();
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaWANStack, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaWANStack, bind_phy_and_radio_driver)
{
    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);
}

TEST_F(Test_LoRaWANStack, initialize_mac_layer)
{
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->initialize_mac_layer(NULL));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));
}

TEST_F(Test_LoRaWANStack, set_lora_callbacks)
{
    lorawan_app_callbacks_t cb;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_lora_callbacks(&cb));
}

TEST_F(Test_LoRaWANStack, connect)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->connect());

    lorawan_connect_t conn;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->connect(conn));
}

TEST_F(Test_LoRaWANStack, add_channels)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->add_channels(plan));
}

TEST_F(Test_LoRaWANStack, remove_a_channel)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->remove_a_channel(1));
}

TEST_F(Test_LoRaWANStack, drop_channel_list)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->drop_channel_list());
}

TEST_F(Test_LoRaWANStack, get_enabled_channels)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->get_enabled_channels(plan));
}

TEST_F(Test_LoRaWANStack, set_confirmed_msg_retry)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_confirmed_msg_retry(1));
}

TEST_F(Test_LoRaWANStack, set_channel_data_rate)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_channel_data_rate(4));
}

TEST_F(Test_LoRaWANStack, enable_adaptive_datarate)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->enable_adaptive_datarate(false));
}

TEST_F(Test_LoRaWANStack, handle_tx)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->handle_tx(0, NULL, 0, 0, true, false));
}

TEST_F(Test_LoRaWANStack, handle_rx)
{
    uint8_t port;
    int flags;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->handle_rx(NULL, 0, port, flags, false));
}

TEST_F(Test_LoRaWANStack, set_link_check_request)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_link_check_request());
}

TEST_F(Test_LoRaWANStack, remove_link_check_request)
{
    object->remove_link_check_request();
}

TEST_F(Test_LoRaWANStack, shutdown)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->shutdown());
}

TEST_F(Test_LoRaWANStack, set_device_class)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_device_class(CLASS_A));
}

TEST_F(Test_LoRaWANStack, acquire_tx_metadata)
{
    lorawan_tx_metadata data;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_tx_metadata(data));
}

TEST_F(Test_LoRaWANStack, acquire_rx_metadata)
{
    lorawan_rx_metadata data;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_rx_metadata(data));
}

TEST_F(Test_LoRaWANStack, acquire_backoff_metadata)
{
    int b;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_backoff_metadata(b));
}

TEST_F(Test_LoRaWANStack, stop_sending)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->stop_sending());
}

TEST_F(Test_LoRaWANStack, lock)
{
    object->lock();
}

TEST_F(Test_LoRaWANStack, unlock)
{
    object->unlock();
}

