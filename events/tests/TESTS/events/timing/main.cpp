/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed_events.h"
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <cstdlib>
#include <cmath>

using namespace utest::v1;

using namespace std::chrono;

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#else

// Test delay
#ifndef TEST_EVENTS_TIMING_TIME
#define TEST_EVENTS_TIMING_TIME 20000
#endif

#ifndef TEST_EVENTS_TIMING_MEAN
#define TEST_EVENTS_TIMING_MEAN 25
#endif

#define TEST_EVENTS_TIMING_TIME_DURATION milliseconds{TEST_EVENTS_TIMING_TIME}
#define TEST_EVENTS_TIMING_MEAN_DURATION milliseconds{TEST_EVENTS_TIMING_MEAN}

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// Random number generation to skew timing values
float gauss(float mu, float sigma)
{
    float x = (float)rand() / ((float)RAND_MAX + 1);
    float y = (float)rand() / ((float)RAND_MAX + 1);
    float x2pi = x * 2.0 * M_PI;
    float g2rad = sqrt(-2.0 * log(1.0 - y));
    float z = cos(x2pi) * g2rad;
    return mu + z * sigma;
}

float chisq(float sigma)
{
    return pow(gauss(0, sqrt(sigma)), 2);
}

milliseconds random_duration(milliseconds sigma)
{
    milliseconds{static_cast<int>(chisq(sigma.count()))};
}

// Macro for test assertions between std::chrono values
#define TEST_ASSERT_DURATION_WITHIN(delta, expected, actual) \
    do { \
        using ct = std::common_type_t<decltype(delta), decltype(expected), decltype(actual)>; \
        TEST_ASSERT_INT_WITHIN(ct(delta).count(), ct(expected).count(), ct(actual).count()); \
    } while (0)

Timer timer;

equeue_sema_t sema;

// Timer timing test
void timer_timing_test()
{
    timer.reset();
    timer.start();
    auto prev = timer.elapsed_time();

    while (prev < TEST_EVENTS_TIMING_TIME_DURATION) {
        const auto next = timer.elapsed_time();
        if (next < prev) {
            printf("backwards drift %lldus -> %lldus (%08llx -> %08llx)\r\n",
                   duration_cast<microseconds>(prev).count(),
                   duration_cast<microseconds>(next).count(),
                   static_cast<uint64_t>(duration_cast<microseconds>(prev).count()),
                   static_cast<uint64_t>(duration_cast<microseconds>(next).count()));
        }
        TEST_ASSERT(next >= prev);
        prev = next;
    }
}

// equeue tick timing test
void tick_timing_test()
{
    // equeue_tick() returns an integral millisecond value
    const auto start = milliseconds{equeue_tick()};
    auto prev = 0us;

    while (prev < TEST_EVENTS_TIMING_TIME_DURATION) {
        const auto next = milliseconds{equeue_tick()} - start;
        if (next < prev) {
            printf("backwards drift %lldus -> %lldus (%08llx -> %08llx)\r\n",
                   duration_cast<microseconds>(prev).count(),
                   duration_cast<microseconds>(next).count(),
                   static_cast<uint64_t>(duration_cast<microseconds>(prev).count()),
                   static_cast<uint64_t>(duration_cast<microseconds>(next).count()));
        }
        TEST_ASSERT(next >= prev);
        prev = next;
    }
}

// equeue semaphore timing test
void semaphore_timing_test()
{
    srand(0);
    timer.reset();
    timer.start();

    const int err = equeue_sema_create(&sema);
    TEST_ASSERT_EQUAL(0, err);

    while (timer.elapsed_time() < TEST_EVENTS_TIMING_TIME_DURATION) {
        const auto delay = random_duration(TEST_EVENTS_TIMING_MEAN_DURATION);

        const auto start = timer.elapsed_time();
        equeue_sema_wait(&sema, duration_cast<milliseconds>(delay).count());
        const auto taken = timer.elapsed_time() - start;

        if (taken < (delay - 5000us) || taken > (delay + 5000us)) {
            printf("delay %lldms => error %lldus\r\n",
                   duration_cast<milliseconds>(delay).count(),
                   abs(duration_cast<microseconds>(delay - taken).count()));
        }

        TEST_ASSERT_DURATION_WITHIN(5000us, taken, delay);
    }

    equeue_sema_destroy(&sema);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP((number_of_cases + 1) * duration_cast<seconds>(TEST_EVENTS_TIMING_TIME_DURATION).count(),
                   "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

const Case cases[] = {
    Case("Testing accuracy of timer", timer_timing_test),
    Case("Testing accuracy of equeue tick", tick_timing_test),
    Case("Testing accuracy of equeue semaphore", semaphore_timing_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
