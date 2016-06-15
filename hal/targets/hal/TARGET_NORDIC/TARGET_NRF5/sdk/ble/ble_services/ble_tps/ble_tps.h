/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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
 * @defgroup ble_sdk_srv_tps TX Power Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief TX Power Service module.
 *
 * @details This module implements the TX Power Service with the TX Power Level characteristic.
 *          During initialization it adds the TX Power Service and TX Power Level characteristic
 *          with the specified initial value to the BLE stack database.
 *
 *          It provides a function for letting the application update the TX Power Level
 *          characteristic.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, this section of source code must not be modified.
 */

#ifndef BLE_TPS_H__
#define BLE_TPS_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

/**@brief TX Power Service init structure. This contains all options and data needed for
 *        initialization of the service. */
typedef struct
{
    int8_t                    initial_tx_power_level;   /**< Initial value of the TX Power Level characteristic (in dBm). */
    ble_srv_security_mode_t   tps_attr_md;              /**< Initial Security Setting for TX Power Service Characteristics. */
} ble_tps_init_t;

/**@brief TX Power Service structure. This contains various status information for the service. */
typedef struct
{
    uint16_t                  service_handle;           /**< Handle of TX Power Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  tx_power_level_handles;   /**< Handles related to the TX Power Level characteristic. */
    uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
} ble_tps_t;

/**@brief Function for initializing the TX Power Service.
 *
 * @param[out]  p_hrs       TX Power Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_tps_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_tps_init(ble_tps_t * p_hrs, const ble_tps_init_t * p_tps_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the TX Power Service.
 *
 * @param[in]   p_tps      TX Power Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_tps_on_ble_evt(ble_tps_t * p_tps, ble_evt_t * p_ble_evt);

/**@brief Function for setting the state of the Sensor Contact Detected bit.
 *
 * @param[in]   p_tps            TX Power Service structure.
 * @param[in]   tx_power_level   New TX Power Level (unit dBm, range -100 to 20).
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_tps_tx_power_level_set(ble_tps_t * p_tps, int8_t tx_power_level);

#endif // BLE_TPS_H__

/** @} */
