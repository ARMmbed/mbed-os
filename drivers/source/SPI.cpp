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
#include "drivers/SPI.h"
#include "platform/mbed_critical.h"

#if DEVICE_SPI_ASYNCH
#include "platform/mbed_power_mgmt.h"
#endif

#if DEVICE_SPI

namespace mbed {

SPI::spi_peripheral_s SPI::_peripherals[SPI_PERIPHERALS_USED];
int SPI::_peripherals_used;

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
#endif
    _mosi(mosi),
    _miso(miso),
    _sclk(sclk),
    _hw_ssel(ssel),
    _sw_ssel(NC),
    _static_pinmap(NULL),
    _init_func(_do_init)
{
    // Need backwards compatibility with HALs not providing API
#ifdef DEVICE_SPI_COUNT
    _peripheral_name = spi_get_peripheral_name(_mosi, _miso, _sclk);
#else
    _peripheral_name = GlobalSPI;
#endif

    _do_construct();
}

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
#endif
    _mosi(mosi),
    _miso(miso),
    _sclk(sclk),
    _hw_ssel(NC),
    _sw_ssel(ssel, 1),
    _static_pinmap(NULL),
    _init_func(_do_init)
{
    // Need backwards compatibility with HALs not providing API
#ifdef DEVICE_SPI_COUNT
    _peripheral_name = spi_get_peripheral_name(_mosi, _miso, _sclk);
#else
    _peripheral_name = GlobalSPI;
#endif
    _do_construct();
}

SPI::SPI(const spi_pinmap_t &pinmap) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
#endif
    _mosi(pinmap.mosi_pin),
    _miso(pinmap.miso_pin),
    _sclk(pinmap.sclk_pin),
    _hw_ssel(pinmap.ssel_pin),
    _sw_ssel(NC),
    _static_pinmap(&pinmap),
    _peripheral_name((SPIName)pinmap.peripheral),
    _init_func(_do_init_direct)

{
    _do_construct();
}

SPI::SPI(const spi_pinmap_t &pinmap, PinName ssel) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
#endif
    _mosi(pinmap.mosi_pin),
    _miso(pinmap.miso_pin),
    _sclk(pinmap.sclk_pin),
    _hw_ssel(NC),
    _sw_ssel(ssel, 1),
    _static_pinmap(&pinmap),
    _peripheral_name((SPIName)pinmap.peripheral),
    _init_func(_do_init_direct)
{
    _do_construct();
}

void SPI::_do_init(SPI *obj)
{
    spi_init(&obj->_peripheral->spi, obj->_mosi, obj->_miso, obj->_sclk, obj->_hw_ssel);
}

void SPI::_do_init_direct(SPI *obj)
{
    spi_init_direct(&obj->_peripheral->spi, obj->_static_pinmap);
}

void SPI::_do_construct()
{
    // No lock needed in the constructor
#if DEVICE_SPI_ASYNCH
    _usage = DMA_USAGE_NEVER;
    _deep_sleep_locked = false;
#endif
    _select_count = 0;
    _bits = 8;
    _mode = 0;
    _hz = 1000000;
    _write_fill = SPI_FILL_CHAR;

    core_util_critical_section_enter();
    // lookup in a critical section if we already have it else initialize it

    _peripheral = SPI::_lookup(_peripheral_name);
    if (!_peripheral) {
        _peripheral = SPI::_alloc();
        _peripheral->name = _peripheral_name;
    }
    core_util_critical_section_exit();

#if DEVICE_SPI_ASYNCH && TRANSACTION_QUEUE_SIZE_SPI
    // prime the SingletonPtr, so we don't have a problem trying to
    // construct the buffer if asynch operation initiated from IRQ
    _peripheral->transaction_buffer.get();
#endif
    // we don't need to _acquire at this stage.
    // this will be done anyway before any operation.
}

SPI::~SPI()
{
    lock();
    /* Make sure a stale pointer isn't left in peripheral's owner field */
    if (_peripheral->owner == this) {
        _peripheral->owner = nullptr;
    }
    unlock();
}

SPI::spi_peripheral_s *SPI::_lookup(SPI::SPIName name)
{
    SPI::spi_peripheral_s *result = nullptr;
    core_util_critical_section_enter();
    for (int idx = 0; idx < _peripherals_used; idx++) {
        if (_peripherals[idx].name == name) {
            result = &_peripherals[idx];
            break;
        }
    }
    core_util_critical_section_exit();
    return result;
}

SPI::spi_peripheral_s *SPI::_alloc()
{
    MBED_ASSERT(_peripherals_used < SPI_PERIPHERALS_USED);
    return &_peripherals[_peripherals_used++];
}

void SPI::format(int bits, int mode)
{
    lock();
    _bits = bits;
    _mode = mode;
    // If changing format while you are the owner then just
    // update format, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        spi_format(&_peripheral->spi, _bits, _mode, 0);
    } else {
        _acquire();
    }
    unlock();
}

