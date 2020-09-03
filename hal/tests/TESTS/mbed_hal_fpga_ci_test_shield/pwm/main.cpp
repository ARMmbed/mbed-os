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

#if !DEVICE_PWMOUT
#error [NOT_SUPPORTED] PWM not supported for this target
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
#include "hal/static_pinmap.h"
#include "test_utils.h"
#include "pwm_fpga_test.h"

using namespace utest::v1;

#define pwm_debug_printf(...)

typedef enum {
    PERIOD_WRITE,
    PERIOD_MS_WRITE,
    PERIOD_US_WRITE,
    PERIOD_PULSEWIDTH,
    PERIOD_PULSEWIDTH_MS,
    PERIOD_PULSEWIDTH_US
} pwm_api_test_t;

#define NUM_OF_PERIODS                  10
#define US_PER_SEC                      1000000
#define US_PER_MS                       1000
#define MS_PER_SEC                      1000

#define DELTA_FACTOR                    20 // 5% delta

#define PERIOD_US(PERIOD_MS) (((PERIOD_MS) * US_PER_MS))
#define PERIOD_FLOAT(PERIOD_MS) (((float)(PERIOD_MS) / US_PER_MS))
#define FILL_FLOAT(PRC) ((float)(PRC) / 100)
#define PULSE_HIGH_US(PERIOD_US, PRC) ((uint32_t)((PERIOD_US) * FILL_FLOAT(PRC)))
#define PULSE_LOW_US(PERIOD_US, PRC) ((uint32_t)((PERIOD_US) * (1.0f - FILL_FLOAT(PRC))))


MbedTester tester(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());


void fpga_pwm_init_free(PinName pin)
{
    pwmout_t pwm_out;

    pwm_debug_printf("PWM init/free test on pin=%s (%i)\r\n", pinmap_ff_default_pin_to_string(pin), pin);

    pwmout_init(&pwm_out, pin);
    pwmout_free(&pwm_out);
}


void fpga_pwm_period_fill_test(PinName pin, uint32_t period_ms, uint32_t fill_prc, pwm_api_test_t api_test, bool init_direct)
{
    pwm_debug_printf("PWM test on pin = %s (%i)\r\n", pinmap_ff_default_pin_to_string(pin), pin);
    pwm_debug_printf("Testing period = %lu ms, duty-cycle = %lu %%\r\n", period_ms, fill_prc);
    pwm_debug_printf("Testing APIs = %d\r\n", (int)api_test);

    tester.reset();
    MbedTester::LogicalPin logical_pin = (MbedTester::LogicalPin)(MbedTester::LogicalPinIOMetrics0);
    tester.pin_map_set(pin, logical_pin);

    pwmout_t pwm_out;

    if (init_direct) {
        const PinMap pinmap = get_pwm_pinmap(pin);
        pwmout_init_direct(&pwm_out, &pinmap);
    } else {
        pwmout_init(&pwm_out, pin);
    }

    core_util_critical_section_enter();

    switch (api_test) {
        case PERIOD_WRITE:
            pwmout_period(&pwm_out, PERIOD_FLOAT(period_ms));
            pwmout_write(&pwm_out, FILL_FLOAT(fill_prc));
            break;

        case PERIOD_MS_WRITE:
            pwmout_period_ms(&pwm_out, (int)period_ms);
            pwmout_write(&pwm_out, FILL_FLOAT(fill_prc));
            break;

        case PERIOD_US_WRITE:
            pwmout_period_us(&pwm_out, PERIOD_US(period_ms));
            pwmout_write(&pwm_out, FILL_FLOAT(fill_prc));
            break;

        case PERIOD_PULSEWIDTH:
            pwmout_period(&pwm_out, PERIOD_FLOAT(period_ms));
            pwmout_pulsewidth(&pwm_out, (float)PULSE_HIGH_US(PERIOD_US(period_ms), fill_prc) / US_PER_SEC);
            break;

        case PERIOD_PULSEWIDTH_MS:
            pwmout_period(&pwm_out, PERIOD_FLOAT(period_ms));
            pwmout_pulsewidth_ms(&pwm_out, (int)PULSE_HIGH_US(PERIOD_US(period_ms), fill_prc) / MS_PER_SEC);
            break;

        case PERIOD_PULSEWIDTH_US:
            pwmout_period(&pwm_out, PERIOD_FLOAT(period_ms));
            pwmout_pulsewidth_us(&pwm_out, (int)PULSE_HIGH_US(PERIOD_US(period_ms), fill_prc));
            break;
    }

    // wait_us is safe to call as this test disable the IRQs on execution.
    wait_us(PERIOD_US(period_ms));

    tester.io_metrics_start();

    wait_us(NUM_OF_PERIODS * PERIOD_US(period_ms));

    tester.io_metrics_stop();
    core_util_critical_section_exit();

    const uint32_t expected_low_pulse_us = PULSE_LOW_US(PERIOD_US(period_ms), fill_prc);
    const uint32_t expected_high_pulse_us = PULSE_HIGH_US(PERIOD_US(period_ms), fill_prc);
    const uint32_t delta_low_pulse = (expected_low_pulse_us / DELTA_FACTOR);
    const uint32_t delta_high_pulse = (expected_high_pulse_us / DELTA_FACTOR);

    pwm_debug_printf("Minimum pulse low %lu us\r\n", tester.io_metrics_min_pulse_low(logical_pin) / 100);
    pwm_debug_printf("Minimum pulse high %lu us\r\n", tester.io_metrics_min_pulse_high(logical_pin) / 100);
    pwm_debug_printf("Maximum pulse low %lu us\r\n", tester.io_metrics_max_pulse_low(logical_pin) / 100);
    pwm_debug_printf("Maximum pulse high %lu us\r\n", tester.io_metrics_max_pulse_high(logical_pin) / 100);
    pwm_debug_printf("Rising edges %lu\r\n", tester.io_metrics_rising_edges(logical_pin));
    pwm_debug_printf("Falling edges %lu\r\n", tester.io_metrics_falling_edges(logical_pin));

    TEST_ASSERT_FLOAT_WITHIN(FILL_FLOAT(fill_prc) / DELTA_FACTOR, FILL_FLOAT(fill_prc), pwmout_read(&pwm_out));

    TEST_ASSERT_UINT32_WITHIN(delta_low_pulse, expected_low_pulse_us, tester.io_metrics_min_pulse_low(logical_pin) / 100);
    TEST_ASSERT_UINT32_WITHIN(delta_low_pulse, expected_low_pulse_us, tester.io_metrics_max_pulse_low(logical_pin) / 100);
    TEST_ASSERT_UINT32_WITHIN(delta_high_pulse, expected_high_pulse_us, tester.io_metrics_min_pulse_high(logical_pin) / 100);
    TEST_ASSERT_UINT32_WITHIN(delta_high_pulse, expected_high_pulse_us, tester.io_metrics_max_pulse_high(logical_pin) / 100);

    TEST_ASSERT_UINT32_WITHIN(1, NUM_OF_PERIODS, tester.io_metrics_rising_edges(logical_pin));
    TEST_ASSERT_UINT32_WITHIN(1, NUM_OF_PERIODS, tester.io_metrics_falling_edges(logical_pin));

    pwmout_free(&pwm_out);
}

