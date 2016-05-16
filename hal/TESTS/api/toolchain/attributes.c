#include "toolchain.h"

#include <stdio.h>
#include <stdint.h>


struct TestAttrPackedStruct {
    char a;
    int x;
} PACKED;

int testPacked() {
    int failed = 0;

    if (sizeof(struct TestAttrPackedStruct) != sizeof(int) + sizeof(char)) {
        failed++;
    }

    return failed;
}


int testUnused1(UNUSED int arg) {
    return 0;
}

int testUnused() {
    return testUnused1(0);
}


DEPRECATED("this message should not be displayed")
void testDeprecatedUnused(); 
void testDeprecatedUnused() { }

DEPRECATED("this message should be displayed")
int testDeprecatedUsed();
int testDeprecatedUsed() {
    return 0;
}

int testDeprecated() {
    return testDeprecatedUsed();
}


int testWeak1();
int testWeak2();

WEAK int testWeak1() {
    return 1;
}

int testWeak2() {
    return 0;
}

int testWeak() {
    return testWeak1() | testWeak2();
}

