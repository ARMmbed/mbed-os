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
#include "Watchdog.h"

using namespace mbed;
extern bool testcase;
// AStyle ignored as the definition is not clear due to preprocessor usage
// *INDENT-OFF*
class TestMbedVirtualWatchdog : public testing::Test {
protected:

    void SetUp()
    {
    }

    void TearDown()
    {
    }
};
// *INDENT-ON*

TEST_F(TestMbedVirtualWatchdog, test_watchdog_start_pass)
{
    EXPECT_TRUE(Watchdog::get_instance().start());
}

TEST_F(TestMbedVirtualWatchdog, test_watchdog_start_fail)
{
    EXPECT_FALSE(Watchdog::get_instance().start());
}

TEST_F(TestMbedVirtualWatchdog, test_watchdog_stop_pass)
{
    EXPECT_TRUE(Watchdog::get_instance().stop());
}

TEST_F(TestMbedVirtualWatchdog, test_watchdog_stop_fail)
{
    EXPECT_FALSE(Watchdog::get_instance().stop());
}

TEST_F(TestMbedVirtualWatchdog, test_mbed_wdog_manager_get_max_timeout)
{
    EXPECT_EQ(0xFFFFFFFF, Watchdog::get_instance().get_max_timeout());
}


TEST_F(TestMbedVirtualWatchdog, test_mbed_wdog_manager_get_timeout)
{
    EXPECT_EQ(500, Watchdog::get_instance().get_timeout());
}

