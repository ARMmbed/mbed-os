/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* 
********************************************************************************
* Copyright 2017-2019 Cypress Semiconductor Corporation
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
********************************************************************************/

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_WCO_IN_ENABLED 1U
#define CYBSP_WCO_IN_PORT GPIO_PRT0
#define CYBSP_WCO_IN_PIN 0U
#define CYBSP_WCO_IN_NUM 0U
#define CYBSP_WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define CYBSP_WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_WCO_OUT_ENABLED 1U
#define CYBSP_WCO_OUT_PORT GPIO_PRT0
#define CYBSP_WCO_OUT_PIN 1U
#define CYBSP_WCO_OUT_NUM 1U
#define CYBSP_WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define CYBSP_WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_LED_RED_ENABLED 1U
#define CYBSP_LED_RED_PORT GPIO_PRT0
#define CYBSP_LED_RED_PIN 3U
#define CYBSP_LED_RED_NUM 3U
#define CYBSP_LED_RED_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED_RED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_3_HSIOM
	#define ioss_0_port_0_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED_RED_HSIOM ioss_0_port_0_pin_3_HSIOM
#define CYBSP_LED_RED_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_SW6_ENABLED 1U
#define CYBSP_SW6_PORT GPIO_PRT0
#define CYBSP_SW6_PIN 4U
#define CYBSP_SW6_NUM 4U
#define CYBSP_SW6_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SW6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
	#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SW6_HSIOM ioss_0_port_0_pin_4_HSIOM
#define CYBSP_SW6_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_ROW6_SPI_MOSI_ENABLED 1U
#define CYBSP_ROW6_SPI_MOSI_PORT GPIO_PRT10
#define CYBSP_ROW6_SPI_MOSI_PIN 0U
#define CYBSP_ROW6_SPI_MOSI_NUM 0U
#define CYBSP_ROW6_SPI_MOSI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_ROW6_SPI_MOSI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_0_HSIOM
	#define ioss_0_port_10_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ROW6_SPI_MOSI_HSIOM ioss_0_port_10_pin_0_HSIOM
#define CYBSP_ROW6_SPI_MOSI_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_COL8_SPI_MISO_ENABLED 1U
#define CYBSP_COL8_SPI_MISO_PORT GPIO_PRT10
#define CYBSP_COL8_SPI_MISO_PIN 1U
#define CYBSP_COL8_SPI_MISO_NUM 1U
#define CYBSP_COL8_SPI_MISO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_COL8_SPI_MISO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_1_HSIOM
	#define ioss_0_port_10_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_COL8_SPI_MISO_HSIOM ioss_0_port_10_pin_1_HSIOM
#define CYBSP_COL8_SPI_MISO_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_ROW7_SPI_CLK_ENABLED 1U
#define CYBSP_ROW7_SPI_CLK_PORT GPIO_PRT10
#define CYBSP_ROW7_SPI_CLK_PIN 2U
#define CYBSP_ROW7_SPI_CLK_NUM 2U
#define CYBSP_ROW7_SPI_CLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_ROW7_SPI_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_2_HSIOM
	#define ioss_0_port_10_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ROW7_SPI_CLK_HSIOM ioss_0_port_10_pin_2_HSIOM
#define CYBSP_ROW7_SPI_CLK_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_COL7_SPI_CS_ENABLED 1U
#define CYBSP_COL7_SPI_CS_PORT GPIO_PRT10
#define CYBSP_COL7_SPI_CS_PIN 3U
#define CYBSP_COL7_SPI_CS_NUM 3U
#define CYBSP_COL7_SPI_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_COL7_SPI_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_3_HSIOM
	#define ioss_0_port_10_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_COL7_SPI_CS_HSIOM ioss_0_port_10_pin_3_HSIOM
#define CYBSP_COL7_SPI_CS_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_BAT_MON_ENABLED 1U
#define CYBSP_BAT_MON_PORT GPIO_PRT10
#define CYBSP_BAT_MON_PIN 4U
#define CYBSP_BAT_MON_NUM 4U
#define CYBSP_BAT_MON_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BAT_MON_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_4_HSIOM
	#define ioss_0_port_10_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BAT_MON_HSIOM ioss_0_port_10_pin_4_HSIOM
