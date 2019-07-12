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
#define CYBSP_SW2_ENABLED 1U
#define CYBSP_SW2_PORT GPIO_PRT0
#define CYBSP_SW2_PIN 4U
#define CYBSP_SW2_NUM 4U
#define CYBSP_SW2_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SW2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
	#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SW2_HSIOM ioss_0_port_0_pin_4_HSIOM
#define CYBSP_SW2_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_LED5_RGB_G_ENABLED 1U
#define CYBSP_LED5_RGB_G_PORT GPIO_PRT0
#define CYBSP_LED5_RGB_G_PIN 5U
#define CYBSP_LED5_RGB_G_NUM 5U
#define CYBSP_LED5_RGB_G_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED5_RGB_G_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_5_HSIOM
	#define ioss_0_port_0_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED5_RGB_G_HSIOM ioss_0_port_0_pin_5_HSIOM
#define CYBSP_LED5_RGB_G_IRQ ioss_interrupts_gpio_0_IRQn
#define CYBSP_LED9_ENABLED 1U
#define CYBSP_LED9_PORT GPIO_PRT11
#define CYBSP_LED9_PIN 1U
#define CYBSP_LED9_NUM 1U
#define CYBSP_LED9_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED9_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
	#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED9_HSIOM ioss_0_port_11_pin_1_HSIOM
#define CYBSP_LED9_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_SS0_ENABLED 1U
#define CYBSP_QSPI_SS0_PORT GPIO_PRT11
#define CYBSP_QSPI_SS0_PIN 2U
#define CYBSP_QSPI_SS0_NUM 2U
#define CYBSP_QSPI_SS0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SS0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SS0_HSIOM ioss_0_port_11_pin_2_HSIOM
#define CYBSP_QSPI_SS0_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_DATA3_ENABLED 1U
#define CYBSP_QSPI_DATA3_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA3_PIN 3U
#define CYBSP_QSPI_DATA3_NUM 3U
#define CYBSP_QSPI_DATA3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define CYBSP_QSPI_DATA3_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_DATA2_ENABLED 1U
#define CYBSP_QSPI_DATA2_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA2_PIN 4U
#define CYBSP_QSPI_DATA2_NUM 4U
#define CYBSP_QSPI_DATA2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define CYBSP_QSPI_DATA2_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_DATA1_ENABLED 1U
#define CYBSP_QSPI_DATA1_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA1_PIN 5U
#define CYBSP_QSPI_DATA1_NUM 5U
#define CYBSP_QSPI_DATA1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define CYBSP_QSPI_DATA1_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_DATA0_ENABLED 1U
#define CYBSP_QSPI_DATA0_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA0_PIN 6U
#define CYBSP_QSPI_DATA0_NUM 6U
#define CYBSP_QSPI_DATA0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define CYBSP_QSPI_DATA0_IRQ ioss_interrupts_gpio_11_IRQn
#define CYBSP_QSPI_SPI_CLOCK_ENABLED 1U
#define CYBSP_QSPI_SPI_CLOCK_PORT GPIO_PRT11
#define CYBSP_QSPI_SPI_CLOCK_PIN 7U
#define CYBSP_QSPI_SPI_CLOCK_NUM 7U
#define CYBSP_QSPI_SPI_CLOCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SPI_CLOCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SPI_CLOCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define CYBSP_QSPI_SPI_CLOCK_IRQ ioss_interrupts_gpio_11_IRQn
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
#define CYBSP_LED5_RGB_R_ENABLED 1U
#define CYBSP_LED5_RGB_R_PORT GPIO_PRT1
#define CYBSP_LED5_RGB_R_PIN 1U
#define CYBSP_LED5_RGB_R_NUM 1U
#define CYBSP_LED5_RGB_R_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED5_RGB_R_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
	#define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED5_RGB_R_HSIOM ioss_0_port_1_pin_1_HSIOM
#define CYBSP_LED5_RGB_R_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_SW4_ENABLED 1U
#define CYBSP_SW4_PORT GPIO_PRT1
#define CYBSP_SW4_PIN 4U
#define CYBSP_SW4_NUM 4U
#define CYBSP_SW4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_SW4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
	#define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SW4_HSIOM ioss_0_port_1_pin_4_HSIOM
