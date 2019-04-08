/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef QUECTEL_EC2X_H
#define QUECTEL_EC2X_H

#include "DigitalOut.h"
#include "AT_CellularDevice.h"

namespace mbed {

class QUECTEL_EC2X : public AT_CellularDevice {
public:

    /**
     * Constructs the Quectel EC2X series driver. It is mandatory to provide
     * a FileHandle object, the power pin and the polarity of the pin.
     * Providing reset pin is optional.
     */
    QUECTEL_EC2X(FileHandle *fh, PinName pwr, bool active_high, PinName rst = NC);

    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();

private:
    nsapi_error_t press_power_button(uint32_t timeout);
    bool _active_high;
    DigitalOut _pwr_key;
    DigitalOut _rst;
};

} // namespace mbed

#endif // QUECTEL_EC2X_H
