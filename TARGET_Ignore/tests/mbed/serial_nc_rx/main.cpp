#include "mbed.h"
#include "test_env.h"

int main() {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(serial_nc_rx_auto);
    MBED_HOSTTEST_DESCRIPTION(Serial NC RX);
    MBED_HOSTTEST_START("MBED_37");

    Serial *pc = new Serial(NC, USBRX);

    char c = pc->getc();

    delete pc;

    // This should be true
    if (c == 'E') {
      Serial *pc = new Serial(USBTX, NC);

      pc->printf("RX OK - Expected\r\n");

      c = pc->getc();

      // This should be false/not get here
      if (c == 'U') {
        pc->printf("RX OK - Unexpected\r\n");
      }

      delete pc;
    }

    while (1) {
    }
}
