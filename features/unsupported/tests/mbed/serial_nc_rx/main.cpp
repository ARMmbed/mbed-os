#include "mbed.h"
#include "test_env.h"

int main() {
    Serial *pc = new Serial(USBTX, USBRX);

    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(serial_nc_rx_auto);
    MBED_HOSTTEST_DESCRIPTION(Serial NC RX);
    MBED_HOSTTEST_START("MBED_37");

    char c = pc->getc();


    // This should be true, sync the start of test
    if (c == 'S') {
        pc->printf("RX OK - Start NC test\r\n");

        // disconnect TX and  get char
        delete pc;
        pc = new Serial(NC, USBRX);
        c = pc->getc();
        if (c == 'E') {
            // ok disconnect Rx and answer to host
            delete pc;
            pc = new Serial(USBTX, NC);
            pc->printf("RX OK - Expected\r\n");

            c = pc->getc();
            // This should be false/not get here
            if (c == 'U') {
                pc->printf("RX OK - Unexpected\r\n");
            }
        }
        delete pc;
    }

    while (1) {
    }
}
