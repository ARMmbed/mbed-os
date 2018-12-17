/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed_poll.h"
#include "FileHandle.h"
#if MBED_CONF_RTOS_PRESENT
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/EventFlags.h"
using namespace rtos;
#else
#include "drivers/Timer.h"
#include "drivers/LowPowerTimer.h"
#endif

namespace mbed {


#if MBED_CONF_RTOS_PRESENT
#define FILE_CHANGED_FLAG 0X01
static EventFlags poll_event_flags;
void on_fd_change()
{
    poll_event_flags.set(FILE_CHANGED_FLAG);
}
#endif


// timeout -1 forever, or milliseconds
int poll(pollfh fhs[], unsigned nfhs, int timeout)
{

#if MBED_CONF_RTOS_PRESENT
    uint64_t start_time = 0;
    if (timeout > 0) {
        start_time = Kernel::get_ms_count();
    }
#define TIME_ELAPSED() int64_t(Kernel::get_ms_count() - start_time)
#else
#if MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER
    LowPowerTimer timer;
#else
    Timer timer;
#endif
    if (timeout > 0) {
        timer.start();
    }
#define TIME_ELAPSED() timer.read_ms()
#endif // MBED_CONF_RTOS_PRESENT

#if MBED_CONF_RTOS_PRESENT
    /* Register for sigio event */
    for(unsigned n = 0; n < nfhs; n++) {
        fhs[n].fh->sigio(callback(on_fd_change));
    }
#endif // MBED_CONF_RTOS_PRESENT

    int count = 0;
    for (;;) {
        /* Scan the file handles */
        for (unsigned n = 0; n < nfhs; n++) {
            FileHandle *fh = fhs[n].fh;
            short mask = fhs[n].events | POLLERR | POLLHUP | POLLNVAL;
            if (fh) {
                fhs[n].revents = fh->poll(mask) & mask;
            } else {
                fhs[n].revents = POLLNVAL;
            }
            if (fhs[n].revents) {
                count++;
            }
        }

        if (count) {
            break;
        }

        /* Nothing selected - this is where timeout handling would be needed */
        if (timeout == 0 || (timeout > 0 && TIME_ELAPSED() > timeout)) {
            break;
        }
#if MBED_CONF_RTOS_PRESENT
        /* Block until being notified of an event or until the specified timeout */
        poll_event_flags.wait_all(FILE_CHANGED_FLAG, 
                                (timeout >= 0) ? timeout - TIME_ELAPSED() : osWaitForever);
#endif // MBED_CONF_RTOS_PRESENT
    }
#if MBED_CONF_RTOS_PRESENT
    /* Unregister the sigio */
    for(unsigned n = 0; n < nfhs; n++) {
        fhs[n].fh->sigio(NULL);
    }
#endif // MBED_CONF_RTOS_PRESENT
    return count;
}

} // namespace mbed

#if MBED_CONF_RTOS_PRESENT
#undef FILE_CHANGED_FLAG
#endif // MBED_CONF_RTOS_PRESENT
