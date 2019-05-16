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
#include "events/EventQueue.h"

#include "LoRaPHY_stub.h"
#include "LoRaMac_stub.h"
#include "equeue_stub.h"
#include "lorawan_data_structures.h"

static uint8_t batt_level = 0;

using namespace events;

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

class my_radio : public LoRaRadio {
public:
    radio_events_t *_ev;

    virtual void init_radio(radio_events_t *events)
    {
        _ev = events;
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



class Test_LoRaWANStack : public testing::Test {
protected:
    LoRaWANStack *object;

    virtual void SetUp()
    {
        LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
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
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    //Visit callback
    if (LoRaMac_stub::_scheduling_failure_handler) {
        LoRaMac_stub::_scheduling_failure_handler.call();
    }
}

void events_cb(lorawan_event_t ev)
{

}

void lc_resp(uint8_t a, uint8_t b)
{

}

uint8_t batt_lvl()
{
    return batt_level;
}

TEST_F(Test_LoRaWANStack, set_lora_callbacks)
{
    lorawan_app_callbacks_t cb;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_lora_callbacks(&cb));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->set_lora_callbacks(NULL));

    cb.events = NULL;
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->set_lora_callbacks(&cb));

    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));
}

TEST_F(Test_LoRaWANStack, connect)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->connect());

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_BUSY;
    EXPECT_TRUE(LORAWAN_STATUS_BUSY == object->connect());

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect());

    //_ctrl_flags & CONN_IN_PROGRESS_FLAG
    EXPECT_TRUE(LORAWAN_STATUS_BUSY == object->connect());

    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = true;
    mlme.req_type = MLME_JOIN;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_value = false;
    radio._ev->rx_done(NULL, 0, 0, 0);

    //_ctrl_flags & CONNECTED_FLAG
    EXPECT_TRUE(LORAWAN_STATUS_ALREADY_CONNECTED == object->connect());

    //Visit rx_interrupt_handler's first if
    radio._ev->rx_done(NULL, 65535, 0, 0);
}

TEST_F(Test_LoRaWANStack, connect_args)
{
    lorawan_connect_t conn;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->connect(conn));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    conn.connect_type = lorawan_connect_type_t(8);
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->connect(conn));

    LoRaMac_stub::status_value = LORAWAN_STATUS_BUSY;
    conn.connect_type = LORAWAN_CONNECTION_OTAA;
    EXPECT_TRUE(LORAWAN_STATUS_BUSY == object->connect(conn));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect(conn));

    //_ctrl_flags & CONN_IN_PROGRESS_FLAG
    EXPECT_TRUE(LORAWAN_STATUS_BUSY == object->connect(conn));

    object->shutdown();
    conn.connect_type = LORAWAN_CONNECTION_ABP;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect(conn));

    //_ctrl_flags & CONNECTED_FLAG
    EXPECT_TRUE(LORAWAN_STATUS_ALREADY_CONNECTED == object->connect(conn));
}

TEST_F(Test_LoRaWANStack, add_channels)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->add_channels(plan));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->add_channels(plan));
}

TEST_F(Test_LoRaWANStack, remove_a_channel)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->remove_a_channel(1));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->remove_a_channel(1));
}

TEST_F(Test_LoRaWANStack, drop_channel_list)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->drop_channel_list());

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->drop_channel_list());
}

TEST_F(Test_LoRaWANStack, get_enabled_channels)
{
    lorawan_channelplan_t plan;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->get_enabled_channels(plan));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->get_enabled_channels(plan));
}

TEST_F(Test_LoRaWANStack, set_confirmed_msg_retry)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_confirmed_msg_retry(1));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->set_confirmed_msg_retry(255));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_confirmed_msg_retry(1));
}

TEST_F(Test_LoRaWANStack, set_channel_data_rate)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_channel_data_rate(4));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_channel_data_rate(4));
}

TEST_F(Test_LoRaWANStack, enable_adaptive_datarate)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->enable_adaptive_datarate(false));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->enable_adaptive_datarate(false));
}

