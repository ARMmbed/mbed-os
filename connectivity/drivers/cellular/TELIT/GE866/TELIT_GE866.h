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

#ifndef CELLULAR_TARGETS_TELIT_GE866_TELIT_GE866_H_
#define CELLULAR_TARGETS_TELIT_GE866_TELIT_GE866_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_TELIT_GE866_TX
#define MBED_CONF_TELIT_GE866_TX D1
#endif
#ifndef MBED_CONF_TELIT_GE866_RX
#define MBED_CONF_TELIT_GE866_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "DigitalOut.h"
#include "AT_CellularDevice.h"

namespace mbed {

class TELIT_GE866 : public AT_CellularDevice {
public:
    /**
     * Constructs the Telit GE866 series driver. It is mandatory to provide
     * a FileHandle object, the reset pin and the polarity of the pin.
     */
    TELIT_GE866(FileHandle *fh, PinName reset, bool active_high);

protected: // AT_CellularDevice
    virtual nsapi_error_t init();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t shutdown();
    virtual nsapi_error_t set_power_save_mode(int periodic_time, int active_time = 0);

private:
    bool _active_high;
    DigitalOut _reset_key;
};
} // namespace mbed
#endif /* CELLULAR_TARGETS_TELIT_GE866_TELIT_GE866_H_ */
