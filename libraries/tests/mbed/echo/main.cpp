#include "mbed.h"
#include "test_env.h"

#define TXPIN     USBTX
#define RXPIN     USBRX


namespace {
    const int BUFFER_SIZE = 48;
    char buffer[BUFFER_SIZE] = {0};
}

int main() {
    TEST_TIMEOUT(20);
    TEST_HOSTTEST(echo);
    TEST_DESCRIPTION(Serial Echo at 115200);
    TEST_START("MBED_A9");

    Serial pc(TXPIN, RXPIN);
    pc.baud(115200);

    while (1) {
        pc.gets(buffer, BUFFER_SIZE - 1);
        pc.printf("%s", buffer);
    }
}
