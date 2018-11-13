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
#include "drivers/SPI.h"
#include "platform/mbed_critical.h"

#if DEVICE_SPI_ASYNCH
#include "platform/mbed_power_mgmt.h"
#endif

#if DEVICE_SPI

namespace mbed {

SPI::spi_peripheral_s SPI::_peripherals[SPI_COUNT];

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
    _peripheral(NULL),
#if DEVICE_SPI_ASYNCH && 0
    _usage(DMA_USAGE_NEVER),
    _deep_sleep_locked(false),
#endif
    _bits(8),
    _mode(SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE),
    _bit_order(SPI_BIT_ORDERING_MSB_FIRST),
    _hz(1000000),
    _write_fill(SPI_FILL_CHAR)
{
    // No lock needed in the constructor
    SPIName name = spi_get_module(mosi, miso, sclk);

    core_util_critical_section_enter();
    // lookup in a critical section if we already have it else initialize it

    _peripheral = SPI::_lookup(name, true);
    if (_peripheral->name == 0) {
        _peripheral->name = name;
        _peripheral->miso = miso;
        _peripheral->mosi = mosi;
        _peripheral->sclk = sclk;
        _peripheral->ssel = ssel;

        MBED_ASSERT(ssel == NC);
        // TODO: ssel managment is not supported yet.
        spi_init(&_peripheral->spi, false, mosi, miso, sclk, NC);
    } else {
        MBED_ASSERT(_peripheral->miso == miso);
        MBED_ASSERT(_peripheral->mosi == mosi);
        MBED_ASSERT(_peripheral->sclk == sclk);
        MBED_ASSERT(_peripheral->ssel == ssel);
    }
    core_util_critical_section_exit();
    // we don't need to _acquire at this stage.
    // this will be done anyway before any operation.
}

SPI::~SPI()
{
    lock();
    if (_peripheral->owner == this) {
        _peripheral->owner = NULL;
    }
    unlock();
}

struct SPI::spi_peripheral_s *SPI::_lookup(SPIName name, bool or_last) {
    struct SPI::spi_peripheral_s *result = NULL;
    core_util_critical_section_enter();
    for (uint32_t idx = 0; idx < SPI_COUNT; idx++) {
        if ((_peripherals[idx].name == name) ||
            ((_peripherals[idx].name == 0) && or_last)) {
            result = &_peripherals[idx];
            break;
        }
    }
    core_util_critical_section_exit();
    return result;
}

void SPI::format(int bits, int mode)
{
    format(bits, (spi_mode_t)mode, SPI_BIT_ORDERING_MSB_FIRST);
}

void SPI::format(uint8_t bits, spi_mode_t mode, spi_bit_ordering_t bit_order)
{
    lock();
    _bits = bits;
    _mode = mode;
    _bit_order = bit_order;
    // If changing format while you are the owner then just
    // update format, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        spi_format(&_peripheral->spi, _bits, _mode, _bit_order);
    }
    unlock();
}

uint32_t SPI::frequency(uint32_t hz)
{
    uint32_t actual_hz;
    lock();
    _hz = hz;
    // If changing format while you are the owner then just
    // update frequency, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        actual_hz = spi_frequency(&_peripheral->spi, _hz);
    } else {
        actual_hz = _acquire();
    }
    unlock();
    return actual_hz;
}


void SPI::acquire()
{
    lock();
    _acquire();
    unlock();
}

// Note: Private function with no locking
uint32_t SPI::_acquire()
{
    uint32_t actual_hz = 0;
    if (_peripheral->owner != this) {
        spi_format(&_peripheral->spi, _bits, _mode, _bit_order);
        actual_hz = spi_frequency(&_peripheral->spi, _hz);
        _peripheral->owner = this;
    }
    return actual_hz;
}

int SPI::write(int value)
{
    lock();
    _acquire();
    uint32_t ret = 0;
    spi_transfer(&_peripheral->spi, &value, (_bits+7)/8, &ret, (_bits+7)/8, NULL);
    unlock();
    return ret;
}

int SPI::write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length)
{
    lock();
    _acquire();
    int ret = spi_transfer(&_peripheral->spi, tx_buffer, tx_length, rx_buffer, rx_length, &_write_fill);
    unlock();
    return ret;
}

bool SPI::lock()
{
    return _peripheral->mutex->lock() == 0; // see cmsis_os2.h osOk == 0
}

void SPI::unlock()
{
    _peripheral->mutex->unlock();
}

void SPI::set_default_write_value(char data)
{
    // this does not actually need to lock the peripheral.
    lock();
    _write_fill = (uint32_t)data;
    unlock();
}

#if DEVICE_SPI_ASYNCH

int SPI::transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
    if (!lock()) {
        return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    }
    start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    return 0;
}

void SPI::abort_transfer()
{
    spi_transfer_async_abort(&_peripheral->spi);
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
        core_util_critical_section_exit();
        if (lock()) {
            dequeue_transaction();
        }
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
    _callback = callback;
    
    spi_transfer_async(&_peripheral->spi, tx_buffer, tx_length, rx_buffer, rx_length, &_write_fill,
                       &SPI::irq_handler_asynch, this, _usage);
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
    } else {
        unlock();
    }
}

#endif

void SPI::irq_handler_asynch(spi_t *obj, void *vctx, spi_async_event_t *event)
{
    SPI *self = (SPI *)vctx;
    
    self->unlock_deep_sleep();
    self->_callback.call(SPI_EVENT_ALL);
#if TRANSACTION_QUEUE_SIZE_SPI
    // SPI peripheral is free (event happened), dequeue transaction
    self->dequeue_transaction();
#else
    self->unlock();
#endif
}

#endif // DEVICE_SPI_ASYNCH

} // namespace mbed

#endif // DEVICE_SPI
