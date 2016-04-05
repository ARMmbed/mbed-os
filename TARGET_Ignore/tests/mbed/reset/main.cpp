#include "mbed.h"

#if !DEVICE_SEMIHOST
  #error [NOT_SUPPORTED] Semihost not supported
#endif

Serial pc(USBTX, USBRX);

extern "C" void mbed_reset();

int main() {
    pc.printf("start\n");
    wait(1);

    unsigned int counter = 0;
    while(1) {
        pc.printf("%u\n",counter++);
        wait(1);
        mbed_reset();
    }
}
