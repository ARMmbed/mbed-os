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
#include "LoRaPHYAU915.h"

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

class Test_LoRaPHYAU915 : public testing::Test {
protected:
    LoRaPHYAU915 *object;
    my_radio radio;

    virtual void SetUp()
    {
        LoRaPHY_stub::radio = &radio;
        object = new LoRaPHYAU915();
    }

    virtual void TearDown()
    {
        LoRaPHY_stub::radio = NULL;
        delete object;
    }
};

TEST_F(Test_LoRaPHYAU915, constructor)
{
    EXPECT_TRUE(object);
}

TEST_F(Test_LoRaPHYAU915, rx_config)
{
    rx_config_params_t p;
    memset(&p, 0, sizeof(p));

    radio.uint8_value = 1;
    EXPECT_TRUE(!object->rx_config(&p));

    radio.uint8_value = 0;
    p.is_repeater_supported = true;
    EXPECT_TRUE(object->rx_config(&p));

    p.is_repeater_supported = false;
    EXPECT_TRUE(object->rx_config(&p));
}

TEST_F(Test_LoRaPHYAU915, tx_config)
{
    tx_config_params_t p;
    memset(&p, 0, sizeof(p));
    int8_t tx = 0;
    lorawan_time_t time;
    p.tx_power = 9;
    EXPECT_TRUE(object->tx_config(&p, &tx, &time));
}

TEST_F(Test_LoRaPHYAU915, link_ADR_request)
{
    adr_req_params_t params;
    memset(&params, 0, sizeof(params));
    int8_t dr_out = 0;
    int8_t tx_power_out = 0;
    uint8_t nb_rep_out = 0;
    uint8_t nb_bytes_parsed = 0;

    uint8_t payload [] = {SRV_MAC_LINK_ADR_REQ, 1, 2, 3, 4};
    params.payload = payload;
    params.payload_size = 5;

    LoRaPHY_stub::uint8_value = 1;
    LoRaPHY_stub::ch_mask_value = 6;
    LoRaPHY_stub::adr_parse_count = 2;
    EXPECT_TRUE(1 == object->link_ADR_request(&params, &dr_out, &tx_power_out, &nb_rep_out, &nb_bytes_parsed));

    LoRaPHY_stub::adr_parse_count = 2;
    LoRaPHY_stub::ch_mask_value = 7;
    EXPECT_TRUE(1 == object->link_ADR_request(&params, &dr_out, &tx_power_out, &nb_rep_out, &nb_bytes_parsed));

    LoRaPHY_stub::adr_parse_count = 2;
    LoRaPHY_stub::ch_mask_value = 5;
    LoRaPHY_stub::uint8_value = 6;
    EXPECT_TRUE(6 == object->link_ADR_request(&params, &dr_out, &tx_power_out, &nb_rep_out, &nb_bytes_parsed));

    LoRaPHY_stub::adr_parse_count = 2;
    LoRaPHY_stub::ch_mask_value = 66;
    LoRaPHY_stub::uint8_value = 7;
    EXPECT_TRUE(7 == object->link_ADR_request(&params, &dr_out, &tx_power_out, &nb_rep_out, &nb_bytes_parsed));
}

TEST_F(Test_LoRaPHYAU915, accept_rx_param_setup_req)
{
    rx_param_setup_req_t p;
    memset(&p, 0, sizeof(p));
    radio.bool_value = false;
    EXPECT_TRUE(0 == object->accept_rx_param_setup_req(&p));

    radio.bool_value = true;
    p.frequency = 923300000 - 1;
    EXPECT_TRUE(0 == object->accept_rx_param_setup_req(&p));

    radio.bool_value = true;
    p.frequency = 927500000 + 1;
    p.datarate = 6;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    EXPECT_TRUE(2 == object->accept_rx_param_setup_req(&p));

    radio.bool_value = true;
    p.frequency = 923300000 + 600000;
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;
    LoRaPHY_stub::bool_table[1] = true;
    EXPECT_TRUE(7 == object->accept_rx_param_setup_req(&p));
}

TEST_F(Test_LoRaPHYAU915, get_alternate_DR)
{
    EXPECT_TRUE(0 == object->get_alternate_DR(0));

    EXPECT_TRUE(6 == object->get_alternate_DR(1));
}

TEST_F(Test_LoRaPHYAU915, set_next_channel)
{
    channel_selection_params_t params;
    uint8_t channel;
    lorawan_time_t time;
    lorawan_time_t timeoff;

    params.current_datarate = 6;
    params.aggregate_timeoff = 0;
    LoRaPHY_stub::uint8_value = 0;
    EXPECT_TRUE(LORAWAN_STATUS_NO_CHANNEL_FOUND == object->set_next_channel(&params, &channel, &time, &timeoff));

    radio.bool_value = false;
    params.aggregate_timeoff = 1;
    EXPECT_TRUE(LORAWAN_STATUS_DUTYCYCLE_RESTRICTED == object->set_next_channel(&params, &channel, &time, &timeoff));

    params.aggregate_timeoff = 0;
    LoRaPHY_stub::uint8_value = 1;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_next_channel(&params, &channel, &time, &timeoff));
}

TEST_F(Test_LoRaPHYAU915, apply_DR_offset)
{
//    { DR_8, DR_8, DR_8, DR_8, DR_8, DR_8 }, // DR_0
//    { DR_9, DR_8, DR_8, DR_8, DR_8, DR_8 }, // DR_1
//    { DR_10, DR_9, DR_8, DR_8, DR_8, DR_8 }, // DR_2
//    { DR_11, DR_10, DR_9, DR_8, DR_8, DR_8 }, // DR_3
//    { DR_12, DR_11, DR_10, DR_9, DR_8, DR_8 }, // DR_4
//    { DR_13, DR_12, DR_11, DR_10, DR_9, DR_8 }, // DR_5
//    { DR_13, DR_13, DR_12, DR_11, DR_10, DR_9 }, // DR_6

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            uint8_t val = 8 + i;
            val -= j;
            if (val > 13) {
                val = 13;
            }
            if (val < 8) {
                val = 8;
            }
            EXPECT_TRUE(val == object->apply_DR_offset(i, j));
        }
    }
}
