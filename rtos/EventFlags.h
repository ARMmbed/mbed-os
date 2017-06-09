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

/** The EventFlags class is used to signal to whom it may concern about an event has occured.
 @note 
 EventFlags support 31 flags so the MSB flag is ignored, it is used to return error code (osFlagsError)
 @note
 Memory considerations: The EventFlags control structures will be created on current thread's stack, both for the mbed OS
 and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
*/
class EventFlags : private mbed::NonCopyable<EventFlags> {
public:
    /** Create and Initialize a EventFlags object */
    EventFlags();

    /** Create and Initialize a EventFlags object

     @param name name to be used for this EventFlags. It has to stay allocated for the lifetime of the thread.
    */
    EventFlags(const char *name);

    /** Set the specified Event Flags.
      @param   flags  specifies the flags that shall be set. (default: 0x7fffffff)
      @return  event flags after setting or error code if highest bit set (osFlagsError).
     */
    uint32_t set(uint32_t flags = 0x7fffffff);

    /** Clear the specified Event Flags.
      @param   flags  specifies the flags that shall be cleared. (default: 0x7fffffff0)
      @return  event flags before clearing or error code if highest bit set (osFlagsError).
     */
    uint32_t clear(uint32_t flags = 0x7fffffff);

    /** Get the current Event Flags.
      @return  current event flags.
     */
    uint32_t get();

    /** Wait for one or more Event Flags to become signaled.
      @param   flags    specifies the flags to wait for. (default: 0)
      @param   timeout  timeout value or 0 in case of no time-out. (default: osWaitForever)
      @return  event flags before clearing or error code if highest bit set (osFlagsError).
      @note    incase of flags 0 the function will wait to any flag and will not clear the flags, 
               the user must clear the flags. otherwise the function to wait all specified flags and clear them. 
     */
    uint32_t wait(uint32_t flags = 0, uint32_t timeout = osWaitForever);

    ~EventFlags();

private:
    void constructor(const char *name = NULL);
    osEventFlagsId_t                _id;
    osEventFlagsAttr_t              _attr;
    mbed_rtos_storage_event_flags_t _obj_mem;
};

}
#endif

/** @}*/
