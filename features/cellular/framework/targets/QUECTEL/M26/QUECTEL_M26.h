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

#ifndef QUECTEL_M26_H_
#define QUECTEL_M26_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_QUECTEL_M26_TX
#define MBED_CONF_QUECTEL_M26_TX D1
#endif
#ifndef MBED_CONF_QUECTEL_M26_RX
#define MBED_CONF_QUECTEL_M26_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

namespace mbed {

class QUECTEL_M26 : public AT_CellularDevice {
public:
    QUECTEL_M26(FileHandle *fh);

protected: // AT_CellularDevice
    virtual nsapi_error_t get_sim_state(SimState &state);
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);
    virtual nsapi_error_t shutdown();

public: // NetworkInterface
    void handle_urc(FileHandle *fh);
};
} // namespace mbed

#endif // QUECTEL_M26_H_
