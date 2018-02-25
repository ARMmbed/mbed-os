/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

#include <stdint.h>
#include "cmsis_os.h"
#include "rtos/Mutex.h"
#include "rtos/Semaphore.h"

#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

struct Waiter;

/** This class provides a safe way to wait for or send notifications of condition changes
 *
 * This class is used in conjunction with a mutex to safely wait for or
 * notify waiters of condition changes to a resource accessible by multiple
 * threads.
 *
 * # Defined behavior
 * - All threads waiting on the condition variable wake when
 *   ConditionVariable::notify_all is called.
 * - If one or more threads are waiting on the condition variable at least
 *   one of them wakes when ConditionVariable::notify is called.
 *
 * # Undefined behavior
 * - The thread which is unblocked on ConditionVariable::notify_one is
 *   undefined if there are multiple waiters.
 * - The order which in which waiting threads acquire the condition variable's
 *   mutex after ConditionVariable::notify_all is called is undefined.
 * - When ConditionVariable::notify_one or ConditionVariable::notify_all is
 *   called and there are one or more waiters and one or more threads attempting
 *   to acquire the condition variable's mutex the order in which the mutex is
 *   acquired is undefined.
 * - The behavior of ConditionVariable::wait and ConditionVariable::wait_for
 *   is undefined if the condition variable's mutex is locked more than once by
 *   the calling thread.
 * - Spurious notifications (not triggered by the application) can occur
 *   and it is not defined when these occur.
 *
 * @note Synchronization level: Thread safe
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * Mutex mutex;
 * ConditionVariable cond(mutex);
 *
 * // These variables are protected by locking mutex
 * uint32_t count = 0;
 * bool done = false;
 *
 * void worker_thread()
 * {
 *     mutex.lock();
 *     do {
 *         printf("Worker: Count %lu\r\n", count);
 *
 *         // Wait for a condition to change
 *         cond.wait();
 *
 *     } while (!done);
 *     printf("Worker: Exiting\r\n");
 *     mutex.unlock();
 * }
 *
 * int main() {
 *     Thread thread;
 *     thread.start(worker_thread);
 *
 *     for (int i = 0; i < 5; i++) {
 *
 *         mutex.lock();
 *         // Change count and notify waiters of this
 *         count++;
 *         printf("Main: Set count to %lu\r\n", count);
 *         cond.notify_all();
 *         mutex.unlock();
 *
 *         wait(1.0);
 *     }
 *
 *     mutex.lock();
 *     // Change done and notify waiters of this
 *     done = true;
 *     printf("Main: Set done\r\n");
 *     cond.notify_all();
 *     mutex.unlock();
 *
 *     thread.join();
 * }
 * @endcode
 */
class ConditionVariable : private mbed::NonCopyable<ConditionVariable> {
public:
    /** Create and Initialize a ConditionVariable object
     *
     * @note You cannot call this function from ISR context.
    */
    ConditionVariable(Mutex &mutex);

    /** Wait for a notification
     *
     * Wait until a notification occurs.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex and it must be locked exactly once
     * @note - Spurious notifications can occur so the caller of this API
     * should check to make sure the condition they are waiting on has
     * been met
     *
     * Example:
     * @code
     * mutex.lock();
     * while (!condition_met) {
     *     cond.wait();
     * }
     *
     * function_to_handle_condition();
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    void wait();

    /** Wait for a notification until specified time
     *
     * @param   millisec  absolute end time referenced to Kernel::get_ms_count()
     * @return  true if a timeout occurred, false otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex and it must be locked exactly once
     * @note - Spurious notifications can occur so the caller of this API
     * should check to make sure the condition they are waiting on has
     * been met
     *
     * Example:
     * @code
     * mutex.lock();
     * uint64_t end_time = Kernel::get_ms_count() + COND_WAIT_TIMEOUT;
     *
     * while (!condition_met) {
     *     if (cond.wait_until(end_time)) {
     *         break;
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    bool wait_until(uint64_t millisec);

    /** Wait for a notification or timeout
     *
     * @param   millisec  timeout value or osWaitForever in case of no time-out.
     * @return  true if a timeout occurred, false otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex and it must be locked exactly once
     * @note - Spurious notifications can occur so the caller of this API
     * should check to make sure the condition they are waiting on has
     * been met
     *
     * Example:
     * @code
     * mutex.lock();
     *
     * while (!condition_met) {
     *     cond.wait_for(MAX_SLEEP_TIME);
     *     if (!condition_met) {
     *         do_other_work_while_condition_false();
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    bool wait_for(uint32_t millisec);

    /** Notify one waiter on this condition variable that a condition changed.
     *
     * @note - The thread calling this function must be the owner of the ConditionVariable's mutex
     *
     * @note You cannot call this function from ISR context.
     */
    void notify_one();

    /** Notify all waiters on this condition variable that a condition changed.
     *
     * @note - The thread calling this function must be the owner of the ConditionVariable's mutex
     *
     * @note You cannot call this function from ISR context.
     */
    void notify_all();

    /** ConditionVariable destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~ConditionVariable();

protected:
    struct Waiter {
        Waiter();
        Semaphore sem;
        Waiter *prev;
        Waiter *next;
        bool in_list;
    };

    static void _add_wait_list(Waiter **wait_list, Waiter *waiter);
    static void _remove_wait_list(Waiter **wait_list, Waiter *waiter);
    Mutex &_mutex;
    Waiter *_wait_list;
};

}
#endif

/** @}*/
