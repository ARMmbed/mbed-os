/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#if TARGET_MTS_MDOT_F411RE
#include "SX1272_LoRaRadio.h"
#endif

#if defined(TARGET_K64F) || defined(TARGET_DISCO_L072CZ_LRWAN1)
#include "SX1276_LoRaRadio.h"
#endif
#include "LoRaWANInterface.h"

using namespace utest::v1;

#define MBED_CONF_LORA_PHY 0

#ifndef MBED_CONF_LORA_PHY
#error "Must set LoRa PHY layer parameters."
#else
#if MBED_CONF_LORA_PHY      == 0
 #include "LoRaPHYEU868.h"
 static LoRaPHYEU868 lora_phy;
#elif MBED_CONF_LORA_PHY    == 1
#include "LoRaPHYAS923.h"
static LoRaPHYAS923 lora_phy;
#elif MBED_CONF_LORA_PHY    == 2
#include "LoRaPHYAU915.h"
static LoRaPHYAU915 lora_phy;
#elif MBED_CONF_LORA_PHY    == 3
#include "LoRaPHYCN470.h"
static LoRaPHYCN470 lora_phy;
#elif MBED_CONF_LORA_PHY    == 4
#include "LoRaPHYCN779.h"
static LoRaPHYCN779 lora_phy;
#elif MBED_CONF_LORA_PHY    == 5
#include "LoRaPHYEU433.h"
static LoRaPHYEU433 lora_phy;
#elif MBED_CONF_LORA_PHY    == 6
#include "LoRaPHYIN865.h"
static LoRaPHYIN865 lora_phy;
#elif MBED_CONF_LORA_PHY    == 7
#include "LoRaPHYKR920.h"
static LoRaPHYKR920 lora_phy;
#elif MBED_CONF_LORA_PHY    == 8
#include "LoRaPHYUS915.h"
static LoRaPHYUS915 lora_phy;
#elif MBED_CONF_LORA_PHY    == 9
#include "LoRaPHYUS915Hybrid.h"
static LoRaPHYUS915Hybrid lora_phy;
#else
#error "Must set LoRa PHY layer parameters."
#endif
#endif

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LSTK"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#define mbed_trace_mutex_wait_function_set(...) (void(0)) //dummies if feature common pal is not added
#define mbed_trace_mutex_release_function_set(...) (void(0)) //dummies if feature common pal is not added
#define mbed_trace_init(...) (void(0)) //dummies if feature common pal is not added
#define mbed_trace_print_function_set(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

Serial pc(USBTX, USBRX);
static Mutex MyMutex;
static void my_mutex_wait()
{
    MyMutex.lock();
}
static void my_mutex_release()
{
    MyMutex.unlock();
}

void trace_printer(const char* str)
{
    printf("%s\r\n", str);
}

void lora_event_handler(lora_events_t events);

#if TARGET_MTS_MDOT_F411RE
    SX1272_LoRaRadio Radio(LORA_MOSI, LORA_MISO, LORA_SCK, LORA_NSS, LORA_RESET,
                           LORA_DIO0, LORA_DIO1, LORA_DIO2, LORA_DIO3, LORA_DIO4,
                           LORA_DIO5, NC, NC, LORA_TXCTL, LORA_RXCTL, NC, NC);
#endif
#if TARGET_K64F
    SX1276_LoRaRadio Radio(D11, D12, D13, D10, A0,
                           D2, D3, D4, D5, D8,
                           D9, NC, NC, NC, NC, A4, NC, NC);
#endif

#if defined(TARGET_DISCO_L072CZ_LRWAN1)
    #define LORA_SPI_MOSI   PA_7
    #define LORA_SPI_MISO   PA_6
    #define LORA_SPI_SCLK   PB_3
    #define LORA_CS         PA_15
    #define LORA_RESET      PC_0
    #define LORA_DIO0       PB_4
    #define LORA_DIO1       PB_1
    #define LORA_DIO2       PB_0
    #define LORA_DIO3       PC_13
    #define LORA_DIO4       PA_5
    #define LORA_DIO5       PA_4
    #define LORA_ANT_RX     PA_1
    #define LORA_ANT_TX     PC_2
    #define LORA_ANT_BOOST  PC_1
    #define LORA_TCXO       PA_12   // 32 MHz

    SX1276_LoRaRadio Radio(LORA_SPI_MOSI, LORA_SPI_MISO, LORA_SPI_SCLK, LORA_CS, LORA_RESET,
                           LORA_DIO0, LORA_DIO1, LORA_DIO2, LORA_DIO3, LORA_DIO4, NC,
                           NC, NC, LORA_ANT_TX, LORA_ANT_RX,
                           NC, LORA_ANT_BOOST, LORA_TCXO);
