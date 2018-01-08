/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#ifndef EVENT_FLAG_H
#define EVENT_FLAG_H

#include <stdint.h>
#include "cmsis_os2.h"
#include "mbed_rtos1_types.h"
#include "mbed_rtos_storage.h"

#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_EventFlags EventFlags class
 * @{
 */
 
/** The EventFlags class is used to signal or wait for an arbitrary event or events.
 @note 
 EventFlags support 31 flags so the MSB flag is ignored, it is used to return an error code (@a osFlagsError)
 @note
 Memory considerations: The EventFlags control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
class EventFlags : private mbed::NonCopyable<EventFlags> {
public:
    /** Create and Initialize an EventFlags object
     *
     * @note You cannot call this function from ISR context.
    */
    EventFlags();

    /** Create and Initialize a EventFlags object

     @param name name to be used for this EventFlags. It has to stay allocated for the lifetime of the thread.

     @note You cannot call this function from ISR context.
    */
    EventFlags(const char *name);

    /** Set the specified Event Flags.
      @param   flags  specifies the flags that shall be set.
      @return  event flags after setting or error code if highest bit set (@a osFlagsError).

      @note This function may be called from ISR context.
     */
    uint32_t set(uint32_t flags);

    /** Clear the specified Event Flags.
      @param   flags  specifies the flags that shall be cleared. (default: 0x7fffffff - all flags)
      @return  event flags before clearing or error code if highest bit set (@a osFlagsError).

      @note You may call this function from ISR context.
     */
    uint32_t clear(uint32_t flags = 0x7fffffff);

    /** Get the currently set Event Flags.
      @return  set event flags.

      @note You may call this function from ISR context.
     */
    uint32_t get() const;

    /** Wait for all of the specified event flags to become signaled.
      @param   flags    specifies the flags to wait for.
      @param   timeout  timeout value or 0 in case of no time-out. (default: osWaitForever)
      @param   clear    specifies wether to clear the flags after waiting for them. (default: true)
      @return  event flags before clearing or error code if highest bit set (@a osFlagsError).

      @note You may call this function from ISR context if the timeout parameter is set to 0.
     */
    uint32_t wait_all(uint32_t flags = 0, uint32_t timeout = osWaitForever, bool clear = true);

    /** Wait for any of the specified event flags to become signaled.
      @param   flags    specifies the flags to wait for. (default: 0)
      @param   timeout  timeout value or 0 in case of no time-out. (default: osWaitForever)
      @param   clear    specifies wether to clear the flags after waiting for them. (default: true)
      @return  event flags before clearing or error code if highest bit set (@a osFlagsError).

      @note This function may be called from ISR context if the timeout parameter is set to 0.
     */
    uint32_t wait_any(uint32_t flags = 0, uint32_t timeout = osWaitForever, bool clear = true);

    /** Event flags destructor

      @note You cannot call this function from ISR context.
    */
    ~EventFlags();

private:
    void constructor(const char *name = NULL);
    uint32_t wait(uint32_t flags, uint32_t opt, uint32_t timeout, bool clear);
    osEventFlagsId_t                _id;
    mbed_rtos_storage_event_flags_t _obj_mem;
};

/** @}*/
/** @}*/

}
#endif

