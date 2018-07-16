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

#if defined(DEVICE_ITM)

#include "hal/itm_api.h"
#include "platform/FileHandle.h"

namespace mbed {

class SerialWireOutput : public FileHandle {

public:

    SerialWireOutput(void)
    {
        /* Initialize ITM using internal init function. */
        mbed_itm_init();
    }

    virtual ssize_t write(const void *buffer, size_t size)
    {
        mbed_itm_send_block(ITM_PORT_SWO, buffer, size);

        return size;
    }

    virtual ssize_t read(void *buffer, size_t size)
    {
        /* Reading is not supported by this file handle */
        return -EBADF;
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        /* Seeking is not support by this file handler */
        return -ESPIPE;
    }

    virtual off_t size()
    {
        /* Size is not defined for this file handle */
        return -EINVAL;
    }

    virtual int isatty()
    {
        /* File handle is used for terminal output */
        return true;
    }

    virtual int close()
    {
        return 0;
    }
};

} // namespace mbed

#endif
