/* mbed Microcontroller Library
 * Copyright (c) 2019-2019 ARM Limited
 *
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#ifdef TARGET_RENESAS
#error [NOT_SUPPORTED] Cortex-A target not supported for this test
#else

using namespace utest::v1;

#ifdef MBED_CONF_RTOS_PRESENT
extern osThreadAttr_t _main_thread_attr;
#endif
extern uint32_t mbed_stack_isr_size;

#define EXPECTED_ISR_STACK_SIZE                  (MBED_CONF_TARGET_BOOT_STACK_SIZE)

#define EXPECTED_MAIN_THREAD_STACK_SIZE          (MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE)

#define EXPECTED_USER_THREAD_DEFAULT_STACK_SIZE  (MBED_CONF_RTOS_THREAD_STACK_SIZE)

#if ((MBED_RAM_SIZE - MBED_CONF_TARGET_BOOT_STACK_SIZE) <= (EXPECTED_MAIN_THREAD_STACK_SIZE + EXPECTED_ISR_STACK_SIZE))
#error [NOT_SUPPORTED] Insufficient stack for staci_size_unification tests
#endif


/* Test sizes of ISR stack, main thread stack, default user thread stack.
 *
 * On some platforms with lower RAM size (e.g. NUCLEO_F070RB - 16 KB RAM) it is impossible
 * to create thread with default stack size to check its size, that is why we will
 * check only macro which specifies default user thread stack.
 *
 */
void stack_size_unification_test()
{
    TEST_ASSERT_EQUAL(EXPECTED_ISR_STACK_SIZE, mbed_stack_isr_size);
#ifdef MBED_CONF_RTOS_PRESENT
    TEST_ASSERT_EQUAL(EXPECTED_MAIN_THREAD_STACK_SIZE, _main_thread_attr.stack_size);
    TEST_ASSERT_EQUAL(EXPECTED_USER_THREAD_DEFAULT_STACK_SIZE, OS_STACK_SIZE);
#endif
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Stack size unification test", stack_size_unification_test)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // TARGET_RENESAS