#define CYBSP_BAT_MON_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_LED_BLUE_ENABLED 1U
#define CYBSP_LED_BLUE_PORT GPIO_PRT10
#define CYBSP_LED_BLUE_PIN 6U
#define CYBSP_LED_BLUE_NUM 6U
#define CYBSP_LED_BLUE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED_BLUE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_6_HSIOM
	#define ioss_0_port_10_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED_BLUE_HSIOM ioss_0_port_10_pin_6_HSIOM
#define CYBSP_LED_BLUE_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_WL_WAKE_ENABLED 1U
#define CYBSP_WL_WAKE_PORT GPIO_PRT10
#define CYBSP_WL_WAKE_PIN 7U
#define CYBSP_WL_WAKE_NUM 7U
#define CYBSP_WL_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_WAKE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_7_HSIOM
	#define ioss_0_port_10_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_WAKE_HSIOM ioss_0_port_10_pin_7_HSIOM
#define CYBSP_WL_WAKE_IRQ ioss_interrupts_gpio_10_IRQn
#define CYBSP_WL_UART_RX_ENABLED 1U
#define CYBSP_WL_UART_RX_PORT GPIO_PRT11
#define CYBSP_WL_UART_RX_PIN 0U
#define CYBSP_WL_UART_RX_NUM 0U
#define CYBSP_WL_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WL_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_0_HSIOM
	#define ioss_0_port_11_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_UART_RX_HSIOM ioss_0_port_11_pin_0_HSIOM
#define CYBSP_WL_UART_RX_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_WL_UART_TX_ENABLED 1U
#define CYBSP_WL_UART_TX_PORT GPIO_PRT11
#define CYBSP_WL_UART_TX_PIN 1U
#define CYBSP_WL_UART_TX_NUM 1U
#define CYBSP_WL_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
	#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_UART_TX_HSIOM ioss_0_port_11_pin_1_HSIOM
#define CYBSP_WL_UART_TX_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_SS_ENABLED 1U
#define CYBSP_QSPI_SS_PORT GPIO_PRT11
#define CYBSP_QSPI_SS_PIN 2U
#define CYBSP_QSPI_SS_NUM 2U
#define CYBSP_QSPI_SS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SS_HSIOM ioss_0_port_11_pin_2_HSIOM
#define CYBSP_QSPI_SS_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_D3_ENABLED 1U
#define CYBSP_QSPI_D3_PORT GPIO_PRT11
#define CYBSP_QSPI_D3_PIN 3U
#define CYBSP_QSPI_D3_NUM 3U
#define CYBSP_QSPI_D3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define CYBSP_QSPI_D3_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_D2_ENABLED 1U
#define CYBSP_QSPI_D2_PORT GPIO_PRT11
#define CYBSP_QSPI_D2_PIN 4U
#define CYBSP_QSPI_D2_NUM 4U
#define CYBSP_QSPI_D2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define CYBSP_QSPI_D2_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_D1_ENABLED 1U
#define CYBSP_QSPI_D1_PORT GPIO_PRT11
#define CYBSP_QSPI_D1_PIN 5U
#define CYBSP_QSPI_D1_NUM 5U
#define CYBSP_QSPI_D1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define CYBSP_QSPI_D1_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_D0_ENABLED 1U
#define CYBSP_QSPI_D0_PORT GPIO_PRT11
#define CYBSP_QSPI_D0_PIN 6U
#define CYBSP_QSPI_D0_NUM 6U
#define CYBSP_QSPI_D0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define CYBSP_QSPI_D0_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_SCK_ENABLED 1U
#define CYBSP_QSPI_SCK_PORT GPIO_PRT11
#define CYBSP_QSPI_SCK_PIN 7U
#define CYBSP_QSPI_SCK_NUM 7U
#define CYBSP_QSPI_SCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define CYBSP_QSPI_SCK_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_BT_GPIO4_ENABLED 1U
#define CYBSP_BT_GPIO4_PORT GPIO_PRT12
#define CYBSP_BT_GPIO4_PIN 0U
#define CYBSP_BT_GPIO4_NUM 0U
#define CYBSP_BT_GPIO4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_0_HSIOM
	#define ioss_0_port_12_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO4_HSIOM ioss_0_port_12_pin_0_HSIOM
#define CYBSP_BT_GPIO4_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_BT_GPIO5_ENABLED 1U
#define CYBSP_BT_GPIO5_PORT GPIO_PRT12
#define CYBSP_BT_GPIO5_PIN 1U
#define CYBSP_BT_GPIO5_NUM 1U
#define CYBSP_BT_GPIO5_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_1_HSIOM
	#define ioss_0_port_12_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO5_HSIOM ioss_0_port_12_pin_1_HSIOM
