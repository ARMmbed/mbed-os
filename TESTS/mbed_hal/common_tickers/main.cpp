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
#include "ticker_api_tests.h"
#include "hal/us_ticker_api.h"
#include "hal/lp_ticker_api.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#endif

#define FORCE_OVERFLOW_TEST (false)
#define TICKER_INT_VAL 500
#define TICKER_DELTA 10

#define LP_TICKER_OVERFLOW_DELTA 0 // this will allow to detect that ticker counter rollovers to 0
#define US_TICKER_OVERFLOW_DELTA 50

#define TICKER_100_TICKS 100

#define MAX_FUNC_EXEC_TIME_US 20
#define DELTA_FUNC_EXEC_TIME_US 5
#define NUM_OF_CALLS 1000

#define NUM_OF_CYCLES 100000

#define US_TICKER_OV_LIMIT 35000
#define LP_TICKER_OV_LIMIT 4000

using namespace utest::v1;

volatile int intFlag = 0;
const ticker_interface_t* intf;
ticker_irq_handler_type prev_irq_handler;
unsigned int ticker_overflow_delta;

/* Auxiliary function to count ticker ticks elapsed during execution of N cycles of empty while loop.
 * Parameter <step> is used to disable compiler optimisation. */
uint32_t count_ticks(uint32_t cycles, uint32_t step)
{
    register uint32_t reg_cycles = cycles;

    const ticker_info_t* p_ticker_info = intf->get_info();
    const uint32_t max_count = ((1 << p_ticker_info->bits) - 1);

    core_util_critical_section_enter();

    const uint32_t start = intf->read();

    while (reg_cycles  -= step) {
        /* Just wait. */
    }

    const uint32_t stop = intf->read();

    core_util_critical_section_exit();

    /* Handle overflow - overflow protection may not work in this case. */
    uint32_t diff = (start <= stop) ? (stop - start) : (uint32_t)(max_count - start + stop + 1);

    return (diff);
}

/* Since according to the ticker requirements min acceptable counter size is
 * - 12 bits for low power timer - max count = 4095,
 * - 16 bits for high frequency timer - max count = 65535
 * then all test cases must be executed in this time windows.
 * HAL ticker layer handles counter overflow and it is not handled in the target
 * ticker drivers. Ensure we have enough time to execute test case without overflow.
 */
void overflow_protect()
{
    uint32_t time_window;

    if (intf == get_us_ticker_data()->interface) {
        time_window = US_TICKER_OV_LIMIT;
    } else {
        time_window = LP_TICKER_OV_LIMIT;
    }

    const uint32_t ticks_now = intf->read();
    const ticker_info_t* p_ticker_info = intf->get_info();

    const uint32_t max_count = ((1 << p_ticker_info->bits) - 1);

    if ((max_count - ticks_now) > time_window) {
        return;
    }

    while (intf->read() > ticks_now);
}

void ticker_event_handler_stub(const ticker_data_t * const ticker)
{
    if (ticker == get_us_ticker_data()) {
        us_ticker_clear_interrupt();
    } else {
#if DEVICE_LPTICKER
        lp_ticker_clear_interrupt();
#endif
    }

    /* Indicate that ISR has been executed in interrupt context. */
    if (IsIrqMode()) {
        intFlag++;
    }
}

void wait_cycles(volatile unsigned int cycles)
{
    while (cycles--);
}

/* Test that ticker_init can be called multiple times and
 * ticker_init allows the ticker to keep counting and disables the ticker interrupt.
 */
void ticker_init_test()
{
    overflow_protect();

    intFlag = 0;

    intf->init();

    /* Wait a while - let the ticker to count. */
    wait_cycles(10000);

    const uint32_t ticks_start = intf->read();

    intf->set_interrupt(ticks_start + TICKER_INT_VAL);

    /* Re-initialise the ticker. */
    intf->init();

    const uint32_t ticks_after_reinit = intf->read();

    /* Wait long enough to fire ticker interrupt (should not be fired). */
    while (intf->read() < (ticks_start + 2 * TICKER_INT_VAL)) {
        /* Just wait. */
    }

    TEST_ASSERT(intf->read() >= (ticks_start + 2 * TICKER_INT_VAL));
    TEST_ASSERT(ticks_start <= ticks_after_reinit);
    TEST_ASSERT_EQUAL(0, intFlag);
}

/* Test that ticker frequency is non-zero and counter is at least 8 bits */
void ticker_info_test(void)
{
    const ticker_info_t* p_ticker_info = intf->get_info();

    TEST_ASSERT(p_ticker_info->frequency != 0);
    TEST_ASSERT(p_ticker_info->bits >= 8);
}

