/*
 * Copyright (c) 2020, Arm Limited and affiliates.
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

#ifndef CELLULAR_TARGETS_TELIT_ME310_TELIT_ME310_H_
#define CELLULAR_TARGETS_TELIT_ME310_TELIT_ME310_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_TELIT_ME310_TX
#define MBED_CONF_TELIT_ME310_TX D1
#endif
#ifndef MBED_CONF_TELIT_ME310_RX
#define MBED_CONF_TELIT_ME310_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "DigitalOut.h"
#include "AT_CellularDevice.h"

namespace mbed {

class TELIT_ME310 : public AT_CellularDevice {
public:
    /**
     * Constructs the Telit ME310 series driver. It is mandatory to provide
     * a FileHandle object, the power pin and the polarity of the pin.
     */
    TELIT_ME310(FileHandle *fh, PinName pwr, bool active_high);

protected: // AT_CellularDevice
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual nsapi_error_t init();
    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);

private:
    bool _active_high;
    DigitalOut _pwr_key;
};
} // namespace mbed
#endif /* CELLULAR_TARGETS_TELIT_ME310_TELIT_ME310_H_ */
