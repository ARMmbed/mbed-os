#include "mbed.h"
#include "test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_STM32F070RB) || defined(TARGET_STM32F072RB)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 1536
#elif defined(TARGET_MCU_NRF51822)
    #define STACK_SIZE 768
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

void print_char(char c = '*') {
    printf("%c", c);
    fflush(stdout);
}

DigitalOut led1(LED1);
DigitalOut led2(LED2);

void led2_thread(void const *argument) {
    while (true) {
        led2 = !led2;
        Thread::wait(1000);
        print_char();
    }
}

int main() {
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(wait_us_auto);
    MBED_HOSTTEST_DESCRIPTION(Basic thread);
    MBED_HOSTTEST_START("RTOS_1");

    Thread thread(led2_thread, NULL, osPriorityNormal, STACK_SIZE);

    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
