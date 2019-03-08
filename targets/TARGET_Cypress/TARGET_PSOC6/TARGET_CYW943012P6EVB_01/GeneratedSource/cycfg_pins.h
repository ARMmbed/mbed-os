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

#define WCO_IN_PORT GPIO_PRT0
#define WCO_IN_PIN 0U
#define WCO_IN_NUM 0U
#define WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#define WCO_OUT_PORT GPIO_PRT0
#define WCO_OUT_PIN 1U
#define WCO_OUT_NUM 1U
#define WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#define LED_RED_PORT GPIO_PRT0
#define LED_RED_PIN 3U
#define LED_RED_NUM 3U
#define LED_RED_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LED_RED_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_3_HSIOM
	#define ioss_0_port_0_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_RED_HSIOM ioss_0_port_0_pin_3_HSIOM
#define LED_RED_IRQ ioss_interrupts_gpio_0_IRQn
#define SW6_PORT GPIO_PRT0
#define SW6_PIN 4U
#define SW6_NUM 4U
#define SW6_DRIVEMODE CY_GPIO_DM_PULLUP
#define SW6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
	#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SW6_HSIOM ioss_0_port_0_pin_4_HSIOM
#define SW6_IRQ ioss_interrupts_gpio_0_IRQn
#define LED_BLUE_PORT GPIO_PRT10
#define LED_BLUE_PIN 6U
#define LED_BLUE_NUM 6U
#define LED_BLUE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LED_BLUE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_6_HSIOM
	#define ioss_0_port_10_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_BLUE_HSIOM ioss_0_port_10_pin_6_HSIOM
#define LED_BLUE_IRQ ioss_interrupts_gpio_10_IRQn
#define WL_UART_RX_PORT GPIO_PRT11
#define WL_UART_RX_PIN 0U
#define WL_UART_RX_NUM 0U
#define WL_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define WL_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_0_HSIOM
	#define ioss_0_port_11_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define WL_UART_RX_HSIOM ioss_0_port_11_pin_0_HSIOM
#define WL_UART_RX_IRQ ioss_interrupts_gpio_11_IRQn
#define WL_UART_TX_PORT GPIO_PRT11
#define WL_UART_TX_PIN 1U
#define WL_UART_TX_NUM 1U
#define WL_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define WL_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
	#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define WL_UART_TX_HSIOM ioss_0_port_11_pin_1_HSIOM
#define WL_UART_TX_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_SS0_PORT GPIO_PRT11
#define QSPI_SS0_PIN 2U
#define QSPI_SS0_NUM 2U
#define QSPI_SS0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define QSPI_SS0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_SS0_HSIOM ioss_0_port_11_pin_2_HSIOM
#define QSPI_SS0_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_DATA3_PORT GPIO_PRT11
#define QSPI_DATA3_PIN 3U
#define QSPI_DATA3_NUM 3U
#define QSPI_DATA3_DRIVEMODE CY_GPIO_DM_STRONG
#define QSPI_DATA3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_DATA3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define QSPI_DATA3_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_DATA2_PORT GPIO_PRT11
#define QSPI_DATA2_PIN 4U
#define QSPI_DATA2_NUM 4U
#define QSPI_DATA2_DRIVEMODE CY_GPIO_DM_STRONG
#define QSPI_DATA2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_DATA2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define QSPI_DATA2_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_DATA1_PORT GPIO_PRT11
#define QSPI_DATA1_PIN 5U
#define QSPI_DATA1_NUM 5U
#define QSPI_DATA1_DRIVEMODE CY_GPIO_DM_STRONG
#define QSPI_DATA1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_DATA1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define QSPI_DATA1_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_DATA0_PORT GPIO_PRT11
#define QSPI_DATA0_PIN 6U
#define QSPI_DATA0_NUM 6U
#define QSPI_DATA0_DRIVEMODE CY_GPIO_DM_STRONG
#define QSPI_DATA0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_DATA0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define QSPI_DATA0_IRQ ioss_interrupts_gpio_11_IRQn
#define QSPI_SPI_CLOCK_PORT GPIO_PRT11
#define QSPI_SPI_CLOCK_PIN 7U
#define QSPI_SPI_CLOCK_NUM 7U
#define QSPI_SPI_CLOCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define QSPI_SPI_CLOCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define QSPI_SPI_CLOCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define QSPI_SPI_CLOCK_IRQ ioss_interrupts_gpio_11_IRQn
#define ioss_0_port_12_pin_6_PORT GPIO_PRT12
#define ioss_0_port_12_pin_6_PIN 6U
#define ioss_0_port_12_pin_6_NUM 6U
#define ioss_0_port_12_pin_6_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_6_HSIOM
	#define ioss_0_port_12_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_6_IRQ ioss_interrupts_gpio_12_IRQn
