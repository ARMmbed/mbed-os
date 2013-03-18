#include "mbed.h"

uint32_t test_64(uint64_t ticks) {
    ticks >>= 3; // divide by 8
    if (ticks > 0xFFFFFFFF) {
        ticks /= 3;
    } else {
        ticks = (ticks * 0x55555556) >> 32; // divide by 3
    }
    return (uint32_t)(0xFFFFFFFF & ticks);
}

int main() {
    // 0xFFFFFFFF *  8 =  0x7fffffff8
    printf("expected        : 55555555\n");
    printf("64:  0x7fffffff8: %x\n", test_64(0x7fffffff8));
    
    // 0xFFFFFFFF * 24 = 0x17ffffffe8
    printf("expected        : ffffffff\n");
    printf("64: 0x17ffffffe8: %x\n", test_64(0x17FFFFFFE8));
    
    DigitalOut led(LED1);
    while (1) {
        led = !led;
        wait(0.5);
    }
}
