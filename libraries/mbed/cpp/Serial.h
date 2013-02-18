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

#include "platform.h"

#if DEVICE_SERIAL

#include "Stream.h"
#include "FunctionPointer.h"
#include "serial_api.h"

namespace mbed {

/** A serial port (UART) for communication with other serial devices
 *
 * Can be used for Full Duplex communication, or Simplex by specifying
 * one pin as NC (Not Connected)
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
class Serial : public Stream {

public:
    /** Create a Serial port, connected to the specified transmit and receive pins
     *
     *  @param tx Transmit pin
     *  @param rx Receive pin
     *
     *  @note
     *    Either tx or rx may be specified as NC if unused
     */
    Serial(PinName tx, PinName rx, const char *name=NULL);

    /** Set the baud rate of the serial port
     *
     *  @param baudrate The baudrate of the serial port (default = 9600).
     */
    void baud(int baudrate);

    enum Parity {
        None = 0,
        Odd,
        Even,
        Forced1,
        Forced0
    };

    enum IrqType {
        RxIrq = 0,
        TxIrq
    };

    /** Set the transmission format used by the Serial port
     *
     *  @param bits The number of bits in a word (5-8; default = 8)
     *  @param parity The parity used (Serial::None, Serial::Odd, Serial::Even, Serial::Forced1, Serial::Forced0; default = Serial::None)
     *  @param stop The number of stop bits (1 or 2; default = 1)
     */
    void format(int bits = 8, Parity parity=Serial::None, int stop_bits=1);

    /** Determine if there is a character available to read
     *
     *  @returns
     *    1 if there is a character available to read,
     *    0 otherwise
     */
    int readable();

    /** Determine if there is space available to write a character
     *
     *  @returns
     *    1 if there is space to write a character,
     *    0 otherwise
     */
    int writeable();

    /** Attach a function to call whenever a serial interrupt is generated
     *
     *  @param fptr A pointer to a void function, or 0 to set as none
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    void attach(void (*fptr)(void), IrqType type=RxIrq);

    /** Attach a member function to call whenever a serial interrupt is generated
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
     */
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void), IrqType type=RxIrq) {
        if((mptr != NULL) && (tptr != NULL)) {
            _irq[type].attach(tptr, mptr);
            serial_irq_set(&_serial, (SerialIrq)type, 1);
        }
    }

    static void _irq_handler(uint32_t id, SerialIrq irq_type);

protected:
    virtual int _getc();
    virtual int _putc(int c);

    serial_t        _serial;
    FunctionPointer _irq[2];
};

} // namespace mbed

#endif

#endif
