#include "mbed_assert.h"
#define THE_ANSWER 42

// Tests for static asserts in different contexts
// multiple asserts are used to guarantee no conflicts occur in generated labels

// Test for static asserts in global context
MBED_STATIC_ASSERT(sizeof(int) >= sizeof(char),
        "An int must be larger than char");
MBED_STATIC_ASSERT(2 + 2 == 4,
        "Hopefully the universe is mathematically consistent");
MBED_STATIC_ASSERT(THE_ANSWER == 42,
        "Said Deep Thought, with infinite majesty and calm");

struct test {
    int dummy;

    // Test for static asserts in struct context
    MBED_STRUCT_STATIC_ASSERT(sizeof(int) >= sizeof(char),
            "An int must be larger than char");
    MBED_STRUCT_STATIC_ASSERT(2 + 2 == 4,
            "Hopefully the universe is mathematically consistent");
    MBED_STRUCT_STATIC_ASSERT(THE_ANSWER == 42,
            "Said Deep Thought, with infinite majesty and calm");
};

MBED_STATIC_ASSERT(sizeof(struct test) == sizeof(int),
        "Static assertions should not change the size of a struct");

void doit_c(void) {
    // Test for static asserts in function context
    MBED_STATIC_ASSERT(sizeof(int) >= sizeof(char),
            "An int must be larger than char");
    MBED_STATIC_ASSERT(2 + 2 == 4,
            "Hopefully the universe is mathematically consistent");
    MBED_STATIC_ASSERT(THE_ANSWER == 42,
            "Said Deep Thought, with infinite majesty and calm");
}

