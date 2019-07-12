/***************************************************************************//**
* \file CY8CPROTO-062-4343W/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-062-4343W kit.
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

/** Pin: WIFI SDIO D0 */
/* Corresponds to: ioss[0].port[2].pin[0], sdhc[0] */
#define CYBSP_WIFI_SDIO_D0     P2_0
/** Pin: WIFI SDIO D1 */
/* Corresponds to: ioss[0].port[2].pin[1], sdhc[0] */
#define CYBSP_WIFI_SDIO_D1     P2_1
/** Pin: WIFI SDIO D2 */
/* Corresponds to: ioss[0].port[2].pin[2], sdhc[0] */
#define CYBSP_WIFI_SDIO_D2     P2_2
/** Pin: WIFI SDIO D3 */
/* Corresponds to: ioss[0].port[2].pin[3], sdhc[0] */
#define CYBSP_WIFI_SDIO_D3     P2_3
/** Pin: WIFI SDIO CMD */
/* Corresponds to: ioss[0].port[2].pin[4], sdhc[0] */
#define CYBSP_WIFI_SDIO_CMD    P2_4
/** Pin: WIFI SDIO CLK */
/* Corresponds to: ioss[0].port[2].pin[5], sdhc[0] */
#define CYBSP_WIFI_SDIO_CLK    P2_5
/** Pin: WIFI ON */
/* Corresponds to: ioss[0].port[2].pin[6], sdhc[0] */
#define CYBSP_WIFI_WL_REG_ON   P2_6
/** Pin: WIFI Host Wakeup */
#define CYBSP_WIFI_HOST_WAKE   P0_4

/** Pin: BT UART RX */
#define CYBSP_BT_UART_RX       P3_0
/** Pin: BT UART TX */
#define CYBSP_BT_UART_TX       P3_1
/** Pin: BT UART RTS */
#define CYBSP_BT_UART_RTS      P3_2
/** Pin: BT UART CTS */
#define CYBSP_BT_UART_CTS      P3_3

/** Pin: BT Power */
#define CYBSP_BT_POWER         P3_4
/** Pin: BT Host Wakeup */
#define CYBSP_BT_HOST_WAKE     P4_0
/** Pin: BT Device Wakeup */
#define CYBSP_BT_DEVICE_WAKE   P3_5

/** Pin: UART RX */
/* Corresponds to: ioss[0].port[5].pin[0], scb[5] */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
/* Corresponds to: ioss[0].port[5].pin[1], scb[5] */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Pin: QUAD SPI SS */
#define CYBSP_QSPI_SS          P11_2
/** Pin: QUAD SPI D3 */
#define CYBSP_QSPI_D3          P11_3
/** Pin: QUAD SPI D2 */
#define CYBSP_QSPI_D2          P11_4
/** Pin: QUAD SPI D1 */
#define CYBSP_QSPI_D1          P11_5
/** Pin: QUAD SPI D0 */
#define CYBSP_QSPI_D0          P11_6
/** Pin: QUAD SPI SCK */
#define CYBSP_QSPI_SCK         P11_7

/** Host-wake GPIO drive mode */
#define CYBSP_WIFI_HOST_WAKE_GPIO_DM CYHAL_GPIO_DRIVE_ANALOG
/** Host-wake IRQ event */
#define CYBSP_WIFI_HOST_WAKE_IRQ_EVENT CYHAL_GPIO_IRQ_RISE

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
    CYBSP_LED_RED = P13_7,

    /* Corresponds to: ioss[0].port[13].pin[7] */
    CYBSP_USER_LED1 = CYBSP_LED_RED,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_SW2 = P0_4,
    
    /* Corresponds to: ioss[0].port[0].pin[4] */
    CYBSP_USER_BTN1 = CYBSP_SW2,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;

/** \} group_bsp_enums */

#if defined(__cplusplus)
}
#endif