#define CYBSP_SW4_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_LED8_ENABLED 1U
#define CYBSP_LED8_PORT GPIO_PRT1
#define CYBSP_LED8_PIN 5U
#define CYBSP_LED8_NUM 5U
#define CYBSP_LED8_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED8_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
	#define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED8_HSIOM ioss_0_port_1_pin_5_HSIOM
#define CYBSP_LED8_IRQ ioss_interrupts_gpio_1_IRQn
#define CYBSP_SDHC0_DAT0_ENABLED 1U
#define CYBSP_SDHC0_DAT0_PORT GPIO_PRT2
#define CYBSP_SDHC0_DAT0_PIN 0U
#define CYBSP_SDHC0_DAT0_NUM 0U
#define CYBSP_SDHC0_DAT0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_DAT0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_0_HSIOM
	#define ioss_0_port_2_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_DAT0_HSIOM ioss_0_port_2_pin_0_HSIOM
#define CYBSP_SDHC0_DAT0_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_SDHC0_DAT1_ENABLED 1U
#define CYBSP_SDHC0_DAT1_PORT GPIO_PRT2
#define CYBSP_SDHC0_DAT1_PIN 1U
#define CYBSP_SDHC0_DAT1_NUM 1U
#define CYBSP_SDHC0_DAT1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_DAT1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_1_HSIOM
	#define ioss_0_port_2_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_DAT1_HSIOM ioss_0_port_2_pin_1_HSIOM
#define CYBSP_SDHC0_DAT1_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_SDHC0_DAT2_ENABLED 1U
#define CYBSP_SDHC0_DAT2_PORT GPIO_PRT2
#define CYBSP_SDHC0_DAT2_PIN 2U
#define CYBSP_SDHC0_DAT2_NUM 2U
#define CYBSP_SDHC0_DAT2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_DAT2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_2_HSIOM
	#define ioss_0_port_2_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_DAT2_HSIOM ioss_0_port_2_pin_2_HSIOM
#define CYBSP_SDHC0_DAT2_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_SDHC0_DAT3_ENABLED 1U
#define CYBSP_SDHC0_DAT3_PORT GPIO_PRT2
#define CYBSP_SDHC0_DAT3_PIN 3U
#define CYBSP_SDHC0_DAT3_NUM 3U
#define CYBSP_SDHC0_DAT3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_DAT3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_3_HSIOM
	#define ioss_0_port_2_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_DAT3_HSIOM ioss_0_port_2_pin_3_HSIOM
#define CYBSP_SDHC0_DAT3_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_SDHC0_CMD_ENABLED 1U
#define CYBSP_SDHC0_CMD_PORT GPIO_PRT2
#define CYBSP_SDHC0_CMD_PIN 4U
#define CYBSP_SDHC0_CMD_NUM 4U
#define CYBSP_SDHC0_CMD_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_CMD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
	#define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_CMD_HSIOM ioss_0_port_2_pin_4_HSIOM
#define CYBSP_SDHC0_CMD_IRQ ioss_interrupts_gpio_2_IRQn
#define CYBSP_SDHC0_CLK_ENABLED 1U
#define CYBSP_SDHC0_CLK_PORT GPIO_PRT2
#define CYBSP_SDHC0_CLK_PIN 5U
#define CYBSP_SDHC0_CLK_NUM 5U
#define CYBSP_SDHC0_CLK_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SDHC0_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
	#define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SDHC0_CLK_HSIOM ioss_0_port_2_pin_5_HSIOM
#define CYBSP_SDHC0_CLK_IRQ ioss_interrupts_gpio_2_IRQn
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
#define CYBSP_BT_POWER_ENABLED 1U
#define CYBSP_BT_POWER_PORT GPIO_PRT3
#define CYBSP_BT_POWER_PIN 4U
#define CYBSP_BT_POWER_NUM 4U
#define CYBSP_BT_POWER_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define CYBSP_BT_POWER_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_POWER_HSIOM ioss_0_port_3_pin_4_HSIOM
#define CYBSP_BT_POWER_IRQ ioss_interrupts_gpio_3_IRQn
#define CYBSP_BT_HOST_WAKE_ENABLED 1U
#define CYBSP_BT_HOST_WAKE_PORT GPIO_PRT3
#define CYBSP_BT_HOST_WAKE_PIN 5U
#define CYBSP_BT_HOST_WAKE_NUM 5U
#define CYBSP_BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_ANALOG
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
#define CYBSP_DEBUG_UART_RX_ENABLED 1U
#define CYBSP_DEBUG_UART_RX_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_RX_PIN 0U
#define CYBSP_DEBUG_UART_RX_NUM 0U
#define CYBSP_DEBUG_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_DEBUG_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
	#define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_RX_HSIOM ioss_0_port_5_pin_0_HSIOM