#define CYBSP_BT_GPIO5_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_BT_GPIO2_ENABLED 1U
#define CYBSP_BT_GPIO2_PORT GPIO_PRT12
#define CYBSP_BT_GPIO2_PIN 2U
#define CYBSP_BT_GPIO2_NUM 2U
#define CYBSP_BT_GPIO2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_2_HSIOM
	#define ioss_0_port_12_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO2_HSIOM ioss_0_port_12_pin_2_HSIOM
#define CYBSP_BT_GPIO2_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_BT_GPIO3_ENABLED 1U
#define CYBSP_BT_GPIO3_PORT GPIO_PRT12
#define CYBSP_BT_GPIO3_PIN 3U
#define CYBSP_BT_GPIO3_NUM 3U
#define CYBSP_BT_GPIO3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_3_HSIOM
	#define ioss_0_port_12_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO3_HSIOM ioss_0_port_12_pin_3_HSIOM
#define CYBSP_BT_GPIO3_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_ECO_IN_ENABLED 1U
#define CYBSP_ECO_IN_PORT GPIO_PRT12
#define CYBSP_ECO_IN_PIN 6U
#define CYBSP_ECO_IN_NUM 6U
#define CYBSP_ECO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_ECO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_6_HSIOM
	#define ioss_0_port_12_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ECO_IN_HSIOM ioss_0_port_12_pin_6_HSIOM
#define CYBSP_ECO_IN_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_ECO_OUT_ENABLED 1U
#define CYBSP_ECO_OUT_PORT GPIO_PRT12
#define CYBSP_ECO_OUT_PIN 7U
#define CYBSP_ECO_OUT_NUM 7U
#define CYBSP_ECO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_ECO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_7_HSIOM
	#define ioss_0_port_12_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ECO_OUT_HSIOM ioss_0_port_12_pin_7_HSIOM
#define CYBSP_ECO_OUT_IRQ ioss_interrupts_gpio_12_IRQn
#define CYBSP_DEBUG_UART_RX_ENABLED 1U
#define CYBSP_DEBUG_UART_RX_PORT GPIO_PRT13
#define CYBSP_DEBUG_UART_RX_PIN 0U
#define CYBSP_DEBUG_UART_RX_NUM 0U
#define CYBSP_DEBUG_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_DEBUG_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_0_HSIOM
	#define ioss_0_port_13_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_RX_HSIOM ioss_0_port_13_pin_0_HSIOM
#define CYBSP_DEBUG_UART_RX_IRQ ioss_interrupts_gpio_13_IRQn
#define CYBSP_DEBUG_UART_TX_ENABLED 1U
#define CYBSP_DEBUG_UART_TX_PORT GPIO_PRT13
#define CYBSP_DEBUG_UART_TX_PIN 1U
#define CYBSP_DEBUG_UART_TX_NUM 1U
#define CYBSP_DEBUG_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_DEBUG_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_1_HSIOM
	#define ioss_0_port_13_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_TX_HSIOM ioss_0_port_13_pin_1_HSIOM
#define CYBSP_DEBUG_UART_TX_IRQ ioss_interrupts_gpio_13_IRQn
#define CYBSP_USB_DEV_VBUS_DET_ENABLED 1U
#define CYBSP_USB_DEV_VBUS_DET_PORT GPIO_PRT13
#define CYBSP_USB_DEV_VBUS_DET_PIN 4U
#define CYBSP_USB_DEV_VBUS_DET_NUM 4U
#define CYBSP_USB_DEV_VBUS_DET_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_USB_DEV_VBUS_DET_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_4_HSIOM
	#define ioss_0_port_13_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DEV_VBUS_DET_HSIOM ioss_0_port_13_pin_4_HSIOM
#define CYBSP_USB_DEV_VBUS_DET_IRQ ioss_interrupts_gpio_13_IRQn
#define CYBSP_USB_HOST_EN_ENABLED 1U
#define CYBSP_USB_HOST_EN_PORT GPIO_PRT13
#define CYBSP_USB_HOST_EN_PIN 5U
#define CYBSP_USB_HOST_EN_NUM 5U
#define CYBSP_USB_HOST_EN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_USB_HOST_EN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_5_HSIOM
	#define ioss_0_port_13_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_HOST_EN_HSIOM ioss_0_port_13_pin_5_HSIOM
