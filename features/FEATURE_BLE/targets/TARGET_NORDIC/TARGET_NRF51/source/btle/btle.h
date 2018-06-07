/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef _BTLE_H_
#define _BTLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common/common.h"

#include "ble_srv_common.h"
#include "headers/nrf_ble.h"

/* number of central links used by the application. 
 * When changing this number remember to adjust the RAM settings */
#ifndef NRF_SDH_BLE_CENTRAL_LINK_COUNT
    #define CENTRAL_LINK_COUNT    3  
#else
    #define CENTRAL_LINK_COUNT    NRF_SDH_BLE_CENTRAL_LINK_COUNT
#endif

/* number of peripheral links used by the application. 
 * When changing this number remember to adjust the RAM settings */
#ifndef NRF_SDH_BLE_PERIPHERAL_LINK_COUNT
    #define PERIPHERAL_LINK_COUNT    1
#else
    #define PERIPHERAL_LINK_COUNT    NRF_SDH_BLE_PERIPHERAL_LINK_COUNT
#endif


/* GATTS attribite table size. 
 * When changing this number remember to adjust the RAM settings */
#ifndef NRF_SDH_BLE_GATTS_ATTR_TAB_SIZE
    #define GATTS_ATTR_TAB_SIZE    0x600
#else
    #define GATTS_ATTR_TAB_SIZE    NRF_SDH_BLE_GATTS_ATTR_TAB_SIZE
#endif


/**
 * Using this call, the application can select whether to include the
 * Service Changed characteristic in the GATT Server. The default in all
 * previous releases has been to include the Service Changed characteristic,
 * but this affects how GATT clients behave. Specifically, it requires
 * clients to subscribe to this attribute and not to cache attribute handles
 * between connections unless the devices are bonded. If the application
 * does not need to change the structure of the GATT server attributes at
 * runtime this adds unnecessary complexity to the interaction with peer
 * clients. If the SoftDevice is enabled with the Service Changed
 * Characteristics turned off, then clients are allowed to cache attribute
 * handles making applications simpler on both sides.
 */
#ifndef NRF_SDH_BLE_SERVICE_CHANGED
    #define IS_SRVC_CHANGED_CHARACT_PRESENT    1
#else
    #define IS_SRVC_CHANGED_CHARACT_PRESENT    NRF_SDH_BLE_SERVICE_CHANGED
#endif

error_t     btle_init(void);

// flag indicating if events have been signaled or not
// It is used by processEvents and signalEventsToProcess
// signalEventsToProcess raise the flag and processEvents
// clears it.
extern bool isEventsSignaled;

#ifdef __cplusplus
}
#endif

#endif // ifndef _BTLE_H_
