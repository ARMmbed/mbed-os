/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !DEVICE_ANALOGIN
#error [NOT_SUPPORTED] Analog in not supported for this target
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "pinmap.h"
#include "test_utils.h"
#include "MbedTester.h"

using namespace utest::v1;

#define analogin_debug_printf(...)

#define DELTA_FLOAT                     0.01f    // 1%
#define DELTA_U16                       655      // 1%

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();

MbedTester tester(form_factor, restricted);

void analogin_init(PinName pin)
{
    analogin_t analogin;

    analogin_init(&analogin, pin);
}

void analogin_test(PinName pin)
{
    tester.reset();
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    /* Test analog input */

    analogin_t analogin;
    analogin_init(&analogin, pin);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, 1.0f, analogin_read(&analogin));
    TEST_ASSERT_UINT16_WITHIN(DELTA_U16, 65535, analogin_read_u16(&analogin));

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, 0.0f, analogin_read(&analogin));
    TEST_ASSERT_UINT16_WITHIN(DELTA_U16, 0, analogin_read_u16(&analogin));

    /* Set gpio back to Hi-Z */
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
}

void analogin_full_test(PinName pin)
{
    /* Test analog input */

    printf("Testing AnalogIn\r\n");

    // Remap pins for test
    tester.reset();

    // Reset tester stats and select GPIO
    tester.peripherals_reset();
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    analogin_t analogin;
    analogin_init(&analogin, pin);

    //enable analog MUX
    tester.set_mux_enable(true);

    //set MUX address
    tester.set_mux_addr(pin);

    float voltage = 0.0;
    bool enable = true;
    uint32_t period = 100;
    uint32_t duty_cycle = 0;
    //enable FPGA system PWM
    tester.set_analog_out(voltage, enable);

    TEST_ASSERT_EQUAL(true, tester.get_pwm_enable());
    TEST_ASSERT_EQUAL(period, tester.get_pwm_period());
    TEST_ASSERT_EQUAL(duty_cycle, tester.get_pwm_cycles_high());
    // test duty cycles 0-100% at 1000ns period
    for (int i = 0; i < 11; i += 1) {
        voltage = (float)i * 0.1f;
        duty_cycle = 10 * i;
        tester.set_analog_out(voltage, enable);
        wait_us(10);
        //read analog input
        //assert pwm duty_cycle is correct based on set analog voltage
        TEST_ASSERT_EQUAL(duty_cycle, tester.get_pwm_cycles_high());
        TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, 0.01f * (float)duty_cycle, analogin_read(&analogin));
    }
    tester.set_pwm_enable(false);
    TEST_ASSERT_EQUAL(false, tester.get_pwm_enable());
    wait_us(10);

    // assert Mbed pin correctly drives AnalogMuxIn
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL(0, tester.sys_pin_read(MbedTester::AnalogMuxIn));
    wait_us(10);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL(1, tester.sys_pin_read(MbedTester::AnalogMuxIn));
    wait_us(10);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL(0, tester.sys_pin_read(MbedTester::AnalogMuxIn));
    wait_us(10);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(10);
    tester.set_mux_enable(false);
    wait_us(10);

    TEST_ASSERT_EQUAL(1, tester.self_test_control_current());//assert control channel still functioning properly
}

Case cases[] = {
    // This will be run for all pins
    Case("AnalogIn - init test", all_ports<AnaloginPort, DefaultFormFactor, analogin_init>),
#if defined(FULL_TEST_SHIELD)
    Case("AnalogIn - full test", all_ports<AnaloginPort, DefaultFormFactor, analogin_full_test>),
#endif

    // This will be run for single pin
    Case("AnalogIn - read test", all_ports<AnaloginPort, DefaultFormFactor, analogin_test>),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_ANALOGIN */
