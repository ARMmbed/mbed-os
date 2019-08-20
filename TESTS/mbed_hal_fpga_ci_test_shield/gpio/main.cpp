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

#if !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"

using namespace utest::v1;

#include "MbedTester.h"
#include "pinmap.h"
#include "test_utils.h"

// This delay is used when reading a floating input that has an internal pull-up
// or pull-down resistor. The voltage response is much slower when the input
// is not driven externally.
#define HI_Z_READ_DELAY_US 5

MbedTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

/* Test basic input & output operations.
 *
 * Given a GPIO instance initialized with a generic gpio_init() function,
 * when basic input and output operations are performed,
 * then all operations succeed.
 */
void test_basic_input_output(PinName pin)
{
    // Reset everything and set all tester pins to hi-Z.
    tester.reset();

    // Map pins for test.
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);

    // Select GPIO0.
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    // Initialize GPIO pin with a generic init fun.
    gpio_t gpio;
    // Test gpio_is_connected() returned value.
    gpio_init(&gpio, NC);
    TEST_ASSERT_EQUAL_INT(0, gpio_is_connected(&gpio));
    gpio_init(&gpio, pin);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));

    // Some targets don't support input pull mode.
#if !defined(TARGET_NANO100) &&         \
    !defined(TARGET_NUC472) &&          \
    !defined(TARGET_M451)
    // Test GPIO used as an input.
    gpio_dir(&gpio, PIN_INPUT);

    // Test input, pull-up mode.
    gpio_mode(&gpio, PullUp);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio)); // hi-Z, pulled up
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio)); // hi-Z, pulled up

    // Test input, pull-down mode.
    gpio_mode(&gpio, PullDown);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio)); // hi-Z, pulled down
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio)); // hi-Z, pulled down

    // Test input, pull-none mode.
    gpio_mode(&gpio, PullNone);
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio));
#endif

    // Test GPIO used as an output.
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    gpio_dir(&gpio, PIN_OUTPUT);
    gpio_write(&gpio, 0);
    TEST_ASSERT_EQUAL_INT(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));
    gpio_write(&gpio, 1);
    TEST_ASSERT_EQUAL_INT(1, tester.gpio_read(MbedTester::LogicalPinGPIO0));
    gpio_write(&gpio, 0);
    TEST_ASSERT_EQUAL_INT(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));
}

/* Test explicit input initialization.
 *
 * Given a GPIO instance,
 * when additional parameters are passed to the input init function,
 * then the GPIO is correctly initialized as an input.
 */
void test_explicit_input(PinName pin)
{
    // Reset everything and set all tester pins to hi-Z.
    tester.reset();

    // Map pins for test.
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);

    // Select GPIO0.
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    gpio_t gpio;

    // Initialize GPIO pin as an input, pull-up mode.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_in_ex(&gpio, pin, PullUp);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio)); // hi-Z, pulled up

    // Initialize GPIO pin as an input, pull-down mode.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_in_ex(&gpio, pin, PullDown);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio)); // hi-Z, pulled down

    // Initialize GPIO pin as an input, pull-up mode.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_inout(&gpio, pin, PIN_INPUT, PullUp, 0);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(1, gpio_read(&gpio)); // hi-Z, pulled up

    // Initialize GPIO pin as an input, pull-down mode.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_inout(&gpio, pin, PIN_INPUT, PullDown, 0);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
    wait_us(HI_Z_READ_DELAY_US);
    TEST_ASSERT_EQUAL_INT(0, gpio_read(&gpio)); // hi-Z, pulled down
}

/* Test explicit output initialization.
 *
 * Given a GPIO instance,
 * when additional parameters are passed to the output init function,
 * then the GPIO is correctly initialized as an output.
 */
void test_explicit_output(PinName pin)
{
    // Reset everything and set all tester pins to hi-Z.
    tester.reset();

    // Map pins for test.
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);

    // Select GPIO0.
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    gpio_t gpio;

    // Initialize GPIO pin as an output, output value = 0.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_out(&gpio, pin);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    TEST_ASSERT_EQUAL_INT(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    // Initialize GPIO pin as an output, output value = 1.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_out_ex(&gpio, pin, 1);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    TEST_ASSERT_EQUAL_INT(1, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    // Initialize GPIO pin as an output, output value = 0.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_out_ex(&gpio, pin, 0);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    TEST_ASSERT_EQUAL_INT(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    // Initialize GPIO pin as an output, output value = 1.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_inout(&gpio, pin, PIN_OUTPUT, PullNone, 1);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    TEST_ASSERT_EQUAL_INT(1, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    // Initialize GPIO pin as an output, output value = 0.
    memset(&gpio, 0, sizeof gpio);
    gpio_init_inout(&gpio, pin, PIN_OUTPUT, PullNone, 0);
    TEST_ASSERT_NOT_EQUAL(0, gpio_is_connected(&gpio));
    TEST_ASSERT_EQUAL_INT(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));
}

Case cases[] = {
    Case("generic init, input & output", all_ports<GPIOPort, DefaultFormFactor, test_basic_input_output>),
    // Some targets don't support input pull mode.
#if !defined(TARGET_NANO100) &&         \
    !defined(TARGET_NUC472) &&          \
    !defined(TARGET_M451)
    Case("explicit init, input", all_ports<GPIOPort, DefaultFormFactor, test_explicit_input>),
#endif
    Case("explicit init, output", all_ports<GPIOPort, DefaultFormFactor, test_explicit_output>),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !COMPONENT_FPGA_CI_TEST_SHIELD */
