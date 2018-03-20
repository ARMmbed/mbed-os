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
#include "nsapi_types.h"

/**
 * Cellular specific event changes.
 * Connect and disconnect are handled via NSAPI_EVENT_CONNECTION_STATUS_CHANGE
 */
typedef enum cellular_event_status {
    CellularDeviceReady                     = NSAPI_EVENT_CELLULAR_STATUS_BASE,     /* Modem is powered and ready to receive commands. No additional info in callback intptr_t. */
    CellularSIMStatusChanged                = NSAPI_EVENT_CELLULAR_STATUS_BASE + 1, /* SIM state changed, call SIM state. enum SimState as additional info callback intptr_t. See enum SimState in ../API/CellularSIM.h  */
    CellularRegistrationStatusChanged       = NSAPI_EVENT_CELLULAR_STATUS_BASE + 2, /* Registering status changed. enum RegistrationStatus as additional info callback intptr_t. See enum RegistrationStatus in ../API/CellularNetwork.h  */
    CellularRegistrationTypeChanged         = NSAPI_EVENT_CELLULAR_STATUS_BASE + 3, /* Registration type changed. enum RegistrationType as additional info callback intptr_t. See enum RegistrationType in ../API/CellularNetwork.h  */
    CellularCellIDChanged                   = NSAPI_EVENT_CELLULAR_STATUS_BASE + 4, /* Network Cell ID have changed. int cellid as additional info callback intptr_t. */
    CellularRadioAccessTechnologyChanged    = NSAPI_EVENT_CELLULAR_STATUS_BASE + 5, /* Network roaming status have changed. enum RadioAccessTechnology as additional info callback intptr_t. See enum RadioAccessTechnology in ../API/CellularNetwork.h  */
} cellular_connection_status_t;

#endif // CELLULAR_COMMON_
