/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 * SPDX-License-Identifier: MIT
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
#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include "rtos/mbed_rtos_types.h"
#include "rtos/internal/mbed_rtos1_types.h"
#include "rtos/internal/mbed_rtos_storage.h"
#include "platform/Callback.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY)

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

/**
 * \defgroup rtos_Thread Thread class
 * @{
 */

/** The Thread class allow defining, creating, and controlling thread functions in the system.
 *
 *  Example:
 *  @code
 *  #include "mbed.h"
 *  #include "rtos.h"
 *
 *  Thread thread;
 *  DigitalOut led1(LED1);
 *  volatile bool running = true;
 *
 *  // Blink function toggles the led in a long running loop
 *  void blink(DigitalOut *led) {
 *      while (running) {
 *          *led = !*led;
 *          ThisThread::sleep_for(1000);
 *      }
 *  }
 *
 *  // Spawns a thread to run blink for 5 seconds
 *  int main() {
 *      thread.start(callback(blink, &led1));
 *      ThisThread::sleep_for(5000);
 *      running = false;
 *      thread.join();
 *  }
 *  @endcode
 *
 * @note
 * Memory considerations: The thread control structures will be created on current thread's stack, both for the mbed OS
 * and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
 * Additionally the stack memory for this thread will be allocated on the heap, if it wasn't supplied to the constructor.
 *
 * @note
 * MBED_TZ_DEFAULT_ACCESS (default:0) flag can be used to change the default access of all user threads in non-secure mode.
 * MBED_TZ_DEFAULT_ACCESS set to 1, means all non-secure user threads have access to call secure functions.
 * MBED_TZ_DEFAULT_ACCESS set to 0, means none of the non-secure user thread have access to call secure functions,
 * to give access to particular thread used overloaded constructor with `tz_module` as argument during thread creation.
 *
 * MBED_TZ_DEFAULT_ACCESS is target specific define, should be set in targets.json file for Cortex-M23/M33 devices.
 *
 * @note
 * Bare metal profile: This class is not supported.
 */

class Thread : private mbed::NonCopyable<Thread> {
public:
    /** Allocate a new thread without starting execution
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size     stack size (in bytes) requirements for the thread function. (default: OS_STACK_SIZE).
      @param   stack_mem      pointer to the stack area to be used by this thread (default: nullptr).
      @param   name           name to be used for this thread. It has to stay allocated for the lifetime of the thread (default: nullptr)

      @note Default value of tz_module will be MBED_TZ_DEFAULT_ACCESS
      @note You cannot call this function from ISR context.
    */

    Thread(osPriority priority = osPriorityNormal,
           uint32_t stack_size = OS_STACK_SIZE,
           unsigned char *stack_mem = nullptr, const char *name = nullptr)
    {
        constructor(priority, stack_size, stack_mem, name);
    }

    /** Allocate a new thread without starting execution
      @param   tz_module      trustzone thread identifier (osThreadAttr_t::tz_module)
                              Context of RTOS threads in non-secure state must be saved when calling secure functions.
                              tz_module ID is used to allocate context memory for threads, and it can be safely set to zero for
                              threads not using secure calls at all. See "TrustZone RTOS Context Management" for more details.
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size     stack size (in bytes) requirements for the thread function. (default: OS_STACK_SIZE).
      @param   stack_mem      pointer to the stack area to be used by this thread (default: nullptr).
      @param   name           name to be used for this thread. It has to stay allocated for the lifetime of the thread (default: nullptr)

      @note You cannot call this function from ISR context.
    */

    Thread(uint32_t tz_module, osPriority priority = osPriorityNormal,
           uint32_t stack_size = OS_STACK_SIZE,
           unsigned char *stack_mem = nullptr, const char *name = nullptr)
    {
        constructor(tz_module, priority, stack_size, stack_mem, name);
    }


    /** Starts a thread executing the specified function.
      @param   task           function to be executed by this thread.
      @return  status code that indicates the execution status of the function,
               or osErrorNoMemory if stack allocation failed.
      @note a thread can only be started once

      @note You cannot call this function ISR context.
    */
    osStatus start(mbed::Callback<void()> task);

    /** Wait for thread to terminate
      @return  status code that indicates the execution status of the function.

      @note You cannot call this function from ISR context.
    */
    osStatus join();

