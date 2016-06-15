#include "mbed.h"
#include "test_env.h"

Serial *pc = new Serial(USBTX, USBRX);

int main() {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(serial_complete_auto);
    MBED_HOSTTEST_DESCRIPTION(Serial Complete);
    MBED_HOSTTEST_START("MBED_39");

    pc->printf("123456789\n");

    while (1) {
        deepsleep();
    }
}
