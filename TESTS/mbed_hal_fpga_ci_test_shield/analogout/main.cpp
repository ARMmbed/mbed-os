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

#if !DEVICE_ANALOGOUT
#error [NOT_SUPPORTED] Analog out not supported for this target
#elif !COMPONENT_FPGA_CI_TEST_SHIELD
#error [NOT_SUPPORTED] FPGA CI Test Shield is needed to run this test
#elif !defined(TARGET_FF_ARDUINO) && !defined(MBED_CONF_TARGET_DEFAULT_FORM_FACTOR)
#error [NOT_SUPPORTED] Test not supported for this form factor
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include <inttypes.h>
#include "mbed.h"

using namespace utest::v1;

#include "MbedTester.h"
#include "pinmap.h"
#include "test_utils.h"

#if !DEVICE_ANALOGOUT
#error [NOT_SUPPORTED] Analog out not supported for this target
#endif

#define analogout_debug_printf(...)

#define DELTA_FLOAT                     0.03f    // 3%

/* Enable for power analysis */
#define DEBUG 0

const PinList *form_factor = pinmap_ff_default_pins();
const PinList *restricted = pinmap_restricted_pins();

MbedTester tester(form_factor, restricted);

void analogout_init_free(PinName pin)
{
    dac_t analogout;

    analogout_debug_printf("Analog output init/free test on pin=%s (%i)\r\n", pinmap_ff_default_pin_to_string(pin), pin);

    analogout_init(&analogout, pin);
    analogout_free(&analogout);
}

void analogout_test(PinName pin)
{
    analogout_debug_printf("Analog input test on pin %s (%i)\r\n", pinmap_ff_default_pin_to_string(pin), pin);

    tester.reset();
    tester.peripherals_reset();

    /* Test analog input */

    dac_t analogout;
    analogout_init(&analogout, pin);

    tester.set_sample_adc(true);//begin ADC sampling on the FPGA

    float anin;
    float i;
    for (i = 0.0f; i < 0.304f; i += 0.0303f) {//0V-1V, float
        analogout_write(&analogout, i);
        anin = tester.get_analog_in();
        TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, (i * 3.3f), anin);
    }

    i = 0.0f;
    for (uint16_t i_d16 = 0; i_d16 < 19851; i_d16 += 1985) {//0V-1V, 16-bit
        analogout_write_u16(&analogout, i_d16);
        anin = tester.get_analog_in();
        TEST_ASSERT_FLOAT_WITHIN(DELTA_FLOAT, (i * 3.3f), anin);
        i += 0.0303f;
    }

    analogout_free(&analogout);

    tester.set_sample_adc(false);//stop ADC sampling on the FPGA

#if DEBUG
    // power analysis
    uint64_t sum;
    uint32_t samples;
    uint64_t cycles;
    tester.get_anin_sum_samples_cycles(0, &sum, &samples, &cycles);
    printf("ANIN0\r\n");
    printf("Sum: %llu\r\n", sum);
    printf("Num power samples: %d\r\n", samples);
    printf("Num power cycles: %llu\r\n", cycles);
    printf("ANIN0 voltage: %.6fV\r\n", tester.get_anin_voltage(0));
#endif
}

Case cases[] = {
    // This will be run for all pins
    Case("Analogout - init test", all_ports<AnalogoutPort, DefaultFormFactor, analogout_init_free>),

    // This will be run for single pin
    Case("Analogout - write/read test", all_ports<AnalogoutPort, DefaultFormFactor, analogout_test>),
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

#endif /* !DEVICE_ANALOGOUT */
