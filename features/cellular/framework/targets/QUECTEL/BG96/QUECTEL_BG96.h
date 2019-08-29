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

#ifndef QUECTEL_BG96_H_
#define QUECTEL_BG96_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_QUECTEL_BG96_TX
#define MBED_CONF_QUECTEL_BG96_TX D1
#endif
#ifndef MBED_CONF_QUECTEL_BG96_RX
#define MBED_CONF_QUECTEL_BG96_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "DigitalOut.h"
#include "AT_CellularDevice.h"

namespace mbed {

class QUECTEL_BG96 : public AT_CellularDevice {
public:
    QUECTEL_BG96(FileHandle *fh, PinName pwr = NC, bool active_high = true, PinName rst = NC);

protected: // AT_CellularDevice
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual AT_CellularInformation *open_information_impl(ATHandler &at);
    virtual void set_ready_cb(Callback<void()> callback);
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();
    virtual void set_at_urcs_impl();

public:
    void handle_urc(FileHandle *fh);

private:
    void press_button(DigitalOut &button, uint32_t timeout);
    bool wake_up(bool reset = false);
    bool _active_high;
    DigitalOut _pwr;
    DigitalOut _rst;
    void urc_pdpdeact();
};
} // namespace mbed
#endif // QUECTEL_BG96_H_
