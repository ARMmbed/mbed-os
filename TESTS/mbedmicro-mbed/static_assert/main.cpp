#include <stdio.h>
#include <stdint.h>

#include "mbed_toolchain.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

using namespace utest::v1;

void no_test() {}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Compilation test", no_test),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
