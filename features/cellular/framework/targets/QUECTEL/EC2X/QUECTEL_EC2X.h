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

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_QUECTEL_EC2X_TX
#define MBED_CONF_QUECTEL_EC2X_TX D1
#endif
#ifndef MBED_CONF_QUECTEL_EC2X_RX
#define MBED_CONF_QUECTEL_EC2X_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "DigitalOut.h"
#include "AT_CellularDevice.h"

namespace mbed {

class QUECTEL_EC2X : public AT_CellularDevice {
public:
    QUECTEL_EC2X(FileHandle *fh, PinName pwr = NC, PinName rst = NC);
    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();

private:
    DigitalOut _pwr_key;
    DigitalOut _rst;
};

} // namespace mbed

#endif // QUECTEL_EC2X_H
