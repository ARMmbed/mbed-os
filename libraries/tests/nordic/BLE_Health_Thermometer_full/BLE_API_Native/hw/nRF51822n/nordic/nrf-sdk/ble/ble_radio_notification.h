/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

/** @file
 *
 * @defgroup ble_radio_notification Radio Notification Event Handler
 * @{
 * @ingroup ble_sdk_lib
 * @brief Module for propagating Radio Notification events to the application.
 */

#ifndef BLE_RADIO_NOTIFICATION_H__
#define BLE_RADIO_NOTIFICATION_H__

#include <stdint.h>
#include <stdbool.h>
#include "nordic_global.h"
#include "nrf_soc.h"

/**@brief Application radio notification event handler type. */
typedef void (*ble_radio_notification_evt_handler_t) (bool radio_active);

/**@brief Function for initializing the Radio Notification module.
 *
 * @param[in]  irq_priority   Interrupt priority for the Radio Notification interrupt handler.
 * @param[in]  distance       The time from an Active event until the radio is activated.
 * @param[in]  evt_handler    Handler to be executed when a radio notification event has been
 *                            received.
 *
 * @return     NRF_SUCCESS on successful initialization, otherwise an error code.
 */
uint32_t ble_radio_notification_init(nrf_app_irq_priority_t               irq_priority,
                                     nrf_radio_notification_distance_t    distance,
                                     ble_radio_notification_evt_handler_t evt_handler);

#endif // BLE_RADIO_NOTIFICATION_H__

/** @} */
