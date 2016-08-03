#include "greentea-client/test_env.h"

namespace {
    bool mbed_main_called = false;
}

extern "C" void mbed_main() {
    printf("MBED: mbed_main() call before main()\r\n");
    mbed_main_called = true;
}

int main() {
    GREENTEA_SETUP(5, "default_auto");
    GREENTEA_TESTCASE_START("Call before main");
    printf("MBED: main() starts now!\r\n");
    GREENTEA_TESTCASE_FINISHED("Call before main", 1, 0);
    GREENTEA_TESTSUITE_RESULT(mbed_main_called);
}