/* Test that ticker interrupt fires only when the ticker counter increments to the value set by ticker_set_interrupt. */
void ticker_interrupt_test(void)
{
    uint32_t ticker_timeout[] = { 100, 200, 300, 500 };

    overflow_protect();

    for (uint32_t i = 0; i < (sizeof(ticker_timeout) / sizeof(uint32_t)); i++) {
        intFlag = 0;
        const uint32_t tick_count = intf->read();

        /* Set interrupt. Interrupt should be fired when tick count is equal to:
         * tick_count + ticker_timeout[i]. */
        intf->set_interrupt(tick_count + ticker_timeout[i]);

        /* Wait until ticker count reach value: tick_count + ticker_timeout[i] - TICKER_DELTA.
         * Interrupt should not be fired. */
        while (intf->read() < (tick_count + ticker_timeout[i] - TICKER_DELTA)) {
            /* Indicate failure if interrupt has fired earlier. */
            TEST_ASSERT_EQUAL_INT_MESSAGE(0, intFlag, "Interrupt fired too early");
        }

        /* Wait until ticker count reach value: tick_count + ticker_timeout[i] + TICKER_DELTA.
         * Interrupt should be fired after this time. */
        while (intf->read() < (tick_count + ticker_timeout[i] + TICKER_DELTA)) {
            /* Just wait. */
        }

        TEST_ASSERT_EQUAL(1, intFlag);

        /* Wait until ticker count reach value: tick_count + 2 * ticker_timeout[i] + TICKER_DELTA.
         * Interrupt should not be triggered again. */
        while (intf->read() < (tick_count + 2 * ticker_timeout[i] + TICKER_DELTA)) {
            /* Just wait. */
        }

        TEST_ASSERT_EQUAL(1, intFlag);
    }
}

/* Test that ticker interrupt is not triggered when ticker_set_interrupt */
void ticker_past_test(void)
{
    intFlag = 0;

    const uint32_t tick_count = intf->read();

    /* Set interrupt tick count to value in the past.
     * Interrupt should not be fired. */
    intf->set_interrupt(tick_count - TICKER_DELTA);

    wait_cycles(1000);

    TEST_ASSERT_EQUAL(0, intFlag);
}

/* Test that ticker can be rescheduled repeatedly before the handler has been called. */
void ticker_repeat_reschedule_test(void)
{
    overflow_protect();

    intFlag = 0;

    const uint32_t tick_count = intf->read();

    /* Set interrupt. Interrupt should be fired when tick count is equal to:
     * tick_count + TICKER_INT_VAL. */
    intf->set_interrupt(tick_count + TICKER_INT_VAL);

    /* Reschedule interrupt - it should not be fired yet.
     * Re-schedule interrupt. */
    intf->set_interrupt(tick_count + (2 * TICKER_INT_VAL));
    intf->set_interrupt(tick_count + (3 * TICKER_INT_VAL));

    /* Wait until ticker count reach value: tick_count + 3*TICKER_INT_VAL - TICKER_DELTA.
     * Interrupt should not be fired. */
    while (intf->read() < (tick_count + 3 * TICKER_INT_VAL - TICKER_DELTA)) {
        /* Indicate failure if interrupt has fired earlier. */
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, intFlag, "Interrupt fired too early");
    }

    /* Wait until ticker count reach value: tick_count + 3*TICKER_INT_VAL + TICKER_DELTA.
     * Interrupt should be fired after this time. */
    while (intf->read() < (tick_count + 3 * TICKER_INT_VAL + TICKER_DELTA)) {
        /* Just wait. */
    }

    TEST_ASSERT_EQUAL(1, intFlag);
}

/* Test that ticker_fire_interrupt causes and interrupt to get fired immediately. */
void ticker_fire_now_test(void)
{
    intFlag = 0;

    intf->fire_interrupt();

    /* On some platforms set_interrupt function sets interrupt in the nearest feature. */
    wait_cycles(1000);

    TEST_ASSERT_EQUAL(1, intFlag);
}

