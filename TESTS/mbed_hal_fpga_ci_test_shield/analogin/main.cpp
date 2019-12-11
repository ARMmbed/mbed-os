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
#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "pinmap.h"
#include "hal/static_pinmap.h"
#include "test_utils.h"
#include "MbedTester.h"
#include "analogin_fpga_test.h"

using namespace utest::v1;

#define analogin_debug_printf(...)

#define DELTA_FLOAT                     (0.1f)       // 10%
#define DELTA_U16                       (2*3277)     // 10%

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();

MbedTester tester(form_factor, restricted);

void fpga_analogin_init_test(PinName pin)
{
    analogin_t analogin;

    analogin_init(&analogin, pin);
    analogin_free(&analogin);
}

template<bool init_direct>
void fpga_analogin_test(PinName pin)
{
    tester.reset();
    tester.pin_map_set(pin, MbedTester::LogicalPinGPIO0);
    tester.select_peripheral(MbedTester::PeripheralGPIO);

    /* Test analog input */

    analogin_t analogin;

    if (init_direct) {
        const PinMap pinmap = get_analogin_pinmap(pin);
        analogin_init_direct(&analogin, &pinmap);
    } else {
        analogin_init(&analogin, pin);
    }

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 1, true);
    TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, 1.0f, analogin_read(&analogin));
    TEST_ASSERT_UINT16_WITHIN(DELTA_U16, 65535, analogin_read_u16(&analogin));

    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, true);
    TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, 0.0f, analogin_read(&analogin));
    TEST_ASSERT_UINT16_WITHIN(DELTA_U16, 0, analogin_read_u16(&analogin));

    /* Set gpio back to Hi-Z */
    tester.gpio_write(MbedTester::LogicalPinGPIO0, 0, false);

    analogin_free(&analogin);
}

Case cases[] = {
    // This will be run for all pins
    Case("AnalogIn - init test", all_ports<AnaloginPort, DefaultFormFactor, fpga_analogin_init_test>),
    // This will be run for single pin
    Case("AnalogIn - read test", all_ports<AnaloginPort, DefaultFormFactor, fpga_analogin_test<false>>),
    Case("AnalogIn (direct init) - read test", all_ports<AnaloginPort, DefaultFormFactor, fpga_analogin_test<true>>),
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
