/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef MBED_SERIALWIREOUTPUT_H
#define MBED_SERIALWIREOUTPUT_H

#include "platform/platform.h"

#if defined(DEVICE_ITM)

#include "platform/FileHandle.h"

namespace mbed {
/**
 * \defgroup drivers_SerialWireOutput SerialWireOutput class
 * \ingroup drivers-public-api
 * @{
 */

class SerialWireOutput : public FileHandle {

public:

    SerialWireOutput(void);

    virtual ssize_t write(const void *buffer, size_t size);

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

/** @}*/

} // namespace mbed

#endif // DEVICE_ITM

#endif // MBED_SERIALWIREOUTPUT_H
