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

/* UART TEST FOR GREEN TEA
 *
 *  For basic functionality of the Grean-Tea and utest frameworks us ticker is not
 *  required. Advanced usage (using of asynchronous test cases) requires
 *  running us ticker since utest framework uses Timeout class to execute callback
 *  after the specified time.
 *
 *
 * utest framework uses Timeout class to execute callbacks after the specified
 * time.
 * Timeout class must provide valid implementation of the following
 * methods (used by utest framework):
 *  - void attach_us(Callback<void()> func, us_timestamp_t t);
 *  - void detach();
 *  For this purpose us ticker target specific drivers must provide
 *  us ticker counter which counts ticks with the specified frequency
 *  and is able to schedule interrupt at the specified point in time.
 *
 * TEST SCENARIO
 * This part of the test is executed on mbed board and communicates with host.
 *
 */
#include "../lib/BMTest.h"
#include "mbed.h"
#include "hal/us_ticker_api.h"

using namespace mbed;

static volatile unsigned int int_cnt;

#define BAUD_RATE 9600
#define TICKS_DELTA 10

void wait_cycles(int volatile cycles)
{
    while (cycles--)
        ;
}

void us_ticker_isr(const ticker_data_t * const ticker)
{
    (void) ticker;

    us_ticker_clear_interrupt();

    int_cnt++;
}

void timeout_callback()
{
    int_cnt++;
}

/* Verify if us ticker count ticks.
 *
 * Given is us ticker.
 * When us ticker is initialised.
 * Then us ticker counts ticks.
 */
void test_us_ticker_count_ticks(BMTest& test)
{
    us_ticker_init();

    const unsigned int ticker_val_1 = us_ticker_read();

    /* Wait 1000 cycles - this should be long enough to prove that ticker
     * counter has been incremented. */
    wait_cycles(1000);

    const unsigned int ticker_val_2 = us_ticker_read();

    BM_TEST_ASSERT(test, ticker_val_2 > ticker_val_1);
}

/* Verify if us ticker interrupt scheduling works.
 *
 * Given is us ticker.
 * When us ticker is initialised and interrupt is set.
 * Then us ticker interrupt handler is executed in the specified point in time.
 */
void test_us_ticker_schedule_int(BMTest& test)
{
    unsigned int timeout_values[] =
    { 100, 200, 300 };

    ticker_irq_handler_type org_us_ticker_irq_handler = set_us_ticker_irq_handler(us_ticker_isr);

    for (unsigned int i = 0; i < (sizeof(timeout_values) / sizeof(unsigned int)); i++) {

        us_ticker_init();

        int_cnt = 0;

        const unsigned int ticker_val_1 = us_ticker_read();

        us_ticker_set_interrupt(ticker_val_1 + timeout_values[i]);

        while (us_ticker_read() < (ticker_val_1 + timeout_values[i] - TICKS_DELTA)) {
            BM_TEST_ASSERT(test, int_cnt == 0);
        }

        while (us_ticker_read() < (ticker_val_1 + timeout_values[i] + TICKS_DELTA)) {
            /* just wait */
        }

        BM_TEST_ASSERT(test, int_cnt == 1);
    }

    set_us_ticker_irq_handler(org_us_ticker_irq_handler);
}

/* Verify if us ticker interrupt re-scheduling works.
 *
 * Given is us ticker.
 * When us ticker is initialised and interrupt is set and then interrupt time is modified.
 * Then us ticker interrupt handler is executed in the specified point in time.
 */
void test_us_ticker_reschedule_int(BMTest& test)
{
    ticker_irq_handler_type org_us_ticker_irq_handler = set_us_ticker_irq_handler(us_ticker_isr);

    us_ticker_init();

    int_cnt = 0;

    const unsigned int ticker_val_1 = us_ticker_read();

    us_ticker_set_interrupt(ticker_val_1 + 300);

    wait_cycles(10);

    us_ticker_set_interrupt(ticker_val_1 + 600);

    while (us_ticker_read() < (ticker_val_1 + 600 - TICKS_DELTA)) {
        BM_TEST_ASSERT(test, int_cnt == 0);
    }

    while (us_ticker_read() < (ticker_val_1 + 600 + TICKS_DELTA)) {
        /* just wait */
    }

    BM_TEST_ASSERT(test, int_cnt == 1);

    set_us_ticker_irq_handler(org_us_ticker_irq_handler);
}

/* Verify if us ticker configuration is valid.
 *
 * Given is us ticker.
 * When us ticker configuration is provided.
 * Then us ticker information indicate that frequency between 250KHz and 8MHz and the counter is at least 16 bits wide.
 */
void test_us_ticker_config(BMTest& test)
{
    const ticker_info_t* p_ticker_info = us_ticker_get_info();

    BM_TEST_ASSERT(test, p_ticker_info->frequency >= 250000 && p_ticker_info->frequency <= 8000000);
    BM_TEST_ASSERT(test, p_ticker_info->bits >= 16);
}

/* Verify if us ticker frequency is consistent with defined frequency.
 *
 * Given is us ticker.
 * When us ticker is initialized.
 * Then us ticker counter is incremented with the specified frequency.
 */
void test_us_ticker_freq(BMTest& test)
{
    const ticker_info_t* p_ticker_info = us_ticker_get_info();
    char buffer[10];

    test.receive(buffer);

    const unsigned int ticker_val_1 = us_ticker_read();

    test.receive(buffer);

    const unsigned int ticker_val_2 = us_ticker_read();

    BM_TEST_ASSERT(test, (ticker_val_2 - ticker_val_1) >= p_ticker_info->frequency - (p_ticker_info->frequency / 10));
    BM_TEST_ASSERT(test, (ticker_val_2 - ticker_val_1) <= p_ticker_info->frequency + (p_ticker_info->frequency / 10));
}

/* Functional test of Timeout class.
 *
 * Given is Timeout class object.
 * When us ticker is enabled and callback function is attached.
 * Then attached function is called in the specified point in time.
 */
void test_timeout(BMTest& test)
{
    Timeout timeout;

    int_cnt = 0;

    timeout.attach(&timeout_callback, 0.3);

    wait(0.1);

    BM_TEST_ASSERT(test, int_cnt == 0);

    wait(0.1);

    BM_TEST_ASSERT(test, int_cnt == 0);

    wait(0.11); // wait a little longer

    BM_TEST_ASSERT(test, int_cnt == 1);

    timeout.detach();

    wait(0.5);

    BM_TEST_ASSERT(test, int_cnt == 1);
}

int main()
{
    BMTest test;

    test.set_transmission_params(USBTX, USBRX, BAUD_RATE);

    test.sync_host_dev();

    test.set_timeout(10);

    test.exec_test_case(test_us_ticker_count_ticks, "Verify if us ticker counts ticks.", NULL);
    test.exec_test_case(test_us_ticker_schedule_int, "Verify if us ticker interrupt scheduling works.", NULL);
    test.exec_test_case(test_us_ticker_reschedule_int, "Verify if us ticker interrupt rescheduling works.", NULL);
    test.exec_test_case(test_us_ticker_config, "Verify if us ticker configuration is valid.", NULL);
    test.exec_test_case(test_us_ticker_freq, "Verify if us ticker frequency is valid.", "freq_test");
    test.exec_test_case(test_timeout, "Functional test of Timeout class.", NULL);

    test.done();

    while (true)
        ;
}

