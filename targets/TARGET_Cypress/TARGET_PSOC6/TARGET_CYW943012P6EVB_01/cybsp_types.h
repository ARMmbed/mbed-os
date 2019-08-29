/***************************************************************************//**
* \file CYW943012P6EVB-01/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CYW943012P6EVB-01 evaluation kit.
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

#include "cyhal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_pins Pin Mappings
* \{
*/

// Arduino connector namings
/** Arduino A0 */
#define CYBSP_A0 P10_0
/** Arduino A1 */
#define CYBSP_A1 P10_1
/** Arduino A2 */
#define CYBSP_A2 P6_4
/** Arduino A3 */
#define CYBSP_A3 P6_5
/** Arduino A4 */
#define CYBSP_A4 P10_4
/** Arduino A5 */
#define CYBSP_A5 P10_5
/** Arduino D0 */
#define CYBSP_D0 P5_0
/** Arduino D1 */
#define CYBSP_D1 P5_1
/** Arduino D2 */
#define CYBSP_D2 P5_2
/** Arduino D3 */
#define CYBSP_D3 P5_3
/** Arduino D4 */
#define CYBSP_D4 P5_4
/** Arduino D5 */
#define CYBSP_D5 P5_5
/** Arduino D6 */
#define CYBSP_D6 P5_6
/** Arduino D7 */
#define CYBSP_D7 P5_7
/** Arduino D8 */
#define CYBSP_D8 NC
/** Arduino D9 */
#define CYBSP_D9 NC
/** Arduino D10 */
#define CYBSP_D10 P0_5
/** Arduino D11 */
#define CYBSP_D11 P0_2
/** Arduino D12 */
#define CYBSP_D12 P0_3
/** Arduino D13 */
#define CYBSP_D13 P0_4
/** Arduino D14 */
#define CYBSP_D14 P1_0
/** Arduino D15 */
#define CYBSP_D15 P1_1

// Generic signal names
/** Pin: CYBSP_WCO_IN         */
#define CYBSP_WCO_IN         P0_0
/** Pin: CYBSP_WCO_OUT        */
#define CYBSP_WCO_OUT        P0_1

/** Pin: CYBSP_CSD_TX         */
#define CYBSP_CSD_TX         P1_0
/** Pin: CYBSP_WL_SECI_IN     */
#define CYBSP_WL_SECI_IN     P1_2
/** Pin: CYBSP_WL_FRAM_SYNC   */
#define CYBSP_WL_FRAM_SYNC   P1_3
/** Pin: CYBSP_WL_PRIORITY    */
#define CYBSP_WL_PRIORITY    P1_4
/** Pin: CYBSP_WL_SECI_OUT    */
#define CYBSP_WL_SECI_OUT    P1_5

/** Pin: CYBSP_WIFI_SDIO_D0   */
/* Corresponds to: ioss[0].port[2].pin[0], udb[0] */
#define CYBSP_WIFI_SDIO_D0   P2_0
/** Pin: CYBSP_WIFI_SDIO_D1   */
/* Corresponds to: ioss[0].port[2].pin[1], udb[0] */
#define CYBSP_WIFI_SDIO_D1   P2_1
/** Pin: CYBSP_WIFI_SDIO_D2   */
/* Corresponds to: ioss[0].port[2].pin[2], udb[0] */
#define CYBSP_WIFI_SDIO_D2   P2_2
/** Pin: CYBSP_WIFI_SDIO_D3   */
/* Corresponds to: ioss[0].port[2].pin[3], udb[0] */
#define CYBSP_WIFI_SDIO_D3   P2_3
/** Pin: CYBSP_WIFI_SDIO_CMD  */
/* Corresponds to: ioss[0].port[2].pin[4], udb[0] */
#define CYBSP_WIFI_SDIO_CMD  P2_4
/** Pin: CYBSP_WIFI_SDIO_CLK  */
/* Corresponds to: ioss[0].port[2].pin[5], udb[0] */
#define CYBSP_WIFI_SDIO_CLK  P2_5
/** Pin: CYBSP_WIFI_WL_REG_ON */
/* Corresponds to: ioss[0].port[2].pin[6], udb[0] */
#define CYBSP_WIFI_WL_REG_ON P2_6
/** Pin: CYBSP_WIFI_HOST_WAKE */
#define CYBSP_WIFI_HOST_WAKE P2_7
/** Host-wake GPIO drive mode */
#define CYBSP_WIFI_HOST_WAKE_GPIO_DM CYHAL_GPIO_DRIVE_ANALOG
/** Host-wake IRQ event */
#define CYBSP_WIFI_HOST_WAKE_IRQ_EVENT CYHAL_GPIO_IRQ_RISE

