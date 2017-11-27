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

void lora_connect_with_params_otaa_wrong()
{
    lora_mac_status_t ret;
    lorawan_connect_t params;
    uint8_t counter = 0;

    //Allow upcoming events
    lora_helper.event_lock = false;

    uint8_t my_dev_eui[] = {0x01, 0x45, 0xB8, 0xDF, 0x01, 0x45, 0xB8, 0xDF};
    uint8_t my_app_eui[] = {0x45, 0xB8, 0xB8, 0xDF, 0x45, 0xB8, 0xB8, 0xDF};
    uint8_t my_app_key[] = {0x45, 0x45, 0xB8, 0xB8, 0xB8, 0xDF, 0x01, 0x45, 0x45, 0xB8, 0xB8, 0xB8, 0xDF};

    params.connect_type = LORAWAN_CONNECTION_OTAA;
    params.connection_u.otaa.dev_eui = my_dev_eui;
    params.connection_u.otaa.app_eui = my_app_eui;
    params.connection_u.otaa.app_key = my_app_key;
    params.connection_u.otaa.nb_trials = LORAWAN_NB_TRIALS;

    ret = lorawan.connect(params);
    TEST_ASSERT_MESSAGE(ret == LORA_MAC_STATUS_OK || ret == LORA_MAC_STATUS_CONNECT_IN_PROGRESS, "MAC status incorrect");

    // Wait for CONNECTED event
    while (1) {
        if (lora_helper.find_event(CONNECTED)) {
            TEST_ASSERT_MESSAGE(false, "Mote connected incorrectly");
            return;
        }

        // Fail on timeout
        if (counter >= 30) {
            //Test passed!
            //break;
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

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Connect with parameters OTAA wrong", lora_connect_with_params_otaa_wrong, greentea_failure_handler)
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
