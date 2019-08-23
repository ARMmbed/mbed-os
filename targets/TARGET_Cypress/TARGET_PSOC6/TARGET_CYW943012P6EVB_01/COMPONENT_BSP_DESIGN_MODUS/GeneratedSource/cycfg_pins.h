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
#if defined (CY_USING_HAL)
	#include "cyhal_hwmgr.h"
#endif //defined (CY_USING_HAL)
#include "cycfg_routing.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CYBSP_WCO_IN_ENABLED 1U
#define CYBSP_WCO_IN_PORT GPIO_PRT0
#define CYBSP_WCO_IN_PORT_NUM 0U
#define CYBSP_WCO_IN_PIN 0U
#define CYBSP_WCO_IN_NUM 0U
#define CYBSP_WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
	#define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_IN_HSIOM ioss_0_port_0_pin_0_HSIOM
#define CYBSP_WCO_IN_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_PORT_PIN P0_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_IN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_WCO_OUT_ENABLED 1U
#define CYBSP_WCO_OUT_PORT GPIO_PRT0
#define CYBSP_WCO_OUT_PORT_NUM 0U
#define CYBSP_WCO_OUT_PIN 1U
#define CYBSP_WCO_OUT_NUM 1U
#define CYBSP_WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WCO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
	#define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WCO_OUT_HSIOM ioss_0_port_0_pin_1_HSIOM
#define CYBSP_WCO_OUT_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_PORT_PIN P0_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WCO_OUT_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_ROW6_SPI_MOSI_ENABLED 1U
#define CYBSP_ROW6_SPI_MOSI_PORT GPIO_PRT10
#define CYBSP_ROW6_SPI_MOSI_PORT_NUM 10U
#define CYBSP_ROW6_SPI_MOSI_PIN 0U
#define CYBSP_ROW6_SPI_MOSI_NUM 0U
#define CYBSP_ROW6_SPI_MOSI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_ROW6_SPI_MOSI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_0_HSIOM
	#define ioss_0_port_10_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ROW6_SPI_MOSI_HSIOM ioss_0_port_10_pin_0_HSIOM
#define CYBSP_ROW6_SPI_MOSI_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_ROW6_SPI_MOSI_HAL_PORT_PIN P10_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW6_SPI_MOSI_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW6_SPI_MOSI_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW6_SPI_MOSI_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_COL8_SPI_MISO_ENABLED 1U
#define CYBSP_COL8_SPI_MISO_PORT GPIO_PRT10
#define CYBSP_COL8_SPI_MISO_PORT_NUM 10U
#define CYBSP_COL8_SPI_MISO_PIN 1U
#define CYBSP_COL8_SPI_MISO_NUM 1U
#define CYBSP_COL8_SPI_MISO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_COL8_SPI_MISO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_1_HSIOM
	#define ioss_0_port_10_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_COL8_SPI_MISO_HSIOM ioss_0_port_10_pin_1_HSIOM
#define CYBSP_COL8_SPI_MISO_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_COL8_SPI_MISO_HAL_PORT_PIN P10_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL8_SPI_MISO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL8_SPI_MISO_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL8_SPI_MISO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_ROW7_SPI_CLK_ENABLED 1U
#define CYBSP_ROW7_SPI_CLK_PORT GPIO_PRT10
#define CYBSP_ROW7_SPI_CLK_PORT_NUM 10U
#define CYBSP_ROW7_SPI_CLK_PIN 2U
#define CYBSP_ROW7_SPI_CLK_NUM 2U
#define CYBSP_ROW7_SPI_CLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_ROW7_SPI_CLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_2_HSIOM
	#define ioss_0_port_10_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ROW7_SPI_CLK_HSIOM ioss_0_port_10_pin_2_HSIOM
#define CYBSP_ROW7_SPI_CLK_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_ROW7_SPI_CLK_HAL_PORT_PIN P10_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW7_SPI_CLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW7_SPI_CLK_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ROW7_SPI_CLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_COL7_SPI_CS_ENABLED 1U
#define CYBSP_COL7_SPI_CS_PORT GPIO_PRT10
#define CYBSP_COL7_SPI_CS_PORT_NUM 10U
#define CYBSP_COL7_SPI_CS_PIN 3U
#define CYBSP_COL7_SPI_CS_NUM 3U
#define CYBSP_COL7_SPI_CS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_COL7_SPI_CS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_3_HSIOM
	#define ioss_0_port_10_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_COL7_SPI_CS_HSIOM ioss_0_port_10_pin_3_HSIOM
