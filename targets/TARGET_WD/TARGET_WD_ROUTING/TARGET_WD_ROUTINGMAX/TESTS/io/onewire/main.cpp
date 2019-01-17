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

static const uint64_t referenceOneWireIds[2] = {0x28FF1F8002160369, 0x28A3953C05000011};
static vector<uint64_t> detectedOneWireIds;

int retrieve_onewire_ids(void)
{
    int sensorCount = routingmax_io.OneWireDS18B20.getSensorCount();
    uint64_t sensorIds[sensorCount];
    routingmax_io.OneWireDS18B20.getSensorIds(sensorIds);

    for (int i = 0; i < sensorCount; i++) {
        detectedOneWireIds.push_back(sensorIds[i]);
    }

    return sensorCount;
}

void test_onewire_enumeration()
{

    //OW_STATUS_CODE res = routingmax_io.OneWireDS18B20.start();
    OW_STATUS_CODE res = routingmax_io.OneWireDS18B20.enumerateSensors();
    wait_ms(200); // safety wait

    // enumeration successful?
    TEST_ASSERT_TRUE(res == OW_OK);

    // retrieve detected sensor IDs
    int sensorCount = retrieve_onewire_ids();

    // all sensors detected?
    for (int i = 0; i < sensorCount; i++) { // iterate through reference sensors

        TEST_ASSERT_TRUE_MESSAGE(find(detectedOneWireIds.begin(), detectedOneWireIds.end(), referenceOneWireIds[i]) != detectedOneWireIds.end(),
                                 "Could not find given 1-Wire sensor on bus!");

    }

    detectedOneWireIds.clear();
}

void test_onewire_temperature()
{

    // set measurement interval of DS18B20 sensor to 1 second
    routingmax_io.OneWireDS18B20.setMeasurementInterval(1);

    OW_STATUS_CODE res = routingmax_io.OneWireDS18B20.enumerateSensors();
    wait_ms(200); // safety wait

    // enumeration successful?
    TEST_ASSERT_TRUE(res == OW_OK);

    retrieve_onewire_ids();

    wait_ms(6000); // wait 6 seconds to collect measurements

    for (vector<uint64_t>::const_iterator it = detectedOneWireIds.begin(); it != detectedOneWireIds.end(); it++) {

        float temperatureValue = routingmax_io.OneWireDS18B20.getValue(*it);
        //wd_log_info("1-Wire %.8X%.8X temperature: %f", (uint32_t)((*it) >> 32), (uint32_t)(*it), temperatureValue);

        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(10.0f, 25.0f, temperatureValue, "Temperature value on 1-wire sensor is not within expected range (15C - 35C)!");

    }

    detectedOneWireIds.clear();
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("ONEWIRE enumeration", test_onewire_enumeration, greentea_failure_handler),
    Case("ONEWIRE temperature", test_onewire_temperature, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
