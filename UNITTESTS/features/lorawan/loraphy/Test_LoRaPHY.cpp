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
#include "LoRaPHY.h"

#include "LoRaWANTimer_stub.h"

class my_LoRaPHY : public LoRaPHY {
public:
    my_LoRaPHY()
    {
        phy_params.adr_ack_delay = 1;
    }

    virtual ~my_LoRaPHY()
    {
    }

    loraphy_params_t &get_phy_params()
    {
        return phy_params;
    }
};

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

class Test_LoRaPHY : public testing::Test {
protected:
    my_LoRaPHY *object;

    virtual void SetUp()
    {
        object = new my_LoRaPHY();
        memset(&object->get_phy_params(), 0, sizeof(object->get_phy_params()));
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaPHY, initialize)
{
    object->initialize(NULL);
}

TEST_F(Test_LoRaPHY, set_radio_instance)
{
    my_radio radio;
    object->set_radio_instance(radio);
}

TEST_F(Test_LoRaPHY, put_radio_to_sleep)
{
    my_radio radio;
    object->set_radio_instance(radio);
    object->put_radio_to_sleep();
}

TEST_F(Test_LoRaPHY, put_radio_to_standby)
{
    my_radio radio;
    object->set_radio_instance(radio);
    object->put_radio_to_standby();
}

TEST_F(Test_LoRaPHY, handle_receive)
{
    my_radio radio;
    object->set_radio_instance(radio);
    object->handle_receive();
}

TEST_F(Test_LoRaPHY, handle_send)
{
    my_radio radio;
    object->set_radio_instance(radio);
    object->handle_send(NULL, 0);
}

TEST_F(Test_LoRaPHY, setup_public_network_mode)
{
    my_radio radio;
    channel_params_t p;
    object->get_phy_params().channels.channel_list = &p;
    object->set_radio_instance(radio);
    object->setup_public_network_mode(false);
}

TEST_F(Test_LoRaPHY, get_radio_rng)
{
    my_radio radio;
    object->set_radio_instance(radio);
    EXPECT_TRUE(0 != object->get_radio_rng());
}

TEST_F(Test_LoRaPHY, calculate_backoff)
{
    channel_params_t p[1];
    p[0].band = 0;
    p[0].dr_range.fields.min = DR_0;
    p[0].dr_range.fields.max = DR_5;
    object->get_phy_params().channels.channel_list = p;
    band_t b[1];
    b[0].duty_cycle = 0;
    b[0].higher_band_freq = 8689000;
    b[0].lower_band_freq = 8687000;
    b[0].max_tx_pwr = 20;
    b[0].last_join_tx_time = 0;
    b[0].last_tx_time = 0;
    b[0].off_time = 0;
    object->get_phy_params().bands.table = b;
    object->calculate_backoff(false, false, false, 0, 10, 12);

    object->calculate_backoff(false, true, false, 0, 3600000 + 10, 12);

    object->calculate_backoff(false, false, true, 0, 3600000 + 36000000 + 10, 12);
}

TEST_F(Test_LoRaPHY, mask_bit_test)
{
    uint16_t buf;
    buf = 0x08;
    EXPECT_TRUE(!object->mask_bit_test(&buf, 0));
}

TEST_F(Test_LoRaPHY, mask_bit_set)
{
    uint16_t buf;
    object->mask_bit_set(&buf, 3);
}

TEST_F(Test_LoRaPHY, mask_bit_clear)
{
    uint16_t buf;
    object->mask_bit_clear(&buf, 0);
}

TEST_F(Test_LoRaPHY, request_new_channel)
{
    band_t b;
    object->get_phy_params().bands.size = 1;
    b.higher_band_freq = 8689000;
    b.lower_band_freq = 8678000;
    b.duty_cycle = 0;
    b.last_join_tx_time = 0;
    b.last_tx_time = 0;
    b.max_tx_pwr = 20;
    b.off_time = 0;
    object->get_phy_params().bands.table = &b;

    channel_params_t p;
    //First 3 channels are set to be default
    p.band = 0;
    p.dr_range.fields.min = DR_0;
    p.dr_range.fields.max = DR_5;
    p.frequency = 8687000;
    p.rx1_frequency = 0;
    uint16_t dflt_msk = 0x07;
    object->get_phy_params().channels.default_mask = &dflt_msk;
    object->get_phy_params().channels.channel_list = &p;
    object->get_phy_params().custom_channelplans_supported = true;

    //Default channel, PARAMETER invalid
    EXPECT_TRUE(0 == object->request_new_channel(0, &p));

    //Freq & DR invalid
    p.frequency = 12345;
    p.dr_range.fields.max = 12;
    object->get_phy_params().max_channel_cnt = 16;
    object->get_phy_params().min_tx_datarate = DR_0;
    object->get_phy_params().max_tx_datarate = DR_5;
    // Frequency and DR are invalid - LORAWAN_STATUS_FREQ_AND_DR_INVALID
    EXPECT_TRUE(0 == object->request_new_channel(0, &p));

    //Freq invalid
    p.frequency = 12345;
    p.dr_range.fields.max = DR_5;
    object->get_phy_params().default_channel_cnt = 3;
    EXPECT_TRUE(2 == object->request_new_channel(0, &p));

    //DR invalid
    p.frequency = 8687000;
    p.dr_range.fields.max = 12;
    p.dr_range.fields.min = 1;
    EXPECT_TRUE(1 == object->request_new_channel(0, &p));

    //STATUS_OK
    p.dr_range.fields.max = DR_5;
    p.dr_range.fields.min = DR_0;
    uint16_t ch_msk = 0x08;
    object->get_phy_params().channels.mask = &ch_msk;
    EXPECT_TRUE(3 == object->request_new_channel(0, &p));
}

TEST_F(Test_LoRaPHY, set_last_tx_done)
{
    channel_params_t p[1];
    p[0].band = 0;
    object->get_phy_params().channels.channel_list = p;
    band_t b[1];
    object->get_phy_params().bands.table = b;
    object->set_last_tx_done(0, false, 0);

    object->set_last_tx_done(0, true, 0);
}

TEST_F(Test_LoRaPHY, restore_default_channels)
{
    channel_params_t p[1];
    p[0].band = 0;
    object->get_phy_params().channels.channel_list = p;
    uint16_t m, dm;
    object->get_phy_params().channels.mask_size = 1;
    object->get_phy_params().channels.default_mask = &dm;
    object->get_phy_params().channels.mask = &m;
    object->restore_default_channels();
}

TEST_F(Test_LoRaPHY, apply_cf_list)
{
    uint8_t list[16];
    memset(list, 0, 16);
    object->apply_cf_list(list, 0);

    object->get_phy_params().cflist_supported = true;
    object->apply_cf_list(list, 0);

    object->get_phy_params().default_channel_cnt = 1;
    object->get_phy_params().cflist_channel_cnt = 0;
    object->get_phy_params().max_channel_cnt = 3;

    uint16_t def_mask = 0x01;
    channel_params_t p[16];
    memset(p, 0, 16);
    //one default channel
    p[0].band = 0;
    p[0].dr_range.fields.min = DR_0;
    p[0].dr_range.fields.min = DR_5;
    p[0].frequency = 8687000;

    object->get_phy_params().channels.default_mask = &def_mask;
    object->get_phy_params().channels.mask = &def_mask;
    object->get_phy_params().channels.channel_list = p;
    object->apply_cf_list(list, 16);

    list[1] = 15;
    object->get_phy_params().cflist_channel_cnt = 1;
    object->apply_cf_list(list, 16);
}

TEST_F(Test_LoRaPHY, get_next_ADR)
{
    int8_t i = 0;
    int8_t j = 0;
    uint32_t ctr = 0;
    object->get_phy_params().min_tx_datarate = 0;
    EXPECT_TRUE(!object->get_next_ADR(false, i, j, ctr));

    i = 1;
    object->get_phy_params().adr_ack_limit = 3;
    EXPECT_TRUE(!object->get_next_ADR(false, i, j, ctr));

    object->get_phy_params().adr_ack_limit = 3;
    ctr = 4;
    object->get_phy_params().max_tx_power = 2;
    object->get_phy_params().adr_ack_delay = 1;
    EXPECT_TRUE(object->get_next_ADR(true, i, j, ctr));

    ctr = 5;
    object->get_phy_params().adr_ack_delay = 2;
    EXPECT_TRUE(!object->get_next_ADR(true, i, j, ctr));
}

TEST_F(Test_LoRaPHY, rx_config)
{
    my_radio radio;
    object->set_radio_instance(radio);
    uint8_t list;
    object->get_phy_params().datarates.table = &list;
    uint8_t list2;
    object->get_phy_params().payloads_with_repeater.table = &list2;
    rx_config_params_t p;
    memset(&p, 0, sizeof(rx_config_params_t));
    p.datarate = 0;
    p.rx_slot = RX_SLOT_WIN_1;
    channel_params_t pp[1];
    object->get_phy_params().channels.channel_list = pp;
    pp[0].rx1_frequency = 2;
    p.channel = 0;
    uint8_t tab[8];
    object->get_phy_params().payloads.table = tab;
    object->get_phy_params().payloads_with_repeater.table = tab;
    EXPECT_TRUE(object->rx_config(&p));

    p.datarate = DR_7;
    p.is_repeater_supported = true;
    object->get_phy_params().fsk_supported = true;
    EXPECT_TRUE(object->rx_config(&p));
}

TEST_F(Test_LoRaPHY, compute_rx_win_params)
{
    uint32_t list[1];
    list[0] = 125000;
    object->get_phy_params().bandwidths.table = list;
    uint8_t list2[1];
    list2[0] = 12;
    object->get_phy_params().datarates.table = &list2;
    channel_params_t ch_lst[16];
    memset(ch_lst, 0, sizeof(channel_params_t) * 16);
    ch_lst[0].band = 0;
    ch_lst[0].dr_range.fields.min = DR_0;
    ch_lst[0].dr_range.fields.max = DR_5;
    ch_lst[0].frequency = 8687000;
    object->get_phy_params().channels.channel_list = ch_lst;
    object->get_phy_params().channels.channel_list_size = 16;
    rx_config_params_t p;
    memset(&p, 0, sizeof(rx_config_params_t));
    p.frequency = 8687000;
    object->compute_rx_win_params(0, 0, 0, &p);

    p.datarate = 0;
    list[0] = 125000;
    object->compute_rx_win_params(0, 0, 0, &p);

    list[0] = 250000;
    object->compute_rx_win_params(0, 0, 0, &p);

    list[0] = 500000;
    object->get_phy_params().fsk_supported = true;
    object->get_phy_params().max_rx_datarate = 0;
    object->compute_rx_win_params(0, 0, 0, &p);
}

TEST_F(Test_LoRaPHY, tx_config)
{
    band_t b;
    memset(&b, 0, sizeof(band_t));
    object->get_phy_params().bands.table = &b;
    channel_params_t pp;
    memset(&pp, 0, sizeof(channel_params_t));
    pp.band = 0;
    object->get_phy_params().channels.channel_list = &pp;
    uint32_t list[1];
    list[0] = 125000;
    object->get_phy_params().bandwidths.table = &list;
    uint8_t list2[1];
    list2[0] = 12;
    object->get_phy_params().datarates.table = &list2;
    my_radio radio;
    object->set_radio_instance(radio);
    tx_config_params_t p;
    memset(&p, 0, sizeof(tx_config_params_t));
    p.channel = 0;
    int8_t i = 20;
    lorawan_time_t t = 36;
    object->tx_config(&p, &i, &t);

    p.datarate = 8;
    object->get_phy_params().max_tx_datarate = 8;
    object->tx_config(&p, &i, &t);
}

TEST_F(Test_LoRaPHY, link_ADR_request)
{
    adr_req_params_t p;
    memset(&p, 0, sizeof(adr_req_params_t));
    uint8_t b[100];
    memset(b, 0, 100);
    p.payload = b;
    b[0] = 0x03;
    b[1] = 1;
    b[2] = 0;
    b[3] = 0;
    b[4] = 1 << 4;
    b[5] = 0x03;
    b[6] = 1;
    b[7] = 1;
    b[8] = 1;
    b[9] = 6 << 4;
    b[10] = 0x03;
    b[11] = 1;
    b[12] = 0xff;
    b[13] = 0xff;
    b[14] = 0;
    b[15] = 0;
    p.payload_size = 16;
    int8_t i = 0, j = 0;
    uint8_t k = 0, l = 0;
    uint8_t t[5] = {12, 11, 10,  9,  8};
    t[0] = 0;
    object->get_phy_params().datarates.size = 5;
    object->get_phy_params().datarates.table = t;
    //Test without ADR payload does not make sense here.

    object->get_phy_params().max_channel_cnt = 16;
    channel_params_t li[16];
    memset(li, 0, sizeof(channel_params_t) * 16);
    object->get_phy_params().channels.channel_list = li;
    li[0].frequency = 0;
    li[1].frequency = 5;
    EXPECT_TRUE(4 == object->link_ADR_request(&p, &i, &j, &k, &l));

    t[0] = 3;
    //verify adr with p.adr_enabled = false
    EXPECT_TRUE(0 == object->link_ADR_request(&p, &i, &j, &k, &l));

    p.current_nb_trans = 0;
    EXPECT_TRUE(0 == object->link_ADR_request(&p, &i, &j, &k, &l));

    p.adr_enabled = true;
    li[0].dr_range.value = 0xff;
    object->get_phy_params().min_tx_datarate = DR_3;
    object->get_phy_params().max_tx_datarate = DR_8;

    //verify adr with status != 0
    EXPECT_TRUE(0 == object->link_ADR_request(&p, &i, &j, &k, &l));

    object->get_phy_params().max_tx_power = 2;
    object->get_phy_params().min_tx_power = 6;
    //verify adr with status != 0
    EXPECT_TRUE(4 == object->link_ADR_request(&p, &i, &j, &k, &l));

    object->get_phy_params().min_tx_datarate = DR_0;
    li[0].dr_range.value = 0xf0;
    EXPECT_TRUE(6 == object->link_ADR_request(&p, &i, &j, &k, &l));

    li[1].dr_range.fields.min = DR_0;
    li[1].dr_range.fields.max = DR_13;
    b[4] = 6 << 4;
    p.payload_size = 5;
    EXPECT_TRUE(7 == object->link_ADR_request(&p, &i, &j, &k, &l));

    uint16_t mask[2];
    object->get_phy_params().channels.mask = mask;
    object->get_phy_params().channels.mask_size = 2;
    EXPECT_TRUE(7 == object->link_ADR_request(&p, &i, &j, &k, &l));

    li[0].dr_range.value = 0xff;
    object->get_phy_params().max_channel_cnt = 0;
    EXPECT_TRUE(5 == object->link_ADR_request(&p, &i, &j, &k, &l));

    b[0] = 0x03;
    b[1] = 1;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    t[0] = 0;
    object->get_phy_params().datarates.size = 1;
    object->get_phy_params().datarates.table = t;
    //Test without ADR payload does not make sense here.

    object->get_phy_params().max_channel_cnt = 2;
    li[0].frequency = 0;
    li[1].frequency = 5;
    EXPECT_TRUE(4 == object->link_ADR_request(&p, &i, &j, &k, &l));
}

TEST_F(Test_LoRaPHY, accept_rx_param_setup_req)
{
    my_radio radio;
    radio.bool_value = true;
    object->set_radio_instance(radio);
    rx_param_setup_req_t req;
    req.datarate = DR_0;
    req.dr_offset = 0;
    req.frequency = 8678000;
    band_t band[1];
    memset(band, 0, sizeof(band_t));
    band[0].higher_band_freq = 8688000;
    band[0].lower_band_freq = 8666000;
    object->get_phy_params().bands.size = 1;
    object->get_phy_params().bands.table = band;
    EXPECT_TRUE(0x07 == object->accept_rx_param_setup_req(&req));
}

TEST_F(Test_LoRaPHY, accept_tx_param_setup_req)
{
    my_radio radio;
    object->set_radio_instance(radio);
    object->get_phy_params().accept_tx_param_setup_req = true;
    EXPECT_TRUE(object->accept_tx_param_setup_req(0, 0));
}

TEST_F(Test_LoRaPHY, dl_channel_request)
{
    EXPECT_TRUE(0 == object->dl_channel_request(0, 0));

    object->get_phy_params().dl_channel_req_supported = true;
    object->get_phy_params().bands.size = 1;
    band_t t[1];
    memset(t, 0, sizeof(band_t));
    t[0].higher_band_freq = 8688000;
    t[0].lower_band_freq = 8668000;
    object->get_phy_params().bands.size = 1;
    object->get_phy_params().bands.table = t;
    channel_params_t p[16];
    memset(p, 0, sizeof(channel_params_t) * 16);
    object->get_phy_params().channels.channel_list_size = 16;
    object->get_phy_params().channels.channel_list = p;

    p[0].frequency = 0;
    EXPECT_TRUE(0 == object->dl_channel_request(0, 1));

    t[0].higher_band_freq = 19;
    t[0].lower_band_freq = 0;
    p[0].frequency = 1;
    EXPECT_TRUE(3 == object->dl_channel_request(0, 1));
}

TEST_F(Test_LoRaPHY, get_alternate_DR)
{
    EXPECT_TRUE(0 == object->get_alternate_DR(0));

    object->get_phy_params().default_max_datarate = 5;
    object->get_phy_params().min_tx_datarate = 4;
    EXPECT_TRUE(5 == object->get_alternate_DR(1));

    object->get_phy_params().default_max_datarate = 6;
    object->get_phy_params().min_tx_datarate = 4;
    EXPECT_TRUE(5 == object->get_alternate_DR(2));
}

TEST_F(Test_LoRaPHY, set_next_channel)
{
    channel_selection_params_t p;
    memset(&p, 0, sizeof(channel_selection_params_t));
    band_t band[1];
    memset(band, 0, sizeof(band_t));
    band[0].higher_band_freq = 8687000;
    object->get_phy_params().bands.size = 1;
    object->get_phy_params().bands.table = band;
    uint8_t ch = 5;
    lorawan_time_t t1 = 16;
    lorawan_time_t t2 = 32;
    p.aggregate_timeoff = 10000;
    EXPECT_TRUE(LORAWAN_STATUS_DUTYCYCLE_RESTRICTED == object->set_next_channel(&p, &ch, &t1, &t2));

    uint16_t list[129];
    memset(list, 0, sizeof(list));
    list[4] = 1;
    list[128] = 1;
    object->get_phy_params().channels.mask = list;
    object->get_phy_params().channels.default_mask = list;
    object->get_phy_params().channels.mask_size = 1;
    p.aggregate_timeoff = 10000;
    EXPECT_TRUE(LORAWAN_STATUS_DUTYCYCLE_RESTRICTED == object->set_next_channel(&p, &ch, &t1, &t2));

    LoRaWANTimer_stub::time_value = 20000;
    EXPECT_TRUE(LORAWAN_STATUS_NO_CHANNEL_FOUND == object->set_next_channel(&p, &ch, &t1, &t2));

    p.joined = false;
    p.dc_enabled = false;
    band_t b[4];
    ch = 5;
    t1 = 16;
    t2 = 32;
    memset(b, 0, sizeof(band_t) * 4);
    object->get_phy_params().bands.size = 2;
    object->get_phy_params().bands.table = &b;
    b[0].off_time = 0;
    b[1].off_time = 9999999;
    memset(list, 0, 129);
    list[4] = 0;
    object->get_phy_params().channels.mask = list;
    object->get_phy_params().channels.default_mask = list;
    object->get_phy_params().channels.mask_size = 128;
    p.current_datarate = DR_1;
    object->get_phy_params().max_channel_cnt = 4;
    EXPECT_TRUE(LORAWAN_STATUS_NO_CHANNEL_FOUND == object->set_next_channel(&p, &ch, &t1, &t2));

    p.dc_enabled = true;
    EXPECT_TRUE(LORAWAN_STATUS_NO_CHANNEL_FOUND == object->set_next_channel(&p, &ch, &t1, &t2));

    list[4] = 1;
    p.joined = true;
    p.dc_enabled = false;
    channel_params_t l[4];
    l[0].dr_range.value = 0xff;
    l[1].dr_range.value = 0xff;
    l[2].dr_range.value = 0xf0;
    l[3].dr_range.value = 0xf0;
    l[2].band = 2;
    l[3].band = 3;
    object->get_phy_params().channels.channel_list = l;
    list[0] = 0xFF;
    b[2].off_time = 9999999;
    b[3].off_time = 0;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_next_channel(&p, &ch, &t1, &t2));

