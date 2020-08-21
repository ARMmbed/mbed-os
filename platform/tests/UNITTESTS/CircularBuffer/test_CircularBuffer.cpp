/*
 * Copyright (c) 2018, Arm Limited and affiliates
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
#include "platform/CircularBuffer.h"

class TestCircularBuffer : public testing::Test {
protected:
    mbed::CircularBuffer<int, 10> *buf;

    virtual void SetUp()
    {
        buf = new mbed::CircularBuffer<int, 10>;
    }

    virtual void TearDown()
    {
        delete buf;
    }
};

TEST_F(TestCircularBuffer, constructor)
{
    EXPECT_TRUE(buf);
}