#define CYBSP_USB_HOST_EN_IRQ ioss_interrupts_gpio_13_IRQn
#define CYBSP_USB_INT_L_ENABLED 1U
#define CYBSP_USB_INT_L_PORT GPIO_PRT13
#define CYBSP_USB_INT_L_PIN 7U
#define CYBSP_USB_INT_L_NUM 7U
#define CYBSP_USB_INT_L_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_USB_INT_L_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_7_HSIOM
	#define ioss_0_port_13_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_INT_L_HSIOM ioss_0_port_13_pin_7_HSIOM
#define CYBSP_USB_INT_L_IRQ ioss_interrupts_gpio_13_IRQn
#define CYBSP_USB_DP_ENABLED 1U
#define CYBSP_USB_DP_PORT GPIO_PRT14
#define CYBSP_USB_DP_PIN 0U
#define CYBSP_USB_DP_NUM 0U
#define CYBSP_USB_DP_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_USB_DP_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_0_HSIOM
	#define ioss_0_port_14_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DP_HSIOM ioss_0_port_14_pin_0_HSIOM
#define CYBSP_USB_DP_IRQ ioss_interrupts_gpio_14_IRQn
#define CYBSP_USB_DM_ENABLED 1U
#define CYBSP_USB_DM_PORT GPIO_PRT14
#define CYBSP_USB_DM_PIN 1U
#define CYBSP_USB_DM_NUM 1U
#define CYBSP_USB_DM_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_USB_DM_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_1_HSIOM
	#define ioss_0_port_14_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DM_HSIOM ioss_0_port_14_pin_1_HSIOM
#define CYBSP_USB_DM_IRQ ioss_interrupts_gpio_14_IRQn
#define CYBSP_CSD_TX_ENABLED 1U
#define CYBSP_CSD_TX_PORT GPIO_PRT1
#define CYBSP_CSD_TX_PIN 0U
#define CYBSP_CSD_TX_NUM 0U
#define CYBSP_CSD_TX_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_TX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define CYBSP_CSD_TX_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_LED_GREEN_ENABLED 1U
#define CYBSP_LED_GREEN_PORT GPIO_PRT1
#define CYBSP_LED_GREEN_PIN 1U
#define CYBSP_LED_GREEN_NUM 1U
#define CYBSP_LED_GREEN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED_GREEN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
	#define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED_GREEN_HSIOM ioss_0_port_1_pin_1_HSIOM
#define CYBSP_LED_GREEN_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_WL_SECI_IN_ENABLED 1U
#define CYBSP_WL_SECI_IN_PORT GPIO_PRT1
#define CYBSP_WL_SECI_IN_PIN 2U
#define CYBSP_WL_SECI_IN_NUM 2U
#define CYBSP_WL_SECI_IN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_SECI_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
	#define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_SECI_IN_HSIOM ioss_0_port_1_pin_2_HSIOM
#define CYBSP_WL_SECI_IN_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_WL_FRAM_SYNC_ENABLED 1U
#define CYBSP_WL_FRAM_SYNC_PORT GPIO_PRT1
#define CYBSP_WL_FRAM_SYNC_PIN 3U
#define CYBSP_WL_FRAM_SYNC_NUM 3U
#define CYBSP_WL_FRAM_SYNC_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_FRAM_SYNC_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_3_HSIOM
	#define ioss_0_port_1_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_FRAM_SYNC_HSIOM ioss_0_port_1_pin_3_HSIOM
#define CYBSP_WL_FRAM_SYNC_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_WL_PRIORITY_ENABLED 1U
#define CYBSP_WL_PRIORITY_PORT GPIO_PRT1
#define CYBSP_WL_PRIORITY_PIN 4U
#define CYBSP_WL_PRIORITY_NUM 4U
#define CYBSP_WL_PRIORITY_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_PRIORITY_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
	#define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_PRIORITY_HSIOM ioss_0_port_1_pin_4_HSIOM
#define CYBSP_WL_PRIORITY_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_WL_SECI_OUT_ENABLED 1U
#define CYBSP_WL_SECI_OUT_PORT GPIO_PRT1
#define CYBSP_WL_SECI_OUT_PIN 5U
#define CYBSP_WL_SECI_OUT_NUM 5U
#define CYBSP_WL_SECI_OUT_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WL_SECI_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
	#define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_SECI_OUT_HSIOM ioss_0_port_1_pin_5_HSIOM
