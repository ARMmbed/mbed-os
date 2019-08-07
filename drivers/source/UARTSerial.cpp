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
#include "drivers/UARTSerial.h"

#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN)

#include "platform/mbed_poll.h"
#include "platform/mbed_thread.h"

namespace mbed {

UARTSerial::UARTSerial(PinName tx, PinName rx, int baud) :
    SerialBase(tx, rx, baud),
    _blocking(true),
    _tx_irq_enabled(false),
    _rx_irq_enabled(false),
    _tx_enabled(true),
    _rx_enabled(true),
    _dcd_irq(NULL)
{
    /* Attatch IRQ routines to the serial device. */
    enable_rx_irq();
}

UARTSerial::~UARTSerial()
{
    delete _dcd_irq;
}

void UARTSerial::dcd_irq()
{
    wake();
}

void UARTSerial::set_baud(int baud)
{
    SerialBase::baud(baud);
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

void UARTSerial::set_format(int bits, Parity parity, int stop_bits)
{
    api_lock();
    SerialBase::format(bits, parity, stop_bits);
    api_unlock();
}

#if DEVICE_SERIAL_FC
void UARTSerial::set_flow_control(Flow type, PinName flow1, PinName flow2)
{
    api_lock();
    SerialBase::set_flow_control(type, flow1, flow2);
    api_unlock();
}
#endif

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
    api_lock();

    while (!_txbuf.empty()) {
        api_unlock();
        // Doing better than wait would require TxIRQ to also do wake() when becoming empty. Worth it?
        thread_sleep_for(1);
        api_lock();
    }

    api_unlock();

    return 0;
}

void UARTSerial::sigio(Callback<void()> func)
{
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

/* Special synchronous write designed to work from critical section, such
 * as in mbed_error_vprintf.
 */
ssize_t UARTSerial::write_unbuffered(const char *buf_ptr, size_t length)
{
    while (!_txbuf.empty()) {
        tx_irq();
    }

    for (size_t data_written = 0; data_written < length; data_written++) {
        SerialBase::_base_putc(*buf_ptr++);
    }

    return length;
}

ssize_t UARTSerial::write(const void *buffer, size_t length)
{
    size_t data_written = 0;
    const char *buf_ptr = static_cast<const char *>(buffer);

    if (length == 0) {
        return 0;
    }

    if (core_util_in_critical_section()) {
        return write_unbuffered(buf_ptr, length);
    }

    api_lock();

    // Unlike read, we should write the whole thing if blocking. POSIX only
    // allows partial as a side-effect of signal handling; it normally tries to
    // write everything if blocking. Without signals we can always write all.
    while (data_written < length) {

        if (_txbuf.full()) {
            if (!_blocking) {
                break;
            }
            do {
                api_unlock();
                thread_sleep_for(1); // XXX todo - proper wait?
                api_lock();
            } while (_txbuf.full());
        }

        while (data_written < length && !_txbuf.full()) {
            _txbuf.push(*buf_ptr++);
            data_written++;
        }

        core_util_critical_section_enter();
        if (_tx_enabled && !_tx_irq_enabled) {
            UARTSerial::tx_irq();                // only write to hardware in one place
            if (!_txbuf.empty()) {
                enable_tx_irq();
            }
        }
        core_util_critical_section_exit();
    }

    api_unlock();

    return data_written != 0 ? (ssize_t) data_written : (ssize_t) - EAGAIN;
}

ssize_t UARTSerial::read(void *buffer, size_t length)
{
    size_t data_read = 0;

    char *ptr = static_cast<char *>(buffer);

    if (length == 0) {
        return 0;
    }

    api_lock();

    while (_rxbuf.empty()) {
        if (!_blocking) {
            api_unlock();
            return -EAGAIN;
        }
        api_unlock();
        thread_sleep_for(1);  // XXX todo - proper wait?
        api_lock();
    }

    while (data_read < length && !_rxbuf.empty()) {
        _rxbuf.pop(*ptr++);
        data_read++;
    }

    core_util_critical_section_enter();
    if (_rx_enabled && !_rx_irq_enabled) {
        UARTSerial::rx_irq();               // only read from hardware in one place
        if (!_rxbuf.full()) {
            enable_rx_irq();
        }
    }
    core_util_critical_section_exit();

    api_unlock();

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

short UARTSerial::poll(short events) const
{

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

void UARTSerial::lock()
{
    // This is the override for SerialBase.
    // No lock required as we only use SerialBase from interrupt or from
    // inside our own critical section.
}

void UARTSerial::unlock()
{
    // This is the override for SerialBase.
}

void UARTSerial::api_lock(void)
{
    _mutex.lock();
}

void UARTSerial::api_unlock(void)
{
    _mutex.unlock();
}

void UARTSerial::rx_irq(void)
{
    bool was_empty = _rxbuf.empty();

    /* Fill in the receive buffer if the peripheral is readable
     * and receive buffer is not full. */
    while (!_rxbuf.full() && SerialBase::readable()) {
        char data = SerialBase::_base_getc();
        _rxbuf.push(data);
    }

    if (_rx_irq_enabled && _rxbuf.full()) {
        disable_rx_irq();
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
    char data;

    /* Write to the peripheral if there is something to write
     * and if the peripheral is available to write. */
    while (SerialBase::writeable() && _txbuf.pop(data)) {
        SerialBase::_base_putc(data);
    }

    if (_tx_irq_enabled && _txbuf.empty()) {
        disable_tx_irq();
    }

    /* Report the File handler that data can be written to peripheral. */
    if (was_full && !_txbuf.full() && !hup()) {
        wake();
    }
}

/* These are all called from critical section */
void UARTSerial::enable_rx_irq()
{
    SerialBase::attach(callback(this, &UARTSerial::rx_irq), RxIrq);
    _rx_irq_enabled = true;
}

void UARTSerial::disable_rx_irq()
{
    SerialBase::attach(NULL, RxIrq);
    _rx_irq_enabled = false;
}

void UARTSerial::enable_tx_irq()
{
    SerialBase::attach(callback(this, &UARTSerial::tx_irq), TxIrq);
    _tx_irq_enabled = true;
}

void UARTSerial::disable_tx_irq()
{
    SerialBase::attach(NULL, TxIrq);
    _tx_irq_enabled = false;
}

int UARTSerial::enable_input(bool enabled)
{
    core_util_critical_section_enter();
    if (_rx_enabled != enabled) {
        if (enabled) {
            UARTSerial::rx_irq();
            if (!_rxbuf.full()) {
                enable_rx_irq();
            }
        } else {
            disable_rx_irq();
        }
        _rx_enabled = enabled;
    }
    core_util_critical_section_exit();

    return 0;
}

int UARTSerial::enable_output(bool enabled)
{
    core_util_critical_section_enter();
    if (_tx_enabled != enabled) {
        if (enabled) {
            UARTSerial::tx_irq();
            if (!_txbuf.empty()) {
                enable_tx_irq();
            }
        } else {
            disable_tx_irq();
        }
        _tx_enabled = enabled;
    }
    core_util_critical_section_exit();

    return 0;
}

} //namespace mbed

#endif //(DEVICE_SERIAL && DEVICE_INTERRUPTIN)