    b[0].off_time = 10000;
    LoRaWANTimer_stub::time_value = 2000;
    p.aggregate_timeoff = 1000;
    p.dc_enabled = true;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_next_channel(&p, &ch, &t1, &t2));
}

TEST_F(Test_LoRaPHY, add_channel)
{
    uint16_t list[16];
    object->get_phy_params().channels.mask = list;
    object->get_phy_params().channels.default_mask = list;
    channel_params_t p;
    p.band = 0;
    p.frequency = 0;
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->add_channel(&p, 0));

    object->get_phy_params().custom_channelplans_supported = true;
    object->get_phy_params().max_channel_cnt = 2;
    object->get_phy_params().min_tx_datarate = 0;
    object->get_phy_params().max_tx_datarate = 13;
    p.dr_range.fields.min = 6;
    p.dr_range.fields.max = 1;
    EXPECT_TRUE(LORAWAN_STATUS_FREQ_AND_DR_INVALID == object->add_channel(&p, 0));
}

TEST_F(Test_LoRaPHY, remove_channel)
{
    channel_params_t pp;
    pp.band = 0;
    object->get_phy_params().channels.channel_list = &pp;
    uint16_t list[16];
    list[0] = 1;
    object->get_phy_params().channels.mask = list;
    object->get_phy_params().channels.default_mask = list;
    EXPECT_TRUE(false == object->remove_channel(0));

    list[0] = 0;
    EXPECT_TRUE(false == object->remove_channel(0));

    object->get_phy_params().channels.mask_size = 1;
    object->get_phy_params().max_channel_cnt = 0;
    EXPECT_TRUE(false == object->remove_channel(0));

    object->get_phy_params().max_channel_cnt = 1;
    EXPECT_TRUE(true == object->remove_channel(0));
}