#define CYBSP_COL7_SPI_CS_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_COL7_SPI_CS_HAL_PORT_PIN P10_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL7_SPI_CS_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL7_SPI_CS_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_COL7_SPI_CS_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_BAT_MON_ENABLED 1U
#define CYBSP_BAT_MON_PORT GPIO_PRT10
#define CYBSP_BAT_MON_PORT_NUM 10U
#define CYBSP_BAT_MON_PIN 4U
#define CYBSP_BAT_MON_NUM 4U
#define CYBSP_BAT_MON_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BAT_MON_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_4_HSIOM
	#define ioss_0_port_10_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BAT_MON_HSIOM ioss_0_port_10_pin_4_HSIOM
#define CYBSP_BAT_MON_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BAT_MON_HAL_PORT_PIN P10_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BAT_MON_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BAT_MON_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BAT_MON_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_WAKE_ENABLED 1U
#define CYBSP_WL_WAKE_PORT GPIO_PRT10
#define CYBSP_WL_WAKE_PORT_NUM 10U
#define CYBSP_WL_WAKE_PIN 7U
#define CYBSP_WL_WAKE_NUM 7U
#define CYBSP_WL_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_WAKE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_7_HSIOM
	#define ioss_0_port_10_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_WAKE_HSIOM ioss_0_port_10_pin_7_HSIOM
#define CYBSP_WL_WAKE_IRQ ioss_interrupts_gpio_10_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_WAKE_HAL_PORT_PIN P10_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_WAKE_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_UART_RX_ENABLED 1U
#define CYBSP_WL_UART_RX_PORT GPIO_PRT11
#define CYBSP_WL_UART_RX_PORT_NUM 11U
#define CYBSP_WL_UART_RX_PIN 0U
#define CYBSP_WL_UART_RX_NUM 0U
#define CYBSP_WL_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WL_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_0_HSIOM
	#define ioss_0_port_11_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_UART_RX_HSIOM ioss_0_port_11_pin_0_HSIOM
#define CYBSP_WL_UART_RX_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_RX_HAL_PORT_PIN P11_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_UART_TX_ENABLED 1U
#define CYBSP_WL_UART_TX_PORT GPIO_PRT11
#define CYBSP_WL_UART_TX_PORT_NUM 11U
#define CYBSP_WL_UART_TX_PIN 1U
#define CYBSP_WL_UART_TX_NUM 1U
#define CYBSP_WL_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
	#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_UART_TX_HSIOM ioss_0_port_11_pin_1_HSIOM
#define CYBSP_WL_UART_TX_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_TX_HAL_PORT_PIN P11_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_TX_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_UART_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_SS_ENABLED 1U
#define CYBSP_QSPI_SS_PORT GPIO_PRT11
#define CYBSP_QSPI_SS_PORT_NUM 11U
#define CYBSP_QSPI_SS_PIN 2U
#define CYBSP_QSPI_SS_NUM 2U
#define CYBSP_QSPI_SS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SS_HSIOM ioss_0_port_11_pin_2_HSIOM
#define CYBSP_QSPI_SS_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS_HAL_PORT_PIN P11_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_D3_ENABLED 1U
#define CYBSP_QSPI_D3_PORT GPIO_PRT11
#define CYBSP_QSPI_D3_PORT_NUM 11U
#define CYBSP_QSPI_D3_PIN 3U
#define CYBSP_QSPI_D3_NUM 3U
#define CYBSP_QSPI_D3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define CYBSP_QSPI_D3_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D3_HAL_PORT_PIN P11_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D3_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_D2_ENABLED 1U
#define CYBSP_QSPI_D2_PORT GPIO_PRT11
#define CYBSP_QSPI_D2_PORT_NUM 11U
#define CYBSP_QSPI_D2_PIN 4U
#define CYBSP_QSPI_D2_NUM 4U
#define CYBSP_QSPI_D2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define CYBSP_QSPI_D2_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D2_HAL_PORT_PIN P11_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D2_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_D1_ENABLED 1U
#define CYBSP_QSPI_D1_PORT GPIO_PRT11
#define CYBSP_QSPI_D1_PORT_NUM 11U
#define CYBSP_QSPI_D1_PIN 5U
#define CYBSP_QSPI_D1_NUM 5U
#define CYBSP_QSPI_D1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define CYBSP_QSPI_D1_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D1_HAL_PORT_PIN P11_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D1_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_D0_ENABLED 1U
#define CYBSP_QSPI_D0_PORT GPIO_PRT11
#define CYBSP_QSPI_D0_PORT_NUM 11U
#define CYBSP_QSPI_D0_PIN 6U
#define CYBSP_QSPI_D0_NUM 6U
#define CYBSP_QSPI_D0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_D0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_D0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define CYBSP_QSPI_D0_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D0_HAL_PORT_PIN P11_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D0_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_D0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_SCK_ENABLED 1U
#define CYBSP_QSPI_SCK_PORT GPIO_PRT11
#define CYBSP_QSPI_SCK_PORT_NUM 11U
#define CYBSP_QSPI_SCK_PIN 7U
#define CYBSP_QSPI_SCK_NUM 7U
#define CYBSP_QSPI_SCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define CYBSP_QSPI_SCK_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SCK_HAL_PORT_PIN P11_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SCK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SCK_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SCK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_GPIO4_ENABLED 1U
#define CYBSP_BT_GPIO4_PORT GPIO_PRT12
#define CYBSP_BT_GPIO4_PORT_NUM 12U
#define CYBSP_BT_GPIO4_PIN 0U
#define CYBSP_BT_GPIO4_NUM 0U
#define CYBSP_BT_GPIO4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_0_HSIOM
	#define ioss_0_port_12_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO4_HSIOM ioss_0_port_12_pin_0_HSIOM
