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
#ifndef MBED_POLL_H
#define MBED_POLL_H

#define POLLIN         0x0001 ///< Data may be read without blocking
#define POLLOUT        0x0010 ///< Data may be written without blocking
#define POLLERR        0x1000 ///< An error has occurred on the device or stream
#define POLLHUP        0x2000 ///< The device has been disconnected
#define POLLNVAL       0x4000 ///< The specified file handle value is invalid

namespace mbed {

class FileHandle;

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_poll poll functions
 * @{
 */

struct pollfh {
    FileHandle *fh;
    short events;
    short revents;
};

/** A mechanism to multiplex input/output over a set of file handles(file descriptors).
 * For every file handle provided, poll() examines it for any events registered for that particular
 * file handle.
 *
 * @param fhs     an array of PollFh struct carrying a FileHandle and bitmasks of events
 * @param nfhs    number of file handles
 * @param timeout timer value to timeout or -1 for loop forever
 *
 * @return number of file handles selected (for which revents is non-zero). 0 if timed out with nothing selected. -1 for error.
 */
int poll(pollfh fhs[], unsigned nfhs, int timeout);

/**@}*/

/**@}*/

} // namespace mbed

#endif //MBED_POLL_H