void SPI::frequency(int hz)
{
    lock();
    _hz = hz;
    // If changing format while you are the owner then just
    // update frequency, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        spi_frequency(&_peripheral->spi, _hz);
    } else {
        _acquire();
    }
    unlock();
}

// Note: Private function with no locking
void SPI::_acquire()
{
    if (_peripheral->owner != this) {
        _init_func(this);
        spi_format(&_peripheral->spi, _bits, _mode, 0);
        spi_frequency(&_peripheral->spi, _hz);
        _peripheral->owner = this;
    }
}

int SPI::write(int value)
{
    select();
    int ret = spi_master_write(&_peripheral->spi, value);
    deselect();
    return ret;
}

int SPI::write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length)
{
    select();
    int ret = spi_master_block_write(&_peripheral->spi, tx_buffer, tx_length, rx_buffer, rx_length, _write_fill);
    deselect();
    return ret;
}

void SPI::_set_ssel(int val)
{
    if (_sw_ssel.is_connected()) {
        _sw_ssel = val;
    }
}

void SPI::lock()
{
    _peripheral->mutex->lock();
}

void SPI::select()
{
    lock();
    if (_select_count++ == 0) {
        _acquire();
        _set_ssel(0);
    }
}

void SPI::unlock()
{
    _peripheral->mutex->unlock();
}

void SPI::deselect()
{
    if (--_select_count == 0) {
        _set_ssel(1);
    }
    unlock();
}

void SPI::set_default_write_value(char data)
{
    // this does not actually need to lock the peripheral.
    lock();
    _write_fill = data;
    unlock();
}

#if DEVICE_SPI_ASYNCH

int SPI::transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
    if (spi_active(&_peripheral->spi)) {
        return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    }
    start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    return 0;
}

void SPI::abort_transfer()
{
    spi_abort_asynch(&_peripheral->spi);
    unlock_deep_sleep();
#if TRANSACTION_QUEUE_SIZE_SPI
    dequeue_transaction();
#endif
}

void SPI::clear_transfer_buffer()
{
#if TRANSACTION_QUEUE_SIZE_SPI
    _peripheral->transaction_buffer->reset();
#endif
}

void SPI::abort_all_transfers()
{
    clear_transfer_buffer();
    abort_transfer();
}

int SPI::set_dma_usage(DMAUsage usage)
{
    if (spi_active(&_peripheral->spi)) {
        return -1;
    }
    _usage = usage;
    return  0;
}

int SPI::queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
#if TRANSACTION_QUEUE_SIZE_SPI
    transaction_t t;

    t.tx_buffer = const_cast<void *>(tx_buffer);
    t.tx_length = tx_length;
    t.rx_buffer = rx_buffer;
    t.rx_length = rx_length;
    t.event = event;
    t.callback = callback;
    t.width = bit_width;
    Transaction<SPI> transaction(this, t);
    if (_peripheral->transaction_buffer->full()) {
        return -1; // the buffer is full
    } else {
        core_util_critical_section_enter();
        _peripheral->transaction_buffer->push(transaction);
        if (!spi_active(&_peripheral->spi)) {
            dequeue_transaction();
        }
        core_util_critical_section_exit();
        return 0;
    }
#else
    return -1;
#endif
}

void SPI::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
    lock_deep_sleep();
    _acquire();
    _set_ssel(0);
    _callback = callback;
    _irq.callback(&SPI::irq_handler_asynch);
    spi_master_transfer(&_peripheral->spi, tx_buffer, tx_length, rx_buffer, rx_length, bit_width, _irq.entry(), event, _usage);
}

void SPI::lock_deep_sleep()
{
    if (_deep_sleep_locked == false) {
        sleep_manager_lock_deep_sleep();
        _deep_sleep_locked = true;
    }
}

void SPI::unlock_deep_sleep()
{
    if (_deep_sleep_locked == true) {
        sleep_manager_unlock_deep_sleep();
        _deep_sleep_locked = false;
    }
}

#if TRANSACTION_QUEUE_SIZE_SPI

void SPI::start_transaction(transaction_t *data)
{
    start_transfer(data->tx_buffer, data->tx_length, data->rx_buffer, data->rx_length, data->width, data->callback, data->event);
}

void SPI::dequeue_transaction()
{
    Transaction<SPI> t;
    if (_peripheral->transaction_buffer->pop(t)) {
        SPI *obj = t.get_object();
        transaction_t *data = t.get_transaction();
        obj->start_transaction(data);
    }
}

#endif

void SPI::irq_handler_asynch(void)
{
    int event = spi_irq_handler_asynch(&_peripheral->spi);
    if (_callback && (event & SPI_EVENT_ALL)) {
        _set_ssel(1);
        unlock_deep_sleep();
        _callback.call(event & SPI_EVENT_ALL);
    }
#if TRANSACTION_QUEUE_SIZE_SPI
    if (event & (SPI_EVENT_ALL | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        // SPI peripheral is free (event happened), dequeue transaction
        dequeue_transaction();
    }
#endif
}

#endif

} // namespace mbed

#endif