/** Pin: CYBSP_BT_UART_RX     */
#define CYBSP_BT_UART_RX     P3_0
/** Pin: CYBSP_BT_UART_TX     */
#define CYBSP_BT_UART_TX     P3_1
/** Pin: CYBSP_BT_UART_RTS    */
#define CYBSP_BT_UART_RTS    P3_2
/** Pin: CYBSP_BT_UART_CTS    */
#define CYBSP_BT_UART_CTS    P3_3

/** Pin: BT Power */
#define CYBSP_BT_POWER       P3_4
/** Pin: CYBSP_BT_HOST_WAKE   */
#define CYBSP_BT_HOST_WAKE   P3_5
/** Pin: CYBSP_BT_DEVICE_WAKE */
#define CYBSP_BT_DEVICE_WAKE P4_0
/** Pin: CYBSP_BT_RST         */
#define CYBSP_BT_RST         P4_1

/** Pin: UART RX */
/* Corresponds to: ioss[0].port[13].pin[0], scb[6] */
#define CYBSP_DEBUG_UART_RX    P13_0
/** Pin: UART TX */
/* Corresponds to: ioss[0].port[13].pin[1], scb[6] */
#define CYBSP_DEBUG_UART_TX    P13_1

/** Pin: CYBSP_I2C_SCL        */
#define CYBSP_I2C_SCL        P6_0
/** Pin: CYBSP_I2C_SDA        */
#define CYBSP_I2C_SDA        P6_1
/** Pin: CYBSP_TDO_SWO        */
#define CYBSP_TDO_SWO        P6_4
/** Pin: CYBSP_TMS_SWDIO      */
#define CYBSP_TMS_SWDIO      P6_6
/** Pin: CYBSP_SWCLK          */
#define CYBSP_SWCLK          P6_7

/** Pin: CYBSP_TRACECLK       */
#define CYBSP_TRACECLK       P7_0
/** Pin: CYBSP_CINTA          */
#define CYBSP_CINTA          P7_1
/** Pin: CYBSP_CINTB          */
#define CYBSP_CINTB          P7_2
/** Pin: CYBSP_CMOD           */
#define CYBSP_CMOD           P7_7

/** Pin: CYBSP_CSD_BTN0       */
#define CYBSP_CSD_BTN0       P8_1
/** Pin: CYBSP_CSD_BTN1       */
#define CYBSP_CSD_BTN1       P8_2
/** Pin: CYBSP_CSD_SLD0       */
#define CYBSP_CSD_SLD0       P8_3
/** Pin: CYBSP_CSD_SLD1       */
#define CYBSP_CSD_SLD1       P8_4
/** Pin: CYBSP_CSD_SLD2       */
#define CYBSP_CSD_SLD2       P8_5
/** Pin: CYBSP_CSD_SLD3       */
#define CYBSP_CSD_SLD3       P8_6
/** Pin: CYBSP_CSD_SLD4       */
#define CYBSP_CSD_SLD4       P8_7

/** Pin: CYBSP_TRACEDATA3     */
#define CYBSP_TRACEDATA3     P9_0
/** Pin: CYBSP_TRACEDATA2     */
#define CYBSP_TRACEDATA2     P9_1
/** Pin: CYBSP_TRACEDATA1     */
#define CYBSP_TRACEDATA1     P9_2
/** Pin: CYBSP_TRACEDATA0     */
#define CYBSP_TRACEDATA0     P9_3

/** Pin: CYBSP_ROW6_SPI_MOSI  */
#define CYBSP_ROW6_SPI_MOSI  P10_0
/** Pin: CYBSP_COL8_SPI_MISO  */
#define CYBSP_COL8_SPI_MISO  P10_1
/** Pin: CYBSP_ROW7_SPI_CLK   */
#define CYBSP_ROW7_SPI_CLK   P10_2
/** Pin: CYBSP_COL7_SPI_CS    */
#define CYBSP_COL7_SPI_CS    P10_3
/** Pin: CYBSP_BAT_MON        */
#define CYBSP_BAT_MON        P10_4
/** Pin: CYBSP_WL_WAKE        */
#define CYBSP_WL_WAKE        P10_7

