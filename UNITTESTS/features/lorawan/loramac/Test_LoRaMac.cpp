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
#include "LoRaMac.h"
#include "LoRaPHY_stub.h"
#include "LoRaMacCrypto_stub.h"
#include "LoRaMacCommand_stub.h"
#include "LoRaWANTimer_stub.h"
#include "EventQueue_stub.h"

using namespace events;

class my_phy : public LoRaPHY {
public:
    my_phy()
    {
    };

    virtual ~my_phy()
    {
    };
};

class Test_LoRaMac : public testing::Test {
protected:
    LoRaMac *object;

    virtual void SetUp()
    {
        object = new LoRaMac();
        LoRaWANTimer_stub::time_value = 1;
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaMac, constructor)
{
    EXPECT_TRUE(object);
}

void my_cb()
{

}

TEST_F(Test_LoRaMac, initialize)
{
    my_phy phy;
    object->bind_phy(phy);

    lorawan_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    uint8_t key[16];
    memset(key, 0, sizeof(key));
    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 2;
    object->prepare_join(&conn, true);

    channel_params_t params[] = {868300000, 0, { ((DR_5 << 4) | DR_0) }, 1};
    LoRaPHY_stub::channel_params_ptr = params;

    LoRaWANTimer_stub::call_cb_immediately = true;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize(NULL, my_cb));
}

TEST_F(Test_LoRaMac, disconnect)
{
    object->disconnect();
}

TEST_F(Test_LoRaMac, nwk_joined)
{
    EXPECT_EQ(false, object->nwk_joined());
}

TEST_F(Test_LoRaMac, add_channel_plan)
{
    lorawan_channelplan_t plan;
    EXPECT_EQ(LORAWAN_STATUS_OK, object->add_channel_plan(plan));

    object->set_tx_ongoing(true);
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->add_channel_plan(plan));
}

TEST_F(Test_LoRaMac, remove_channel_plan)
{
    EXPECT_EQ(LORAWAN_STATUS_OK, object->remove_channel_plan());

    object->set_tx_ongoing(true);
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->remove_channel_plan());
}

TEST_F(Test_LoRaMac, get_channel_plan)
{
    lorawan_channelplan_t plan;
    EXPECT_EQ(LORAWAN_STATUS_OK, object->get_channel_plan(plan));
}

TEST_F(Test_LoRaMac, remove_single_channel)
{
    EXPECT_EQ(LORAWAN_STATUS_OK, object->remove_single_channel(1));

    object->set_tx_ongoing(true);
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->remove_single_channel(1));
}

TEST_F(Test_LoRaMac, multicast_channel_link)
{
    multicast_params_t p;
    memset(&p, 0, sizeof(p));

    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->multicast_channel_link(NULL));

    object->set_tx_ongoing(true);
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->multicast_channel_link(&p));

    object->set_tx_ongoing(false);
    EXPECT_EQ(LORAWAN_STATUS_OK, object->multicast_channel_link(&p));
}

TEST_F(Test_LoRaMac, multicast_channel_unlink)
{
    multicast_params_t p;
    memset(&p, 0, sizeof(p));

    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->multicast_channel_unlink(NULL));

    object->set_tx_ongoing(true);
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->multicast_channel_unlink(&p));

    object->set_tx_ongoing(false);
    EXPECT_EQ(LORAWAN_STATUS_OK, object->multicast_channel_unlink(&p));
}

TEST_F(Test_LoRaMac, send)
{
    loramac_mhdr_t mac_hdr;
    memset(&mac_hdr, 0, sizeof(mac_hdr));
    uint8_t buf[15];
    memset(buf, 0, sizeof(buf));
    mac_hdr.bits.mtype = FRAME_TYPE_DATA_CONFIRMED_UP;
    object->send(&mac_hdr, 1, buf, 15);
}

TEST_F(Test_LoRaMac, get_default_tx_datarate)
{
    object->get_default_tx_datarate();
}

TEST_F(Test_LoRaMac, enable_adaptive_datarate)
{
    object->enable_adaptive_datarate(true);
}

TEST_F(Test_LoRaMac, set_channel_data_rate)
{
    object->set_channel_data_rate(8);
}

TEST_F(Test_LoRaMac, tx_ongoing)
{
    object->tx_ongoing();
}

