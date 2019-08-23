/***************************************************************************//**
* \file CY8CKIT-062S2-43012/cybsp_types.h
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CKIT-062S2-43012 kit.
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
#define CYBSP_A2 P10_2
/** Arduino A3 */
#define CYBSP_A3 P10_3
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
#define CYBSP_D7 P0_2
/** Arduino D8 */
#define CYBSP_D8 P13_0
/** Arduino D9 */
#define CYBSP_D9 P13_1
/** Arduino D10 */
#define CYBSP_D10 P12_3
/** Arduino D11 */
#define CYBSP_D11 P12_0
/** Arduino D12 */
#define CYBSP_D12 P12_1
/** Arduino D13 */
#define CYBSP_D13 P12_2
/** Arduino D14 */
#define CYBSP_D14 P6_1
/** Arduino D15 */
#define CYBSP_D15 P6_0


/** Pin: WCO input */
#define CYBSP_WCO_IN           P0_0
/** Pin: WCO output */
#define CYBSP_WCO_OUT          P0_1

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
#define CYBSP_WIFI_HOST_WAKE   P4_1

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
/** Pin: UART RX */
/* Corresponds to: ioss[0].port[5].pin[2], scb[5] */
#define CYBSP_DEBUG_UART_RTS   P5_2
/** Pin: UART TX */
/* Corresponds to: ioss[0].port[5].pin[3], scb[5] */
#define CYBSP_DEBUG_UART_CTS   P5_3

/** Pin: I2C SCL */
#define CYBSP_I2C_SCL          P6_0
/** Pin: I2C SDA */
#define CYBSP_I2C_SDA          P6_1

/** Pin: SWO */
#define CYBSP_SWO              P6_4
/** Pin: SWDIO */
#define CYBSP_SWDIO            P6_6
/** Pin: SWDCK */
#define CYBSP_SWDCK            P6_7

/** Pin: CapSesnse RX */
#define CYBSP_CSD_RX           P1_0
/** Pin: CapSesnse CINA */
#define CYBSP_CINA             P7_1
/** Pin: CapSesnse CINB */
#define CYBSP_CINB             P7_2
/** Pin: CapSesnse CMOD */
#define CYBSP_CMOD             P7_7
/** Pin: CapSesnse Button 0 */
#define CYBSP_CSD_BTN0         P8_1
/** Pin: CapSesnse Button 1 */
#define CYBSP_CSD_BTN1         P8_2
/** Pin: CapSesnse Slider 0 */
#define CYBSP_CSD_SLD0         P8_3
/** Pin: CapSesnse Slider 1 */
#define CYBSP_CSD_SLD1         P8_4
/** Pin: CapSesnse Slider 2 */
#define CYBSP_CSD_SLD2         P8_5
/** Pin: CapSesnse Slider 3 */
#define CYBSP_CSD_SLD3         P8_6
/** Pin: CapSesnse Slider 4 */
#define CYBSP_CSD_SLD4         P8_7

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

/** BSP user LED reference designator to pin mapping */
#define LED9_R                 P11_1
/** BSP user LED reference designator to pin mapping */
#define LED8_O                 P1_5
/** BSP user LED reference designator to pin mapping */
#define LED5_RGB_R             P1_1
/** BSP user LED reference designator to pin mapping */
#define LED5_RGB_G             P0_5
/** BSP user LED reference designator to pin mapping */
#define LED5_RGB_B             P7_3

/** BSP LED defines by LED color */
#define CYBSP_LED_RED          LED5_RGB_R
/** BSP LED defines by LED color */
#define CYBSP_LED_GREEN        LED5_RGB_G
/** BSP LED defines by LED color */
#define CYBSP_LED_BLUE         LED5_RGB_B
/** BSP LED defines by LED color */
#define CYBSP_LED_ORANGE       LED8


/** BSP user button reference designator to pin mapping */
#define SW2                    P0_4
/** BSP user button reference designator to pin mapping */
#define SW4                    P1_4


/** \} group_bsp_cy8ckit_pins */

/**
* \addtogroup group_bsp_enums Enumerated Types
* \{
*/

/** Enum defining the different states for the LED. */
typedef enum
{
    CYBSP_LED_STATE_ON             = 0,
    CYBSP_LED_STATE_OFF            = 1,
} cybsp_led_state_t;

/** Enum defining the different states for a button. */
typedef enum
{
    CYBSP_BTN_PRESSED              = 0,
    CYBSP_BTN_OFF                  = 1,
} cybsp_btn_state_t;


/** Enum defining the different user LEDs available on the board. */
typedef enum
{
    CYBSP_LED_RGB_RED = LED5_RGB_R,
    CYBSP_LED_RGB_GREEN = LED5_RGB_G,
    CYBSP_LED_RGB_BLUE = LED5_RGB_B,

    /* Corresponds to: ioss[0].port[11].pin[1] */
    CYBSP_USER_LED1 = LED9_R,
    /* Corresponds to: ioss[0].port[1].pin[5] */
    CYBSP_USER_LED2 = LED8_O,
    /* Corresponds to: ioss[0].port[1].pin[1] */
    CYBSP_USER_LED3 = CYBSP_LED_RGB_RED,
    /* Corresponds to: ioss[0].port[0].pin[5] */
    CYBSP_USER_LED4 = CYBSP_LED_RGB_GREEN,
    /* Corresponds to: ioss[0].port[7].pin[3] */
    CYBSP_USER_LED5 = CYBSP_LED_RGB_BLUE,
    CYBSP_USER_LED = CYBSP_USER_LED1,
} cybsp_led_t;


/** Enum defining the different user buttons available on the board. */
typedef enum
{
    /* Corresponds to: ioss[0].port[0].pin[4] */
    CYBSP_USER_BTN1 = SW2,
    /* Corresponds to: ioss[0].port[1].pin[4] */
    CYBSP_USER_BTN2 = SW4,
    CYBSP_USER_BTN = CYBSP_USER_BTN1,
} cybsp_btn_t;


/** \} group_bsp_enums */

#if defined(__cplusplus)
}
#endif
