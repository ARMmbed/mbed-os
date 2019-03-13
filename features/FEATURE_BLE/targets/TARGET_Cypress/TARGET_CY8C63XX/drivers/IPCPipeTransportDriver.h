/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef PSOC6_IPCPIPE_TRANSPORT_DRIVER_H_
#define PSOC6_IPCPIPE_TRANSPORT_DRIVER_H_

#include <stdint.h>
#include "mbed.h"
#include "CordioHCITransportDriver.h"


namespace ble {
namespace vendor {
namespace cypress {

using namespace ble::vendor;

/**
 * Implementation of the H4 driver over PSoC6 IPC pipe.
 */
class IPCPipeTransportDriver : public cordio::CordioHCITransportDriver {
public:
    /**
     * Initialize the transport driver.
     *
     */
    IPCPipeTransportDriver();

    /**
     * Destructor
     */
    virtual ~IPCPipeTransportDriver() { }

    /**
     * @see CordioHCITransportDriver::initialize
     */
    virtual void initialize();

    /**
     * @see CordioHCITransportDriver::terminate
     */
    virtual void terminate();

    /**
     * @see CordioHCITransportDriver::write
     */
    virtual uint16_t write(uint8_t type, uint16_t len, uint8_t *pData);

private:

};

} // namespace cypress
} // namespace vendor
} // namespace ble

#endif /* PSOC6_IPCPIPE_TRANSPORT_DRIVER_H_ */
