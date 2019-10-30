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
#include "MbedTester.h"
#include "pinmap.h"
#include "gpio_irq_fpga_test.h"

using namespace utest::v1;

#include "MbedTester.h"
#include "pinmap.h"
#include "test_utils.h"

MbedTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());

static volatile uint32_t call_counter;
void test_gpio_irq_handler(uint32_t id, gpio_irq_event event)
{
    call_counter++;
}

#define WAIT() wait_us(10)

void fpga_gpio_irq_test(PinName pin)
{
    // Reset everything and set all tester pins to hi-Z.
    tester.reset();

    // Map pins for test.
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);

    // Select GPIO0.
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    gpio_t gpio;
    // configure pin as input
    gpio_init_in(&gpio, pin);

    gpio_irq_t gpio_irq;
    uint32_t id = 123;
    TEST_ASSERT_EQUAL(0, gpio_irq_init(&gpio_irq, pin, test_gpio_irq_handler, id));

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

void fpga_gpio_irq_init_free_test(PinName pin)
{
    gpio_t gpio;
    gpio_irq_t gpio_irq;
    gpio_init_in(&gpio, pin);
    TEST_ASSERT_EQUAL(0, gpio_irq_init(&gpio_irq, pin, test_gpio_irq_handler, 123));
    gpio_irq_free(&gpio_irq);
}

Case cases[] = {
    Case("init/free", all_ports<GPIOIRQPort, DefaultFormFactor, fpga_gpio_irq_init_free_test>),
    Case("rising & falling edge", all_ports<GPIOIRQPort, DefaultFormFactor, fpga_gpio_irq_test>),
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

#endif /* !DEVICE_INTERRUPTIN */