#define CYBSP_BT_GPIO4_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO4_HAL_PORT_PIN P12_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_GPIO5_ENABLED 1U
#define CYBSP_BT_GPIO5_PORT GPIO_PRT12
#define CYBSP_BT_GPIO5_PORT_NUM 12U
#define CYBSP_BT_GPIO5_PIN 1U
#define CYBSP_BT_GPIO5_NUM 1U
#define CYBSP_BT_GPIO5_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_1_HSIOM
	#define ioss_0_port_12_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO5_HSIOM ioss_0_port_12_pin_1_HSIOM
#define CYBSP_BT_GPIO5_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO5_HAL_PORT_PIN P12_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO5_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO5_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO5_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_GPIO2_ENABLED 1U
#define CYBSP_BT_GPIO2_PORT GPIO_PRT12
#define CYBSP_BT_GPIO2_PORT_NUM 12U
#define CYBSP_BT_GPIO2_PIN 2U
#define CYBSP_BT_GPIO2_NUM 2U
#define CYBSP_BT_GPIO2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_2_HSIOM
	#define ioss_0_port_12_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO2_HSIOM ioss_0_port_12_pin_2_HSIOM
#define CYBSP_BT_GPIO2_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO2_HAL_PORT_PIN P12_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO2_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_GPIO3_ENABLED 1U
#define CYBSP_BT_GPIO3_PORT GPIO_PRT12
#define CYBSP_BT_GPIO3_PORT_NUM 12U
#define CYBSP_BT_GPIO3_PIN 3U
#define CYBSP_BT_GPIO3_NUM 3U
#define CYBSP_BT_GPIO3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_GPIO3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_3_HSIOM
	#define ioss_0_port_12_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_GPIO3_HSIOM ioss_0_port_12_pin_3_HSIOM
#define CYBSP_BT_GPIO3_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO3_HAL_PORT_PIN P12_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO3_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_GPIO3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_ECO_IN_ENABLED 1U
#define CYBSP_ECO_IN_PORT GPIO_PRT12
#define CYBSP_ECO_IN_PORT_NUM 12U
#define CYBSP_ECO_IN_PIN 6U
#define CYBSP_ECO_IN_NUM 6U
#define CYBSP_ECO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_ECO_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_6_HSIOM
	#define ioss_0_port_12_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ECO_IN_HSIOM ioss_0_port_12_pin_6_HSIOM
#define CYBSP_ECO_IN_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_IN_HAL_PORT_PIN P12_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_IN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_IN_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_IN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_ECO_OUT_ENABLED 1U
#define CYBSP_ECO_OUT_PORT GPIO_PRT12
#define CYBSP_ECO_OUT_PORT_NUM 12U
#define CYBSP_ECO_OUT_PIN 7U
#define CYBSP_ECO_OUT_NUM 7U
#define CYBSP_ECO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_ECO_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_12_pin_7_HSIOM
	#define ioss_0_port_12_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_ECO_OUT_HSIOM ioss_0_port_12_pin_7_HSIOM
#define CYBSP_ECO_OUT_IRQ ioss_interrupts_gpio_12_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_OUT_HAL_PORT_PIN P12_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_OUT_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_OUT_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_ECO_OUT_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_USB_DEV_VBUS_DET_ENABLED 1U
#define CYBSP_USB_DEV_VBUS_DET_PORT GPIO_PRT13
#define CYBSP_USB_DEV_VBUS_DET_PORT_NUM 13U
#define CYBSP_USB_DEV_VBUS_DET_PIN 4U
#define CYBSP_USB_DEV_VBUS_DET_NUM 4U
#define CYBSP_USB_DEV_VBUS_DET_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_USB_DEV_VBUS_DET_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_4_HSIOM
	#define ioss_0_port_13_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DEV_VBUS_DET_HSIOM ioss_0_port_13_pin_4_HSIOM
