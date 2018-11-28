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
 * @defgroup ble_radio_notification Radio Notification Event Handler
 * @{
 * @ingroup ble_sdk_lib
 * @brief Module for propagating Radio Notification events to the application.
 */

#ifndef BLE_RADIO_NOTIFICATION_H__
#define BLE_RADIO_NOTIFICATION_H__

#include <stdint.h>
#include <stdbool.h>
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
