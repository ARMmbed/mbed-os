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
#include "LoRaPHYUS915.h"

class Test_LoRaPHYUS915 : public testing::Test {
protected:
    LoRaPHYUS915 *object;

    virtual void SetUp()
    {
        object = new LoRaPHYUS915();
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaPHYUS915, constructor)
{
    EXPECT_TRUE(object);
}