#define CYBSP_USB_DEV_VBUS_DET_IRQ ioss_interrupts_gpio_13_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DEV_VBUS_DET_HAL_PORT_PIN P13_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DEV_VBUS_DET_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DEV_VBUS_DET_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DEV_VBUS_DET_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_USB_HOST_EN_ENABLED 1U
#define CYBSP_USB_HOST_EN_PORT GPIO_PRT13
#define CYBSP_USB_HOST_EN_PORT_NUM 13U
#define CYBSP_USB_HOST_EN_PIN 5U
#define CYBSP_USB_HOST_EN_NUM 5U
#define CYBSP_USB_HOST_EN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_USB_HOST_EN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_5_HSIOM
	#define ioss_0_port_13_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_HOST_EN_HSIOM ioss_0_port_13_pin_5_HSIOM
#define CYBSP_USB_HOST_EN_IRQ ioss_interrupts_gpio_13_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_USB_HOST_EN_HAL_PORT_PIN P13_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_HOST_EN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_HOST_EN_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_HOST_EN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_USB_INT_L_ENABLED 1U
#define CYBSP_USB_INT_L_PORT GPIO_PRT13
#define CYBSP_USB_INT_L_PORT_NUM 13U
#define CYBSP_USB_INT_L_PIN 7U
#define CYBSP_USB_INT_L_NUM 7U
#define CYBSP_USB_INT_L_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_USB_INT_L_INIT_DRIVESTATE 1
#ifndef ioss_0_port_13_pin_7_HSIOM
	#define ioss_0_port_13_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_INT_L_HSIOM ioss_0_port_13_pin_7_HSIOM
#define CYBSP_USB_INT_L_IRQ ioss_interrupts_gpio_13_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_USB_INT_L_HAL_PORT_PIN P13_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_INT_L_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_INT_L_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_INT_L_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_USB_DP_ENABLED 1U
#define CYBSP_USB_DP_PORT GPIO_PRT14
#define CYBSP_USB_DP_PORT_NUM 14U
#define CYBSP_USB_DP_PIN 0U
#define CYBSP_USB_DP_NUM 0U
#define CYBSP_USB_DP_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_USB_DP_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_0_HSIOM
	#define ioss_0_port_14_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DP_HSIOM ioss_0_port_14_pin_0_HSIOM
#define CYBSP_USB_DP_IRQ ioss_interrupts_gpio_14_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DP_HAL_PORT_PIN P14_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DP_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DP_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DP_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_USB_DM_ENABLED 1U
#define CYBSP_USB_DM_PORT GPIO_PRT14
#define CYBSP_USB_DM_PORT_NUM 14U
#define CYBSP_USB_DM_PIN 1U
#define CYBSP_USB_DM_NUM 1U
#define CYBSP_USB_DM_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_USB_DM_INIT_DRIVESTATE 1
#ifndef ioss_0_port_14_pin_1_HSIOM
	#define ioss_0_port_14_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_USB_DM_HSIOM ioss_0_port_14_pin_1_HSIOM
#define CYBSP_USB_DM_IRQ ioss_interrupts_gpio_14_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DM_HAL_PORT_PIN P14_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DM_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DM_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_USB_DM_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_TX_ENABLED 1U
#define CYBSP_CSD_TX_PORT GPIO_PRT1
#define CYBSP_CSD_TX_PORT_NUM 1U
#define CYBSP_CSD_TX_PIN 0U
#define CYBSP_CSD_TX_NUM 0U
#define CYBSP_CSD_TX_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_TX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define CYBSP_CSD_TX_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_TX_HAL_PORT_PIN P1_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_TX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_SECI_IN_ENABLED 1U
#define CYBSP_WL_SECI_IN_PORT GPIO_PRT1
#define CYBSP_WL_SECI_IN_PORT_NUM 1U
#define CYBSP_WL_SECI_IN_PIN 2U
#define CYBSP_WL_SECI_IN_NUM 2U
#define CYBSP_WL_SECI_IN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_SECI_IN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
	#define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_SECI_IN_HSIOM ioss_0_port_1_pin_2_HSIOM
#define CYBSP_WL_SECI_IN_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_IN_HAL_PORT_PIN P1_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_IN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_IN_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_IN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_FRAM_SYNC_ENABLED 1U
#define CYBSP_WL_FRAM_SYNC_PORT GPIO_PRT1
#define CYBSP_WL_FRAM_SYNC_PORT_NUM 1U
#define CYBSP_WL_FRAM_SYNC_PIN 3U
#define CYBSP_WL_FRAM_SYNC_NUM 3U
#define CYBSP_WL_FRAM_SYNC_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_FRAM_SYNC_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_3_HSIOM
	#define ioss_0_port_1_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_FRAM_SYNC_HSIOM ioss_0_port_1_pin_3_HSIOM
#define CYBSP_WL_FRAM_SYNC_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_FRAM_SYNC_HAL_PORT_PIN P1_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_FRAM_SYNC_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_FRAM_SYNC_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_FRAM_SYNC_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_PRIORITY_ENABLED 1U
#define CYBSP_WL_PRIORITY_PORT GPIO_PRT1
#define CYBSP_WL_PRIORITY_PORT_NUM 1U
#define CYBSP_WL_PRIORITY_PIN 4U
#define CYBSP_WL_PRIORITY_NUM 4U
#define CYBSP_WL_PRIORITY_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_WL_PRIORITY_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
	#define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_PRIORITY_HSIOM ioss_0_port_1_pin_4_HSIOM
