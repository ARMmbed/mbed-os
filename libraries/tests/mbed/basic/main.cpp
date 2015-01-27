#include "test_env.h"

int main() {
    TEST_TIMEOUT(20);
    TEST_HOSTTEST(default_auto);
    TEST_DESCRIPTION(Basic);
    TEST_START("MBED_A1");
    TEST_RESULT(true);
}
