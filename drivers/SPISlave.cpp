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
#include "drivers/SPISlave.h"

#if DEVICE_SPISLAVE

namespace mbed {

SPISlave::SPISlave(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
    _spi(),
    _bits(8),
    _mode(SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE),
    _hz(1000000),
    _buffer(0),
    _dummy(0xFFFFFFFF),
    _is_pending(false),
    _has_received(false)
{
    spi_init(&_spi, true, mosi, miso, sclk, ssel);
    spi_format(&_spi, _bits, _mode, SPI_BIT_ORDERING_MSB_FIRST);
    spi_frequency(&_spi, _hz);
}

void SPISlave::format(int bits, int mode)
{
    _bits = bits;
    _mode = (spi_mode_t)mode;
    spi_format(&_spi, _bits, _mode, SPI_BIT_ORDERING_MSB_FIRST);
}

void SPISlave::frequency(int hz)
{
    _hz = hz;
    spi_frequency(&_spi, _hz);
}

void SPISlave::irq_handler(spi_t *obj, void *ctx, spi_async_event_t *event) {
    SPISlave *spi = (SPISlave *)ctx;
    spi->_is_pending = false;
    spi->_has_received = !(event->error);
}

int SPISlave::receive(void)
{
    if (!_is_pending) {
        _is_pending = spi_transfer_async(&_spi, &_dummy, 1, (void *)&_buffer, 1, &_dummy, &SPISlave::irq_handler, this, (DMAUsage)0);
    }
    return ((_has_received)? 1 : 0);
}

int SPISlave::read(void)
{
    while (receive() == 0) ;
    return (int)_buffer;
}

void SPISlave::reply(int value)
{
    if (_is_pending) {
        spi_transfer_async_abort(&_spi);
        _is_pending = false;
    }
    _is_pending = spi_transfer_async(&_spi, &value, 1, (void *)&_buffer, 1, &_dummy, &SPISlave::irq_handler, this, (DMAUsage)0);
    while (_is_pending) ;
}

} // namespace mbed

#endif
