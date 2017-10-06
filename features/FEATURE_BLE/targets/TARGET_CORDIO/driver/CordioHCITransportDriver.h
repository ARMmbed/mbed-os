/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef CORDIO_HCI_TRANSPORT_DRIVER_H_
#define CORDIO_HCI_TRANSPORT_DRIVER_H_

#include <stdint.h>

namespace ble {
namespace vendor {
namespace cordio {

/**
 * Base class of the HCI transport driver.
 * It allow the stack to write data in the HCI channel.
 */
class CordioHCITransportDriver {
public:
    /**
     * Driver destructor.
     */
    virtual ~CordioHCITransportDriver() { }

    /**
     * Inialization of the transport.
     */
    virtual void initialize() = 0;

    /**
     * termination of the transport.
     */
    virtual void terminate() = 0;

    /**
     * Write data in the transport channel.
     *
     * @param type The type of packet to transmit. It might be an HCI command
     * packet, ACL packet or EVT packet. Depending on the type of transport
     * it can prefix the packet itself.
     * @param len Number of bytes to transmit.
     * @param pData pointer to the data to transmit.
     *
     * @return The number of bytes which have been transmited.
     */
    virtual uint16_t write(uint8_t type, uint16_t len, uint8_t *pData) = 0;

    /**
     * The driver shall call this function whenever data bytes are received.
     *
     * @param data Pointer to the data received.
     * @param len Number of bytes received.
     */
    static void on_data_received(uint8_t* data, uint16_t len);
};

} // namespace cordio
} // namespace vendor
} // namespace ble

#endif /* CORDIO_HCI_TRANSPORT_DRIVER_H_ */
