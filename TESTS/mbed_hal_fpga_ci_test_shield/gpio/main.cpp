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

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();
MbedTester tester(form_factor, restricted);

void gpio_inout_test(PinName pin)
{
    gpio_t gpio;
    gpio_init_inout(&gpio, pin, PIN_OUTPUT, PullNone, 0);
    TEST_ASSERT_EQUAL(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    /* Test GPIO output */

    gpio_write(&gpio, 1);
    TEST_ASSERT_EQUAL(1, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    gpio_write(&gpio, 0);
    TEST_ASSERT_EQUAL(0, tester.gpio_read(MbedTester::LogicalPinGPIO0));

    gpio_dir(&gpio, PIN_INPUT);

    /* Test GPIO input */

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_EQUAL(0, gpio_read(&gpio));

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_EQUAL(1, gpio_read(&gpio));

    /* Set gpio back to Hi-Z */

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);
}

void gpio_inout_test()
{
    for (int i = 0; i < form_factor->count; i++) {
        const PinName test_pin = form_factor->pins[i];
        if (test_pin == NC) {
            continue;
        }
        if (pinmap_list_has_pin(restricted, test_pin)) {
            printf("Skipping gpio pin %s (%i)\r\n", pinmap_ff_default_pin_to_string(test_pin), test_pin);
            continue;
        }
        tester.pin_map_reset();
        tester.pin_map_set(test_pin, MbedTester::LogicalPinGPIO0);

        printf("GPIO test on pin %s (%i)\r\n", pinmap_ff_default_pin_to_string(test_pin), test_pin);
        gpio_inout_test(test_pin);
    }
}

utest::v1::status_t setup(const Case *const source, const size_t index_of_case)
{
    tester.reset();
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t teardown(const Case *const source, const size_t passed, const size_t failed,
                             const failure_t reason)
{
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

Case cases[] = {
    Case("GPIO - inout", setup, gpio_inout_test, teardown),
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
