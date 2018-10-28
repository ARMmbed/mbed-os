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
#include "LoRaPHYAS923.h"

#include "LoRaPHY_stub.h"

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
        return uint8_value;
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
    uint8_t uint8_value;
};

class Test_LoRaPHYAS923 : public testing::Test {
protected:
    LoRaPHYAS923 *object;
    my_radio radio;

    virtual void SetUp()
    {
        LoRaPHY_stub::radio = &radio;
        object = new LoRaPHYAS923();
    }

    virtual void TearDown()
    {
        LoRaPHY_stub::radio = NULL;
        delete object;
    }
};

TEST_F(Test_LoRaPHYAS923, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaPHYAS923, get_alternate_DR)
{
    EXPECT_TRUE(2 == object->get_alternate_DR(1));
}

TEST_F(Test_LoRaPHYAS923, set_next_channel)
{
    channel_selection_params_t next_channel;
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

TEST_F(Test_LoRaPHYAS923, apply_DR_offset)
{
    //0, 1, 2, 3, 4, 5, -1, -2
    for (int i = 0; i < 8; i++) {
        uint8_t val = i > 5 ? 5 : 2;
        EXPECT_TRUE(object->apply_DR_offset(0, i));
    }
}

