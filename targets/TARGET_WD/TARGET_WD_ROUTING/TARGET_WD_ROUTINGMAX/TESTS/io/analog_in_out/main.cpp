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

#include "io.h"
#include <cstdio>

using namespace utest::v1;

void test_analog_in_out_set_and_read_state(uint value)
{

    //wd_log_info("setting output value to %d", value);
    for (int i = 0; i < AOUTCount; i++) {
        routingmax_io.AOUTs[i].setValue(value);
    }

    // start basync measurement
    for (int i = 0; i < 2; i++) { // only IN1 and IN2 are connected (to OUT1 and OUT2, respectively)
        //wd_log_info("measured value: %.2f", routingmax_io.AINs[i].getValue());
        routingmax_io.AINs[i].start();
    }

    // wait for all measurements to refresh (mux selection + median window)
    wait_ms(8000);
    // for (int i = 0; i < 80; i++) {
    //     routingmax_io.AINs[i].an
    //     wait_ms(100);
    // }

    // accuracy: DIN and DOUT each 0.25% without calibration -> 16.0f * ( 0.0025f + 0.0025f )
    //wd_log_info("check if corresponding input state matches value");
    for (int i = 0; i < 2; i++) { // only IN1 and IN2 are connected (to OUT1 and OUT2, respectively)
        //wd_log_info("measured value: %.2f", routingmax_io.AINs[i].getValue());
        char msg_buf [55];
        sprintf(msg_buf, "AI[%d] Input value was not within expected range!", i + 1);

        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(value * (0.0025f + 0.0025f), value, routingmax_io.AINs[i].getValue(), msg_buf);
    }

}

void test_analog_in_out_set_4ma(void)
{
    return test_analog_in_out_set_and_read_state(4);
}
void test_analog_in_out_set_5ma(void)
{
    return test_analog_in_out_set_and_read_state(5);
}
void test_analog_in_out_set_6ma(void)
{
    return test_analog_in_out_set_and_read_state(6);
}
void test_analog_in_out_set_7ma(void)
{
    return test_analog_in_out_set_and_read_state(7);
}
void test_analog_in_out_set_8ma(void)
{
    return test_analog_in_out_set_and_read_state(8);
}
void test_analog_in_out_set_9ma(void)
{
    return test_analog_in_out_set_and_read_state(9);
}
void test_analog_in_out_set_10ma(void)
{
    return test_analog_in_out_set_and_read_state(10);
}
void test_analog_in_out_set_11ma(void)
{
    return test_analog_in_out_set_and_read_state(11);
}
void test_analog_in_out_set_12ma(void)
{
    return test_analog_in_out_set_and_read_state(12);
}
void test_analog_in_out_set_13ma(void)
{
    return test_analog_in_out_set_and_read_state(13);
}
void test_analog_in_out_set_14ma(void)
{
    return test_analog_in_out_set_and_read_state(14);
}
void test_analog_in_out_set_15ma(void)
{
    return test_analog_in_out_set_and_read_state(15);
}
void test_analog_in_out_set_16ma(void)
{
    return test_analog_in_out_set_and_read_state(16);
}
void test_analog_in_out_set_17ma(void)
{
    return test_analog_in_out_set_and_read_state(17);
}
void test_analog_in_out_set_18ma(void)
{
    return test_analog_in_out_set_and_read_state(18);
}
void test_analog_in_out_set_19ma(void)
{
    return test_analog_in_out_set_and_read_state(19);
}
void test_analog_in_out_set_20ma(void)
{
    return test_analog_in_out_set_and_read_state(20);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("ANALOG IN OUT set and read state (04 mA)", test_analog_in_out_set_4ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (05 mA)", test_analog_in_out_set_5ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (06 mA)", test_analog_in_out_set_6ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (07 mA)", test_analog_in_out_set_7ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (08 mA)", test_analog_in_out_set_8ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (09 mA)", test_analog_in_out_set_9ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (10 mA)", test_analog_in_out_set_10ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (11 mA)", test_analog_in_out_set_11ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (12 mA)", test_analog_in_out_set_12ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (13 mA)", test_analog_in_out_set_13ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (14 mA)", test_analog_in_out_set_14ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (15 mA)", test_analog_in_out_set_15ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (16 mA)", test_analog_in_out_set_16ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (17 mA)", test_analog_in_out_set_17ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (18 mA)", test_analog_in_out_set_18ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (19 mA)", test_analog_in_out_set_19ma, greentea_failure_handler),
    Case("ANALOG IN OUT set and read state (20 mA)", test_analog_in_out_set_20ma, greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(150, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