#define CYBSP_WL_PRIORITY_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_PRIORITY_HAL_PORT_PIN P1_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_PRIORITY_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_PRIORITY_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_PRIORITY_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_WL_SECI_OUT_ENABLED 1U
#define CYBSP_WL_SECI_OUT_PORT GPIO_PRT1
#define CYBSP_WL_SECI_OUT_PORT_NUM 1U
#define CYBSP_WL_SECI_OUT_PIN 5U
#define CYBSP_WL_SECI_OUT_NUM 5U
#define CYBSP_WL_SECI_OUT_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WL_SECI_OUT_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
	#define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WL_SECI_OUT_HSIOM ioss_0_port_1_pin_5_HSIOM
#define CYBSP_WL_SECI_OUT_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_OUT_HAL_PORT_PIN P1_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_OUT_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_OUT_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WL_SECI_OUT_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_WIFI_HOST_WAKE_ENABLED 1U
#define CYBSP_WIFI_HOST_WAKE_PORT GPIO_PRT2
#define CYBSP_WIFI_HOST_WAKE_PORT_NUM 2U
#define CYBSP_WIFI_HOST_WAKE_PIN 7U
#define CYBSP_WIFI_HOST_WAKE_NUM 7U
#define CYBSP_WIFI_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_WIFI_HOST_WAKE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_7_HSIOM
	#define ioss_0_port_2_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_HOST_WAKE_HSIOM ioss_0_port_2_pin_7_HSIOM
#define CYBSP_WIFI_HOST_WAKE_IRQ ioss_interrupts_gpio_2_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_PORT_PIN P2_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_UART_RX_ENABLED 1U
#define CYBSP_BT_UART_RX_PORT GPIO_PRT3
#define CYBSP_BT_UART_RX_PORT_NUM 3U
#define CYBSP_BT_UART_RX_PIN 0U
#define CYBSP_BT_UART_RX_NUM 0U
#define CYBSP_BT_UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_0_HSIOM
	#define ioss_0_port_3_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_RX_HSIOM ioss_0_port_3_pin_0_HSIOM
#define CYBSP_BT_UART_RX_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RX_HAL_PORT_PIN P3_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_UART_TX_ENABLED 1U
#define CYBSP_BT_UART_TX_PORT GPIO_PRT3
#define CYBSP_BT_UART_TX_PORT_NUM 3U
#define CYBSP_BT_UART_TX_PIN 1U
#define CYBSP_BT_UART_TX_NUM 1U
#define CYBSP_BT_UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_1_HSIOM
	#define ioss_0_port_3_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_TX_HSIOM ioss_0_port_3_pin_1_HSIOM
#define CYBSP_BT_UART_TX_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_TX_HAL_PORT_PIN P3_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_TX_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_UART_RTS_ENABLED 1U
#define CYBSP_BT_UART_RTS_PORT GPIO_PRT3
#define CYBSP_BT_UART_RTS_PORT_NUM 3U
#define CYBSP_BT_UART_RTS_PIN 2U
#define CYBSP_BT_UART_RTS_NUM 2U
#define CYBSP_BT_UART_RTS_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_UART_RTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_2_HSIOM
	#define ioss_0_port_3_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_RTS_HSIOM ioss_0_port_3_pin_2_HSIOM
#define CYBSP_BT_UART_RTS_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RTS_HAL_PORT_PIN P3_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RTS_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RTS_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_RTS_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_UART_CTS_ENABLED 1U
#define CYBSP_BT_UART_CTS_PORT GPIO_PRT3
#define CYBSP_BT_UART_CTS_PORT_NUM 3U
#define CYBSP_BT_UART_CTS_PIN 3U
#define CYBSP_BT_UART_CTS_NUM 3U
#define CYBSP_BT_UART_CTS_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_UART_CTS_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
	#define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_UART_CTS_HSIOM ioss_0_port_3_pin_3_HSIOM
#define CYBSP_BT_UART_CTS_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_CTS_HAL_PORT_PIN P3_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_CTS_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_CTS_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_UART_CTS_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_REG_ON_ENABLED 1U
#define CYBSP_BT_REG_ON_PORT GPIO_PRT3
#define CYBSP_BT_REG_ON_PORT_NUM 3U
#define CYBSP_BT_REG_ON_PIN 4U
#define CYBSP_BT_REG_ON_NUM 4U
#define CYBSP_BT_REG_ON_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define CYBSP_BT_REG_ON_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_REG_ON_HSIOM ioss_0_port_3_pin_4_HSIOM
#define CYBSP_BT_REG_ON_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_REG_ON_HAL_PORT_PIN P3_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_REG_ON_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_REG_ON_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_REG_ON_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_HOST_WAKE_ENABLED 1U
#define CYBSP_BT_HOST_WAKE_PORT GPIO_PRT3
#define CYBSP_BT_HOST_WAKE_PORT_NUM 3U
#define CYBSP_BT_HOST_WAKE_PIN 5U
#define CYBSP_BT_HOST_WAKE_NUM 5U
#define CYBSP_BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CYBSP_BT_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_5_HSIOM
	#define ioss_0_port_3_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_HOST_WAKE_HSIOM ioss_0_port_3_pin_5_HSIOM
