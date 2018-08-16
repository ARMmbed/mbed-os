#include "platform/mbed_assert.h"
#include <stdio.h>

void mbed_assert_internal(const char *expr, const char *file, int line)
{
    fprintf(stderr, "mbed assertation failed: %s, file: %s, line %d \n", expr, file, line);
}
