/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#ifndef CELLULAR_API_CELLULARPOWER_H_
#define CELLULAR_API_CELLULARPOWER_H_

#include "nsapi_types.h"
#include "Callback.h"

namespace mbed {

/**
 *  Class CellularPower
 *
 *  An interface that provides power handling functions for modem/module.
 */
class CellularPower {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /**
     * virtual Destructor
     */
    virtual ~CellularPower() {}

public:
    /** Set cellular device power on. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power on is done by toggling power pin/button.
     *
     *  @remark init must be called to initialize cellular device
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if not overridden by the target modem
     */
    virtual nsapi_error_t on() = 0;

    /** Set cellular device power off. Default implementation is empty.
     *  Device power on/off is modem/board specific behavior and must be done on inherited class if needed.
     *  Power off is done by toggling power pin/button.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if not overridden by the target modem
     */
    virtual nsapi_error_t off() = 0;

    /** Reset and wake-up cellular device.
     *
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t reset() = 0;
};

} // namespace mbed

#endif /* CELLULAR_API_CELLULARPOWER_H_ */