#define CYBSP_BT_HOST_WAKE_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_PORT_PIN P3_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_DEVICE_WAKE_ENABLED 1U
#define CYBSP_BT_DEVICE_WAKE_PORT GPIO_PRT4
#define CYBSP_BT_DEVICE_WAKE_PORT_NUM 4U
#define CYBSP_BT_DEVICE_WAKE_PIN 0U
#define CYBSP_BT_DEVICE_WAKE_NUM 0U
#define CYBSP_BT_DEVICE_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_DEVICE_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_0_HSIOM
	#define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_DEVICE_WAKE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define CYBSP_BT_DEVICE_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE_HAL_PORT_PIN P4_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_RST_ENABLED 1U
#define CYBSP_BT_RST_PORT GPIO_PRT4
#define CYBSP_BT_RST_PORT_NUM 4U
#define CYBSP_BT_RST_PIN 1U
#define CYBSP_BT_RST_NUM 1U
#define CYBSP_BT_RST_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_RST_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_1_HSIOM
	#define ioss_0_port_4_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_RST_HSIOM ioss_0_port_4_pin_1_HSIOM
#define CYBSP_BT_RST_IRQ ioss_interrupts_gpio_4_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_RST_HAL_PORT_PIN P4_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_RST_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_RST_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_RST_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_EZI2C_SCL_ENABLED 1U
#define CYBSP_EZI2C_SCL_PORT GPIO_PRT6
#define CYBSP_EZI2C_SCL_PORT_NUM 6U
#define CYBSP_EZI2C_SCL_PIN 0U
#define CYBSP_EZI2C_SCL_NUM 0U
#define CYBSP_EZI2C_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_EZI2C_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_0_HSIOM
	#define ioss_0_port_6_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_EZI2C_SCL_HSIOM ioss_0_port_6_pin_0_HSIOM
#define CYBSP_EZI2C_SCL_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SCL_HAL_PORT_PIN P6_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SCL_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif //defined (CY_USING_HAL)
#define CYBSP_EZI2C_SDA_ENABLED 1U
#define CYBSP_EZI2C_SDA_PORT GPIO_PRT6
#define CYBSP_EZI2C_SDA_PORT_NUM 6U
#define CYBSP_EZI2C_SDA_PIN 1U
#define CYBSP_EZI2C_SDA_NUM 1U
#define CYBSP_EZI2C_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_EZI2C_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
	#define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_EZI2C_SDA_HSIOM ioss_0_port_6_pin_1_HSIOM
#define CYBSP_EZI2C_SDA_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SDA_HAL_PORT_PIN P6_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SDA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SDA_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_EZI2C_SDA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif //defined (CY_USING_HAL)
#define CYBSP_SWO_ENABLED 1U
#define CYBSP_SWO_PORT GPIO_PRT6
#define CYBSP_SWO_PORT_NUM 6U
#define CYBSP_SWO_PIN 4U
#define CYBSP_SWO_NUM 4U
#define CYBSP_SWO_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_SWO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
	#define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWO_HSIOM ioss_0_port_6_pin_4_HSIOM
#define CYBSP_SWO_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_PORT_PIN P6_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT6
#define CYBSP_SWDIO_PORT_NUM 6U
#define CYBSP_SWDIO_PIN 6U
#define CYBSP_SWDIO_NUM 6U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_PORT_PIN P6_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif //defined (CY_USING_HAL)
#define CYBSP_SWCLK_ENABLED 1U
#define CYBSP_SWCLK_PORT GPIO_PRT6
#define CYBSP_SWCLK_PORT_NUM 6U
#define CYBSP_SWCLK_PIN 7U
#define CYBSP_SWCLK_NUM 7U
#define CYBSP_SWCLK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define CYBSP_SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWCLK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define CYBSP_SWCLK_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWCLK_HAL_PORT_PIN P6_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWCLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWCLK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWCLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLDOWN
#endif //defined (CY_USING_HAL)
#define CYBSP_TRACECLK_ENABLED 1U
#define CYBSP_TRACECLK_PORT GPIO_PRT7
#define CYBSP_TRACECLK_PORT_NUM 7U
#define CYBSP_TRACECLK_PIN 0U
#define CYBSP_TRACECLK_NUM 0U
#define CYBSP_TRACECLK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACECLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_0_HSIOM
	#define ioss_0_port_7_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACECLK_HSIOM ioss_0_port_7_pin_0_HSIOM
