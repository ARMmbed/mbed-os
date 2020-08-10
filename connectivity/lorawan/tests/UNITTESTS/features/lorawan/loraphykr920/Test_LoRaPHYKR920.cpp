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
#include "LoRaPHYKR920.h"
#include "LoRaPHY_stub.h"
#include "LoRaRadio.h"

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
        return bool_value;
    };

    virtual void start_cad(void)
    {
    };

    virtual bool check_rf_frequency(uint32_t frequency)
    {
        return bool_value;
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

    bool bool_value;
};

class Test_LoRaPHYKR920 : public testing::Test {
protected:
    LoRaPHYKR920 *object;
    my_radio radio;

    virtual void SetUp()
    {
        LoRaPHY_stub::radio = &radio;
        object = new LoRaPHYKR920();
    }

    virtual void TearDown()
    {
        LoRaPHY_stub::radio = NULL;
        delete object;
    }
};

TEST_F(Test_LoRaPHYKR920, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaPHYKR920, verify_frequency_for_band)
{
    radio.bool_value = false;
    EXPECT_TRUE(false == object->verify_frequency_for_band(0, 0));

    radio.bool_value = true;
    EXPECT_TRUE(false == object->verify_frequency_for_band(0, 0));

    EXPECT_TRUE(true == object->verify_frequency_for_band(921100000, 0));
}

TEST_F(Test_LoRaPHYKR920, tx_config)
{
    tx_config_params_t tx_config;
    memset(&tx_config, 0, sizeof(tx_config));
    int8_t tx_power = 0;
    lorawan_time_t time = 0;

    tx_config.tx_power = 9;
    EXPECT_TRUE(true == object->tx_config(&tx_config, &tx_power, &time));
}

TEST_F(Test_LoRaPHYKR920, set_next_channel)
{
    channel_selection_params_t next_channel;
    memset(&next_channel, 0, sizeof(next_channel));
    lorawan_time_t backoff_time = 0;
    lorawan_time_t time = 0;
    uint8_t ch = 1;

    next_channel.aggregate_timeoff = 0;
    LoRaPHY_stub::uint8_value = 0;
    EXPECT_TRUE(LORAWAN_STATUS_NO_CHANNEL_FOUND == object->set_next_channel(&next_channel, &ch, &backoff_time, &time));

    next_channel.aggregate_timeoff = 1;
    radio.bool_value = false;
    EXPECT_TRUE(LORAWAN_STATUS_DUTYCYCLE_RESTRICTED == object->set_next_channel(&next_channel, &ch, &backoff_time, &time));

    next_channel.aggregate_timeoff = 0;
    LoRaPHY_stub::uint8_value = 1;
    EXPECT_TRUE(LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND == object->set_next_channel(&next_channel, &ch, &backoff_time, &time));

    radio.bool_value = true;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_next_channel(&next_channel, &ch, &backoff_time, &time));
}

TEST_F(Test_LoRaPHYKR920, set_tx_cont_mode)
{
    cw_mode_params_t params;
    memset(&params, 0, sizeof(params));
    params.tx_power = 9;
    object->set_tx_cont_mode(&params, 0);
}