TEST_F(Test_LoRaPHY, set_tx_cont_mode)
{
    channel_params_t pp;
    pp.band = 0;
    object->get_phy_params().channels.channel_list = &pp;
    band_t b;
    b.max_tx_pwr = 10;
    object->get_phy_params().bands.table = &b;
    my_radio radio;
    object->set_radio_instance(radio);

    cw_mode_params_t p;
    p.max_eirp = 0;
    p.channel = 0;
    p.tx_power = -1;
    p.datarate = 0;
    p.antenna_gain = 1;
    object->set_tx_cont_mode(&p);

    p.max_eirp = 1;
    p.antenna_gain = 1;
    object->set_tx_cont_mode(&p, 1);
}

TEST_F(Test_LoRaPHY, apply_DR_offset)
{
    EXPECT_TRUE(0 == object->apply_DR_offset(0, 0));

    object->get_phy_params().min_tx_datarate = 1;
    EXPECT_TRUE(1 == object->apply_DR_offset(0, 2));
}

TEST_F(Test_LoRaPHY, reset_to_default_values)
{
    loramac_protocol_params p;
    object->reset_to_default_values(&p);

    object->reset_to_default_values(&p, true);
}

TEST_F(Test_LoRaPHY, get_next_lower_tx_datarate)
{
    EXPECT_TRUE(DR_0 == object->get_next_lower_tx_datarate(DR_2));

    object->get_phy_params().ul_dwell_time_setting = 1;
    object->get_phy_params().dwell_limit_datarate = DR_1;
    EXPECT_TRUE(DR_1 == object->get_next_lower_tx_datarate(DR_2));
}

