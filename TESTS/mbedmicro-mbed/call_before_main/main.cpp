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
    printf("MBED: main() starts now!\r\n");
    GREENTEA_TESTSUITE_RESULT(mbed_main_called);
}
