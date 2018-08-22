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

#ifndef MBED_PN512_SPI_TRANSPORT_DRIVER_H
#define MBED_PN512_SPI_TRANSPORT_DRIVER_H

#include <stdint.h>

#include "platform/platform.h"
#include "platform/nfc_transport.h"
#include "PN512TransportDriver.h"

#include "drivers/SPI.h"
#include "drivers/DigitalOut.h"
#include "drivers/InterruptIn.h"

namespace mbed {
namespace nfc {

class PN512SPITransportDriver : public PN512TransportDriver {
public:
    PN512SPITransportDriver(PinName mosi, PinName miso, PinName sclk, PinName ssel, PinName irq, PinName rst);

private:
    virtual void initialize();
    virtual nfc_transport_t *get_transport();

    void transport_write(uint8_t address, const uint8_t *outBuf, size_t outLen);
    void transport_read(uint8_t address, uint8_t *inBuf, size_t inLen);

    // Callbacks from munfc
    static void s_transport_write(uint8_t address, const uint8_t *outBuf, size_t outLen, void *pUser);
    static void s_transport_read(uint8_t address, uint8_t *inBuf, size_t inLen, void *pUser);

    nfc_transport_t _nfc_transport;
    mbed::SPI _spi;
    mbed::DigitalOut _ssel;
    mbed::InterruptIn _irq;
    mbed::DigitalOut _rst;
};

} // namespace nfc
} // namespace mbed

#endif