TEST_F(Test_LoRaPHY, get_minimum_rx_datarate)
{
    EXPECT_TRUE(DR_0 == object->get_minimum_rx_datarate());

    object->get_phy_params().dl_dwell_time_setting = 1;
    object->get_phy_params().dwell_limit_datarate = DR_1;
    EXPECT_TRUE(DR_1 == object->get_minimum_rx_datarate());
}

TEST_F(Test_LoRaPHY, get_minimum_tx_datarate)
{
    EXPECT_TRUE(DR_0 == object->get_minimum_tx_datarate());

    object->get_phy_params().ul_dwell_time_setting = 1;
    object->get_phy_params().dwell_limit_datarate = DR_1;
    EXPECT_TRUE(DR_1 == object->get_minimum_tx_datarate());
}

TEST_F(Test_LoRaPHY, get_default_tx_datarate)
{
    EXPECT_TRUE(0 == object->get_default_tx_datarate());
}

TEST_F(Test_LoRaPHY, get_default_max_tx_datarate)
{
    EXPECT_TRUE(DR_0 == object->get_default_max_tx_datarate());
}

TEST_F(Test_LoRaPHY, get_default_tx_power)
{
    EXPECT_TRUE(0 == object->get_default_tx_power());
}

TEST_F(Test_LoRaPHY, get_max_payload)
{
    uint8_t list = 8;
    object->get_phy_params().payloads.table = &list;
    object->get_phy_params().payloads_with_repeater.table = &list;
    EXPECT_TRUE(8 == object->get_max_payload(0));

    EXPECT_TRUE(8 == object->get_max_payload(0, true));
}

