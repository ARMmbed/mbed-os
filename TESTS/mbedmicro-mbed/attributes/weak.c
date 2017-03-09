#include "mbed_toolchain.h"

int testWeak1() {
    return 0;
}

MBED_WEAK int testWeak2() {
    return 1;
}

