/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef UBLOX_AT_H_
#define UBLOX_AT_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_UBLOX_AT_TX
#define MBED_CONF_UBLOX_AT_TX D1
#endif
#ifndef MBED_CONF_UBLOX_AT_RX
#define MBED_CONF_UBLOX_AT_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "APN_db.h"
#include "AT_CellularDevice.h"
#include "AT_CellularContext.h"
#include "UBLOX_AT_CellularNetwork.h"
#include "UBLOX_AT_CellularContext.h"

namespace mbed {

class UBLOX_AT : public AT_CellularDevice {
public:
    UBLOX_AT(FileHandle *fh);

protected: // AT_CellularDevice
    virtual AT_CellularNetwork *open_network_impl(ATHandler &at);
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
public: // NetworkInterface
    void handle_urc(FileHandle *fh);

    virtual nsapi_error_t init();

private:

    UBLOX_AT_CellularContext *ubx_context;

    /** Length of IMSI buffer.
     */
    static const int MAX_IMSI_LENGTH = 15;

    nsapi_error_t config_authentication_parameters();

    nsapi_error_t set_authentication_parameters(const char *apn, const char *username, const char *password, CellularContext::AuthenticationType auth);

    /** Read IMSI of modem.
     */
    nsapi_error_t get_imsi(char *imsi);
};

} // namespace mbed

#endif // UBLOX_AT_H_
