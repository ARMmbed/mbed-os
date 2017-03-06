#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#define TEST_STACK_SIZE 512

#define TEST_ONE_SEC_MS  (1000)
#define TEST_HALF_SEC_MS (500)
#define TEST_HALF_SEC_US (500000)
#define TEST_ONE_MS_US   (1000)

#define THREAD_DELAY     50
#define SIGNALS_TO_EMIT  100

Mutex stdio_mutex;

volatile int change_counter = 0;
volatile bool changing_counter = false;
volatile bool mutex_defect = false;

bool manipulate_protected_zone(const int thread_delay) {
    bool result = true;

    osStatus stat = stdio_mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);
    if (changing_counter == true) {
        result = false;
        mutex_defect = true;
    }
    changing_counter = true;

    change_counter++;
    Thread::wait(thread_delay);

    changing_counter = false;
    stat = stdio_mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
    return result;
}

void test_thread(int const *thread_delay) {
    while (true) {
        manipulate_protected_zone(*thread_delay);
    }
}

void test_multiple_threads(void)
{
    const int t1_delay = THREAD_DELAY * 1;
    const int t2_delay = THREAD_DELAY * 2;
    const int t3_delay = THREAD_DELAY * 3;

    Thread t2(osPriorityNormal, TEST_STACK_SIZE);
    Thread t3(osPriorityNormal, TEST_STACK_SIZE);

    t2.start(callback(test_thread, &t2_delay));
    t3.start(callback(test_thread, &t3_delay));

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

    TEST_ASSERT_EQUAL(mutex_defect, false);
}

void test_dual_thread_nolock_lock_thread(Mutex *mutex)
{
    bool stat_b = mutex->trylock();
    TEST_ASSERT_EQUAL(stat_b, true);

    osStatus stat = mutex->unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

void test_dual_thread_nolock_trylock_thread(Mutex *mutex)
{
    bool stat_b = mutex->trylock();
    TEST_ASSERT_EQUAL(stat_b, true);

    osStatus stat = mutex->unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

template <void (*F)(Mutex *)>
void test_dual_thread_nolock(void)
{
    Mutex mutex;
    Thread thread;

    thread.start(callback(F, &mutex));

    wait_us(TEST_HALF_SEC_MS);
}

void test_dual_thread_lock_unlock_thread(Mutex *mutex)
{
    osStatus stat = mutex->lock(osWaitForever);
    TEST_ASSERT_EQUAL(stat, osOK);
}

void test_dual_thread_lock_unlock(void)
{
    Mutex mutex;
    osStatus stat;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);

    thread.start(callback(test_dual_thread_lock_unlock_thread, &mutex));

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);

    wait_us(TEST_HALF_SEC_MS);
}

void test_dual_thread_lock_trylock_thread(Mutex *mutex)
{
    bool stat = mutex->trylock();
    TEST_ASSERT_EQUAL(stat, false);
}

void test_dual_thread_lock_lock_thread(Mutex *mutex)
{
    uint32_t start = us_ticker_read();

    osStatus stat = mutex->lock(TEST_HALF_SEC_MS);
    TEST_ASSERT_EQUAL(stat, osEventTimeout);
    TEST_ASSERT_UINT32_WITHIN(TEST_ONE_MS_US, TEST_HALF_SEC_US, us_ticker_read() - start);
}

template <void (*F)(Mutex *)>
void test_dual_thread_lock(void)
{
    Mutex mutex;
    osStatus stat;
    Thread thread(osPriorityNormal, TEST_STACK_SIZE);

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);

    thread.start(callback(F, &mutex));

    wait_us(TEST_ONE_SEC_MS);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

void test_single_thread_lock_recursive(void)
{
    Mutex mutex;
    osStatus stat;

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

void test_single_thread_trylock(void)
{
    Mutex mutex;

    bool stat_b = mutex.trylock();
    TEST_ASSERT_EQUAL(stat_b, true);

    osStatus stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

void test_single_thread_lock(void)
{
    Mutex mutex;
    osStatus stat;

    stat = mutex.lock();
    TEST_ASSERT_EQUAL(stat, osOK);

    stat = mutex.unlock();
    TEST_ASSERT_EQUAL(stat, osOK);
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test single thread lock", test_single_thread_lock),
    Case("Test single thread trylock", test_single_thread_trylock),
    Case("Test single thread lock recursive", test_single_thread_lock_recursive),
    Case("Test dual thread lock locked", test_dual_thread_lock<test_dual_thread_lock_lock_thread>),
    Case("Test dual thread trylock locked", test_dual_thread_lock<test_dual_thread_lock_trylock_thread>),
    Case("Test dual thread lock unlock", test_dual_thread_lock_unlock),
    Case("Test dual thread second thread lock", test_dual_thread_nolock<test_dual_thread_nolock_lock_thread>),
    Case("Test dual thread second thread trylock", test_dual_thread_nolock<test_dual_thread_nolock_trylock_thread>),
    Case("Test multiple thread", test_multiple_threads),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
