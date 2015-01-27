#include "mbed.h"
#include "test_env.h"

int main() {
    TEST_TIMEOUT(10);
    TEST_HOSTTEST(detect_auto);
    TEST_DESCRIPTION(Simple detect test);
    TEST_START("DTCT_1");

    notify_start();
    printf("MBED: Target '%s'\r\n", TEST_SUITE_TARGET_NAME);
    printf("MBED: Test ID '%s'\r\n", TEST_SUITE_TEST_ID);
    printf("MBED: UUID '%s'\r\n", TEST_SUITE_UUID);
    TEST_RESULT(true);
}