#define ioss_0_port_12_pin_7_PORT GPIO_PRT12
#define ioss_0_port_12_pin_7_PIN 7U
#define ioss_0_port_12_pin_7_NUM 7U
#define ioss_0_port_12_pin_7_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_12_pin_7_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_7_HSIOM
	#define ioss_0_port_12_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_12_pin_7_IRQ ioss_interrupts_gpio_12_IRQn
#define UART_RX_PORT GPIO_PRT13
#define UART_RX_PIN 0U
#define UART_RX_NUM 0U
#define UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_0_HSIOM
	#define ioss_0_port_13_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define UART_RX_HSIOM ioss_0_port_13_pin_0_HSIOM
#define UART_RX_IRQ ioss_interrupts_gpio_13_IRQn
#define UART_TX_PORT GPIO_PRT13
#define UART_TX_PIN 1U
#define UART_TX_NUM 1U
#define UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_1_HSIOM
	#define ioss_0_port_13_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define UART_TX_HSIOM ioss_0_port_13_pin_1_HSIOM
#define UART_TX_IRQ ioss_interrupts_gpio_13_IRQn
#define LED9_PORT GPIO_PRT13
#define LED9_PIN 7U
#define LED9_NUM 7U
#define LED9_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LED9_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_7_HSIOM
	#define ioss_0_port_13_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define LED9_HSIOM ioss_0_port_13_pin_7_HSIOM
#define LED9_IRQ ioss_interrupts_gpio_13_IRQn
#define ioss_0_port_14_pin_0_PORT GPIO_PRT14
#define ioss_0_port_14_pin_0_PIN 0U
#define ioss_0_port_14_pin_0_NUM 0U
#define ioss_0_port_14_pin_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_14_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_0_HSIOM
	#define ioss_0_port_14_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_14_pin_0_IRQ ioss_interrupts_gpio_14_IRQn
#define ioss_0_port_14_pin_1_PORT GPIO_PRT14
#define ioss_0_port_14_pin_1_PIN 1U
#define ioss_0_port_14_pin_1_NUM 1U
#define ioss_0_port_14_pin_1_DRIVEMODE CY_GPIO_DM_ANALOG
#define ioss_0_port_14_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_1_HSIOM
	#define ioss_0_port_14_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_14_pin_1_IRQ ioss_interrupts_gpio_14_IRQn
#define CSD_TX_PORT GPIO_PRT1
#define CSD_TX_PIN 0U
#define CSD_TX_NUM 0U
#define CSD_TX_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_TX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define CSD_TX_IRQ ioss_interrupts_gpio_1_IRQn
#define LED_GREEN_PORT GPIO_PRT1
#define LED_GREEN_PIN 1U
#define LED_GREEN_NUM 1U
#define LED_GREEN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LED_GREEN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
	#define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define LED_GREEN_HSIOM ioss_0_port_1_pin_1_HSIOM
#define LED_GREEN_IRQ ioss_interrupts_gpio_1_IRQn
#define LED8_PORT GPIO_PRT1
#define LED8_PIN 5U
#define LED8_NUM 5U
#define LED8_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define LED8_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
	#define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define LED8_HSIOM ioss_0_port_1_pin_5_HSIOM
#define LED8_IRQ ioss_interrupts_gpio_1_IRQn
#define SDHC0_DAT0_PORT GPIO_PRT2
#define SDHC0_DAT0_PIN 0U
#define SDHC0_DAT0_NUM 0U
#define SDHC0_DAT0_DRIVEMODE CY_GPIO_DM_STRONG
#define SDHC0_DAT0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_0_HSIOM
	#define ioss_0_port_2_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_DAT0_HSIOM ioss_0_port_2_pin_0_HSIOM
