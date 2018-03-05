/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#include "FileHandle.h"
#include "platform/mbed_retarget.h"
#include "platform/mbed_critical.h"

namespace mbed {

off_t FileHandle::size()
{
    /* remember our current position */
    off_t off = seek(0, SEEK_CUR);
    if (off < 0) {
        return off;
    }
    /* seek to the end to get the file length */
    off_t size = seek(0, SEEK_END);
    /* return to our old position */
    seek(off, SEEK_SET);
    return size;
}

} // namespace mbed
