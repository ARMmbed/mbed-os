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

#include "headers/nrf_ble.h"

#if NRF_SD_BLE_API_VERSION >= 5
#include "sdk_config.h"
#endif

#define NRF_CONNECTION_TAG  1 /**<Connection tag to use with softdevice */

#define CENTRAL_LINK_COUNT      NRF_SDH_BLE_CENTRAL_LINK_COUNT  /**<number of central links used by the application. When changing this number remember to adjust the RAM settings */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_PERIPHERAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define PERIPHERAL_LINK_COUNT   NRF_SDH_BLE_PERIPHERAL_LINK_COUNT    /**<number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_CENTRAL_LINKS was used, ram settings are adjusted by the yotta target module. */
#define TOTAL_LINK_COUNT        NRF_SDH_BLE_TOTAL_LINK_COUNT          /**<number of total links used by the application. When changing this number remember to adjust the RAM settings */                                                             
#define GATTS_ATTR_TAB_SIZE     NRF_SDH_BLE_GATTS_ATTR_TAB_SIZE /**< GATTS attribite table size. */
                                                                       /** If value for YOTTA_CFG_NORDIC_BLE_GATTS_ATTR_TAB_SIZE was used, ram settings are adjusted by the yotta target module. */

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
#define IS_SRVC_CHANGED_CHARACT_PRESENT NRF_SDH_BLE_SERVICE_CHANGED

#define UUID_TABLE_MAX_ENTRIES  NRF_SDH_BLE_VS_UUID_COUNT /* This is the maximum number of 128-bit UUIDs with distinct bases that
                                                   * we expect to be in use; increase this limit if needed. */

#define NRF_SDK14PLUS_EVENT_HANDLERS (NRF_SD_BLE_API_VERSION >= 5) // Softdevice event dispatching has changed in SDK14

error_t     btle_init(void);

// flag indicating if events have been signaled or not
// It is used by processEvents and signalEventsToProcess
// signalEventsToProcess raise the flag and processEvents
// clears it.
extern volatile bool isEventsSignaled;

#ifdef __cplusplus
}
#endif

#endif // ifndef _BTLE_H_