#define CYBSP_TRACECLK_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_TRACECLK_HAL_PORT_PIN P7_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACECLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACECLK_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACECLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_CINTA_ENABLED 1U
#define CYBSP_CINTA_PORT GPIO_PRT7
#define CYBSP_CINTA_PORT_NUM 7U
#define CYBSP_CINTA_PIN 1U
#define CYBSP_CINTA_NUM 1U
#define CYBSP_CINTA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINTA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINTA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CINTA_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINTA_HAL_PORT_PIN P7_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTA_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CINTB_ENABLED 1U
#define CYBSP_CINTB_PORT GPIO_PRT7
#define CYBSP_CINTB_PORT_NUM 7U
#define CYBSP_CINTB_PIN 2U
#define CYBSP_CINTB_NUM 2U
#define CYBSP_CINTB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINTB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINTB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CINTB_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINTB_HAL_PORT_PIN P7_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTB_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTB_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINTB_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CMOD_ENABLED 1U
#define CYBSP_CMOD_PORT GPIO_PRT7
#define CYBSP_CMOD_PORT_NUM 7U
#define CYBSP_CMOD_PIN 7U
#define CYBSP_CMOD_NUM 7U
#define CYBSP_CMOD_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CMOD_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_7_HSIOM
	#define ioss_0_port_7_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CMOD_HSIOM ioss_0_port_7_pin_7_HSIOM
#define CYBSP_CMOD_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_PORT_PIN P7_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CMOD_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_BTN0_ENABLED 1U
#define CYBSP_CSD_BTN0_PORT GPIO_PRT8
#define CYBSP_CSD_BTN0_PORT_NUM 8U
#define CYBSP_CSD_BTN0_PIN 1U
#define CYBSP_CSD_BTN0_NUM 1U
#define CYBSP_CSD_BTN0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN0_HSIOM ioss_0_port_8_pin_1_HSIOM
#define CYBSP_CSD_BTN0_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_PORT_PIN P8_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_BTN1_ENABLED 1U
#define CYBSP_CSD_BTN1_PORT GPIO_PRT8
#define CYBSP_CSD_BTN1_PORT_NUM 8U
#define CYBSP_CSD_BTN1_PIN 2U
#define CYBSP_CSD_BTN1_NUM 2U
#define CYBSP_CSD_BTN1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_2_HSIOM
	#define ioss_0_port_8_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN1_HSIOM ioss_0_port_8_pin_2_HSIOM
#define CYBSP_CSD_BTN1_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_PORT_PIN P8_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD0_ENABLED 1U
#define CYBSP_CSD_SLD0_PORT GPIO_PRT8
#define CYBSP_CSD_SLD0_PORT_NUM 8U
#define CYBSP_CSD_SLD0_PIN 3U
#define CYBSP_CSD_SLD0_NUM 3U
#define CYBSP_CSD_SLD0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_3_HSIOM
	#define ioss_0_port_8_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD0_HSIOM ioss_0_port_8_pin_3_HSIOM
#define CYBSP_CSD_SLD0_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_PORT_PIN P8_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD1_ENABLED 1U
#define CYBSP_CSD_SLD1_PORT GPIO_PRT8
#define CYBSP_CSD_SLD1_PORT_NUM 8U
#define CYBSP_CSD_SLD1_PIN 4U
#define CYBSP_CSD_SLD1_NUM 4U
#define CYBSP_CSD_SLD1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_4_HSIOM
	#define ioss_0_port_8_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD1_HSIOM ioss_0_port_8_pin_4_HSIOM
#define CYBSP_CSD_SLD1_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_PORT_PIN P8_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD2_ENABLED 1U
#define CYBSP_CSD_SLD2_PORT GPIO_PRT8
#define CYBSP_CSD_SLD2_PORT_NUM 8U
#define CYBSP_CSD_SLD2_PIN 5U
#define CYBSP_CSD_SLD2_NUM 5U
#define CYBSP_CSD_SLD2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_5_HSIOM
	#define ioss_0_port_8_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD2_HSIOM ioss_0_port_8_pin_5_HSIOM
#define CYBSP_CSD_SLD2_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_PORT_PIN P8_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD3_ENABLED 1U
#define CYBSP_CSD_SLD3_PORT GPIO_PRT8
#define CYBSP_CSD_SLD3_PORT_NUM 8U
#define CYBSP_CSD_SLD3_PIN 6U
#define CYBSP_CSD_SLD3_NUM 6U
#define CYBSP_CSD_SLD3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_6_HSIOM
	#define ioss_0_port_8_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD3_HSIOM ioss_0_port_8_pin_6_HSIOM
