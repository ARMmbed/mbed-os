/*
 * Copyright (c) 2019 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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
#if !DEVICE_WATCHDOG
#error [NOT_SUPPORTED] Watchdog not supported for this target
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
#include "test_utils.h"
#include "hal/watchdog_api.h"

#define MSG_VALUE_DUMMY "0"
#define CASE_DATA_INVALID 0xffffffffUL
#define CASE_DATA_PHASE2_OK 0xfffffffeUL

#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24

#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_START_CASE "start_case"
#define MSG_KEY_DEVICE_RESET "dev_reset"

#define WATCHDOG_PULSE_PERIOD_US 1000

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

struct testcase_data {
    int index;
    int start_index;
    uint32_t received_data;
};

testcase_data current_case;

/**
 * Get the first pin from the whitelist that is absent form the blacklist.
 *
 * @param whitelist List of pins to choose from
 * @param blacklist List of pins which cannot be used
 * @return The first pin from the whitelist absent from the blacklist.
 */
PinName get_pin_to_restrict(const PinList *whitelist, const PinList *blacklist)
{
    for (uint32_t i = 0; i < whitelist->count; i++) {
        if (!pinmap_list_has_pin(blacklist, whitelist->pins[i])) {
            return whitelist->pins[i];
        }
    }
    return NC;
}

/**
 * Allocate a new PinList, copy the pin_list and add the pin at the end.
 *
 * @param pin_list List of pins to copy
 * @param pin The pin to add at the end of the new PinList
 * @return Pointer to the allocated PinList or NULL on error.
 */
PinList *alloc_extended_pinlist(const PinList *pin_list, PinName pin)
{
    PinName *pins = (PinName *) calloc(pin_list->count + 1, sizeof(PinName));
    if (pins == NULL) {
        return NULL;
    }
    for (uint32_t i = 0; i < pin_list->count; i++) {
        pins[i] = pin_list->pins[i];
    }
    pins[pin_list->count] = pin;
    PinList *new_pinlist = (PinList *) calloc(1, sizeof(PinList));
    if (new_pinlist == NULL) {
        free(pins);
        return NULL;
    }
    new_pinlist->count = pin_list->count + 1;
    new_pinlist->pins = pins;
    return new_pinlist;
}

/**
 * Free a previously allocated PinList.
 *
 * @param pin_list List of pins to free.
 */
void free_pinlist(PinList *pin_list)
{
    if (pin_list) {
        if (pin_list->pins) {
            free((PinName *) pin_list->pins);
        }
        free(pin_list);
    }
}

bool send_reset_notification(testcase_data *tcdata, uint32_t delay_ms)
{
    char msg_value[12];
    int str_len = snprintf(msg_value, sizeof msg_value, "%02x,%08lx", tcdata->start_index + tcdata->index, delay_ms);
    if (str_len < 0) {
        utest_printf("Failed to compose a value string to be sent to host.");
        return false;
    }
    greentea_send_kv(MSG_KEY_DEVICE_RESET, msg_value);
    return true;
}

