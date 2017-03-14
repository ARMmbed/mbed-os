#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;

// static functions
void func0() {
    TEST_ASSERT(true);
}

void func1(int a) {
    TEST_ASSERT(a == 1);
}

void func2(int a, int b) {
    TEST_ASSERT(a == 1 && b == 2);
}

void func3(int a, int b, int c) {
    TEST_ASSERT(a == 1 && b == 2 && c == 3);
}

void func4(int a, int b, int c, int d) {
    TEST_ASSERT(a == 1 && b == 2 && c == 3 && d == 4);
}

void func5(int a, int b, int c, int d, int e) {
    TEST_ASSERT(a == 1 && b == 2 && c == 3 && d == 4 && e == 5);
}

// member functions
struct Thing {
    int t;

    void func0() {
        TEST_ASSERT(t == -1);
    }

    void func1(int a) {
        TEST_ASSERT(t == -1 && a == 1);
    }

    void func2(int a, int b) {
        TEST_ASSERT(t == -1 && a == 1 && b == 2);
    }

    void func3(int a, int b, int c) {
        TEST_ASSERT(t == -1 && a == 1 && b == 2 && c == 3);
    }

    void func4(int a, int b, int c, int d) {
        TEST_ASSERT(t == -1 && a == 1 && b == 2 && c == 3 && d == 4);
    }

    void func5(int a, int b, int c, int d, int e) {
        TEST_ASSERT(t == -1 && a == 1 && b == 2 && c == 3 && d == 4 && e == 5);
    }
};

void test_bind_static() {
    bind(func0)();
    bind(func1)(1);
    bind(func2)(1, 2);
    bind(func3)(1, 2, 3);
    bind(func4)(1, 2, 3, 4);
    bind(func5)(1, 2, 3, 4, 5);
    bind(func1, 1)();
    bind(func2, 1)(2);
    bind(func3, 1)(2, 3);
    bind(func4, 1)(2, 3, 4);
    bind(func5, 1)(2, 3, 4, 5);
    bind(func2, 1, 2)();
    bind(func3, 1, 2)(3);
    bind(func4, 1, 2)(3, 4);
    bind(func5, 1, 2)(3, 4, 5);
    bind(func3, 1, 2, 3)();
    bind(func4, 1, 2, 3)(4);
    bind(func5, 1, 2, 3)(4, 5);
    bind(func4, 1, 2, 3, 4)();
    bind(func5, 1, 2, 3, 4)(5);
    bind(func5, 1, 2, 3, 4, 5)();
}

void test_bind_member() {
    Thing t = {-1};

    bind(&t, &Thing::func0)();
    bind(&t, &Thing::func1)(1);
    bind(&t, &Thing::func2)(1, 2);
    bind(&t, &Thing::func3)(1, 2, 3);
    bind(&t, &Thing::func4)(1, 2, 3, 4);
    bind(&t, &Thing::func5)(1, 2, 3, 4, 5);
    bind(&t, &Thing::func1, 1)();
    bind(&t, &Thing::func2, 1)(2);
    bind(&t, &Thing::func3, 1)(2, 3);
    bind(&t, &Thing::func4, 1)(2, 3, 4);
    bind(&t, &Thing::func5, 1)(2, 3, 4, 5);
    bind(&t, &Thing::func2, 1, 2)();
    bind(&t, &Thing::func3, 1, 2)(3);
    bind(&t, &Thing::func4, 1, 2)(3, 4);
    bind(&t, &Thing::func5, 1, 2)(3, 4, 5);
    bind(&t, &Thing::func3, 1, 2, 3)();
    bind(&t, &Thing::func4, 1, 2, 3)(4);
    bind(&t, &Thing::func5, 1, 2, 3)(4, 5);
    bind(&t, &Thing::func4, 1, 2, 3, 4)();
    bind(&t, &Thing::func5, 1, 2, 3, 4)(5);
    bind(&t, &Thing::func5, 1, 2, 3, 4, 5)();
}

void test_bind_callback() {
    bind(callback(func0))();
    bind(callback(func1))(1);
    bind(callback(func2))(1, 2);
    bind(callback(func3))(1, 2, 3);
    bind(callback(func4))(1, 2, 3, 4);
    bind(callback(func5))(1, 2, 3, 4, 5);
    bind(callback(func1), 1)();
    bind(callback(func2), 1)(2);
    bind(callback(func3), 1)(2, 3);
    bind(callback(func4), 1)(2, 3, 4);
    bind(callback(func5), 1)(2, 3, 4, 5);
    bind(callback(func2), 1, 2)();
    bind(callback(func3), 1, 2)(3);
    bind(callback(func4), 1, 2)(3, 4);
    bind(callback(func5), 1, 2)(3, 4, 5);
    bind(callback(func3), 1, 2, 3)();
    bind(callback(func4), 1, 2, 3)(4);
    bind(callback(func5), 1, 2, 3)(4, 5);
    bind(callback(func4), 1, 2, 3, 4)();
    bind(callback(func5), 1, 2, 3, 4)(5);
    bind(callback(func5), 1, 2, 3, 4, 5)();
}

void test_bind_nested() {
    bind(bind(func0))();
    bind(bind(func1))(1);
    bind(bind(func2))(1, 2);
    bind(bind(func3))(1, 2, 3);
    bind(bind(func4))(1, 2, 3, 4);
    bind(bind(func5))(1, 2, 3, 4, 5);
    bind(bind(func1, 1))();
    bind(bind(func2, 1))(2);
    bind(bind(func3, 1))(2, 3);
    bind(bind(func4, 1))(2, 3, 4);
    bind(bind(func5, 1))(2, 3, 4, 5);
    bind(bind(func2, 1, 2))();
    bind(bind(func3, 1, 2))(3);
    bind(bind(func4, 1, 2))(3, 4);
    bind(bind(func5, 1, 2))(3, 4, 5);
    bind(bind(func3, 1, 2, 3))();
    bind(bind(func4, 1, 2, 3))(4);
    bind(bind(func5, 1, 2, 3))(4, 5);
    bind(bind(func4, 1, 2, 3, 4))();
    bind(bind(func5, 1, 2, 3, 4))(5);
    bind(bind(func5, 1, 2, 3, 4, 5))();
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing static binds",    test_bind_static),
    Case("Testing member binds",    test_bind_member),
    Case("Testing callback binds",  test_bind_callback),
    Case("Testing nested binds",    test_bind_nested),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