/** Pin: CYBSP_UART_RX        */
#define CYBSP_UART_RX        P11_0
/** Pin: CYBSP_UART_TX        */
#define CYBSP_UART_TX        P11_1
/** Pin: CYBSP_QSPI_SS        */
#define CYBSP_QSPI_SS        P11_2
/** Pin: CYBSP_QSPI_D3        */
#define CYBSP_QSPI_D3        P11_3
/** Pin: CYBSP_QSPI_D2        */
#define CYBSP_QSPI_D2        P11_4
/** Pin: CYBSP_QSPI_D1        */
#define CYBSP_QSPI_D1        P11_5
/** Pin: CYBSP_QSPI_D0        */
#define CYBSP_QSPI_D0        P11_6
/** Pin: CYBSP_QSPI_SCK       */
#define CYBSP_QSPI_SCK       P11_7

/** Pin: CYBSP_BT_GPIO4       */
#define CYBSP_BT_GPIO4       P12_0
/** Pin: CYBSP_BT_GPIO5       */
#define CYBSP_BT_GPIO5       P12_1
/** Pin: CYBSP_BT_GPIO2       */
#define CYBSP_BT_GPIO2       P12_2
/** Pin: CYBSP_BT_GPIO3       */
#define CYBSP_BT_GPIO3       P12_3
/** Pin: CYBSP_ECO_IN         */
#define CYBSP_ECO_IN         P12_6
/** Pin: CYBSP_ECO_OUT        */
#define CYBSP_ECO_OUT        P12_7

/** Pin: CYBSP_P6_UART_RX     */
/* Corresponds to: ioss[0].port[13].pin[0], scb[6] */
#define CYBSP_P6_UART_RX     P13_0
/** Pin: CYBSP_P6_UART_TX     */
/* Corresponds to: ioss[0].port[13].pin[1], scb[6] */
#define CYBSP_P6_UART_TX     P13_1
/** Pin: CYBSP_USB_DEV_VBUS_DET */
#define CYBSP_USB_DEV_VBUS_DET P13_4
/** Pin: CYBSP_USB_HOST_EN    */
#define CYBSP_USB_HOST_EN    P13_5
/** Pin: CYBSP_USB_INT_L      */
#define CYBSP_USB_INT_L      P13_7

/** Pin: CYBSP_USB_DP         */
#define CYBSP_USB_DP         P14_0
/** Pin: CYBSP_USB_DM         */
#define CYBSP_USB_DM         P14_1

/** \} group_bsp_pins */

/**
* \addtogroup group_bsp_enums Enumerated Types
* \{
*/

/** Enum defining the different states for the LED. */
typedef enum
{
    CYBSP_LED_STATE_ON          = 0,
    CYBSP_LED_STATE_OFF         = 1,
} cybsp_led_state_t;

/** Enum defining the different states for a button. */
typedef enum
{
    CYBSP_BTN_PRESSED           = 0,
    CYBSP_BTN_OFF               = 1,
} cybsp_btn_state_t;

/** Enum defining the different LED pins on the board. */
typedef enum
{
    CYBSP_LED_RGB_RED = P0_3,
    CYBSP_LED_RGB_GREEN = P1_1,
    CYBSP_LED_RGB_BLUE = P10_6,

    /* Corresponds to: ioss[0].port[0].pin[3] */
    CYBSP_USER_LED1 = CYBSP_LED_RGB_RED,
    /* Corresponds to: ioss[0].port[1].pin[1] */
    CYBSP_USER_LED2 = CYBSP_LED_RGB_GREEN,
    /* Corresponds to: ioss[0].port[10].pin[6] */
    CYBSP_USER_LED3 = CYBSP_LED_RGB_BLUE,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_SW6 = P0_4,

    /* Corresponds to: ioss[0].port[0].pin[4] */
    CYBSP_USER_BTN1 = CYBSP_SW6,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;

/** \} group_bsp_enums */

#if defined(__cplusplus)
}
#endif
