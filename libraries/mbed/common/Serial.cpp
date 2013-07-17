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
#include "Serial.h"
#include "wait_api.h"

#if DEVICE_SERIAL

namespace mbed {

Serial::Serial(PinName tx, PinName rx, const char *name) : Stream(name) {
    serial_init(&_serial, tx, rx);
    _baud = 9600;
    serial_irq_handler(&_serial, Serial::_irq_handler, (uint32_t)this);
}

void Serial::baud(int baudrate) {
    serial_baud(&_serial, baudrate);
    _baud = baudrate;
}

void Serial::format(int bits, Parity parity, int stop_bits) {
    serial_format(&_serial, bits, (SerialParity)parity, stop_bits);
}

int Serial::readable() {
    return serial_readable(&_serial);
}


int Serial::writeable() {
    return serial_writable(&_serial);
}

void Serial::attach(void (*fptr)(void), IrqType type) {
    if (fptr) {
        _irq[type].attach(fptr);
        serial_irq_set(&_serial, (SerialIrq)type, 1);
    } else {
        serial_irq_set(&_serial, (SerialIrq)type, 0);
    }
}


void Serial::_irq_handler(uint32_t id, SerialIrq irq_type) {
    Serial *handler = (Serial*)id;
    handler->_irq[irq_type].call();
}

int Serial::_getc() {
    return serial_getc(&_serial);
}

int Serial::_putc(int c) {
    serial_putc(&_serial, c);
    return c;
}

void Serial::send_break() {
  // Wait for 1.5 frames before clearing the break condition
  // This will have different effects on our platforms, but should
  // ensure that we keep the break active for at least one frame.
  // We consider a full frame (1 start bit + 8 data bits bits + 
  // 1 parity bit + 2 stop bits = 12 bits) for computation.
  // One bit time (in us) = 1000000/_baud
  // Twelve bits: 12000000/baud delay
  // 1.5 frames: 18000000/baud delay
  serial_break_set(&_serial);
  wait_us(18000000/_baud);
  serial_break_clear(&_serial);
}

} // namespace mbed

#endif
