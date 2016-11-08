#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "SynchronizedIntegral.h"
#include "LockGuard.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define STACK_SIZE 512
#elif defined(TARGET_STM32F070RB) || defined(TARGET_STM32F072RB) || defined(TARGET_STM32F103RB) || defined(TARGET_STM32F091RC)
    #define STACK_SIZE 512
#elif defined(TARGET_STM32F410RB)
    #define STACK_SIZE 512
#elif defined(TARGET_STM32L073RZ)
    #define STACK_SIZE 512
#elif defined(TARGET_XDOT_L151CC)
    #define STACK_SIZE 1024
#elif defined(TARGET_HI2110)
    #define STACK_SIZE 512
#elif defined(TARGET_EFR32)
    #define STACK_SIZE 512
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

using namespace utest::v1;

// The counter type used accross all the tests
// It is internall ysynchronized so read
typedef SynchronizedIntegral<int> counter_t;

// Tasks with different functions to test on threads
void increment(counter_t* counter) {
    (*counter)++;
}

void increment_with_yield(counter_t* counter) {
    Thread::yield();
    (*counter)++;
}

void increment_with_wait(counter_t* counter) {
    Thread::wait(100);
    (*counter)++;
}

void increment_with_child(counter_t* counter) {
    Thread child(counter, increment, osPriorityNormal, STACK_SIZE);
    child.join();
}

void increment_with_murder(counter_t* counter) {
    {
        // take ownership of the counter mutex so it prevent the child to
        // modify counter.
        LockGuard lock(counter->internal_mutex());
        Thread child(counter, increment, osPriorityNormal, STACK_SIZE);
        child.terminate();
    }

    (*counter)++;
}

void self_terminate(Thread *self) {
    self->terminate();
    // Code should not get here
    TEST_ASSERT(0);
}

// Tests that spawn tasks in different configurations
template <void (*F)(counter_t *)>
void test_single_thread() {
    counter_t counter(0);
    Thread thread(&counter, F, osPriorityNormal, STACK_SIZE);
    thread.join();
    TEST_ASSERT_EQUAL(counter, 1);
}

template <int N, void (*F)(counter_t *)>
void test_parallel_threads() {
    counter_t counter(0);
    Thread *threads[N];

    for (int i = 0; i < N; i++) {
        threads[i] = new Thread(&counter, F, osPriorityNormal, STACK_SIZE);
    }

    for (int i = 0; i < N; i++) {
        threads[i]->join();
        delete threads[i];
    }

    TEST_ASSERT_EQUAL(counter, N);
}

template <int N, void (*F)(counter_t *)>
void test_serial_threads() {
    counter_t counter(0);

    for (int i = 0; i < N; i++) {
        Thread thread(&counter, F, osPriorityNormal, STACK_SIZE);
        thread.join();
    }

    TEST_ASSERT_EQUAL(counter, N);
}

void test_self_terminate() {
    Thread *thread = new Thread(osPriorityNormal, STACK_SIZE);
    thread->start(thread, self_terminate);
    thread->join();
    delete thread;
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("Testing single thread", test_single_thread<increment>),
    Case("Testing parallel threads", test_parallel_threads<3, increment>),
    Case("Testing serial threads", test_serial_threads<10, increment>),

    Case("Testing single thread with yield", test_single_thread<increment_with_yield>),
    Case("Testing parallel threads with yield", test_parallel_threads<3, increment_with_yield>),
    Case("Testing serial threads with yield", test_serial_threads<10, increment_with_yield>),

    Case("Testing single thread with wait", test_single_thread<increment_with_wait>),
    Case("Testing parallel threads with wait", test_parallel_threads<3, increment_with_wait>),
    Case("Testing serial threads with wait", test_serial_threads<10, increment_with_wait>),

    Case("Testing single thread with child", test_single_thread<increment_with_child>),
    Case("Testing parallel threads with child", test_parallel_threads<2, increment_with_child>),
    Case("Testing serial threads with child", test_serial_threads<10, increment_with_child>),

    Case("Testing single thread with murder", test_single_thread<increment_with_murder>),
    Case("Testing parallel threads with murder", test_parallel_threads<3, increment_with_murder>),
    Case("Testing serial threads with murder", test_serial_threads<10, increment_with_murder>),

    Case("Testing thread self terminate", test_self_terminate),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
