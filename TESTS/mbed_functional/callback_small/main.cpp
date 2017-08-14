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
#include "unity.h"
#include "utest.h"

using namespace utest::v1;


// static functions
template <typename T>
T static_func0() { return 0; }
template <typename T>
T static_func1(T a0) { return 0 | a0; }
template <typename T>
T static_func2(T a0, T a1) { return 0 | a0 | a1; }
template <typename T>
T static_func3(T a0, T a1, T a2) { return 0 | a0 | a1 | a2; }
template <typename T>
T static_func4(T a0, T a1, T a2, T a3) { return 0 | a0 | a1 | a2 | a3; }
template <typename T>
T static_func5(T a0, T a1, T a2, T a3, T a4) { return 0 | a0 | a1 | a2 | a3 | a4; }

// class functions
template <typename T>
struct Thing {
    T t;
    Thing() : t(0x80) {}

    T member_func0() { return t; }
    T member_func1(T a0) { return t | a0; }
    T member_func2(T a0, T a1) { return t | a0 | a1; }
    T member_func3(T a0, T a1, T a2) { return t | a0 | a1 | a2; }
    T member_func4(T a0, T a1, T a2, T a3) { return t | a0 | a1 | a2 | a3; }
    T member_func5(T a0, T a1, T a2, T a3, T a4) { return t | a0 | a1 | a2 | a3 | a4; }

    T const_member_func0() const { return t; }
    T const_member_func1(T a0) const { return t | a0; }
    T const_member_func2(T a0, T a1) const { return t | a0 | a1; }
    T const_member_func3(T a0, T a1, T a2) const { return t | a0 | a1 | a2; }
    T const_member_func4(T a0, T a1, T a2, T a3) const { return t | a0 | a1 | a2 | a3; }
    T const_member_func5(T a0, T a1, T a2, T a3, T a4) const { return t | a0 | a1 | a2 | a3 | a4; }

    T volatile_member_func0() volatile { return t; }
    T volatile_member_func1(T a0) volatile { return t | a0; }
    T volatile_member_func2(T a0, T a1) volatile { return t | a0 | a1; }
    T volatile_member_func3(T a0, T a1, T a2) volatile { return t | a0 | a1 | a2; }
    T volatile_member_func4(T a0, T a1, T a2, T a3) volatile { return t | a0 | a1 | a2 | a3; }
    T volatile_member_func5(T a0, T a1, T a2, T a3, T a4) volatile { return t | a0 | a1 | a2 | a3 | a4; }

    T const_volatile_member_func0() const volatile { return t; }
    T const_volatile_member_func1(T a0) const volatile { return t | a0; }
    T const_volatile_member_func2(T a0, T a1) const volatile { return t | a0 | a1; }
    T const_volatile_member_func3(T a0, T a1, T a2) const volatile { return t | a0 | a1 | a2; }
    T const_volatile_member_func4(T a0, T a1, T a2, T a3) const volatile { return t | a0 | a1 | a2 | a3; }
    T const_volatile_member_func5(T a0, T a1, T a2, T a3, T a4) const volatile { return t | a0 | a1 | a2 | a3 | a4; }
};

