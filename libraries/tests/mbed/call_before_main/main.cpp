#include "test_env.h"

namespace {
    bool mbed_main_called = false;
}

extern "C" void mbed_main() {
    printf("MBED: mbed_main() call before main()\r\n");
    mbed_main_called = true;
}

int main() {
    TEST_TIMEOUT(20);
    TEST_HOSTTEST(default_auto);
    TEST_DESCRIPTION(Call function mbed_main before main);
    TEST_START("MBED_A21");

    printf("MBED: main() starts now!\r\n");

    TEST_RESULT(mbed_main_called);
}
