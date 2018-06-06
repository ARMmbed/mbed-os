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

#define DELAY_MS 50
#define DELTA_MS 5
#define RESTART_DELAY_MS 10
#define DELAY2_MS 30

#if RESTART_DELAY_MS >= DELAY_MS
#error invalid RESTART_DELAY_MS value
#endif

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

class Stopwatch: public Timer {
private:
    Semaphore _sem;

public:
    Stopwatch() :
            Timer(), _sem(1)
    {
    }

    ~Stopwatch()
    {
    }

    void start(void)
    {
        _sem.wait(0);
        Timer::start();
    }

    void stop(void)
    {
        Timer::stop();
        _sem.release();
    }

    int32_t wait_until_stopped(uint32_t millisec = osWaitForever)
    {
        core_util_critical_section_enter();
        int running = _running;
        core_util_critical_section_exit();
        if (!running) {
            return 1;
        }
        return _sem.wait(millisec);
    }
};

void sem_callback(Semaphore *sem)
{
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
void error(const char* format, ...)
{
    (void) format;
}

mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number) 
{
    return MBED_SUCCESS;
}
#endif

/** Test one-shot not restarted when elapsed
 *
 * Given a one-shot RtosTimer
 * When the timer is started
 *     and given time elapses
 * Then timer stops
 *     and elapsed time matches given delay
 *     and timer stays stopped
 */