#define CYBSP_DEBUG_UART_RX_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_DEBUG_UART_TX_ENABLED 1U
#define CYBSP_DEBUG_UART_TX_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_TX_PIN 1U
#define CYBSP_DEBUG_UART_TX_NUM 1U
#define CYBSP_DEBUG_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_DEBUG_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
	#define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_TX_HSIOM ioss_0_port_5_pin_1_HSIOM
#define CYBSP_DEBUG_UART_TX_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_DEBUG_UART_RTS_ENABLED 1U
#define CYBSP_DEBUG_UART_RTS_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_RTS_PIN 2U
#define CYBSP_DEBUG_UART_RTS_NUM 2U
#define CYBSP_DEBUG_UART_RTS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_DEBUG_UART_RTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_2_HSIOM
	#define ioss_0_port_5_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_RTS_HSIOM ioss_0_port_5_pin_2_HSIOM
#define CYBSP_DEBUG_UART_RTS_IRQ ioss_interrupts_gpio_5_IRQn
#define CYBSP_DEBUG_UART_CTS_ENABLED 1U
#define CYBSP_DEBUG_UART_CTS_PORT GPIO_PRT5
#define CYBSP_DEBUG_UART_CTS_PIN 3U
#define CYBSP_DEBUG_UART_CTS_NUM 3U
#define CYBSP_DEBUG_UART_CTS_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_DEBUG_UART_CTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_3_HSIOM
	#define ioss_0_port_5_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_DEBUG_UART_CTS_HSIOM ioss_0_port_5_pin_3_HSIOM
#define CYBSP_DEBUG_UART_CTS_IRQ ioss_interrupts_gpio_5_IRQn
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
#define CYBSP_SWDCK_ENABLED 1U
#define CYBSP_SWDCK_PORT GPIO_PRT6
#define CYBSP_SWDCK_PIN 7U
#define CYBSP_SWDCK_NUM 7U
#define CYBSP_SWDCK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define CYBSP_SWDCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDCK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define CYBSP_SWDCK_IRQ ioss_interrupts_gpio_6_IRQn
#define CYBSP_CINA_ENABLED 1U
#define CYBSP_CINA_PORT GPIO_PRT7
#define CYBSP_CINA_PIN 1U
#define CYBSP_CINA_NUM 1U
#define CYBSP_CINA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CINA_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_CINB_ENABLED 1U
#define CYBSP_CINB_PORT GPIO_PRT7
#define CYBSP_CINB_PIN 2U
#define CYBSP_CINB_NUM 2U
#define CYBSP_CINB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CINB_IRQ ioss_interrupts_gpio_7_IRQn
#define CYBSP_LED5_RGB_B_ENABLED 1U
#define CYBSP_LED5_RGB_B_PORT GPIO_PRT7
#define CYBSP_LED5_RGB_B_PIN 3U
#define CYBSP_LED5_RGB_B_NUM 3U
#define CYBSP_LED5_RGB_B_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED5_RGB_B_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_3_HSIOM
	#define ioss_0_port_7_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED5_RGB_B_HSIOM ioss_0_port_7_pin_3_HSIOM
#define CYBSP_LED5_RGB_B_IRQ ioss_interrupts_gpio_7_IRQn
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

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SW2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED5_RGB_G_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED9_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SPI_CLOCK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED5_RGB_R_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SW4_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED8_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_DAT0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_DAT1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_DAT2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_DAT3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_CMD_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SDHC0_CLK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_POWER_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_CTS_config;
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SCL_config;
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SDA_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWO_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
extern const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CINA_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CINB_config;
extern const cy_stc_gpio_pin_config_t CYBSP_LED5_RGB_B_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CMOD_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD0_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD1_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD2_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD3_config;
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
