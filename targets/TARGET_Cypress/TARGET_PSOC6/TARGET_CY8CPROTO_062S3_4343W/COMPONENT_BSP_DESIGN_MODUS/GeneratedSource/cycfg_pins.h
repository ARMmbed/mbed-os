/*******************************************************************************
* File Name: cycfg_pins.h
*
* Description:
* Pin configuration
* This file was automatically generated and should not be modified.
* Device Configurator: 2.0.0.1483
* Device Support Library (../../../../output/libs/COMPONENT_PSOC6/psoc6pdl): 1.4.0.1889
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
#define SWDIO_ENABLED 1U
#define SWDIO_PORT GPIO_PRT6
#define SWDIO_PORT_NUM 6U
#define SWDIO_PIN 6U
#define SWDIO_NUM 6U
#define SWDIO_DRIVEMODE CY_GPIO_DM_PULLUP
#define SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_6_HSIOM
	#define ioss_0_port_6_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define SWDIO_HSIOM ioss_0_port_6_pin_6_HSIOM
#define SWDIO_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define SWDIO_HAL_PORT_PIN P6_6
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif //defined (CY_USING_HAL)
#define SWCLK_ENABLED 1U
#define SWCLK_PORT GPIO_PRT6
#define SWCLK_PORT_NUM 6U
#define SWCLK_PIN 7U
#define SWCLK_NUM 7U
#define SWCLK_DRIVEMODE CY_GPIO_DM_PULLDOWN
#define SWCLK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_7_HSIOM
	#define ioss_0_port_6_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define SWCLK_HSIOM ioss_0_port_6_pin_7_HSIOM
#define SWCLK_IRQ ioss_interrupts_gpio_6_IRQn
#if defined (CY_USING_HAL)
	#define SWCLK_HAL_PORT_PIN P6_7
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWCLK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWCLK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#endif //defined (CY_USING_HAL)
#if defined (CY_USING_HAL)
	#define SWCLK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLDOWN
#endif //defined (CY_USING_HAL)
#define CYBSP_CSD_SLD0_ENABLED 1U
#define CYBSP_CSD_SLD0_PORT GPIO_PRT7
#define CYBSP_CSD_SLD0_PORT_NUM 7U
#define CYBSP_CSD_SLD0_PIN 0U
#define CYBSP_CSD_SLD0_NUM 0U
#define CYBSP_CSD_SLD0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_0_HSIOM
	#define ioss_0_port_7_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD0_HSIOM ioss_0_port_7_pin_0_HSIOM
#define CYBSP_CSD_SLD0_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD0_HAL_PORT_PIN P7_0
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
#define CYBSP_CSD_SLD1_PORT GPIO_PRT7
#define CYBSP_CSD_SLD1_PORT_NUM 7U
#define CYBSP_CSD_SLD1_PIN 1U
#define CYBSP_CSD_SLD1_NUM 1U
#define CYBSP_CSD_SLD1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_1_HSIOM
	#define ioss_0_port_7_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD1_HSIOM ioss_0_port_7_pin_1_HSIOM
#define CYBSP_CSD_SLD1_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD1_HAL_PORT_PIN P7_1
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
#define CYBSP_CSD_SLD2_PORT GPIO_PRT7
#define CYBSP_CSD_SLD2_PORT_NUM 7U
#define CYBSP_CSD_SLD2_PIN 2U
#define CYBSP_CSD_SLD2_NUM 2U
#define CYBSP_CSD_SLD2_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_2_HSIOM
	#define ioss_0_port_7_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD2_HSIOM ioss_0_port_7_pin_2_HSIOM
#define CYBSP_CSD_SLD2_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD2_HAL_PORT_PIN P7_2
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
#define CYBSP_CSD_SLD3_PORT GPIO_PRT7
#define CYBSP_CSD_SLD3_PORT_NUM 7U
#define CYBSP_CSD_SLD3_PIN 3U
#define CYBSP_CSD_SLD3_NUM 3U
#define CYBSP_CSD_SLD3_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_7_pin_3_HSIOM
	#define ioss_0_port_7_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD3_HSIOM ioss_0_port_7_pin_3_HSIOM
#define CYBSP_CSD_SLD3_IRQ ioss_interrupts_gpio_7_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD3_HAL_PORT_PIN P7_3
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
#define CYBSP_CSD_BTN0_PIN 0U
#define CYBSP_CSD_BTN0_NUM 0U
#define CYBSP_CSD_BTN0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_0_HSIOM
	#define ioss_0_port_8_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN0_HSIOM ioss_0_port_8_pin_0_HSIOM
#define CYBSP_CSD_BTN0_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN0_HAL_PORT_PIN P8_0
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
#define CYBSP_CSD_BTN1_PIN 1U
#define CYBSP_CSD_BTN1_NUM 1U
#define CYBSP_CSD_BTN1_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_BTN1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
	#define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_BTN1_HSIOM ioss_0_port_8_pin_1_HSIOM
#define CYBSP_CSD_BTN1_IRQ ioss_interrupts_gpio_8_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_BTN1_HAL_PORT_PIN P8_1
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
#define CYBSP_CSD_SLD4_ENABLED 1U
#define CYBSP_CSD_SLD4_PORT GPIO_PRT9
#define CYBSP_CSD_SLD4_PORT_NUM 9U
#define CYBSP_CSD_SLD4_PIN 0U
#define CYBSP_CSD_SLD4_NUM 0U
#define CYBSP_CSD_SLD4_DRIVEMODE CY_GPIO_DM_ANALOG
#define CYBSP_CSD_SLD4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_9_pin_0_HSIOM
	#define ioss_0_port_9_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_CSD_SLD4_HSIOM ioss_0_port_9_pin_0_HSIOM
#define CYBSP_CSD_SLD4_IRQ ioss_interrupts_gpio_9_IRQn
#if defined (CY_USING_HAL)
	#define CYBSP_CSD_SLD4_HAL_PORT_PIN P9_0
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
extern const cy_stc_gpio_pin_config_t SWDIO_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t SWDIO_obj;
#endif //defined (CY_USING_HAL)
extern const cy_stc_gpio_pin_config_t SWCLK_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t SWCLK_obj;
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
extern const cy_stc_gpio_pin_config_t CYBSP_CSD_SLD4_config;
#if defined (CY_USING_HAL)
	extern const cyhal_resource_inst_t CYBSP_CSD_SLD4_obj;
#endif //defined (CY_USING_HAL)

void init_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PINS_H */
