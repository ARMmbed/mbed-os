#include <stdio.h>
#include <stdint.h>

#include "toolchain.h"
#include "test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;


// Test functions declared as C functions to avoid issues with name mangling
extern "C" {
    int testPacked();
    int testUnused();
    int testDeprecated();
    int testWeak();
}


// Test wrapper and test cases for utest
template <int (*F)()>
void test_wrapper() {
    TEST_ASSERT_UNLESS(F());
}

status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing PACKED attribute",     test_wrapper<testPacked>),
    Case("Testing UNUSED attribute",     test_wrapper<testUnused>),
    Case("Testing DEPRECATED attribute", test_wrapper<testDeprecated>),
    Case("Testing WEAK attribute",       test_wrapper<testWeak>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
