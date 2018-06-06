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

using namespace utest::v1;

/* Structure for complex type. */
typedef struct
{
    int a;
    char b;
    int c;
} COMPLEX_TYPE;

/* Function to check if complex type object holds specified values.*/
bool comp_is_equal(COMPLEX_TYPE *object, int a, char b, int c)
{
    if (object->a == a && object->b == b && object->c == c) {
        return true;
    }

    return false;
}

/* Function to set complex type object fields.*/
void comp_set(COMPLEX_TYPE *object, int a, char b, int c)
{
    object->a = a;
    object->b = b;
    object->c = c;
}

/* Test circular buffer - input does not exceed capacity.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When circular buffer is filled with N elements.
 * Then all elements are read from the circular buffer in the FIFO order.
 *
 */
template<typename T, uint32_t BufferSize, typename CounterType>
void test_input_does_not_exceed_capacity_push_max_pop_max()
{
    CircularBuffer<T, BufferSize, CounterType> cb;
    T data = 0;

    for (uint32_t i = 0; i < BufferSize; i++) {
        data = (0xAA + i);
        cb.push(data);
        TEST_ASSERT_EQUAL(i + 1, cb.size());
    }

    for (uint32_t i = 0; i < BufferSize; i++) {
        TEST_ASSERT_TRUE(cb.pop(data));
        TEST_ASSERT_EQUAL(0xAA + i, data);
        TEST_ASSERT_EQUAL(BufferSize - i - 1, cb.size());
    }
}

/* Test circular buffer - input does not exceed capacity.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When circular buffer is filled as follows: (2 * N - 2) times 2 elements are pushed and 1 element is popped.
 * Then all elements are read from the circular buffer in the FIFO order.
 *
 */
template<typename T, uint32_t BufferSize, typename CounterType>
void test_input_does_not_exceed_capacity_push_2_pop_1()
{
    CircularBuffer<T, BufferSize, CounterType> cb;
    static const int num_of_elem_push = (2 * BufferSize - 2);
    T push_buffer = 0;
    T pop_buffer = 0;

    /* Push 2 elements and pop one element in each cycle. */
    for (uint32_t i = 0; i < num_of_elem_push; i += 2) {
        push_buffer = (0xAA + i);
        cb.push(push_buffer);
        push_buffer++;
        cb.push(push_buffer);
        TEST_ASSERT_EQUAL(i / 2 + 2, cb.size());

        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        TEST_ASSERT_EQUAL(0xAA + i / 2, pop_buffer);
        TEST_ASSERT_EQUAL(i / 2 + 1, cb.size());
    }

    /* Pop the rest. */
    for (uint32_t i = 0; i < (num_of_elem_push / 2); i++) {
        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        TEST_ASSERT_EQUAL(0xAA + num_of_elem_push / 2 + i, pop_buffer);
        TEST_ASSERT_EQUAL(num_of_elem_push / 2 - 1 - i, cb.size());

    }
}

/* Test circular buffer - input exceeds capacity.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When circular buffer is filled with N + 1 elements.
 * Then first pushed element is lost (overwritten by the last element) and
 * elements are read from the circular buffer in the FIFO order.
 *
 */
template<typename T, uint32_t BufferSize, typename CounterType>
void test_input_exceeds_capacity_push_max_plus_1_pop_max()
{
    CircularBuffer<T, BufferSize, CounterType> cb;
    static const int num_of_elem_push = (BufferSize + 1);
    T data = 0;

    for (uint32_t i = 0; i < num_of_elem_push; i++) {
        data = (0xAA + i);
        cb.push(data);
        if (i < BufferSize) {
            TEST_ASSERT_EQUAL(i + 1, cb.size());
        } else {
            TEST_ASSERT_EQUAL(BufferSize, cb.size());
        }

    }

    for (uint32_t i = 0; i < (BufferSize - 1); i++) {
        TEST_ASSERT_TRUE(cb.pop(data));
        TEST_ASSERT_EQUAL(0xAA + i + 1, data);
        TEST_ASSERT_EQUAL(BufferSize - 1 - i, cb.size());
    }

    /* First element should be overwritten. */
    TEST_ASSERT_TRUE(cb.pop(data));
    TEST_ASSERT_EQUAL((0xAA + num_of_elem_push - 1), data);
    TEST_ASSERT_EQUAL(0, cb.size());
}

