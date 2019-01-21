/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
//#include "wd_logging.h"
#include <vector>
#include <algorithm>

#include "io.h"

using namespace utest::v1;

//static const uint64_t referenceOneWireIds[2] = {0x28D7D82700008045, 0x285D012800008057}; //theese are the sensor ids of the connected sensors
static const uint64_t referenceOneWireIds[2] = {0x281BEB270000806D, 0x2834B127000080D7}; //theese are the sensor ids of the connected sensors

static vector<uint64_t> detectedOneWireIds;


///
/// !!!!! TEST DISABLED IN .mbedignore
///
int retrieve_onewire_ids(void)
{
    events::EventQueue *queue = mbed_event_queue();

    int sensorCount = routingmax_io.OneWireDS18B20.getSensorCount();
    uint64_t sensorIds[sensorCount];
    routingmax_io.OneWireDS18B20.getSensorIds(sensorIds);
    queue->dispatch(1000);

    for (int i = 0; i < sensorCount; i++) {
        if (std::find(detectedOneWireIds.begin(), detectedOneWireIds.end(), sensorIds[i]) == detectedOneWireIds.end()) {
            detectedOneWireIds.push_back(sensorIds[i]);
        }
    }

    return sensorCount;
}

///
/// !!!!! TEST DISABLED IN .mbedignore
///
void test_onewire_enumeration()
{
    events::EventQueue *queue = mbed_event_queue();
    queue->dispatch(5000);

    routingmax_io.OneWireDS18B20.setMeasurementInterval(1);
    queue->dispatch(1000);

    OW_STATUS_CODE res = routingmax_io.OneWireDS18B20.enumerateSensors();

    // enumeration successful?
    TEST_ASSERT_EQUAL_INT_MESSAGE(OW_OK, res, "OneWire DS18B20 enumeration failed");

    // retrieve detected sensor IDs
    int sensorCount = retrieve_onewire_ids();

    TEST_ASSERT_EQUAL_UINT_MESSAGE(2, sensorCount, "expecting 2 onewire sensors");

    // all sensors detected?
    for (int i = 0; i < sensorCount; i++) { // iterate through reference sensors
        TEST_ASSERT_TRUE_MESSAGE(find(detectedOneWireIds.begin(), detectedOneWireIds.end(), referenceOneWireIds[i]) != detectedOneWireIds.end(), "Could not find given 1-Wire sensor on bus!");
    }

    detectedOneWireIds.clear();
}

///
/// !!!!! TEST DISABLED IN .mbedignore
///
void test_onewire_temperature()
{

    events::EventQueue *queue = mbed_event_queue();
    queue->dispatch(5000);

    // set measurement interval of DS18B20 sensor to 1 second
    routingmax_io.OneWireDS18B20.setMeasurementInterval(1);
    queue->dispatch(1000);

    OW_STATUS_CODE res = routingmax_io.OneWireDS18B20.enumerateSensors();

    // enumeration successful?
    TEST_ASSERT_EQUAL_INT_MESSAGE(OW_OK, res, "OneWire DS18B20 enumeration failed");

    // retrieve detected sensor IDs
    int sensorCount = retrieve_onewire_ids();

    TEST_ASSERT_EQUAL_UINT_MESSAGE(2, sensorCount, "expecting 2 onewire sensors");

    queue->dispatch(10000);

    for (auto it = detectedOneWireIds.cbegin(); it != detectedOneWireIds.cend(); it++) {

        float temperatureValue = routingmax_io.OneWireDS18B20.getValue(*it);
        //wd_log_info("1-Wire %.8X%.8X temperature: %f", (uint32_t)((*it) >> 32), (uint32_t)(*it), temperatureValue);

        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(10.0f, 25.0f, temperatureValue, "Temperature value on 1-wire sensor is not within expected range (15C - 35C)!");

    }

    detectedOneWireIds.clear();
}

///
/// !!!!! TEST DISABLED IN .mbedignore
///
utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

///
/// !!!!! TEST DISABLED IN .mbedignore
///
Case cases[] = {
    Case("ONEWIRE enumeration", test_onewire_enumeration, greentea_failure_handler)
    , Case("ONEWIRE temperature", test_onewire_temperature, greentea_failure_handler)
};

///
/// !!!!! TEST DISABLED IN .mbedignore
///
utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

///
/// !!!!! TEST DISABLED IN .mbedignore
///
Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

///
/// !!!!! TEST DISABLED IN .mbedignore
///
int main()
{
    Harness::run(specification);
}
