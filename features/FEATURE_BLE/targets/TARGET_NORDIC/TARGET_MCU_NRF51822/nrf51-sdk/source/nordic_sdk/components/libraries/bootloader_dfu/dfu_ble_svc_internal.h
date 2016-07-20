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
  
/** @file
 *
 * @defgroup nrf_dfu_ble_svc_internal DFU BLE SVC internal
 * @{
 *
 * @brief DFU BLE SVC internal functions in bootloader. The DFU BLE SuperVisor Calls allow an 
 *        application to execute functions in the installed bootloader. This interface provides 
 *        internal Bootloader DFU functions for retrieving data exchanged through SuperVisor Calls.
 *
 */

#ifndef DFU_BLE_SVC_INTERNAL_H__
#define DFU_BLE_SVC_INTERNAL_H__

#include <stdint.h>
#include "dfu_ble_svc.h"
#include "ble_gap.h"

/**@brief Internal bootloader/DFU function for retrieving peer data provided from application.
 *
 * @param[out] p_peer_data Peer data set by application to be used for DFU connection.
 *
 * @retval NRF_SUCCESS            If peer data is valid and can be used for connection.
 * @retval NRF_ERROR_NULL         If p_peer_data is a NULL pointer.
 * @retval NRF_ERROR_INVALID_DATA If peer data is not available or invalid.
 */
uint32_t dfu_ble_peer_data_get(dfu_ble_peer_data_t * p_peer_data);

#endif // DFU_BLE_SVC_INTERNAL_H__

/** @} */
