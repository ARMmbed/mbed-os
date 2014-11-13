#include "CppUTest\CommandLineTestRunner.h"
#include <stdio.h>
#include "mbed.h"
#include "testrunner.h"
#include "test_env.h"

/**
Object 'console' is used to show prints on console.
It is declared in \cpputest\src\Platforms\armcc\UtestPlatform.cpp
*/
Serial console(STDIO_UART_TX, STDIO_UART_RX);

int main(int ac, char** av)
{
    unsigned failureCount = 0;
    {
        // Some compilers may not pass ac, av so we need to supply them ourselves
        int ac = 2;
        char* av[] = {__FILE__, "-v"};
        failureCount = CommandLineTestRunner::RunAllTests(ac, av);
    }

    notify_completion(failureCount == 0);
    return failureCount;
}
