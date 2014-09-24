#include "mbed.h"
#include "test_env.h"

int main() {
    notify_start();
    printf("MBED: Target '%s'\r\n", TEST_SUITE_TARGET_NAME);
    printf("MBED: Test ID '%s'\r\n", TEST_SUITE_TEST_ID);
    notify_completion(true);
}