#define SDHC0_DAT0_IRQ ioss_interrupts_gpio_2_IRQn
#define SDHC0_DAT1_PORT GPIO_PRT2
#define SDHC0_DAT1_PIN 1U
#define SDHC0_DAT1_NUM 1U
#define SDHC0_DAT1_DRIVEMODE CY_GPIO_DM_STRONG
#define SDHC0_DAT1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_1_HSIOM
	#define ioss_0_port_2_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_DAT1_HSIOM ioss_0_port_2_pin_1_HSIOM
#define SDHC0_DAT1_IRQ ioss_interrupts_gpio_2_IRQn
#define SDHC0_DAT2_PORT GPIO_PRT2
#define SDHC0_DAT2_PIN 2U
#define SDHC0_DAT2_NUM 2U
#define SDHC0_DAT2_DRIVEMODE CY_GPIO_DM_STRONG
#define SDHC0_DAT2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_2_HSIOM
	#define ioss_0_port_2_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_DAT2_HSIOM ioss_0_port_2_pin_2_HSIOM
#define SDHC0_DAT2_IRQ ioss_interrupts_gpio_2_IRQn
#define SDHC0_DAT3_PORT GPIO_PRT2
#define SDHC0_DAT3_PIN 3U
#define SDHC0_DAT3_NUM 3U
#define SDHC0_DAT3_DRIVEMODE CY_GPIO_DM_STRONG
#define SDHC0_DAT3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_3_HSIOM
	#define ioss_0_port_2_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_DAT3_HSIOM ioss_0_port_2_pin_3_HSIOM
#define SDHC0_DAT3_IRQ ioss_interrupts_gpio_2_IRQn
#define SDHC0_CMD_PORT GPIO_PRT2
#define SDHC0_CMD_PIN 4U
#define SDHC0_CMD_NUM 4U
#define SDHC0_CMD_DRIVEMODE CY_GPIO_DM_STRONG
#define SDHC0_CMD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
	#define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_CMD_HSIOM ioss_0_port_2_pin_4_HSIOM
#define SDHC0_CMD_IRQ ioss_interrupts_gpio_2_IRQn
#define SDHC0_CLK_PORT GPIO_PRT2
#define SDHC0_CLK_PIN 5U
#define SDHC0_CLK_NUM 5U
#define SDHC0_CLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SDHC0_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
	#define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define SDHC0_CLK_HSIOM ioss_0_port_2_pin_5_HSIOM
#define SDHC0_CLK_IRQ ioss_interrupts_gpio_2_IRQn
#define ENABLE_WIFI_PORT GPIO_PRT2
#define ENABLE_WIFI_PIN 6U
#define ENABLE_WIFI_NUM 6U
#define ENABLE_WIFI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ENABLE_WIFI_INIT_DRIVESTATE 0
#ifndef ioss_0_port_2_pin_6_HSIOM
	#define ioss_0_port_2_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ENABLE_WIFI_HSIOM ioss_0_port_2_pin_6_HSIOM
#define ENABLE_WIFI_IRQ ioss_interrupts_gpio_2_IRQn
#define BT_UART_RX_PORT GPIO_PRT3
#define BT_UART_RX_PIN 0U
#define BT_UART_RX_NUM 0U
#define BT_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define BT_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_0_HSIOM
	#define ioss_0_port_3_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_RX_HSIOM ioss_0_port_3_pin_0_HSIOM
#define BT_UART_RX_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_TX_PORT GPIO_PRT3
#define BT_UART_TX_PIN 1U
#define BT_UART_TX_NUM 1U
#define BT_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define BT_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_1_HSIOM
	#define ioss_0_port_3_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_TX_HSIOM ioss_0_port_3_pin_1_HSIOM
