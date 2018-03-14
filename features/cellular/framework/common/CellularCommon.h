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

#ifndef CELLULAR_COMMON_
#define CELLULAR_COMMON_

#include <stdint.h>

/**
 * Cellular specific event changes.
 * Connect and disconnect are handled via NSAPI_EVENT_CONNECTION_STATUS_CHANGE
 */
typedef enum cellular_event_status {
    CellularDeviceReady                     = 0,    /*!< Modem is powered and ready to receive commands */
    CellularSIMStatusChanged                = 1,    /*!< SIM state changed, call SIM state  */
    CellularRegistrationStatusChanged       = 2,    /*!< Registering status changed, e.g. roaming, registered, smsonly...  */
    CellularRegistrationTypeChanged         = 3,    /*!< Registration type changed, e.g. C_EREG, C_GREG, C_REG */
    CellularCellIDChanged                   = 4,    /*!< Network Cell ID have changed  */
    CellularRadioAccessTechnologyChanged    = 5,    /*!< Network roaming status have changed  */
} cellular_connection_status_t;

#endif // CELLULAR_COMMON_
