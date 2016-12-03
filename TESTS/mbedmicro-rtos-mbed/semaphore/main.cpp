#include "mbed.h"
#include "greentea-client/test_env.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

#define THREAD_DELAY     75
#define SEMAPHORE_SLOTS  2
#define SEM_CHANGES      100

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_STM32F334R8) && (defined(TOOLCHAIN_GCC) || defined(TOOLCHAIN_IAR))
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif defined(TARGET_STM32F103RB)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F070RB)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2	
#elif defined(TARGET_STM32F072RB)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2	
#elif defined(TARGET_STM32F302R8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2		
#elif defined(TARGET_STM32L073RZ)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F303K8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 1536
#elif (defined(TARGET_EFR32)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define STACK_SIZE 768
#elif defined(TARGET_XDOT_L151CC)
    #define STACK_SIZE 1024
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

void print_char(char c = '*') {
    printf("%c", c);
    fflush(stdout);
}

Semaphore two_slots(SEMAPHORE_SLOTS);

volatile int change_counter = 0;
volatile int sem_counter = 0;
volatile bool sem_defect = false;

void test_thread(void const *delay) {
    const int thread_delay = int(delay);
    while (true) {
        two_slots.wait();
        sem_counter++;
        const bool sem_lock_failed = sem_counter > SEMAPHORE_SLOTS;
        const char msg = sem_lock_failed ? 'e' : sem_counter + '0';
        print_char(msg);
        if (sem_lock_failed) {
            sem_defect = true;
        }
        Thread::wait(thread_delay);
        print_char('.');
        sem_counter--;
        change_counter++;
        two_slots.release();
    }
}

int main (void) {
    GREENTEA_SETUP(20, "default_auto");

    const int t1_delay = THREAD_DELAY * 1;
    const int t2_delay = THREAD_DELAY * 2;
    const int t3_delay = THREAD_DELAY * 3;
    Thread t1(test_thread, (void *)t1_delay, osPriorityNormal, STACK_SIZE);
    Thread t2(test_thread, (void *)t2_delay, osPriorityNormal, STACK_SIZE);
    Thread t3(test_thread, (void *)t3_delay, osPriorityNormal, STACK_SIZE);

    while (true) {
        if (change_counter >= SEM_CHANGES or sem_defect == true) {
            t1.terminate();
            t2.terminate();
            t3.terminate();
            break;
        }
    }

    fflush(stdout);
    GREENTEA_TESTSUITE_RESULT(!sem_defect);
    return 0;
}
