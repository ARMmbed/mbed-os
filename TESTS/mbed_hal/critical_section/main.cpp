/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "critical_section_test.h"
#include "hal/critical_section_api.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "cmsis.h"
#if defined(TARGET_NRF5x) // for all NRF5x targets
#include "nrf_nvic.h" // for __NRF_NVIC_APP_IRQS_0 / __NRF_NVIC_APP_IRQS_1
#endif

using utest::v1::Case;

bool test_are_interrupts_enabled(void)
{
    // NRF5x targets don't disable interrupts when in critical section, instead they mask application interrupts this is due to BLE stack
    // (BLE to be operational requires some interrupts to be always enabled)
#if defined(TARGET_NRF52)
	// check if APP interrupts are masked for NRF52 boards
	return (((NVIC->ISER[0] & __NRF_NVIC_APP_IRQS_0) != 0) || ((NVIC->ISER[1] & __NRF_NVIC_APP_IRQS_1) != 0));
#elif defined(TARGET_NRF51)
	// check if APP interrupts are masked for other NRF51 boards
	return ((NVIC->ISER[0] & __NRF_NVIC_APP_IRQS_0) != 0);
#else
#if defined(__CORTEX_A9)
    return ((__get_CPSR() & 0x80) == 0);
#else
    return ((__get_PRIMASK() & 0x1) == 0);
#endif
#endif
}


template<int N>
void test_critical_section()
{
    TEST_ASSERT_FALSE(hal_in_critical_section());
    TEST_ASSERT_TRUE(test_are_interrupts_enabled());

    for (int i = 0; i < N; i++) {
        hal_critical_section_enter();
        TEST_ASSERT_TRUE(hal_in_critical_section());
        TEST_ASSERT_FALSE(test_are_interrupts_enabled());
    }

    // assumed to be called once (according API)
    hal_critical_section_exit();

    TEST_ASSERT_FALSE(hal_in_critical_section());
    TEST_ASSERT_TRUE(test_are_interrupts_enabled());
}

Case cases[] = {
    Case("Test critical section single lock", test_critical_section<1>),
    Case("Test critical section nested lock", test_critical_section<10>)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "timing_drift_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