template<uint32_t period_ms, uint32_t fill_prc, pwm_api_test_t api_test, bool init_direct>
void fpga_pwm_period_fill_test(PinName pin)
{
    fpga_pwm_period_fill_test(pin, period_ms, fill_prc, api_test, init_direct);
}


Case cases[] = {
    // This will be run for all pins
    Case("PWM - init/free test", all_ports<PWMPort, DefaultFormFactor, fpga_pwm_init_free>),

    // This will be run for single pin
    Case("PWM - period: 10 ms, fill: 10%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_WRITE, false> >),
    Case("PWM (direct init) - period: 10 ms, fill: 10%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_WRITE, true> >),

    Case("PWM - period: 10 ms, fill: 10%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 10%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 10%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 10 ms, fill: 10%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 10 ms, fill: 10%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 10, PERIOD_PULSEWIDTH_US, false> >),

    Case("PWM - period: 10 ms, fill: 50%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 50%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 50%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 50%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 10 ms, fill: 50%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 10 ms, fill: 50%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 50, PERIOD_PULSEWIDTH_US, false> >),

    Case("PWM - period: 10 ms, fill: 90%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 90%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 90%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 10 ms, fill: 90%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 10 ms, fill: 90%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 10 ms, fill: 90%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<10, 90, PERIOD_PULSEWIDTH_US, false> >),

    Case("PWM - period: 30 ms, fill: 10%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 10%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 10%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 10%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 30 ms, fill: 10%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 30 ms, fill: 10%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 10, PERIOD_PULSEWIDTH_US, false> >),

    Case("PWM - period: 30 ms, fill: 50%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 50%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 50%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 50%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 30 ms, fill: 50%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 30 ms, fill: 50%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 50, PERIOD_PULSEWIDTH_US, false> >),

    Case("PWM - period: 30 ms, fill: 90%, api: period/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 90%, api: period_ms/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_MS_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 90%, api: period_us/write", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_US_WRITE, false> >),
    Case("PWM - period: 30 ms, fill: 90%, api: period/pulse_width", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_PULSEWIDTH, false> >),
    Case("PWM - period: 30 ms, fill: 90%, api: period/pulse_width_ms", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_PULSEWIDTH_MS, false> >),
    Case("PWM - period: 30 ms, fill: 90%, api: period/pulse_width_us", one_peripheral<PWMPort, DefaultFormFactor, fpga_pwm_period_fill_test<30, 90, PERIOD_PULSEWIDTH_US, false> >)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
#ifdef FPGA_FORCE_ALL_PORTS
    GREENTEA_SETUP(300, "default_auto");
#else
    GREENTEA_SETUP(120, "default_auto");
#endif
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif /* !DEVICE_PWMOUT */
