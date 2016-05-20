#include "toolchain.h"

#include <stdio.h>
#include <stdint.h>


typedef struct {
    char a;
    int x;
} PACKED TestAttrPackedStruct;

int testPacked() {
    int failed = 0;

    if (sizeof(TestAttrPackedStruct) != sizeof(int) + sizeof(char)) {
        failed++;
    }

    return failed;
}


int testAlign() {
    int failed = 0;

    ALIGN(8)  char a;
    ALIGN(8)  char b;
    ALIGN(16) char c;
    ALIGN(8)  char d;
    ALIGN(16) char e;

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


int testUnused1(UNUSED int arg) {
    return 0;
}

int testUnused() {
    return testUnused1(0);
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


PURE int testPure1() {
    return 0;
}

int testPure() {
    return testPure1();
}


FORCEINLINE int testForceInline1() {
    return 0;
}

int testForceInline() {
    return testForceInline1();
}


NORETURN int testNoReturn1() {
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

    UNREACHABLE;
}

int testUnreachable() {
    return testUnreachable1(0);
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

