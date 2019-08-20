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

#ifndef UBLOX_N2XX_H_
#define UBLOX_N2XX_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_UBLOX_N2XX_TX
#define MBED_CONF_UBLOX_N2XX_TX D1
#endif
#ifndef MBED_CONF_UBLOX_N2XX_RX
#define MBED_CONF_UBLOX_N2XX_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "mbed.h"
#include "CellularLog.h"
#include "AT_CellularDevice.h"
#include "AT_CellularNetwork.h"
#include "UBLOX_N2XX_CellularSMS.h"
#include "UBLOX_N2XX_CellularContext.h"

namespace mbed {

class UBLOX_N2XX : public AT_CellularDevice {

public:

    UBLOX_N2XX(FileHandle *fh);
    virtual ~UBLOX_N2XX();

    virtual nsapi_error_t init();
    virtual nsapi_error_t get_sim_state(SimState &state);
    virtual nsapi_error_t set_pin(const char *sim_pin);


protected: // AT_CellularDevice

    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual AT_CellularSMS *open_sms_impl(ATHandler &at);
    virtual void set_at_urcs_impl();

public: // NetworkInterface

    void handle_urc(FileHandle *fh);

private:

    static const int MAX_SIM_RESPONSE_LENGTH = 25;

    // URC handlers
    void NPIN_URC();

    char simstr[MAX_SIM_RESPONSE_LENGTH];
};

} // namespace mbed

#endif // UBLOX_N2XX_H_
