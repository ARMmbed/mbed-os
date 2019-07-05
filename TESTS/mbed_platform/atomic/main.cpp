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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#if !MBED_CONF_RTOS_PRESENT
#error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_STACK 512

using utest::v1::Case;


namespace {

/* Lock-free operations will be much faster - keep runtime down */
#if MBED_ATOMIC_INT_LOCK_FREE
#define ADD_ITERATIONS (SystemCoreClock / 1000)
#else
#define ADD_ITERATIONS (SystemCoreClock / 8000)
#endif

template <typename T>
void add_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        core_util_atomic_fetch_add(ptr, T(1));
    }
}

template <typename T>
void add_release_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        core_util_atomic_fetch_add_explicit(ptr, T(1), mbed_memory_order_release);
    }
}

template <typename T>
void sub_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        core_util_atomic_fetch_sub(ptr, T(-1));
    }
}

template <typename T>
void bitops_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        core_util_atomic_fetch_add(ptr, T(1));
        core_util_atomic_fetch_and(ptr, T(-1));
        core_util_atomic_fetch_or(ptr, T(0));
    }
}

template <typename T>
void weak_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        T val = core_util_atomic_load(ptr);
        do {
        } while (!core_util_atomic_compare_exchange_weak(ptr, &val, T(val + 1)));
    }
}

template <typename T>
void strong_incrementer(T *ptr)
{
    for (long i = ADD_ITERATIONS; i > 0; i--) {
        T val = core_util_atomic_load(ptr);
        do {
        } while (!core_util_atomic_compare_exchange_strong(ptr, &val, T(val + 1)));
    }
}


/*
 * Run multiple threads incrementing each data item
 * ADD_ITERATIONS times, and at the end, check that
 * each item is <number of threads> * ADD_ITERATIONS.
 * Items are adjacent to catch any interference.
 *
 * Using core_util_atomic_ templates, and exercising
 * load and store briefly.
 */
template<typename T, void (*Fn)(T *)>
void test_atomic_add()
{
    struct  {
        volatile T nonatomic1;
        T atomic1;
        T atomic2;
        volatile T nonatomic2;
    } data;

    data.nonatomic1 = 0;
    core_util_atomic_store(&data.atomic1, T(0));
    core_util_atomic_store(&data.atomic2, T(0));
    data.nonatomic2 = 0;

    Thread t1(osPriorityNormal, THREAD_STACK);
    Thread t2(osPriorityNormal, THREAD_STACK);
    Thread t3(osPriorityNormal, THREAD_STACK);
    Thread t4(osPriorityNormal, THREAD_STACK);

    TEST_ASSERT_EQUAL(osOK, t1.start(callback(Fn, &data.atomic1)));
    TEST_ASSERT_EQUAL(osOK, t2.start(callback(Fn, &data.atomic1)));
    TEST_ASSERT_EQUAL(osOK, t3.start(callback(Fn, &data.atomic2)));
    TEST_ASSERT_EQUAL(osOK, t4.start(callback(Fn, &data.atomic2)));

    for (long i = ADD_ITERATIONS; i > 0; i--) {
        data.nonatomic1++;
        data.nonatomic2++;
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    TEST_ASSERT_EQUAL(T(ADD_ITERATIONS), data.nonatomic1);
    TEST_ASSERT_EQUAL(T(2 * ADD_ITERATIONS), core_util_atomic_load(&data.atomic1));
    TEST_ASSERT_EQUAL(T(2 * ADD_ITERATIONS), core_util_atomic_load(&data.atomic2));
    TEST_ASSERT_EQUAL(T(ADD_ITERATIONS), data.nonatomic2);
}

} // namespace

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test atomic add 8-bit", test_atomic_add<uint8_t, add_incrementer>),
    Case("Test atomic add 16-bit", test_atomic_add<uint16_t, add_incrementer>),
    Case("Test atomic add 32-bit", test_atomic_add<uint32_t, add_incrementer>),
    Case("Test atomic add 64-bit", test_atomic_add<uint64_t, add_incrementer>),
    Case("Test atomic add signed 8-bit", test_atomic_add<int8_t, add_incrementer>),
    Case("Test atomic add signed 16-bit", test_atomic_add<int16_t, add_incrementer>),
    Case("Test atomic add signed 32-bit", test_atomic_add<int32_t, add_incrementer>),
    Case("Test atomic add signed 64-bit", test_atomic_add<int64_t, add_incrementer>),
    Case("Test atomic add release 32-bit", test_atomic_add<uint32_t, add_release_incrementer>),
    Case("Test atomic sub 8-bit", test_atomic_add<uint8_t, sub_incrementer>),
    Case("Test atomic sub 16-bit", test_atomic_add<uint16_t, sub_incrementer>),
    Case("Test atomic sub 32-bit", test_atomic_add<uint32_t, sub_incrementer>),
    Case("Test atomic sub 64-bit", test_atomic_add<uint64_t, sub_incrementer>),
    Case("Test atomic bitops 8-bit", test_atomic_add<uint8_t, bitops_incrementer>),
    Case("Test atomic bitops 16-bit", test_atomic_add<uint16_t, bitops_incrementer>),
    Case("Test atomic bitops 32-bit", test_atomic_add<uint32_t, bitops_incrementer>),
    Case("Test atomic bitops 64-bit", test_atomic_add<uint64_t, bitops_incrementer>),
    Case("Test atomic compare exchange weak 8-bit", test_atomic_add<uint8_t, weak_incrementer>),
    Case("Test atomic compare exchange weak 16-bit", test_atomic_add<uint16_t, weak_incrementer>),
    Case("Test atomic compare exchange weak 32-bit", test_atomic_add<uint32_t, weak_incrementer>),
    Case("Test atomic compare exchange weak 64-bit", test_atomic_add<uint64_t, weak_incrementer>),
    Case("Test atomic compare exchange strong 8-bit", test_atomic_add<uint8_t, strong_incrementer>),
    Case("Test atomic compare exchange strong 16-bit", test_atomic_add<uint16_t, strong_incrementer>),
    Case("Test atomic compare exchange strong 32-bit", test_atomic_add<uint32_t, strong_incrementer>),
    Case("Test atomic compare exchange strong 64-bit", test_atomic_add<uint64_t, strong_incrementer>)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
