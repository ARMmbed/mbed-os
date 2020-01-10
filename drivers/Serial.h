/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_SERIAL_H
#define MBED_SERIAL_H

#include "platform/platform.h"

#if DEVICE_SERIAL || defined(DOXYGEN_ONLY)

#include "platform/Stream.h"
#include "drivers/SerialBase.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \defgroup drivers_Serial Serial class
 * \ingroup drivers-public-api-uart
 * @{
 */

/** @deprecated
 * A serial port (UART) for communication with other serial devices
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
 */
class
    MBED_DEPRECATED_SINCE(
        "mbed-os-6.0.0",
        "Use printf and puts instead to access the console, BufferedSerial for blocking applications or UnbufferedSerial if bypassing locks in IRQ or short of RAM."
    ) Serial : public SerialBase, public Stream, private NonCopyable<Serial> {

public:
#if DEVICE_SERIAL_ASYNCH
    using SerialBase::read;
    using SerialBase::write;
#endif

    /** Resolve ambiguities in SerialBase and FileHandle
     * (for enable_input and enable_output)
     */
    using SerialBase::enable_input;
    using SerialBase::enable_output;

    /** @deprecated
     * Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param name The name of the stream associated with this serial port (optional)
     *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE or 9600)
     *
     *  @note
     *    Either tx or rx may be specified as NC (Not Connected) if unused
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(PinName tx, PinName rx, const char *name = NULL, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);

    /** @deprecated
     * Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     *  @param name The name of the stream associated with this serial port (optional)
     *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE or 9600)
     *
     *  @note
     *    Either tx or rx may be specified as NC (Not Connected) if unused
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(const serial_pinmap_t &static_pinmap, const char *name = NULL, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(const serial_pinmap_t &&, const char * = NULL, int = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) = delete; // prevent passing of temporary objects

    /** @deprecated
     * Create a Serial port, connected to the specified transmit and receive pins, with the specified baud
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param baud The baud rate of the serial port
     *
     *  @note
     *    Either tx or rx may be specified as NC (Not Connected) if unused
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(PinName tx, PinName rx, int baud);

    /** @deprecated
     * Create a Serial port, connected to the specified transmit and receive pins, with the specified baud
     *
     *  @param static_pinmap reference to structure which holds static pinmap.
     *  @param baud The baud rate of the serial port
     *
     *  @note
     *    Either tx or rx may be specified as NC (Not Connected) if unused
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(const serial_pinmap_t &static_pinmap, int baud);
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    Serial(const serial_pinmap_t &&, int) = delete; // prevent passing of temporary objects

    /* Stream gives us a FileHandle with non-functional poll()/readable()/writable. Pass through
     * the calls from the SerialBase instead for backwards compatibility. This problem is
     * part of why Stream and Serial should be deprecated.
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    bool readable()
    {
        return SerialBase::readable();
    }
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    bool writable()
    {
        return SerialBase::writeable();
    }
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    bool writeable()
    {
        return SerialBase::writeable();
    }

#if !(DOXYGEN_ONLY)
protected:
    virtual int _getc();
    virtual int _putc(int c);
    virtual void lock();
    virtual void unlock();

    PlatformMutex _mutex;
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