    /** Terminate execution of a thread and remove it from Active Threads
      @return  status code that indicates the execution status of the function.

      @note You cannot call this function from ISR context.
    */
    osStatus terminate();

    /** Set priority of an active thread
      @param   priority  new priority value for the thread function.
      @return  status code that indicates the execution status of the function.

      @note You cannot call this function from ISR context.
    */
    osStatus set_priority(osPriority priority);

    /** Get priority of an active thread
      @return  current priority value of the thread function.

      @note You cannot call this function from ISR context.
    */
    osPriority get_priority() const;

    /** Set the specified Thread Flags for the thread.
      @param   flags  specifies the flags of the thread that should be set.
      @return  thread flags after setting or osFlagsError in case of incorrect parameters.

      @note You may call this function from ISR context.
    */
    uint32_t flags_set(uint32_t flags);

    /** State of the Thread */
    enum State {
        Inactive,           /**< NOT USED */
        Ready,              /**< Ready to run */
        Running,            /**< Running */
        WaitingDelay,       /**< Waiting for a delay to occur */
        WaitingJoin,        /**< Waiting for thread to join. Only happens when using RTX directly. */
        WaitingThreadFlag,  /**< Waiting for a thread flag to be set */
        WaitingEventFlag,   /**< Waiting for a event flag to be set */
        WaitingMutex,       /**< Waiting for a mutex event to occur */
        WaitingSemaphore,   /**< Waiting for a semaphore event to occur */
        WaitingMemoryPool,  /**< Waiting for a memory pool */
        WaitingMessageGet,  /**< Waiting for message to arrive */
        WaitingMessagePut,  /**< Waiting for message to be send */
        WaitingInterval,    /**< NOT USED */
        WaitingOr,          /**< NOT USED */
        WaitingAnd,         /**< NOT USED */
        WaitingMailbox,     /**< NOT USED (Mail is implemented as MemoryPool and Queue) */

        /* Not in sync with RTX below here */
        Deleted,            /**< The task has been deleted or not started */
    };

    /** State of this Thread
      @return  the State of this Thread

      @note You cannot call this function from ISR context.
    */
    State get_state() const;

    /** Get the total stack memory size for this Thread
      @return  the total stack memory size in bytes

      @note You cannot call this function from ISR context.
    */
    uint32_t stack_size() const;

    /** Get the currently unused stack memory for this Thread
      @return  the currently unused stack memory in bytes

      @note You cannot call this function from ISR context.
    */
    uint32_t free_stack() const;

    /** Get the currently used stack memory for this Thread
      @return  the currently used stack memory in bytes

      @note You cannot call this function from ISR context.
    */
    uint32_t used_stack() const;

    /** Get the maximum stack memory usage to date for this Thread
      @return  the maximum stack memory usage to date in bytes

      @note You cannot call this function from ISR context.
    */
    uint32_t max_stack() const;

    /** Get thread name
      @return  thread name or nullptr if the name was not set.

      @note You may call this function from ISR context.
     */
    const char *get_name() const;

    /** Get thread id
      @return  thread ID for reference by other functions.

      @note You may call this function from ISR context.
     */
    osThreadId_t get_id() const;

    /** Thread destructor
     *
     * @note You cannot call this function from ISR context.
     */
    virtual ~Thread();

private:
    // Required to share definitions without
    // delegated constructors
    void constructor(osPriority priority = osPriorityNormal,
                     uint32_t stack_size = OS_STACK_SIZE,
                     unsigned char *stack_mem = nullptr,
                     const char *name = nullptr);
    void constructor(uint32_t tz_module,
                     osPriority priority = osPriorityNormal,
                     uint32_t stack_size = OS_STACK_SIZE,
                     unsigned char *stack_mem = nullptr,
                     const char *name = nullptr);
    static void _thunk(void *thread_ptr);

    mbed::Callback<void()>     _task;
    osThreadId_t               _tid;
    osThreadAttr_t             _attr;
    bool                       _dynamic_stack;
    bool                       _finished;
    Semaphore                  _join_sem;
    mutable Mutex              _mutex;
    mbed_rtos_storage_thread_t _obj_mem;
};
/** @}*/
/** @}*/
}
#endif

#endif
