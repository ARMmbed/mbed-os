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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

using namespace utest::v1;

#define TEST_DELAY_MS 50
#define TEST_DELAY2_MS 30
#define TEST_DELAY_MS_DELTA 1
#define TEST_RESTART_DELAY_MS 10

#if TEST_RESTART_DELAY_MS >= TEST_DELAY_MS
#error invalid TEST_RESTART_DELAY_MS value
#endif

void timer_callback(void const *arg)
{
    Semaphore *sem = (Semaphore *) arg;
    sem->release();
}

/* In order to successfully run this test suite when compiled with --profile=debug
 * error() has to be redefined as noop.
 *
 * RtosTimer calls RTX API which uses Event Recorder functionality. When compiled
 * with MBED_TRAP_ERRORS_ENABLED=1 (set in debug profile) EvrRtxTimerError() calls error()
 * which aborts test program.
 */
#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED
void error(const char* format, ...) {
    (void) format;
}
#endif

/** Test one-shot not restarted when elapsed
 *
 * Given a one-shot timer
 * When the timer is started
 *     and given time elapses
 * Then timer stops
 *     and elapsed time matches given delay
 *     and timer stays stopped
 */
void test_oneshot_not_restarted()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    Timer t;
    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    t.start();
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_DELAY_MS + 1);
    t.stop();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(TEST_DELAY_MS_DELTA * 1000, TEST_DELAY_MS * 1000, t.read_us());

    slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(0, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test periodic repeats continuously
 *
 * Given a periodic timer
 * When timer is started
 *     and given time elapses
 * Then timer repeats its operation
 * When timer is stopped
 * Then timer stops operation
 */
void test_periodic_repeats()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerPeriodic);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    Timer t;
    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    t.start();
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_DELAY_MS + 1);
    int t1 = t.read_us();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(TEST_DELAY_MS_DELTA * 1000, TEST_DELAY_MS * 1000, t1);

    slots = sem.wait(TEST_DELAY_MS + 1);
    t.stop();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(TEST_DELAY_MS_DELTA * 1000, TEST_DELAY_MS * 1000, t.read_us() - t1);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osOK, stat);

    slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(0, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test timer can be restarted
 *
 * Given a one-shot timer
 * When the timer is started
 *     and @a start is called again before given time elapses
 *     and given time elapses
 * Then timer stops
 *     and elapsed time is greater than original delay
 */
void test_restart()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    Timer t;
    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    t.start();
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_RESTART_DELAY_MS);
    TEST_ASSERT_EQUAL(0, slots);

    stat = timer.start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, stat);

    slots = sem.wait(TEST_DELAY_MS + 1);
    t.stop();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(TEST_DELAY_MS_DELTA * 1000, (TEST_DELAY_MS + TEST_RESTART_DELAY_MS) * 1000, t.read_us());

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test timer can be started again
 *
 * Given a one-shot timer
 * When the timer is started
 *     and given time elapses
 * Then timer stops
 * When the timer is started again
 *     and given time elapses
 * Then timer stops again
 */
void test_start_again()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(1, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    stat = timer.start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, stat);

    slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(1, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test timer restart updates delay
 *
 * Given a one-shot timer
 * When the timer is started
 *     and @a start is called again with a different delay before given time elapses
 *     and updated delay elapses
 * Then timer stops
 *     and time elapsed since the second @a start call matches updated delay
 */
void test_restart_updates_delay()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_RESTART_DELAY_MS);
    TEST_ASSERT_EQUAL(0, slots);

    Timer t;
    stat = timer.start(TEST_DELAY2_MS);
    t.start();
    TEST_ASSERT_EQUAL(osOK, stat);

    slots = sem.wait(TEST_DELAY2_MS + 1);
    t.stop();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(TEST_DELAY_MS_DELTA * 1000, TEST_DELAY2_MS * 1000, t.read_us());

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test timer is created in stopped state
 *
 * Given a one-shot timer
 * When the timer has not been started
 * Then the timer is stopped
 */
void test_created_stopped()
{
    RtosTimer timer(mbed::callback(timer_callback, (void const *) NULL), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test one-shot can be stopped
 *
 * Given a one-shot timer
 * When the timer is started
 *     and timer is stopped while still running
 * Then timer stops operation
 */
void test_stop()
{
    Semaphore sem(1);
    RtosTimer timer(mbed::callback(timer_callback, (void const *) &sem), osTimerOnce);
    osStatus stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);

    sem.wait(0);
    stat = timer.start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, stat);

    int32_t slots = sem.wait(TEST_RESTART_DELAY_MS);
    TEST_ASSERT_EQUAL(0, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osOK, stat);

    slots = sem.wait(TEST_DELAY_MS + 1);
    TEST_ASSERT_EQUAL(0, slots);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

/** Test timer started with infinite delay
 *
 * Given a one-shot timer
 * When the timer is started with @a osWaitForever delay
 * Then @a start return status is @a osOK
 */
void test_wait_forever()
{
    RtosTimer timer(mbed::callback(timer_callback, (void const *) NULL), osTimerOnce);

    osStatus stat = timer.start(osWaitForever);
    TEST_ASSERT_EQUAL(osOK, stat);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osOK, stat);
}

/** Test timer started with zero delay
 *
 * Given a one-shot timer
 * When the timer is started with 0 delay
 * Then @a start return status is @a osErrorParameter
 */
void test_no_wait()
{
    RtosTimer timer(mbed::callback(timer_callback, (void const *) NULL), osTimerOnce);

    osStatus stat = timer.start(0);
    TEST_ASSERT_EQUAL(osErrorParameter, stat);

    stat = timer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, stat);
}

void timer_isr_call(void const *arg)
{
    RtosTimer *timer = (RtosTimer *) arg;
    osStatus stat = timer->start(TEST_DELAY_MS);
    TEST_ASSERT_EQUAL(osErrorISR, stat);

    stat = timer->stop();
    TEST_ASSERT_EQUAL(osErrorISR, stat);
}

/** Test timer method calls from an ISR fail
 *
 * Given a one-shot timer
 * When a timer method is called from an ISR
 * Then method return status is @a osErrorISR
 */
void test_isr_calls_fail()
{
    RtosTimer timer(mbed::callback(timer_callback, (void const *) NULL), osTimerOnce);

    Ticker ticker;
    ticker.attach(mbed::callback(timer_isr_call, (void const *) &timer), (float) TEST_DELAY_MS / 1000.0);

    wait_ms(TEST_DELAY_MS + 1);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test one-shot not restarted when elapsed", test_oneshot_not_restarted),
    Case("Test periodic repeats continuously", test_periodic_repeats),
    Case("Test timer can be restarted while running", test_restart),
    Case("Test stopped timer can be started again", test_start_again),
    Case("Test restart changes timeout", test_restart_updates_delay),
    Case("Test can be stopped", test_stop),
    Case("Test timer is created in stopped state", test_created_stopped),
    Case("Test timer started with infinite delay", test_wait_forever),
    Case("Test timer started with zero delay", test_no_wait),
    Case("Test calls from ISR fail", test_isr_calls_fail)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
