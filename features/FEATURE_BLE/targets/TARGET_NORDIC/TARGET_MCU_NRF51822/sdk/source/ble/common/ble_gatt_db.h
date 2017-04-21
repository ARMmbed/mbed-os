/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef BLE_GATT_DB_H__
#define BLE_GATT_DB_H__

#include "stdint.h"
#include "nrf_ble.h"
#include "ble_gattc.h"

#define BLE_GATT_DB_MAX_CHARS                                 4          /**< The maximum number of characteristics present in a service record. */

/**@brief Structure for holding the characteristic and the handle of its CCCD present on a server.
 */
typedef struct
{
    ble_gattc_char_t characteristic;  /**< Structure containing information about the characteristic. */
    uint16_t         cccd_handle;     /**< CCCD Handle value for this characteristic. This will be set to BLE_GATT_HANDLE_INVALID if a CCCD is not present at the server. */
} ble_gatt_db_char_t;

/**@brief Structure for holding information about the service and the characteristics present on a
 *        server.
 */
typedef struct
{
    ble_uuid_t               srv_uuid;                                  /**< UUID of the service. */    
    uint8_t                  char_count;                                /**< Number of characteristics present in the service. */
    ble_gattc_handle_range_t handle_range;                              /**< Service Handle Range. */
    ble_gatt_db_char_t       charateristics[BLE_GATT_DB_MAX_CHARS];     /**< Array of information related to the characteristics present in the service. This list can extend further than one. */
} ble_gatt_db_srv_t;

#endif /* BLE_GATT_DB_H__ */
