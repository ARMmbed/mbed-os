/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//CppUTest includes should be after your and system includes
#include "CppUTest/TestHarness.h"
#include "test_m2mblockmessage.h"

TEST_GROUP(M2MBlockMessage)
{
    Test_M2MBlockMessage* block_message;
    void setup()
    {
        block_message = new Test_M2MBlockMessage();
    }
    void teardown()
    {
        delete block_message;
    }
};

TEST(M2MBlockMessage, Create)
{
    CHECK(block_message->block_message != NULL);
}

TEST(M2MBlockMessage, test_set_message_info)
{
    block_message->test_set_message_info();
}

