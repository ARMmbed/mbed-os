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
#include "SPI.h"

#if DEVICE_SPI

namespace mbed {

#if DEVICE_SPI_ASYNCH && TRANSACTION_QUEUE_SIZE_SPI
CircularBuffer<Transaction<SPI>, TRANSACTION_QUEUE_SIZE_SPI> SPI::_transaction_buffer;
#endif

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
        _spi(),
#if DEVICE_SPI_ASYNCH
        _irq(this),
        _usage(DMA_USAGE_NEVER),
#endif
        _bits(8),
        _mode(0),
        _hz(1000000) {
    spi_init(&_spi, mosi, miso, sclk, ssel);
    spi_format(&_spi, _bits, _mode, 0);
    spi_frequency(&_spi, _hz);
}

void SPI::format(int bits, int mode) {
    _bits = bits;
    _mode = mode;
    SPI::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

void SPI::frequency(int hz) {
    _hz = hz;
    SPI::_owner = NULL; // Not that elegant, but works. rmeyer
    aquire();
}

SPI* SPI::_owner = NULL;

// ignore the fact there are multiple physical spis, and always update if it wasnt us last
void SPI::aquire() {
     if (_owner != this) {
        spi_format(&_spi, _bits, _mode, 0);
        spi_frequency(&_spi, _hz);
        _owner = this;
    }
}

int SPI::write(int value) {
    aquire();
    return spi_master_write(&_spi, value);
}

#if DEVICE_SPI_ASYNCH

int SPI::transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event)
{
    if (spi_active(&_spi)) {
        return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    }
    start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, bit_width, callback, event);
    return 0;
}

void SPI::abort_transfer()
{
    spi_abort_asynch(&_spi);
#if TRANSACTION_QUEUE_SIZE_SPI
    dequeue_transaction();
#endif
}


void SPI::clear_transfer_buffer()
{
#if TRANSACTION_QUEUE_SIZE_SPI
    _transaction_buffer.reset();
#endif
}

void SPI::abort_all_transfers()
{
    clear_transfer_buffer();
    abort_transfer();
}

int SPI::set_dma_usage(DMAUsage usage)
{
    if (spi_active(&_spi)) {
        return -1;
    }
    _usage = usage;
    return  0;
}

int SPI::queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event)
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
    if (_transaction_buffer.full()) {
        return -1; // the buffer is full
    } else {
        _transaction_buffer.push(transaction);
        return 0;
    }
#else
    return -1;
#endif
}

void SPI::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t& callback, int event)
{
    aquire();
    _callback = callback;
    _irq.callback(&SPI::irq_handler_asynch);
    spi_master_transfer(&_spi, tx_buffer, tx_length, rx_buffer, rx_length, bit_width, _irq.entry(), event , _usage);
}

#if TRANSACTION_QUEUE_SIZE_SPI

void SPI::start_transaction(transaction_t *data)
{
    start_transfer(data->tx_buffer, data->tx_length, data->rx_buffer, data->rx_length, data->width, data->callback, data->event);
}

void SPI::dequeue_transaction()
{
    Transaction<SPI> t;
    if (_transaction_buffer.pop(t)) {
        SPI* obj = t.get_object();
        transaction_t* data = t.get_transaction();
        obj->start_transaction(data);
    }
}

#endif

void SPI::irq_handler_asynch(void)
{
    int event = spi_irq_handler_asynch(&_spi);
    if (_callback && (event & SPI_EVENT_ALL)) {
        _callback.call(event & SPI_EVENT_ALL);
    }
#if TRANSACTION_QUEUE_SIZE_SPI
    if (event & (SPI_EVENT_ALL | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        // SPI peripheral is free (event happend), dequeue transaction
        dequeue_transaction();
    }
#endif
}

#endif

} // namespace mbed

#endif
