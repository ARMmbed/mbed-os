/* Copyright (c) 2019 ARM Limited
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

/*
 * Purpose of this test is just to be empty baseline
 * that does nothing, but includes all source files
 * in the build.
 * Do not add any tests here
 */

class EmptyBaseline : public testing::Test {
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(EmptyBaseline, constructor)
{
    EXPECT_TRUE(true);
}
