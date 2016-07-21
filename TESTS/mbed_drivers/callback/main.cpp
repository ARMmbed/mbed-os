#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;


// static functions
template <typename T>
T static_func5(T a0, T a1, T a2, T a3, T a4) { return a0 | a1 | a2 | a3 | a4; }
template <typename T>
T static_func4(T a0, T a1, T a2, T a3) { return a0 | a1 | a2 | a3; }
template <typename T>
T static_func3(T a0, T a1, T a2) { return a0 | a1 | a2; }
template <typename T>
T static_func2(T a0, T a1) { return a0 | a1; }
template <typename T>
T static_func1(T a0) { return a0; }
template <typename T>
T static_func0() { return 0; }

// class functions
template <typename T>
struct Thing {
    T t;
    Thing() : t(0x80) {}

    T member_func5(T a0, T a1, T a2, T a3, T a4) { return t | a0 | a1 | a2 | a3 | a4; }
    T member_func4(T a0, T a1, T a2, T a3) { return t | a0 | a1 | a2 | a3; }
    T member_func3(T a0, T a1, T a2) { return t | a0 | a1 | a2; }
    T member_func2(T a0, T a1) { return t | a0 | a1; }
    T member_func1(T a0) { return t | a0; }
    T member_func0() { return t; }
};

