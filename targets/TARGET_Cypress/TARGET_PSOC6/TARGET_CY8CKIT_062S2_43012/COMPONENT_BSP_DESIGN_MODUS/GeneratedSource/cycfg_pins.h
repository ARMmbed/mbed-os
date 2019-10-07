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
#define CYBSP_QSPI_SS0_ENABLED 1U
#define CYBSP_QSPI_SS0_PORT GPIO_PRT11
#define CYBSP_QSPI_SS0_PORT_NUM 11U
#define CYBSP_QSPI_SS0_PIN 2U
#define CYBSP_QSPI_SS0_NUM 2U
#define CYBSP_QSPI_SS0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SS0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
	#define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SS0_HSIOM ioss_0_port_11_pin_2_HSIOM
#define CYBSP_QSPI_SS0_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS0_HAL_PORT_PIN P11_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS0_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SS0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_DATA3_ENABLED 1U
#define CYBSP_QSPI_DATA3_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA3_PORT_NUM 11U
#define CYBSP_QSPI_DATA3_PIN 3U
#define CYBSP_QSPI_DATA3_NUM 3U
#define CYBSP_QSPI_DATA3_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
	#define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA3_HSIOM ioss_0_port_11_pin_3_HSIOM
#define CYBSP_QSPI_DATA3_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA3_HAL_PORT_PIN P11_3
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA3_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_DATA2_ENABLED 1U
#define CYBSP_QSPI_DATA2_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA2_PORT_NUM 11U
#define CYBSP_QSPI_DATA2_PIN 4U
#define CYBSP_QSPI_DATA2_NUM 4U
#define CYBSP_QSPI_DATA2_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
	#define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA2_HSIOM ioss_0_port_11_pin_4_HSIOM
#define CYBSP_QSPI_DATA2_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA2_HAL_PORT_PIN P11_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA2_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_DATA1_ENABLED 1U
#define CYBSP_QSPI_DATA1_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA1_PORT_NUM 11U
#define CYBSP_QSPI_DATA1_PIN 5U
#define CYBSP_QSPI_DATA1_NUM 5U
#define CYBSP_QSPI_DATA1_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
	#define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA1_HSIOM ioss_0_port_11_pin_5_HSIOM
#define CYBSP_QSPI_DATA1_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA1_HAL_PORT_PIN P11_5
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA1_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_DATA0_ENABLED 1U
#define CYBSP_QSPI_DATA0_PORT GPIO_PRT11
#define CYBSP_QSPI_DATA0_PORT_NUM 11U
#define CYBSP_QSPI_DATA0_PIN 6U
#define CYBSP_QSPI_DATA0_NUM 6U
#define CYBSP_QSPI_DATA0_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_QSPI_DATA0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_6_HSIOM
	#define ioss_0_port_11_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_DATA0_HSIOM ioss_0_port_11_pin_6_HSIOM
#define CYBSP_QSPI_DATA0_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA0_HAL_PORT_PIN P11_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA0_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_DATA0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif //defined (CY_USING_HAL)
#define CYBSP_QSPI_SPI_CLOCK_ENABLED 1U
#define CYBSP_QSPI_SPI_CLOCK_PORT GPIO_PRT11
#define CYBSP_QSPI_SPI_CLOCK_PORT_NUM 11U
#define CYBSP_QSPI_SPI_CLOCK_PIN 7U
#define CYBSP_QSPI_SPI_CLOCK_NUM 7U
#define CYBSP_QSPI_SPI_CLOCK_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_QSPI_SPI_CLOCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_7_HSIOM
	#define ioss_0_port_11_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_QSPI_SPI_CLOCK_HSIOM ioss_0_port_11_pin_7_HSIOM
#define CYBSP_QSPI_SPI_CLOCK_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SPI_CLOCK_HAL_PORT_PIN P11_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SPI_CLOCK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SPI_CLOCK_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_QSPI_SPI_CLOCK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
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
#define CYBSP_CSD_RX_ENABLED 1U
#define CYBSP_CSD_RX_PORT GPIO_PRT1
#define CYBSP_CSD_RX_PORT_NUM 1U
#define CYBSP_CSD_RX_PIN 0U
#define CYBSP_CSD_RX_NUM 0U
#define CYBSP_CSD_RX_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
	#define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_RX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define CYBSP_CSD_RX_IRQ ioss_interrupts_gpio_1_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_PORT_PIN P1_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
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
#define CYBSP_BT_POWER_ENABLED 1U
#define CYBSP_BT_POWER_PORT GPIO_PRT3
#define CYBSP_BT_POWER_PORT_NUM 3U
#define CYBSP_BT_POWER_PIN 4U
#define CYBSP_BT_POWER_NUM 4U
#define CYBSP_BT_POWER_DRIVEMODE CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF
#define CYBSP_BT_POWER_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
	#define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_POWER_HSIOM ioss_0_port_3_pin_4_HSIOM
