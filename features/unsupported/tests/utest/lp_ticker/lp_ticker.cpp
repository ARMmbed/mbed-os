/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include <stdio.h>
#include "TestHarness.h"
#include "mbed.h"
#include "us_ticker_api.h"

/* Low power timer test.
 */

#if !DEVICE_LPTICKER
#error This test unit requires low power to be defined for a target
#endif

volatile bool complete;

void cbdone() {
    complete = true;
}

TEST_GROUP(LowPowerTimerTest)
{
    LowPowerTimeout *obj;

    void setup() {
        obj = new LowPowerTimeout;
        complete = false;
    }

    void teardown() {
        delete obj;
        obj = NULL;
    }

};

// 2 second timeout using lp ticker, time is measured using us ticker
TEST(LowPowerTimerTest, lp_ticker_callback_2sec_timeout)
{
    timestamp_t start = us_ticker_read();
    obj->attach(&cbdone, 2.0f);
    while (!complete);
    timestamp_t end = us_ticker_read();

    // Not accurate for longer delays, thus +/-
    if ((end - start) > 2100000) {
        printf("Error! Start: %u, end: %u. It took longer than 2.1 sec.", start, end);
        CHECK_EQUAL(0, 1);
    }
    CHECK_EQUAL(complete, 1);
}

// 50 microsecond timeout
TEST(LowPowerTimerTest, lp_ticker_callback_50us_timeout)
{
    timestamp_t start = us_ticker_read();
    obj->attach_us(&cbdone, 50);
    while(!complete);
    timestamp_t end = us_ticker_read();
    // roughly should be around 50us +/- 500us, for example with 32kHz, it can be 32us the lowest
    CHECK_EQUAL(((end - start) > 1) && ((end - start) < 500) ,1);
    CHECK_EQUAL(complete, 1);
}

// 1 milisecond timeout
TEST(LowPowerTimerTest, lp_ticker_callback_1ms_timeout)
{
    timestamp_t start = us_ticker_read();
    obj->attach_us(&cbdone, 1000);
    while(!complete);
    timestamp_t end = us_ticker_read();

    CHECK_EQUAL(((end - start) > 800) && ((end - start) < 1600) ,1);
    CHECK_EQUAL(complete, 1);
}

// 5 second wake up from deep sleep
TEST(LowPowerTimerTest, lp_ticker_deepsleep_wakeup_5sec_timeout)
{
    timestamp_t start = lp_ticker_read();
    obj->attach(&cbdone, 5.0f);
    deepsleep();
    while (!complete);
    timestamp_t end = lp_ticker_read();
    // roughly should be around 5seconds +/- 100ms
    CHECK_EQUAL(((end - start) > 4900000) && ((end - start) < 5100000) ,1);
    CHECK_EQUAL(complete, 1);
}

// 1ms wake up from deep sleep
TEST(LowPowerTimerTest, lp_ticker_deepsleep_wakeup_1ms_timeout)
{
    timestamp_t start = lp_ticker_read();
    obj->attach(&cbdone, 0.001f);
    deepsleep();
    while (!complete);
    timestamp_t end = lp_ticker_read();
    // 1ms timeout +/- 600us
    CHECK_EQUAL(((end - start) > 400) && ((end - start) < 1600) ,1);
    CHECK_EQUAL(complete, 1);
}
