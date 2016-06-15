/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
