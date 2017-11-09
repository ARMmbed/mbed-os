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
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "cmsis_os2.h"
#include "mbed_rtos1_types.h"
#include "mbed_rtos_storage.h"
#include "platform/NonCopyable.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/
/**
 * \defgroup rtos_Semaphore Semaphore class
 * @{
 */

/** The Semaphore class is used to manage and protect access to a set of shared resources.
 *
 * @note
 * Memory considerations: The semaphore control structures will be created on current thread's stack, both for the mbed OS
 * and underlying RTOS objects (static or dynamic RTOS memory pools are not being used).
 */
class Semaphore : private mbed::NonCopyable<Semaphore> {
public:
    /** Create and Initialize a Semaphore object used for managing resources.
      @param count      number of available resources; maximum index value is (count-1). (default: 0).
    */
    Semaphore(int32_t count=0);

    /** Create and Initialize a Semaphore object used for managing resources.
      @param  count     number of available resources
      @param  max_count maximum number of available resources
    */
    Semaphore(int32_t count, uint16_t max_count);

    /** Wait until a Semaphore resource becomes available.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  number of available tokens, before taking one; or -1 in case of incorrect parameters
    */
    int32_t wait(uint32_t millisec=osWaitForever);

    /** Release a Semaphore resource that was obtain with Semaphore::wait.
      @return status code that indicates the execution status of the function:
              @a osOK the token has been correctly released.
              @a osErrorResource the maximum token count has been reached.
              @a osErrorParameter internal error.
    */
    osStatus release(void);

    ~Semaphore();

private:
    void constructor(int32_t count, uint16_t max_count);

    osSemaphoreId_t               _id;
    mbed_rtos_storage_semaphore_t _obj_mem;
};
/** @}*/
/** @}*/
}
#endif


