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

#ifndef QUECTEL_UG96_H_
#define QUECTEL_UG96_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_QUECTEL_UG96_TX
#define MBED_CONF_QUECTEL_UG96_TX D1
#endif
#ifndef MBED_CONF_QUECTEL_UG96_RX
#define MBED_CONF_QUECTEL_UG96_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

namespace mbed {

#ifdef TARGET_WIO_3G
#define CELLULAR_SERIAL_TX PA_2
#define CELLULAR_SERIAL_RX PA_3
#else
#define CELLULAR_SERIAL_TX PC_1
#define CELLULAR_SERIAL_RX PC_0
#endif

class QUECTEL_UG96 : public AT_CellularDevice {
public:
    QUECTEL_UG96(FileHandle *fh);

protected: // AT_CellularDevice
    virtual AT_CellularContext *create_context_impl(ATHandler &at, const char *apn, bool cp_req = false, bool nonip_req = false);

public: // NetworkInterface
    void handle_urc(FileHandle *fh);
};

} // namespace mbed
#endif // QUECTEL_UG96_H_
