#include "mbed.h"
#include "test_env.h"

#if defined(TARGET_LPC1549)
CAN can1(D9, D8);
#elif defined(TARGET_LPC1768) || defined(TARGET_LPC4088)
CAN can1(p9, p10);
#endif

#define TEST_ITERATIONS     127

int main() {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(dev_null);
    MBED_HOSTTEST_DESCRIPTION(CAN Loopback);
    MBED_HOSTTEST_START("MBED_A27");

    can1.mode(CAN::Reset);

    if (!can1.mode(CAN::LocalTest)) {
        printf("Mode change failed\n");
    }

    char success_count = 0;
    for (char i=0; i < TEST_ITERATIONS; i++) {
        unsigned int id = 1337;
        CANMessage tx_msg(id, &i, sizeof(i));
        bool sent = false;
        if (can1.write(tx_msg)) {
            printf("Sent %u: %d\n", id, i);
            sent = true;
        }
        wait_ms(50);

        bool read = false;
        CANMessage rx_msg;
        if (can1.read(rx_msg)) {
            printf("Read %u: %d\n", rx_msg.id, rx_msg.data[0]);
            read = (rx_msg.id == id) && (rx_msg.data[0] == i);
        }

        bool success = sent && read;

        if (success) {
            success_count++;
        }
    }

    MBED_HOSTTEST_RESULT(success_count == TEST_ITERATIONS);
}
