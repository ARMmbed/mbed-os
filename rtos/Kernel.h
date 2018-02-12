/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** Functions in the Kernel namespace control RTOS kernel information. */
namespace Kernel {

/** Read the current RTOS kernel millisecond tick count.
     The tick count corresponds to the tick count used by the RTOS for timing
     purposes. It increments monotonically from 0 at boot, hence effectively
     never wraps. If the underlying RTOS only provides a 32-bit tick count,
     this method expands it to 64 bits.
     @return  RTOS kernel current tick count
     @note mbed OS always uses millisecond RTOS ticks, and this could only wrap
           after half a billion years
     @note You cannot call this function from ISR context.
 */
uint64_t get_ms_count();

} // namespace Kernel

} // namespace rtos
#endif

/** @}*/