TEST_F(Test_LoRaPHY, get_maximum_frame_counter_gap)
{
    EXPECT_TRUE(0 == object->get_maximum_frame_counter_gap());
}

TEST_F(Test_LoRaPHY, get_ack_timeout)
{
    EXPECT_TRUE(0 == object->get_ack_timeout());
}

TEST_F(Test_LoRaPHY, get_default_rx2_frequency)
{
    EXPECT_TRUE(0 == object->get_default_rx2_frequency());
}

TEST_F(Test_LoRaPHY, get_default_rx2_datarate)
{
    EXPECT_TRUE(0 == object->get_default_rx2_datarate());
}

TEST_F(Test_LoRaPHY, get_channel_mask)
{
    EXPECT_TRUE(0 == object->get_channel_mask());
    EXPECT_TRUE(0 == object->get_channel_mask(true));
}

TEST_F(Test_LoRaPHY, get_max_nb_channels)
{
    EXPECT_TRUE(0 == object->get_max_nb_channels());
}

TEST_F(Test_LoRaPHY, get_phy_channels)
{
    EXPECT_TRUE(0 == object->get_phy_channels());
}

TEST_F(Test_LoRaPHY, is_custom_channel_plan_supported)
{
    EXPECT_TRUE(false == object->is_custom_channel_plan_supported());
}