TEST_F(Test_LoRaWANStack, handle_tx)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->handle_tx(0, NULL, 0, 0, true, false));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->handle_tx(0, NULL, 0, 0, false, false));

    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_link_check_request());

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_NO_ACTIVE_SESSIONS == object->handle_tx(0, NULL, 0, 0, true, false));

    lorawan_connect_t conn;
    conn.connect_type = LORAWAN_CONNECTION_ABP;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect(conn));

    LoRaMac_stub::bool_value = false;
    EXPECT_TRUE(LORAWAN_STATUS_NO_NETWORK_JOINED == object->handle_tx(0, NULL, 0, 0, true, false));

    LoRaMac_stub::bool_value = true;
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_tx(0, NULL, 0, 0, true, false));

    LoRaMac_stub::bool_false_counter = 1;
    LoRaMac_stub::bool_value = true;
    //set_application_port fails
    EXPECT_TRUE(LORAWAN_STATUS_PORT_INVALID == object->handle_tx(0, NULL, 0, 0, true, false));

    LoRaMac_stub::bool_false_counter = 1;
    LoRaMac_stub::bool_value = true;
    //Wrong flags -> LORAWAN_STATUS_PARAMETER_INVALID
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->handle_tx(1, NULL, 0, 0x04, true, false));

    LoRaMac_stub::bool_false_counter = 1;
    //Actual sending
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->handle_tx(1, NULL, 0, 0x08, true, false));

}

TEST_F(Test_LoRaWANStack, handle_rx)
{
    uint8_t port;
    int flags;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->handle_rx(NULL, 0, port, flags, false));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_NO_ACTIVE_SESSIONS == object->handle_rx(NULL, 0, port, flags, false));

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;

    lorawan_connect_t conn;
    conn.connect_type = LORAWAN_CONNECTION_ABP;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->connect(conn));
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_rx(NULL, 0, port, flags, false));

    //Prepare ready for receive state
    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));

    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    loramac_mcps_confirm_t conf;
    conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    ind.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = false;
    mlme.req_type = MLME_JOIN;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_value = true;
    conf.req_type = MCPS_PROPRIETARY;

    ind.pending = true;
    LoRaMac_stub::dev_class_value = CLASS_A;

    loramac_mlme_indication_t mlme_ind;
    mlme_ind.pending = false;
    LoRaMac_stub::mlme_ind_ptr = &mlme_ind;

    uint8_t ind_buf[150];
    for (int i = 0; i < 110; i++) {
        ind_buf[i] = i;
    }
    ind.buffer = ind_buf;
    ind.buffer_size = 150;
    ind.type = MCPS_UNCONFIRMED;
    ind.port = 15;
    ind.is_data_recvd = true;
    ind.fpending_status = false;
    LoRaMac_stub::dev_class_value = CLASS_A;
    radio._ev->rx_done(NULL, 0, 0, 0);

    //data == NULL || LENGTH == 0 (2 cases)
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->handle_rx(NULL, 0, port, flags, false));
    uint8_t data[50];
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->handle_rx(data, 0, port, flags, false));

    //validate_params returns Would block
    port = 43;
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_rx(data, 50, port, flags, true));

    ind.type = MCPS_CONFIRMED;
    radio._ev->rx_done(NULL, 0, 0, 0);
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_rx(data, 50, port, flags, true));
    //Call again to visit send_automatic_uplink_message error case
    LoRaMac_stub::bool_true_counter = 1;
    ind.type = MCPS_CONFIRMED;
    ind.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    LoRaMac_stub::bool_value = false;
    radio._ev->rx_done(NULL, 0, 0, 0);

    ind.status = LORAMAC_EVENT_INFO_STATUS_OK;

    LoRaMac_stub::bool_value = true;
    //convert_to_msg_flag cases
    ind.fpending_status = true;
    ind.type = MCPS_PROPRIETARY;
    radio._ev->rx_done(NULL, 0, 0, 0);
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_rx(data, 50, port, flags, true));

    ind.type = MCPS_MULTICAST;
    radio._ev->rx_done(NULL, 0, 0, 0);
    EXPECT_TRUE(LORAWAN_STATUS_WOULD_BLOCK == object->handle_rx(data, 50, port, flags, true));

    ind.type = MCPS_UNCONFIRMED;
    radio._ev->rx_done(NULL, 0, 0, 0);

    //read not complete
    EXPECT_TRUE(50 == object->handle_rx(data, 50, port, flags, false));
    EXPECT_EQ(10, data[10]);

    EXPECT_TRUE(50 == object->handle_rx(data, 50, port, flags, false));
    EXPECT_EQ(60, data[10]);

    //read complete
    EXPECT_TRUE(50 == object->handle_rx(data, 50, port, flags, false));
    EXPECT_EQ(100, data[0]);

    //read can fit the buffer
    for (int i = 0; i < 110; i++) {
        ind_buf[i] = i;
    }
    ind.buffer = ind_buf;
    ind.buffer_size = 50;
    ind.type = mcps_type_t(66);
    radio._ev->rx_done(NULL, 0, 0, 0);
    EXPECT_TRUE(50 == object->handle_rx(data, 50, port, flags, false));
    EXPECT_EQ(10, data[10]);
}