#define BT_UART_TX_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_RTS_PORT GPIO_PRT3
#define BT_UART_RTS_PIN 2U
#define BT_UART_RTS_NUM 2U
#define BT_UART_RTS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define BT_UART_RTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_2_HSIOM
	#define ioss_0_port_3_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_RTS_HSIOM ioss_0_port_3_pin_2_HSIOM
#define BT_UART_RTS_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_UART_CTS_PORT GPIO_PRT3
#define BT_UART_CTS_PIN 3U
#define BT_UART_CTS_NUM 3U
#define BT_UART_CTS_DRIVEMODE CY_GPIO_DM_HIGHZ
#define BT_UART_CTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
	#define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_UART_CTS_HSIOM ioss_0_port_3_pin_3_HSIOM
#define BT_UART_CTS_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_POWER_PORT GPIO_PRT3
#define BT_POWER_PIN 4U
#define BT_POWER_NUM 4U
#define BT_POWER_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define BT_POWER_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_POWER_HSIOM ioss_0_port_3_pin_4_HSIOM
#define BT_POWER_IRQ ioss_interrupts_gpio_3_IRQn
#define BT_HOST_WAKE_PORT GPIO_PRT4
#define BT_HOST_WAKE_PIN 0U
#define BT_HOST_WAKE_NUM 0U
#define BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_ANALOG
#define BT_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_0_HSIOM
	#define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_HOST_WAKE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define BT_HOST_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#define BT_DEVICE_WAKE_PORT GPIO_PRT3
#define BT_DEVICE_WAKE_PIN 5U
#define BT_DEVICE_WAKE_NUM 5U
#define BT_DEVICE_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define BT_DEVICE_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_5_HSIOM
	#define ioss_0_port_3_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define BT_DEVICE_WAKE_HSIOM ioss_0_port_3_pin_5_HSIOM
#define BT_DEVICE_WAKE_IRQ ioss_interrupts_gpio_3_IRQn
#define EZI2C_SCL_PORT GPIO_PRT6
#define EZI2C_SCL_PIN 0U
#define EZI2C_SCL_NUM 0U
#define EZI2C_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define EZI2C_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_0_HSIOM
	#define ioss_0_port_6_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define EZI2C_SCL_HSIOM ioss_0_port_6_pin_0_HSIOM
#define EZI2C_SCL_IRQ ioss_interrupts_gpio_6_IRQn
#define EZI2C_SDA_PORT GPIO_PRT6
#define EZI2C_SDA_PIN 1U
#define EZI2C_SDA_NUM 1U
#define EZI2C_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define EZI2C_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
	#define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define EZI2C_SDA_HSIOM ioss_0_port_6_pin_1_HSIOM
#define EZI2C_SDA_IRQ ioss_interrupts_gpio_6_IRQn
#define SWO_PORT GPIO_PRT6
#define SWO_PIN 4U
#define SWO_NUM 4U
#define SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define SWO_IRQ ioss_interrupts_gpio_6_IRQn
#define SWDIO_PORT GPIO_PRT6
#define SWDIO_PIN 6U
#define SWDIO_NUM 6U
#define SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#define SWDCK_PORT GPIO_PRT6
#define SWDCK_PIN 7U
#define SWDCK_NUM 7U
#define SWDCK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define SWDCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define SWDCK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define SWDCK_IRQ ioss_interrupts_gpio_6_IRQn
#define ioss_0_port_7_pin_0_PORT GPIO_PRT7
#define ioss_0_port_7_pin_0_PIN 0U
#define ioss_0_port_7_pin_0_NUM 0U
#define ioss_0_port_7_pin_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_7_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_0_HSIOM
	#define ioss_0_port_7_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_7_pin_0_IRQ ioss_interrupts_gpio_7_IRQn
#define CINA_PORT GPIO_PRT7
#define CINA_PIN 1U
#define CINA_NUM 1U
#define CINA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CINA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CINA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CINA_IRQ ioss_interrupts_gpio_7_IRQn
#define CINB_PORT GPIO_PRT7
#define CINB_PIN 2U
#define CINB_NUM 2U
#define CINB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CINB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CINB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CINB_IRQ ioss_interrupts_gpio_7_IRQn
#define CMOD_PORT GPIO_PRT7
#define CMOD_PIN 7U
#define CMOD_NUM 7U
#define CMOD_DRIVEMODE CY_GPIO_DM_ANALOG
#define CMOD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_7_HSIOM
	#define ioss_0_port_7_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CMOD_HSIOM ioss_0_port_7_pin_7_HSIOM