#endif

class LoRaTestHelper
{
public:
    LoRaTestHelper() :cur_event(0), event_lock(false) {
        memset(event_buffer, 0xFF, sizeof(event_buffer));
    };
    ~LoRaTestHelper() {};

    bool find_event(uint8_t event_code);

    uint8_t event_buffer[10];
    uint8_t cur_event;
    bool event_lock;
};

bool LoRaTestHelper::find_event(uint8_t event_code)
{
    event_lock = true;

    for (uint8_t i = 0; i < 10; i++) {
        if (event_buffer[i] == event_code) {
            event_buffer[i] = 0xFF;
            event_lock = false;
            return true;
        }
    }

    event_lock = false;
    return false;
}

LoRaWANInterface lorawan(Radio);
LoRaTestHelper lora_helper;

void lora_connect()
{
    lora_mac_status_t ret;
    uint8_t counter = 0;

    //Allow upcoming events
    lora_helper.event_lock = false;

    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_connect_with_params_wrong_type()
{
    lora_mac_status_t ret;
    lorawan_connect_t params;

    //Allow upcoming events
    lora_helper.event_lock = false;

    params.connect_type = (lorawan_connect_type_t)100;
    ret = lorawan.connect(params);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_PARAMETER_INVALID, "Incorrect return value, expected LORA_MAC_STATUS_PARAMETER_INVALID");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_connect_with_params_otaa_ok()
{
    lora_mac_status_t ret;
    lorawan_connect_t params;
    uint8_t counter = 0;

    //Allow upcoming events
    lora_helper.event_lock = false;

    uint8_t my_dev_eui[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    uint8_t my_app_eui[] = {0x6D, 0x75, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
    uint8_t my_app_key[] = {0xE8, 0x65, 0x60, 0xC8, 0x5E, 0x94, 0xFE, 0x49, 0xD3, 0xE1, 0x0E, 0x3E, 0x9A, 0xC6, 0x94, 0xA5};

    params.connect_type = LORAWAN_CONNECTION_OTAA;
    params.connection_u.otaa.dev_eui = my_dev_eui;
    params.connection_u.otaa.app_eui = my_app_eui;
    params.connection_u.otaa.app_key = my_app_key;

    ret = lorawan.connect(params);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_connect_with_params_abp_ok()
{
    lora_mac_status_t ret;
    lorawan_connect_t params;
    uint8_t counter = 0;

    uint8_t my_app_nwk_skey[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
    uint8_t my_app_skey[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    //Allow upcoming events
    lora_helper.event_lock = false;

    params.connect_type = LORAWAN_CONNECTION_ABP;
    params.connection_u.abp.dev_addr = 0x11111111;
    params.connection_u.abp.nwk_skey = my_app_nwk_skey;
    params.connection_u.abp.app_skey = my_app_skey;

    ret = lorawan.connect(params);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_disconnected()
{
    lora_mac_status_t ret;
    uint8_t counter = 0;

    //Allow upcoming events
    lora_helper.event_lock = false;

    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    counter = 0;
    wait_ms(2000);

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    // Wait for DISCONNECTED event
    while (1) {
        if (lora_helper.find_event(DISCONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Disconnect timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_tx_send_incorrect_type()
{
    int16_t ret;
    uint8_t counter = 0;
    uint8_t tx_data[11] = {"l"};
    int type_incorrect = 0x03; //No 0x03 type defined

    //Allow upcoming events
    lora_helper.event_lock = false;

    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    //ret = lorawan.send(session, message);
    ret = lorawan.send(LORAWAN_APP_PORT, tx_data, sizeof(tx_data), type_incorrect);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_PARAMETER_INVALID, "Incorrect return value, expected LORA_MAC_STATUS_PARAMETER_INVALID");

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_tx_send_fill_buffer()
{
    int16_t ret;
    uint8_t counter = 0;
    uint8_t tx_data[11] = {"l"};

    //Allow upcoming events
    lora_helper.event_lock = false;

    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    ret = lorawan.send(LORAWAN_APP_PORT, tx_data, sizeof(tx_data), MSG_UNCONFIRMED_FLAG);
    TEST_ASSERT_MESSAGE(ret == sizeof(tx_data), "Incorrect return value");

    ret = lorawan.send(LORAWAN_APP_PORT, tx_data, sizeof(tx_data), MSG_UNCONFIRMED_FLAG);
    TEST_ASSERT_MESSAGE(LORA_MAC_STATUS_WOULD_BLOCK, "Incorrect return value, expected LORA_MAC_STATUS_OK");

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_tx_send_without_connect()
{
    int16_t ret;
    uint8_t tx_data[11] = {"l"};

    //Allow upcoming events
    lora_helper.event_lock = false;

    ret = lorawan.send(LORAWAN_APP_PORT, tx_data, sizeof(tx_data), MSG_UNCONFIRMED_FLAG);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_NO_NETWORK_JOINED, "Incorrect return value");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_get_channel_plan_test()
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif

    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    int16_t ret;
    uint8_t counter = 0;

    lora_channels_t expected[16] = {
         { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
         { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
         { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
    };

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Get plan from stack
    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 3, "Invalid default channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));

}

void lora_remove_channel_test()
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif

    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    plan.nb_channels = 3;
    int16_t ret;
    uint8_t counter = 0;

    lora_channels_t expected[16] = {
        { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 4, 867300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 5, 867500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
    };

    plan.channels[0].id = 3;
    plan.channels[0].ch_param.band = 0;
    plan.channels[0].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[0].ch_param.frequency = 867100000;
    plan.channels[0].ch_param.rx1_frequency = 0;

    plan.channels[1].id = 4;
    plan.channels[1].ch_param.band = 0;
    plan.channels[1].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[1].ch_param.frequency = 867300000;
    plan.channels[1].ch_param.rx1_frequency = 0;

    plan.channels[2].id = 5;
    plan.channels[2].ch_param.band = 0;
    plan.channels[2].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[2].ch_param.frequency = 867500000;
    plan.channels[2].ch_param.rx1_frequency = 0;

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Set plan to stack
    if (lorawan.set_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "set_channel_plan failed");
        return;
    }

    // Make sure plan is zero'ed
    plan.nb_channels = 0;
    memset(&channels, 0, sizeof(channels));

    if (lorawan.remove_channel(3) != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel failed");
        return;
    }

    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 5, "Invalid channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));
}

void lora_remove_channel_plan() 
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif
    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    plan.nb_channels = 3;
    int16_t ret;
    uint8_t counter = 0;

    lora_channels_t expected[16] = {
        { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
    };

    plan.channels[0].id = 3;
    plan.channels[0].ch_param.band = 0;
    plan.channels[0].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[0].ch_param.frequency = 867100000;
    plan.channels[0].ch_param.rx1_frequency = 0;

    plan.channels[1].id = 4;
    plan.channels[1].ch_param.band = 0;
    plan.channels[1].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[1].ch_param.frequency = 867300000;
    plan.channels[1].ch_param.rx1_frequency = 0;

    plan.channels[2].id = 5;
    plan.channels[2].ch_param.band = 0;
    plan.channels[2].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[2].ch_param.frequency = 867500000;
    plan.channels[2].ch_param.rx1_frequency = 0;

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Set plan to stack
    if (lorawan.set_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "set_channel_plan failed");
        return;
    }

    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Make sure plan is zero'ed
    plan.nb_channels = 0;
    memset(&channels, 0, sizeof(channels));

    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 3, "Invalid channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));

}

void lora_set_all_channel_plan_test()
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif

    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    plan.nb_channels = 13;
    int16_t ret;
    uint8_t counter = 0;

    lora_channels_t expected[16] = {
        { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },      //EU868_LC1,
        { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },      //EU868_LC2
        { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },      //EU868_LC3
        { 3, 867100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 4, 867300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 5, 867500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 6, 867700000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 7, 867900000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 8, 868800000, 0, { ( ( DR_7 << 4 ) | DR_7 ) }, 2 },
        { 9, 868300000, 0, { ( ( DR_6 << 4 ) | DR_6 ) }, 1 },
        { 10, 868200000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 11, 869700000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 4 },
        { 12, 869400000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 3 },
        { 13, 868700000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 2 },
        { 14, 869200000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 2 },
        { 15, 868600000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 }
    };

    plan.channels[0].id = 3;
    plan.channels[0].ch_param.frequency = 867100000;
    plan.channels[0].ch_param.rx1_frequency = 0;
    plan.channels[0].ch_param.band = 0;
    plan.channels[0].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[0].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[1].id = 4;
    plan.channels[1].ch_param.frequency = 867300000;
    plan.channels[1].ch_param.rx1_frequency = 0;
    plan.channels[1].ch_param.band = 0;
    plan.channels[1].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[1].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[2].id = 5;
    plan.channels[2].ch_param.frequency = 867500000;
    plan.channels[2].ch_param.rx1_frequency = 0;
    plan.channels[2].ch_param.band = 0;
    plan.channels[2].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[2].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[3].id = 6;
    plan.channels[3].ch_param.frequency = 867700000;
    plan.channels[3].ch_param.rx1_frequency = 0;
    plan.channels[3].ch_param.band = 0;
    plan.channels[3].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[3].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[4].id = 7;
    plan.channels[4].ch_param.frequency = 867900000;
    plan.channels[4].ch_param.rx1_frequency = 0;
    plan.channels[4].ch_param.band = 0;
    plan.channels[4].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[4].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[5].id = 8;
    plan.channels[5].ch_param.frequency = 868800000;
    plan.channels[5].ch_param.rx1_frequency = 0;
    plan.channels[5].ch_param.band = 2;
    plan.channels[5].ch_param.dr_range.lora_mac_fields_s.max = DR_7;
    plan.channels[5].ch_param.dr_range.lora_mac_fields_s.min = DR_7;

    plan.channels[6].id = 9;
    plan.channels[6].ch_param.frequency = 868300000;
    plan.channels[6].ch_param.rx1_frequency = 0;
    plan.channels[6].ch_param.band = 1;
    plan.channels[6].ch_param.dr_range.lora_mac_fields_s.max = DR_6;
    plan.channels[6].ch_param.dr_range.lora_mac_fields_s.min = DR_6;

    plan.channels[7].id = 10;
    plan.channels[7].ch_param.frequency = 868200000;
    plan.channels[7].ch_param.rx1_frequency = 0;
    plan.channels[7].ch_param.band = 1;
    plan.channels[7].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[7].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[8].id = 11;
    plan.channels[8].ch_param.frequency = 869700000;
    plan.channels[8].ch_param.rx1_frequency = 0;
    plan.channels[8].ch_param.band = 4;
    plan.channels[8].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[8].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[9].id = 12;
    plan.channels[9].ch_param.frequency = 869400000;
    plan.channels[9].ch_param.rx1_frequency = 0;
    plan.channels[9].ch_param.band = 3;
    plan.channels[9].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[9].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[10].id = 13;
    plan.channels[10].ch_param.frequency = 868700000;
    plan.channels[10].ch_param.rx1_frequency = 0;
    plan.channels[10].ch_param.band = 2;
    plan.channels[10].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[10].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[11].id = 14;
    plan.channels[11].ch_param.frequency = 869200000;
    plan.channels[11].ch_param.rx1_frequency = 0;
    plan.channels[11].ch_param.band = 2;
    plan.channels[11].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[11].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    plan.channels[12].id = 15;
    plan.channels[12].ch_param.frequency = 868600000;
    plan.channels[12].ch_param.rx1_frequency = 0;
    plan.channels[12].ch_param.band = 1;
    plan.channels[12].ch_param.dr_range.lora_mac_fields_s.max = DR_5;
    plan.channels[12].ch_param.dr_range.lora_mac_fields_s.min = DR_0;

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Set plan to stack
    if (lorawan.set_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "set_channel_plan failed");
        return;
    }

    // Make sure plan is zero'ed
    plan.nb_channels = 0;
    memset(&channels, 0, sizeof(channels));

    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 16, "Invalid channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));

}

void lora_set_6_channel_plan_test()
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif

    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    plan.nb_channels = 3;
    int16_t ret;
    uint8_t counter = 0;

    lora_channels_t expected[16] = {
        { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 3, 867100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 4, 867300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 5, 867500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
    };

    plan.channels[0].id = 3;
    plan.channels[0].ch_param.band = 0;
    plan.channels[0].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[0].ch_param.frequency = 867100000;
    plan.channels[0].ch_param.rx1_frequency = 0;

    plan.channels[1].id = 4;
    plan.channels[1].ch_param.band = 0;
    plan.channels[1].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[1].ch_param.frequency = 867300000;
    plan.channels[1].ch_param.rx1_frequency = 0;

    plan.channels[2].id = 5;
    plan.channels[2].ch_param.band = 0;
    plan.channels[2].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[2].ch_param.frequency = 867500000;
    plan.channels[2].ch_param.rx1_frequency = 0;

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    // Set plan to stack
    if (lorawan.set_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "set_channel_plan failed");
        return;
    }

    // Make sure plan is zero'ed
    plan.nb_channels = 0;
    memset(&channels, 0, sizeof(channels));

    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 6, "Invalid channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));

}

void lora_channel_plan_extended()
{
#if MBED_CONF_LORA_PHY      != 0
    TEST_ASSERT_MESSAGE(false, "Only EU band is supported");
    return;
#endif

    //Allow upcoming events
    lora_helper.event_lock = false;

    lora_channels_t channels[16];
    lora_channelplan_t plan;
    plan.channels = channels;
    plan.nb_channels = 3;
    int16_t ret;
    uint8_t counter = 0;
    uint8_t rx_data[64] = { 0 };
    uint8_t tx_data[] = "test_confirmed_message";

    lora_channels_t expected[16] = {
        { 0, 868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 1, 868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 2, 868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1 },
        { 3, 867100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 4, 867300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
        { 5, 867500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 },
    };

    // get_channel_plan requires join to be done before calling it
    ret = lorawan.connect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "Connect failed");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            break;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Connection timeout");
            return;
        }
        wait_ms(1000);
        counter++;
    }

    // Perform packet send and receive, then verify that mote still sticks to current plan
    // Reset timeout counter
    counter = 0;

    // Gateway may add channels with Join Accept in CFList slot (optional)
    // Remove those channels from the channel plan before adding own custom channels
    if (lorawan.remove_channel_plan() != 0) {
        TEST_ASSERT_MESSAGE(false, "remove_channel_plan failed");
        return;
    }

    plan.channels[0].id = 3;
    plan.channels[0].ch_param.band = 0;
    plan.channels[0].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[0].ch_param.frequency = 867100000;
    plan.channels[0].ch_param.rx1_frequency = 0;

    plan.channels[1].id = 4;
    plan.channels[1].ch_param.band = 0;
    plan.channels[1].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[1].ch_param.frequency = 867300000;
    plan.channels[1].ch_param.rx1_frequency = 0;

    plan.channels[2].id = 5;
    plan.channels[2].ch_param.band = 0;
    plan.channels[2].ch_param.dr_range.value = ((DR_5 << 4) | DR_0);
    plan.channels[2].ch_param.frequency = 867500000;
    plan.channels[2].ch_param.rx1_frequency = 0;

    // Set plan to stack
    if (lorawan.set_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "set_channel_plan failed");
        return;
    }

    // Make sure plan is zero'ed
    plan.nb_channels = 0;
    memset(&channels, 0, sizeof(channels));

    lorawan.set_confirmed_msg_retries(5);
    ret = lorawan.send(LORAWAN_APP_PORT, tx_data, sizeof(tx_data), MSG_CONFIRMED_FLAG);


    if (ret != sizeof(tx_data)) {
        TEST_ASSERT_MESSAGE(false, "TX-message buffering failed");
        return;
    }

    while (1) {
        // Wait for TX_DONE event
        if (lora_helper.find_event(TX_DONE)) {
            break;
        }

        // Fail on TX_TIMEOUT
        if (lora_helper.find_event(TX_TIMEOUT)) {
            TEST_ASSERT_MESSAGE(false, "TX timeout");
            return;
        }

        // Fail on TX_ERROR
        if (lora_helper.find_event(TX_ERROR)) {
            TEST_ASSERT_MESSAGE(false, "TX error");
            return;
        }

        // Fail on TX_CRYPTO_ERROR
        if (lora_helper.find_event(TX_CRYPTO_ERROR)) {
            TEST_ASSERT_MESSAGE(false, "TX crypto error");
            return;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Send timeout");
            return;
        }

        wait_ms(1000);
        counter++;
    }

    // Reset timeout counter
    counter = 0;

    while (1) {
        // Wait for RX_DONE event
        if (lora_helper.find_event(RX_DONE)) {
            break;
        }

        // Fail on RX_TIMEOUT
        if (lora_helper.find_event(RX_TIMEOUT)) {
            TEST_ASSERT_MESSAGE(false, "RX timeout");
            return;
        }

        // Fail on RX_ERROR
        if (lora_helper.find_event(RX_ERROR)) {
            TEST_ASSERT_MESSAGE(false, "RX error");
            return;
        }

        // Fail on timeout
        if (counter >= 60) {
            TEST_ASSERT_MESSAGE(false, "Receive timeout");
            return;
        }

        wait_ms(1000);
        counter++;
    }

    ret = lorawan.receive(LORAWAN_APP_PORT, rx_data, 64, MSG_CONFIRMED_FLAG);
    if (ret < 0) {
        TEST_ASSERT_MESSAGE(false, "Receive failed");
        return;
    }

    if (strcmp((char*)rx_data, "Confirmed message") != 0) {
        TEST_ASSERT_MESSAGE(false, "Message incorrect");
        return;
    }

    if (lorawan.get_channel_plan(plan) != LORA_MAC_STATUS_OK) {
        TEST_ASSERT_MESSAGE(false, "get_channel_plan failed");
        return;
    }

    //EU band should have join channels by default
    TEST_ASSERT_MESSAGE(plan.nb_channels == 6, "Invalid channel count");

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        TEST_ASSERT_MESSAGE(plan.channels[i].id == expected[i].id, "Index mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.frequency == expected[i].ch_param.frequency, "Frequency mismatch");
        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.rx1_frequency == expected[i].ch_param.rx1_frequency, "Rx1 frequency mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.min, "Dr range min mismatch");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max == 
                expected[i].ch_param.dr_range.lora_mac_fields_s.max, "Dr range max mismath");

        TEST_ASSERT_MESSAGE(plan.channels[i].ch_param.band == expected[i].ch_param.band, "Band mismatch");
    }

    ret = lorawan.disconnect();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Disconnect failed");

    //Prevent upcoming events between tests
    lora_helper.event_lock = true;
    //Clear the event buffer
    memset(lora_helper.event_buffer, 0xFF, sizeof(lora_helper.event_buffer));

}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Connect without parameters test", lora_connect, greentea_failure_handler),
    Case("Connect with parameters wrong type", lora_connect_with_params_wrong_type, greentea_failure_handler),
    Case("Connect with parameters OTAA ok", lora_connect_with_params_otaa_ok, greentea_failure_handler),
    Case("Connect with parameters ABP ok", lora_connect_with_params_abp_ok, greentea_failure_handler),
    Case("Disconnect", lora_disconnected, greentea_failure_handler),
    Case("Get default channel plan", lora_get_channel_plan_test, greentea_failure_handler),
    Case("Remove channel", lora_remove_channel_test, greentea_failure_handler),
    Case("Remove channel plan", lora_remove_channel_plan, greentea_failure_handler),
    Case("Set all channels to channel plan", lora_set_all_channel_plan_test, greentea_failure_handler),
    Case("Set 6 channels to channel plan", lora_set_6_channel_plan_test, greentea_failure_handler),
    Case("Channel plan extended", lora_channel_plan_extended, greentea_failure_handler),
    Case("TX, send incorrect type message", lora_tx_send_incorrect_type, greentea_failure_handler),
    Case("TX, send fill buffer", lora_tx_send_fill_buffer, greentea_failure_handler),
    Case("TX, send without connection", lora_tx_send_without_connect, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(600, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}


int main() {
    lora_mac_status_t ret;

    pc.baud(115200);
    mbed_trace_mutex_wait_function_set( my_mutex_wait ); // only if thread safety is needed
    mbed_trace_mutex_release_function_set( my_mutex_release ); // only if thread safety is needed
    mbed_trace_init(); // initialize the trace library
    mbed_trace_print_function_set(trace_printer);

    lorawan.lora_event_callback(lora_event_handler);

    ret = lorawan.initialize();
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK, "Lora layer initialization failed");

    Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    Harness::run(specification);
}

void lora_event_handler(lora_events_t events)
{
    tr_debug("event handler");

    if (lora_helper.event_lock) {
        return;
    }

    tr_debug("insert event %i to index %i", events, lora_helper.cur_event % 10);
    lora_helper.event_buffer[lora_helper.cur_event % 10] = events;
    lora_helper.cur_event++;
}
