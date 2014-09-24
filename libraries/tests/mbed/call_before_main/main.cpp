#include "test_env.h"

namespace {
    bool mbed_main_called = false;
}

extern "C" void mbed_main() {
    printf("MBED: mbed_main() call before main()\r\n");
    mbed_main_called = true;
}

int main() {
    printf("MBED: main() starts now!\r\n");
    notify_completion(mbed_main_called);
}