/* Test circular buffer - input exceeds capacity.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When circular buffer is filled as follows: (2 * N) times 2 elements are pushed and 1 element is popped.
 * Then first pushed element is lost (overwritten by the last element) and
 * elements are read from the circular buffer in the FIFO order.
 *
 */
template<typename T, uint32_t BufferSize, typename CounterType>
void test_input_exceeds_capacity_push_2_pop_1()
{
    CircularBuffer<T, BufferSize, CounterType> cb;
    static const int num_of_elem_push = (2 * BufferSize);
    T push_buffer = 0;
    T pop_buffer = 0;

    /* Push 2 elements and pop one element in each cycle. */
    for (uint32_t i = 0; i < num_of_elem_push; i += 2) {
        push_buffer = (0xAA + i);
        cb.push(push_buffer);
        push_buffer++;
        cb.push(push_buffer);
        if ((i / 2 + 1) < BufferSize) {
            TEST_ASSERT_EQUAL(i / 2 + 2, cb.size());
        } else {
            TEST_ASSERT_EQUAL(BufferSize, cb.size());
        }

        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        if ((i / 2 + 1) < BufferSize) {
            TEST_ASSERT_EQUAL(i / 2 + 1, cb.size());
        } else {
            TEST_ASSERT_EQUAL(BufferSize - 1, cb.size());
        }

        /* First element has been overwritten. */
        if (i == (num_of_elem_push - 2)) {
            TEST_ASSERT_EQUAL(0xAA + i / 2 + 1, pop_buffer);
        } else {
            TEST_ASSERT_EQUAL(0xAA + i / 2, pop_buffer);
        }
    }

    /* Pop the rest - one element has been overwritten. */
    for (uint32_t i = 0; i < (num_of_elem_push / 2 - 1); i++) {
        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        TEST_ASSERT_EQUAL(0xAA + num_of_elem_push / 2 + i + 1, pop_buffer);
    }
}

/* Test circular buffer - input exceeds capacity (complex type).
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When circular buffer is filled as follows: (2 * N) times 2 elements are pushed and 1 element is popped.
 * Then first pushed element is lost (overwritten by the last element) and
 * elements are read from the circular buffer in the FIFO order.
 *
 */
template<uint32_t BufferSize, typename CounterType>
void test_input_exceeds_capacity_push_2_pop_1_complex_type()
{
    CircularBuffer<COMPLEX_TYPE, BufferSize, CounterType> cb;
    static const int num_of_elem_push = (2 * BufferSize);
    COMPLEX_TYPE push_buffer = {0};
    COMPLEX_TYPE pop_buffer = {0};

    /* Push 2 elements and pop one element in each cycle. */
    for (uint32_t i = 0; i < num_of_elem_push; i += 2) {
        comp_set(&push_buffer, 0xAA + i, 0xBB + i, 0xCC + i);
        cb.push(push_buffer);
        comp_set(&push_buffer, 0xAA + i + 1, 0xBB + i + 1, 0xCC + i + 1);
        cb.push(push_buffer);
        if ((i / 2 + 1) < BufferSize) {
            TEST_ASSERT_EQUAL(i / 2 + 2, cb.size());
        } else {
            TEST_ASSERT_EQUAL(BufferSize, cb.size());
        }

        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        if ((i / 2 + 1) < BufferSize) {
            TEST_ASSERT_EQUAL(i / 2 + 1, cb.size());
        } else {
            TEST_ASSERT_EQUAL(BufferSize - 1, cb.size());
        }

        /* First element has been overwritten. */
        if (i == (num_of_elem_push - 2)) {
            const bool result = comp_is_equal(&pop_buffer, 0xAA + 1 + i / 2, 0xBB + 1 + i / 2, 0xCC + 1 + i / 2);
            TEST_ASSERT_TRUE(result);
        } else {
            const bool result = comp_is_equal(&pop_buffer, 0xAA + i / 2, 0xBB + i / 2, 0xCC + i / 2);
            TEST_ASSERT_TRUE(result);
        }
    }

    /* Pop the rest - one element has been overwritten. */
    for (uint32_t i = 0; i < (num_of_elem_push / 2 - 1); i++) {
        TEST_ASSERT_TRUE(cb.pop(pop_buffer));
        const bool result = comp_is_equal(&pop_buffer, 0xAA + num_of_elem_push / 2 + i + 1,
                                          0xBB + num_of_elem_push / 2 + i + 1, 0xCC + num_of_elem_push / 2 + i + 1);
        TEST_ASSERT_TRUE(result);
    }
}