#define CYBSP_WL_SECI_OUT_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_WIFI_SDIO_D0_ENABLED 1U
#define CYBSP_WIFI_SDIO_D0_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_D0_PIN 0U
#define CYBSP_WIFI_SDIO_D0_NUM 0U
#define CYBSP_WIFI_SDIO_D0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_WIFI_SDIO_D0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_0_HSIOM
	#define ioss_0_port_2_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_D0_HSIOM ioss_0_port_2_pin_0_HSIOM
#define CYBSP_WIFI_SDIO_D0_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_SDIO_D1_ENABLED 1U
#define CYBSP_WIFI_SDIO_D1_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_D1_PIN 1U
#define CYBSP_WIFI_SDIO_D1_NUM 1U
#define CYBSP_WIFI_SDIO_D1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_WIFI_SDIO_D1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_1_HSIOM
	#define ioss_0_port_2_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_D1_HSIOM ioss_0_port_2_pin_1_HSIOM
#define CYBSP_WIFI_SDIO_D1_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_SDIO_D2_ENABLED 1U
#define CYBSP_WIFI_SDIO_D2_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_D2_PIN 2U
#define CYBSP_WIFI_SDIO_D2_NUM 2U
#define CYBSP_WIFI_SDIO_D2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_WIFI_SDIO_D2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_2_HSIOM
	#define ioss_0_port_2_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_D2_HSIOM ioss_0_port_2_pin_2_HSIOM
#define CYBSP_WIFI_SDIO_D2_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_SDIO_D3_ENABLED 1U
#define CYBSP_WIFI_SDIO_D3_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_D3_PIN 3U
#define CYBSP_WIFI_SDIO_D3_NUM 3U
#define CYBSP_WIFI_SDIO_D3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_WIFI_SDIO_D3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_3_HSIOM
	#define ioss_0_port_2_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_D3_HSIOM ioss_0_port_2_pin_3_HSIOM
#define CYBSP_WIFI_SDIO_D3_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_SDIO_CMD_ENABLED 1U
#define CYBSP_WIFI_SDIO_CMD_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_CMD_PIN 4U
#define CYBSP_WIFI_SDIO_CMD_NUM 4U
#define CYBSP_WIFI_SDIO_CMD_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_WIFI_SDIO_CMD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
	#define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_CMD_HSIOM ioss_0_port_2_pin_4_HSIOM
#define CYBSP_WIFI_SDIO_CMD_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_SDIO_CLK_ENABLED 1U
#define CYBSP_WIFI_SDIO_CLK_PORT GPIO_PRT2
#define CYBSP_WIFI_SDIO_CLK_PIN 5U
#define CYBSP_WIFI_SDIO_CLK_NUM 5U
#define CYBSP_WIFI_SDIO_CLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WIFI_SDIO_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
	#define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_SDIO_CLK_HSIOM ioss_0_port_2_pin_5_HSIOM
#define CYBSP_WIFI_SDIO_CLK_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_WL_REG_ON_ENABLED 1U
#define CYBSP_WIFI_WL_REG_ON_PORT GPIO_PRT2
#define CYBSP_WIFI_WL_REG_ON_PIN 6U
#define CYBSP_WIFI_WL_REG_ON_NUM 6U
#define CYBSP_WIFI_WL_REG_ON_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WIFI_WL_REG_ON_INIT_DRIVESTATE 0
#ifndef ioss_0_port_2_pin_6_HSIOM
	#define ioss_0_port_2_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_WL_REG_ON_HSIOM ioss_0_port_2_pin_6_HSIOM
#define CYBSP_WIFI_WL_REG_ON_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_WIFI_HOST_WAKE_ENABLED 1U
#define CYBSP_WIFI_HOST_WAKE_PORT GPIO_PRT2
#define CYBSP_WIFI_HOST_WAKE_PIN 7U
#define CYBSP_WIFI_HOST_WAKE_NUM 7U
#define CYBSP_WIFI_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WIFI_HOST_WAKE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_7_HSIOM
	#define ioss_0_port_2_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_HOST_WAKE_HSIOM ioss_0_port_2_pin_7_HSIOM