// bound functions
template <typename T>
T bound_func0(Thing<T> *t) { return t->t; }
template <typename T>
T bound_func1(Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T bound_func2(Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T bound_func3(Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T bound_func4(Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T bound_func5(Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }

// const bound functions
template <typename T>
T const_func0(const Thing<T> *t) { return t->t; }
template <typename T>
T const_func1(const Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T const_func2(const Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T const_func3(const Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T const_func4(const Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T const_func5(const Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }

// volatile bound functions
template <typename T>
T volatile_func0(volatile Thing<T> *t) { return t->t; }
template <typename T>
T volatile_func1(volatile Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T volatile_func2(volatile Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T volatile_func3(volatile Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T volatile_func4(volatile Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T volatile_func5(volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }

// const volatile bound functions
template <typename T>
T const_volatile_func0(const volatile Thing<T> *t) { return t->t; }
template <typename T>
T const_volatile_func1(const volatile Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T const_volatile_func2(const volatile Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T const_volatile_func3(const volatile Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T const_volatile_func4(const volatile Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T const_volatile_func5(const volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }


// function call and result verification
template <typename T>
struct Verifier {
    static void verify0(Callback<T()> func) {
        T result = func();
        TEST_ASSERT_EQUAL(result, 0x00);
    }

    template <typename O, typename M>
    static void verify0(O *obj, M method) {
        Callback<T()> func(obj, method);
        T result = func();
        TEST_ASSERT_EQUAL(result, 0x80);
    }

    static void verify1(Callback<T(T)> func) {
        T result = func((1 << 0));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0));
    }

    template <typename O, typename M>
    static void verify1(O *obj, M method) {
        Callback<T(T)> func(obj, method);
        T result = func((1 << 0));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0));
    }

    static void verify2(Callback<T(T, T)> func) {
        T result = func((1 << 0), (1 << 1));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1));
    }

    template <typename O, typename M>
    static void verify2(O *obj, M method) {
        Callback<T(T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1));
    }

    static void verify3(Callback<T(T, T, T)> func) {
        T result = func((1 << 0), (1 << 1), (1 << 2));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2));
    }

    template <typename O, typename M>
    static void verify3(O *obj, M method) {
        Callback<T(T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2));
    }

    static void verify4(Callback<T(T, T, T, T)> func) {
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
    }

    template <typename O, typename M>
    static void verify4(O *obj, M method) {
        Callback<T(T, T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
    }

    static void verify5(Callback<T(T, T, T, T, T)> func) {
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4));
        TEST_ASSERT_EQUAL(result, 0x00 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    }

    template <typename O, typename M>
    static void verify5(O *obj, M method) {
        Callback<T(T, T, T, T, T)> func(obj, method);
        T result = func((1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4));
        TEST_ASSERT_EQUAL(result, 0x80 | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    }
};


// test dispatch
template <typename T>
void test_dispatch0() {
    Thing<T> thing;
    Verifier<T>::verify0(static_func0<T>);
    Verifier<T>::verify0(&thing, &Thing<T>::member_func0);
    Verifier<T>::verify0((const Thing<T>*)&thing, &Thing<T>::const_member_func0);
    Verifier<T>::verify0((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func0);
    Verifier<T>::verify0((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func0);
    Verifier<T>::verify0(&bound_func0<T>, &thing);
    Verifier<T>::verify0(&const_func0<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify0(&volatile_func0<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify0(&const_volatile_func0<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify0(callback(static_func0<T>));

    Callback<T()> cb(static_func0);
    Verifier<T>::verify0(cb);
    cb = static_func0;
    Verifier<T>::verify0(cb);
    cb.attach(&bound_func0<T>, &thing);
    Verifier<T>::verify0(&cb, &Callback<T()>::call);
    Verifier<T>::verify0(&Callback<T()>::thunk, (void*)&cb);
}

template <typename T>
void test_dispatch1() {
    Thing<T> thing;
    Verifier<T>::verify1(static_func1<T>);
    Verifier<T>::verify1(&thing, &Thing<T>::member_func1);
    Verifier<T>::verify1((const Thing<T>*)&thing, &Thing<T>::const_member_func1);
    Verifier<T>::verify1((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func1);
    Verifier<T>::verify1((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func1);
    Verifier<T>::verify1(&bound_func1<T>, &thing);
    Verifier<T>::verify1(&const_func1<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify1(&volatile_func1<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify1(&const_volatile_func1<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify1(callback(static_func1<T>));

    Callback<T(T)> cb(static_func1);
    Verifier<T>::verify1(cb);
    cb = static_func1;
    Verifier<T>::verify1(cb);
    cb.attach(&bound_func1<T>, &thing);
    Verifier<T>::verify1(&cb, &Callback<T(T)>::call);
    Verifier<T>::verify1(&Callback<T(T)>::thunk, (void*)&cb);
}

template <typename T>
void test_dispatch2() {
    Thing<T> thing;
    Verifier<T>::verify2(static_func2<T>);
    Verifier<T>::verify2(&thing, &Thing<T>::member_func2);
    Verifier<T>::verify2((const Thing<T>*)&thing, &Thing<T>::const_member_func2);
    Verifier<T>::verify2((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func2);
    Verifier<T>::verify2((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func2);
    Verifier<T>::verify2(&bound_func2<T>, &thing);
    Verifier<T>::verify2(&const_func2<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify2(&volatile_func2<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify2(&const_volatile_func2<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify2(callback(static_func2<T>));

    Callback<T(T, T)> cb(static_func2);
    Verifier<T>::verify2(cb);
    cb = static_func2;
    Verifier<T>::verify2(cb);
    cb.attach(&bound_func2<T>, &thing);
    Verifier<T>::verify2(&cb, &Callback<T(T, T)>::call);
    Verifier<T>::verify2(&Callback<T(T, T)>::thunk, (void*)&cb);
}

template <typename T>
void test_dispatch3() {
    Thing<T> thing;
    Verifier<T>::verify3(static_func3<T>);
    Verifier<T>::verify3(&thing, &Thing<T>::member_func3);
    Verifier<T>::verify3((const Thing<T>*)&thing, &Thing<T>::const_member_func3);
    Verifier<T>::verify3((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func3);
    Verifier<T>::verify3((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func3);
    Verifier<T>::verify3(&bound_func3<T>, &thing);
    Verifier<T>::verify3(&const_func3<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify3(&volatile_func3<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify3(&const_volatile_func3<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify3(callback(static_func3<T>));

    Callback<T(T, T, T)> cb(static_func3);
    Verifier<T>::verify3(cb);
    cb = static_func3;
    Verifier<T>::verify3(cb);
    cb.attach(&bound_func3<T>, &thing);
    Verifier<T>::verify3(&cb, &Callback<T(T, T, T)>::call);
    Verifier<T>::verify3(&Callback<T(T, T, T)>::thunk, (void*)&cb);
}

template <typename T>
void test_dispatch4() {
    Thing<T> thing;
    Verifier<T>::verify4(static_func4<T>);
    Verifier<T>::verify4(&thing, &Thing<T>::member_func4);
    Verifier<T>::verify4((const Thing<T>*)&thing, &Thing<T>::const_member_func4);
    Verifier<T>::verify4((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func4);
    Verifier<T>::verify4((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func4);
    Verifier<T>::verify4(&bound_func4<T>, &thing);
    Verifier<T>::verify4(&const_func4<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify4(&volatile_func4<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify4(&const_volatile_func4<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify4(callback(static_func4<T>));

    Callback<T(T, T, T, T)> cb(static_func4);
    Verifier<T>::verify4(cb);
    cb = static_func4;
    Verifier<T>::verify4(cb);
    cb.attach(&bound_func4<T>, &thing);
    Verifier<T>::verify4(&cb, &Callback<T(T, T, T, T)>::call);
    Verifier<T>::verify4(&Callback<T(T, T, T, T)>::thunk, (void*)&cb);
}

template <typename T>
void test_dispatch5() {
    Thing<T> thing;
    Verifier<T>::verify5(static_func5<T>);
    Verifier<T>::verify5(&thing, &Thing<T>::member_func5);
    Verifier<T>::verify5((const Thing<T>*)&thing, &Thing<T>::const_member_func5);
    Verifier<T>::verify5((volatile Thing<T>*)&thing, &Thing<T>::volatile_member_func5);
    Verifier<T>::verify5((const volatile Thing<T>*)&thing, &Thing<T>::const_volatile_member_func5);
    Verifier<T>::verify5(&bound_func5<T>, &thing);
    Verifier<T>::verify5(&const_func5<T>, (const Thing<T>*)&thing);
    Verifier<T>::verify5(&volatile_func5<T>, (volatile Thing<T>*)&thing);
    Verifier<T>::verify5(&const_volatile_func5<T>, (const volatile Thing<T>*)&thing);
    Verifier<T>::verify5(callback(static_func5<T>));

    Callback<T(T, T, T, T, T)> cb(static_func5);
    Verifier<T>::verify5(cb);
    cb = static_func5;
    Verifier<T>::verify5(cb);
    cb.attach(&bound_func5<T>, &thing);
    Verifier<T>::verify5(&cb, &Callback<T(T, T, T, T, T)>::call);
    Verifier<T>::verify5(&Callback<T(T, T, T, T, T)>::thunk, (void*)&cb);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing callbacks with 0 uchars", test_dispatch0<unsigned char>),
    Case("Testing callbacks with 1 uchars", test_dispatch1<unsigned char>),
    Case("Testing callbacks with 2 uchars", test_dispatch2<unsigned char>),
    Case("Testing callbacks with 3 uchars", test_dispatch3<unsigned char>),
    Case("Testing callbacks with 4 uchars", test_dispatch4<unsigned char>),
    Case("Testing callbacks with 5 uchars", test_dispatch5<unsigned char>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
