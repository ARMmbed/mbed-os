/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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
#include "cmsis_os.h"
#include "platform/Callback.h"
#include "platform/toolchain.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** The RtosTimer class allow creating and and controlling of timer functions in the system.
 A timer function is called when a time period expires whereby both on-shot and
 periodic timers are possible. A timer can be started, restarted, or stopped.

 Timers are handled in the thread osTimerThread.
 Callback functions run under control of this thread and may use CMSIS-RTOS API calls.
*/
class RtosTimer {
public:
    /** Create timer.
      @param   func      function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behaviour. (default: osTimerPeriodic)
      @param   argument  argument to the timer call back function. (default: NULL)
      @deprecated Replaced with RtosTimer(Callback<void()>, os_timer_type)
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "Replaced with RtosTimer(Callback<void()>, os_timer_type)")
    RtosTimer(void (*func)(void const *argument), os_timer_type type=osTimerPeriodic, void *argument=NULL) {
        constructor(mbed::callback((void (*)(void *))func, argument), type);
    }
    
    /** Create timer.
      @param   func      function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behaviour. (default: osTimerPeriodic)
    */
    RtosTimer(mbed::Callback<void()> func, os_timer_type type=osTimerPeriodic) {
        constructor(func, type);
    }
    
    /** Create timer.
      @param   obj       pointer to the object to call the member function on.
      @param   method    member function to be executed by this timer.
      @param   type      osTimerOnce for one-shot or osTimerPeriodic for periodic behaviour. (default: osTimerPeriodic)
      @deprecated
          The RtosTimer constructor does not support cv-qualifiers. Replaced by
          RtosTimer(callback(obj, method), os_timer_type).
    */
    template <typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "The RtosTimer constructor does not support cv-qualifiers. Replaced by "
        "RtosTimer(callback(obj, method), os_timer_type).")
    RtosTimer(T *obj, M method, os_timer_type type=osTimerPeriodic) {
        constructor(mbed::callback(obj, method), type);
    }

    /** Stop the timer.
      @return  status code that indicates the execution status of the function.
    */
    osStatus stop(void);

    /** Start the timer.
      @param   millisec  time delay value of the timer.
      @return  status code that indicates the execution status of the function.
    */
    osStatus start(uint32_t millisec);

    ~RtosTimer();

private:
    // Required to share definitions without
    // delegated constructors
    void constructor(mbed::Callback<void()> func, os_timer_type type);
    
    mbed::Callback<void()> _function;
    osTimerId _timer_id;
    osTimerDef_t _timer;
#if defined(CMSIS_OS_RTX) && !defined(__MBED_CMSIS_RTOS_CM)
    uint32_t _timer_data[5];
#else
    uint32_t _timer_data[6];
#endif
};

}

#endif

/** @}*/
