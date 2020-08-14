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
#include "unity.h"
#include "utest.h"
#include <mstd_functional>

using namespace utest::v1;

template <typename T>
using func0_type = T();

template <typename T>
using func1_type = T(T);

template <typename T>
using func2_type = T(T, T);

template <typename T>
using func3_type = T(T, T, T);

template <typename T>
using func4_type = T(T, T, T, T);

template <typename T>
using func5_type = T(T, T, T, T, T);

// static functions
template <typename T>
T static_func0()
{
    return 0;
}
template <typename T>
T static_func1(T a0)
{
    return 0 | a0;
}
template <typename T>
T static_func2(T a0, T a1)
{
    return 0 | a0 | a1;
}
template <typename T>
T static_func3(T a0, T a1, T a2)
{
    return 0 | a0 | a1 | a2;
}
template <typename T>
T static_func4(T a0, T a1, T a2, T a3)
{
    return 0 | a0 | a1 | a2 | a3;
}
template <typename T>
T static_func5(T a0, T a1, T a2, T a3, T a4)
{
    return 0 | a0 | a1 | a2 | a3 | a4;
}

// class functions
template <typename T>
struct Thing {
    T t;
    Thing() : t(0x80) {}

    T member_func0()
    {
        return t;
    }
    T member_func1(T a0)
    {
        return t | a0;
    }
    T member_func2(T a0, T a1)
    {
        return t | a0 | a1;
    }
    T member_func3(T a0, T a1, T a2)
    {
        return t | a0 | a1 | a2;
    }
    T member_func4(T a0, T a1, T a2, T a3)
    {
        return t | a0 | a1 | a2 | a3;
    }
    T member_func5(T a0, T a1, T a2, T a3, T a4)
    {
        return t | a0 | a1 | a2 | a3 | a4;
    }

    T const_member_func0() const
    {
        return t;
    }
    T const_member_func1(T a0) const
    {
        return t | a0;
    }
    T const_member_func2(T a0, T a1) const
    {
        return t | a0 | a1;
    }
    T const_member_func3(T a0, T a1, T a2) const
    {
        return t | a0 | a1 | a2;
    }
    T const_member_func4(T a0, T a1, T a2, T a3) const
    {
        return t | a0 | a1 | a2 | a3;
    }
    T const_member_func5(T a0, T a1, T a2, T a3, T a4) const
    {
        return t | a0 | a1 | a2 | a3 | a4;
    }

    T volatile_member_func0() volatile
    {
        return t;
    }
    T volatile_member_func1(T a0) volatile
    {
        return t | a0;
    }
    T volatile_member_func2(T a0, T a1) volatile
    {
        return t | a0 | a1;
    }
    T volatile_member_func3(T a0, T a1, T a2) volatile
    {
        return t | a0 | a1 | a2;
    }
    T volatile_member_func4(T a0, T a1, T a2, T a3) volatile
    {
        return t | a0 | a1 | a2 | a3;
    }
    T volatile_member_func5(T a0, T a1, T a2, T a3, T a4) volatile
    {
        return t | a0 | a1 | a2 | a3 | a4;
    }

    T const_volatile_member_func0() const volatile
    {
        return t;
    }
    T const_volatile_member_func1(T a0) const volatile
    {
        return t | a0;
    }
    T const_volatile_member_func2(T a0, T a1) const volatile
    {
        return t | a0 | a1;
    }
    T const_volatile_member_func3(T a0, T a1, T a2) const volatile
    {
        return t | a0 | a1 | a2;
    }
    T const_volatile_member_func4(T a0, T a1, T a2, T a3) const volatile
    {
        return t | a0 | a1 | a2 | a3;
    }
    T const_volatile_member_func5(T a0, T a1, T a2, T a3, T a4) const volatile
    {
        return t | a0 | a1 | a2 | a3 | a4;
    }
};