#define CMOD_IRQ ioss_interrupts_gpio_7_IRQn
#define CSD_BTN0_PORT GPIO_PRT8
#define CSD_BTN0_PIN 1U
#define CSD_BTN0_NUM 1U
#define CSD_BTN0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_BTN0_HSIOM ioss_0_port_8_pin_1_HSIOM
#define CSD_BTN0_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_BTN1_PORT GPIO_PRT8
#define CSD_BTN1_PIN 2U
#define CSD_BTN1_NUM 2U
#define CSD_BTN1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_BTN1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_2_HSIOM
	#define ioss_0_port_8_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_BTN1_HSIOM ioss_0_port_8_pin_2_HSIOM
#define CSD_BTN1_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_SLD0_PORT GPIO_PRT8
#define CSD_SLD0_PIN 3U
#define CSD_SLD0_NUM 3U
#define CSD_SLD0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_SLD0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_SLD0_HSIOM ioss_0_port_8_pin_3_HSIOM
#define CSD_SLD0_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_SLD1_PORT GPIO_PRT8
#define CSD_SLD1_PIN 4U
#define CSD_SLD1_NUM 4U
#define CSD_SLD1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_SLD1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_4_HSIOM
	#define ioss_0_port_8_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_SLD1_HSIOM ioss_0_port_8_pin_4_HSIOM
#define CSD_SLD1_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_SLD2_PORT GPIO_PRT8
#define CSD_SLD2_PIN 5U
#define CSD_SLD2_NUM 5U
#define CSD_SLD2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_SLD2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_5_HSIOM
	#define ioss_0_port_8_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_SLD2_HSIOM ioss_0_port_8_pin_5_HSIOM
#define CSD_SLD2_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_SLD3_PORT GPIO_PRT8
#define CSD_SLD3_PIN 6U
#define CSD_SLD3_NUM 6U
#define CSD_SLD3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_SLD3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_6_HSIOM
	#define ioss_0_port_8_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_SLD3_HSIOM ioss_0_port_8_pin_6_HSIOM
#define CSD_SLD3_IRQ ioss_interrupts_gpio_8_IRQn
#define CSD_SLD4_PORT GPIO_PRT8
#define CSD_SLD4_PIN 7U
#define CSD_SLD4_NUM 7U
#define CSD_SLD4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CSD_SLD4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_7_HSIOM
	#define ioss_0_port_8_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CSD_SLD4_HSIOM ioss_0_port_8_pin_7_HSIOM
#define CSD_SLD4_IRQ ioss_interrupts_gpio_8_IRQn
#define ioss_0_port_9_pin_0_PORT GPIO_PRT9
#define ioss_0_port_9_pin_0_PIN 0U
#define ioss_0_port_9_pin_0_NUM 0U
#define ioss_0_port_9_pin_0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_9_pin_0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_9_pin_0_IRQ ioss_interrupts_gpio_9_IRQn
#define ioss_0_port_9_pin_1_PORT GPIO_PRT9
#define ioss_0_port_9_pin_1_PIN 1U
#define ioss_0_port_9_pin_1_NUM 1U
#define ioss_0_port_9_pin_1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_9_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_1_HSIOM
	#define ioss_0_port_9_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_9_pin_1_IRQ ioss_interrupts_gpio_9_IRQn
#define ioss_0_port_9_pin_2_PORT GPIO_PRT9
#define ioss_0_port_9_pin_2_PIN 2U
#define ioss_0_port_9_pin_2_NUM 2U
#define ioss_0_port_9_pin_2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_9_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_2_HSIOM
	#define ioss_0_port_9_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_9_pin_2_IRQ ioss_interrupts_gpio_9_IRQn
#define ioss_0_port_9_pin_3_PORT GPIO_PRT9
#define ioss_0_port_9_pin_3_PIN 3U
#define ioss_0_port_9_pin_3_NUM 3U
#define ioss_0_port_9_pin_3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_9_pin_3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_3_HSIOM
	#define ioss_0_port_9_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_9_pin_3_IRQ ioss_interrupts_gpio_9_IRQn

