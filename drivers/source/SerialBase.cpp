/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "drivers/SerialBase.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"

#if DEVICE_SERIAL

namespace mbed {

SerialBase::SerialBase(PinName tx, PinName rx, int baud) :
#if DEVICE_SERIAL_ASYNCH
    _thunk_irq(this), _tx_usage(DMA_USAGE_NEVER),
    _rx_usage(DMA_USAGE_NEVER), _tx_callback(NULL),
    _rx_callback(NULL), _tx_asynch_set(false),
    _rx_asynch_set(false),
#endif
    _serial(), _baud(baud)
{
    // No lock needed in the constructor

    for (size_t i = 0; i < sizeof _irq / sizeof _irq[0]; i++) {
        _irq[i] = NULL;
    }

    serial_init(&_serial, tx, rx);
    serial_baud(&_serial, _baud);
    serial_irq_handler(&_serial, SerialBase::_irq_handler, (uint32_t)this);
}

void SerialBase::baud(int baudrate)
{
    lock();
    serial_baud(&_serial, baudrate);
    _baud = baudrate;
    unlock();
}

void SerialBase::format(int bits, Parity parity, int stop_bits)
{
    lock();
    serial_format(&_serial, bits, (SerialParity)parity, stop_bits);
    unlock();
}

int SerialBase::readable()
{
    lock();
    int ret = serial_readable(&_serial);
    unlock();
    return ret;
}


int SerialBase::writeable()
{
    lock();
    int ret = serial_writable(&_serial);
    unlock();
    return ret;
}

void SerialBase::attach(Callback<void()> func, IrqType type)
{
    lock();
    // Disable interrupts when attaching interrupt handler
    core_util_critical_section_enter();
    if (func) {
        // lock deep sleep only the first time
        if (!_irq[type]) {
            sleep_manager_lock_deep_sleep();
        }
        _irq[type] = func;
        serial_irq_set(&_serial, (SerialIrq)type, 1);
    } else {
        // unlock deep sleep only the first time
        if (_irq[type]) {
            sleep_manager_unlock_deep_sleep();
        }
        _irq[type] = NULL;
        serial_irq_set(&_serial, (SerialIrq)type, 0);
    }
    core_util_critical_section_exit();
    unlock();
}

void SerialBase::_irq_handler(uint32_t id, SerialIrq irq_type)
{
    SerialBase *handler = (SerialBase *)id;
    if (handler->_irq[irq_type]) {
        handler->_irq[irq_type]();
    }
}

int SerialBase::_base_getc()
{
    // Mutex is already held
    return serial_getc(&_serial);
}

int SerialBase::_base_putc(int c)
{
    // Mutex is already held
    serial_putc(&_serial, c);
    return c;
}

void SerialBase::set_break()
{
    lock();
    serial_break_set(&_serial);
    unlock();
}

void SerialBase::clear_break()
{
    lock();
    serial_break_clear(&_serial);
    unlock();
}

void SerialBase::send_break()
{
    lock();
    // Wait for 1.5 frames before clearing the break condition
    // This will have different effects on our platforms, but should
    // ensure that we keep the break active for at least one frame.
    // We consider a full frame (1 start bit + 8 data bits bits +
    // 1 parity bit + 2 stop bits = 12 bits) for computation.
    // One bit time (in us) = 1000000/_baud
    // Twelve bits: 12000000/baud delay
    // 1.5 frames: 18000000/baud delay
    serial_break_set(&_serial);
    wait_us(18000000 / _baud);
    serial_break_clear(&_serial);
    unlock();
}

void SerialBase::lock()
{
    // Stub
}

void SerialBase:: unlock()
{
    // Stub
}

SerialBase::~SerialBase()
{
    // No lock needed in destructor

    // Detaching interrupts releases the sleep lock if it was locked
    for (int irq = 0; irq < IrqCnt; irq++) {
        attach(NULL, (IrqType)irq);
    }
}

#if DEVICE_SERIAL_FC
void SerialBase::set_flow_control(Flow type, PinName flow1, PinName flow2)
{
    lock();
    FlowControl flow_type = (FlowControl)type;
    switch (type) {
        case RTS:
            serial_set_flow_control(&_serial, flow_type, flow1, NC);
            break;

        case CTS:
            serial_set_flow_control(&_serial, flow_type, NC, flow1);
            break;

        case RTSCTS:
        case Disabled:
            serial_set_flow_control(&_serial, flow_type, flow1, flow2);
            break;

        default:
            break;
    }
    unlock();
}
#endif

#if DEVICE_SERIAL_ASYNCH

int SerialBase::write(const uint8_t *buffer, int length, const event_callback_t &callback, int event)
{
    int result = 0;
    lock();
    if (!serial_tx_active(&_serial) && !_tx_asynch_set) {
        start_write((void *)buffer, length, 8, callback, event);
    } else {
        result = -1; // transaction ongoing
    }
    unlock();
    return result;
}

int SerialBase::write(const uint16_t *buffer, int length, const event_callback_t &callback, int event)
{
    int result = 0;
    lock();
    if (!serial_tx_active(&_serial) && !_tx_asynch_set) {
        start_write((void *)buffer, length, 16, callback, event);
    } else {
        result = -1; // transaction ongoing
    }
    unlock();
    return result;
}

void SerialBase::start_write(const void *buffer, int buffer_size, char buffer_width, const event_callback_t &callback, int event)
{
    _tx_asynch_set = true;
    _tx_callback = callback;

    _thunk_irq.callback(&SerialBase::interrupt_handler_asynch);
    sleep_manager_lock_deep_sleep();
    serial_tx_asynch(&_serial, buffer, buffer_size, buffer_width, _thunk_irq.entry(), event, _tx_usage);
}

void SerialBase::abort_write(void)
{
    lock();
    core_util_critical_section_enter();
    if (_tx_asynch_set) {
        _tx_callback = NULL;
        _tx_asynch_set = false;
        serial_tx_abort_asynch(&_serial);
        sleep_manager_unlock_deep_sleep();
    }
    core_util_critical_section_exit();
    unlock();
}

void SerialBase::abort_read(void)
{
    lock();
    core_util_critical_section_enter();
    if (_rx_asynch_set) {
        _rx_callback = NULL;
        _rx_asynch_set = false;
        serial_rx_abort_asynch(&_serial);
        sleep_manager_unlock_deep_sleep();
    }
    core_util_critical_section_exit();
    unlock();
}

int SerialBase::set_dma_usage_tx(DMAUsage usage)
{
    if (serial_tx_active(&_serial)) {
        return -1;
    }
    _tx_usage = usage;
    return 0;
}

int SerialBase::set_dma_usage_rx(DMAUsage usage)
{
    if (serial_tx_active(&_serial)) {
        return -1;
    }
    _rx_usage = usage;
    return 0;
}

int SerialBase::read(uint8_t *buffer, int length, const event_callback_t &callback, int event, unsigned char char_match)
{
    int result = 0;
    lock();
    if (!serial_rx_active(&_serial) && !_rx_asynch_set) {
        start_read((void *)buffer, length, 8, callback, event, char_match);
    } else {
        result = -1; // transaction ongoing
    }
    unlock();
    return result;
}


int SerialBase::read(uint16_t *buffer, int length, const event_callback_t &callback, int event, unsigned char char_match)
{
    int result = 0;
    lock();
    if (!serial_rx_active(&_serial) && !_rx_asynch_set) {
        start_read((void *)buffer, length, 16, callback, event, char_match);
    } else {
        result = -1; // transaction ongoing
    }
    unlock();
    return result;
}


void SerialBase::start_read(void *buffer, int buffer_size, char buffer_width, const event_callback_t &callback, int event, unsigned char char_match)
{
    _rx_asynch_set = true;
    _rx_callback = callback;
    _thunk_irq.callback(&SerialBase::interrupt_handler_asynch);
    sleep_manager_lock_deep_sleep();
    serial_rx_asynch(&_serial, buffer, buffer_size, buffer_width, _thunk_irq.entry(), event, char_match, _rx_usage);
}

void SerialBase::interrupt_handler_asynch(void)
{
    int event = serial_irq_handler_asynch(&_serial);
    int rx_event = event & SERIAL_EVENT_RX_MASK;

    if (_rx_asynch_set && rx_event) {
        event_callback_t cb = _rx_callback;
        _rx_asynch_set = false;
        _rx_callback = NULL;
        if (cb) {
            cb.call(rx_event);
        }
        sleep_manager_unlock_deep_sleep();
    }

    int tx_event = event & SERIAL_EVENT_TX_MASK;
    if (_tx_asynch_set && tx_event) {
        event_callback_t cb = _tx_callback;
        _tx_asynch_set = false;
        _tx_callback = NULL;
        if (cb) {
            cb.call(tx_event);
        }
        sleep_manager_unlock_deep_sleep();
    }
}

#endif

} // namespace mbed

#endif
