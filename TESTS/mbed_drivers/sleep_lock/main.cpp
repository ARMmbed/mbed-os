
/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if !DEVICE_SLEEP
    #error [NOT_SUPPORTED] Sleep not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"

using namespace utest::v1;

void deep_sleep_lock_lock_test()
{
    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Check basic usage works
        DeepSleepLock lock;
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Check that unlock and lock change can deep sleep as expected
        DeepSleepLock lock;
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
        lock.unlock();
        TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
        lock.lock();
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Check that unlock releases sleep based on count
        DeepSleepLock lock;
        lock.lock();
        lock.lock();
        lock.unlock();
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Check that unbalanced locks do not leave deep sleep locked
        DeepSleepLock lock;
        lock.lock();
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
    }
    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());

}

void timer_lock_test()
{
    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Just creating a timer object does not lock sleep
        Timer timer;
        TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Starting a timer does lock sleep
        Timer timer;
        timer.start();
        TEST_ASSERT_EQUAL(false, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Stopping a timer after starting it allows sleep
        Timer timer;
        timer.start();
        timer.stop();
        TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    }

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Starting a timer multiple times still lets you sleep
        Timer timer;
        timer.start();
        timer.start();
    }
    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());

    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    {
        // Stopping a timer multiple times still lets you sleep
        Timer timer;
        timer.start();
        timer.stop();
        timer.stop();
        TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
    }
    TEST_ASSERT_EQUAL(true, sleep_manager_can_deep_sleep());
}

Case cases[] = {
    Case("DeepSleepLock lock test", deep_sleep_lock_lock_test),
    Case("timer lock test", timer_lock_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
