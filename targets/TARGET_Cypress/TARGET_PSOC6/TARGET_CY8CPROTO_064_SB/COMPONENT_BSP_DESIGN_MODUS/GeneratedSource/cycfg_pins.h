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
#define CYBSP_I2C_SCL_ENABLED 1U
#define CYBSP_I2C_SCL_PORT GPIO_PRT6
#define CYBSP_I2C_SCL_PORT_NUM 6U
#define CYBSP_I2C_SCL_PIN 0U
#define CYBSP_I2C_SCL_NUM 0U
#define CYBSP_I2C_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_I2C_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_0_HSIOM
	#define ioss_0_port_6_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_I2C_SCL_HSIOM ioss_0_port_6_pin_0_HSIOM
#define CYBSP_I2C_SCL_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SCL_HAL_PORT_PIN P6_0
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SCL_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif //defined (CY_USING_HAL)
#define CYBSP_I2C_SDA_ENABLED 1U
#define CYBSP_I2C_SDA_PORT GPIO_PRT6
#define CYBSP_I2C_SDA_PORT_NUM 6U
#define CYBSP_I2C_SDA_PIN 1U
#define CYBSP_I2C_SDA_NUM 1U
#define CYBSP_I2C_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define CYBSP_I2C_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
	#define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_I2C_SDA_HSIOM ioss_0_port_6_pin_1_HSIOM
#define CYBSP_I2C_SDA_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SDA_HAL_PORT_PIN P6_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SDA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SDA_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define CYBSP_I2C_SDA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
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

extern const cy_stc_gpio_pin_config_t CYBSP_WCO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_WCO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_WCO_OUT_obj;
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
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_IN_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ECO_IN_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_ECO_OUT_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_ECO_OUT_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DP_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DP_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_USB_DM_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_USB_DM_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_I2C_SCL_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_I2C_SCL_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_I2C_SDA_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_I2C_SDA_obj;
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

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