TEST_F(Test_LoRaMac, set_tx_ongoing)
{
    object->set_tx_ongoing(true);
}

TEST_F(Test_LoRaMac, reset_ongoing_tx)
{
    object->reset_ongoing_tx(true);
}

TEST_F(Test_LoRaMac, prepare_ongoing_tx)
{
    uint8_t buf[16];
    memset(buf, 0, sizeof(buf));
    object->prepare_ongoing_tx(1, buf, 16, 1, 0);
}

TEST_F(Test_LoRaMac, send_ongoing_tx)
{
    object->send_ongoing_tx();
}

TEST_F(Test_LoRaMac, get_device_class)
{
    object->get_device_class();
}

void exp_cb()
{

}

TEST_F(Test_LoRaMac, set_device_class)
{
    object->set_device_class(CLASS_B, exp_cb);

    my_phy phy;
    object->bind_phy(phy);
    object->set_device_class(CLASS_C, exp_cb);
}

TEST_F(Test_LoRaMac, setup_link_check_request)
{
    object->setup_link_check_request();
}

TEST_F(Test_LoRaMac, prepare_join)
{
    lorawan_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    object->prepare_join(&conn, false);

    my_phy phy;
    object->bind_phy(phy);
    EXPECT_EQ(LORAWAN_STATUS_OK, object->join(false));

    uint8_t key[16];
    conn.connection_u.otaa.app_key = NULL;
    conn.connection_u.otaa.app_eui = NULL;
    conn.connection_u.otaa.dev_eui = NULL;
    conn.connection_u.otaa.nb_trials = 0;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, true));

    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = NULL;
    conn.connection_u.otaa.dev_eui = NULL;
    conn.connection_u.otaa.nb_trials = 0;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, true));

    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = NULL;
    conn.connection_u.otaa.nb_trials = 0;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, true));

    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 0;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, true));

    LoRaPHY_stub::bool_table[0] = false;
    LoRaPHY_stub::bool_counter = 0;
    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 2;
    EXPECT_EQ(LORAWAN_STATUS_OK, object->prepare_join(&conn, true));

    conn.connection_u.abp.dev_addr = 0;
    conn.connection_u.abp.nwk_id = 0;
    conn.connection_u.abp.nwk_skey = NULL;
    conn.connection_u.abp.app_skey = NULL;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, false));

    conn.connection_u.abp.dev_addr = 1;
    conn.connection_u.abp.nwk_id = 0;
    conn.connection_u.abp.nwk_skey = NULL;
    conn.connection_u.abp.app_skey = NULL;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, false));

    conn.connection_u.abp.dev_addr = 1;
    conn.connection_u.abp.nwk_id = 2;
    conn.connection_u.abp.nwk_skey = NULL;
    conn.connection_u.abp.app_skey = NULL;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, false));

    conn.connection_u.abp.dev_addr = 1;
    conn.connection_u.abp.nwk_id = 2;
    conn.connection_u.abp.nwk_skey = key;
    conn.connection_u.abp.app_skey = NULL;
    EXPECT_EQ(LORAWAN_STATUS_PARAMETER_INVALID, object->prepare_join(&conn, false));

    conn.connection_u.abp.dev_addr = 1;
    conn.connection_u.abp.nwk_id = 2;
    conn.connection_u.abp.nwk_skey = key;
    conn.connection_u.abp.app_skey = key;
    EXPECT_EQ(LORAWAN_STATUS_OK, object->prepare_join(&conn, false));

    EXPECT_EQ(LORAWAN_STATUS_OK, object->prepare_join(NULL, false));
}

TEST_F(Test_LoRaMac, join)
{
    my_phy phy;
    object->bind_phy(phy);
    EXPECT_EQ(LORAWAN_STATUS_OK, object->join(false));

    lorawan_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    uint8_t key[16];
    memset(key, 0, sizeof(key));
    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 2;
    object->prepare_join(&conn, true);
    EXPECT_EQ(LORAWAN_STATUS_CONNECT_IN_PROGRESS, object->join(true));
}

TEST_F(Test_LoRaMac, on_radio_tx_done)
{
    my_phy phy;
    object->bind_phy(phy);
    object->on_radio_tx_done(100);
}

TEST_F(Test_LoRaMac, on_radio_rx_done)
{
    uint8_t buf[16];
    memset(buf, 0, sizeof(buf));
    object->on_radio_rx_done(buf, 16, 0, 0);
}

