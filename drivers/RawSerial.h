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
#ifndef MBED_RAW_SERIAL_H
#define MBED_RAW_SERIAL_H

#include "platform/platform.h"

#if DEVICE_SERIAL || defined(DOXYGEN_ONLY)

#include "platform/mbed_toolchain.h"
#include "drivers/SerialBase.h"
#include "platform/NonCopyable.h"
#include <cstdarg>

namespace mbed {

/**
 * \defgroup drivers_RawSerial RawSerial class
 * \ingroup drivers-public-api-uart
 * @{
 */

/** @deprecated
 * A serial port (UART) for communication with other serial devices
 * This is a variation of the Serial class that doesn't use streams,
 * thus making it safe to use in interrupt handlers with the RTOS.
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected)
 *
 * @note Synchronization level: Not protected
 *
 * Example:
 * @code
 * // Send a char to the PC
 *
 * #include "mbed.h"
 *
 * RawSerial pc(USBTX, USBRX);
 *
 * int main() {
 *     pc.putc('A');
 * }
 * @endcode
 */
class
    MBED_DEPRECATED_SINCE(
        "mbed-os-6.0.0",
        "Use UnbufferedSerial instead."
    ) RawSerial: public SerialBase, private NonCopyable<RawSerial> {

public:
    /** @deprecated
     * Create a RawSerial port, connected to the specified transmit and receive pins, with the specified baud.
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    RawSerial(PinName tx, PinName rx, int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);

    /** @deprecated
     * Write a char to the serial port
     *
     * @param c The char to write
     *
     * @returns The written char or -1 if an error occurred
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    int putc(int c);

    /** @deprecated
     * Read a char from the serial port
     *
     * @returns The char read from the serial port
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    int getc();

    /** @deprecated
     * Write a string to the serial port
     *
     * @param str The string to write
     *
     * @returns 0 if the write succeeds, EOF for error
     */
    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    int puts(const char *str);

    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    int printf(const char *format, ...) MBED_PRINTF_METHOD(1, 2);

    MBED_DEPRECATED("The class has been deprecated and will be removed in the future.")
    int vprintf(const char *format, std::va_list arg);

#if !(DOXYGEN_ONLY)
protected:

    /* Acquire exclusive access to this serial port
     */
    virtual void lock(void);

    /* Release exclusive access to this serial port
     */
    virtual void unlock(void);
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
