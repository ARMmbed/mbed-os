#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_STM32F070RB) && defined(TOOLCHAIN_GCC)
#define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
#define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
#define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
#define STACK_SIZE 1536
#elif defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
#define STACK_SIZE 768
#elif defined(TARGET_XDOT_L151CC)
#define STACK_SIZE 1024
#else
#define STACK_SIZE DEFAULT_STACK_SIZE
#endif

#define SIGNAL_PRINT_TICK 0x01

DigitalOut led1(LED1);

const int total_ticks = 10;

void print_tick_thread() {
    for (int i = 0; i <= total_ticks; i++) {
      Thread::signal_wait(SIGNAL_PRINT_TICK);
      greentea_send_kv("tick", i);
      led1 = !led1;
    }
}

int main() {
    GREENTEA_SETUP(total_ticks + 5, "timing_drift_auto");
    
    Thread tick_thread(osPriorityNormal, STACK_SIZE);
    tick_thread.start(print_tick_thread);
    
    for (int i = 0; i <= total_ticks; i++) {
        Thread::wait(1000);
        tick_thread.signal_set(SIGNAL_PRINT_TICK);
    }
    
    tick_thread.join();
    GREENTEA_TESTSUITE_RESULT(1);
}
