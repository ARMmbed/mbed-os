#include "mbed_toolchain.h"

#include <stdio.h>
#include <stdint.h>


MBED_PACKED(struct) TestAttrPackedStruct1 {
    char a;
    int x;
};

typedef MBED_PACKED(struct) {
    char a;
    int x;
} TestAttrPackedStruct2;

int testPacked() {
    int failed = 0;

    if (sizeof(struct TestAttrPackedStruct1) != sizeof(int) + sizeof(char)) {
        failed++;
    }

    if (sizeof(TestAttrPackedStruct2) != sizeof(int) + sizeof(char)) {
        failed++;
    }

    return failed;
}


MBED_ALIGN(8)  char a;
MBED_ALIGN(8)  char b;
MBED_ALIGN(16) char c;
MBED_ALIGN(8)  char d;
MBED_ALIGN(16) char e;

int testAlign() {
    int failed = 0;

    if(((uintptr_t)&a) & 0x7){
        failed++;
    }
    if(((uintptr_t)&b) & 0x7){
        failed++;
    }
    if(((uintptr_t)&c) & 0xf){
        failed++;
    }
    if(((uintptr_t)&d) & 0x7){
        failed++;
    }
    if(((uintptr_t)&e) & 0xf){
        failed++;
    }

    return failed;
}


int testUnused1(MBED_UNUSED int arg) {
    return 0;
}

int testUnused() {
    return testUnused1(0);
}


int testWeak1();
int testWeak2();

MBED_WEAK int testWeak1() {
    return 1;
}

int testWeak2() {
    return 0;
}

int testWeak() {
    return testWeak1() | testWeak2();
}


MBED_PURE int testPure1() {
    return 0;
}

int testPure() {
    return testPure1();
}


MBED_FORCEINLINE int testForceInline1() {
    return 0;
}

int testForceInline() {
    return testForceInline1();
}


MBED_NORETURN int testNoReturn1() {
    while (1) {}
}

int testNoReturn() {
    if (0) {
        testNoReturn1();
    }
    return 0;
}


int testUnreachable1(int i) {
    switch (i) {
        case 0:
            return 0;
    }

    MBED_UNREACHABLE;
}

int testUnreachable() {
    return testUnreachable1(0);
}


MBED_DEPRECATED("this message should not be displayed")
void testDeprecatedUnused(); 
void testDeprecatedUnused() { }

MBED_DEPRECATED("this message should be displayed")
int testDeprecatedUsed();
int testDeprecatedUsed() {
    return 0;
}

MBED_DEPRECATED_SINCE("mbed-os-3.14", "this message should not be displayed")
void testDeprecatedSinceUnused();
void testDeprecatedSinceUnused() { }

MBED_DEPRECATED_SINCE("mbed-os-3.14", "this message should be displayed")
int testDeprecatedSinceUsed();
int testDeprecatedSinceUsed() {
    return 0;
}

int testDeprecated() {
    return testDeprecatedUsed() + testDeprecatedSinceUsed();
}

