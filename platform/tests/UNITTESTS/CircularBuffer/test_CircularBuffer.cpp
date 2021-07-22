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

#define TEST_BUFFER_SIZE (10)

class TestCircularBuffer : public testing::Test {
protected:
    mbed::CircularBuffer<int, TEST_BUFFER_SIZE> *buf;

    virtual void SetUp()
    {
        buf = new mbed::CircularBuffer<int, TEST_BUFFER_SIZE>;
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

TEST_F(TestCircularBuffer, push_pop)
{
    int item = 0;
    buf->push(1);
    bool ret = buf->pop(item);
    EXPECT_TRUE(ret);
    EXPECT_EQ(item, 1);
}

TEST_F(TestCircularBuffer, reset)
{
    buf->push(1);
    EXPECT_EQ(buf->size(), 1);
    buf->reset();
    EXPECT_EQ(buf->size(), 0);
}

TEST_F(TestCircularBuffer, pop_empty)
{
    int item = 0;
    bool ret = buf->pop(item);
    EXPECT_FALSE(ret);
}

TEST_F(TestCircularBuffer, push_pop_multiple)
{
    const int test_numbers[TEST_BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    /* this will check pushing across the buffer end */
    for (int i = 1; i < TEST_BUFFER_SIZE; i++) {
        int test_numbers_popped[TEST_BUFFER_SIZE] = { 0 };
        buf->push(test_numbers, i);
        EXPECT_EQ(buf->size(), i);
        int number_of_items = buf->pop(test_numbers_popped, i);
        EXPECT_EQ(buf->size(), 0);
        EXPECT_EQ(number_of_items, i);
        EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, i));
    }
}

TEST_F(TestCircularBuffer, overflow)
{
    const int test_numbers[TEST_BUFFER_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int test_numbers_popped[TEST_BUFFER_SIZE] = { 0 };

    buf->push(-1);

    /* there is now not enough space for all the elements, old ones should be overwritten */

    buf->push(test_numbers, TEST_BUFFER_SIZE);

    int number_of_items = buf->pop(test_numbers_popped, TEST_BUFFER_SIZE);
    EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
    EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, TEST_BUFFER_SIZE));

    /* there is a difference where the overflow is caused by a smaller write
     * and the buffer should retain part of old values */

    buf->push(-1);
    buf->push(-2);
    buf->push(test_numbers, TEST_BUFFER_SIZE-1); /* -1 is overwritten but -2 is kept */

    int popped_number;
    buf->pop(popped_number);
    EXPECT_EQ(popped_number, -2);

    buf->pop(test_numbers_popped, TEST_BUFFER_SIZE - 1);
    EXPECT_TRUE(0 == memcmp(test_numbers, test_numbers_popped, TEST_BUFFER_SIZE - 1));
}

TEST_F(TestCircularBuffer, writing_over_max_capacity)
{
    const int test_numbers[TEST_BUFFER_SIZE + 1] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int test_numbers_popped[TEST_BUFFER_SIZE] = { 0 };

    /* the loop creates different amounts of existing elements prior to write over capacity */
    for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
        for (int j = 0; j < i; j++) {
            buf->push(-1);
        }
        /* first element should be dropped */
        buf->push(test_numbers, TEST_BUFFER_SIZE + 1);

        int number_of_items = buf->pop(test_numbers_popped, TEST_BUFFER_SIZE + 1);
        EXPECT_EQ(number_of_items, TEST_BUFFER_SIZE);
        EXPECT_EQ(buf->size(), 0);
        EXPECT_TRUE(0 == memcmp(test_numbers + 1, test_numbers_popped, TEST_BUFFER_SIZE));
    }
}
