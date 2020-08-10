/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "platform/platform.h"

#if (defined (DEVICE_SPI) && defined (DEVICE_INTERRUPTIN)) || defined(DOXYGEN_ONLY)

#include "PN512SPITransportDriver.h"

#include "stack/transceiver/transceiver.h"
#include "platform/mbed_wait_api.h"

using namespace mbed;
using namespace mbed::nfc;

PN512SPITransportDriver::PN512SPITransportDriver(PinName mosi, PinName miso, PinName sclk, PinName ssel, PinName irq, PinName rst) :
    _spi(mosi, miso, sclk),
    _ssel(ssel, 1),
    _irq(irq, PullNone),
    _rst(rst, 1)
{

    // Use SPI mode 0
    _spi.format(8, 0);

    // The PN512 supports SPI clock frequencies up to 10MHz, so use this if we can
    _spi.frequency(10000000UL);

    // Initialize NFC transport
    nfc_transport_init(&_nfc_transport, &PN512SPITransportDriver::s_transport_write, &PN512SPITransportDriver::s_transport_read, this);
}

void PN512SPITransportDriver::initialize()
{
    // Deactivate IRQ
    _irq.rise(callback<void>());

    // Assert reset pin
    // According to the datasheet, it needs to be asserted for at least 100ns
    // Wait for 1us as that's the shortest time we can wait for
    _rst = 0;
    wait_us(1);
    _rst = 1;

    // Setup IRQ pin
    _irq.rise(callback(this, &PN512SPITransportDriver::hw_interrupt));
}

nfc_transport_t *PN512SPITransportDriver::get_transport()
{
    return &_nfc_transport;
}

void PN512SPITransportDriver::transport_write(uint8_t address, const uint8_t *outBuf, size_t outLen)
{
    if (outLen == 0) {
        return;
    }

    // First byte is (address << 1) | 0x00 for a write
    address = (address << 1) | 0x00;
    _ssel = 0;
    _spi.write(address); // First write address byte
    _spi.write((const char *) outBuf, outLen, (char *) NULL, 0); // Ignore read bytes
    _ssel = 1;
}

void PN512SPITransportDriver::transport_read(uint8_t address, uint8_t *inBuf, size_t inLen)
{
    if (inLen == 0) {
        return;
    }

    // Address byte is (address << 1) | 0x80 for a read
    // This should be repeated accross the transfer, except for the last byte which should be 0
    address = (address << 1) | 0x80;

    // Set this byte across inBuf so that it's repeated accross the transfer
    // Bit cheeky, but will work
    memset(inBuf, address, inLen - 1);

    // Also terminate with 0 so that it's a no-op
    inBuf[inLen - 1] = 0;

    _ssel = 0;
    _spi.write(address); // First write address byte
    _spi.write((const char *) inBuf, inLen, (char *) inBuf, inLen);
    _ssel = 1;
}

// Callbacks from munfc
void PN512SPITransportDriver::s_transport_write(uint8_t address, const uint8_t *outBuf, size_t outLen, void *pUser)
{
    PN512SPITransportDriver *self = (PN512SPITransportDriver *)pUser;
    self->transport_write(address, outBuf, outLen);
}

void PN512SPITransportDriver::s_transport_read(uint8_t address, uint8_t *inBuf, size_t inLen, void *pUser)
{
    PN512SPITransportDriver *self = (PN512SPITransportDriver *)pUser;
    self->transport_read(address, inBuf, inLen);
}

#endif