#define CYBSP_BT_POWER_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_POWER_HAL_PORT_PIN P3_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_POWER_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_POWER_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_POWER_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESHIGH
#endif //defined (CY_USING_HAL)
#define CYBSP_BT_DEVICE_WAKE_ENABLED 1U
#define CYBSP_BT_DEVICE_WAKE_PORT GPIO_PRT3
#define CYBSP_BT_DEVICE_WAKE_PORT_NUM 3U
#define CYBSP_BT_DEVICE_WAKE_PIN 5U
#define CYBSP_BT_DEVICE_WAKE_NUM 5U
#define CYBSP_BT_DEVICE_WAKE_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BT_DEVICE_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_3_pin_5_HSIOM
	#define ioss_0_port_3_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_DEVICE_WAKE_HSIOM ioss_0_port_3_pin_5_HSIOM
#define CYBSP_BT_DEVICE_WAKE_IRQ ioss_interrupts_gpio_3_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_DEVICE_WAKE_HAL_PORT_PIN P3_5
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
#define CYBSP_BT_HOST_WAKE_ENABLED 1U
#define CYBSP_BT_HOST_WAKE_PORT GPIO_PRT4
#define CYBSP_BT_HOST_WAKE_PORT_NUM 4U
#define CYBSP_BT_HOST_WAKE_PIN 0U
#define CYBSP_BT_HOST_WAKE_NUM 0U
#define CYBSP_BT_HOST_WAKE_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_BT_HOST_WAKE_INIT_DRIVESTATE 0
#ifndef ioss_0_port_4_pin_0_HSIOM
	#define ioss_0_port_4_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BT_HOST_WAKE_HSIOM ioss_0_port_4_pin_0_HSIOM
#define CYBSP_BT_HOST_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_PORT_PIN P4_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_BT_HOST_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_WIFI_HOST_WAKE_ENABLED 1U
#define CYBSP_WIFI_HOST_WAKE_PORT GPIO_PRT4
#define CYBSP_WIFI_HOST_WAKE_PORT_NUM 4U
#define CYBSP_WIFI_HOST_WAKE_PIN 1U
#define CYBSP_WIFI_HOST_WAKE_NUM 1U
#define CYBSP_WIFI_HOST_WAKE_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_WIFI_HOST_WAKE_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_1_HSIOM
	#define ioss_0_port_4_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_WIFI_HOST_WAKE_HSIOM ioss_0_port_4_pin_1_HSIOM
#define CYBSP_WIFI_HOST_WAKE_IRQ ioss_interrupts_gpio_4_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_PORT_PIN P4_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_IRQ CYHAL_GPIO_IRQ_RISE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_WIFI_HOST_WAKE_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
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
#define CYBSP_SWDCK_ENABLED 1U
#define CYBSP_SWDCK_PORT GPIO_PRT6
#define CYBSP_SWDCK_PORT_NUM 6U
#define CYBSP_SWDCK_PIN 7U
#define CYBSP_SWDCK_NUM 7U
#define CYBSP_SWDCK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define CYBSP_SWDCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDCK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define CYBSP_SWDCK_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_PORT_PIN P6_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_SWDCK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLDOWN
#endif //defined (CY_USING_HAL)
#define CYBSP_CINA_ENABLED 1U
#define CYBSP_CINA_PORT GPIO_PRT7
#define CYBSP_CINA_PORT_NUM 7U
#define CYBSP_CINA_PIN 1U
#define CYBSP_CINA_NUM 1U
#define CYBSP_CINA_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINA_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CINA_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_PORT_PIN P7_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_CINB_ENABLED 1U
#define CYBSP_CINB_PORT GPIO_PRT7
#define CYBSP_CINB_PORT_NUM 7U
#define CYBSP_CINB_PIN 2U
#define CYBSP_CINB_NUM 2U
#define CYBSP_CINB_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CINB_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CINB_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CINB_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_PORT_PIN P7_2
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_CINB_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
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

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SS0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_SS0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA3_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_DATA3_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA2_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_DATA2_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA1_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_DATA1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_DATA0_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_DATA0_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_QSPI_SPI_CLOCK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_QSPI_SPI_CLOCK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DP_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DP_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DM_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DM_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_RX_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_RX_obj;
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
extern const cy_stc_gpio_pin_config_t CYBSP_BT_POWER_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_POWER_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_DEVICE_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_DEVICE_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_BT_HOST_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_BT_HOST_WAKE_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WIFI_HOST_WAKE_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WIFI_HOST_WAKE_obj;
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
extern const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_SWDCK_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINA_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINA_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_CINB_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CINB_obj;
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

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
