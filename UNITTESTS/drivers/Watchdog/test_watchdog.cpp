/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "drivers/VirtualWatchdog.h"

class TestWatchdog : public testing::Test {
public:
    static uint32_t expect_assert_count;
    static uint32_t expect_reset_count;
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

uint32_t TestWatchdog::expect_assert_count = 0;
uint32_t TestWatchdog::expect_reset_count = 0;

void mbed_assert_internal(const char *expr, const char *file, int line)
{
    TestWatchdog::expect_assert_count++;
}

void mock_system_reset()
{
    TestWatchdog::expect_reset_count++;
}

TEST_F(TestWatchdog, wdog_constructor)
{
    EXPECT_LE(sizeof(mbed::VirtualWatchdog), 1024);
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
}

TEST_F(TestWatchdog, wdog_constructor_with_default_value)
{
    mbed::VirtualWatchdog watchdog;
}

TEST_F(TestWatchdog, wdog_start_pass)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.start();
    watchdog.stop();
    EXPECT_EQ(0, TestWatchdog::expect_assert_count);
}

TEST_F(TestWatchdog, wdog_kick_pass)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.start();
    watchdog.kick();
    watchdog.stop();
    EXPECT_EQ(0, TestWatchdog::expect_assert_count);
}

TEST_F(TestWatchdog, wdog_stop_fail)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.start();
    watchdog.stop();
    watchdog.stop();
    EXPECT_EQ(1, TestWatchdog::expect_assert_count);
    TestWatchdog::expect_assert_count = 0;
}
TEST_F(TestWatchdog, wdog_kick_fail)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.kick();
    EXPECT_EQ(1, TestWatchdog::expect_assert_count);
    TestWatchdog::expect_assert_count = 0;
}

TEST_F(TestWatchdog, wdog_start_kick_pass)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    mbed::VirtualWatchdog watchdog1(600, "watchdog_unittest_1");
    mbed::VirtualWatchdog watchdog2(700, "watchdog_unittest_2");
    watchdog.start();
    watchdog1.start();
    watchdog2.start();
    watchdog1.kick();
    watchdog.kick();
    watchdog2.kick();
    watchdog1.stop();
    watchdog.stop();
    watchdog2.stop();
    EXPECT_EQ(0, TestWatchdog::expect_assert_count);
    EXPECT_EQ(0, TestWatchdog::expect_reset_count);
}

TEST_F(TestWatchdog, wdog_start_process_pass)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.start();
    watchdog.kick();
    watchdog.stop();
    EXPECT_EQ(0, TestWatchdog::expect_assert_count);
    EXPECT_EQ(0, TestWatchdog::expect_reset_count);
}

TEST_F(TestWatchdog, wdog_start_process_fail)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    mbed::VirtualWatchdog watchdog1(500, "watchdog_unittest-1");
    watchdog.start();
    watchdog1.start();
    watchdog1.kick();
    watchdog.stop();
    watchdog1.stop();
    EXPECT_EQ(0, TestWatchdog::expect_assert_count);
    EXPECT_EQ(1, TestWatchdog::expect_reset_count);
    TestWatchdog::expect_reset_count = 0;
}

TEST_F(TestWatchdog, wdog_start_fail)
{
    mbed::VirtualWatchdog watchdog(500, "watchdog_unittest");
    watchdog.start();
    watchdog.start();
    watchdog.stop();
    EXPECT_EQ(1, TestWatchdog::expect_assert_count);
    TestWatchdog::expect_assert_count = 0;
}