void test_oneshot_not_restarted()
{
    Stopwatch stopwatch;
    RtosTimer rtostimer(mbed::callback(&stopwatch, &Stopwatch::stop), osTimerOnce);

    stopwatch.start();
    osStatus status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    int32_t slots = stopwatch.wait_until_stopped();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(DELTA_MS, DELAY_MS, stopwatch.read_ms());
    stopwatch.start();

    slots = stopwatch.wait_until_stopped(DELAY_MS + DELTA_MS);
    TEST_ASSERT_EQUAL(0, slots);
    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test periodic repeats continuously
 *
 * Given a periodic RtosTimer
 * When timer is started
 *     and given time elapses
 * Then timer repeats its operation
 * When timer is stopped
 * Then timer stops operation
 */
void test_periodic_repeats()
{
    Stopwatch stopwatch;
    RtosTimer rtostimer(mbed::callback(&stopwatch, &Stopwatch::stop), osTimerPeriodic);

    stopwatch.start();
    osStatus status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    int32_t slots = stopwatch.wait_until_stopped();
    int t1 = stopwatch.read_ms();
    stopwatch.reset();
    stopwatch.start();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(DELTA_MS, DELAY_MS, t1);

    slots = stopwatch.wait_until_stopped();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(DELTA_MS, DELAY_MS, stopwatch.read_ms());
    stopwatch.start();

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osOK, status);

    slots = stopwatch.wait_until_stopped(DELAY_MS + DELTA_MS);
    TEST_ASSERT_EQUAL(0, slots);
    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test timer can be started again
 *
 * Given a one-shot Rtosimer
 * When the timer is started
 *     and given time elapses
 * Then timer stops
 * When the timer is started again
 *     and given time elapses
 * Then timer stops again
 */
void test_start_again()
{
    Semaphore sem(0, 1);
    RtosTimer rtostimer(mbed::callback(sem_callback, &sem), osTimerOnce);

    osStatus status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    int32_t slots = sem.wait(DELAY_MS + DELTA_MS);
    TEST_ASSERT_EQUAL(1, slots);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);

    status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    slots = sem.wait(DELAY_MS + DELTA_MS);
    TEST_ASSERT_EQUAL(1, slots);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test timer restart updates delay
 *
 * Given a one-shot RtosTimer
 * When the timer is started
 *     and @a start is called again with a different delay before given time elapses
 *     and updated delay elapses
 * Then timer stops
 *     and time elapsed since the second @a start call matches updated delay
 */
void test_restart_updates_delay()
{
    Stopwatch stopwatch;
    RtosTimer rtostimer(mbed::callback(&stopwatch, &Stopwatch::stop), osTimerOnce);

    stopwatch.start();
    osStatus status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    int32_t slots = stopwatch.wait_until_stopped(RESTART_DELAY_MS);
    TEST_ASSERT_EQUAL(0, slots);

    stopwatch.reset();
    stopwatch.start();
    status = rtostimer.start(DELAY2_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    slots = stopwatch.wait_until_stopped();
    TEST_ASSERT_EQUAL(1, slots);
    TEST_ASSERT_INT_WITHIN(DELTA_MS, DELAY2_MS, stopwatch.read_ms());

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test timer is created in stopped state
 *
 * Given a one-shot RtosTimer
 * When the timer has not been started
 * Then the timer is stopped
 */
void test_created_stopped()
{
    RtosTimer rtostimer(mbed::callback(sem_callback, (Semaphore *) NULL), osTimerOnce);
    osStatus status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test one-shot can be stopped
 *
 * Given a one-shot RtosTimer
 * When the timer is started
 *     and timer is stopped while still running
 * Then timer stops operation
 */
void test_stop()
{
    Semaphore sem(0, 1);
    RtosTimer rtostimer(mbed::callback(sem_callback, &sem), osTimerOnce);

    osStatus status = rtostimer.start(DELAY_MS);
    TEST_ASSERT_EQUAL(osOK, status);

    int32_t slots = sem.wait(RESTART_DELAY_MS);
    TEST_ASSERT_EQUAL(0, slots);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osOK, status);

    slots = sem.wait(DELAY_MS + DELTA_MS);
    TEST_ASSERT_EQUAL(0, slots);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

/** Test timer started with infinite delay
 *
 * Given a one-shot RtosTimer
 * When the timer is started with @a osWaitForever delay
 * Then @a start return status is @a osOK
 */
void test_wait_forever()
{
    RtosTimer rtostimer(mbed::callback(sem_callback, (Semaphore *) NULL), osTimerOnce);

    osStatus status = rtostimer.start(osWaitForever);
    TEST_ASSERT_EQUAL(osOK, status);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osOK, status);
}

/** Test timer started with zero delay
 *
 * Given a one-shot RtosTimer
 * When the timer is started with 0 delay
 * Then @a start return status is @a osErrorParameter
 */
void test_no_wait()
{
    RtosTimer rtostimer(mbed::callback(sem_callback, (Semaphore *) NULL), osTimerOnce);

    osStatus status = rtostimer.start(0);
    TEST_ASSERT_EQUAL(osErrorParameter, status);

    status = rtostimer.stop();
    TEST_ASSERT_EQUAL(osErrorResource, status);
}

void rtostimer_isr_call(RtosTimer *rtostimer)
{
    osStatus status = rtostimer->start(DELAY_MS);
    TEST_ASSERT_EQUAL(osErrorISR, status);

    status = rtostimer->stop();
    TEST_ASSERT_EQUAL(osErrorISR, status);
}

/** Test timer method calls from an ISR fail
 *
 * Given a one-shot RtosTimer
 * When a timer method is called from an ISR
 * Then method return status is @a osErrorISR
 */
void test_isr_calls_fail()
{
    RtosTimer rtostimer(mbed::callback(sem_callback, (Semaphore *) NULL), osTimerOnce);

    Ticker ticker;
    ticker.attach(mbed::callback(rtostimer_isr_call, &rtostimer), (float) DELAY_MS / 1000.0);

    wait_ms(DELAY_MS + DELTA_MS);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("One-shot not restarted when elapsed", test_oneshot_not_restarted),
    Case("Periodic repeats continuously", test_periodic_repeats),
    Case("Stopped timer can be started again", test_start_again),
    Case("Restart changes timeout", test_restart_updates_delay),
    Case("Timer can be stopped", test_stop),
    Case("Timer is created in stopped state", test_created_stopped),
    Case("Timer started with infinite delay", test_wait_forever),
    Case("Timer started with zero delay", test_no_wait),
    Case("Calls from ISR fail", test_isr_calls_fail)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
