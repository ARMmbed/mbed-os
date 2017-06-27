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

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "us_ticker_api.h"
#include "lp_ticker_api.h"

using namespace utest::v1;

static const uint32_t QUICK_EVENT_COUNT = 100000;

static const ticker_interface_t* intf;
static volatile uint32_t irq_count;
static void (*irq_cb)(void);

static void ticker_irq_handler(void)
{
    intf->clear_interrupt();
    irq_count++;
    if (irq_cb) {
        irq_cb();
    }
}

void us_ticker_irq_handler(void)
{
    ticker_irq_handler();
}

#if DEVICE_LOWPOWERTIMER
void lp_ticker_irq_handler(void)
{
    ticker_irq_handler();
}
#endif

static void run_next_event(void)
{
    if (irq_count < QUICK_EVENT_COUNT) {
        intf->set_interrupt(intf->read() - 1);
    }
}

/**
 * Test that events schedule in the past still trigger an interrupt
 *
 * Events can be scheduled in the past if there is a delay between the
 * call to timer->read() and timer->set_interrupt(). This delay can be
 * caused by a task switch or interrupt, in addition to other things.
 * This test ensures that the hardware gracefully handles this case.
 */
void test_events_in_past(void)
{
    static const uint32_t us_in_past[] = {
        0,
        1,
        10,
        1000000,
        0x70000000
    };
    irq_count = 0;
    irq_cb = 0;
    intf->init();

    for (uint32_t i = 0; i < sizeof(us_in_past) / sizeof(us_in_past[0]); i++) {
        printf("Testing interrupt %lu us in the past\r\n", us_in_past[i]);
        intf->set_interrupt(intf->read() - us_in_past[i]);
        wait(0.01);
        TEST_ASSERT_EQUAL(i + 1, irq_count);
    }

}

/**
 * Test that the port can handle events being added non-stop
 *
 * In the past some ports processed new pending events recursively.
 * With this type of implementation, if too many events are pending at
 * the same time then the recursion will be deep enough to cause a stack
 * overflow. This test simulates that to ensure there will not be a stack
 * overflow.
 */
void test_events_quickly(void)
{
    irq_count = 0;
    irq_cb = run_next_event;
    intf->init();

    printf("Running %lu events now\r\n", QUICK_EVENT_COUNT);
    intf->set_interrupt(intf->read() - 1);
    while (irq_count < QUICK_EVENT_COUNT) {
        wait(0.01);
    }
    TEST_ASSERT_EQUAL(QUICK_EVENT_COUNT, irq_count);
}

utest::v1::status_t us_ticker_setup(const Case *const source, const size_t index_of_case)
{
  intf = get_us_ticker_data()->interface;
  irq_count = 0;
  irq_cb = 0;
  return greentea_case_setup_handler(source, index_of_case);
}

#if DEVICE_LOWPOWERTIMER
utest::v1::status_t lp_ticker_setup(const Case *const source, const size_t index_of_case)
{
    intf = get_lp_ticker_data()->interface;
    irq_count = 0;
    irq_cb = 0;
  return greentea_case_setup_handler(source, index_of_case);
}
#endif

Case cases[] = {
    Case("Test us ticker events in the past", us_ticker_setup, test_events_in_past),
    Case("Test us ticker events occurring quickly", us_ticker_setup, test_events_quickly),
#if DEVICE_LOWPOWERTIMER
    Case("Test lp ticker events in the past", lp_ticker_setup, test_events_in_past),
    Case("Test lp ticker events occurring quickly", lp_ticker_setup, test_events_quickly),
#endif
};


utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(1200, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    return Harness::run(specification);
}
