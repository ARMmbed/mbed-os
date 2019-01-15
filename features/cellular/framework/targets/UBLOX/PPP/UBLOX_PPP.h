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

#ifndef UBLOX_PPP_H_
#define UBLOX_PPP_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_UBLOX_PPP_TX
#define MBED_CONF_UBLOX_PPP_TX D1
#endif
#ifndef MBED_CONF_UBLOX_PPP_RX
#define MBED_CONF_UBLOX_PPP_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

namespace mbed {

class UBLOX_PPP : public AT_CellularDevice {
public:
    UBLOX_PPP(FileHandle *fh);
};

} // namespace mbed

#endif // UBLOX_PPP_H_
