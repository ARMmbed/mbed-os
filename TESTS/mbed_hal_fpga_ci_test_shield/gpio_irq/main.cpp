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

#if !DEVICE_INTERRUPTIN
#error [NOT_SUPPORTED] test not supported
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"

using namespace utest::v1;

#include "mbed.h"
#include "MbedTester.h"
#include "pinmap.h"

static uint32_t call_counter;
void test_gpio_irq_handler(uint32_t id, gpio_irq_event event)
{
    call_counter++;
}

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();
MbedTester tester(form_factor, restricted);

#define WAIT() wait_us(10)


void gpio_irq_test(PinName pin)
{
    gpio_t gpio;
    // configure pin as input
    gpio_init_in(&gpio, pin);

    gpio_irq_t gpio_irq;
    uint32_t id = 123;
    gpio_irq_init(&gpio_irq, pin, test_gpio_irq_handler, id);

    gpio_irq_set(&gpio_irq, IRQ_RISE, true);
    gpio_irq_enable(&gpio_irq);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();

    // test irq on rising edge
    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    gpio_irq_disable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    gpio_irq_enable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    // test irq on both rising and falling edge
    gpio_irq_set(&gpio_irq, IRQ_FALL, true);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(3, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(4, call_counter);

    gpio_irq_disable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    gpio_irq_enable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(3, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(4, call_counter);

    // test irq on falling edge
    gpio_irq_set(&gpio_irq, IRQ_RISE, false);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    gpio_irq_disable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(0, call_counter);

    gpio_irq_enable(&gpio_irq);

    call_counter = 0;
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(1, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    WAIT();
    TEST_ASSERT_EQUAL(2, call_counter);


    gpio_irq_free(&gpio_irq);
}


void gpio_irq_test()
{
    for (uint32_t i = 0; i < form_factor->count; i++) {
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

        printf("GPIO irq test on pin %3s (%3i)\r\n", pinmap_ff_default_pin_to_string(test_pin), test_pin);
        gpio_irq_test(test_pin);
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
    Case("GPIO - irq test", setup, gpio_irq_test, teardown)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_INTERRUPTIN */
