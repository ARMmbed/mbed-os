/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#ifndef NRF_CORDIO_HCI_TRANSPORT_DRIVER_H_
#define NRF_CORDIO_HCI_TRANSPORT_DRIVER_H_

#include "ble/driver/CordioHCITransportDriver.h"

namespace ble {


/**
 * Nordic port of CordioHCITransportDriver
 */
class NRFCordioHCITransportDriver : public CordioHCITransportDriver {
public:
    /**
     * Driver destructor.
     */
    virtual ~NRFCordioHCITransportDriver();

    /**
     * Inialization of the transport.
     */
    virtual void initialize();

    /**
     * termination of the transport.
     */
    virtual void terminate();

    /**
     * Write data in the transport channel.
     *
     * @param type The type of packet to transmit. It might be an HCI command
     * packet, ACL packet or EVT packet. Depending on the type of transport
     * it can prefix the packet itself.
     * @param len Number of bytes to transmit.
     * @param pData Pointer to the data to transmit. This is an WSF buffer
     * and if CORDIO_ZERO_COPY_HCI is enabled we receive ownership.
     *
     * @return The number of bytes which have been transmited.
     */
    virtual uint16_t write(uint8_t type, uint16_t len, uint8_t *pData);
};

} // namespace ble

#endif /* NRF_CORDIO_HCI_TRANSPORT_DRIVER_H_ */
