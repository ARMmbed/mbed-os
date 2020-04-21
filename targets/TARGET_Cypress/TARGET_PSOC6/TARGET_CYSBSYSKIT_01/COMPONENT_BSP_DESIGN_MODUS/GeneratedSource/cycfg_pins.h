/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* Device Configurator: 2.0.0.1483
* Device Support Library (../../../psoc6pdl): 1.3.1.1499
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

#define CYBSP_SW1_ENABLED 1U
#define CYBSP_SW1_PORT GPIO_PRT0
#define CYBSP_SW1_PORT_NUM 0U
#define CYBSP_SW1_PIN 4U
#define CYBSP_SW1_NUM 4U
#define CYBSP_SW1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_SW1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_4_HSIOM
#define ioss_0_port_0_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SW1_HSIOM ioss_0_port_0_pin_4_HSIOM
#define CYBSP_SW1_IRQ ioss_interrupts_gpio_0_IRQn
#if defined (CY_USING_HAL)
#define CYBSP_SW1_HAL_PORT_PIN P0_4
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_SW1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_SW1_HAL_DIR CYHAL_GPIO_DIR_INPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_SW1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif //defined (CY_USING_HAL)
#define CYBSP_LED1_ENABLED 1U
#define CYBSP_LED1_PORT GPIO_PRT11
#define CYBSP_LED1_PORT_NUM 11U
#define CYBSP_LED1_PIN 1U
#define CYBSP_LED1_NUM 1U
#define CYBSP_LED1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
#define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED1_HSIOM ioss_0_port_11_pin_1_HSIOM
#define CYBSP_LED1_IRQ ioss_interrupts_gpio_11_IRQn
#if defined (CY_USING_HAL)
#define CYBSP_LED1_HAL_PORT_PIN P11_1
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_LED1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_LED1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
#define CYBSP_LED1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
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

extern const cy_stc_gpio_pin_config_t CYBSP_SW1_config;
#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SW1_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_LED1_config;
#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_LED1_obj;
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
extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;
#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config;
#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SWDCK_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