/* Test circular buffer - test pop(), peek(), empty(), full(), size() after CircularBuffer creation.
 *
 * Given is a circular buffer.
 * When circular buffer is created.
 * Then circular buffer is empty:
 * - empty() returns true,
 * - pop() function returns false,
 * - peek() function returns false,
 * - full() function returns false,
 * - size() function returns 0,
 *
 */
void test_pop_empty_full_size_after_creation()
{
    CircularBuffer<int, 1> cb;
    int data = 0;

    TEST_ASSERT_TRUE(cb.empty());
    TEST_ASSERT_FALSE(cb.pop(data));
    TEST_ASSERT_FALSE(cb.peek(data));
    TEST_ASSERT_FALSE(cb.full());
    TEST_ASSERT_EQUAL(0, cb.size());
}

/* Test circular buffer - test empty() function.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When operations on circular buffer are performed (push, pop).
 * Then empty() function returns true if buffer is empty, false otherwise.
 *
 */
template<typename T, uint32_t BufferSize>
void test_empty()
{
    CircularBuffer<T, BufferSize> cb;
    T data = 0;

    /* Push max elements. */
    for (uint32_t i = 0; i < BufferSize; i++) {
        cb.push(data);
        TEST_ASSERT_FALSE(cb.empty());
    }

    /* Push next 2*BufferSize elements (overwrite entries). */
    for (uint32_t i = 0; i < (2 * BufferSize); i++) {
        cb.push(data);
        TEST_ASSERT_FALSE(cb.empty());
    }

    /* Pop (BufferSize - 1) elements (leave one). */
    for (uint32_t i = 0; i < (BufferSize - 1); i++) {
        TEST_ASSERT_TRUE(cb.pop(data));
        TEST_ASSERT_FALSE(cb.empty());
    }

    /* Take one which is left. */
    TEST_ASSERT_TRUE(cb.pop(data));
    TEST_ASSERT_TRUE(cb.empty());

    /* Add one element to the empty buffer. */
    cb.push(data);
    TEST_ASSERT_FALSE(cb.empty());
}

/* Test circular buffer - test full() function.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When operations on circular buffer are performed (push, pop).
 * Then full() function returns true if buffer is full, false otherwise.
 *
 */
template<typename T, uint32_t BufferSize>
void test_full()
{
    CircularBuffer<T, BufferSize> cb;
    T data = 0;

    /* Push max elements - 1. */
    for (uint32_t i = 0; i < (BufferSize - 1); i++) {
        cb.push(data);
        TEST_ASSERT_FALSE(cb.full());
    }

    /* Push one element - buffer should be full now. */
    cb.push(data);
    TEST_ASSERT_TRUE(cb.full());

    /* Push next 2*BufferSize elements (overwrite entries). */
    for (uint32_t i = 0; i < (2 * BufferSize); i++) {
        cb.push(data);
        TEST_ASSERT_TRUE(cb.full());
    }

    /* Pop all elements. */
    for (uint32_t i = 0; i < BufferSize; i++) {
        TEST_ASSERT_TRUE(cb.pop(data));
        TEST_ASSERT_FALSE(cb.full());
    }
}

/* Test circular buffer - test reset() function.
 *
 * Given is a circular buffer with the capacity equal to N (BufferSize).
 * When reset operation is performed on circular buffer.
 * Then circular buffer is cleared.
 *
 */
template<typename T, uint32_t BufferSize>
void test_reset()
{
    CircularBuffer<T, BufferSize> cb;
    T data = 0xAA;

    /* Push max elements. */
    for (uint32_t i = 0; i < BufferSize; i++) {
        cb.push(data);
    }

    TEST_ASSERT_TRUE(cb.full());
    TEST_ASSERT_FALSE(cb.empty());
    TEST_ASSERT_EQUAL(BufferSize, cb.size());

    cb.reset();

    TEST_ASSERT_FALSE(cb.full());
    TEST_ASSERT_TRUE(cb.empty());
    TEST_ASSERT_FALSE(cb.pop(data));
    TEST_ASSERT_EQUAL(0, cb.size());

    /* Check if after reset push and pop operations work. */
    for (uint32_t i = 0; i < BufferSize; i++) {
        cb.push(data);
        data++;
    }

    for (uint32_t i = 0; i < BufferSize; i++) {
        cb.pop(data);
        TEST_ASSERT_EQUAL(0xAA + i, data);
    }
}