#define CYBSP_WIFI_HOST_WAKE_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_BT_UART_RX_ENABLED 1U
#define CYBSP_BT_UART_RX_PORT GPIO_PRT3
#define CYBSP_BT_UART_RX_PIN 0U
#define CYBSP_BT_UART_RX_NUM 0U
#define CYBSP_BT_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_0_HSIOM
	#define ioss_0_port_3_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_RX_HSIOM ioss_0_port_3_pin_0_HSIOM
#define CYBSP_BT_UART_RX_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_UART_TX_ENABLED 1U
#define CYBSP_BT_UART_TX_PORT GPIO_PRT3
#define CYBSP_BT_UART_TX_PIN 1U
#define CYBSP_BT_UART_TX_NUM 1U
#define CYBSP_BT_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_1_HSIOM
	#define ioss_0_port_3_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_TX_HSIOM ioss_0_port_3_pin_1_HSIOM
#define CYBSP_BT_UART_TX_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_UART_RTS_ENABLED 1U
#define CYBSP_BT_UART_RTS_PORT GPIO_PRT3
#define CYBSP_BT_UART_RTS_PIN 2U
#define CYBSP_BT_UART_RTS_NUM 2U
#define CYBSP_BT_UART_RTS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_UART_RTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_2_HSIOM
	#define ioss_0_port_3_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_RTS_HSIOM ioss_0_port_3_pin_2_HSIOM
#define CYBSP_BT_UART_RTS_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_UART_CTS_ENABLED 1U
#define CYBSP_BT_UART_CTS_PORT GPIO_PRT3
#define CYBSP_BT_UART_CTS_PIN 3U
#define CYBSP_BT_UART_CTS_NUM 3U
#define CYBSP_BT_UART_CTS_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_UART_CTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
	#define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_CTS_HSIOM ioss_0_port_3_pin_3_HSIOM
#define CYBSP_BT_UART_CTS_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_REG_ON_ENABLED 1U
#define CYBSP_BT_REG_ON_PORT GPIO_PRT3
#define CYBSP_BT_REG_ON_PIN 4U
#define CYBSP_BT_REG_ON_NUM 4U
#define CYBSP_BT_REG_ON_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define CYBSP_BT_REG_ON_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_REG_ON_HSIOM ioss_0_port_3_pin_4_HSIOM
#define CYBSP_BT_REG_ON_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_HOST_WAKE_ENABLED 1U
#define CYBSP_BT_HOST_WAKE_PORT GPIO_PRT3
#define CYBSP_BT_HOST_WAKE_PIN 5U
#define CYBSP_BT_HOST_WAKE_NUM 5U
#define CYBSP_BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_5_HSIOM
	#define ioss_0_port_3_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_HOST_WAKE_HSIOM ioss_0_port_3_pin_5_HSIOM
#define CYBSP_BT_HOST_WAKE_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_DEVICE_WAKE_ENABLED 1U
#define CYBSP_BT_DEVICE_WAKE_PORT GPIO_PRT4
#define CYBSP_BT_DEVICE_WAKE_PIN 0U
#define CYBSP_BT_DEVICE_WAKE_NUM 0U
#define CYBSP_BT_DEVICE_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_DEVICE_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_0_HSIOM
	#define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_DEVICE_WAKE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define CYBSP_BT_DEVICE_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#define CYBSP_BT_RST_ENABLED 1U
#define CYBSP_BT_RST_PORT GPIO_PRT4
#define CYBSP_BT_RST_PIN 1U
#define CYBSP_BT_RST_NUM 1U
#define CYBSP_BT_RST_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_RST_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_1_HSIOM
	#define ioss_0_port_4_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_RST_HSIOM ioss_0_port_4_pin_1_HSIOM
#define CYBSP_BT_RST_IRQ ioss_interrupts_gpio_4_IRQn
#define CYBSP_EZI2C_SCL_ENABLED 1U
#define CYBSP_EZI2C_SCL_PORT GPIO_PRT6
#define CYBSP_EZI2C_SCL_PIN 0U
#define CYBSP_EZI2C_SCL_NUM 0U
#define CYBSP_EZI2C_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_EZI2C_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_0_HSIOM
	#define ioss_0_port_6_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_EZI2C_SCL_HSIOM ioss_0_port_6_pin_0_HSIOM