template<uint32_t timeout_ms>
void fpga_test_watchdog_timeout_accuracy()
{
    watchdog_features_t features = hal_watchdog_get_platform_features();
    if (timeout_ms > features.max_timeout) {
        TEST_IGNORE_MESSAGE("Requested timeout value not supported for this target -- ignoring test case.");
        return;
    }

    PinName watchdog_pulse_pin = get_pin_to_restrict(DefaultFormFactor::pins(), DefaultFormFactor::restricted_pins());
    TEST_ASSERT(watchdog_pulse_pin != NC);
    PinList *blacklist = alloc_extended_pinlist(DefaultFormFactor::restricted_pins(), watchdog_pulse_pin);
    TEST_ASSERT_NOT_NULL(blacklist);
    MbedTester tester(DefaultFormFactor::pins(), blacklist);

    // Phase 2. -- verify the test results.
    // Verify the FPGA time measurement is within given range:
    // 1. The watchdog should trigger at, or after the timeout value.
    // 2. The watchdog should trigger before twice the timeout value.
    if (current_case.received_data != CASE_DATA_INVALID) {
        TEST_ASSERT_EQUAL(CASE_DATA_PHASE2_OK, current_case.received_data);
        current_case.received_data = CASE_DATA_INVALID;
        tester.io_metrics_stop();
        uint32_t num_falling_edges = tester.io_metrics_falling_edges(MbedTester::LogicalPinIOMetrics0);
        uint32_t actual_timeout_ms = 0;
        if (num_falling_edges > 0) {
            actual_timeout_ms = (num_falling_edges - 1) * WATCHDOG_PULSE_PERIOD_US / 1000;
        }
        utest_printf("The FPGA shield measured %lu ms timeout.", actual_timeout_ms);
        TEST_ASSERT(actual_timeout_ms >= timeout_ms);
        TEST_ASSERT(actual_timeout_ms < 2 * timeout_ms);
        free_pinlist(blacklist);
        return;
    }

    // Phase 1. -- run the test code.
    tester.reset();
    tester.pin_map_set(watchdog_pulse_pin, MbedTester::LogicalPinIOMetrics0);
    gpio_t pulse_pin;
    int pulse_pin_value = 1;
    gpio_init_out_ex(&pulse_pin, watchdog_pulse_pin, pulse_pin_value);
    // Init the watchdog and wait for a device reset.
    watchdog_config_t config = { timeout_ms };
    if (send_reset_notification(&current_case, 2 * timeout_ms) == false) {
        TEST_ASSERT_MESSAGE(0, "Dev-host communication error.");
        free_pinlist(blacklist);
        return;
    }
    tester.io_metrics_start();
    TEST_ASSERT_EQUAL(WATCHDOG_STATUS_OK, hal_watchdog_init(&config));
    while (1) {
        pulse_pin_value ^= 1;
        gpio_write(&pulse_pin, pulse_pin_value);
        wait_us(WATCHDOG_PULSE_PERIOD_US / 2);
    }
}

utest::v1::status_t case_setup(const Case *const source, const size_t index_of_case)
{
    current_case.index = index_of_case;
    return utest::v1::greentea_case_setup_handler(source, index_of_case);
}

int testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(90, "watchdog_reset");
    utest::v1::status_t status = utest::v1::greentea_test_setup_handler(number_of_cases);
    if (status != utest::v1::STATUS_CONTINUE) {
        return status;
    }

    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };

    greentea_send_kv(MSG_KEY_DEVICE_READY, MSG_VALUE_DUMMY);
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);

    if (strcmp(key, MSG_KEY_START_CASE) != 0) {
        utest_printf("Invalid message key.\n");
        return utest::v1::STATUS_ABORT;
    }

    int num_args = sscanf(value, "%02x,%08lx", &(current_case.start_index), &(current_case.received_data));
    if (num_args == 0 || num_args == EOF) {
        utest_printf("Invalid data received from host\n");
        return utest::v1::STATUS_ABORT;
    }

    utest_printf("This test suite is composed of %i test cases. Starting at index %i.\n", number_of_cases,
                 current_case.start_index);
    return current_case.start_index;
}

Case cases[] = {
    Case("Timeout accuracy,  200 ms", case_setup, fpga_test_watchdog_timeout_accuracy<200UL>),
    Case("Timeout accuracy,  500 ms", case_setup, fpga_test_watchdog_timeout_accuracy<500UL>),
    Case("Timeout accuracy, 1000 ms", case_setup, fpga_test_watchdog_timeout_accuracy<1000UL>),
    Case("Timeout accuracy, 3000 ms", case_setup, fpga_test_watchdog_timeout_accuracy<3000UL>),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup, cases);

int main()
{
    // Harness will start with a test case index provided by host script.
    return !Harness::run(specification);
}

#endif // !DEVICE_WATCHDOG
