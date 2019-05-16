/* mbed Microcontroller Library
 * A generic CMSIS include header
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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

#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include "cy_device_headers.h"

/* #define BLE provided by Cypress PDL conflicts with mbed BLE class
 * defined in features/FEATURE_BLE/ble/BLE.h */
#undef BLE

/* #define PROFILE provided by Cypress PDL conflicts with u-blox PROFILE macro
 * defined in ./features/cellular/framework/targets/UBLOX/AT/UBLOX_AT_CellularStack.h */
#undef PROFILE

#endif