extern const cy_stc_gpio_pin_config_t WCO_IN_config;
extern const cy_stc_gpio_pin_config_t WCO_OUT_config;
extern const cy_stc_gpio_pin_config_t LED_RED_config;
extern const cy_stc_gpio_pin_config_t SW6_config;
extern const cy_stc_gpio_pin_config_t LED_BLUE_config;
extern const cy_stc_gpio_pin_config_t WL_UART_RX_config;
extern const cy_stc_gpio_pin_config_t WL_UART_TX_config;
extern const cy_stc_gpio_pin_config_t QSPI_SS0_config;
extern const cy_stc_gpio_pin_config_t QSPI_DATA3_config;
extern const cy_stc_gpio_pin_config_t QSPI_DATA2_config;
extern const cy_stc_gpio_pin_config_t QSPI_DATA1_config;
extern const cy_stc_gpio_pin_config_t QSPI_DATA0_config;
extern const cy_stc_gpio_pin_config_t QSPI_SPI_CLOCK_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_6_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_12_pin_7_config;
extern const cy_stc_gpio_pin_config_t UART_RX_config;
extern const cy_stc_gpio_pin_config_t UART_TX_config;
extern const cy_stc_gpio_pin_config_t LED9_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_14_pin_1_config;
extern const cy_stc_gpio_pin_config_t CSD_TX_config;
extern const cy_stc_gpio_pin_config_t LED_GREEN_config;
extern const cy_stc_gpio_pin_config_t LED8_config;
extern const cy_stc_gpio_pin_config_t SDHC0_DAT0_config;
extern const cy_stc_gpio_pin_config_t SDHC0_DAT1_config;
extern const cy_stc_gpio_pin_config_t SDHC0_DAT2_config;
extern const cy_stc_gpio_pin_config_t SDHC0_DAT3_config;
extern const cy_stc_gpio_pin_config_t SDHC0_CMD_config;
extern const cy_stc_gpio_pin_config_t SDHC0_CLK_config;
extern const cy_stc_gpio_pin_config_t ENABLE_WIFI_config;
extern const cy_stc_gpio_pin_config_t BT_UART_RX_config;
extern const cy_stc_gpio_pin_config_t BT_UART_TX_config;
extern const cy_stc_gpio_pin_config_t BT_UART_RTS_config;
extern const cy_stc_gpio_pin_config_t BT_UART_CTS_config;
extern const cy_stc_gpio_pin_config_t BT_POWER_config;
extern const cy_stc_gpio_pin_config_t BT_HOST_WAKE_config;
extern const cy_stc_gpio_pin_config_t BT_DEVICE_WAKE_config;
extern const cy_stc_gpio_pin_config_t EZI2C_SCL_config;
extern const cy_stc_gpio_pin_config_t EZI2C_SDA_config;
extern const cy_stc_gpio_pin_config_t SWO_config;
extern const cy_stc_gpio_pin_config_t SWDIO_config;
extern const cy_stc_gpio_pin_config_t SWDCK_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_7_pin_0_config;
extern const cy_stc_gpio_pin_config_t CINA_config;
extern const cy_stc_gpio_pin_config_t CINB_config;
extern const cy_stc_gpio_pin_config_t CMOD_config;
extern const cy_stc_gpio_pin_config_t CSD_BTN0_config;
extern const cy_stc_gpio_pin_config_t CSD_BTN1_config;
extern const cy_stc_gpio_pin_config_t CSD_SLD0_config;
extern const cy_stc_gpio_pin_config_t CSD_SLD1_config;
extern const cy_stc_gpio_pin_config_t CSD_SLD2_config;
extern const cy_stc_gpio_pin_config_t CSD_SLD3_config;
extern const cy_stc_gpio_pin_config_t CSD_SLD4_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_9_pin_0_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_9_pin_1_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_9_pin_2_config;
extern const cy_stc_gpio_pin_config_t ioss_0_port_9_pin_3_config;

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