TEST_F(Test_LoRaWANStack, set_link_check_request)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_link_check_request());

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_PARAMETER_INVALID == object->set_link_check_request());

    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_link_check_request());
}

TEST_F(Test_LoRaWANStack, remove_link_check_request)
{
    object->remove_link_check_request();
}

TEST_F(Test_LoRaWANStack, shutdown)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->shutdown());

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    EXPECT_TRUE(LORAWAN_STATUS_DEVICE_OFF == object->shutdown());
}

TEST_F(Test_LoRaWANStack, set_device_class)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->set_device_class(CLASS_A));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_UNSUPPORTED == object->set_device_class(CLASS_B));

    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_device_class(CLASS_A));
}

TEST_F(Test_LoRaWANStack, acquire_tx_metadata)
{
    lorawan_tx_metadata data;
    memset(&data, 0, sizeof(data));
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_tx_metadata(data));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    // stale = true;
    EXPECT_TRUE(LORAWAN_STATUS_METADATA_NOT_AVAILABLE == object->acquire_tx_metadata(data));

    // stale = false;
    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    memset(&conf, 0, sizeof(conf));
    conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    LoRaMac_stub::slot_value = RX_SLOT_WIN_2;
    radio._ev->rx_timeout();

    EXPECT_TRUE(LORAWAN_STATUS_OK == object->acquire_tx_metadata(data));
}

TEST_F(Test_LoRaWANStack, acquire_rx_metadata)
{
    lorawan_rx_metadata data;
    memset(&data, 0, sizeof(data));
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_rx_metadata(data));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    // stale = true;
    EXPECT_TRUE(LORAWAN_STATUS_METADATA_NOT_AVAILABLE == object->acquire_rx_metadata(data));

    // stale = false;
    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    memset(&conf, 0, sizeof(conf));
    conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    memset(&ind, 0, sizeof(ind));
    ind.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = true;
    mlme.req_type = MLME_JOIN;

    //Visit mlme_confirm_handler here also
    mlme.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
    LoRaMac_stub::bool_value = false;
    radio._ev->rx_done(NULL, 0, 0, 0);

    mlme.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;
    radio._ev->rx_done(NULL, 0, 0, 0);

    mlme.status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;
    LoRaMac_stub::slot_value = RX_SLOT_WIN_2;
    radio._ev->rx_done(NULL, 0, 0, 0);

    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));
    mlme.req_type = MLME_LINK_CHECK;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_true_counter = true;
    radio._ev->rx_done(NULL, 0, 0, 0);

    EXPECT_TRUE(LORAWAN_STATUS_OK == object->acquire_rx_metadata(data));
}

TEST_F(Test_LoRaWANStack, acquire_backoff_metadata)
{
    int b;
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->acquire_backoff_metadata(b));

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::int_value = 2;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->acquire_backoff_metadata(b));

    LoRaMac_stub::int_value = 0;
    EXPECT_TRUE(LORAWAN_STATUS_METADATA_NOT_AVAILABLE == object->acquire_backoff_metadata(b));
}

TEST_F(Test_LoRaWANStack, stop_sending)
{
    EXPECT_TRUE(LORAWAN_STATUS_NOT_INITIALIZED == object->stop_sending());

    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    LoRaMac_stub::status_value = LORAWAN_STATUS_BUSY;
    EXPECT_TRUE(LORAWAN_STATUS_BUSY == object->stop_sending());

    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->stop_sending());
}

TEST_F(Test_LoRaWANStack, lock)
{
    object->lock();
}

TEST_F(Test_LoRaWANStack, unlock)
{
    object->unlock();
}

TEST_F(Test_LoRaWANStack, interrupt_functions)
{
    lorawan_connect_t conn;
    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = true;
    mlme.req_type = MLME_JOIN;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_value = false;
    radio._ev->rx_done(NULL, 0, 0, 0);

    radio._ev->rx_done(NULL, 0, 0, 0);

    radio._ev->rx_error();
    LoRaMac_stub::slot_value = RX_SLOT_WIN_2;
    radio._ev->rx_error();

    conf.req_type = MCPS_UNCONFIRMED;
    LoRaMac_stub::bool_value = true;
    radio._ev->rx_error();

    conf.req_type = MCPS_CONFIRMED;
    radio._ev->rx_error();

    LoRaMac_stub::bool_value = false;

    LoRaMac_stub::slot_value = RX_SLOT_WIN_1;
    radio._ev->rx_timeout();

    radio._ev->tx_timeout();

    object->shutdown();
    conn.connect_type = LORAWAN_CONNECTION_OTAA;
    object->connect(conn);
    LoRaMac_stub::status_value = LORAWAN_STATUS_OK;
    object->connect(conn);
    radio._ev->tx_timeout();
}

