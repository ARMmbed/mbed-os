#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define SIGNAL_SET_VALUE    0x01
const int SIGNALS_TO_EMIT = 100;
const int SIGNAL_HANDLE_DELEY = 25;

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_STM32L053R8) || defined(TARGET_STM32L053C8)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif (defined(TARGET_STM32F030R8)) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 1536
#elif defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define STACK_SIZE 768
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

DigitalOut led(LED1);
int signal_counter = 0;

void led_thread(void const *argument) {
    while (true) {
        // Signal flags that are reported as event are automatically cleared.
        Thread::signal_wait(SIGNAL_SET_VALUE);
        led = !led;
        signal_counter++;
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    Thread thread(led_thread, NULL, osPriorityNormal, STACK_SIZE);
    bool result = false;

    printf("Handling %d signals...\r\n", SIGNALS_TO_EMIT);

    while (true) {
        Thread::wait(2 * SIGNAL_HANDLE_DELEY);
        thread.signal_set(SIGNAL_SET_VALUE);
        if (signal_counter == SIGNALS_TO_EMIT) {
            printf("Handled %d signals\r\n", signal_counter);
            result = true;
            break;
        }
    }
    GREENTEA_TESTSUITE_RESULT(result);
    return 0;
}
