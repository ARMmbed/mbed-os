#include "test_env.h"

int main()
{
    TEST_TIMEOUT(10);
    TEST_HOSTTEST(hello_auto);
    TEST_DESCRIPTION(Hello World);
    TEST_START("MBED_10");

    printf("Hello World\r\n");

    while(1);
}
