#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if (defined(TARGET_STM32L053R8) || defined(TARGET_STM32L053C8)) && defined(TOOLCHAIN_GCC)
#define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_STM32F030R8) || defined(TARGET_STM32F070RB)) && defined(TOOLCHAIN_GCC)
#define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_STM32F030R8)) && defined(TOOLCHAIN_IAR)
#define STACK_SIZE DEFAULT_STACK_SIZE/2
#else
#define STACK_SIZE DEFAULT_STACK_SIZE
#endif

DigitalOut led1(LED1);
DigitalOut led2(LED2);

void led2_thread(void const *argument) {
    static int count = 0;
    while (true) {
        led2 = !led2;
        Thread::wait(1000);
        greentea_send_kv("tick", count++);
    }
}

int main() {
    GREENTEA_SETUP(15, "wait_us_auto");

    Thread thread(led2_thread, NULL, osPriorityNormal, STACK_SIZE);

    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}
