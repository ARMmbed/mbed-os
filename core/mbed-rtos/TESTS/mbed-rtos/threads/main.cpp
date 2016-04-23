#include "mbed.h"
#include "test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"


using namespace utest::v1;


// Tasks with different functions to test on threads
void increment(const void *var) {
    (*(int *)var)++;
}

void increment_with_yield(const void *var) {
    Thread::yield();
    (*(int *)var)++;
}

void increment_with_wait(const void *var) {
    Thread::wait(100);
    (*(int *)var)++;
}

void increment_with_child(const void *var) {
    Thread child(increment, (void*)var);
    child.join();
}

void increment_with_murder(const void *var) {
    Thread child(increment_with_wait, (void*)var);
    // Kill child before it can increment var
    child.terminate();
    (*(int *)var)++;
}


// Tests that spawn tasks in different configurations
template <void (*F)(const void *)>
void test_single_thread() {
    int var = 0;
    Thread thread(F, &var);
    thread.join();
    TEST_ASSERT_EQUAL(var, 1);
}

template <int N, void (*F)(const void *)>
void test_parallel_threads() {
    int var = 0;
    Thread *threads[N];

    for (int i = 0; i < N; i++) {
        threads[i] = new Thread(F, &var);
    }

    for (int i = 0; i < N; i++) {
        threads[i]->join();
        delete threads[i];
    }

    TEST_ASSERT_EQUAL(var, N);
}

template <int N, void (*F)(const void *)>
void test_serial_threads() {
    int var = 0;

    for (int i = 0; i < N; i++) {
        Thread thread(F, &var);
        thread.join();
    }

    TEST_ASSERT_EQUAL(var, N);
}
    

status_t test_setup(const size_t number_of_cases) {
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
    Case("Testing parallel threads with child", test_parallel_threads<3, increment_with_child>),
    Case("Testing serial threads with child", test_serial_threads<10, increment_with_child>),

    Case("Testing single thread with murder", test_single_thread<increment_with_murder>),
    Case("Testing parallel threads with murder", test_parallel_threads<3, increment_with_murder>),
    Case("Testing serial threads with murder", test_serial_threads<10, increment_with_murder>),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