TEST_F(Test_LoRaMac, on_radio_tx_timeout)
{
    object->on_radio_tx_timeout();
}

TEST_F(Test_LoRaMac, on_radio_rx_timeout)
{
    object->on_radio_rx_timeout(true);
}

TEST_F(Test_LoRaMac, continue_joining_process)
{
    my_phy phy;
    object->bind_phy(phy);
    lorawan_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    uint8_t key[16];
    memset(key, 0, sizeof(key));
    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 2;
    object->prepare_join(&conn, true);
    object->continue_joining_process();
}

TEST_F(Test_LoRaMac, continue_sending_process)
{
    my_phy phy;
    object->bind_phy(phy);
    object->continue_sending_process();
}

TEST_F(Test_LoRaMac, get_mcps_confirmation)
{
    object->get_mcps_confirmation();
}

TEST_F(Test_LoRaMac, get_mcps_indication)
{
    object->get_mcps_indication();
}

TEST_F(Test_LoRaMac, get_mlme_confirmation)
{
    object->get_mlme_confirmation();
}

TEST_F(Test_LoRaMac, get_mlme_indication)
{
    object->get_mlme_indication();
}

TEST_F(Test_LoRaMac, post_process_mcps_req)
{
    uint8_t data[16];
    memset(data, 0, sizeof(data));
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;

    my_phy phy;
    object->bind_phy(phy);
    object->join(false);

    object->prepare_ongoing_tx(1, data, 15, 0x01, 2);
    object->send_ongoing_tx();
    object->post_process_mcps_req();

    LoRaPHY_stub::bool_counter = 0;
    object->prepare_ongoing_tx(1, data, 15, 0x02, 2);
    object->send_ongoing_tx();
    object->post_process_mcps_req();

    //_mcps_confirmation.ack_received missing here
    uint8_t payload[16] = {};
    LoRaPHY_stub::uint16_value = 5;
    payload[0] = FRAME_TYPE_DATA_CONFIRMED_DOWN << 5;
    payload[5] = 1 << 5;

    //address != _params.dev_addr
    payload[2] = 2;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    payload[2] = 0;
    //mic failure
    payload[13] = 2;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    payload[13] = 0;
    //crypto failure
    LoRaMacCrypto_stub::int_table_idx_value = 0;
    LoRaMacCrypto_stub::int_table[0] = 4;
    LoRaMacCrypto_stub::int_table[1] = 4;
//    LoRaPHY_stub::uint16_value = 0;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    //process_mac_commands failure
    LoRaMacCommand_stub::status_value = LORAWAN_STATUS_BUSY;
    LoRaMacCrypto_stub::int_table[0] = 0;
    LoRaMacCrypto_stub::int_table[1] = 0;
    payload[7] = 1;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    //FOpts_len != 0
    payload[5] = (1 << 5) + 1;
    payload[7] = 0;
    LoRaMacCommand_stub::status_value = LORAWAN_STATUS_OK;
    payload[0] = FRAME_TYPE_DATA_UNCONFIRMED_DOWN << 5;

    object->on_radio_rx_done(payload, 13, 0, 0);

    //_mac_commands.process_mac_commands fails
    LoRaMacCommand_stub::status_value = LORAWAN_STATUS_DATARATE_INVALID;
    object->on_radio_rx_done(payload, 13, 0, 0);

    object->post_process_mcps_req();

    payload[9] = 1;
    LoRaMacCommand_stub::status_value = LORAWAN_STATUS_OK;
    payload[0] = FRAME_TYPE_PROPRIETARY << 5;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    payload[9] = 0;
    payload[5] = 1 << 5;
    LoRaMacCommand_stub::status_value = LORAWAN_STATUS_OK;
    object->on_radio_rx_done(payload, 16, 0, 0);
    object->post_process_mcps_req();

    LoRaPHY_stub::bool_counter = 0;
    object->prepare_ongoing_tx(1, data, 15, 0x04, 2);
    object->send_ongoing_tx();
    object->post_process_mcps_req();

    LoRaPHY_stub::bool_counter = 0;
    object->prepare_ongoing_tx(1, data, 15, 0x08, 2);
    object->send_ongoing_tx();
    object->post_process_mcps_req();
}

