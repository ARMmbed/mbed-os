/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef RTOS_TIMER_H
#define RTOS_TIMER_H

#include <stdint.h>
#include "rtos/mbed_rtos_types.h"
#include "rtos/mbed_rtos_storage.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "platform/mbed_toolchain.h"
#include "rtos/mbed_rtos1_types.h"

#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY)

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

/**
 * \defgroup rtos_RtosTimer RtosTimer class
 * @{
 */

/** The RtosTimer class allow creating and and controlling of timer functions in the system.
 A timer function is called when a time period expires whereby both on-shot and
 periodic timers are possible. A timer can be started, restarted, or stopped.

 Timers are handled in the thread osTimerThread.
 Callback functions run under control of this thread and may use CMSIS-RTOS API calls.

 @deprecated
 The RtosTimer has been superseded by the EventQueue. The RtosTimer and EventQueue duplicate
 the functionality of timing events outside of interrupt context, however the EventQueue
 has additional features to handle deferring other events to multiple contexts.

 For an example, the following code shows a simple use of the RtosTimer:
 @code
 DigitalOut led(LED1);
 void blink() {
     led = !led;
 }

 RtosTimer timer(&blink);
 int main() {
     timer.start(1000); // call blink every 1s
     ThisThread::sleep_for(5000);
     timer.stop(); // stop after 5s
 }
 @endcode

 This is the above example rewritten to use the EventQueue:
 @code
 DigitalOut led(LED1);
 void blink() {
     led = !led;
 }

 EventQueue queue(4*EVENTS_EVENT_SIZE);
 int main() {
    int blink_id = queue.call_every(1000, &blink); // call blink every 1s
    queue.dispatch(5000);
    queue.cancel(blink_id); // stop after 5s
 }
 @endcode

 @note
 Memory considerations: The timer control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
class RtosTimer : private mbed::NonCopyable<RtosTimer> {
public:
    /** Create timer.
      @param   func      function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behavior. (default: osTimerPeriodic)
      @param   argument  argument to the timer call back function. (default: nullptr)
      @deprecated Replaced with RtosTimer(Callback<void()>, os_timer_type)
      @deprecated
          The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details

      @note You cannot call this function from ISR context.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "Replaced with RtosTimer(Callback<void()>, os_timer_type)")
    MBED_DEPRECATED_SINCE("mbed-os-5.2",
                          "The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details")
    RtosTimer(void (*func)(void const *argument), os_timer_type type = osTimerPeriodic, void *argument = nullptr)
    {
        constructor(mbed::callback((void (*)(void *))func, argument), type);
    }

    /** Create timer.
      @param   func      function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behavior. (default: osTimerPeriodic)
      @deprecated
          The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details

      @note You cannot call this function from ISR context.
    */
    MBED_DEPRECATED_SINCE("mbed-os-5.2",
                          "The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details")
    RtosTimer(mbed::Callback<void()> func, os_timer_type type = osTimerPeriodic)
    {
        constructor(func, type);
    }

    /** Create timer.
      @param   obj       pointer to the object to call the member function on.
      @param   method    member function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behavior. (default: osTimerPeriodic)
      @deprecated
          The RtosTimer constructor does not support cv-qualifiers. Replaced by
          RtosTimer(callback(obj, method), os_timer_type).
      @deprecated
          The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details

      @note You cannot call this function from ISR context.
    */
    template <typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "The RtosTimer constructor does not support cv-qualifiers. Replaced by "
                          "RtosTimer(callback(obj, method), os_timer_type).")
    MBED_DEPRECATED_SINCE("mbed-os-5.2",
                          "The RtosTimer has been superseded by the EventQueue. See RtosTimer.h for more details")
    RtosTimer(T *obj, M method, os_timer_type type = osTimerPeriodic)
    {
        constructor(mbed::callback(obj, method), type);
    }

    /** Stop the timer.
      @return  status code that indicates the execution status of the function:
          @a osOK the timer has been stopped.
          @a osErrorISR @a stop cannot be called from interrupt service routines.
          @a osErrorParameter internal error.
          @a osErrorResource the timer is not running.

      @note You cannot call this function from ISR context.
    */
    osStatus stop(void);

    /** Start or restart the timer.
      @param   millisec  non-zero value of the timer.
      @return  status code that indicates the execution status of the function:
          @a osOK the timer has been started or restarted.
          @a osErrorISR @a start cannot be called from interrupt service routines.
          @a osErrorParameter internal error or incorrect parameter value.
          @a osErrorResource internal error (the timer is in an invalid timer state).

      @note You cannot call this function from ISR context.
    */
    osStatus start(uint32_t millisec);

    /** RtosTimer destructor
     *
     * @note You cannot call this function from ISR context.
     */
    ~RtosTimer();

private:
    // Required to share definitions without
    // delegated constructors
    void constructor(mbed::Callback<void()> func, os_timer_type type);

    osTimerId_t _id;
    mbed_rtos_storage_timer_t _obj_mem;
    mbed::Callback<void()> _function;
};
/** @}*/
/** @}*/

}

#endif

#endif
