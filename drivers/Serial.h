/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_SERIAL_H
#define MBED_SERIAL_H

#include "platform/platform.h"

#if defined (DEVICE_SERIAL) || defined(DOXYGEN_ONLY)

#include "Stream.h"
#include "SerialBase.h"
#include "PlatformMutex.h"
#include "serial_api.h"

namespace mbed {
/** \addtogroup drivers */

/** A serial port (UART) for communication with other serial devices
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected)
 *
 * @note Synchronization level: Thread safe
 *
 * Example:
 * @code
 * // Print "Hello World" to the PC
 *
 * #include "mbed.h"
 *
 * Serial pc(USBTX, USBRX);
 *
 * int main() {
 *     pc.printf("Hello World\n");
 * }
 * @endcode
 * @ingroup drivers
 */
class Serial : public SerialBase, public Stream {

public:
#if DEVICE_SERIAL_ASYNCH
    using SerialBase::read;
    using SerialBase::write;
#endif

    /** Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param name The name of the stream associated with this serial port (optional)
     *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
    Serial(PinName tx, PinName rx, const char *name=NULL, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);


    /** Create a Serial port, connected to the specified transmit and receive pins, with the specified baud
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param baud The baud rate of the serial port
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
    Serial(PinName tx, PinName rx, int baud);

    /* Stream gives us a FileHandle with non-functional poll()/readable()/writable. Pass through
     * the calls from the SerialBase instead for backwards compatibility. This problem is
     * part of why Stream and Serial should be deprecated.
     */
    bool readable()
    {
        return SerialBase::readable();
    }
    bool writable()
    {
        return SerialBase::writeable();
    }
    bool writeable()
    {
        return SerialBase::writeable();
    }

protected:
    virtual int _getc();
    virtual int _putc(int c);
    virtual void lock();
    virtual void unlock();

    PlatformMutex _mutex;
};

} // namespace mbed

#endif

#endif