TEST_F(Test_LoRaMac, handle_join_accept_frame)
{
    LoRaPHY_stub::bool_counter = 0;
    LoRaPHY_stub::bool_table[0] = true;

    my_phy phy;
    object->bind_phy(phy);

    uint8_t payload[16] = {};
    LoRaPHY_stub::uint16_value = 5;
    payload[0] = FRAME_TYPE_JOIN_ACCEPT << 5;
    payload[5] = 1 << 5;

    LoRaMacCrypto_stub::int_table_idx_value = 0;
    LoRaMacCrypto_stub::int_table[0] = 4;
    LoRaMacCrypto_stub::int_table[1] = 4;
    LoRaMacCrypto_stub::int_table[2] = 4;
    LoRaMacCrypto_stub::int_table[3] = 4;
    object->on_radio_rx_done(payload, 16, 0, 0);

    LoRaMacCrypto_stub::int_table_idx_value = 0;
    LoRaMacCrypto_stub::int_table[0] = 0;
    object->on_radio_rx_done(payload, 16, 0, 0);

    LoRaMacCrypto_stub::int_table_idx_value = 0;
    LoRaMacCrypto_stub::int_table[1] = 0;
    object->on_radio_rx_done(payload, 16, 0, 0);

    //mic failure case
    payload[13] = 17;
    LoRaMacCrypto_stub::int_table_idx_value = 0;
    object->on_radio_rx_done(payload, 16, 0, 0);

    payload[13] = 0;
    LoRaMacCrypto_stub::int_table_idx_value = 0;
    LoRaMacCrypto_stub::int_table[2] = 0;
    object->on_radio_rx_done(payload, 16, 0, 0);
}

TEST_F(Test_LoRaMac, post_process_mcps_ind)
{
    object->post_process_mcps_ind();
}

TEST_F(Test_LoRaMac, post_process_mlme_request)
{
    object->post_process_mlme_request();
}

TEST_F(Test_LoRaMac, post_process_mlme_ind)
{
    object->post_process_mlme_ind();
}

uint8_t batt_cb()
{

}

TEST_F(Test_LoRaMac, set_batterylevel_callback)
{
    object->set_batterylevel_callback(batt_cb);
}

TEST_F(Test_LoRaMac, get_backoff_timer_event_id)
{
    object->get_backoff_timer_event_id();
}

TEST_F(Test_LoRaMac, clear_tx_pipe)
{
    EXPECT_EQ(LORAWAN_STATUS_NO_OP, object->clear_tx_pipe()); //timer id == 0

    my_phy phy;
    object->bind_phy(phy);

    lorawan_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    uint8_t key[16];
    memset(key, 0, sizeof(key));
    conn.connection_u.otaa.app_key = key;
    conn.connection_u.otaa.app_eui = key;
    conn.connection_u.otaa.dev_eui = key;
    conn.connection_u.otaa.nb_trials = 2;
    object->prepare_join(&conn, true);

    channel_params_t params[] = {868300000, 0, { ((DR_5 << 4) | DR_0) }, 1};
    LoRaPHY_stub::channel_params_ptr = params;
    LoRaWANTimer_stub::call_cb_immediately = true;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->initialize(NULL, my_cb));
    EventQueue_stub::int_value = 0;
    EXPECT_EQ(LORAWAN_STATUS_BUSY, object->clear_tx_pipe());
    loramac_mhdr_t machdr;
    machdr.bits.mtype = MCPS_UNCONFIRMED;
    uint8_t buf[1];
    buf[0] = 'T';
    LoRaPHY_stub::lorawan_status_value = LORAWAN_STATUS_DUTYCYCLE_RESTRICTED;
    EXPECT_TRUE(LORAWAN_STATUS_OK == object->send(&machdr, 15, buf, 1));

    EventQueue_stub::int_value = 1;
    EXPECT_EQ(LORAWAN_STATUS_OK, object->clear_tx_pipe());
}

TEST_F(Test_LoRaMac, get_current_time)
{
    object->get_current_time();
}

TEST_F(Test_LoRaMac, get_current_slot)
{
    object->get_current_slot();
}

TEST_F(Test_LoRaMac, get_QOS_level)
{
    EXPECT_EQ(0, object->get_QOS_level());
}

TEST_F(Test_LoRaMac, get_prev_QOS_level)
{
    EXPECT_EQ(1, object->get_prev_QOS_level());
}
