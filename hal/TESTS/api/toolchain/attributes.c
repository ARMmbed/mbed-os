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

ALIGN(8) static char a_align_test;
ALIGN(8) static char b_align_test;
ALIGN(16)static char c_align_test;
ALIGN(8) static char d_align_test;
ALIGN(16)static char e_align_test;

int testAlign() {
    int failed = 0;

    if(((uintptr_t)&a_align_test) & 0x7){
        failed++;
    }
    if(((uintptr_t)&b_align_test) & 0x7){
        failed++;
    }
    if(((uintptr_t)&c_align_test) & 0xf){
        failed++;
    }
    if(((uintptr_t)&d_align_test) & 0x7){
        failed++;
    }
    if(((uintptr_t)&e_align_test) & 0xf){
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

