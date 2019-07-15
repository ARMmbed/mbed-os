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

/**
* \addtogroup group_bsp_cy8cproto_062_4343w CY8CPROTO-062-4343W
* \ingroup group_bsp
* \{
* \defgroup group_bsp_cy8cproto_062_4343w_macros Macros
* \defgroup group_bsp_cy8cproto_062_4343w_enums Enumerated Types
*/

#pragma once

#include "cyhal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_bsp_cy8cproto_062_4343w_macros
* \{
*/

/** Pin: WIFI SDIO D0 */
#define CYBSP_WIFI_SDIO_D0     P2_0
/** Pin: WIFI SDIO D1 */
#define CYBSP_WIFI_SDIO_D1     P2_1
/** Pin: WIFI SDIO D2 */
#define CYBSP_WIFI_SDIO_D2     P2_2
/** Pin: WIFI SDIO D3 */
#define CYBSP_WIFI_SDIO_D3     P2_3
/** Pin: WIFI SDIO CMD */
#define CYBSP_WIFI_SDIO_CMD    P2_4
/** Pin: WIFI SDIO CLK */
#define CYBSP_WIFI_SDIO_CLK    P2_5
/** Pin: WIFI ON */
#define CYBSP_WIFI_WL_REG_ON   P2_6
/** Pin: WIFI Host Wakeup */
#define CYBSP_WIFI_HOST_WAKE   P1_4

/** Pin: UART RX */
#define CYBSP_DEBUG_UART_RX    P5_0
/** Pin: UART TX */
#define CYBSP_DEBUG_UART_TX    P5_1

/** Host-wake GPIO drive mode */
#define CYBSP_WIFI_HOST_WAKE_GPIO_DM CYHAL_GPIO_DRIVE_ANALOG
/** Host-wake IRQ event */
#define CYBSP_WIFI_HOST_WAKE_IRQ_EVENT CYHAL_GPIO_IRQ_RISE

/** \} group_bsp_cy8cproto_062_4343w_macros */

/**
* \addtogroup group_bsp_cy8cproto_062_4343w_enums
* \{
*/

/** Enum defining the different states for the LED. */
typedef enum
{
	CYBSP_LED_STATE_ON        	= 0,
	CYBSP_LED_STATE_OFF        	= 1,
} cybsp_led_state_t;

/** Enum defining the different states for a button. */
typedef enum
{
	CYBSP_BTN_PRESSED  	      	= 0,
	CYBSP_BTN_OFF	          	= 1,
} cybsp_btn_state_t;

/** Enum defining the different LED pins on the board. */
typedef enum
{
    CYBSP_LED_RED = P13_7,

    CYBSP_USER_LED1 = CYBSP_LED_RED,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;

/** Enum defining the different button pins on the board. */
typedef enum
{
    CYBSP_SW2 = P0_4,
    
    CYBSP_USER_BTN1 = CYBSP_SW2,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;

/** \} group_bsp_cy8cproto_062_4343w_enums */

#if defined(__cplusplus)
}
#endif

/** \} group_bsp_cy8cproto_062_4343w */
