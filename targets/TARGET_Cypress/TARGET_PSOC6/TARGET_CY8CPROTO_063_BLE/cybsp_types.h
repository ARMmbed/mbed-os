/***************************************************************************//**
* \file CY8CPROTO-063-BLE/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-063-BLE kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#pragma once

#include "cyhal_pin_package.h"

#if defined(__cplusplus)
extern "C" {
#endif


/**
* \addtogroup group_bsp_pin_state Pin States
* \{
*/

/** Pin state for the LED on. */
#define CYBSP_LED_STATE_ON          (0U)
/** Pin state for the LED off. */
#define CYBSP_LED_STATE_OFF         (1U)

/** Pin state for when a button is pressed. */
#define CYBSP_BTN_PRESSED           (0U)
/** Pin state for when a button is released. */
#define CYBSP_BTN_OFF               (1U)

/** \} group_bsp_pin_state */


/**
* \addtogroup group_bsp_pins Pin Mappings
* \{
*/

/**
* \addtogroup group_bsp_pins_led LED Pins
* \{
*/

/** LED 3; User LED1 (red) */
#define CYBSP_LED3                  (P6_3)
/** LED 4; User LED2 (green) */
#define CYBSP_LED4                  (P7_1)

/** LED 3; User LED1 (red) */
#define CYBSP_USER_LED1             (CYBSP_LED3)
/** LED 4; User LED2 (green) */
#define CYBSP_USER_LED2             (CYBSP_LED4)
/** LED 3; User LED1 (red) */
#define CYBSP_USER_LED              (CYBSP_USER_LED1)

/** \} group_bsp_pins_led */

/**
* \addtogroup group_bsp_pins_btn Button Pins
* \{
*/

/** Switch 2; User Button 1 */
#define CYBSP_SW2                   (P0_4)

/** Switch 2; User Button 1 */
#define CYBSP_USER_BTN1             (CYBSP_SW2)
/** Switch 2; User Button 1 */
#define CYBSP_USER_BTN              (CYBSP_USER_BTN1)

/** \} group_bsp_pins_btn */


/**
* \addtogroup group_bsp_pins_comm Communication Pins
* \{
*/

/** Pin: UART RX */
#define CYBSP_UART_RX          P5_0
/** Pin: UART TX */
#define CYBSP_UART_TX          P5_1

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL          P6_4
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA          P6_5

/** Pin: SWDIO */
#define CYBSP_SWDIO            P6_6
/** Pin: SWDCK */
#define CYBSP_SWDCK            P6_7

/** \} group_bsp_pins_comm */

/** \} group_bsp_pins */

#if defined(__cplusplus)
}
#endif