// bound functions
template <typename T>
T bound_func5(Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }
template <typename T>
T bound_func4(Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T bound_func3(Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T bound_func2(Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T bound_func1(Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T bound_func0(Thing<T> *t) { return t->t; }

// const bound functions
template <typename T>
T const_func5(const Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }
template <typename T>
T const_func4(const Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T const_func3(const Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T const_func2(const Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T const_func1(const Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T const_func0(const Thing<T> *t) { return t->t; }

// volatile bound functions
template <typename T>
T volatile_func5(volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }
template <typename T>
T volatile_func4(volatile Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T volatile_func3(volatile Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T volatile_func2(volatile Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T volatile_func1(volatile Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T volatile_func0(volatile Thing<T> *t) { return t->t; }

// const volatil bound functions
template <typename T>
T const_volatile_func5(const volatile Thing<T> *t, T a0, T a1, T a2, T a3, T a4) { return t->t | a0 | a1 | a2 | a3 | a4; }
template <typename T>
T const_volatile_func4(const volatile Thing<T> *t, T a0, T a1, T a2, T a3) { return t->t | a0 | a1 | a2 | a3; }
template <typename T>
T const_volatile_func3(const volatile Thing<T> *t, T a0, T a1, T a2) { return t->t | a0 | a1 | a2; }
template <typename T>
T const_volatile_func2(const volatile Thing<T> *t, T a0, T a1) { return t->t | a0 | a1; }
template <typename T>
T const_volatile_func1(const volatile Thing<T> *t, T a0) { return t->t | a0; }
template <typename T>
T const_volatile_func0(const volatile Thing<T> *t) { return t->t; }


// function call and result verification
template <typename T>
struct Verifier {
    static void verify5(Callback<T(T,T,T,T,T)> func) {
        T result = func(0x01, 0x02, 0x04, 0x08, 0x10);
        TEST_ASSERT_EQUAL(result, 0x1f);
    }

    template <typename O, typename M>
    static void verify5(O *obj, M method) {
        Callback<T(T,T,T,T,T)> func(obj, method);
        T result = func(0x01, 0x02, 0x04, 0x08, 0x10);
        TEST_ASSERT_EQUAL(result, 0x9f);
    }

    static void verify4(Callback<T(T,T,T,T)> func) {
        T result = func(0x01, 0x02, 0x04, 0x08);
        TEST_ASSERT_EQUAL(result, 0x0f);
    }

    template <typename O, typename M>
    static void verify4(O *obj, M method) {
        Callback<T(T,T,T,T)> func(obj, method);
        T result = func(0x01, 0x02, 0x04, 0x08);
        TEST_ASSERT_EQUAL(result, 0x8f);
    }

    static void verify3(Callback<T(T,T,T)> func) {
        T result = func(0x01, 0x02, 0x04);
        TEST_ASSERT_EQUAL(result, 0x07);
    }

    template <typename O, typename M>
    static void verify3(O *obj, M method) {
        Callback<T(T,T,T)> func(obj, method);
        T result = func(0x01, 0x02, 0x04);
        TEST_ASSERT_EQUAL(result, 0x87);
    }

    static void verify2(Callback<T(T,T)> func) {
        T result = func(0x01, 0x02);
        TEST_ASSERT_EQUAL(result, 0x03);
    }

    template <typename O, typename M>
    static void verify2(O *obj, M method) {
        Callback<T(T,T)> func(obj, method);
        T result = func(0x01, 0x02);
        TEST_ASSERT_EQUAL(result, 0x83);
    }

    static void verify1(Callback<T(T)> func) {
        T result = func(0x01);
        TEST_ASSERT_EQUAL(result, 0x01);
    }

    template <typename O, typename M>
    static void verify1(O *obj, M method) {
        Callback<T(T)> func(obj, method);
        T result = func(0x01);
        TEST_ASSERT_EQUAL(result, 0x81);
    }

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
};


// test dispatch
template <typename T>
void test_dispatch5() {
    Thing<T> thing;
    Verifier<T>::verify5(static_func5<T>);
    Verifier<T>::verify5(&thing, &Thing<T>::member_func5);
    Verifier<T>::verify5(&thing, &bound_func5<T>);
    Verifier<T>::verify5((const Thing<T>*)&thing, &const_func5<T>);
    Verifier<T>::verify5((volatile Thing<T>*)&thing, &volatile_func5<T>);
    Verifier<T>::verify5((const volatile Thing<T>*)&thing, &const_volatile_func5<T>);

    Callback<T(T,T,T,T,T)> callback(static_func5);
    Verifier<T>::verify5(callback);
    callback.attach(&thing, &bound_func5<T>);
    Verifier<T>::verify5(&callback, &Callback<T(T,T,T,T,T)>::call);
    Verifier<T>::verify5((void*)&callback, &Callback<T(T,T,T,T,T)>::thunk);
}

template <typename T>
void test_dispatch4() {
    Thing<T> thing;
    Verifier<T>::verify4(static_func4<T>);
    Verifier<T>::verify4(&thing, &Thing<T>::member_func4);
    Verifier<T>::verify4(&thing, &bound_func4<T>);
    Verifier<T>::verify4((const Thing<T>*)&thing, &const_func4<T>);
    Verifier<T>::verify4((volatile Thing<T>*)&thing, &volatile_func4<T>);
    Verifier<T>::verify4((const volatile Thing<T>*)&thing, &const_volatile_func4<T>);

    Callback<T(T,T,T,T)> callback(static_func4);
    Verifier<T>::verify4(callback);
    callback.attach(&thing, &bound_func4<T>);
    Verifier<T>::verify4(&callback, &Callback<T(T,T,T,T)>::call);
    Verifier<T>::verify4((void*)&callback, &Callback<T(T,T,T,T)>::thunk);
}

template <typename T>
void test_dispatch3() {
    Thing<T> thing;
    Verifier<T>::verify3(static_func3<T>);
    Verifier<T>::verify3(&thing, &Thing<T>::member_func3);
    Verifier<T>::verify3(&thing, &bound_func3<T>);
    Verifier<T>::verify3((const Thing<T>*)&thing, &const_func3<T>);
    Verifier<T>::verify3((volatile Thing<T>*)&thing, &volatile_func3<T>);
    Verifier<T>::verify3((const volatile Thing<T>*)&thing, &const_volatile_func3<T>);

    Callback<T(T,T,T)> callback(static_func3);
    Verifier<T>::verify3(callback);
    callback.attach(&thing, &bound_func3<T>);
    Verifier<T>::verify3(&callback, &Callback<T(T,T,T)>::call);
    Verifier<T>::verify3((void*)&callback, &Callback<T(T,T,T)>::thunk);
}

template <typename T>
void test_dispatch2() {
    Thing<T> thing;
    Verifier<T>::verify2(static_func2<T>);
    Verifier<T>::verify2(&thing, &Thing<T>::member_func2);
    Verifier<T>::verify2(&thing, &bound_func2<T>);
    Verifier<T>::verify2((const Thing<T>*)&thing, &const_func2<T>);
    Verifier<T>::verify2((volatile Thing<T>*)&thing, &volatile_func2<T>);
    Verifier<T>::verify2((const volatile Thing<T>*)&thing, &const_volatile_func2<T>);

    Callback<T(T,T)> callback(static_func2);
    Verifier<T>::verify2(callback);
    callback.attach(&thing, &bound_func2<T>);
    Verifier<T>::verify2(&callback, &Callback<T(T,T)>::call);
    Verifier<T>::verify2((void*)&callback, &Callback<T(T,T)>::thunk);
}

template <typename T>
void test_dispatch1() {
    Thing<T> thing;
    Verifier<T>::verify1(static_func1<T>);
    Verifier<T>::verify1(&thing, &Thing<T>::member_func1);
    Verifier<T>::verify1(&thing, &bound_func1<T>);
    Verifier<T>::verify1((const Thing<T>*)&thing, &const_func1<T>);
    Verifier<T>::verify1((volatile Thing<T>*)&thing, &volatile_func1<T>);
    Verifier<T>::verify1((const volatile Thing<T>*)&thing, &const_volatile_func1<T>);

    Callback<T(T)> callback(static_func1);
    Verifier<T>::verify1(callback);
    callback.attach(&thing, &bound_func1<T>);
    Verifier<T>::verify1(&callback, &Callback<T(T)>::call);
    Verifier<T>::verify1((void*)&callback, &Callback<T(T)>::thunk);
}

template <typename T>
void test_dispatch0() {
    Thing<T> thing;
    Verifier<T>::verify0(static_func0<T>);
    Verifier<T>::verify0(&thing, &Thing<T>::member_func0);
    Verifier<T>::verify0(&thing, &bound_func0<T>);
    Verifier<T>::verify0((const Thing<T>*)&thing, &const_func0<T>);
    Verifier<T>::verify0((volatile Thing<T>*)&thing, &volatile_func0<T>);
    Verifier<T>::verify0((const volatile Thing<T>*)&thing, &const_volatile_func0<T>);

    Callback<T()> callback(static_func0);
    Verifier<T>::verify0(callback);
    callback.attach(&thing, &bound_func0<T>);
    Verifier<T>::verify0(&callback, &Callback<T()>::call);
    Verifier<T>::verify0((void*)&callback, &Callback<T()>::thunk);
}

template <typename T>
void test_fparg1() {
    Thing<T> thing;
    FunctionPointerArg1<T,T> fp(static_func1<T>);
    Verifier<T>::verify1(fp);
    Verifier<T>::verify1(fp.get_function());
}

template <typename T>
void test_fparg0() {
    Thing<T> thing;
    FunctionPointerArg1<T,void> fp(static_func0<T>);
    Verifier<T>::verify0(fp);
    Verifier<T>::verify0(fp.get_function());
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing callbacks with 5 ints", test_dispatch5<int>),
    Case("Testing callbacks with 4 ints", test_dispatch4<int>),
    Case("Testing callbacks with 3 ints", test_dispatch3<int>),
    Case("Testing callbacks with 2 ints", test_dispatch2<int>),
    Case("Testing callbacks with 1 ints", test_dispatch1<int>),
    Case("Testing callbacks with 0 ints", test_dispatch0<int>),

    Case("Testing callbacks with 5 uchars", test_dispatch5<unsigned char>),
    Case("Testing callbacks with 4 uchars", test_dispatch4<unsigned char>),
    Case("Testing callbacks with 3 uchars", test_dispatch3<unsigned char>),
    Case("Testing callbacks with 2 uchars", test_dispatch2<unsigned char>),
    Case("Testing callbacks with 1 uchars", test_dispatch1<unsigned char>),
    Case("Testing callbacks with 0 uchars", test_dispatch0<unsigned char>),

    Case("Testing callbacks with 5 uint64s", test_dispatch5<uint64_t>),
    Case("Testing callbacks with 4 uint64s", test_dispatch4<uint64_t>),
    Case("Testing callbacks with 3 uint64s", test_dispatch3<uint64_t>),
    Case("Testing callbacks with 2 uint64s", test_dispatch2<uint64_t>),
    Case("Testing callbacks with 1 uint64s", test_dispatch1<uint64_t>),
    Case("Testing callbacks with 0 uint64s", test_dispatch0<uint64_t>),

    Case("Testing FunctionPointerArg1 compatibility", test_fparg1<int>),
    Case("Testing FunctionPointer compatibility", test_fparg0<int>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