/* Test circular buffer - creation of circular buffer with max buffer size consistent with counter type.
 *
 * Given is a circular buffer.
 * When circular buffer is created with buffer size equal to 255 and counter type equal to unsigned char.
 * Then circular buffer is successfully created.
 *
 */
void test_counter_type_buffer_size()
{
    CircularBuffer<int, 255, unsigned char> cb;
    int data = 100;

    /* Perform some operations. */
    cb.push(data);
    data = 0;
    cb.pop(data);
    TEST_ASSERT_EQUAL(100, data);
}

/* Test circular buffer - peek should not update buffer data.
 *
 * When circular buffer peek operation is performed along with
 * push and pop, it should not update the buffer data elements.
 * Elements read using pop/peek operation should match.
 *
 */
void test_peek_no_pop()
{
    CircularBuffer<int, 3, unsigned char> cb;
    int data = 0;
    int peek_data = 0;

    for (uint32_t i = 0; i < 3; i++) {
        data = (0xAA + i);
        cb.push(data);
        cb.peek(peek_data);
        TEST_ASSERT_EQUAL(i + 1, cb.size());
    }

    for (uint32_t i = 0; i < 3; i++) {
        TEST_ASSERT_TRUE(cb.peek(peek_data));
        TEST_ASSERT_EQUAL(0xAA + i, peek_data);
        TEST_ASSERT_TRUE(cb.pop(data));
        TEST_ASSERT_EQUAL(0xAA + i, data);
        TEST_ASSERT_EQUAL(3 - i - 1, cb.size());
    }
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Input does not exceed capacity(1) push max, pop max.",
         test_input_does_not_exceed_capacity_push_max_pop_max<uint32_t, 1, unsigned int>, greentea_failure_handler),
    Case("Input does not exceed capacity(3) push max, pop max.",
         test_input_does_not_exceed_capacity_push_max_pop_max<char, 3, unsigned int>, greentea_failure_handler),

    Case("Input does not exceed capacity(5) push 2, pop 1.",
         test_input_does_not_exceed_capacity_push_2_pop_1<uint32_t, 5, unsigned char>, greentea_failure_handler),
    Case("Input does not exceed capacity(10) push 2, pop 1.",
         test_input_does_not_exceed_capacity_push_2_pop_1<char, 10, unsigned char>, greentea_failure_handler),

    Case("Input exceeds capacity(1) push max+1, pop max.",
         test_input_exceeds_capacity_push_max_plus_1_pop_max<uint32_t, 1, unsigned int>, greentea_failure_handler),
    Case("Input exceeds capacity(3) push max+1, pop max.",
         test_input_exceeds_capacity_push_max_plus_1_pop_max<char, 3, unsigned int>, greentea_failure_handler),

    Case("Input exceeds capacity(5) push 2, pop 1.",
         test_input_exceeds_capacity_push_2_pop_1<uint32_t, 5, unsigned short>, greentea_failure_handler),
    Case("Input exceeds capacity(10) push 2, pop 1.",
         test_input_exceeds_capacity_push_2_pop_1<char, 10, unsigned short>, greentea_failure_handler),

    Case("empty() returns true when buffer(3 elements) is empty.", test_empty<uint32_t, 3>, greentea_failure_handler),
    Case("empty() returns true when buffer(5 elements) is empty.", test_empty<uint32_t, 5>, greentea_failure_handler),

    Case("full() returns true when buffer(3 elements) is full.", test_full<uint32_t, 3>, greentea_failure_handler),
    Case("full() returns true when buffer(5 elements) is full.", test_full<uint32_t, 5>, greentea_failure_handler),

    Case("reset() clears the buffer.", test_reset<uint32_t, 5>, greentea_failure_handler),

    Case("Test pop(), peek(), empty(), full(), size() after CircularBuffer creation.",
         test_pop_empty_full_size_after_creation, greentea_failure_handler),

    Case("Test CounterType/BufferSize boarder case.", test_counter_type_buffer_size, greentea_failure_handler),

    Case("Input exceeds capacity(5) push 2, pop 1 - complex type.",
         test_input_exceeds_capacity_push_2_pop_1_complex_type<5, unsigned short>, greentea_failure_handler),

    Case("peek() return data without popping the element.", test_peek_no_pop, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return Harness::run(specification);
}