#define CYBSP_EZI2C_SCL_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_EZI2C_SDA_ENABLED 1U
#define CYBSP_EZI2C_SDA_PORT GPIO_PRT6
#define CYBSP_EZI2C_SDA_PIN 1U
#define CYBSP_EZI2C_SDA_NUM 1U
#define CYBSP_EZI2C_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_EZI2C_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
	#define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_EZI2C_SDA_HSIOM ioss_0_port_6_pin_1_HSIOM
#define CYBSP_EZI2C_SDA_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_SWO_ENABLED 1U
#define CYBSP_SWO_PORT GPIO_PRT6
#define CYBSP_SWO_PIN 4U
#define CYBSP_SWO_NUM 4U
#define CYBSP_SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define CYBSP_SWO_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT6
#define CYBSP_SWDIO_PIN 6U
#define CYBSP_SWDIO_NUM 6U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_SWCLK_ENABLED 1U
#define CYBSP_SWCLK_PORT GPIO_PRT6
#define CYBSP_SWCLK_PIN 7U
#define CYBSP_SWCLK_NUM 7U
#define CYBSP_SWCLK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define CYBSP_SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWCLK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define CYBSP_SWCLK_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_TRACECLK_ENABLED 1U
#define CYBSP_TRACECLK_PORT GPIO_PRT7
#define CYBSP_TRACECLK_PIN 0U
#define CYBSP_TRACECLK_NUM 0U
#define CYBSP_TRACECLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACECLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_0_HSIOM
	#define ioss_0_port_7_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACECLK_HSIOM ioss_0_port_7_pin_0_HSIOM
#define CYBSP_TRACECLK_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_CINTA_ENABLED 1U
#define CYBSP_CINTA_PORT GPIO_PRT7
#define CYBSP_CINTA_PIN 1U
#define CYBSP_CINTA_NUM 1U
#define CYBSP_CINTA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINTA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINTA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CINTA_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_CINTB_ENABLED 1U
#define CYBSP_CINTB_PORT GPIO_PRT7
#define CYBSP_CINTB_PIN 2U
#define CYBSP_CINTB_NUM 2U
#define CYBSP_CINTB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINTB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINTB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CINTB_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_CMOD_ENABLED 1U
#define CYBSP_CMOD_PORT GPIO_PRT7
#define CYBSP_CMOD_PIN 7U
#define CYBSP_CMOD_NUM 7U
#define CYBSP_CMOD_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CMOD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_7_HSIOM
	#define ioss_0_port_7_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CMOD_HSIOM ioss_0_port_7_pin_7_HSIOM
#define CYBSP_CMOD_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_CSD_BTN0_ENABLED 1U
#define CYBSP_CSD_BTN0_PORT GPIO_PRT8
#define CYBSP_CSD_BTN0_PIN 1U
#define CYBSP_CSD_BTN0_NUM 1U
#define CYBSP_CSD_BTN0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN0_HSIOM ioss_0_port_8_pin_1_HSIOM
#define CYBSP_CSD_BTN0_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_BTN1_ENABLED 1U
#define CYBSP_CSD_BTN1_PORT GPIO_PRT8
#define CYBSP_CSD_BTN1_PIN 2U
#define CYBSP_CSD_BTN1_NUM 2U
#define CYBSP_CSD_BTN1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_2_HSIOM
	#define ioss_0_port_8_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN1_HSIOM ioss_0_port_8_pin_2_HSIOM
#define CYBSP_CSD_BTN1_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_SLD0_ENABLED 1U
#define CYBSP_CSD_SLD0_PORT GPIO_PRT8
#define CYBSP_CSD_SLD0_PIN 3U
#define CYBSP_CSD_SLD0_NUM 3U
#define CYBSP_CSD_SLD0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD0_HSIOM ioss_0_port_8_pin_3_HSIOM
#define CYBSP_CSD_SLD0_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_SLD1_ENABLED 1U
#define CYBSP_CSD_SLD1_PORT GPIO_PRT8
#define CYBSP_CSD_SLD1_PIN 4U
#define CYBSP_CSD_SLD1_NUM 4U
#define CYBSP_CSD_SLD1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_4_HSIOM
	#define ioss_0_port_8_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD1_HSIOM ioss_0_port_8_pin_4_HSIOM