TEST_F(Test_LoRaWANStack, process_transmission)
{
    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));

    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    object->connect();

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = true;
    mlme.req_type = MLME_JOIN;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_value = false;
    radio._ev->rx_done(NULL, 0, 0, 0);

    LoRaMac_stub::bool_value = true;
    conf.req_type = MCPS_PROPRIETARY;
    LoRaMac_stub::bool_false_counter = 1;
    LoRaMac_stub::dev_class_value = CLASS_A;
    object->handle_tx(1, NULL, 0, MSG_UNCONFIRMED_FLAG, true, false);
    radio._ev->tx_done();

    LoRaMac_stub::bool_false_counter = 1;
    LoRaMac_stub::dev_class_value = CLASS_A;
    object->handle_tx(1, NULL, 0, MSG_UNCONFIRMED_FLAG, true, false);
    radio._ev->tx_done();

    LoRaMac_stub::bool_false_counter = 1;
    LoRaMac_stub::dev_class_value = CLASS_C;
    object->handle_tx(1, NULL, 0, MSG_UNCONFIRMED_FLAG, true, false);
    radio._ev->tx_done();

    LoRaMac_stub::bool_false_counter = 1;
    conf.req_type = MCPS_CONFIRMED;
    object->handle_tx(1, NULL, 0, MSG_UNCONFIRMED_FLAG, true, false);
    radio._ev->tx_done();
}

TEST_F(Test_LoRaWANStack, process_reception)
{
    EventQueue queue;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize_mac_layer(&queue));

    //Prepare ready for receive state
    lorawan_app_callbacks_t cb;
    cb.events = events_cb;
    cb.link_check_resp = lc_resp;
    cb.battery_level = batt_lvl;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->set_lora_callbacks(&cb));

    my_radio radio;
    my_LoRaPHY phy;
    object->bind_phy_and_radio_driver(radio, phy);

    struct equeue_event ptr;
    equeue_stub.void_ptr = &ptr;
    equeue_stub.call_cb_immediately = true;
    loramac_mcps_confirm_t conf;
    memset(&conf, 0, sizeof(&conf));
    LoRaMac_stub::mcps_conf_ptr = &conf;
    radio._ev->tx_done();

    loramac_mcps_indication_t ind;
    memset(&ind, 0, sizeof(ind));
    LoRaMac_stub::mcps_ind_ptr = &ind;

    loramac_mlme_confirm_t mlme;
    LoRaMac_stub::mlme_conf_ptr = &mlme;
    mlme.pending = false;
    mlme.req_type = MLME_JOIN;
    mlme.status = LORAMAC_EVENT_INFO_STATUS_OK;
    LoRaMac_stub::bool_value = true;
    conf.req_type = MCPS_PROPRIETARY;

    ind.pending = true;
    LoRaMac_stub::dev_class_value = CLASS_C;

    loramac_mlme_indication_t mlme_ind;
    mlme_ind.pending = false;
    LoRaMac_stub::mlme_ind_ptr = &mlme_ind;

    uint8_t ind_buf[150];
    for (int i = 0; i < 110; i++) {
        ind_buf[i] = i;
    }
    ind.buffer = ind_buf;
    ind.buffer_size = 150;

    //_loramac.get_mcps_confirmation()->req_type == MCPS_CONFIRMED
    conf.req_type = MCPS_CONFIRMED;
    conf.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;
    radio._ev->rx_done(NULL, 0, 0, 0);

    ind.is_ack_recvd = false;
    LoRaMac_stub::bool_true_counter = 1;
    LoRaMac_stub::bool_value = false;
    LoRaMac_stub::slot_value = RX_SLOT_WIN_2;
    conf.status = LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;
    radio._ev->rx_done(NULL, 0, 0, 0);

    conf.req_type = MCPS_UNCONFIRMED;
    LoRaMac_stub::dev_class_value = CLASS_A;
    LoRaMac_stub::bool_true_counter++;
    mlme_ind.pending = true;
    mlme_ind.indication_type = MLME_SCHEDULE_UPLINK;
    conf.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    radio._ev->rx_done(NULL, 0, 0, 0);

    ind.is_ack_recvd = true;
    conf.req_type = MCPS_CONFIRMED;
    conf.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;
    radio._ev->rx_done(NULL, 0, 0, 0);
}

