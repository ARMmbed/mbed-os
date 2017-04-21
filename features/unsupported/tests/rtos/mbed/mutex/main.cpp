#include "mbed.h"
#include "test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_DELAY     50
#define SIGNALS_TO_EMIT  100

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_STM32F334R8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif (defined(TARGET_STM32F070RB) || defined(TARGET_STM32F072RB))
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F302R8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F303K8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F334C8)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32L073RZ)
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

Mutex stdio_mutex;
DigitalOut led(LED1);

volatile int change_counter = 0;
volatile bool changing_counter = false;
volatile bool mutex_defect = false;

bool manipulate_protected_zone(const int thread_delay) {
    bool result = true;

    stdio_mutex.lock(); // LOCK
    if (changing_counter == true) {
        // 'e' stands for error. If changing_counter is true access is not exclusively
        print_char('e');
        result = false;
        mutex_defect = true;
    }
    changing_counter = true;

    // Some action on protected
    led = !led;
    change_counter++;
    print_char('.');
    Thread::wait(thread_delay);

    changing_counter = false;
    stdio_mutex.unlock();   // UNLOCK
    return result;
}

void test_thread(void const *args) {
    const int thread_delay = int(args);
    while (true) {
        manipulate_protected_zone(thread_delay);
    }
}

int main() {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(default);
    MBED_HOSTTEST_DESCRIPTION(Mutex resource lock);
    MBED_HOSTTEST_START("RTOS_2");

    const int t1_delay = THREAD_DELAY * 1;
    const int t2_delay = THREAD_DELAY * 2;
    const int t3_delay = THREAD_DELAY * 3;
    Thread t2(test_thread, (void *)t2_delay, osPriorityNormal, STACK_SIZE);
    Thread t3(test_thread, (void *)t3_delay, osPriorityNormal, STACK_SIZE);

    while (true) {
        // Thread 1 action
        Thread::wait(t1_delay);
        manipulate_protected_zone(t1_delay);
        if (change_counter >= SIGNALS_TO_EMIT or mutex_defect == true) {
            t2.terminate();
            t3.terminate();
            break;
        }
    }

    fflush(stdout);
    MBED_HOSTTEST_RESULT(!mutex_defect);
    return 0;
}
