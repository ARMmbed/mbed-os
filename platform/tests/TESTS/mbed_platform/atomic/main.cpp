/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include <mstd_atomic>

#if !MBED_CONF_RTOS_PRESENT
#error [NOT_SUPPORTED] test not supported
#else

#define THREAD_STACK 512

using utest::v1::Case;

namespace {

using mstd::atomic;

/* Lock-free operations will be much faster - keep runtime down */
#define ADD_UNLOCKED_ITERATIONS (SystemCoreClock / 1000)
#define ADD_LOCKED_ITERATIONS   (SystemCoreClock / 8000)

template <typename A>
static inline long add_iterations(A &a)
{
    return a.is_lock_free() ? ADD_UNLOCKED_ITERATIONS : ADD_LOCKED_ITERATIONS;
}

template <typename A>
struct add_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            ++(*ptr);
        }
    }
};

template <typename A>
struct add_release_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            ptr->fetch_add(1, mstd::memory_order_release);
        }
    }
};

template <typename A>
struct sub_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            ptr->fetch_sub(-1);
        }
    }
};

template <typename A>
struct bitops_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            (*ptr) += 1;
            (*ptr) &= -1;
            (*ptr) |= 0;
        }
    }
};

template <typename A>
struct weak_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            typename A::value_type val = ptr->load();
            do {
            } while (!ptr->compare_exchange_weak(val, val + 1));
        }
    }
};

template <typename A>
struct strong_incrementer {
    static void op(A *ptr)
    {
        for (long i = add_iterations(*ptr); i > 0; i--) {
            typename A::value_type val = ptr->load();
            do {
            } while (!ptr->compare_exchange_strong(val, val + 1));
        }
    }
};



/*
 * Run multiple threads incrementing each data item
 * ADD_ITERATIONS times, and at the end, check that
 * each item is <number of threads> * ADD_ITERATIONS.
 * Items are adjacent to catch any interference.
 *
 * Using core_util_atomic_ templates, and exercising
 * load and store briefly.
 */
template<typename T, template<typename A> class Fn>
void test_atomic_add()
{
    struct  {
        volatile T nonatomic1;
        atomic<T> atomic1;
        volatile atomic<T> atomic2; // use volatile just to exercise the templates' volatile methods
        volatile T nonatomic2;
    } data = { 0, { 0 }, { 1 }, 0 }; // test initialisation

    TEST_ASSERT_EQUAL(sizeof(T), sizeof data.nonatomic1);
    TEST_ASSERT_EQUAL(sizeof(T), sizeof data.atomic1);
    TEST_ASSERT_EQUAL(4 * sizeof(T), sizeof data);

    // test store
    data.atomic2 = 0;

    Thread t1(osPriorityNormal, THREAD_STACK);
    Thread t2(osPriorityNormal, THREAD_STACK);
    Thread t3(osPriorityNormal, THREAD_STACK);
    Thread t4(osPriorityNormal, THREAD_STACK);

    TEST_ASSERT_EQUAL(osOK, t1.start(callback(Fn<decltype(data.atomic1)>::op, &data.atomic1)));
    TEST_ASSERT_EQUAL(osOK, t2.start(callback(Fn<decltype(data.atomic1)>::op, &data.atomic1)));
    TEST_ASSERT_EQUAL(osOK, t3.start(callback(Fn<decltype(data.atomic2)>::op, &data.atomic2)));
    TEST_ASSERT_EQUAL(osOK, t4.start(callback(Fn<decltype(data.atomic2)>::op, &data.atomic2)));

    for (long i = ADD_UNLOCKED_ITERATIONS; i > 0; i--) {
        data.nonatomic1++;
        data.nonatomic2++;
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    TEST_ASSERT_EQUAL(T(ADD_UNLOCKED_ITERATIONS), data.nonatomic1);
    TEST_ASSERT_EQUAL(T(2 * add_iterations(data.atomic1)), data.atomic1);
    TEST_ASSERT_EQUAL(T(2 * add_iterations(data.atomic2)), data.atomic2);
    TEST_ASSERT_EQUAL(T(ADD_UNLOCKED_ITERATIONS), data.nonatomic2);
}

// This should fit into a uint32_t container, and there
// will be 1 byte of padding to ignore.
struct small {
    uint8_t a;
    uint8_t b;
    uint8_t c;
};

// An 11-byte weird structure. Should work with critical sections.
struct large {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t dummy[8];
};

template<typename A>
void struct_incrementer_a(A *data)
{
    for (long i = add_iterations(*data); i > 0; i--) {
        typename A::value_type curval = *data, newval;
        do {
            newval = curval;
            newval.a++;
        } while (!data->compare_exchange_weak(curval, newval));
    }
}

template<typename A>
void struct_incrementer_b(A *data)
{
    for (long i = add_iterations(*data); i > 0; i--) {
        typename A::value_type curval = *data, newval;
        do {
            newval = curval;
            newval.b++;
        } while (!data->compare_exchange_weak(curval, newval));
    }
}

template<typename T, size_t N>
void test_atomic_struct()
{
    TEST_ASSERT_EQUAL(N, sizeof(atomic<T>));

    // Small structures don't have value constructor implemented;
    atomic<T> data;
    atomic_init(&data, T{0, 0, 0});

    Thread t1(osPriorityNormal, THREAD_STACK);
    Thread t2(osPriorityNormal, THREAD_STACK);

    TEST_ASSERT_EQUAL(osOK, t1.start(callback(struct_incrementer_a<atomic<T> >, &data)));
    TEST_ASSERT_EQUAL(osOK, t2.start(callback(struct_incrementer_b<atomic<T> >, &data)));

    for (long i = add_iterations(data); i > 0; i--) {
        T curval = data, newval;
        do {
            newval = curval;
            newval.c++;
        } while (!data.compare_exchange_weak(curval, newval));
    }

    t1.join();
    t2.join();

    T final_val = data;

    TEST_ASSERT_EQUAL(uint8_t(add_iterations(data)), final_val.a);
    TEST_ASSERT_EQUAL(uint8_t(add_iterations(data)), final_val.b);
    TEST_ASSERT_EQUAL(uint8_t(add_iterations(data)), final_val.c);
}

} // namespace

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
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
    Case("Test atomic compare exchange strong 64-bit", test_atomic_add<uint64_t, strong_incrementer>),
    Case("Test small atomic custom structure", test_atomic_struct<small, 4>),
    Case("Test large atomic custom structure", test_atomic_struct<large, 11>)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}

#endif // !MBED_CONF_RTOS_PRESENT