#define CYBSP_CSD_SLD1_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_SLD2_ENABLED 1U
#define CYBSP_CSD_SLD2_PORT GPIO_PRT8
#define CYBSP_CSD_SLD2_PIN 5U
#define CYBSP_CSD_SLD2_NUM 5U
#define CYBSP_CSD_SLD2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_5_HSIOM
	#define ioss_0_port_8_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD2_HSIOM ioss_0_port_8_pin_5_HSIOM
#define CYBSP_CSD_SLD2_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_SLD3_ENABLED 1U
#define CYBSP_CSD_SLD3_PORT GPIO_PRT8
#define CYBSP_CSD_SLD3_PIN 6U
#define CYBSP_CSD_SLD3_NUM 6U
#define CYBSP_CSD_SLD3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_6_HSIOM
	#define ioss_0_port_8_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD3_HSIOM ioss_0_port_8_pin_6_HSIOM
#define CYBSP_CSD_SLD3_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_CSD_SLD4_ENABLED 1U
#define CYBSP_CSD_SLD4_PORT GPIO_PRT8
#define CYBSP_CSD_SLD4_PIN 7U
#define CYBSP_CSD_SLD4_NUM 7U
#define CYBSP_CSD_SLD4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_7_HSIOM
	#define ioss_0_port_8_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD4_HSIOM ioss_0_port_8_pin_7_HSIOM
#define CYBSP_CSD_SLD4_IRQ ioss_interrupts_gpio_8_IRQn
#define CYBSP_TRACEDATA3_ENABLED 1U
#define CYBSP_TRACEDATA3_PORT GPIO_PRT9
#define CYBSP_TRACEDATA3_PIN 0U
#define CYBSP_TRACEDATA3_NUM 0U
#define CYBSP_TRACEDATA3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA3_HSIOM ioss_0_port_9_pin_0_HSIOM
#define CYBSP_TRACEDATA3_IRQ ioss_interrupts_gpio_9_IRQn
#define CYBSP_TRACEDATA2_ENABLED 1U
#define CYBSP_TRACEDATA2_PORT GPIO_PRT9
#define CYBSP_TRACEDATA2_PIN 1U
#define CYBSP_TRACEDATA2_NUM 1U
#define CYBSP_TRACEDATA2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_1_HSIOM
	#define ioss_0_port_9_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA2_HSIOM ioss_0_port_9_pin_1_HSIOM
#define CYBSP_TRACEDATA2_IRQ ioss_interrupts_gpio_9_IRQn
#define CYBSP_TRACEDATA1_ENABLED 1U
#define CYBSP_TRACEDATA1_PORT GPIO_PRT9
#define CYBSP_TRACEDATA1_PIN 2U
#define CYBSP_TRACEDATA1_NUM 2U
#define CYBSP_TRACEDATA1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_2_HSIOM
	#define ioss_0_port_9_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA1_HSIOM ioss_0_port_9_pin_2_HSIOM
#define CYBSP_TRACEDATA1_IRQ ioss_interrupts_gpio_9_IRQn
#define CYBSP_TRACEDATA0_ENABLED 1U
#define CYBSP_TRACEDATA0_PORT GPIO_PRT9
#define CYBSP_TRACEDATA0_PIN 3U
#define CYBSP_TRACEDATA0_NUM 3U
#define CYBSP_TRACEDATA0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_3_HSIOM
	#define ioss_0_port_9_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA0_HSIOM ioss_0_port_9_pin_3_HSIOM
#define CYBSP_TRACEDATA0_IRQ ioss_interrupts_gpio_9_IRQn

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED_RED_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SW6_config;
extern const cy_stc_gpio_pin_config_t CYBSP_ROW6_SPI_MOSI_config;
extern const cy_stc_gpio_pin_config_t CYBSP_COL8_SPI_MISO_config;
extern const cy_stc_gpio_pin_config_t CYBSP_ROW7_SPI_CLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_COL7_SPI_CS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BAT_MON_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED_BLUE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SCK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO4_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO5_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DEV_VBUS_DET_config;
extern const cy_stc_gpio_pin_config_t CYBSP_USB_HOST_EN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_USB_INT_L_config;
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DP_config;
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DM_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED_GREEN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_SECI_IN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_FRAM_SYNC_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_PRIORITY_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WL_SECI_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_D3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CMD_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_SDIO_CLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_WL_REG_ON_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_REG_ON_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_RST_config;
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SCL_config;
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SDA_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWO_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_TRACECLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CINTA_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CINTB_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CMOD_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config;
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA0_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
