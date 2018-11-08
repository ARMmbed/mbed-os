/*
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
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

/**@file
 *
 * @defgroup ble_stack_handler_types Types definitions for BLE support in SoftDevice handler.
 * @{
 * @ingroup  softdevice_handler
 * @brief    This file contains the declarations of types required for BLE stack support. These
 *           types will be defined when the preprocessor define BLE_STACK_SUPPORT_REQD is defined.
 */

#ifndef BLE_STACK_HANDLER_TYPES_H__
#define BLE_STACK_HANDLER_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BLE_STACK_SUPPORT_REQD

#include <stdlib.h>
#include "headers/nrf_ble.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "app_util.h"
#include "sdk_config.h"

/** @brief  Default Maximum ATT MTU size.
 *
 * This define should be defined in the sdk_config.h file to override the default.
 */
#ifndef NRF_BLE_GATT_MAX_MTU_SIZE
#if ((NRF_SD_BLE_API_VERSION == 2) || (NRF_SD_BLE_API_VERSION == 3))
#define NRF_BLE_GATT_MAX_MTU_SIZE GATT_MTU_SIZE_DEFAULT
#else
#define NRF_BLE_GATT_MAX_MTU_SIZE BLE_GATT_MTU_SIZE_DEFAULT
#endif
#endif

#define BLE_STACK_EVT_MSG_BUF_SIZE       (sizeof(ble_evt_t) + (NRF_BLE_GATT_MAX_MTU_SIZE))     /**< Size of BLE event message buffer. This will be provided to the SoftDevice while fetching an event. */
#define BLE_STACK_HANDLER_SCHED_EVT_SIZE 0                                                     /**< The size of the scheduler event used by SoftDevice handler when passing BLE events using the @ref app_scheduler. */

/**@brief Application stack event handler type. */
typedef void (*ble_evt_handler_t)(ble_evt_t *p_ble_evt);

/**@brief     Function for registering for BLE events.
 *
 * @details   The application should use this function to register for receiving BLE events from
 *            the SoftDevice. If the application does not call this function, then any BLE event
 *            that may be generated by the SoftDevice will NOT be fetched. Once the application has
 *            registered for the events, it is not possible to cancel the registration.
 *            However, it is possible to register a different function for handling the events at
 *            any point of time.
 *
 * @param[in] ble_evt_handler Function to be called for each received BLE event.
 *
 * @retval    NRF_SUCCESS     Successful registration.
 * @retval    NRF_ERROR_NULL  Null pointer provided as input.
 */
uint32_t softdevice_ble_evt_handler_set(ble_evt_handler_t ble_evt_handler);

#else

#define BLE_STACK_EVT_MSG_BUF_SIZE        0                                                /**< Since the BLE stack support is not required, this is equated to 0, so that the @ref softdevice_handler.h can compute the internal event buffer size without having to care for BLE events.*/
#define BLE_STACK_HANDLER_SCHED_EVT_SIZE  0

#endif // BLE_STACK_SUPPORT_REQD


#ifdef __cplusplus
}
#endif

#endif // BLE_STACK_HANDLER_TYPES_H__

/** @} */