TEST_F(Test_LoRaPHY, verify_rx_datarate)
{
    EXPECT_TRUE(false == object->verify_rx_datarate(0));

    object->get_phy_params().datarates.size = 1;
    uint8_t t[1];
    t[0] = 2;
    object->get_phy_params().datarates.table = t;
    object->get_phy_params().dl_dwell_time_setting = 0;

    EXPECT_TRUE(true == object->verify_rx_datarate(0));

    object->get_phy_params().dl_dwell_time_setting = 1;
    object->get_phy_params().min_rx_datarate = 0;

    EXPECT_TRUE(true == object->verify_rx_datarate(0));
}

TEST_F(Test_LoRaPHY, verify_tx_datarate)
{
    EXPECT_TRUE(false == object->verify_tx_datarate(0));

    object->get_phy_params().datarates.size = 1;
    uint8_t t[1];
    t[0] = 2;
    object->get_phy_params().datarates.table = t;
    object->get_phy_params().ul_dwell_time_setting = 0;
    EXPECT_TRUE(true == object->verify_tx_datarate(0));

    object->get_phy_params().ul_dwell_time_setting = 1;
    EXPECT_TRUE(true == object->verify_tx_datarate(0));

    object->get_phy_params().ul_dwell_time_setting = 1;
    EXPECT_TRUE(true == object->verify_tx_datarate(0, true));
}

TEST_F(Test_LoRaPHY, verify_tx_power)
{
    EXPECT_TRUE(true == object->verify_tx_power(0));
}

TEST_F(Test_LoRaPHY, verify_duty_cycle)
{
    EXPECT_TRUE(true == object->verify_duty_cycle(false));

    EXPECT_TRUE(false == object->verify_duty_cycle(true));
}

TEST_F(Test_LoRaPHY, verify_nb_join_trials)
{
    EXPECT_TRUE(false == object->verify_nb_join_trials(0));
    EXPECT_TRUE(true == object->verify_nb_join_trials(100));
}