// bound functions
template <typename T>
T bound_func0(Thing<T> *t)
{
    return t->t;
}
template <typename T>
T bound_func1(Thing<T> *t, T a0)
{
    return t->t | a0;
}
template <typename T>
T bound_func2(Thing<T> *t, T a0, T a1)
{
    return t->t | a0 | a1;
}
template <typename T>
T bound_func3(Thing<T> *t, T a0, T a1, T a2)
{
    return t->t | a0 | a1 | a2;
}
template <typename T>
T bound_func4(Thing<T> *t, T a0, T a1, T a2, T a3)
{
    return t->t | a0 | a1 | a2 | a3;
}
template <typename T>
T bound_func5(Thing<T> *t, T a0, T a1, T a2, T a3, T a4)
{
    return t->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T const_bound_func0(const Thing<T> *t)
{
    return t->t;
}
template <typename T>
T const_bound_func1(const Thing<T> *t, T a0)
{
    return t->t | a0;
}
template <typename T>
T const_bound_func2(const Thing<T> *t, T a0, T a1)
{
    return t->t | a0 | a1;
}
template <typename T>
T const_bound_func3(const Thing<T> *t, T a0, T a1, T a2)
{
    return t->t | a0 | a1 | a2;
}
template <typename T>
T const_bound_func4(const Thing<T> *t, T a0, T a1, T a2, T a3)
{
    return t->t | a0 | a1 | a2 | a3;
}
template <typename T>
T const_bound_func5(const Thing<T> *t, T a0, T a1, T a2, T a3, T a4)
{
    return t->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T volatile_bound_func0(volatile Thing<T> *t)
{
    return t->t;
}
template <typename T>
T volatile_bound_func1(volatile Thing<T> *t, T a0)
{
    return t->t | a0;
}
template <typename T>
T volatile_bound_func2(volatile Thing<T> *t, T a0, T a1)
{
    return t->t | a0 | a1;
}
template <typename T>
T volatile_bound_func3(volatile Thing<T> *t, T a0, T a1, T a2)
{
    return t->t | a0 | a1 | a2;
}
template <typename T>
T volatile_bound_func4(volatile Thing<T> *t, T a0, T a1, T a2, T a3)
{
    return t->t | a0 | a1 | a2 | a3;
}
template <typename T>
T volatile_bound_func5(volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4)
{
    return t->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T const_volatile_bound_func0(const volatile Thing<T> *t)
{
    return t->t;
}
template <typename T>
T const_volatile_bound_func1(const volatile Thing<T> *t, T a0)
{
    return t->t | a0;
}
template <typename T>
T const_volatile_bound_func2(const volatile Thing<T> *t, T a0, T a1)
{
    return t->t | a0 | a1;
}
template <typename T>
T const_volatile_bound_func3(const volatile Thing<T> *t, T a0, T a1, T a2)
{
    return t->t | a0 | a1 | a2;
}
template <typename T>
T const_volatile_bound_func4(const volatile Thing<T> *t, T a0, T a1, T a2, T a3)
{
    return t->t | a0 | a1 | a2 | a3;
}
template <typename T>
T const_volatile_bound_func5(const volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4)
{
    return t->t | a0 | a1 | a2 | a3 | a4;
}

// void functions
template <typename T>
T void_func0(void *t)
{
    return static_cast<Thing<T>*>(t)->t;
}
template <typename T>
T void_func1(void *t, T a0)
{
    return static_cast<Thing<T>*>(t)->t | a0;
}
template <typename T>
T void_func2(void *t, T a0, T a1)
{
    return static_cast<Thing<T>*>(t)->t | a0 | a1;
}
template <typename T>
T void_func3(void *t, T a0, T a1, T a2)
{
    return static_cast<Thing<T>*>(t)->t | a0 | a1 | a2;
}
template <typename T>
T void_func4(void *t, T a0, T a1, T a2, T a3)
{
    return static_cast<Thing<T>*>(t)->t | a0 | a1 | a2 | a3;
}
template <typename T>
T void_func5(void *t, T a0, T a1, T a2, T a3, T a4)
{
    return static_cast<Thing<T>*>(t)->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T const_void_func0(const void *t)
{
    return static_cast<const Thing<T>*>(t)->t;
}
template <typename T>
T const_void_func1(const void *t, T a0)
{
    return static_cast<const Thing<T>*>(t)->t | a0;
}
template <typename T>
T const_void_func2(const void *t, T a0, T a1)
{
    return static_cast<const Thing<T>*>(t)->t | a0 | a1;
}
template <typename T>
T const_void_func3(const void *t, T a0, T a1, T a2)
{
    return static_cast<const Thing<T>*>(t)->t | a0 | a1 | a2;
}
template <typename T>
T const_void_func4(const void *t, T a0, T a1, T a2, T a3)
{
    return static_cast<const Thing<T>*>(t)->t | a0 | a1 | a2 | a3;
}
template <typename T>
T const_void_func5(const void *t, T a0, T a1, T a2, T a3, T a4)
{
    return static_cast<const Thing<T>*>(t)->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T volatile_void_func0(volatile void *t)
{
    return static_cast<volatile Thing<T>*>(t)->t;
}
template <typename T>
T volatile_void_func1(volatile void *t, T a0)
{
    return static_cast<volatile Thing<T>*>(t)->t | a0;
}
template <typename T>
T volatile_void_func2(volatile void *t, T a0, T a1)
{
    return static_cast<volatile Thing<T>*>(t)->t | a0 | a1;
}
template <typename T>
T volatile_void_func3(volatile void *t, T a0, T a1, T a2)
{
    return static_cast<volatile Thing<T>*>(t)->t | a0 | a1 | a2;
}
template <typename T>
T volatile_void_func4(volatile void *t, T a0, T a1, T a2, T a3)
{
    return static_cast<volatile Thing<T>*>(t)->t | a0 | a1 | a2 | a3;
}
template <typename T>
T volatile_void_func5(volatile void *t, T a0, T a1, T a2, T a3, T a4)
{
    return static_cast<volatile Thing<T>*>(t)->t | a0 | a1 | a2 | a3 | a4;
}
template <typename T>
T const_volatile_void_func0(const volatile void *t)
{
    return static_cast<const volatile Thing<T>*>(t)->t;
}
template <typename T>
T const_volatile_void_func1(const volatile void *t, T a0)
{
    return static_cast<const volatile Thing<T>*>(t)->t | a0;
}
template <typename T>
T const_volatile_void_func2(const volatile void *t, T a0, T a1)
{
    return static_cast<const volatile Thing<T>*>(t)->t | a0 | a1;
}
template <typename T>
T const_volatile_void_func3(const volatile void *t, T a0, T a1, T a2)
{
    return static_cast<const volatile Thing<T>*>(t)->t | a0 | a1 | a2;
}
template <typename T>
T const_volatile_void_func4(const volatile void *t, T a0, T a1, T a2, T a3)
{
    return static_cast<const volatile Thing<T>*>(t)->t | a0 | a1 | a2 | a3;
}
template <typename T>
T const_volatile_void_func5(const volatile void *t, T a0, T a1, T a2, T a3, T a4)
{
    return static_cast<const volatile Thing<T>*>(t)->t | a0 | a1 | a2 | a3 | a4;
}

// Inheriting class
template <typename T>
class Thing2 : public Thing<T> {
};


// function call and result verification
template <typename T>
struct Verifier {
    static void verify0(Callback<T()> func)
    {
        T result = func();
        TEST_ASSERT_EQUAL(result, 0x00);
    }

    template <typename O, typename M>
    static void verify0(O *obj, M method)
    {
        Callback<T()> func(obj, method);
        T result = func();
        TEST_ASSERT_EQUAL(result, 0x80);
    }

    static void verify1(Callback<T(T)> func)
    {
        T result = func((1 << 0));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0));
    }

    template <typename O, typename M>
    static void verify1(O *obj, M method)
    {
        Callback<T(T)> func(obj, method);
        T result = func((1 << 0));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0));
    }

    static void verify2(Callback<T(T, T)> func)
    {
        T result = func((1 << 0), (1 << 1));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1));
    }

    template <typename O, typename M>
    static void verify2(O *obj, M method)
    {
        Callback<T(T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1));
    }

    static void verify3(Callback<T(T, T, T)> func)
    {
        T result = func((1 << 0), (1 << 1), (1 << 2));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2));
    }

    template <typename O, typename M>
    static void verify3(O *obj, M method)
    {
        Callback<T(T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2));
    }

    static void verify4(Callback<T(T, T, T, T)> func)
    {
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
    }

    template <typename O, typename M>
    static void verify4(O *obj, M method)
    {
        Callback<T(T, T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
    }

    static void verify5(Callback<T(T, T, T, T, T)> func)
    {
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    }

    template <typename O, typename M>
    static void verify5(O *obj, M method)
    {
        Callback<T(T, T, T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    }
};


// test dispatch
template <typename T>
void test_dispatch0()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify0(static_func0<T>);
    Verifier<T>::verify0(&thing, &Thing<T>::member_func0);
    Verifier<T>::verify0((const Thing<T> *)&thing, &Thing<T>::const_member_func0);
    Verifier<T>::verify0((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func0);
    Verifier<T>::verify0((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func0);
    Verifier<T>::verify0(&thing2, &Thing2<T>::member_func0);
    Verifier<T>::verify0(&bound_func0<T>, &thing);
    Verifier<T>::verify0(&const_bound_func0<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify0(&volatile_bound_func0<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify0(&const_volatile_bound_func0<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify0(&bound_func0<T>, &thing2);
    Verifier<T>::verify0(&void_func0<T>, &thing);
    Verifier<T>::verify0(&const_void_func0<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify0(&volatile_void_func0<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify0(&const_volatile_void_func0<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify0(callback(static_func0<T>));

    Callback<T()> cb(static_func0<T>);
    Verifier<T>::verify0(cb);
    TEST_ASSERT_TRUE(cb);
    func0_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func0<T>;
    Verifier<T>::verify0(cb);
    cb = {&bound_func0<T>, &thing};
    Verifier<T>::verify0(&cb, &Callback<T()>::call);
    Verifier<T>::verify0(&Callback<T()>::thunk, &cb);
}

template <typename T>
void test_dispatch1()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify1(static_func1<T>);
    Verifier<T>::verify1(&thing, &Thing<T>::member_func1);
    Verifier<T>::verify1((const Thing<T> *)&thing, &Thing<T>::const_member_func1);
    Verifier<T>::verify1((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func1);
    Verifier<T>::verify1((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func1);
    Verifier<T>::verify1(&thing2, &Thing2<T>::member_func1);
    Verifier<T>::verify1(&bound_func1<T>, &thing);
    Verifier<T>::verify1(&const_bound_func1<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify1(&volatile_bound_func1<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify1(&const_volatile_bound_func1<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify1(&bound_func1<T>, &thing2);
    Verifier<T>::verify1(&void_func1<T>, &thing);
    Verifier<T>::verify1(&const_void_func1<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify1(&volatile_void_func1<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify1(&const_volatile_void_func1<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify1(callback(static_func1<T>));

    Callback<T(T)> cb(static_func1<T>);
    Verifier<T>::verify1(cb);
    TEST_ASSERT_TRUE(cb);
    func1_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func1<T>;
    Verifier<T>::verify1(cb);
    cb = {&bound_func1<T>, &thing};
    Verifier<T>::verify1(&cb, &Callback<T(T)>::call);
    Verifier<T>::verify1(&Callback<T(T)>::thunk, (void *)&cb);
}

template <typename T>
void test_dispatch2()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify2(static_func2<T>);
    Verifier<T>::verify2(&thing, &Thing<T>::member_func2);
    Verifier<T>::verify2((const Thing<T> *)&thing, &Thing<T>::const_member_func2);
    Verifier<T>::verify2((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func2);
    Verifier<T>::verify2((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func2);
    Verifier<T>::verify2(&thing2, &Thing2<T>::member_func2);
    Verifier<T>::verify2(&bound_func2<T>, &thing);
    Verifier<T>::verify2(&const_bound_func2<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify2(&volatile_bound_func2<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify2(&const_volatile_bound_func2<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify2(&bound_func2<T>, &thing2);
    Verifier<T>::verify2(&void_func2<T>, &thing);
    Verifier<T>::verify2(&const_void_func2<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify2(&volatile_void_func2<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify2(&const_volatile_void_func2<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify2(callback(static_func2<T>));

    Callback<T(T, T)> cb(static_func2<T>);
    Verifier<T>::verify2(cb);
    TEST_ASSERT_TRUE(cb);
    func2_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func2<T>;
    Verifier<T>::verify2(cb);
    cb = {&bound_func2<T>, &thing};
    Verifier<T>::verify2(&cb, &Callback<T(T, T)>::call);
    Verifier<T>::verify2(&Callback<T(T, T)>::thunk, (void *)&cb);
}

template <typename T>
void test_dispatch3()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify3(static_func3<T>);
    Verifier<T>::verify3(&thing, &Thing<T>::member_func3);
    Verifier<T>::verify3((const Thing<T> *)&thing, &Thing<T>::const_member_func3);
    Verifier<T>::verify3((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func3);
    Verifier<T>::verify3((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func3);
    Verifier<T>::verify3(&thing2, &Thing2<T>::member_func3);
    Verifier<T>::verify3(&bound_func3<T>, &thing);
    Verifier<T>::verify3(&const_bound_func3<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify3(&volatile_bound_func3<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify3(&const_volatile_bound_func3<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify3(&bound_func3<T>, &thing2);
    Verifier<T>::verify3(&void_func3<T>, &thing);
    Verifier<T>::verify3(&const_void_func3<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify3(&volatile_void_func3<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify3(&const_volatile_void_func3<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify3(callback(static_func3<T>));

    Callback<T(T, T, T)> cb(static_func3<T>);
    Verifier<T>::verify3(cb);
    TEST_ASSERT_TRUE(cb);
    func3_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func3<T>;
    Verifier<T>::verify3(cb);
    cb = {&bound_func3<T>, &thing};
    Verifier<T>::verify3(&cb, &Callback<T(T, T, T)>::call);
    Verifier<T>::verify3(&Callback<T(T, T, T)>::thunk, (void *)&cb);
}

template <typename T>
void test_dispatch4()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify4(static_func4<T>);
    Verifier<T>::verify4(&thing, &Thing<T>::member_func4);
    Verifier<T>::verify4((const Thing<T> *)&thing, &Thing<T>::const_member_func4);
    Verifier<T>::verify4((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func4);
    Verifier<T>::verify4((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func4);
    Verifier<T>::verify4(&thing2, &Thing2<T>::member_func4);
    Verifier<T>::verify4(&bound_func4<T>, &thing);
    Verifier<T>::verify4(&const_bound_func4<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify4(&volatile_bound_func4<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify4(&const_volatile_bound_func4<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify4(&bound_func4<T>, &thing2);
    Verifier<T>::verify4(&void_func4<T>, &thing);
    Verifier<T>::verify4(&const_void_func4<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify4(&volatile_void_func4<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify4(&const_volatile_void_func4<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify4(callback(static_func4<T>));

    Callback<T(T, T, T, T)> cb(static_func4<T>);
    Verifier<T>::verify4(cb);
    TEST_ASSERT_TRUE(cb);
    func4_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func4<T>;
    Verifier<T>::verify4(cb);
    cb = {&bound_func4<T>, &thing};
    Verifier<T>::verify4(&cb, &Callback<T(T, T, T, T)>::call);
    Verifier<T>::verify4(&Callback<T(T, T, T, T)>::thunk, (void *)&cb);
}

template <typename T>
void test_dispatch5()
{
    Thing<T> thing;
    Thing2<T> thing2;
    Verifier<T>::verify5(static_func5<T>);
    Verifier<T>::verify5(&thing, &Thing<T>::member_func5);
    Verifier<T>::verify5((const Thing<T> *)&thing, &Thing<T>::const_member_func5);
    Verifier<T>::verify5((volatile Thing<T> *)&thing, &Thing<T>::volatile_member_func5);
    Verifier<T>::verify5((const volatile Thing<T> *)&thing, &Thing<T>::const_volatile_member_func5);
    Verifier<T>::verify5(&thing2, &Thing2<T>::member_func5);
    Verifier<T>::verify5(&bound_func5<T>, &thing);
    Verifier<T>::verify5(&const_bound_func5<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify5(&volatile_bound_func5<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify5(&const_volatile_bound_func5<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify5(&bound_func5<T>, &thing2);
    Verifier<T>::verify5(&void_func5<T>, &thing);
    Verifier<T>::verify5(&const_void_func5<T>, (const Thing<T> *)&thing);
    Verifier<T>::verify5(&volatile_void_func5<T>, (volatile Thing<T> *)&thing);
    Verifier<T>::verify5(&const_volatile_void_func5<T>, (const volatile Thing<T> *)&thing);
    Verifier<T>::verify5(callback(static_func5<T>));

    Callback<T(T, T, T, T, T)> cb(static_func5<T>);
    Verifier<T>::verify5(cb);
    TEST_ASSERT_TRUE(cb);
    func5_type<T> *p = nullptr;
    cb = p;
    TEST_ASSERT_FALSE(cb);
    cb = static_func5<T>;
    Verifier<T>::verify5(cb);
    cb = {&bound_func5<T>, &thing};
    Verifier<T>::verify5(&cb, &Callback<T(T, T, T, T, T)>::call);
#if 0
    Verifier<T>::verify5(&Callback<T(T, T, T, T, T)>::thunk, (void *)&cb);
#endif
}

#include <mstd_functional>

struct TrivialFunctionObject {
    TrivialFunctionObject(int n) : val(n)
    {
    }

    int operator()(int x) const
    {
        return x + val;
    }
private:
    int val;
};

/* Exact count of copy, construction and destruction may vary depending on
 * copy elision by compiler, but the derived live count can be relied upon.
 */
static int construct_count;
static int destruct_count;
static int copy_count;

static int live_count()
{
    return construct_count - destruct_count;
}

struct FunctionObject {
    FunctionObject(int n) : val(n)
    {
        construct_count++;
    }

    FunctionObject(const FunctionObject &other) : val(other.val)
    {
        construct_count++;
        copy_count++;
    }

    ~FunctionObject()
    {
        destruct_count++;
        destroyed = true;
    }

    int operator()(int x) const
    {
        return destroyed ? -1000 : x + val;
    }
private:
    const int val;
    bool destroyed = false;
};

void test_trivial()
{
    TrivialFunctionObject fn(1);
    TEST_ASSERT_EQUAL(2, fn(1));
    Callback<int(int)> cb(fn);
    TEST_ASSERT_TRUE(cb);
    TEST_ASSERT_EQUAL(2, cb(1));
    fn = 5;
    TEST_ASSERT_EQUAL(6, fn(1));
    TEST_ASSERT_EQUAL(2, cb(1));
    cb = std::ref(fn);
    fn = 10;
    TEST_ASSERT_EQUAL(11, fn(1));
    TEST_ASSERT_EQUAL(11, cb(1));
    cb = TrivialFunctionObject(3);
    TEST_ASSERT_EQUAL(7, cb(4));
    cb = nullptr;
    TEST_ASSERT_FALSE(cb);
    cb = TrivialFunctionObject(7);
    Callback<int(int)> cb2(cb);
    TEST_ASSERT_EQUAL(8, cb(1));
    TEST_ASSERT_EQUAL(9, cb2(2));
    cb2 = cb;
    TEST_ASSERT_EQUAL(6, cb2(-1));
    cb = cb;
    TEST_ASSERT_EQUAL(8, cb(1));
    cb = std::negate<int>();
    TEST_ASSERT_EQUAL(-4, cb(4));
    cb = [](int x) {
        return x - 7;
    };
    TEST_ASSERT_EQUAL(1, cb(8));
    cb = cb2 = nullptr;
    TEST_ASSERT_FALSE(cb);
}

#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
void test_nontrivial()
{
    {
        FunctionObject fn(1);
        TEST_ASSERT_EQUAL(1, construct_count);
        TEST_ASSERT_EQUAL(0, destruct_count);
        TEST_ASSERT_EQUAL(2, fn(1));
        Callback<int(int)> cb(fn);
        TEST_ASSERT_TRUE(cb);
        TEST_ASSERT_EQUAL(2, live_count());
        TEST_ASSERT_EQUAL(2, cb(1));
        cb = std::ref(fn);
        TEST_ASSERT_EQUAL(1, live_count());
        TEST_ASSERT_EQUAL(5, cb(4));
        cb = FunctionObject(3);
        TEST_ASSERT_EQUAL(2, live_count());
        TEST_ASSERT_EQUAL(7, cb(4));
        cb = nullptr;
        TEST_ASSERT_FALSE(cb);
        TEST_ASSERT_EQUAL(1, live_count());
        cb = FunctionObject(7);
        TEST_ASSERT_EQUAL(2, live_count());
        int old_copy_count = copy_count;
        Callback<int(int)> cb2(cb);
        TEST_ASSERT_EQUAL(old_copy_count + 1, copy_count);
        TEST_ASSERT_EQUAL(3, live_count());
        TEST_ASSERT_EQUAL(8, cb(1));
        TEST_ASSERT_EQUAL(9, cb2(2));
        old_copy_count = copy_count;
        cb2 = cb;
        TEST_ASSERT_EQUAL(old_copy_count + 1, copy_count);
        TEST_ASSERT_EQUAL(3, live_count());
        TEST_ASSERT_EQUAL(6, cb2(-1));
        int old_construct_count = construct_count;
        old_copy_count = copy_count;
        cb = cb;
        TEST_ASSERT_EQUAL(3, live_count());
        TEST_ASSERT_EQUAL(old_construct_count, construct_count);
        TEST_ASSERT_EQUAL(old_copy_count, copy_count);
        cb = cb2 = nullptr;
        TEST_ASSERT_FALSE(cb);
        TEST_ASSERT_EQUAL(1, live_count());
    }
    TEST_ASSERT_EQUAL(0, live_count());
}
#endif


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
#ifdef DO_BIG_TEST
    Case("Testing callbacks with 0 uint64s", test_dispatch0<uint64_t>),
    Case("Testing callbacks with 1 uint64s", test_dispatch1<uint64_t>),
    Case("Testing callbacks with 2 uint64s", test_dispatch2<uint64_t>),
    Case("Testing callbacks with 3 uint64s", test_dispatch3<uint64_t>),
    Case("Testing callbacks with 4 uint64s", test_dispatch4<uint64_t>),
// IAR currently crashes at link time with this test - skip it as it's well beyond anything needed by real code
#ifndef __ICCARM__
    Case("Testing callbacks with 5 uint64s", test_dispatch5<uint64_t>),
#endif
#elif DO_SMALL_TEST
    Case("Testing callbacks with 0 uchars", test_dispatch0<unsigned char>),
    Case("Testing callbacks with 1 uchars", test_dispatch1<unsigned char>),
    Case("Testing callbacks with 2 uchars", test_dispatch2<unsigned char>),
    Case("Testing callbacks with 3 uchars", test_dispatch3<unsigned char>),
    Case("Testing callbacks with 4 uchars", test_dispatch4<unsigned char>),
    Case("Testing callbacks with 5 uchars", test_dispatch5<unsigned char>),
#else
    Case("Testing callbacks with 0 ints", test_dispatch0<int>),
    Case("Testing callbacks with 1 ints", test_dispatch1<int>),
    Case("Testing callbacks with 2 ints", test_dispatch2<int>),
    Case("Testing callbacks with 3 ints", test_dispatch3<int>),
    Case("Testing callbacks with 4 ints", test_dispatch4<int>),
    Case("Testing callbacks with 5 ints", test_dispatch5<int>),
    Case("Testing trivial function object", test_trivial),
#if MBED_CONF_PLATFORM_CALLBACK_NONTRIVIAL
    Case("Testing non-trivial function object", test_nontrivial),
#endif
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