/* Test that the ticker correctly handles overflow. */
void ticker_overflow_test(void)
{
    const ticker_info_t* p_ticker_info = intf->get_info();

    /* We need to check how long it will take to overflow.
     * We will perform this test only if this time is no longer than 30 sec.
     */
    const uint32_t max_count = (1 << p_ticker_info->bits) - 1;
    const uint32_t required_time_sec = (max_count / p_ticker_info->frequency);

    if (required_time_sec > 30 && !FORCE_OVERFLOW_TEST) {
        TEST_ASSERT_TRUE(true);
        printf("Test has been skipped.\n");
        return;
    }

    intFlag = 0;

    /* Wait for max count. */
    while (intf->read() != (max_count - ticker_overflow_delta)) {
        /* Just wait. */
    }

    /* Now we are near/at the overflow point. Detect rollover. */
    while (intf->read() > ticker_overflow_delta);

    const uint32_t after_overflow = intf->read();

    /* Now we are just after overflow. Wait a while assuming that ticker still counts. */
    while (intf->read() < TICKER_100_TICKS) {
        /* Just wait. */
    }

    const uint32_t next_after_overflow = intf->read();

    /* Check that after the overflow ticker continue count. */
    TEST_ASSERT(after_overflow <= ticker_overflow_delta);
    TEST_ASSERT(next_after_overflow >= TICKER_100_TICKS);
    TEST_ASSERT_EQUAL(0, intFlag);

    const uint32_t tick_count = intf->read();

    /* Check if interrupt scheduling still works. */
    intf->set_interrupt(tick_count + TICKER_INT_VAL);

    /* Wait for the interrupt. */
    while (intf->read() < (tick_count + TICKER_INT_VAL + TICKER_DELTA)) {
        /* Just wait. */
    }

    TEST_ASSERT_EQUAL(1, intFlag);
}

/* Test that the ticker increments by one on each tick. */
void ticker_increment_test(void)
{
    const ticker_info_t* p_ticker_info = intf->get_info();

    /* Perform test based on ticker speed. */
    if (p_ticker_info->frequency <= 250000) {    // low frequency tickers
        const uint32_t base_tick_count = intf->read();
        uint32_t next_tick_count = base_tick_count;

        while (next_tick_count == base_tick_count) {
            next_tick_count = intf->read();
        }

        TEST_ASSERT_UINT32_WITHIN(1, next_tick_count, base_tick_count);
    } else {  // high frequency tickers

        uint32_t num_of_cycles = NUM_OF_CYCLES;
        const uint32_t repeat_count = 20;
        const uint32_t max_inc_val = 100;

        uint32_t base_tick_count = count_ticks(num_of_cycles, 1);
        uint32_t next_tick_count = base_tick_count;
        uint32_t inc_val = 0;
        uint32_t repeat_cnt = 0;

        while (inc_val < max_inc_val) {
            next_tick_count = count_ticks(num_of_cycles + inc_val, 1);

            if (next_tick_count == base_tick_count) {

                /* Same tick count, so repeat 20 times and than
                 * increase num of cycles by 1.
                 */
                if (repeat_cnt == repeat_count) {
                    inc_val++;
                    repeat_cnt = 0;
                }

                repeat_cnt++;
            } else {
                /* Check if we got 1 tick diff. */
                if (next_tick_count - base_tick_count == 1 ||
                    base_tick_count - next_tick_count == 1) {
                    break;
                }

                /* It is possible that the difference between base and next
                 * tick count on some platforms is greater that 1, in this case we need
                 * to repeat counting with the reduced number of cycles (for slower boards).
                 * In cases if difference is exactly 1 we can exit the loop.
                 */
                num_of_cycles /= 2;
                inc_val = 0;
                repeat_cnt = 0;
                base_tick_count = count_ticks(num_of_cycles, 1);
            }
        }

        /* Since we are here we know that next_tick_count != base_tick_count.
         * The accuracy of our measurement method is +/- 1 tick, so it is possible that
         * next_tick_count == base_tick_count - 1. This is also valid result.
         */
        TEST_ASSERT_UINT32_WITHIN(1, next_tick_count, base_tick_count);
    }
}

/* Test that common ticker functions complete with the required amount of time. */
void ticker_speed_test(void)
{
    Timer timer;
    int counter = NUM_OF_CALLS;

    /* ---- Test ticker_read function. ---- */
    timer.reset();
    timer.start();
    while (counter--) {
        intf->read();
    }
    timer.stop();

    TEST_ASSERT(timer.read_us() < (NUM_OF_CALLS * (MAX_FUNC_EXEC_TIME_US + DELTA_FUNC_EXEC_TIME_US)));

    /* ---- Test ticker_clear_interrupt function. ---- */
    counter = NUM_OF_CALLS;
    timer.reset();
    timer.start();
    while (counter--) {
        intf->clear_interrupt();
    }
    timer.stop();

    TEST_ASSERT(timer.read_us() < (NUM_OF_CALLS * (MAX_FUNC_EXEC_TIME_US + DELTA_FUNC_EXEC_TIME_US)));

    /* ---- Test ticker_set_interrupt function. ---- */
    counter = NUM_OF_CALLS;
    timer.reset();
    timer.start();
    while (counter--) {
        intf->set_interrupt(0);
    }
    timer.stop();

    TEST_ASSERT(timer.read_us() < (NUM_OF_CALLS * (MAX_FUNC_EXEC_TIME_US + DELTA_FUNC_EXEC_TIME_US)));

    /* ---- Test fire_interrupt function. ---- */
    counter = NUM_OF_CALLS;
    timer.reset();
    timer.start();
    while (counter--) {
        intf->fire_interrupt();
    }
    timer.stop();

    TEST_ASSERT(timer.read_us() < (NUM_OF_CALLS * (MAX_FUNC_EXEC_TIME_US + DELTA_FUNC_EXEC_TIME_US)));

    /* ---- Test disable_interrupt function. ---- */
    counter = NUM_OF_CALLS;
    timer.reset();
    timer.start();
    while (counter--) {
        intf->disable_interrupt();
    }
    timer.stop();

    TEST_ASSERT(timer.read_us() < (NUM_OF_CALLS * (MAX_FUNC_EXEC_TIME_US + DELTA_FUNC_EXEC_TIME_US)));
}

