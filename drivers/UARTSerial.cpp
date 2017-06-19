/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#if DEVICE_SERIAL

#include <errno.h>
#include "UARTSerial.h"
#include "platform/mbed_poll.h"
#include "platform/mbed_wait_api.h"

namespace mbed {

UARTSerial::UARTSerial(PinName tx, PinName rx, int baud) :
        SerialBase(tx, rx, baud),
        _blocking(true),
        _tx_irq_enabled(false),
        _dcd_irq(NULL)
{
    /* Attatch IRQ routines to the serial device. */
    SerialBase::attach(callback(this, &UARTSerial::rx_irq), RxIrq);
}

UARTSerial::~UARTSerial()
{
    delete _dcd_irq;
}

void UARTSerial::dcd_irq()
{
    wake();
}

void UARTSerial::set_data_carrier_detect(PinName dcd_pin, bool active_high)
{
     delete _dcd_irq;
    _dcd_irq = NULL;

    if (dcd_pin != NC) {
        _dcd_irq = new InterruptIn(dcd_pin);
        if (active_high) {
            _dcd_irq->fall(callback(this, &UARTSerial::dcd_irq));
        } else {
            _dcd_irq->rise(callback(this, &UARTSerial::dcd_irq));
        }
    }
}

int UARTSerial::close()
{
    /* Does not let us pass a file descriptor. So how to close ?
     * Also, does it make sense to close a device type file descriptor*/
    return 0;
}

int UARTSerial::isatty()
{
    return 1;

}

off_t UARTSerial::seek(off_t offset, int whence)
{
    /*XXX lseek can be done theoratically, but is it sane to mark positions on a dynamically growing/shrinking
     * buffer system (from an interrupt context) */
    return -ESPIPE;
}

int UARTSerial::sync()
{
    lock();

    while (!_txbuf.empty()) {
        unlock();
        // Doing better than wait would require TxIRQ to also do wake() when becoming empty. Worth it?
        wait_ms(1);
        lock();
    }

    unlock();

    return 0;
}

void UARTSerial::sigio(Callback<void()> func) {
    core_util_critical_section_enter();
    _sigio_cb = func;
    if (_sigio_cb) {
        short current_events = poll(0x7FFF);
        if (current_events) {
            _sigio_cb();
        }
    }
    core_util_critical_section_exit();
}

ssize_t UARTSerial::write(const void* buffer, size_t length)
{
    size_t data_written = 0;
    const char *buf_ptr = static_cast<const char *>(buffer);

    lock();

    while (_txbuf.full()) {
        if (!_blocking) {
            unlock();
            return -EAGAIN;
        }
        unlock();
        wait_ms(1); // XXX todo - proper wait, WFE for non-rtos ?
        lock();
    }

    while (data_written < length && !_txbuf.full()) {
        _txbuf.push(*buf_ptr++);
        data_written++;
    }

    core_util_critical_section_enter();
    if (!_tx_irq_enabled) {
        UARTSerial::tx_irq();                // only write to hardware in one place
        if (!_txbuf.empty()) {
            SerialBase::attach(callback(this, &UARTSerial::tx_irq), TxIrq);
            _tx_irq_enabled = true;
        }
    }
    core_util_critical_section_exit();

    unlock();

    return data_written;
}

ssize_t UARTSerial::read(void* buffer, size_t length)
{
    size_t data_read = 0;

    char *ptr = static_cast<char *>(buffer);

    lock();

    while (_rxbuf.empty()) {
        if (!_blocking) {
            unlock();
            return -EAGAIN;
        }
        unlock();
        wait_ms(1);  // XXX todo - proper wait, WFE for non-rtos ?
        lock();
    }

    while (data_read < length && !_rxbuf.empty()) {
        _rxbuf.pop(*ptr++);
        data_read++;
    }

    unlock();

    return data_read;
}

bool UARTSerial::hup() const
{
    return _dcd_irq && _dcd_irq->read() != 0;
}

void UARTSerial::wake()
{
    if (_sigio_cb) {
        _sigio_cb();
    }
}

short UARTSerial::poll(short events) const {

    short revents = 0;
    /* Check the Circular Buffer if space available for writing out */


    if (!_rxbuf.empty()) {
        revents |= POLLIN;
    }

    /* POLLHUP and POLLOUT are mutually exclusive */
    if (hup()) {
        revents |= POLLHUP;
    } else if (!_txbuf.full()) {
        revents |= POLLOUT;
    }

    /*TODO Handle other event types */

    return revents;
}

void UARTSerial::lock(void)
{
    _mutex.lock();
}

void UARTSerial::unlock(void)
{
    _mutex.unlock();
}

void UARTSerial::rx_irq(void)
{
    bool was_empty = _rxbuf.empty();

    /* Fill in the receive buffer if the peripheral is readable
     * and receive buffer is not full. */
    while (SerialBase::readable()) {
        char data = SerialBase::_base_getc();
        if (!_rxbuf.full()) {
            _rxbuf.push(data);
        } else {
            /* Drop - can we report in some way? */
        }
    }

    /* Report the File handler that data is ready to be read from the buffer. */
    if (was_empty && !_rxbuf.empty()) {
        wake();
    }
}

// Also called from write to start transfer
void UARTSerial::tx_irq(void)
{
    bool was_full = _txbuf.full();

    /* Write to the peripheral if there is something to write
     * and if the peripheral is available to write. */
    while (!_txbuf.empty() && SerialBase::writeable()) {
        char data;
        _txbuf.pop(data);
        SerialBase::_base_putc(data);
    }

    if (_tx_irq_enabled && _txbuf.empty()) {
        SerialBase::attach(NULL, TxIrq);
        _tx_irq_enabled = false;
    }

    /* Report the File handler that data can be written to peripheral. */
    if (was_full && !_txbuf.full() && !hup()) {
        wake();
    }
}

} //namespace mbed

#endif //DEVICE_SERIAL
