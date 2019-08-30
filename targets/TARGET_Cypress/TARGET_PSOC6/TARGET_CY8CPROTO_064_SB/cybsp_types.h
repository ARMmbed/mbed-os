/***************************************************************************//**
* \file CY8CPROTO-064-SB/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-064-SB kit.
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

/* Board components mapping */
/** Pin: LED3 in the CY8CPROTO-064-SB board */
#define CYBSP_LED3                P13_7
/** Pin: LED4 in the CY8CPROTO-064-SB board */
#define CYBSP_LED4                P1_5
/** Pin: SW2 in the CY8CPROTO-064-SB board */
#define CYBSP_SW2                 P0_4

/* Board peripheral count */
/** Macro: Number of LEDs on CY8CPROTO-064-SB board */
#define CYBSP_LED_COUNT     2
/** Macro: Number of buttons on CY8CPROTO-064-SB board */
#define CYBSP_BTN_COUNT     1

/* Generic signal names */
/** Pin: WCO input */
#define CYBSP_WCO_IN           P0_0
/** Pin: WCO output */
#define CYBSP_WCO_OUT          P0_1

/** Pin: ECO input */
#define CYBSP_ECO_IN           P12_6
/** Pin: ECO output */
#define CYBSP_ECO_OUT          P12_7

/** Pin: UART RX */
#define CYBSP_UART_RX          P5_0
/** Pin: UART TX */
#define CYBSP_UART_TX          P5_1

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL          P6_0
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA          P6_1

/** Pin: SWDIO */
#define CYBSP_SWDIO            P6_6
/** Pin: SWDCK */
#define CYBSP_SWDCK            P6_7
/** Pin: SWO */
#define CYBSP_SWO              P6_4

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

/** Pin: USB Device D+ */
#define CYBSP_USB_DP           P14_0
/** Pin: USB Device D- */
#define CYBSP_USB_DM           P14_1

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
    CYBSP_LED_RED = CYBSP_LED3,
    CYBSP_LED_GREEN = CYBSP_LED4,

    CYBSP_USER_LED = CYBSP_LED_RED,
    CYBSP_USER_LED1 = CYBSP_LED_RED,
    CYBSP_USER_LED2 = CYBSP_LED_GREEN,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_USER_BTN = CYBSP_SW2,
    CYBSP_USER_BTN1 = CYBSP_SW2,
} cybsp_btn_t;

/** \} group_bsp_enums */

#if defined(__cplusplus)
}
#endif