#define CYBSP_CSD_SLD3_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_PORT_PIN P8_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD4_ENABLED 1U
#define CYBSP_CSD_SLD4_PORT GPIO_PRT8
#define CYBSP_CSD_SLD4_PORT_NUM 8U
#define CYBSP_CSD_SLD4_PIN 7U
#define CYBSP_CSD_SLD4_NUM 7U
#define CYBSP_CSD_SLD4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_7_HSIOM
	#define ioss_0_port_8_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD4_HSIOM ioss_0_port_8_pin_7_HSIOM
#define CYBSP_CSD_SLD4_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_PORT_PIN P8_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_TRACEDATA3_ENABLED 1U
#define CYBSP_TRACEDATA3_PORT GPIO_PRT9
#define CYBSP_TRACEDATA3_PORT_NUM 9U
#define CYBSP_TRACEDATA3_PIN 0U
#define CYBSP_TRACEDATA3_NUM 0U
#define CYBSP_TRACEDATA3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA3_HSIOM ioss_0_port_9_pin_0_HSIOM
#define CYBSP_TRACEDATA3_IRQ ioss_interrupts_gpio_9_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA3_HAL_PORT_PIN P9_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA3_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_TRACEDATA2_ENABLED 1U
#define CYBSP_TRACEDATA2_PORT GPIO_PRT9
#define CYBSP_TRACEDATA2_PORT_NUM 9U
#define CYBSP_TRACEDATA2_PIN 1U
#define CYBSP_TRACEDATA2_NUM 1U
#define CYBSP_TRACEDATA2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_1_HSIOM
	#define ioss_0_port_9_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA2_HSIOM ioss_0_port_9_pin_1_HSIOM
#define CYBSP_TRACEDATA2_IRQ ioss_interrupts_gpio_9_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA2_HAL_PORT_PIN P9_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA2_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_TRACEDATA1_ENABLED 1U
#define CYBSP_TRACEDATA1_PORT GPIO_PRT9
#define CYBSP_TRACEDATA1_PORT_NUM 9U
#define CYBSP_TRACEDATA1_PIN 2U
#define CYBSP_TRACEDATA1_NUM 2U
#define CYBSP_TRACEDATA1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_2_HSIOM
	#define ioss_0_port_9_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA1_HSIOM ioss_0_port_9_pin_2_HSIOM
#define CYBSP_TRACEDATA1_IRQ ioss_interrupts_gpio_9_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA1_HAL_PORT_PIN P9_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_TRACEDATA0_ENABLED 1U
#define CYBSP_TRACEDATA0_PORT GPIO_PRT9
#define CYBSP_TRACEDATA0_PORT_NUM 9U
#define CYBSP_TRACEDATA0_PIN 3U
#define CYBSP_TRACEDATA0_NUM 3U
#define CYBSP_TRACEDATA0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_TRACEDATA0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_3_HSIOM
	#define ioss_0_port_9_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_TRACEDATA0_HSIOM ioss_0_port_9_pin_3_HSIOM
#define CYBSP_TRACEDATA0_IRQ ioss_interrupts_gpio_9_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA0_HAL_PORT_PIN P9_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA0_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_TRACEDATA0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_ROW6_SPI_MOSI_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ROW6_SPI_MOSI_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_COL8_SPI_MISO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_COL8_SPI_MISO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_ROW7_SPI_CLK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ROW7_SPI_CLK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_COL7_SPI_CS_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_COL7_SPI_CS_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BAT_MON_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BAT_MON_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_UART_RX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_UART_RX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_UART_TX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_UART_TX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_SS_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_D3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_D2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_D1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_D0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_D0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SCK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_SCK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO4_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_GPIO4_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO5_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_GPIO5_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_GPIO2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_GPIO3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_GPIO3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ECO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ECO_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DEV_VBUS_DET_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DEV_VBUS_DET_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_HOST_EN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_HOST_EN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_INT_L_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_INT_L_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DP_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DP_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DM_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DM_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_TX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_TX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_SECI_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_SECI_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_FRAM_SYNC_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_FRAM_SYNC_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_PRIORITY_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_PRIORITY_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WL_SECI_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WL_SECI_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WIFI_HOST_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_UART_RX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_TX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_UART_TX_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_RTS_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_UART_RTS_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_UART_CTS_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_UART_CTS_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_REG_ON_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_REG_ON_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_HOST_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_DEVICE_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_RST_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_RST_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SCL_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_EZI2C_SCL_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_EZI2C_SDA_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_EZI2C_SDA_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWCLK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWCLK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_TRACECLK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_TRACECLK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINTA_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINTA_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINTB_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINTB_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CMOD_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CMOD_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_BTN0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_BTN1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_BTN1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD4_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_TRACEDATA3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_TRACEDATA2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_TRACEDATA1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_TRACEDATA0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_TRACEDATA0_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
