#include "mbed.h"
#include "test_env.h"

#define TXPIN     USBTX
#define RXPIN     USBRX


namespace {
    const int BUFFER_SIZE = 48;
}

int main() {
    char buffer[BUFFER_SIZE] = {0};

    Serial pc(TXPIN, RXPIN);
    pc.baud(115200);

    pc.puts("{{");
    pc.puts(TEST_ENV_START);    // Host test is expecting preamble
    pc.puts("}}");

    while (1) {
        pc.gets(buffer, BUFFER_SIZE - 1);
        pc.printf("%s", buffer);
    }
}
