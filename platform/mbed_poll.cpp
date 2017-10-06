/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "Timer.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "rtos/Thread.h"
#endif

namespace mbed {

// timeout -1 forever, or milliseconds
int poll(pollfh fhs[], unsigned nfhs, int timeout)
{
    /**
     * TODO Proper wake-up mechanism.
     * In order to correctly detect availability of read/write a FileHandle, we needed
     * a select or poll mechanisms. We opted for poll as POSIX defines in
     * http://pubs.opengroup.org/onlinepubs/009695399/functions/poll.html Currently,
     * mbed::poll() just spins and scans filehandles looking for any events we are
     * interested in. In future, his spinning behaviour will be replaced with
     * condition variables.
     */
    Timer timer;
    if (timeout > 0) {
        timer.start();
    }

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
        if (timeout == 0 || (timeout > 0 && timer.read_ms() > timeout)) {
            break;
        }
#ifdef MBED_CONF_RTOS_PRESENT
        // TODO - proper blocking
        // wait for condition variable, wait queue whatever here
        rtos::Thread::yield();
#endif
    }
    return count;
}

} // namespace mbed
