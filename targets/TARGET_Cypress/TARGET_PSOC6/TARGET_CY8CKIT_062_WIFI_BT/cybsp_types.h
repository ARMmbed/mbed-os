/***************************************************************************//**
* \file CY8CKIT-062-WIFI-BT/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062-WIFI-BT pioneer kit.
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

/** LED 8; User LED1 */
#define CYBSP_LED8                  (P1_5)
/** LED 9; User LED2 */
#define CYBSP_LED9                  (P13_7)
/** LED 5: RGB LED - Red; User LED3 */
#define CYBSP_LED_RGB_RED           (P0_3)
/** LED 5: RGB LED - Green; User LED4 */
#define CYBSP_LED_RGB_GREEN         (P1_1)
/** LED 5: RGB LED - Blue; User LED5 */
#define CYBSP_LED_RGB_BLUE          (P11_1)

/** LED 8; User LED1 */
#define CYBSP_USER_LED1             (CYBSP_LED8)
/** LED 9; User LED2 */
#define CYBSP_USER_LED2             (CYBSP_LED9)
/** LED 5: RGB LED - Red; User LED3 */
#define CYBSP_USER_LED3             (CYBSP_LED_RGB_RED)
/** LED 5: RGB LED - Green; User LED4 */
#define CYBSP_USER_LED4             (CYBSP_LED_RGB_GREEN)
/** LED 5: RGB LED - Blue; User LED5 */
#define CYBSP_USER_LED5             (CYBSP_LED_RGB_BLUE)
/** LED 8; User LED1 */
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

/** Pin: WIFI SDIO D0 */
#define CYBSP_WIFI_SDIO_D0          (P2_0)
/** Pin: WIFI SDIO D1 */
#define CYBSP_WIFI_SDIO_D1          (P2_1)
/** Pin: WIFI SDIO D2 */
#define CYBSP_WIFI_SDIO_D2          (P2_2)
/** Pin: WIFI SDIO D3 */
#define CYBSP_WIFI_SDIO_D3          (P2_3)
/** Pin: WIFI SDIO CMD */
#define CYBSP_WIFI_SDIO_CMD         (P2_4)
/** Pin: WIFI SDIO CLK */
#define CYBSP_WIFI_SDIO_CLK         (P2_5)
/** Pin: WIFI ON */
#define CYBSP_WIFI_WL_REG_ON        (P2_6)
/** Pin: WIFI Host Wakeup */
#define CYBSP_WIFI_HOST_WAKE        (P2_7)

/** Pin: BT UART RX */
#define CYBSP_BT_UART_RX            (P3_0)
/** Pin: BT UART TX */
#define CYBSP_BT_UART_TX            (P3_1)
/** Pin: BT UART RTS */
#define CYBSP_BT_UART_RTS           (P3_2)
/** Pin: BT UART CTS */
#define CYBSP_BT_UART_CTS           (P3_3)

/** Pin: BT Power */
#define CYBSP_BT_POWER              (P3_4)
/** Pin: BT Host Wakeup */
#define CYBSP_BT_HOST_WAKE          (P3_5)
/** Pin: BT Device Wakeup */
#define CYBSP_BT_DEVICE_WAKE        (P4_0)

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX         (P5_0)
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX         (P5_1)

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL               (P6_0)
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA               (P6_1)

/** Pin: SWO */
#define CYBSP_SWO                   (P6_4)
/** Pin: SWDIO */
#define CYBSP_SWDIO                 (P6_6)
/** Pin: SWDCK */
#define CYBSP_SWDCK                 (P6_7)

/** Pin: QUAD SPI SS */
#define CYBSP_QSPI_SS               (P11_2)
/** Pin: QUAD SPI D3 */
#define CYBSP_QSPI_D3               (P11_3)
/** Pin: QUAD SPI D2 */
#define CYBSP_QSPI_D2               (P11_4)
/** Pin: QUAD SPI D1 */
#define CYBSP_QSPI_D1               (P11_5)
/** Pin: QUAD SPI D0 */
#define CYBSP_QSPI_D0               (P11_6)
/** Pin: QUAD SPI SCK */
#define CYBSP_QSPI_SCK              (P11_7)

/** Host-wake GPIO drive mode */
#define CYBSP_WIFI_HOST_WAKE_GPIO_DM (CYHAL_GPIO_DRIVE_ANALOG)
/** Host-wake IRQ event */
#define CYBSP_WIFI_HOST_WAKE_IRQ_EVENT (CYHAL_GPIO_IRQ_RISE)

/** \} group_bsp_pins_comm */


/**
* \addtogroup group_bsp_pins_arduino Arduino Header Pins
* \{
*/

/** Arduino A0 */
#define CYBSP_A0                    P10_0
/** Arduino A1 */
#define CYBSP_A1                    P10_1
/** Arduino A2 */
#define CYBSP_A2                    P10_2
/** Arduino A3 */
#define CYBSP_A3                    P10_3
/** Arduino A4 */
#define CYBSP_A4                    P10_4
/** Arduino A5 */
#define CYBSP_A5                    P10_5
/** Arduino D0 */
#define CYBSP_D0                    (P5_0)
/** Arduino D1 */
#define CYBSP_D1                    (P5_1)
/** Arduino D2 */
#define CYBSP_D2                    (P5_2)
/** Arduino D3 */
#define CYBSP_D3                    (P5_3)
/** Arduino D4 */
#define CYBSP_D4                    (P5_4)
/** Arduino D5 */
#define CYBSP_D5                    (P5_5)
/** Arduino D6 */
#define CYBSP_D6                    (P5_6)
/** Arduino D7 */
#define CYBSP_D7                    (P0_2)
/** Arduino D8 */
#define CYBSP_D8                    (P13_0)
/** Arduino D9 */
#define CYBSP_D9                    (P13_1)
/** Arduino D10 */
#define CYBSP_D10                   (P12_3)
/** Arduino D11 */
#define CYBSP_D11                   (P12_0)
/** Arduino D12 */
#define CYBSP_D12                   (P12_1)
/** Arduino D13 */
#define CYBSP_D13                   (P12_2)
/** Arduino D14 */
#define CYBSP_D14                   (P6_1)
/** Arduino D15 */
#define CYBSP_D15                   (P6_0)

/** \} group_bsp_pins_arduino */

/** \} group_bsp_pins */

#if defined(__cplusplus)
}
#endif
