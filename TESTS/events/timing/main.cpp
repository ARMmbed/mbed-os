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
#include "mbed_events.h"
#include "mbed.h"
#include "rtos.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <cstdlib>
#include <cmath>

using namespace utest::v1;


// Test delay
#ifndef TEST_EVENTS_TIMING_TIME
#define TEST_EVENTS_TIMING_TIME 20000
#endif

#ifndef TEST_EVENTS_TIMING_MEAN
#define TEST_EVENTS_TIMING_MEAN 25
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// Random number generation to skew timing values
float gauss(float mu, float sigma) {
    float x = (float)rand() / ((float)RAND_MAX+1);
    float y = (float)rand() / ((float)RAND_MAX+1);
    float x2pi = x*2.0*M_PI;
    float g2rad = sqrt(-2.0 * log(1.0-y));
    float z = cos(x2pi) * g2rad;
    return mu + z*sigma;
}

float chisq(float sigma) {
    return pow(gauss(0, sqrt(sigma)), 2);
}


Timer timer;
DigitalOut led(LED1);

equeue_sema_t sema;

// Timer timing test
void timer_timing_test() {
    timer.reset();
    timer.start();
    int prev = timer.read_us();

    while (prev < TEST_EVENTS_TIMING_TIME*1000) {
        int next = timer.read_us();
        if (next < prev) {
            printf("backwards drift %d -> %d (%08x -> %08x)\r\n",
                prev, next, prev, next);
        }
        TEST_ASSERT(next >= prev);
        prev = next;
    }
}

// equeue tick timing test
void tick_timing_test() {
    unsigned start = equeue_tick();
    int prev = 0;

    while (prev < TEST_EVENTS_TIMING_TIME) {
        int next = equeue_tick() - start;
        if (next < prev) {
            printf("backwards drift %d -> %d (%08x -> %08x)\r\n",
                prev, next, prev, next);
        }
        TEST_ASSERT(next >= prev);
        prev = next;
    }
}

// equeue semaphore timing test
void semaphore_timing_test() {
    srand(0);
    timer.reset();
    timer.start();

    int err = equeue_sema_create(&sema);
    TEST_ASSERT_EQUAL(0, err);

    while (timer.read_ms() < TEST_EVENTS_TIMING_TIME) {
        int delay = chisq(TEST_EVENTS_TIMING_MEAN);

        int start = timer.read_us();
        equeue_sema_wait(&sema, delay);
        int taken = timer.read_us() - start;

        printf("delay %dms => error %dus\r\n", delay, abs(1000*delay - taken));
        TEST_ASSERT_INT_WITHIN(5000, taken, delay * 1000);

        led = !led;
    }

    equeue_sema_destroy(&sema);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP((number_of_cases+1)*TEST_EVENTS_TIMING_TIME/1000, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

const Case cases[] = {
    Case("Testing accuracy of timer", timer_timing_test),
    Case("Testing accuracy of equeue tick", tick_timing_test),
    Case("Testing accuracy of equeue semaphore", semaphore_timing_test),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}

