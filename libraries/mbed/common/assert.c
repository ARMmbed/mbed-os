#include "assert.h"
#include "device.h"

#if DEVICE_STDIO_MESSAGES
#include <stdio.h>
#endif

#include <stdlib.h>
#include "mbed_interface.h"

void mbed_assert_internal(const char *expr, const char *file, int line)
{
#if DEVICE_STDIO_MESSAGES
    fprintf(stderr, "mbed assertation failed: %s, file: %s, line %d \n", expr, file, line);
#endif
    mbed_die();
}