utest::v1::status_t us_ticker_setup(const Case *const source, const size_t index_of_case)
{
    intf = get_us_ticker_data()->interface;

    OS_Tick_Disable();

    intf->init();

    prev_irq_handler = set_us_ticker_irq_handler(ticker_event_handler_stub);

    ticker_overflow_delta = US_TICKER_OVERFLOW_DELTA;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t us_ticker_teardown(const Case * const source, const size_t passed, const size_t failed,
        const failure_t reason)
{
    set_us_ticker_irq_handler(prev_irq_handler);

    prev_irq_handler = NULL;

    OS_Tick_Enable();

    return greentea_case_teardown_handler(source, passed, failed, reason);
}

#if DEVICE_LPTICKER
utest::v1::status_t lp_ticker_setup(const Case *const source, const size_t index_of_case)
{
    intf = get_lp_ticker_data()->interface;

    OS_Tick_Disable();

    intf->init();

    prev_irq_handler = set_lp_ticker_irq_handler(ticker_event_handler_stub);

    ticker_overflow_delta = LP_TICKER_OVERFLOW_DELTA;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t lp_ticker_teardown(const Case * const source, const size_t passed, const size_t failed,
        const failure_t reason)
{
    set_lp_ticker_irq_handler(prev_irq_handler);

    prev_irq_handler = NULL;

    OS_Tick_Enable();

    return greentea_case_teardown_handler(source, passed, failed, reason);
}
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Microsecond ticker init is safe to call repeatedly", us_ticker_setup, ticker_init_test, us_ticker_teardown),
    Case("Microsecond ticker info test", us_ticker_setup, ticker_info_test, us_ticker_teardown),
    Case("Microsecond ticker interrupt test", us_ticker_setup, ticker_interrupt_test, us_ticker_teardown),
    Case("Microsecond ticker past interrupt test", us_ticker_setup, ticker_past_test, us_ticker_teardown),
    Case("Microsecond ticker reschedule test", us_ticker_setup, ticker_repeat_reschedule_test, us_ticker_teardown),
    Case("Microsecond ticker fire interrupt", us_ticker_setup, ticker_fire_now_test, us_ticker_teardown),
    Case("Microsecond ticker overflow test", us_ticker_setup, ticker_overflow_test, us_ticker_teardown),
    Case("Microsecond ticker increment test", us_ticker_setup, ticker_increment_test, us_ticker_teardown),
    Case("Microsecond ticker speed test", us_ticker_setup, ticker_speed_test, us_ticker_teardown),
#if DEVICE_LPTICKER
    Case("lp ticker init is safe to call repeatedly", lp_ticker_setup, ticker_init_test, lp_ticker_teardown),
    Case("lp ticker info test", lp_ticker_setup, ticker_info_test, lp_ticker_teardown),
    Case("lp ticker interrupt test", lp_ticker_setup, ticker_interrupt_test, lp_ticker_teardown),
    Case("lp ticker past interrupt test", lp_ticker_setup, ticker_past_test, lp_ticker_teardown),
    Case("lp ticker reschedule test", lp_ticker_setup, ticker_repeat_reschedule_test, lp_ticker_teardown),
    Case("lp ticker fire interrupt", lp_ticker_setup, ticker_fire_now_test, lp_ticker_teardown),
    Case("lp ticker overflow test", lp_ticker_setup, ticker_overflow_test, lp_ticker_teardown),
    Case("lp ticker increment test", lp_ticker_setup, ticker_increment_test, lp_ticker_teardown),
    Case("lp ticker speed test", lp_ticker_setup, ticker_speed_test, lp_ticker_teardown),
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
