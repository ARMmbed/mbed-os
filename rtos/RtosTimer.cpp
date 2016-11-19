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
#include "rtos/RtosTimer.h"

#include <string.h>

#include "mbed.h"
#include "cmsis_os.h"
#include "platform/mbed_error.h"

namespace rtos {

void RtosTimer::constructor(mbed::Callback<void()> func, os_timer_type type) {
#ifdef CMSIS_OS_RTX
    _timer.ptimer = (void (*)(const void *))Callback<void()>::thunk;

    memset(_timer_data, 0, sizeof(_timer_data));
    _timer.timer = _timer_data;
#endif
    _function = func;
    _timer_id = osTimerCreate(&_timer, type, &_function);
}

osStatus RtosTimer::start(uint32_t millisec) {
    return osTimerStart(_timer_id, millisec);
}

osStatus RtosTimer::stop(void) {
    return osTimerStop(_timer_id);
}

RtosTimer::~RtosTimer() {
    osTimerDelete(_timer_id);
}

}
